#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <utility>
#include <nlohmann/json.hpp>
#include "dllmain.h"
#include "Game.h"
#include "Sections.h"

namespace
{
	constexpr char kSupportedBio4Sha256[] = "19aed4af0ab06a748ff8744d45ac5580fcd6be6b6b7e944b1ab8822a00c8ee4a";
	constexpr char kExpectedAssetSha256[] = "66e43b6221bef6b99583e15ec29661b18e64c260afed28d1efc325baa5071769";
	constexpr uint16_t kBowItemId = 82;
	constexpr uint32_t kBowWeaponNo = 28;
	constexpr int kBowPrice = 1000;
	constexpr char kBowInventoryPath[] = "SS/cmn/ss_wep28.dat";

	using InventoryPathRoutine = void(__cdecl*)(char*, uint32_t);
	using MerchantInitRoutine = void(__cdecl*)();
	using StockDataAddRoutine = void(__cdecl*)(STOCK_INFO*, const STOCK_INFO*, uint32_t);
	using MerchantPriceRoutine = int(__thiscall*)(void*, uint32_t, int);

	InventoryPathRoutine InventoryPathOriginal = nullptr;
	MerchantInitRoutine MerchantInitOriginal = nullptr;
	StockDataAddRoutine StockDataAdd = nullptr;
	MerchantPriceRoutine MerchantPriceOriginal = nullptr;
	PRICE_INFO** MerchantPriceSlotA = nullptr;
	PRICE_INFO** MerchantPriceSlotB = nullptr;
	STOCK_INFO* MerchantStock = nullptr;
	std::array<PRICE_INFO, 256> ExtendedPriceTable{};
	bool MerchantLogged = false;
	bool MerchantTableErrorLogged = false;

	bool IsLeon()
	{
		return GlobalPtr() && GlobalPtr()->pl_type_4FC8 == PlayerCharacter::Leon;
	}

	bool LoadEnabledConfig()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"leon-bow.json";
		if (!std::filesystem::exists(configPath))
		{
			spd::log()->info("Leon Krauser bow disabled: {} was not found", configPath.string());
			return false;
		}

		try
		{
			std::ifstream configFile(configPath);
			if (!configFile)
			{
				spd::log()->error("Leon Krauser bow disabled: unable to open {}", configPath.string());
				return false;
			}

			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Leon Krauser bow disabled: unsupported schema or target hash");
				return false;
			}
			if (!config.value("enabled", false))
			{
				spd::log()->info("Leon Krauser bow disabled by configuration");
				return false;
			}
			if (config.value("item_id", -1) != kBowItemId ||
				config.value("weapon_no", -1) != kBowWeaponNo ||
				config.value("merchant_price", -1) != kBowPrice ||
				config.value("asset_sha256", std::string()) != kExpectedAssetSha256)
			{
				spd::log()->error("Leon Krauser bow disabled: configuration constants do not match the investigated candidate");
				return false;
			}
			return true;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Leon Krauser bow disabled: invalid leon-bow.json ({})", error.what());
			return false;
		}
	}

	bool ResolveThunk(uint8_t* callSite, uint8_t* expectedTarget, uintptr_t& thunkAddress)
	{
		if (!callSite || callSite[0] != 0xE8)
			return false;

		ReadCall(callSite, thunkAddress);
		if (!thunkAddress || *reinterpret_cast<uint8_t*>(thunkAddress) != 0xE9)
			return false;

		uintptr_t targetAddress = 0;
		ReadCall(thunkAddress, targetAddress);
		return targetAddress == reinterpret_cast<uintptr_t>(expectedTarget);
	}

	bool BuildExtendedPriceTable(const PRICE_INFO* source)
	{
		if (!source || source == ExtendedPriceTable.data())
			return false;

		size_t count = 0;
		for (; count + 2 < ExtendedPriceTable.size(); ++count)
		{
			if (source[count].item_id_0 == -1)
				break;
			if (source[count].item_id_0 == kBowItemId)
				return false;
			ExtendedPriceTable[count] = source[count];
		}
		if (count + 2 >= ExtendedPriceTable.size() || source[count].item_id_0 != -1)
			return false;

		ExtendedPriceTable[count] = { static_cast<int16_t>(kBowItemId), 100, 1 };
		ExtendedPriceTable[count + 1] = { -1, 0, 0 };
		return true;
	}

	void __cdecl InventoryPathHook(char* destination, uint32_t weaponNo)
	{
		if (destination && weaponNo == kBowWeaponNo && IsLeon())
		{
			std::memcpy(destination, kBowInventoryPath, sizeof(kBowInventoryPath));
			return;
		}
		InventoryPathOriginal(destination, weaponNo);
	}

	int __fastcall MerchantPriceHook(void* merchant, void*, uint32_t itemId, int quantity)
	{
		if (static_cast<uint16_t>(itemId) == kBowItemId && IsLeon() && quantity > 0 &&
			quantity <= std::numeric_limits<int>::max() / kBowPrice)
		{
			return quantity * kBowPrice;
		}
		return MerchantPriceOriginal(merchant, itemId, quantity);
	}

	void __cdecl MerchantInitHook()
	{
		MerchantInitOriginal();
		if (!IsLeon())
			return;

		const auto source = MerchantPriceSlotA ? *MerchantPriceSlotA : nullptr;
		if (!BuildExtendedPriceTable(source))
		{
			if (!MerchantTableErrorLogged)
			{
				spd::log()->error("Leon Krauser bow: Merchant price table extension refused");
				MerchantTableErrorLogged = true;
			}
			return;
		}

		*MerchantPriceSlotA = ExtendedPriceTable.data();
		*MerchantPriceSlotB = ExtendedPriceTable.data();
		const STOCK_INFO bowStock = { kBowItemId, 1, { 0, 0, 0, 0 } };
		StockDataAdd(MerchantStock, &bowStock, 2);
		if (!MerchantLogged)
		{
			spd::log()->info("Leon Krauser bow: Merchant stock enabled at 1000 ptas");
			MerchantLogged = true;
		}
	}
}

void re4t::init::LeonKrauserBow()
{
	if (!LoadEnabledConfig())
		return;
	if (GameVersion() != "1.1.0")
	{
		spd::log()->error("Leon Krauser bow disabled: unsupported game version {}", GameVersion());
		return;
	}

	auto leonWeaponPattern = hook::pattern("8D 3C DD ? ? ? ? 66 83 3F 00 0F 84 ? ? ? ? A0");
	auto krauserWeaponPattern = hook::pattern("BB 17 00 00 00 8D 3C DD ? ? ? ? E9 ? ? ? ?");
	auto inventoryTargetPattern = hook::pattern("55 8B EC 8B 0D ? ? ? ? 0F B6 81 C8 4F 00 00 83 F8 05 0F 87 ? ? ? ? FF 24 85 ? ? ? ?");
	auto inventoryCallerPattern = hook::pattern("0F B7 C1 66 89 0D ? ? ? ? 50 8D 4D BC 51 66 89 15 ? ? ? ? E8 ? ? ? ? 8B 15");
	auto stockAddPattern = hook::pattern("55 8B EC 83 EC 10 8B 45 0C 89 45 0C 8B 45 10 85 C0 75 18 8B 0D ? ? ? ?");
	auto priceTargetPattern = hook::pattern("55 8B EC 83 EC 08 8B D1 8B 4A 04 0F B7 01 56 BE FF FF 00 00 66 3B F0");
	auto priceCallerPattern = hook::pattern("8B 45 0C 53 56 57 8B 7D 08 50 57 8B F1 E8 ? ? ? ? 8B D8");
	auto merchantTargetPattern = hook::pattern("8B 0D ? ? ? ? F7 41 54 00 10 00 00 74 05 E9 ? ? ? ? B8 ? ? ? ? C7 05 ? ? ? ? ? ? ? ?");
	auto merchantCallerPattern = hook::pattern("A1 ? ? ? ? 81 48 54 00 08 00 00 E8 ? ? ? ? 68 00 00 00 80 53 53 8B CB 51");

	const std::array<std::pair<const char*, size_t>, 9> matchCounts = {{
		{ "Leon weapon table", leonWeaponPattern.size() },
		{ "Krauser weapon table", krauserWeaponPattern.size() },
		{ "inventory target", inventoryTargetPattern.size() },
		{ "inventory caller", inventoryCallerPattern.size() },
		{ "stockDataAdd", stockAddPattern.size() },
		{ "price target", priceTargetPattern.size() },
		{ "price caller", priceCallerPattern.size() },
		{ "Merchant target", merchantTargetPattern.size() },
		{ "Merchant caller", merchantCallerPattern.size() },
	}};
	for (const auto& match : matchCounts)
	{
		if (match.second != 1)
		{
			spd::log()->error("Leon Krauser bow disabled: {} signature matched {} locations", match.first, match.second);
			return;
		}
	}

	auto inventoryTarget = inventoryTargetPattern.get(0).get<uint8_t>();
	auto priceTarget = priceTargetPattern.get(0).get<uint8_t>();
	auto merchantTarget = merchantTargetPattern.get(0).get<uint8_t>();
	uintptr_t inventoryThunk = 0;
	uintptr_t priceThunk = 0;
	uintptr_t merchantThunk = 0;
	if (!ResolveThunk(inventoryCallerPattern.get(0).get<uint8_t>(0x16), inventoryTarget, inventoryThunk) ||
		!ResolveThunk(priceCallerPattern.get(0).get<uint8_t>(0x0D), priceTarget, priceThunk) ||
		!ResolveThunk(merchantCallerPattern.get(0).get<uint8_t>(0x0C), merchantTarget, merchantThunk))
	{
		spd::log()->error("Leon Krauser bow disabled: a verified caller/thunk did not resolve to its investigated target");
		return;
	}

	auto leonWeaponBase = *leonWeaponPattern.get(0).get<uint32_t>(3);
	auto krauserWeaponBase = *krauserWeaponPattern.get(0).get<uint32_t>(8);
	auto leonBowEntry = reinterpret_cast<uint8_t*>(leonWeaponBase + kBowWeaponNo * 8);
	auto krauserBowEntry = reinterpret_cast<uint8_t*>(krauserWeaponBase + kBowWeaponNo * 8);
	const std::array<uint8_t, 8> expectedLeonEntry = {};
	const std::array<uint8_t, 8> expectedKrauserEntry = { 0xC3, 0x00, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00 };
	if (std::memcmp(leonBowEntry, expectedLeonEntry.data(), expectedLeonEntry.size()) != 0 ||
		std::memcmp(krauserBowEntry, expectedKrauserEntry.data(), expectedKrauserEntry.size()) != 0)
	{
		spd::log()->error("Leon Krauser bow disabled: weapon table entries differ from the investigated build");
		return;
	}

	if (merchantTarget[0x14] != 0xB8 || merchantTarget[0x19] != 0xC7 || merchantTarget[0x1A] != 0x05 ||
		merchantTarget[0x23] != 0xC7 || merchantTarget[0x24] != 0x05 || merchantTarget[0x2D] != 0xA3 ||
		merchantTarget[0x32] != 0xA3)
	{
		spd::log()->error("Leon Krauser bow disabled: Merchant data setup opcodes differ from the investigated build");
		return;
	}

	MerchantStock = reinterpret_cast<STOCK_INFO*>(*reinterpret_cast<uint32_t*>(merchantTarget + 0x29));
	MerchantPriceSlotA = reinterpret_cast<PRICE_INFO**>(*reinterpret_cast<uint32_t*>(merchantTarget + 0x2E));
	MerchantPriceSlotB = reinterpret_cast<PRICE_INFO**>(*reinterpret_cast<uint32_t*>(merchantTarget + 0x33));
	if (!MerchantStock || !MerchantPriceSlotA || !MerchantPriceSlotB)
	{
		spd::log()->error("Leon Krauser bow disabled: Merchant data pointers are null");
		return;
	}

	InventoryPathOriginal = reinterpret_cast<InventoryPathRoutine>(inventoryTarget);
	MerchantPriceOriginal = reinterpret_cast<MerchantPriceRoutine>(priceTarget);
	MerchantInitOriginal = reinterpret_cast<MerchantInitRoutine>(merchantTarget);
	StockDataAdd = reinterpret_cast<StockDataAddRoutine>(stockAddPattern.get(0).get<uint8_t>());

	// Mutate only after every signature, thunk, opcode, and data row has passed.
	injector::WriteMemoryRaw(leonBowEntry, krauserBowEntry, expectedKrauserEntry.size(), true);
	injector::MakeJMP(inventoryThunk, InventoryPathHook, true);
	injector::MakeJMP(priceThunk, MerchantPriceHook, true);
	injector::MakeCALL(merchantCallerPattern.get(0).get<uint8_t>(0x0C), MerchantInitHook, true);

	spd::log()->info("Leon Krauser bow candidate installed: item_id=82, weapon_no=28, Merchant price=1000 ptas");
}
