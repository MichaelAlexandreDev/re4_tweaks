#ifndef GLOBAL_H
#define GLOBAL_H

#include "bind.h"
#include "types.h"

// Partial GLOBAL_WK of the Steam build. Only fields with a known offset are
// named; names follow re4_tweaks/dllmain/SDK/global.h.
struct GLOBAL_WK {
    u8 pad_0000[0x54];
    u32 flags_SYSTEM[1];       // 0x54  SYS_PS2_ADA_GAME = 0x1000 (Separate Ways)
    u8 pad_0058[0x4FCB - 0x58];
    u8 subCostume;             // 0x4FCB  Ashley: 0 Normal, 1 Popstar, 2 Armor
};

#define SYS_PS2_ADA_GAME 0x1000

DECOMP_GLOBAL(GLOBAL_WK*, pG, 0x00C06F3C);

#endif
