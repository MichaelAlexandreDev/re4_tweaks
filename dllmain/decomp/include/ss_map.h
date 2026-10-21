#ifndef DECOMP_SS_MAP_H
#define DECOMP_SS_MAP_H

#include "types.h"

// Verified Steam offsets for the first loader method only.
struct SUB_SCREEN {
    u8 pad_00[0x2C];
    s32 open_flag;  // 0x2C
};

class SsMapInit {
public:
    u8 pad_00[0x10];
    s32 state;  // 0x10
    void init(SUB_SCREEN* wk);
};

// Only the field used by the ported init methods is represented here.
// Steam x86 assembly writes byte [ECX+0x10] in both methods.
class MapZoomIn {
public:
    u8 pad_00[0x10];
    s8 count;  // 0x10
    void init(SUB_SCREEN* wk);
};

class MapZoomOut {
public:
    u8 pad_00[0x10];
    s8 count;  // 0x10
    void init(SUB_SCREEN* wk);
};

#endif
