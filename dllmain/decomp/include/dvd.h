#ifndef DVD_H
#define DVD_H

#include "bind.h"
#include "types.h"

// Result of a finished read (0x208 bytes; the read routine at 0x005ABD40
// fills all of it, including +0x200 and +0x204).
struct DvdReadInfo {
    u32 addr[2][32];     // 0x000  destination of every part, per header level
    u32 size[2][32];     // 0x100
    u32 mramSize;        // 0x200
    u32 aramSize;        // 0x204
};

// File table entry (index = file id); only the name is used here.
struct FileTblEntry {
    const char* name;    // 0x0
    u32 attr;            // 0x4
};

// Real length unknown; the bound only keeps indexing well-defined.
DECOMP_GLOBAL_ARRAY(FileTblEntry, FileTbl, 4096, 0x00BFC650);

class cDvd {
public:
    DECOMP_METHOD(int, cDvd, FileExistCheck, (const char* name, u32* pLength), (name, pLength), 0x00403CC9);
    DECOMP_METHOD(int, cDvd, ReadCheck, (int id, DvdReadInfo* pInfo), (id, pInfo), 0x004047E6);
    DECOMP_METHOD(int, cDvd, ReadCheckOne, (int id, u32* mram_size, u32* aram_size, void** addr),
                  (id, mram_size, aram_size, addr), 0x00403170);
};

DECOMP_GLOBAL(cDvd, Dvd, 0x00C53118);

// Queues a read; `file` and `line` come from the call site (src\read.cpp on PC).
DECOMP_FUNC(int, DvdReadN, (const char* name, void* addr, u32 a2, u32 a3, u32 a4, u32 mode,
                            const char* file, int line), 0x0040C5DB);

#endif
