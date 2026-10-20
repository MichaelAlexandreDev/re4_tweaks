#ifndef MEM_H
#define MEM_H

#include "bind.h"
#include "types.h"

DECOMP_FUNC(void*, Mem_alloc, (u32 size, const char* file, int line, int align, int area), 0x0040651E);
DECOMP_FUNC(void, Mem_free, (void* p), 0x00404606);
DECOMP_FUNC(void, Debug_free, (void* p), 0x00404070);
DECOMP_FUNC(void, memclr_asm, (void* p, u32 size), 0x0040C559);

#endif
