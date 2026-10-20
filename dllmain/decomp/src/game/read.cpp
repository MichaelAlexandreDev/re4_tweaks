// Enemy module table of the Steam build. Logic follows
// references/re4-gamecube/src/game/read.cpp; Steam differences are noted.

#include "global.h"
#include "read.h"

// Release helpers reached through the incremental-link table; names are
// provisional until their bodies are identified.
DECOMP_FUNC(void, DLL_Unlink, (void* module), 0x0040E24B);
DECOMP_FUNC(void, Debug_free, (void* p), 0x00404070);
DECOMP_FUNC(void, Mem_free, (void* p), 0x00404606);
DECOMP_FUNC(void, memclr_asm, (void* p, u32 size), 0x0040C559);

// FUNCTION: BIO4 0x006ADAA0
// STATUS: matching
void InitModule(ReadModule* m)
{
    if (m->pModule != NULL && (m->flag & 2)) {
        DLL_Unlink(m->pModule);
    }
    if (m->pArc != NULL && (m->flag & 4)) {
        if (m->flag & 8) {
            Debug_free(m->pArc);
        } else {
            Mem_free(m->pArc);
        }
    }
    if (m->pModule != NULL && (m->flag & 1)) {
        Mem_free(m->pModule);
    }
    memclr_asm(m, sizeof(ReadModule));
}

// Ashley's module. Steam adds the Armor costume (module 0x0C) and forces the
// normal module in Separate Ways; the GameCube only knew 3 and 5.
static u8 checkAshleyId(u8 id)
{
    if (id == 3 || id == 5 || id == 0x0C) {
        if (pG->subCostume == 0) {
            id = 3;
        } else {
            id = (pG->subCostume == 2) ? 0x0C : 5;
        }
        if (pG->flags_SYSTEM[0] & SYS_PS2_ADA_GAME) {
            id = 3;
        }
    }
    return id;
}

// FUNCTION: BIO4 0x006AD4D0
// STATUS: matching
ReadModule* SearchEmModule(u8 id)
{
    id = checkAshleyId(id);
    for (u32 i = 0; i < 4; i++) {
        if (EmReadModule[i].id == id) {
            return &EmReadModule[i];
        }
    }
    return NULL;
}

// FUNCTION: BIO4 0x006AD570
// STATUS: matching
ReadModule* pullEmModule()
{
    for (int i = 0; i < 4; i++) {
        if (EmReadModule[i].pArc == NULL) {
            return &EmReadModule[i];
        }
    }
    return NULL;
}
