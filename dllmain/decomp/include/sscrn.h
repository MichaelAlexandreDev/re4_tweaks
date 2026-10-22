#ifndef DECOMP_SSCRN_H
#define DECOMP_SSCRN_H

#include "types.h"

// Sub screens (Sscrn/): the work every screen receives. One definition for
// all of them: the harness compiles the units together. Offsets verified on
// Steam by the ss_map and ss_shop ports.

class pzlPlayer;
class Merchant;
struct ShopWork;
struct SsMapWork;

// Archive of a sub screen: offsets from its own start.
struct SsArc {
    u32 ofs[0x12];
};
#define SS_ARC_PTR(arc, no) ((void*) (((SsArc*) (arc))->ofs[no] + (u32) (arc)))

// SUB_SCREEN of the Steam build (0x358 bytes; GameCube offsets differ from 0x1B0 on).
struct SUB_SCREEN {
    char filename[0x20];      // 0x000
    u8 pad_020[0x2C - 0x20];
    s32 open_flag;            // 0x02C
    s32 attr_flag;            // 0x030
    u32 close_flag;           // 0x034  0x10000: the shop closed
    u32 model_flag;           // 0x038
    u8 pad_03C[0x41 - 0x3C];
    u8 trans_off;             // 0x041
    u8 pad_042[0x44 - 0x42];
    u32 str_id;               // 0x044  merchant voice stream
    u8 (*scrn_out_func)(SUB_SCREEN*); // 0x048  exit routine, run until it returns 1
    u8 pad_04C[0x1AC - 0x4C];
    s8 stage_no;              // 0x1AC
    u8 pad_1AD;
    u16 room_no;              // 0x1AE
    u8 pad_1B0[0x1BC - 0x1B0];
    u8* pBuf;                 // 0x1BC
    u32 pFreeOffs;            // 0x1C0
    u8 pad_1C4[0x1D8 - 0x1C4];
    u8* pCmmn;                // 0x1D8  common archive (SsArc)
    u8* pSwitchDat;           // 0x1DC  screen archive being read (SsArc)
    SsArc* pPzzlDat;          // 0x1E0
    u8 pad_1E4[0x1F0 - 0x1E4];
    u8* pMapDat;              // 0x1F0
    u8* pMapObj;              // 0x1F4
    u8 pad_1F8[0x200 - 0x1F8];
    SsArc* pShopDat;          // 0x200
    u8 pad_204[0x260 - 0x204];
    s8 menu_no;               // 0x260
    u8 pad_261;
    s8 menu_old;              // 0x262
    u8 cursor_mode;           // 0x263
    u8 pad_264[0x26C - 0x264];
    Mtx pl_mat_map;           // 0x26C
    u8 pad_29C[0x2AA - 0x29C];
    s8 board_size;            // 0x2AA
    s8 board_next;            // 0x2AB
    pzlPlayer* puzzlePlayer;  // 0x2AC
    u8 back2;                 // 0x2B0  the bought piece is in hand (case placement)
    u8 pad_2B1[0x304 - 0x2B1];
    SsMapWork* map;           // 0x304
    u8 pad_308[0x310 - 0x308];
    ShopWork* shop;           // 0x310
    Merchant* merchant;       // 0x314
    u8 pad_318[0x34C - 0x318];
    u8 map_mode;              // 0x34C
    u8 pad_34D[0x358 - 0x34D];
};
typedef char decomp_check_SUB_SCREEN_size[sizeof(SUB_SCREEN) == 0x358 ? 1 : -1];

#endif
