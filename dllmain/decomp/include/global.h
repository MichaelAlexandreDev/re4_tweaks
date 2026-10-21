#ifndef GLOBAL_H
#define GLOBAL_H

#include "bind.h"
#include "types.h"

// EMI points of the room (enemy info: hide / goto / route spots); Steam
// layout from re4_tweaks SDK EMINFO_WK / EMINFO_DATA, GameCube names.
struct EmiEntry {
    u8 type;              // 0x00
    u8 sub;               // 0x01
    u8 state;             // 0x02
    u8 pad_3;
    Vec pos;              // 0x04
    f32 rotY;             // 0x10
    u8 pad_14[0x40 - 0x14];
};
struct EmiData {
    u32 n;                // 0x0
    u32 id;               // 0x4
    EmiEntry entry[256];  // 0x8
};

// Partial GLOBAL_WK of the Steam build. Only fields with a known offset are
// named; names follow re4_tweaks/dllmain/SDK/global.h.
struct GLOBAL_WK {
    u8 pad_0000[0x20];
    u8 Rno0;                   // 0x20  main routine; 2 = in game
    u8 pad_0021[0x54 - 0x21];
    u32 flags_SYSTEM[1];       // 0x54  SYS_PS2_ADA_GAME = 0x1000 (Separate Ways)
    u32 Disp_flg;              // 0x58  display flags
    u8 pad_005C[0x70 - 0x5C];
    f32 deltaTime;             // 0x70  frame time scale of the PC port (timers add deltaTime - 1)
    u8 pad_0074[0x4F30 - 0x74];
    EmiData* pEmi;             // 0x4F30
    u8 pad_4F34[0x4FAC - 0x4F34];
    u16 curRoomId;             // 0x4FAC  G_ROOM_ID
    u8 pad_4FAE[0x4FC8 - 0x4FAE];
    u8 pl_type;                // 0x4FC8  0 Leon, 1 Ashley, 2 Ada, 3 HUNK, 4 Krauser, 5 Wesker
    u8 pad_4FC9[0x4FCB - 0x4FC9];
    u8 subCostume;             // 0x4FCB  Ashley: 0 Normal, 1 Popstar, 2 Armor
};

#define SYS_PS2_ADA_GAME 0x1000

// As in references/re4-gamecube/include/global.h; using the helper instead of
// `f &= ~b` changes how MSVC schedules the surrounding stores.
static inline void BitOff16(u16& f, u16 b) { f &= ~b; }

DECOMP_GLOBAL(GLOBAL_WK*, pG, 0x00C06F3C);

#endif
