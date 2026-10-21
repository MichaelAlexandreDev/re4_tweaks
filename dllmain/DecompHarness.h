#pragma once

// Shared between DecompHarness.cpp and the generated decomp/DecompUnits.cpp.
// System headers stay outside bio4; generated game sources are included later.
#include "DecompShadow.h"

namespace bio4
{
	struct DecompEntry
	{
		const char* name;
		unsigned int va;         // address in the 0x00400000-based image
		void* replacement;       // counting wrapper around the rewritten function
		const char* signature;   // original bytes, relocated bytes as '?'
		void* shadowReplacement = nullptr; // audited dual-execution wrapper only
		unsigned int shadowStolenBytes = 0; // whole instructions, no relative branches
	};

	extern const DecompEntry kDecompEntries[];
	extern const unsigned int kDecompEntryCount;

	// Address of `va` in the running, possibly ASLR-relocated, bio4.exe.
	unsigned int decomp_va(unsigned int va);

	// Called by each wrapper; logs the first call of every replaced function.
	void decomp_count(unsigned int index);

	// Log line for decomp mods: message followed by three hex values.
	void decomp_log(const char* message, unsigned int a, unsigned int b, unsigned int c);

	// Shadow wrappers are generated only for explicitly audited descriptors.
	void* decomp_shadow_original(unsigned int index);
	bool decomp_shadow_keep_original();
	bool decomp_shadow_enter(unsigned int index);
	void decomp_shadow_leave();
	void decomp_shadow_skip(unsigned int index, const char* reason);
	void decomp_shadow_finish(unsigned int index, const shadow::Difference& difference);
	struct DecompShadowScope
	{
		~DecompShadowScope() { decomp_shadow_leave(); }
	};

	// Mods under decomp/mods.
	bool EmFamilyDispatch_Install();
	bool EmModuleSlots_Install();
}
