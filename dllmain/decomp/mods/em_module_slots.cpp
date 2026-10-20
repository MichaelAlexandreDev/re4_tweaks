// Mod: more enemy module slots (harness only, off by default).
// Investigation: reverse/investigations/em-family-dispatch/report.md
//
// EmReadModule holds four modules; r100 already needs four (em12, em23, em21
// and em2A), so any extra family leaves a scene element without its module.
// Only SearchEmModule, pullEmModule, EmReadSearch/readEm, InitModule and
// EmReadInit touch the table, all of them decompiled. With the mod on and
// all of them replaced, they use the larger table below instead of the
// game's; pointers handed to the game keep the same 0x98-byte layout.

#include "read.h"

static const unsigned int kEmReadModuleVa = 0x00C670A0;
static const unsigned int kGameModuleSlots = 4;
static const unsigned int kModModuleSlots = 8;

static ReadModule g_moduleSlots[kModModuleSlots];
static bool g_moreSlots = false;

ReadModule* decomp_em_read_module()
{
    if (g_moreSlots) {
        return g_moduleSlots;
    }
    return reinterpret_cast<ReadModule*>(decomp_va(kEmReadModuleVa));
}

int decomp_em_read_module_count()
{
    return (int) (g_moreSlots ? kModModuleSlots : kGameModuleSlots);
}

// Called by the harness after every table user is replaced and before the
// game loads its first module.
bool EmModuleSlots_Install()
{
    ReadModule* game = reinterpret_cast<ReadModule*>(decomp_va(kEmReadModuleVa));
    for (unsigned int i = 0; i < kGameModuleSlots; i++) {
        if (game[i].pArc != NULL || game[i].pModule != NULL) {
            decomp_log("Decomp mod: em_module_slots refused, a module is already loaded (slot, archive, module)",
                       i, (unsigned int) game[i].pArc, (unsigned int) game[i].pModule);
            return false;
        }
    }
    for (unsigned int i = 0; i < kModModuleSlots; i++) {
        memclr_asm(&g_moduleSlots[i], sizeof(ReadModule));
    }
    g_moreSlots = true;
    decomp_log("Decomp mod: em_module_slots installed (slots, table, game table)",
               kModModuleSlots, (unsigned int) g_moduleSlots, decomp_va(kEmReadModuleVa));
    return true;
}
