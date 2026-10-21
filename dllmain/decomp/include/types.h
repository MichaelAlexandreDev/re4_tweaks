#ifndef TYPES_H
#define TYPES_H

// Fixed-width names used by the GameCube decompilation, mapped to MSVC 2010.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

struct Vec {
    f32 x, y, z;
};
typedef f32 Mtx[3][4];

#ifndef NULL
#define NULL 0
#endif

#endif
