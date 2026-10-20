#ifndef READ_H
#define READ_H

#include "bind.h"
#include "types.h"

// One loaded enemy module (layout confirmed in
// reverse/investigations/enemy-udas-loader/report.md; 0x98 bytes as on GameCube).
class ReadModule {
public:
    u8 bss[0x80];        // 0x00
    u16 id;              // 0x80
    u16 flag;            // 0x82  bit0 module block owned, bit1 linked, bit2 data allocated, bit3 debug heap
    void* pArc;          // 0x84
    void* pModule;       // 0x88
    u32 size;            // 0x8C
    u32 bssSize;         // 0x90
    void* pInitFunc;     // 0x94
};

DECOMP_GLOBAL_ARRAY(ReadModule, EmReadModule, 4, 0x00C670A0);

void InitModule(ReadModule* m);
ReadModule* SearchEmModule(u8 id);
ReadModule* pullEmModule();

#endif
