// Mod: per-family Ganado configuration (harness only, off by default).
// Investigation: reverse/investigations/em-family-dispatch/report.md
//
// Every family module's prolog overwrites the single global Em10SetFunc, so
// with two families loaded the last one configures every cEm10. The hooks in
// setEmModule record the routine each module's prolog wrote, per module slot,
// and em10_R0_Init (the only reader of Em10SetFunc) is entered through
// Em10_R0_move_tbl[0] with the routine of the slot whose archive the enemy
// was built from (cEm10 +0x380).

#include "read.h"
#include "types.h"

typedef void (__cdecl* Em10Func)(void* em);

DECOMP_GLOBAL(Em10Func, Em10SetFunc, 0x00C5017C);
DECOMP_GLOBAL_ARRAY(Em10Func, Em10_R0_move_tbl, 5, 0x00BF39B0);

static const unsigned int kEm10R0InitVa = 0x00461580;
static const unsigned int kEmArchiveOffset = 0x380;

static bool g_familyDispatch = false;
static Em10Func g_savedSetFunc = NULL;
static Em10Func g_slotSetFunc[4] = {};
static Em10Func g_em10R0Init = NULL;
static bool g_slotLogged[4] = {};

static int emModuleSlot(const ReadModule* m)
{
    for (int i = 0; i < 4; i++) {
        if (m == &EmReadModule[i]) {
            return i;
        }
    }
    return -1;
}

void decomp_hook_EmModulePrologBefore(ReadModule* m)
{
    if (!g_familyDispatch || emModuleSlot(m) < 0) {
        return;
    }
    g_savedSetFunc = Em10SetFunc;
    Em10SetFunc = NULL;
}

void decomp_hook_EmModulePrologAfter(ReadModule* m)
{
    int slot = emModuleSlot(m);
    if (!g_familyDispatch || slot < 0) {
        return;
    }
    if (Em10SetFunc != NULL) {
        g_slotSetFunc[slot] = Em10SetFunc;
        g_slotLogged[slot] = false;
        decomp_log("Decomp mod: em family routine recorded (slot, family id, routine)",
                   slot, m->id, (unsigned int) Em10SetFunc);
    } else {
        // Not a cEm10 family module: keep the global as the game left it.
        Em10SetFunc = g_savedSetFunc;
        g_slotSetFunc[slot] = NULL;
    }
}

void decomp_hook_EmModuleRelease(ReadModule* m)
{
    int slot = emModuleSlot(m);
    if (slot >= 0) {
        g_slotSetFunc[slot] = NULL;
    }
}

static void __cdecl em10_R0_Init_dispatch(void* em)
{
    void* archive = *(void**) ((u8*) em + kEmArchiveOffset);
    for (int i = 0; i < 4; i++) {
        if (EmReadModule[i].pArc == archive && g_slotSetFunc[i] != NULL) {
            if (Em10SetFunc != g_slotSetFunc[i] && !g_slotLogged[i]) {
                g_slotLogged[i] = true;
                decomp_log("Decomp mod: em family routine switched (slot, family id, routine)",
                           i, EmReadModule[i].id, (unsigned int) g_slotSetFunc[i]);
            }
            Em10SetFunc = g_slotSetFunc[i];
            break;
        }
    }
    g_em10R0Init(em);
}

// Called by the harness after the loader functions are replaced.
bool EmFamilyDispatch_Install()
{
    Em10Func expected = reinterpret_cast<Em10Func>(decomp_va(kEm10R0InitVa));
    if (Em10_R0_move_tbl[0] != expected) {
        decomp_log("Decomp mod: em_family_dispatch refused, Em10_R0_move_tbl[0] is not em10_R0_Init (found, expected, 0)",
                   (unsigned int) Em10_R0_move_tbl[0], (unsigned int) expected, 0);
        return false;
    }
    g_em10R0Init = expected;
    g_familyDispatch = true;
    Em10_R0_move_tbl[0] = &em10_R0_Init_dispatch;
    decomp_log("Decomp mod: em_family_dispatch installed (table, original, dispatch)",
               decomp_va(0x00BF39B0), (unsigned int) expected, (unsigned int) &em10_R0_Init_dispatch);
    return true;
}
