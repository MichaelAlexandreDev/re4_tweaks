#include <cstdint>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>
#include <nlohmann/json.hpp>
#include "dllmain.h"
#include "Game.h"
#include "DecompHarness.h"

// Replaces original bio4.exe functions with their rewrites from decomp/
// (see docs/pc-decomp-roadmap.md, phase 4). Every function is off unless
// re4_tweaks/decomp.json enables both the harness and that function.

namespace bio4
{
	namespace
	{
		constexpr unsigned int kImageBase = 0x00400000u;
		constexpr unsigned int kMaxEntries = 64;
		volatile LONG DecompCalls[kMaxEntries] = {};
	}

	unsigned int decomp_va(unsigned int va)
	{
		static const unsigned int base = static_cast<unsigned int>(
			reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)));
		return base + (va - kImageBase);
	}

	void decomp_count(unsigned int index)
	{
		if (index < kMaxEntries && InterlockedIncrement(&DecompCalls[index]) == 1)
			spd::log()->info("Decomp: first call of {} observed", kDecompEntries[index].name);
	}
}

namespace
{
	constexpr char kSupportedBio4Sha256[] = "19aed4af0ab06a748ff8744d45ac5580fcd6be6b6b7e944b1ab8822a00c8ee4a";

	bool LoadConfig(std::set<std::string>& enabledFunctions)
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"decomp.json";
		if (!std::filesystem::exists(configPath))
		{
			spd::log()->info("Decomp harness disabled: {} was not found", configPath.string());
			return false;
		}

		try
		{
			std::ifstream configFile(configPath);
			if (!configFile)
			{
				spd::log()->error("Decomp harness disabled: unable to open {}", configPath.string());
				return false;
			}

			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Decomp harness disabled: unsupported schema or target hash");
				return false;
			}
			if (!config.value("enabled", false))
			{
				spd::log()->info("Decomp harness disabled by configuration");
				return false;
			}
			if (!config.contains("functions") || !config.at("functions").is_object())
			{
				spd::log()->error("Decomp harness disabled: \"functions\" must be an object");
				return false;
			}
			const auto& functions = config.at("functions");
			for (auto it = functions.begin(); it != functions.end(); ++it)
			{
				const std::string name = it.key();
				const auto& value = it.value();
				if (!value.is_boolean())
				{
					spd::log()->error("Decomp harness: {} must be true or false; ignored", name);
					continue;
				}
				if (value.get<bool>())
					enabledFunctions.insert(name);
			}
			return true;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Decomp harness disabled: invalid decomp.json ({})", error.what());
			return false;
		}
	}
}

void re4t::init::DecompHarness()
{
	std::set<std::string> enabledFunctions;
	if (!LoadConfig(enabledFunctions))
		return;

	if (GameVersion() != "1.1.0")
	{
		spd::log()->error("Decomp harness disabled: unsupported game version {}", GameVersion());
		return;
	}
	if (bio4::kDecompEntryCount > bio4::kMaxEntries)
	{
		spd::log()->error("Decomp harness disabled: {} entries exceed the limit of {}",
			bio4::kDecompEntryCount, bio4::kMaxEntries);
		return;
	}

	std::set<std::string> known;
	for (unsigned int i = 0; i < bio4::kDecompEntryCount; i++)
		known.insert(bio4::kDecompEntries[i].name);
	for (const auto& name : enabledFunctions)
	{
		if (!known.count(name))
			spd::log()->error("Decomp harness: unknown function {} in decomp.json; ignored", name);
	}

	unsigned int replaced = 0;
	for (unsigned int i = 0; i < bio4::kDecompEntryCount; i++)
	{
		const auto& entry = bio4::kDecompEntries[i];
		if (!enabledFunctions.count(entry.name))
			continue;

		auto pattern = hook::pattern(entry.signature);
		const uintptr_t expected = bio4::decomp_va(entry.va);
		if (pattern.size() != 1)
		{
			spd::log()->error("Decomp: {} refused, signature matched {} times (expected 1)",
				entry.name, pattern.size());
			continue;
		}
		auto* address = pattern.get(0).get<uint8_t>(0);
		if (reinterpret_cast<uintptr_t>(address) != expected)
		{
			spd::log()->error("Decomp: {} refused, signature found at {:#x} instead of {:#x}",
				entry.name, reinterpret_cast<uintptr_t>(address), expected);
			continue;
		}

		injector::MakeJMP(address, entry.replacement, true);
		replaced++;
		spd::log()->info("Decomp: {} replaced at {:#x} (VA {:#010x})", entry.name, expected, entry.va);
	}

	spd::log()->info("Decomp harness: {} of {} requested functions replaced",
		replaced, enabledFunctions.size());
}
