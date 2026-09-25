#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "dllmain.h"
#include "ConsoleWnd.h"
#include "EnemyProfiles.h"
#include "Game.h"
#include "Sections.h"
#include "Settings.h"
#include "input.hpp"

struct ADA_WEAPON_LEVEL
{
	ITEM_ID id_0;
	int8_t power_2;
	int8_t speed_3;
	int8_t reload_4;
	int8_t bullet_5;
};

struct FILE_MSG_TBL_mb
{
	uint8_t top_0;
	uint8_t color_1;
	uint8_t attr_2;
	uint8_t layout_3;
};
FILE_MSG_TBL_mb* file_msg_tbl_35 = nullptr;

typedef void(__cdecl* wepXX_routine)(cPlayer* a1);
wepXX_routine wep17_r3_ready00 = nullptr;
wepXX_routine wep17_r3_ready10 = nullptr;
wepXX_routine wep02_r3_ready10 = nullptr;

bool wep17_justFired = false;

namespace
{
	constexpr char kSupportedBio4Sha256[] = "19aed4af0ab06a748ff8744d45ac5580fcd6be6b6b7e944b1ab8822a00c8ee4a";
	constexpr size_t kWeaponCount = 49;
	constexpr size_t kFirepowerLevelCount = 7;

	struct WeaponFirepowerDefinition
	{
		const char* configName;
		EItemId itemId;
	};

	struct WeaponFirepowerOverride
	{
		const char* configName;
		EItemId itemId;
		std::array<float, kFirepowerLevelCount> levels;
	};

	struct EnemyHitObservationConfig
	{
		bool enabled = false;
		std::vector<uint8_t> entityIds;
	};

	struct EnemySpawnProfile
	{
		uint16_t roomId;
		uint8_t emListNumber;
		uint8_t emListIndex;
		uint8_t expectedEntityId;
		uint8_t expectedType;
		std::string label;
		std::string stage;
		float hpMultiplier;
	};

	struct EnemyListOverride
	{
		uint16_t roomId;
		uint8_t emListNumber;
		uint8_t emListIndex;
		uint8_t expectedEntityId;
		uint8_t expectedType;
		uint8_t replacementEntityId;
		uint8_t replacementType;
		uint8_t replacementSet;
		uint32_t replacementFlags;
		int16_t replacementHp;
		uint8_t replacementEmsetNo;
		int8_t replacementCharacter;
		int16_t replacementGuardRadius;
		uint16_t replacementMotionSpeed;
		uint16_t replacementScale;
		std::string label;
	};

	struct EnemyModulePreload
	{
		uint16_t roomId;
		std::vector<uint8_t> entityIds;
	};

	struct EnemyFamilyDispatch
	{
		uint16_t roomId;
		std::vector<uint8_t> entityIds;
	};

	using EnemyLifeDownRoutine = int(__cdecl*)(cEm*, int, int, uint32_t);
	using EnemyListLoadRoutine = void(__cdecl*)(uint32_t);
	using EnemySetEventRoutine = cEm* (__cdecl*)(EM_LIST*);
	using EmReadSearchRoutine = void* (__cdecl*)(uint32_t, void*, uint32_t);
	using EnemyFamilyConfigureRoutine = void(__cdecl*)(void*);
	EnemyLifeDownRoutine EnemyLifeDown = nullptr;
	EnemyListLoadRoutine EnemyListLoad = nullptr;
	EnemySetEventRoutine EnemySetEvent = nullptr;
	EmReadSearchRoutine EmReadSearch = nullptr;
	std::vector<uint8_t> EnemyHitObservationEntityIds;
	std::vector<EnemySpawnProfile> EnemySpawnProfiles;
	std::vector<EnemyListOverride> EnemyListOverrides;
	std::vector<EnemyModulePreload> EnemyModulePreloads;
	std::vector<EnemyFamilyDispatch> EnemyFamilyDispatches;
	std::array<EnemyFamilyConfigureRoutine, 0x100> EnemyFamilyConfigureRoutines{};
	std::array<EnemyFamilyConfigureRoutine, 0x100> ExpectedEnemyFamilyConfigureRoutines{};
	EnemyFamilyConfigureRoutine* EnemyFamilyConfigureSlot = nullptr;

	constexpr WeaponFirepowerDefinition kWeaponDefinitions[] = {
		{ "handgun", EItemId::Ruger },
		{ "punisher", EItemId::FN57 },
		{ "matilda", EItemId::VP70 },
		{ "red9", EItemId::Mauser },
		{ "blacktail", EItemId::XD9 },
		{ "shotgun", EItemId::Shotgun },
		{ "riot_gun", EItemId::Riot_Gun },
		{ "striker", EItemId::Striker },
	};

	std::optional<std::vector<WeaponFirepowerOverride>> LoadWeaponFirepowerOverrides()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"weapons.yaml";
		if (!std::filesystem::exists(configPath))
		{
			spd::log()->info("Weapon overrides disabled: {} was not found", configPath.string());
			return std::nullopt;
		}

		try
		{
			std::ifstream configFile(configPath);
			if (!configFile)
			{
				spd::log()->error("Weapon overrides disabled: unable to open {}", configPath.string());
				return std::nullopt;
			}

			nlohmann::json config;
			configFile >> config;

			if (config.value("schema_version", 0) != 1)
			{
				spd::log()->error("Weapon overrides disabled: unsupported schema_version");
				return std::nullopt;
			}

			if (config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Weapon overrides disabled: target_sha256 does not match this build");
				return std::nullopt;
			}

			const auto& weapons = config.at("weapons");
			std::vector<WeaponFirepowerOverride> overrides;
			for (const auto& definition : kWeaponDefinitions)
			{
				if (!weapons.contains(definition.configName))
					continue;

				const auto& weapon = weapons.at(definition.configName);
				if (!weapon.value("enabled", false))
					continue;
				if (weapon.value("item_id", -1) != int(definition.itemId))
				{
					spd::log()->error("Weapon overrides disabled: {} item_id must be {}", definition.configName, int(definition.itemId));
					return std::nullopt;
				}

				const auto& values = weapon.at("firepower_levels");
				if (!values.is_array() || values.size() != kFirepowerLevelCount)
				{
					spd::log()->error("Weapon overrides disabled: {} must provide exactly {} firepower levels", definition.configName, kFirepowerLevelCount);
					return std::nullopt;
				}

				WeaponFirepowerOverride override{ definition.configName, definition.itemId, {} };
				for (size_t index = 0; index < kFirepowerLevelCount; ++index)
				{
					override.levels[index] = values.at(index).get<float>();
					if (!std::isfinite(override.levels[index]) || override.levels[index] <= 0.0f || override.levels[index] > 999.0f)
					{
						spd::log()->error("Weapon overrides disabled: {} firepower level {} must be in (0, 999]", definition.configName, index + 1);
						return std::nullopt;
					}
				}
				overrides.push_back(override);
			}

			return overrides;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Weapon overrides disabled: invalid weapons.yaml ({})", error.what());
			return std::nullopt;
		}
	}

	void ApplyWeaponFirepowerOverrides()
	{
		const auto overrides = LoadWeaponFirepowerOverrides();
		if (!overrides || overrides->empty())
			return;

		if (GameVersion() != "1.1.0")
		{
			spd::log()->error("Weapon firepower overrides disabled: unsupported game version {}", GameVersion());
			return;
		}

		// These signatures have exactly one match on the target executable recorded
		// in docs/target-build.md. Validate both before mutating either code or data.
		auto tablePattern = hook::pattern("D9 04 8D ? ? ? ? D9 5D ? 75 ? 8B CE 83 E9");
		const auto tableMatchCount = tablePattern.size();
		if (tableMatchCount != 1)
		{
			spd::log()->error(
				"Weapon firepower overrides disabled: WeaponLevelTbl signature matched {} locations",
				tableMatchCount);
			return;
		}

		auto displayPattern = hook::pattern(
			"D9 04 8D ? ? ? ? D8 35 ? ? ? ? D9 5D ? D9 45 ? 8B E5 5D C3");
		const auto displayMatchCount = displayPattern.size();
		if (displayMatchCount != 1)
		{
			spd::log()->error(
				"Weapon firepower overrides disabled: Merchant display signature matched {} locations",
				displayMatchCount);
			return;
		}

		auto WeaponLevelTbl = *tablePattern.get(0).get<float(*)[49][7]>(3);
		std::vector<uint8_t> weaponRows;
		weaponRows.reserve(overrides->size());
		for (const auto& override : *overrides)
		{
			const auto weaponNo = bio4::WeaponId2WeaponNo(ITEM_ID(override.itemId));
			if (weaponNo >= kWeaponCount)
			{
				spd::log()->error("Weapon firepower overrides disabled: {} resolved invalid weapon number {}", override.configName, weaponNo);
				return;
			}
			weaponRows.push_back(weaponNo);
		}

		// getPowerRatio normally divides every displayed value by the Handgun's
		// level-one value. That value is 1.0 in the original table, so removing the
		// division preserves vanilla displays while allowing the overridden Handgun
		// row to be shown as its absolute firepower.
		injector::MakeNOP(displayPattern.get(0).get<uint8_t>(7), 6, true);
		for (size_t index = 0; index < overrides->size(); ++index)
		{
			const auto& override = overrides->at(index);
			std::copy(override.levels.begin(), override.levels.end(), std::begin((*WeaponLevelTbl)[weaponRows[index]]));
			spd::log()->info("{} firepower override enabled: item_id={}, weapon_no={}, levels=[{},{},{},{},{},{},{}]",
				override.configName, int(override.itemId), weaponRows[index],
				override.levels[0], override.levels[1], override.levels[2], override.levels[3],
				override.levels[4], override.levels[5], override.levels[6]);
		}
	}

	std::optional<EnemyHitObservationConfig> LoadEnemyHitObservationConfig()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"enemy-profiles.json";
		if (!std::filesystem::exists(configPath))
		{
			spd::log()->info("Enemy hit observation disabled: {} was not found", configPath.string());
			return std::nullopt;
		}

		try
		{
			std::ifstream configFile(configPath);
			if (!configFile)
			{
				spd::log()->error("Enemy hit observation disabled: unable to open {}", configPath.string());
				return std::nullopt;
			}

			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1)
			{
				spd::log()->error("Enemy hit observation disabled: unsupported schema_version");
				return std::nullopt;
			}
			if (config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Enemy hit observation disabled: target_sha256 does not match this build");
				return std::nullopt;
			}

			const auto& observation = config.at("observation");
			EnemyHitObservationConfig result;
			result.enabled = observation.value("enabled", false);
			if (!result.enabled)
				return result;

			const auto& entityIds = observation.at("entity_ids");
			if (!entityIds.is_array())
			{
				spd::log()->error("Enemy hit observation disabled: entity_ids must be an array");
				return std::nullopt;
			}
			for (const auto& value : entityIds)
			{
				const int entityId = value.get<int>();
				if (entityId < 0 || entityId > 0xFF)
				{
					spd::log()->error("Enemy hit observation disabled: entity id {} is outside [0, 255]", entityId);
					return std::nullopt;
				}
				result.entityIds.push_back(static_cast<uint8_t>(entityId));
			}
			return result;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Enemy hit observation disabled: invalid enemy-profiles.json ({})", error.what());
			return std::nullopt;
		}
	}

	std::optional<std::vector<EnemySpawnProfile>> LoadEnemySpawnProfiles()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"enemy-profiles.json";
		if (!std::filesystem::exists(configPath))
			return std::nullopt;

		try
		{
			std::ifstream configFile(configPath);
			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Enemy spawn profiles disabled: schema or target hash mismatch");
				return std::nullopt;
			}

			const auto& section = config.at("spawn_profiles");
			if (!section.value("enabled", false))
				return std::vector<EnemySpawnProfile>();

			const auto& entries = section.at("entries");
			if (!entries.is_array())
			{
				spd::log()->error("Enemy spawn profiles disabled: entries must be an array");
				return std::nullopt;
			}

			std::vector<EnemySpawnProfile> profiles;
			for (const auto& entry : entries)
			{
				const int roomId = entry.at("room_id").get<int>();
				const int emListNumber = entry.at("em_list_number").get<int>();
				const int emListIndex = entry.at("em_list_index").get<int>();
				const int entityId = entry.at("expected_entity_id").get<int>();
				const int entityType = entry.at("expected_type").get<int>();
				const float hpMultiplier = entry.at("hp_multiplier").get<float>();
				if (roomId < 0 || roomId > 0xFFFF || emListNumber < 0 || emListNumber > 0xFF ||
					emListIndex < 0 || emListIndex > 0xFE || entityId < 0 || entityId > 0xFF ||
					entityType < 0 || entityType > 0xFF || !std::isfinite(hpMultiplier) ||
					hpMultiplier <= 0.0f || hpMultiplier > 15.0f)
				{
					spd::log()->error("Enemy spawn profiles disabled: invalid values in one profile entry");
					return std::nullopt;
				}

				EnemySpawnProfile profile{
					static_cast<uint16_t>(roomId), static_cast<uint8_t>(emListNumber),
					static_cast<uint8_t>(emListIndex), static_cast<uint8_t>(entityId),
					static_cast<uint8_t>(entityType), entry.at("label").get<std::string>(),
					entry.at("stage").get<std::string>(), hpMultiplier
				};
				if (profile.label.empty() || profile.stage.empty())
				{
					spd::log()->error("Enemy spawn profiles disabled: label and stage must not be empty");
					return std::nullopt;
				}
				const auto duplicate = std::find_if(profiles.begin(), profiles.end(), [&](const EnemySpawnProfile& other) {
					return other.roomId == profile.roomId && other.emListNumber == profile.emListNumber &&
						other.emListIndex == profile.emListIndex;
				});
				if (duplicate != profiles.end())
				{
					spd::log()->error("Enemy spawn profiles disabled: duplicate key room=0x{:04X}, list={}, index={}",
						roomId, emListNumber, emListIndex);
					return std::nullopt;
				}
				profiles.push_back(std::move(profile));
			}
			return profiles;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Enemy spawn profiles disabled: invalid enemy-profiles.json ({})", error.what());
			return std::nullopt;
		}
	}

	std::optional<std::vector<EnemyListOverride>> LoadEnemyListOverrides()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"enemy-profiles.json";
		if (!std::filesystem::exists(configPath))
			return std::nullopt;

		try
		{
			std::ifstream configFile(configPath);
			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Enemy list overrides disabled: schema or target hash mismatch");
				return std::nullopt;
			}
			if (!config.contains("list_overrides"))
				return std::vector<EnemyListOverride>();

			const auto& section = config.at("list_overrides");
			if (!section.value("enabled", false))
				return std::vector<EnemyListOverride>();

			const auto& entries = section.at("entries");
			if (!entries.is_array())
			{
				spd::log()->error("Enemy list overrides disabled: entries must be an array");
				return std::nullopt;
			}

			std::vector<EnemyListOverride> overrides;
			for (const auto& entry : entries)
			{
				const int roomId = entry.at("room_id").get<int>();
				const int emListNumber = entry.at("em_list_number").get<int>();
				const int emListIndex = entry.at("em_list_index").get<int>();
				const int expectedEntityId = entry.at("expected_entity_id").get<int>();
				const int expectedType = entry.at("expected_type").get<int>();
				const int replacementEntityId = entry.at("replacement_entity_id").get<int>();
				const int replacementType = entry.at("replacement_type").get<int>();
				const int replacementSet = entry.at("replacement_set").get<int>();
				const uint64_t replacementFlags = entry.at("replacement_flags").get<uint64_t>();
				const int replacementHp = entry.at("replacement_hp").get<int>();
				const int replacementEmsetNo = entry.at("replacement_emset_no").get<int>();
				const int replacementCharacter = entry.at("replacement_character").get<int>();
				const int replacementGuardRadius = entry.at("replacement_guard_radius").get<int>();
				const int replacementMotionSpeed = entry.value("replacement_motion_speed", 0);
				const int replacementScale = entry.value("replacement_scale", 0);

				if (roomId < 0 || roomId > 0xFFFF || emListNumber < 0 || emListNumber > 0x12 ||
					emListIndex < 0 || emListIndex > 0xFF || expectedEntityId < 0 || expectedEntityId > 0xFF ||
					expectedType < 0 || expectedType > 0xFF || replacementEntityId < 0 || replacementEntityId > 0xFF ||
					replacementType < 0 || replacementType > 0xFF || replacementSet < 0 || replacementSet > 0xFF ||
					replacementFlags > std::numeric_limits<uint32_t>::max() || replacementHp <= 0 || replacementHp > std::numeric_limits<int16_t>::max() ||
					replacementEmsetNo < 0 || replacementEmsetNo > 0xFF || replacementCharacter < std::numeric_limits<int8_t>::min() ||
					replacementCharacter > std::numeric_limits<int8_t>::max() || replacementGuardRadius < std::numeric_limits<int16_t>::min() ||
					replacementGuardRadius > std::numeric_limits<int16_t>::max() || replacementMotionSpeed < 0 || replacementMotionSpeed > 0xFFFF ||
					replacementScale < 0 || replacementScale > 0xFFFF)
				{
					spd::log()->error("Enemy list overrides disabled: invalid values in one override entry");
					return std::nullopt;
				}

				EnemyListOverride override{
					static_cast<uint16_t>(roomId), static_cast<uint8_t>(emListNumber), static_cast<uint8_t>(emListIndex),
					static_cast<uint8_t>(expectedEntityId), static_cast<uint8_t>(expectedType),
					static_cast<uint8_t>(replacementEntityId), static_cast<uint8_t>(replacementType), static_cast<uint8_t>(replacementSet),
					static_cast<uint32_t>(replacementFlags), static_cast<int16_t>(replacementHp), static_cast<uint8_t>(replacementEmsetNo),
					static_cast<int8_t>(replacementCharacter), static_cast<int16_t>(replacementGuardRadius),
					static_cast<uint16_t>(replacementMotionSpeed), static_cast<uint16_t>(replacementScale),
					entry.at("label").get<std::string>()
				};
				if (override.label.empty())
				{
					spd::log()->error("Enemy list overrides disabled: label must not be empty");
					return std::nullopt;
				}
				const auto duplicate = std::find_if(overrides.begin(), overrides.end(), [&](const EnemyListOverride& other) {
					return other.roomId == override.roomId && other.emListNumber == override.emListNumber &&
						other.emListIndex == override.emListIndex;
				});
				if (duplicate != overrides.end())
				{
					spd::log()->error("Enemy list overrides disabled: duplicate key room=0x{:04X}, list={}, index={}",
						roomId, emListNumber, emListIndex);
					return std::nullopt;
				}
				overrides.push_back(std::move(override));
			}
			return overrides;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Enemy list overrides disabled: invalid enemy-profiles.json ({})", error.what());
			return std::nullopt;
		}
	}

	std::optional<std::vector<EnemyModulePreload>> LoadEnemyModulePreloads()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"enemy-profiles.json";
		if (!std::filesystem::exists(configPath))
			return std::nullopt;

		try
		{
			std::ifstream configFile(configPath);
			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Enemy module preloads disabled: schema or target hash mismatch");
				return std::nullopt;
			}
			if (!config.contains("module_preloads"))
				return std::vector<EnemyModulePreload>();

			const auto& section = config.at("module_preloads");
			if (!section.value("enabled", false))
				return std::vector<EnemyModulePreload>();
			const auto& entries = section.at("entries");
			if (!entries.is_array())
			{
				spd::log()->error("Enemy module preloads disabled: entries must be an array");
				return std::nullopt;
			}

			std::vector<EnemyModulePreload> preloads;
			for (const auto& entry : entries)
			{
				const int roomId = entry.at("room_id").get<int>();
				if (roomId != 0x0100 && roomId != 0x0101)
				{
					spd::log()->error("Enemy module preloads disabled: unsupported room 0x{:04X}", roomId);
					return std::nullopt;
				}
				if (std::any_of(preloads.begin(), preloads.end(), [&](const EnemyModulePreload& other) {
					return other.roomId == roomId;
				}))
				{
					spd::log()->error("Enemy module preloads disabled: duplicate room 0x{:04X}", roomId);
					return std::nullopt;
				}

				const auto& ids = entry.at("entity_ids");
				const size_t capacity = roomId == 0x0100 ? 3 : 2;
				if (!ids.is_array() || ids.empty() || ids.size() > capacity)
				{
					spd::log()->error("Enemy module preloads disabled: room 0x{:04X} needs between 1 and {} entity IDs", roomId, capacity);
					return std::nullopt;
				}

				EnemyModulePreload preload{ static_cast<uint16_t>(roomId), {} };
				for (const auto& value : ids)
				{
					const int entityId = value.get<int>();
					const bool originalFamily = (roomId == 0x0100 && entityId == 0x12) ||
						(roomId == 0x0101 && (entityId == 0x15 || entityId == 0x26));
					if (entityId <= 0 || entityId > 0xFF || originalFamily ||
						std::find(preload.entityIds.begin(), preload.entityIds.end(), entityId) != preload.entityIds.end())
					{
						spd::log()->error("Enemy module preloads disabled: invalid or duplicate id 0x{:02X} for room 0x{:04X}", entityId, roomId);
						return std::nullopt;
					}
					preload.entityIds.push_back(static_cast<uint8_t>(entityId));
				}
				preloads.push_back(std::move(preload));
			}
			return preloads;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Enemy module preloads disabled: invalid enemy-profiles.json ({})", error.what());
			return std::nullopt;
		}
	}

	std::optional<std::vector<EnemyFamilyDispatch>> LoadEnemyFamilyDispatches()
	{
		const auto configPath = std::filesystem::path(rootPath) / L"re4_tweaks" / L"enemy-profiles.json";
		if (!std::filesystem::exists(configPath))
			return std::vector<EnemyFamilyDispatch>();

		try
		{
			std::ifstream configFile(configPath);
			nlohmann::json config;
			configFile >> config;
			if (config.value("schema_version", 0) != 1 ||
				config.value("target_sha256", std::string()) != kSupportedBio4Sha256)
			{
				spd::log()->error("Enemy family dispatch disabled: schema or target hash mismatch");
				return std::nullopt;
			}
			if (!config.contains("family_dispatch"))
				return std::vector<EnemyFamilyDispatch>();

			const auto& section = config.at("family_dispatch");
			if (!section.value("enabled", false))
				return std::vector<EnemyFamilyDispatch>();
			const auto& entries = section.at("entries");
			if (!entries.is_array() || entries.empty())
			{
				spd::log()->error("Enemy family dispatch disabled: entries must be a non-empty array");
				return std::nullopt;
			}

			std::vector<EnemyFamilyDispatch> dispatches;
			for (const auto& entry : entries)
			{
				const int roomId = entry.at("room_id").get<int>();
				if (roomId != 0x0100 && roomId != 0x0101 ||
					std::any_of(dispatches.begin(), dispatches.end(), [&](const EnemyFamilyDispatch& other) {
						return other.roomId == roomId;
					}))
				{
					spd::log()->error("Enemy family dispatch disabled: unsupported or duplicate room 0x{:04X}", roomId);
					return std::nullopt;
				}

				const auto& ids = entry.at("entity_ids");
				if (!ids.is_array() || ids.size() != 2)
				{
					spd::log()->error("Enemy family dispatch disabled: room 0x{:04X} needs exactly two family IDs", roomId);
					return std::nullopt;
				}

				EnemyFamilyDispatch dispatch{ static_cast<uint16_t>(roomId), {} };
				for (const auto& value : ids)
				{
					const int entityId = value.get<int>();
					if ((entityId != 0x12 && entityId != 0x15 && entityId != 0x20) ||
						std::find(dispatch.entityIds.begin(), dispatch.entityIds.end(), entityId) != dispatch.entityIds.end())
					{
						spd::log()->error("Enemy family dispatch disabled: unsupported or duplicate id 0x{:02X}", entityId);
						return std::nullopt;
					}
					dispatch.entityIds.push_back(static_cast<uint8_t>(entityId));
				}
				const uint8_t originalId = roomId == 0x0100 ? 0x12 : 0x15;
				if (std::find(dispatch.entityIds.begin(), dispatch.entityIds.end(), originalId) == dispatch.entityIds.end() ||
					std::find(dispatch.entityIds.begin(), dispatch.entityIds.end(), 0x20) == dispatch.entityIds.end())
				{
					spd::log()->error("Enemy family dispatch disabled: room 0x{:04X} must contain its original family and em20", roomId);
					return std::nullopt;
				}
				dispatches.push_back(std::move(dispatch));
			}
			return dispatches;
		}
		catch (const std::exception& error)
		{
			spd::log()->error("Enemy family dispatch disabled: invalid enemy-profiles.json ({})", error.what());
			return std::nullopt;
		}
	}

	const EnemyFamilyDispatch* FindEnemyFamilyDispatch(uint16_t roomId)
	{
		const auto match = std::find_if(EnemyFamilyDispatches.begin(), EnemyFamilyDispatches.end(),
			[&](const EnemyFamilyDispatch& dispatch) { return dispatch.roomId == roomId; });
		return match == EnemyFamilyDispatches.end() ? nullptr : &*match;
	}

	void __cdecl EnemyFamilyConfigureDispatcher(void* enemy)
	{
		const uint8_t entityId = *(reinterpret_cast<const uint8_t*>(enemy) + 0x100);
		const EnemyFamilyConfigureRoutine routine = EnemyFamilyConfigureRoutines[entityId];
		if (routine != nullptr)
		{
			spd::log()->info("Enemy family dispatch: id=0x{:02X}, routine=0x{:08X}", entityId,
				reinterpret_cast<uintptr_t>(routine));
			routine(enemy);
			return;
		}

		spd::log()->error("Enemy family dispatch refused: no captured routine for id=0x{:02X}", entityId);
	}

	void CaptureEnemyFamilyConfigureRoutine(uint16_t roomId, uint8_t entityId)
	{
		const EnemyFamilyDispatch* dispatch = FindEnemyFamilyDispatch(roomId);
		if (dispatch == nullptr || EnemyFamilyConfigureSlot == nullptr)
			return;

		const EnemyFamilyConfigureRoutine observed = *EnemyFamilyConfigureSlot;
		if (observed != EnemyFamilyConfigureDispatcher)
		{
			const bool configuredFamily = std::find(dispatch->entityIds.begin(), dispatch->entityIds.end(), entityId) != dispatch->entityIds.end();
			const EnemyFamilyConfigureRoutine expected = ExpectedEnemyFamilyConfigureRoutines[entityId];
			if (configuredFamily && observed != expected)
			{
				spd::log()->error(
					"Enemy family dispatch refused: room=0x{:04X}, id=0x{:02X}, expected routine=0x{:08X}, observed=0x{:08X}",
					roomId, entityId, reinterpret_cast<uintptr_t>(expected), reinterpret_cast<uintptr_t>(observed));
				return;
			}
			EnemyFamilyConfigureRoutines[entityId] = observed;
			spd::log()->info("Enemy family dispatch captured: room=0x{:04X}, id=0x{:02X}, routine=0x{:08X}",
				roomId, entityId, reinterpret_cast<uintptr_t>(observed));
		}

		const bool allCaptured = std::all_of(dispatch->entityIds.begin(), dispatch->entityIds.end(),
			[](const uint8_t id) { return EnemyFamilyConfigureRoutines[id] != nullptr; });
		if (allCaptured)
		{
			*EnemyFamilyConfigureSlot = EnemyFamilyConfigureDispatcher;
			spd::log()->info("Enemy family dispatch installed for room=0x{:04X}", roomId);
		}
	}

	void ApplyActiveEnemyOverride(EM_LIST* source)
	{
		if (source == nullptr)
			return;

		const uint16_t roomId = static_cast<uint16_t>(GlobalPtr()->curRoomId_4FAC);
		const uint8_t sourceId = static_cast<uint8_t>(source->id_1);
		const uint8_t sourceType = static_cast<uint8_t>(source->type_2);
		const auto match = std::find_if(EnemyListOverrides.begin(), EnemyListOverrides.end(),
			[&](const EnemyListOverride& entry) {
				return entry.roomId == roomId && entry.expectedEntityId == sourceId && entry.expectedType == sourceType;
			});
		if (match == EnemyListOverrides.end())
			return;

		source->id_1 = static_cast<char>(match->replacementEntityId);
		source->type_2 = static_cast<char>(match->replacementType);
		spd::log()->info(
			"Active enemy override applied: room=0x{:04X}, source={} id/type 0x{:02X}/0x{:02X} -> 0x{:02X}/0x{:02X}",
			roomId, static_cast<const void*>(source), sourceId, sourceType,
			match->replacementEntityId, match->replacementType);
	}

	cEm* __cdecl EnemySetEventHook(EM_LIST* source)
	{
		ApplyActiveEnemyOverride(source);
		cEm* const entity = EnemySetEvent(source);
		if (entity != nullptr)
		{
			spd::log()->info("Active enemy spawn result: source={} id/type 0x{:02X}/0x{:02X}, entity id/type 0x{:02X}/0x{:02X}",
				static_cast<const void*>(source), static_cast<uint8_t>(source->id_1), static_cast<uint8_t>(source->type_2),
				entity->id_100, entity->type_101);
		}
		return entity;
	}

	void InitializeActiveEnemyOverrides()
	{
		if (EnemyListOverrides.empty())
			return;
		if (re4t::cfg->bEnableModExpansion)
		{
			spd::log()->error("Active enemy overrides disabled: ModExpansion already owns EmSetEvent");
			return;
		}

		auto callPattern = hook::pattern("52 66 89 45 E4 66 89 4D F6 E8");
		if (callPattern.size() != 1)
		{
			spd::log()->error("Active enemy overrides disabled: EmSetEvent caller signature matched {} locations", callPattern.size());
			return;
		}
		const uintptr_t thunkAddress = injector::GetBranchDestination(callPattern.get(0).get<uint32_t>(9)).as_int();
		ReadCall(thunkAddress, EnemySetEvent);
		if (EnemySetEvent == nullptr)
		{
			spd::log()->error("Active enemy overrides disabled: failed to resolve EmSetEvent");
			return;
		}
		InjectHook(thunkAddress, EnemySetEventHook, HookType::Jump);
		spd::log()->info("Active enemy overrides installed: EmSetEvent thunk=0x{:08X}, entries={}",
			thunkAddress, EnemyListOverrides.size());
	}

	void PreloadEnemyModules(uint16_t roomId)
	{
		const auto match = std::find_if(EnemyModulePreloads.begin(), EnemyModulePreloads.end(),
			[&](const EnemyModulePreload& preload) { return preload.roomId == roomId; });
		if (match == EnemyModulePreloads.end())
			return;

		for (const uint8_t entityId : match->entityIds)
		{
			void* result = EmReadSearch(entityId, nullptr, 0);
			if (result == nullptr)
				spd::log()->error("Enemy module preload failed: room=0x{:04X}, id=0x{:02X}", roomId, entityId);
			else
			{
				CaptureEnemyFamilyConfigureRoutine(roomId, entityId);
				spd::log()->info("Enemy module preload complete: room=0x{:04X}, id=0x{:02X}, data={}", roomId, entityId, result);
			}
		}
	}

	void* __cdecl EnemyModuleReadSearchHook(uint32_t entityId, void* destination, uint32_t size)
	{
		void* result = EmReadSearch(entityId, destination, size);
		const uint16_t roomId = static_cast<uint16_t>(GlobalPtr()->curRoomId_4FAC);
		spd::log()->info(
			"Enemy module request observed: room=0x{:04X}, id=0x{:02X}, result={}",
			roomId, entityId, result);
		if (result != nullptr)
		{
			CaptureEnemyFamilyConfigureRoutine(roomId, static_cast<uint8_t>(entityId));
			PreloadEnemyModules(roomId);
		}
		return result;
	}

	void InitializeEnemyModulePreloads()
	{
		const auto preloads = LoadEnemyModulePreloads();
		const auto dispatches = LoadEnemyFamilyDispatches();
		if (!preloads || !dispatches)
			return;
		if (preloads->empty())
		{
			if (!dispatches->empty())
				spd::log()->error("Enemy family dispatch disabled: module preloads must be enabled");
			spd::log()->info("Enemy module preloads disabled by configuration");
			return;
		}
		if (GameVersion() != "1.1.0")
		{
			spd::log()->error("Enemy module preloads disabled: unsupported game version {}", GameVersion());
			return;
		}

		auto readPattern = hook::pattern(
			"55 8B EC 53 8A 5D 08 88 5D 08 80 FB 03 74 ? 80 FB 05 74 ? 80 FB 0C 75 ?");
		auto thunkPattern = hook::pattern(
			"E9 9A 64 2A 00 E9 C5 D9 3E 00 E9 E0 18 33 00");
		auto em10InitPattern = hook::pattern(
			"55 8B EC 83 EC 20 A1 ? ? ? ? 33 C5 89 45 FC A1 ? ? ? ? 56 8B 75 08 85 C0 75 ?");
		auto em12PrologPattern = hook::pattern(
			"C7 05 68 70 C5 00 14 BF 40 00 C7 05 7C 01 C5 00 EE 2B 40 00 C3");
		auto em15PrologPattern = hook::pattern(
			"C7 05 68 70 C5 00 10 7D 40 00 C7 05 7C 01 C5 00 D8 46 40 00 C3");
		auto em20PrologPattern = hook::pattern(
			"C7 05 68 70 C5 00 18 7A 40 00 C7 05 7C 01 C5 00 31 E8 40 00 C3");
		const bool dispatchRequested = !dispatches->empty();
		if (readPattern.size() != 1 || thunkPattern.size() != 1 ||
			(dispatchRequested && (em10InitPattern.size() != 1 || em12PrologPattern.size() != 1 ||
				em15PrologPattern.size() != 1 || em20PrologPattern.size() != 1)))
		{
			spd::log()->error(
				"Enemy module preloads disabled: signatures matched EmReadSearch={}, thunk={}, em10={}, em12={}, em15={}, em20={}",
				readPattern.size(), thunkPattern.size(), em10InitPattern.size(), em12PrologPattern.size(),
				em15PrologPattern.size(), em20PrologPattern.size());
			return;
		}

		const uintptr_t readAddress = readPattern.get(0).get_uintptr(0);
		const uintptr_t thunkAddress = thunkPattern.get(0).get_uintptr(0);
		const uintptr_t thunkTarget = injector::GetBranchDestination(thunkAddress).as_int();
		if (thunkTarget != readAddress)
		{
			spd::log()->error(
				"Enemy module preloads disabled: thunk target 0x{:08X} does not match EmReadSearch 0x{:08X}",
				thunkTarget, readAddress);
			return;
		}

		EnemyModulePreloads = *preloads;
		EnemyFamilyDispatches = *dispatches;
		EmReadSearch = reinterpret_cast<EmReadSearchRoutine>(readAddress);
		if (dispatchRequested)
		{
			const uintptr_t configureSlotAddress = *em10InitPattern.get(0).get<uintptr_t>(0x11);
			if (configureSlotAddress != 0x00C5017C)
			{
				spd::log()->error("Enemy family dispatch disabled: unexpected configure slot 0x{:08X}", configureSlotAddress);
				EnemyFamilyDispatches.clear();
			}
			else
			{
				EnemyFamilyConfigureSlot = reinterpret_cast<EnemyFamilyConfigureRoutine*>(configureSlotAddress);
				ExpectedEnemyFamilyConfigureRoutines[0x12] = reinterpret_cast<EnemyFamilyConfigureRoutine>(*em12PrologPattern.get(0).get<uintptr_t>(0x10));
				ExpectedEnemyFamilyConfigureRoutines[0x15] = reinterpret_cast<EnemyFamilyConfigureRoutine>(*em15PrologPattern.get(0).get<uintptr_t>(0x10));
				ExpectedEnemyFamilyConfigureRoutines[0x20] = reinterpret_cast<EnemyFamilyConfigureRoutine>(*em20PrologPattern.get(0).get<uintptr_t>(0x10));
				spd::log()->info("Enemy family dispatch armed: slot=0x{:08X}, room_entries={}",
					configureSlotAddress, EnemyFamilyDispatches.size());
			}
		}
		InjectHook(thunkAddress, EnemyModuleReadSearchHook, HookType::Jump);
		spd::log()->info(
			"Enemy module preloads installed: EmReadSearch=0x{:08X}, thunk=0x{:08X}, room_entries={}",
			readAddress, thunkAddress, EnemyModulePreloads.size());
	}

	void ApplyEnemyListOverrides()
	{
		GLOBAL_WK* global = GlobalPtr();
		const uint8_t emListNumber = static_cast<uint8_t>(global->curEmListNumber_4FB3);
		for (const auto& override : EnemyListOverrides)
		{
			if (override.emListNumber != emListNumber)
				continue;

			EM_LIST& record = global->Em_list_5410[override.emListIndex];
			const uint8_t currentId = static_cast<uint8_t>(record.id_1);
			const uint8_t currentType = static_cast<uint8_t>(record.type_2);
			if (record.room_18 != override.roomId || currentId != override.expectedEntityId || currentType != override.expectedType)
			{
				const bool alreadyApplied = record.room_18 == override.roomId &&
					currentId == override.replacementEntityId && currentType == override.replacementType;
				if (!alreadyApplied)
				{
					spd::log()->error(
						"Enemy list override refused: {} list={}, index={}, expected room/id/type=0x{:04X}/0x{:02X}/0x{:02X}, found=0x{:04X}/0x{:02X}/0x{:02X}",
						override.label, override.emListNumber, override.emListIndex, override.roomId,
						override.expectedEntityId, override.expectedType, record.room_18, currentId, currentType);
				}
				continue;
			}

			record.id_1 = static_cast<char>(override.replacementEntityId);
			record.type_2 = static_cast<char>(override.replacementType);
			record.set_3 = static_cast<char>(override.replacementSet);
			record.flag_4 = override.replacementFlags;
			record.hp_8 = override.replacementHp;
			record.emset_no_A = override.replacementEmsetNo;
			record.Character_B = static_cast<char>(override.replacementCharacter);
			record.Guard_r_1A = override.replacementGuardRadius;
			record.percentageMotionSpeed_1C = override.replacementMotionSpeed;
			record.percentageScale_1E = override.replacementScale;
			spd::log()->info(
				"Enemy list override applied: {} list={}, index={}, room=0x{:04X}, id/type 0x{:02X}/0x{:02X} -> 0x{:02X}/0x{:02X}, flags=0x{:08X}, hp={}",
				override.label, override.emListNumber, override.emListIndex, override.roomId,
				override.expectedEntityId, override.expectedType, override.replacementEntityId,
				override.replacementType, override.replacementFlags, override.replacementHp);
		}
	}

	void __cdecl EnemyListLoadHook(uint32_t flags)
	{
		EnemyListLoad(flags);
		ApplyEnemyListOverrides();
	}

	void InitializeEnemyListOverrides()
	{
		const auto overrides = LoadEnemyListOverrides();
		if (!overrides || overrides->empty())
		{
			spd::log()->info("Enemy list overrides disabled by configuration");
			return;
		}
		if (GameVersion() != "1.1.0")
		{
			spd::log()->error("Enemy list overrides disabled: unsupported game version {}", GameVersion());
			return;
		}

		auto loaderPattern = hook::pattern(
			"55 8B EC 51 53 56 8B 35 ? ? ? ? 0F B7 86 AC 4F 00 00 50 E8 ? ? ? ? 8B D8 83 C4 04 85 DB 0F 88 ? ? ? ?");
		const auto loaderMatchCount = loaderPattern.size();
		if (loaderMatchCount != 1)
		{
			spd::log()->error("Enemy list overrides disabled: loader signature matched {} locations", loaderMatchCount);
			return;
		}

		auto thunkPattern = hook::pattern("E9 AA 96 2C 00");
		const auto thunkMatchCount = thunkPattern.size();
		if (thunkMatchCount != 1)
		{
			spd::log()->error("Enemy list overrides disabled: loader thunk signature matched {} locations", thunkMatchCount);
			return;
		}

		const auto loaderAddress = loaderPattern.get(0).get_uintptr(0);
		const auto thunkAddress = thunkPattern.get(0).get_uintptr(0);
		const auto thunkDestination = injector::GetBranchDestination(thunkAddress).as_int();
		if (thunkDestination != loaderAddress)
		{
			spd::log()->error(
				"Enemy list overrides disabled: thunk destination 0x{:08X} does not match loader 0x{:08X}",
				thunkDestination, loaderAddress);
			return;
		}

		EnemyListLoad = reinterpret_cast<EnemyListLoadRoutine>(loaderAddress);
		EnemyListOverrides = *overrides;
		InjectHook(thunkAddress, EnemyListLoadHook, HookType::Jump);
		spd::log()->info(
			"Enemy list overrides installed: loader=0x{:08X}, thunk=0x{:08X}, entries={}",
			loaderAddress, thunkAddress, EnemyListOverrides.size());
	}

	const EnemySpawnProfile* FindEnemySpawnProfile(uint16_t roomId, uint8_t emListNumber, uint8_t emListIndex)
	{
		const auto match = std::find_if(EnemySpawnProfiles.begin(), EnemySpawnProfiles.end(),
			[&](const EnemySpawnProfile& profile) {
				return profile.roomId == roomId && profile.emListNumber == emListNumber &&
					profile.emListIndex == emListIndex;
			});
		return match != EnemySpawnProfiles.end() ? &*match : nullptr;
	}

	void InitializeEnemySpawnProfiles()
	{
		const auto profiles = LoadEnemySpawnProfiles();
		if (!profiles || profiles->empty())
		{
			spd::log()->info("Enemy spawn profiles disabled by configuration");
			return;
		}
		if (GameVersion() != "1.1.0")
		{
			spd::log()->error("Enemy spawn profiles disabled: unsupported game version {}", GameVersion());
			return;
		}
		EnemySpawnProfiles = *profiles;
		spd::log()->info("Enemy spawn profiles loaded: {} entries", EnemySpawnProfiles.size());
	}

	int __cdecl EnemyLifeDownObserver(cEm* entity, int damage, int randomAmplitude, uint32_t flags)
	{
		const bool shouldObserve = entity != nullptr && IsEnemy(entity->id_100) &&
			(EnemyHitObservationEntityIds.empty() ||
				std::find(EnemyHitObservationEntityIds.begin(), EnemyHitObservationEntityIds.end(), entity->id_100) != EnemyHitObservationEntityIds.end());
		if (!shouldObserve)
			return EnemyLifeDown(entity, damage, randomAmplitude, flags);

		const int hpBefore = entity->hp_324;
		const int partNumber = entity->m_DmgInfo_328.m_pDamageYarare_18 != nullptr
			? entity->m_DmgInfo_328.m_pDamageYarare_18->parts_no_26
			: -1;
		const unsigned weapon = entity->m_DmgInfo_328.m_Wep_6;
		const unsigned roomId = GlobalPtr()->curRoomId_4FAC;
		const unsigned emListNumber = static_cast<uint8_t>(GlobalPtr()->curEmListNumber_4FB3);
		const unsigned emListIndex = entity->emListIndex_3A0;
		const unsigned entityId = entity->id_100;
		const unsigned entityType = entity->type_101;
		const unsigned guid = entity->guid_F8;
		const auto* profile = FindEnemySpawnProfile(static_cast<uint16_t>(roomId),
			static_cast<uint8_t>(emListNumber), static_cast<uint8_t>(emListIndex));
		const std::string profileLabel = profile != nullptr ? profile->label : "none";
		const std::string profileStage = profile != nullptr ? profile->stage : "none";

		const int result = EnemyLifeDown(entity, damage, randomAmplitude, flags);
		spd::log()->info(
			"Enemy hit observation: room=0x{:04X}, em_list={}, index={}, id=0x{:02X}, type=0x{:02X}, guid=0x{:08X}, profile={}, stage={}, weapon=0x{:02X}, part={}, hp_before={}, requested_damage={}, random_amplitude={}, flags=0x{:08X}, hp_after={}",
			roomId, emListNumber, emListIndex, entityId, entityType, guid, profileLabel, profileStage, weapon, partNumber,
			hpBefore, damage, randomAmplitude, flags, result);
		return result;
	}

	void InstallEnemyHitObserver()
	{
		const auto config = LoadEnemyHitObservationConfig();
		if (!config || !config->enabled)
		{
			spd::log()->info("Enemy hit observation disabled by configuration");
			return;
		}
		if (GameVersion() != "1.1.0")
		{
			spd::log()->error("Enemy hit observation disabled: unsupported game version {}", GameVersion());
			return;
		}

		auto lifeDownPattern = hook::pattern(
			"55 8B EC 53 56 57 E8 ? ? ? ? 0F B6 F0 C1 E6 08 E8 ? ? ? ? 8B 4D 10 8B 7D 0C");
		const auto lifeDownMatchCount = lifeDownPattern.size();
		if (lifeDownMatchCount != 1)
		{
			spd::log()->error("Enemy hit observation disabled: LifeDown signature matched {} locations", lifeDownMatchCount);
			return;
		}

		auto thunkPattern = hook::pattern("E9 2A E9 1A 00");
		const auto thunkMatchCount = thunkPattern.size();
		if (thunkMatchCount != 1)
		{
			spd::log()->error("Enemy hit observation disabled: LifeDown thunk signature matched {} locations", thunkMatchCount);
			return;
		}

		const auto lifeDownAddress = lifeDownPattern.get(0).get_uintptr(0);
		const auto thunkAddress = thunkPattern.get(0).get_uintptr(0);
		const auto thunkDestination = injector::GetBranchDestination(thunkAddress).as_int();
		if (thunkDestination != lifeDownAddress)
		{
			spd::log()->error(
				"Enemy hit observation disabled: LifeDown thunk destination 0x{:08X} does not match signature address 0x{:08X}",
				thunkDestination, lifeDownAddress);
			return;
		}

		EnemyLifeDown = reinterpret_cast<EnemyLifeDownRoutine>(lifeDownAddress);
		EnemyHitObservationEntityIds = config->entityIds;
		InjectHook(thunkAddress, EnemyLifeDownObserver, HookType::Jump);
		spd::log()->info(
			"Enemy hit observation installed: LifeDown=0x{:08X}, thunk=0x{:08X}, entity_filter_count={}",
			lifeDownAddress, thunkAddress, EnemyHitObservationEntityIds.size());
	}
}

void re4t::enemy_profiles::ApplySpawnProfile(cEm* entity, const EM_LIST* source, uint8_t emListIndex)
{
	if (entity == nullptr || source == nullptr || EnemySpawnProfiles.empty() || emListIndex == 0xFF)
		return;

	const auto* profile = FindEnemySpawnProfile(GlobalPtr()->curRoomId_4FAC,
		static_cast<uint8_t>(GlobalPtr()->curEmListNumber_4FB3), emListIndex);
	if (profile == nullptr)
		return;
	if (entity->id_100 != profile->expectedEntityId || entity->type_101 != profile->expectedType ||
		static_cast<uint8_t>(source->id_1) != profile->expectedEntityId ||
		static_cast<uint8_t>(source->type_2) != profile->expectedType)
	{
		spd::log()->error(
			"Enemy spawn profile refused: {} expected id/type=0x{:02X}/0x{:02X}, runtime=0x{:02X}/0x{:02X}",
			profile->label, profile->expectedEntityId, profile->expectedType, entity->id_100, entity->type_101);
		return;
	}

	const int vanillaHp = source->hp_8;
	const long scaledHp = std::lround(static_cast<double>(vanillaHp) * profile->hpMultiplier);
	if (vanillaHp <= 0 || scaledHp <= 0 || scaledHp > std::numeric_limits<int16_t>::max())
	{
		spd::log()->error(
			"Enemy spawn profile refused: {} HP {} x {} would produce {} outside [1, 32767]",
			profile->label, vanillaHp, profile->hpMultiplier, scaledHp);
		return;
	}

	entity->hp_324 = static_cast<int16_t>(scaledHp);
	entity->hp_max_326 = static_cast<int16_t>(scaledHp);
	spd::log()->info(
		"Enemy spawn profile applied: {} stage={}, room=0x{:04X}, list={}, index={}, id=0x{:02X}, type=0x{:02X}, source_flags=0x{:08X}, runtime_flags=0x{:08X}, vanilla_hp={}, multiplier={}, final_hp={}",
		profile->label, profile->stage, profile->roomId, profile->emListNumber, profile->emListIndex,
		profile->expectedEntityId, profile->expectedType, source->flag_4, entity->flag_3D0,
		vanillaHp, profile->hpMultiplier, scaledHp);
}

float(__cdecl* CameraControl__getCameraDirection)();
void __cdecl wep17_r3_ready00_Hook(cPlayer* a1)
{
	// Update weapon direction to match camera if allowing quickturn
	if (re4t::cfg->bAllowMatildaQuickturn)
		a1->Wep_7D8->m_CamAdjY_30 = CameraControl__getCameraDirection();

	wep17_r3_ready00(a1);
}

void __cdecl wep17_r3_ready10_Hook(cPlayer* a1)
{
	// Jump to wep02_r3_ready10 to allow Mathilda to quickturn character
	if (re4t::cfg->bAllowMatildaQuickturn)
		wep02_r3_ready10(a1);
	else
		wep17_r3_ready10(a1);
}

void(__fastcall* cPlayer__weaponInit)(cPlayer* thisptr, void* unused);
void __fastcall cPlayer__weaponInit_Hook(cPlayer* thisptr, void* unused)
{
	// Fix Ditman glitch by resetting player anim speed to 1 when weapon changing (weaponInit is called during change)
	// Ditman glitch seems to be caused by changing weapon while in-between different weapon states
	// The Striker has unique code inside wep07_r3_ready00 state which increases this speed var to 1.4
	// (maybe as a workaround to increase the anim speed without redoing anims, no other weps have similar code)
	// Normally this would then be nearly-instantly reverted back to 1.0 by the wep07_r3_ready10 state, but changing weapons can interrupt that
	// We fix that here by resetting speed to 1 whenever weapon change is occurring, pretty simple fix

	if (re4t::cfg->bFixDitmanGlitch)
		thisptr->Motion_1D8.Seq_speed_C0 = 1.0f;

	cPlayer__weaponInit(thisptr, unused);
}

bool bShouldDropChicagoAmmo = false;
void ChicagoAmmoDropCheck() noexcept
{
	bool hasChicago = ItemMgr->num((ITEM_ID)EItemId::Thompson) /* || ItemMgr->num((ITEM_ID)EItemId::Ada_Machine_Gun) */;
	if (!hasChicago)
	{
		bShouldDropChicagoAmmo = false;
		return;
	}

	auto chicagoPtr = ItemMgr->search((ITEM_ID)EItemId::Thompson);
	if (!chicagoPtr)
	{
		bShouldDropChicagoAmmo = false;
		return;
	}

	// Don't drop ammo if the weapon has unlimited ammo already
	if ((chicagoPtr->getCapacity() + 1) == 7)
	{
		bShouldDropChicagoAmmo = false;
		return;
	}
	
	int curChicagoAmmo = ItemMgr->num((ITEM_ID)EItemId::Bullet_45in_M);
	bool rnd = GetRandomInt(0, 31) == 5; // 5/31 probability (16%)

	bShouldDropChicagoAmmo = (rnd || (curChicagoAmmo <= 35));
}

// A bit ugly, but I couldn't find a better way to do this without reimplementing the entire func...
uintptr_t ChicagoAmmoDrop_finish;
void __declspec(naked) ChicagoAmmoDrop()
{
	_asm
	{
		pushad
		pushfd
	}

	ChicagoAmmoDropCheck();

	if (bShouldDropChicagoAmmo)
	{
		bShouldDropChicagoAmmo = false;
		static int ret_id = int(EItemId::Bullet_45in_M);
		static int ret_num = 70;

		_asm
		{
			popfd
			popad

			mov eax, ret_id
			mov ebx, ret_num
			mov edx, [ebp + 0x8]
			mov ecx, [ebp + 0xC]
			mov dword ptr[edx], eax
			mov dword ptr[ecx], ebx
			mov eax, 0x1
			mov esp, ebp
			pop ebp
			ret
		}
	}
	else
	{
		_asm
		{
			popfd
			popad

			mov ebx, [ebp - 0x10]
			cmp[ebp - 0x14], ebx
			jmp ChicagoAmmoDrop_finish
		}
	}
}

void re4t::init::Gameplay()
{
	ApplyWeaponFirepowerOverrides();
	InitializeEnemyModulePreloads();
	InitializeEnemySpawnProfiles();
	InitializeEnemyListOverrides();
	InitializeActiveEnemyOverrides();
	InstallEnemyHitObserver();

	// Make the Chicago Typewriter not upgraded by default and try to balance it more for normal gameplay.
	// This mostly works fine for Ada too (minus the fact the Merchant has no upgrades in SW...), but the bigger problem
	// is that Ada's Chicago Typewriter never calls its reload func. Haven't figured out why that is. For now, we'll just
	// not enable any of this for her, so I've left everyting I got so far commented out.
	if (re4t::cfg->bBalancedChicagoTypewriter)
	{
		// Hook GetDropBullet to make the game drop the unused Chicago Typewriter ammo
		auto pattern = hook::pattern("8B 5D ? 39 5D ? 72 ? E8 ? ? ? ? 0F B6 ? 81 E2 ? ? ? ? 79 ? 4A 83 CA ? 42 83 FA ? 0F 85");
		injector::MakeNOP(pattern.count(1).get(0).get<uint32_t>(0), 6, true);
		ChicagoAmmoDrop_finish = (uintptr_t)pattern.count(1).get(0).get<uint32_t>(6);
		injector::MakeJMP(pattern.count(1).get(0).get<uint32_t>(0), ChicagoAmmoDrop, true);

		// Get WeaponLevelTbl to change the Chicago Typewriter firepower stats.
		// Originally it is always 10.0f regardless of what upgrade level you have. We now make it grow over time, with
		// the max upgrade level bringing it to the vanilla firepower of 10.0f.
		pattern = hook::pattern("D9 04 8D ? ? ? ? D9 5D ? 75 ? 8B CE 83 E9");
		auto WeaponLevelTbl = *pattern.count(1).get(0).get<float(*)[49][7]>(3);

		// Get wep num of Leon's Chicago
		auto TypewriterNumLeon = bio4::WeaponId2WeaponNo(ITEM_ID(EItemId::Thompson));

		/*
		// Get wep num of Ada's Chicago
		auto TypewriterNumAda = bio4::WeaponId2WeaponNo(ITEM_ID(EItemId::Ada_Machine_Gun));
		*/

		// Our new fire power array
		float NewChicagoFirepower[] = { 0.9f, 1.5f, 1.7f, 2.0f, 2.5f, 3.5f, 10.0f };

		// Write new values
		std::copy(std::begin(NewChicagoFirepower), std::end(NewChicagoFirepower), std::begin((*WeaponLevelTbl)[TypewriterNumLeon]));
		/* std::copy(std::begin(NewChicagoFirepower), std::end(NewChicagoFirepower), std::begin((*WeaponLevelTbl)[TypewriterNumAda])); */

		// Nop special case inside dispBuyItemList that makes the weapon stats be shown as fully upgraded
		pattern = hook::pattern("BE ? ? ? ? F7 42 ? ? ? ? ? 74 ? 83 C0 ? 83 F8 ? 77 ? 0F B6 90");
		injector::MakeNOP(pattern.count(1).get(0).get<uint32_t>(0), 5, true);

		// Prevent cItemMgr::construct from forcefully applying upgrades the chicago when bought
		pattern = hook::pattern("B9 ? ? ? ? 66 89 4E ? 8B 15 ? ? ? ? F7 42");
		injector::WriteMemory(pattern.count(1).get(0).get<uint32_t>(1), uint32_t(0), true);

		/*
		// Get ada_weapon_level to change Ada's Chicago Typewriter fire power stats.
		pattern = hook::pattern("0F BE 8C 12 ? ? ? ? 03 D2 66 ? 66 33 4E");
		auto tbl_addr = *pattern.count(1).get(0).get<uintptr_t>(4);
		tbl_addr -= 2;

		auto ada_weapon_level = (ADA_WEAPON_LEVEL(*)[7])tbl_addr;

		(*ada_weapon_level)[5].power_2 = 1;
		(*ada_weapon_level)[5].bullet_5 = 1;
		*/

		// Add chicago to the Merchant's upgrade list
		pattern = hook::pattern("68 ? ? ? ? E8 ? ? ? ? 6A 00 68 ? ? ? ? EB 54");
		static auto merchantData = *pattern.count(1).get(0).get<MERCHANT_DATA(*)[1]>(1);

		static LEVEL_INFO level_ext_tompson[2] = {
			{ 0x34, {7, 1, 1, 7}, {0, 0} },
			{ 0xFFFF, {0, 0, 0, 0}, {0, 0} }
		};
		
		pattern = hook::pattern("8B 0D ? ? ? ? F7 41 ? ? ? ? ? 74 ? 6A");
		struct MerchantRoomInit_hook_chicagolvl
		{
			void operator()(injector::reg_pack& regs)
			{
				regs.ecx = uint32_t(SystemSavePtr());

				bio4::levelDataAdd(*merchantData, level_ext_tompson, 2);
			}
		}; injector::MakeInline<MerchantRoomInit_hook_chicagolvl>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(6));

		/* <- Works, but it isn't really necessary. Meh.
		* 
		// Add new value to level_price
		pattern = hook::pattern("C7 05 ? ? ? ? ? ? ? ? 0F B7 81 ? ? ? ? 8D");
		static auto level_price = *pattern.count(1).get(0).get<LEVEL_PRICE(*)[16]>(6);

		// define the new element
		LEVEL_PRICE chicagoLvlUpPrice = {
			0x34, // id
			{ 700, 1400, 1800, 2400, 3500, 10000, 0 },
			{ 0, 0, 0 },
			{ 500, 1500, 0 },
			{ 700, 1500, 2000, 2500, 3500, 0, 0 }
		};

		// copy all the elements from the original array into a array
		static LEVEL_PRICE newArray[18];
		std::copy(std::begin(*level_price), std::end(*level_price), newArray);

		// add the new element to the end of the array
		newArray[17] = chicagoLvlUpPrice;

		memset(*level_price, 0, sizeof(*level_price));

		injector::WriteMemory(pattern.count(1).get(0).get<uint32_t>(6), &newArray, true);
		*/

		spd::log()->info("BalancedChicagoTypewriter enabled");
	}

	// Unlock JP-only classic camera angle during Ashley segment
	{
		static uint8_t* pSys = nullptr;
		struct UnlockAshleyJPCameraAngles
		{
			void operator()(injector::reg_pack& regs)
			{
				bool unlock = *(uint32_t*)(pSys + 8) == 0 // pSys->language_8
					|| re4t::cfg->bAshleyJPCameraAngles;

				// set zero-flag if we're unlocking the camera, for the jz game uses after this hook
				if (unlock)
					regs.ef |= (1 << regs.zero_flag);
				else
					regs.ef &= ~(1 << regs.zero_flag); // clear zero_flag if unlock is false, in case it was set by something previously
			}
		};

		auto pattern = hook::pattern("8B 0D ? ? ? ? 80 79 08 00 75 ? 8A 80 A3 4F 00 00");
		pSys = *pattern.count(1).get(0).get<uint8_t*>(2);

		injector::MakeInline<UnlockAshleyJPCameraAngles>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(10));

		if (re4t::cfg->bAshleyJPCameraAngles)
			spd::log()->info("AshleyJPCameraAngles enabled");
	}

	// NTSC mode
	// Enables difficulty modifiers previously exclusive to the NTSC console versions of RE4.
	// These were locked behind checks for pSys->language_8 == 1 (NTSC English). Since RE4 UHD uses PAL English (language_8 == 2), PC players never saw these.
	if (re4t::cfg->bEnableNTSCMode)
	{
		// Normal mode and Separate Ways: increased starting difficulty (3500->5500)
		auto pattern = hook::pattern("05 7C 15 00 00 6A 00 89 81 94 4F 00 00");
		struct GamePointInit_NormalGameRank
		{
			void operator()(injector::reg_pack& regs)
			{
				*(uint32_t*)(regs.ecx + 0x4F94) = SystemSavePtr()->language_8 == 1 || re4t::cfg->bEnableNTSCMode ? 5500 : 3500;
			}
		}; injector::MakeInline<GamePointInit_NormalGameRank>(pattern.count(1).get(0).get<uint32_t>(7), pattern.count(1).get(0).get<uint32_t>(13));

		// Assignment Ada: increased difficulty (4500->6500)
		pattern = hook::pattern("8B ? ? ? ? ? 80 7E 08 01 74");
		struct GameAddPoint_AAdaGameRank
		{
			void operator()(injector::reg_pack& regs)
			{
				if (SystemSavePtr()->language_8 == 1 || re4t::cfg->bEnableNTSCMode)
					regs.ef |= (1 << regs.zero_flag);
				else
					regs.ef &= ~(1 << regs.zero_flag);
			}
		}; injector::MakeInline<GameAddPoint_AAdaGameRank>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(10));

		// Shooting range: increased bottle cap score requirements (1000->3000)
		pattern = hook::pattern("8B F9 8A ? ? 8B ? ? FE C9");
		Patch(pattern.count(1).get(0).get<uint32_t>(2), { 0xB1, 0x01, 0x90 }); // cCap::check, { mov cl, 1 }

		// Shooting range: use NTSC strings for the game rules note
		// (only supports English for now, as only eng/ss_file_01.MDT contains the additional strings necessary for this)
		pattern = hook::pattern(re4t::sections::data, "? 00 01 00 46 00 01 00");
		file_msg_tbl_35 = pattern.count(1).get(0).get<FILE_MSG_TBL_mb>(0);
		// update the note's message index whenever we load into r22c
		pattern = hook::pattern("89 41 78 83 C1 7C E8");
		struct R22cInit_UpdateMsgIdx
		{
			void operator()(injector::reg_pack& regs)
			{
				file_msg_tbl_35[0].top_0 = SystemSavePtr()->language_8 == 2 ? 0x9D : 0x3F;

				// code we overwrote
				*(uint32_t*)(regs.ecx + 0x78) = regs.eax;
				regs.ecx += 0x7C;
			}
		}; injector::MakeInline<R22cInit_UpdateMsgIdx>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(6));

		// Shooting range: only check for bottle cap reward once per results screen
		pattern = hook::pattern("8B 15 ? ? ? ? 80 7A ? 01 74");
		Patch(pattern.count(2).get(1).get<uint32_t>(10), { 0xEB }); // shootResult, jz -> jmp

		// Mercenaries: unlock village stage difficulty, requires 60fps fix
		Patch(pattern.count(2).get(0).get<uint32_t>(10), { 0xEB }); // GameAddPoint, jz -> jmp

		// remove Easy mode from the difficulty menu
		pattern = hook::pattern("A1 ? ? ? ? 80 78 ? 01 75");
		injector::MakeNOP(pattern.count(1).get(0).get<uint32_t>(9), 2); // titleLevelInit

		// Swap NEW GAME texture for START on a fresh system save
		pattern = hook::pattern("89 51 68 8B 57 68 8B");
		struct titleMenuInit_StartTex
		{
			void operator()(injector::reg_pack& regs)
			{
				bool newSystemSave = !FlagIsSet(SystemSavePtr()->flags_EXTRA_4, uint32_t(Flags_EXTRA::EXT_HARD_MODE));
				if (newSystemSave)
				{
					float texW;

					// texW = aspect ratio of image file * size0_H_E0 (14)
					switch (SystemSavePtr()->language_8)
					{
					case 4: // French
						texW = 131.0f;
						break;
					case 5: // Spanish
						texW = 70.0f;
						break;
					case 6: // Traditional Chinese / Italian
						texW = GameVersion() == "1.1.0" ? 66.0f : 68.0f;
						break;
					case 8: // Italian
						texW = 68.0f;
						break;
					default: // English, German, Japanese, Simplified Chinese
						texW = 66.0f;
						break;
					}

					IdSysPtr()->unitPtr(0x1u, IDC_TITLE_MENU_0)->texId_78 = 164;
					IdSysPtr()->unitPtr(0x1u, IDC_TITLE_MENU_0)->size0_W_DC = texW;
					IdSysPtr()->unitPtr(0x2u, IDC_TITLE_MENU_0)->texId_78 = 164;
					IdSysPtr()->unitPtr(0x2u, IDC_TITLE_MENU_0)->size0_W_DC = texW;
				}

				// Code we overwrote
				*(uint32_t*)(regs.ecx + 0x68) = regs.edx;
				regs.edx = *(uint32_t*)(regs.edi + 0x68);
			}
		}; injector::MakeInline<titleMenuInit_StartTex>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(6));

		// Patches for Japanese language support (language_8 == 0):

		// repurpose the hide Professional mode block to hide Amateur mode instead
		pattern = hook::pattern("C7 46 30 01 00 00 00");
		injector::MakeNOP(pattern.count(1).get(0).get<uint32_t>(7), 2); // titleLevelInit
		injector::MakeNOP(pattern.count(1).get(0).get<uint32_t>(16), 2);
		Patch(pattern.count(1).get(0).get<uint32_t>(21), { 0x09 });
		Patch(pattern.count(1).get(0).get<uint32_t>(38), { 0x0A });
		// erase the rest of the block
		pattern = hook::pattern("C7 46 ? 03 00 00 00 85 DB");
		injector::MakeNOP(pattern.get_first(0), 37);

		// disable JP difficulty select confirmation prompts
		pattern = hook::pattern("A1 ? ? ? ? 38 58 08 75");
		Patch(pattern.count(1).get(0).get<uint32_t>(8), { 0xEB }); // titleMain, jnz -> jmp

		// remove JP only 20% damage armor from Mercenaries mode
		pattern = hook::pattern("F7 46 54 00 00 00 40");
		Patch(pattern.count(1).get(0).get<uint32_t>(7), { 0xEB }); // LifeDownSet2, jz -> jmp

		spd::log()->info("NTSC mode enabled");
	}

	// Patch Ashley suplex glitch back into the game
	// They originally fixed this by adding a check for Ashley player-type inside em10ActEvtSetFS
	// However, the caller of that function (em10_R1_Dm_Small) already has player-type checking inside it, which will demote the suplex to a Kick for certain characters
	// Because of the way they patched it, enemies that are put into Suplex-state won't have any actions at all while playing as Ashley, while other chars are able to Kick
	// So we'll fix this by first removing the patch they added inside em10ActEvtSetFS, then add checks inside em10_R1_Dm_Small instead so that Ashley can Kick
	// (or if user wants we'll leave it with patch removed, so that they can play with the Ashley suplex :)
	// TODO: some reason the below doesn't actually let Ashley use SetKick, might be player-type checks inside it, so right now this just disables Suplex if bAllowAshleySuplex isn't set
	{
		// Remove player-type check from em10ActEvtSetFS
		auto pattern = hook::pattern("0F 8E ? ? ? ? A1 ? ? ? ? 80 B8 C8 4F 00 00 01 0F 84");
		injector::MakeNOP(pattern.count(1).get(0).get<uint8_t>(0x12), 6, true);

		pattern = hook::pattern("0F B6 81 C8 4F 00 00 83 C0 FE 83 F8 03 77");
		struct SuplexCheckPlayerAshley
		{
			void operator()(injector::reg_pack& regs)
			{
				int playerType = *(uint8_t*)(regs.ecx + 0x4FC8);

				// code we patched over
				regs.eax = playerType;
				regs.eax = regs.eax - 2;

				// Add Ashley player-type check, make it use Ada/Hunk/Wesker case which calls SetKick
				// TODO: some reason SetKick doesn't work for Ashley? might be player-type checks inside it, so right now this just disables Suplex if bAllowAshleySuplex isn't set
				if (!re4t::cfg->bAllowAshleySuplex && playerType == 1)
				{
					regs.eax = 0;
				}
			}
		}; injector::MakeInline<SuplexCheckPlayerAshley>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(10));
	}

	// Hooks to allow quickturning character when wielding Matilda
	// (credits to qingsheng8848 for finding out this method!)
	{
		// Get pointer to wep02_r2_ready func table
		auto pattern = hook::pattern("89 45 FC 53 56 8B 75 08 0F B6 86 FF 00 00 00 8B 0C 85");
		uint32_t* wep02_r2_ready_funcTbl = *pattern.count(1).get(0).get<uint32_t*>(0x12);

		wep02_r3_ready10 = (wepXX_routine)wep02_r2_ready_funcTbl[1];

		// Get pointer to wep17_r2_ready (mathilda) func table
		pattern = hook::pattern("C6 40 24 01 0F B6 86 FF 00 00 00 8B 0C 85 ? ? ? ? 56 FF D1 83 C4 04 83 3D");
		uint32_t* wep17_r2_ready_funcTbl = *pattern.count(1).get(0).get<uint32_t*>(0xE);

		wep17_r3_ready00 = (wepXX_routine)wep17_r2_ready_funcTbl[0];
		wep17_r3_ready10 = (wepXX_routine)wep17_r2_ready_funcTbl[1];

		// Hook wep17_r3_ready00 & ready10
		wep17_r2_ready_funcTbl[0] = (uint32_t)&wep17_r3_ready00_Hook;
		wep17_r2_ready_funcTbl[1] = (uint32_t)&wep17_r3_ready10_Hook;

		// Fetch CameraControl::getCameraDirection addr
		pattern = hook::pattern("E8 ? ? ? ? 8B 96 D8 07 00 00 D9 5A 30");
		ReadCall(injector::GetBranchDestination(pattern.count(1).get(0).get<uint32_t>(0)).as_int(), CameraControl__getCameraDirection);
	}

	// Hook cPlayer::weaponInit so we can add code to fix ditman glitch
	{
		auto pattern = hook::pattern("83 C4 0C E8 ? ? ? ? D9 EE 8B 06 D9 9E 44 05 00 00");

		ReadCall(injector::GetBranchDestination(pattern.count(1).get(0).get<uint32_t>(3)).as_int(), cPlayer__weaponInit);
		InjectHook(injector::GetBranchDestination(pattern.count(1).get(0).get<uint32_t>(3)).as_int(), cPlayer__weaponInit_Hook, HookType::Jump);
	}

	// Disable Automatic Reload
	{
		auto pattern = hook::pattern("8B 86 D8 07 00 00 8B 48 34 8B 11 8B 42 4C FF D0");
		struct DisableReloadCheck
		{
			void operator()(injector::reg_pack& regs)
			{
				if (PlayerPtr()->Wep_7D8->m_pWep_34->reloadable() && !re4t::cfg->bDisableAutomaticReload)
					regs.ef &= ~(1 << regs.zero_flag);
				else
					regs.ef |= (1 << regs.zero_flag);
			}
		};
		// for Handguns, Matilda, TMP, Shotguns
		injector::MakeInline<DisableReloadCheck>(pattern.count(5).get(0).get<uint32_t>(0), pattern.count(5).get(0).get<uint32_t>(18));
		injector::MakeInline<DisableReloadCheck>(pattern.count(5).get(1).get<uint32_t>(0), pattern.count(5).get(1).get<uint32_t>(18));
		injector::MakeInline<DisableReloadCheck>(pattern.count(5).get(2).get<uint32_t>(0), pattern.count(5).get(2).get<uint32_t>(18));
		injector::MakeInline<DisableReloadCheck>(pattern.count(5).get(4).get<uint32_t>(0), pattern.count(5).get(4).get<uint32_t>(18));
		// for Rifles, Mine Thrower
		pattern = hook::pattern("8B 8E D8 07 00 00 8B 49 34 8B 11 8B 42 4C FF");
		injector::MakeInline<DisableReloadCheck>(pattern.count(3).get(1).get<uint32_t>(0), pattern.count(3).get(1).get<uint32_t>(18));
		injector::MakeInline<DisableReloadCheck>(pattern.count(3).get(2).get<uint32_t>(0), pattern.count(3).get(2).get<uint32_t>(18));
	}

	// Limit the Matilda to one three round burst per trigger pull
	{
		auto pattern = hook::pattern("E8 ? ? ? ? 85 C0 74 ? 8B 96 D8 07 00 00 8B 4A 34 E8");
		struct wep17_r2_set_joyFireTrig
		{
			void operator()(injector::reg_pack& regs)
			{
				if (bio4::joyFireTrg())
				{
					wep17_justFired = true;
					regs.ef &= ~(1 << regs.zero_flag);
				}
				else
					regs.ef |= (1 << regs.zero_flag);
			}
		}; injector::MakeInline<wep17_r2_set_joyFireTrig>(pattern.count(2).get(1).get<uint32_t>(0), pattern.count(2).get(1).get<uint32_t>(7));

		pattern = hook::pattern("E8 ? ? ? ? 85 C0 74 ? 8B 8E D8 07 00 00 8B 49 34 E8 ? ? ? ? 84 C0 0F ? ? ? ? ? 8B");
		struct wep17_r2_set_joyFireOn 
		{
			void operator()(injector::reg_pack& regs)
			{
				if ((!re4t::cfg->bLimitMatildaBurst || !wep17_justFired) && bio4::joyFireOn())
				{
					wep17_justFired = true;
					regs.ef &= ~(1 << regs.zero_flag);
				}
				else
					regs.ef |= (1 << regs.zero_flag);
			}
		}; injector::MakeInline<wep17_r2_set_joyFireOn>(pattern.count(1).get(0).get<uint32_t>(0), pattern.count(1).get(0).get<uint32_t>(7));

		pattern = hook::pattern("55 8B EC 56 8B ? ? 0F B6 86 FE 00 00 00 8B ? ? ? ? ? ? 56 FF ? 8B 8E D8 07 00 00 83 C4 04 E8 ? ? ? ? 83");
		struct wep17_move_LimitMatildaBurst
		{
			void operator()(injector::reg_pack& regs)
			{
				// reset justFired flag anytime the player lifts up on the trigger
				if (!bio4::joyFireOn())
					wep17_justFired = false;

				// Code we overwrote
				regs.eax = *(uint8_t*)(regs.esi + 0xFE);
			}
		}; injector::MakeInline<wep17_move_LimitMatildaBurst>(pattern.count(3).get(2).get<uint32_t>(7), pattern.count(3).get(2).get<uint32_t>(14));
	}
}
