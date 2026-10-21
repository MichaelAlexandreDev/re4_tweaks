#ifndef CTRL_H
#define CTRL_H

#include "bind.h"
#include "types.h"

// Group controllers (GameCube include/ctrl.h; PC addresses checked in the
// assembly: type 0x12 controller, s16 values at +0x13, counters at +0x2D).
class cCtrl;

enum CTRL12_ID {
    CTRL12_ID_EM1A_LIVE = 0,
    CTRL12_ID_EM27_RUSH = 1,
    CTRL12_ID_EM29_RUSH = 2,
    CTRL12_ID_EM29_LIVE = 3,
    CTRL12_ID_EM_ATK = 4,
    CTRL12_ID_EM10_PARASITE = 5,
    CTRL12_ID_EM10_ATK = 6,
    CTRL12_ID_EM10_LOST = 7,
    CTRL12_ID_EM10_THROW = 8,
    CTRL12_ID_EM10_NOT_NEAR = 9,
    CTRL12_ID_EM2B_ATK = 10,
    CTRL12_ID_BIGEFF = 11,
    CTRL12_ID_BACKSIGN = 12,
    CTRL12_ID_NUM = 13
};

enum CTRL12_ID_CNT {
    CTRL12_ID_CNT_EM1A_DIE = 0,
    CTRL12_ID_CNT_EM27_DIE = 1,
    CTRL12_ID_CNT_EM29_DIE = 2,
    CTRL12_ID_CNT_EM10_SET = 3,
    CTRL12_ID_CNT_PARASITE = 4,
    CTRL12_ID_CNT_EM2D_RESET = 5,
    CTRL12_ID_CNT_NUM = 6
};

DECOMP_FUNC(void, Ctrl12Set, (cCtrl* pCtrl, int idx, s16 val), 0x006F7BF0);
DECOMP_FUNC(int, Ctrl12Ck, (cCtrl* pCtrl, int id), 0x006F7C50);
DECOMP_FUNC(void, Ctrl12CntAdd, (cCtrl* pCtrl, int id, int add), 0x006F7C90);
DECOMP_FUNC(int, Ctrl12CntCk, (cCtrl* pCtrl, int id, u16 over), 0x006F7CC0);

#endif
