#ifndef READ_H
#define READ_H

#include "bind.h"
#include "types.h"

// Header of a linked enemy module (REL-style DLL image).
struct DLLHeader {
    u8 pad_00[0x20];
    u32 bssSize;         // 0x20
    u8 pad_24[0x34 - 0x24];
    void (*prolog)();    // 0x34  registers EmInitFunc and the family set function
};

// One loaded enemy module (layout confirmed in
// reverse/investigations/enemy-udas-loader/report.md; 0x98 bytes as on GameCube).
class ReadModule {
public:
    u8 bss[0x80];        // 0x00
    u16 id;              // 0x80
    u16 flag;            // 0x82  bit0 module block owned, bit1 linked, bit2 data allocated, bit3 debug heap
    void* pArc;          // 0x84
    DLLHeader* pModule;  // 0x88
    u32 size;            // 0x8C
    u32 bssSize;         // 0x90
    void* pInitFunc;     // 0x94
};

// Per-character enemy file table entry (index = enemy family id).
struct ReadFile {
    u16 file;            // 0x0  data file id (FileTbl)
    u16 dll;             // 0x2  module file id, 0 = none
    u32 flag;            // 0x4
};

DECOMP_GLOBAL_ARRAY(ReadModule, EmReadModule, 4, 0x00C670A0);
DECOMP_GLOBAL_ARRAY(ReadFile, EmFileTbl, 64, 0x00C11430);
DECOMP_GLOBAL_ARRAY(ReadFile, EmFileTbl_Ada, 64, 0x00C116B0);
DECOMP_GLOBAL_ARRAY(ReadFile, EmFileTbl_Wesker, 64, 0x00C11930);
DECOMP_GLOBAL_ARRAY(ReadFile, EmFileTbl_Klauser, 64, 0x00C11BB0);
DECOMP_GLOBAL(void*, EmInitFunc, 0x00C57068);

void InitModule(ReadModule* m);
ReadModule* SearchEmModule(u8 id);
ReadModule* pullEmModule();
void* EmReadSearch(u8 id, void* data_addr, u32 malloc_size);

#endif
