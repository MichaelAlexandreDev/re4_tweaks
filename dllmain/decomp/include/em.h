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

// One sequence key (GameCube model.h MotionSeqKey; 4 bytes on PC too).
struct MotionSeqKey {
    u16 frame;    // 0x00
    u8 Se;        // 0x02  sound number + 1
    u8 Free;      // 0x03  free bits (object / player event bits)
};

// MOTION_INFO of the Steam build (0xD0 bytes; re4_tweaks SDK model.h).
struct MotionWork {
    s32 fcvDataPtr;           // 0x00
    u32* pHermite_data;       // 0x04
    u16 Key_hist[4][3];       // 0x08
    f32 Mot_frame_max;        // 0x20
    f32 Mot_frame;            // 0x24
    f32 Mot_frame_sav;        // 0x28
    f32 Mot_frame_old;        // 0x2C
    u8 Joint_num;             // 0x30
    u8* pJoint_no;            // 0x34
    void* pJoint_kind;        // 0x38
    u16 Null_pos;             // 0x3C
    u16 Null_rot;             // 0x3E
    u16 Mot_attr;             // 0x40
    u16 Mot_state;            // 0x42
    u32 Mot_flag;             // 0x44
    Vec Pos_move_old;         // 0x48
    Vec Ang;                  // 0x54
    Vec Ang_old;              // 0x60
    Vec Ang_dist;             // 0x6C
    Vec Pos;                  // 0x78
    Vec Pos_old;              // 0x84
    Vec Pos_dist;             // 0x90
    Vec Pos_world;            // 0x9C
    void* pSeq_top;           // 0xA8
    MotionSeqKey Seq;         // 0xAC
    MotionSeqKey Seq_old;     // 0xB0
    MotionSeqKey Seq_old2;    // 0xB4
    f32 Seq_frame;            // 0xB8
    u16 Seq_frame_num;        // 0xBC
    f32 Seq_speed;            // 0xC0
    u8 Hokan_frame;           // 0xC4
    u8 Hokan_cnt;             // 0xC5
    u8 pad_C6[2];
    f32 Brate;                // 0xC8
    void* attachCamera;       // 0xCC
};
typedef char decomp_check_MotionWork_size[sizeof(MotionWork) == 0xD0 ? 1 : -1];

// cDmgInfo of the Steam build (0x1C bytes; re4_tweaks SDK em.h).
struct cDmgInfo {
    f32 m_Dist0;              // 0x00
    u8 m_Flag;                // 0x04
    u8 m_Timer;               // 0x05
    u8 m_Wep;                 // 0x06
    u8 pad_07;
    Vec m_PosFrom;            // 0x08
    f32 m_Dist;               // 0x14
    void* m_pDamageYarare;    // 0x18
};

class cParts;

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
    u8 pad_008[0x70 - 0x8];
    Vec world;            // 0x070  world position (cCoord)
    Vec world_old;        // 0x07C
    Vec world_old2;       // 0x088
    Vec pos;              // 0x094
    Vec ang;              // 0x0A0
    Vec scale;            // 0x0AC
    Vec r_scale;          // 0x0B8
    u8 pad_0C4[0xFC - 0xC4];
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

    DECOMP_METHOD(cModel*, cModel, getPartsPtr, (int no), (no), 0x00402BE9);
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
    cDmgInfo dmg;         // 0x328
    u8 pad_344[0x380 - 0x344];
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
DECOMP_OFFSET_CHECK(cModel, world, 0x70);
DECOMP_OFFSET_CHECK(cEm, dmg, 0x328);

// Player and partner (Ashley); layouts beyond cEm are not needed yet.
class cPlayer : public cEm {
};
DECOMP_GLOBAL(cPlayer*, pPL, 0x00C57054);
DECOMP_GLOBAL(cPlayer*, pSUB, 0x00C57060);

// The routine bytes written as one dword (GameCube em.h).
#define EmRoutineSetW(em, r0, r1, r2, r3) \
    (*(u32*) &(em)->r_no_0 = ((u32) (r3) << 24) | ((u32) (r2) << 16) | ((u32) (r1) << 8) | (u32) (r0))

// The four routine bytes; MSVC merges the stores into one dword.
static inline void EmRoutineSet(cEm* em, int r0, int r1, int r2, int r3)
{
    em->r_no_0 = r0;
    em->r_no_1 = r1;
    em->r_no_2 = r2;
    em->r_no_3 = r3;
}

DECOMP_FUNC(u8, Rnd, (), 0x0040173A);
DECOMP_FUNC(u32, MotionMove, (cModel* m, void* camera), 0x00406BF9);
// PC: the start frame became f32 and an 8th argument (0 at every em10 call)
// was added; the default keeps the GameCube calls unchanged.
DECOMP_FUNC(int, MotionSetCore8, (cModel* m, MotionWork* w, void* data, void* seq, int hokan, int flags, f32 frame, int a8),
            0x0040B14A);
static inline int MotionSetCore(cModel* m, MotionWork* w, void* data, void* seq, int hokan, int flags, f32 frame,
                                int a8 = 0)
{
    return MotionSetCore8(m, w, data, seq, hokan, flags, frame, a8);
}
DECOMP_FUNC(int, EstSet, (cModel* a, int b, Vec* pos, Vec* rot, int c, u8 d, u16 e, u8 f, void* g, void* h),
            0x004014C4);

#define EFF_CORE 0
#define ESP_CORE_KIND_NONE 0
#define EFF_EM10 0x10

#endif
