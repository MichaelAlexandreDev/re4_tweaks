#ifndef MATH_SUB_H
#define MATH_SUB_H

#include "bind.h"
#include "types.h"

// Game math (GameCube game/math_sub.cpp, game/sub2.cpp and the Dolphin vector
// library, which the PC port reimplemented in C). Names and prototypes from
// references/re4-gamecube/include/math_sub_decl.h; addresses from symbols/
// (0 = not located yet: only stub code calls those).

#define PI 3.1415927f
#define PI2 6.2831855f
#define DEG2RAD 0.017453292f
#define DEG(d) ((d) * DEG2RAD)
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

DECOMP_FUNC(f32, Muku, (Vec* v0, Vec* v1, f32 dir, f32 dy), 0x006D0020);
DECOMP_FUNC(f32, Muku2, (f32 src_dir, f32 dst_dir, f32 add), 0x006D00F0);
DECOMP_FUNC(f32, LIMIT_ANGLE, (f32 x), 0x0065AB20);
DECOMP_FUNC(f32, SQRTF, (f32 x), 0x0065AAA0);
DECOMP_FUNC(f32, GetXZAngle, (Vec* v0, Vec* v1), 0x006CFE10);
DECOMP_FUNC(void, PSVECAdd, (const Vec* a, const Vec* b, Vec* ab), 0x00975E50);
DECOMP_FUNC(void, PSVECScale, (const Vec* src, Vec* dst, f32 scale), 0x009761D0);
DECOMP_FUNC(void, PSVECSubtract, (const Vec* a, const Vec* b, Vec* a_b), 0x009762E0);
DECOMP_FUNC(void, PSMTXMultVecSR, (const Mtx m, const Vec* src, Vec* dst), 0x009725E0);
DECOMP_FUNC(void, PSMTXMultVec, (const Mtx m, const Vec* src, Vec* dst), 0x00000000);
DECOMP_FUNC(u32, PSMTXInverse, (const Mtx src, Mtx inv), 0x00000000);
DECOMP_FUNC(void, PSMTXConcat, (const Mtx a, const Mtx b, Mtx ab), 0x00000000);
DECOMP_FUNC(void, PSMTXRotRad, (Mtx m, char axis, f32 rad), 0x00000000);
DECOMP_FUNC(void, TransMatrix, (Mtx m, Vec* pos), 0x00000000);
DECOMP_FUNC(void, RotMatrix, (Mtx m, Vec* vec), 0x00000000);
DECOMP_FUNC(void, ScaleMatrix, (Mtx m, Vec* scale), 0x00000000);
DECOMP_FUNC(void, PosToPos, (Vec* pos1, Vec* pos2, Vec* pos3, f32 per), 0x00000000);
DECOMP_FUNC(f32, GetDistance, (Vec* v0, Vec* v1), 0x00000000);
DECOMP_FUNC(f32, GetDistance3, (Vec* v0, Vec* v1), 0x00000000);

extern "C" double __cdecl fabs(double x);
extern "C" double __cdecl atan2(double y, double x);
#pragma intrinsic(fabs, atan2)
static inline f32 fabsf(f32 x) { return (f32) fabs(x); }
static inline f32 atan2f(f32 y, f32 x) { return (f32) atan2(y, x); }

#endif
