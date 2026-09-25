#pragma once

#include <cstdint>

class cEm;
struct EM_LIST;

namespace re4t::enemy_profiles
{
	void ApplySpawnProfile(cEm* entity, const EM_LIST* source, uint8_t emListIndex);
}
