#ifndef EM_H
#define EM_H

#include "bind.h"
#include "types.h"

// Steam layouts of the model and enemy base classes. Offsets come from the
// assembly and agree with re4_tweaks/dllmain/SDK (model.h, em.h); names
// follow the GameCube decompilation (references/re4-gamecube/include).
// Only fields used by decompiled code are named; the rest is padding.

#define DECOMP_OFFSET_CHECK(type, member, offset) \
    typedef char decomp_check_##type##_##member[((u32) & ((type*) 0)->member == (offset)) ? 1 : -1]

// Motion archive: offsets of the sub-files from the archive start.
struct PlArc {
    u32 ofs[0x400];
};
#define PL_ARC_PTR(arc, no) ((void*) ((arc)->ofs[no] + (u32) (arc)))

struct MotionData {
    u16 maxFrame;   // 0x0  frame count in the low 14 bits
};

// MOTION_INFO of the Steam build (0xD0 bytes); opaque here.
struct MotionWork {
    u8 pad_00[0xD0];
};

// cAtariInfo (0x4C bytes); the collision flags are at 0x1A.
struct cAtariInfo {
    u8 pad_00[0x1A];
    u16 m_flag;     // 0x1A  SAT_TYPE flags; 0x100 | 0x200 = collide with enemies
    u8 pad_1C[0x4C - 0x1C];
};

class cModel {
public:
    // cUnit, cCoord and cModel virtuals (re4_tweaks SDK order).
    virtual void vf00_dtor();
    virtual void beginEvent(u32 flag);
    virtual void endEvent(u32 flag);
    virtual void matUpdate();
    virtual void move();
    virtual void setNoSuspend(int onoff);

    u32 be_flag;          // 0x004
    u8 pad_008[0x94 - 0x8];
    Vec pos;              // 0x094
    Vec ang;              // 0x0A0
    Vec scale;            // 0x0AC
    u8 pad_0B8[0xFC - 0xB8];
    u8 r_no_0;            // 0x0FC
    u8 r_no_1;            // 0x0FD
    u8 r_no_2;            // 0x0FE
    u8 r_no_3;            // 0x0FF
    u8 id;                // 0x100
    u8 type;              // 0x101
    u8 pad_102[0x1D8 - 0x102];
    MotionWork Motion;    // 0x1D8
    u8 pad_2A8[0x2B4 - 0x2A8];
    cAtariInfo atari;     // 0x2B4
    u8 pad_300[0x324 - 0x300];
};

#define MOTION(m) (&(m)->Motion)

enum {
    EM_STATUS_ACTIVE = 5
};

class cEm : public cModel {
public:
    virtual void setItem(u16 item_id, u16 num, u16 item_flg, u16 auto_item_flg, s8 item_eff);
    virtual void setNoItem();
    virtual int checkThrow();

    s16 hp;               // 0x324
    s16 hp_max;           // 0x326
    u8 pad_328[0x380 - 0x328];
    PlArc* subArc;        // 0x380
    PlArc* subArc2;       // 0x384
    u8 pad_388[0x395 - 0x388];
    u8 set;               // 0x395  ESL set: chooses the initial role
    u8 pad_396[0x3A0 - 0x396];
    u8 emset_no;          // 0x3A0  ESL row index (0xFF when not from a list)
    u8 pad_3A1[0x3C9 - 0x3A1];
    u8 RckPad0;           // 0x3C9  PC: set to 1 at init for every type but 6
    u8 pad_3CA[0x3D0 - 0x3CA];
    u32 flag;             // 0x3D0  ESL flags
    f32 Guard_r;          // 0x3D4
    u8 Character;         // 0x3D8
    u8 pad_3D9[0x408 - 0x3D9];

    DECOMP_METHOD(void, cEm, setStatus, (int id), (id), 0x0040BCE9);
    DECOMP_METHOD(void, cEm, clearStatus, (int id), (id), 0x0040BDDE);
};

DECOMP_OFFSET_CHECK(cModel, be_flag, 0x4);
DECOMP_OFFSET_CHECK(cModel, r_no_0, 0xFC);
DECOMP_OFFSET_CHECK(cModel, Motion, 0x1D8);
DECOMP_OFFSET_CHECK(cModel, atari, 0x2B4);
DECOMP_OFFSET_CHECK(cEm, hp, 0x324);
DECOMP_OFFSET_CHECK(cEm, set, 0x395);
DECOMP_OFFSET_CHECK(cEm, flag, 0x3D0);

// The four routine bytes; MSVC merges the stores into one dword.
static inline void EmRoutineSet(cEm* em, int r0, int r1, int r2, int r3)
{
    em->r_no_0 = r0;
    em->r_no_1 = r1;
    em->r_no_2 = r2;
    em->r_no_3 = r3;
}

DECOMP_FUNC(u8, Rnd, (), 0x0040173A);
DECOMP_FUNC(int, MotionSetCore, (cModel* m, MotionWork* w, void* data, void* seq, int hokan, int flags, f32 frame, int a8),
            0x0040B14A);
DECOMP_FUNC(int, EstSet, (cModel* m, int a1, int a2, int a3, int kind, int no, int a6, int kind_id, cModel* m2, int a9),
            0x004014C4);

#define EFF_CORE 0
#define EFF_EM10 0x10

#endif
