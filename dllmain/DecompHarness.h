#pragma once

// Shared between DecompHarness.cpp and the generated decomp/DecompUnits.cpp.
// No system headers: DecompUnits.cpp compiles the decomp sources right after
// this file, inside namespace bio4.

namespace bio4
{
	struct DecompEntry
	{
		const char* name;
		unsigned int va;         // address in the 0x00400000-based image
		void* replacement;       // counting wrapper around the rewritten function
		const char* signature;   // original bytes, relocated bytes as '?'
	};

	extern const DecompEntry kDecompEntries[];
	extern const unsigned int kDecompEntryCount;

	// Address of `va` in the running, possibly ASLR-relocated, bio4.exe.
	unsigned int decomp_va(unsigned int va);

	// Called by each wrapper; logs the first call of every replaced function.
	void decomp_count(unsigned int index);

	// Log line for decomp mods: message followed by three hex values.
	void decomp_log(const char* message, unsigned int a, unsigned int b, unsigned int c);

	// Mods under decomp/mods.
	bool EmFamilyDispatch_Install();
	bool EmModuleSlots_Install();
}
