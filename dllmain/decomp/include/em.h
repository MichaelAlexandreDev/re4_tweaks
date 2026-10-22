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

class cModelData;

// Model info of a model or part (game/model.cpp). Partial Steam layout;
// model_addr confirmed by em10HandSet (0x0042E0B0).
class cModelInfo {
public:
    u8 pad_00[0x0C];
    cModelData* model_addr;  // 0x0C
};

// Model info pool (game/model.cpp `ModInfoMgr`).
class cModInfoMgr {
public:
    DECOMP_METHOD(cModelInfo*, cModInfoMgr, create, (void* bin, void* tpl), (bin, tpl), 0x0065E780);
};
DECOMP_GLOBAL(cModInfoMgr, ModInfoMgr, 0x00C07150);

class cModel {
public:
    // cUnit, cCoord and cModel virtuals (re4_tweaks SDK order).
    virtual void vf00_dtor();
    virtual void beginEvent(u32 flag);
    virtual void endEvent(u32 flag);
    virtual void matUpdate();
    virtual void move();
    virtual void setNoSuspend(int onoff);

    // cUnit
    u32 be_flag;          // 0x004
    cModel* pNext;        // 0x008
    // cCoord (same offsets as the GameCube)
    Mtx mat;              // 0x00C  world matrix
    Mtx l_mat;            // 0x03C  local matrix
    cModel* pParent;      // 0x06C
    Vec world;            // 0x070
    Vec world_old;        // 0x07C
    Vec world_old2;       // 0x088
    Vec pos;              // 0x094
    Vec ang;              // 0x0A0
    Vec scale;            // 0x0AC
    Vec r_scale;          // 0x0B8
    Mtx prevMat;          // 0x0C4
    // cModel
    cModel* pParts;       // 0x0F4  child parts list
    u32 guid;             // 0x0F8
    u8 r_no_0;            // 0x0FC
    u8 r_no_1;            // 0x0FD
    u8 r_no_2;            // 0x0FE
    u8 r_no_3;            // 0x0FF
    u8 id;                // 0x100
    u8 type;              // 0x101
    u8 nParts;            // 0x102
    u8 alpha_omit;        // 0x103
    Vec speed;            // 0x104
    Vec pos_old;          // 0x110
    Vec Wall_norm;        // 0x11C
    Vec* pFloor_norm;     // 0x128
    u8 z_mode;            // 0x12C
    u8 TevScaleGroup;     // 0x12D
    u8 kindid;            // 0x12E
    u8 ot_type;           // 0x12F
    void* pCldShMd;       // 0x130
    u8 Shd_color;         // 0x134
    u8 CullMode;          // 0x135
    u8 Shader_type;       // 0x136
    u8 Refract_pow;       // 0x137
    u8 Refract_ratio;     // 0x138
    u8 AddAmb_r;          // 0x139
    u8 AddAmb_g;          // 0x13A
    u8 AddAmb_b;          // 0x13B
    int Fix_parts;        // 0x13C
    Vec Fix_pos;          // 0x140
    u8 invisible_trg;     // 0x14C
    u8 invisible_old;     // 0x14D
    u8 invisible_mode;    // 0x14E
    u8 invisible_busy;    // 0x14F
    u32 invisible_timer;  // 0x150
    f32 invisible_factor; // 0x154
    f32 invisible_factor2; // 0x158
    cModelInfo* pModelInfo;       // 0x15C
    cModelInfo* pShadowModelInfo; // 0x160
    u8 LightInfo[0x1D8 - 0x164];  // 0x164  cLightInfo
    MotionWork Motion;    // 0x1D8
    MotionWork* pMotionB; // 0x2A8
    s16* pXFlip;          // 0x2AC
    u32* pDblJnt;         // 0x2B0
    cAtariInfo atari;     // 0x2B4
    Vec* inscreen_pos;    // 0x300
    u32 pPath;            // 0x304
    void* pFsdTbl;        // 0x308
    u8 litArea[0x10];     // 0x30C  EmLightArea
    void* pTexChg;        // 0x31C
    u8 field_320[4];      // 0x320

    DECOMP_METHOD(cModel*, cModel, getPartsPtr, (int no), (no), 0x00402BE9);
    // Replaces the info using `addr` by `info` (1 when found).
    DECOMP_METHOD(int, cModel, swapModelInfo, (cModelData* addr, cModelInfo* info), (addr, info), 0x0065CA80);
    DECOMP_METHOD(void, cModel, addModel, (cModelInfo* info), (info), 0x0065C5B0);
};

#define MOTION(m) (&(m)->Motion)

enum {
    EM_STATUS_ATTACKING = 0,
    EM_STATUS_LOCKOFF = 1,
    EM_STATUS_MIST_ON = 2,
    EM_STATUS_IK_OFF = 3,
    EM_STATUS_ALERT = 4,
    EM_STATUS_ACTIVE = 5,
    EM_STATUS_DOGCK = 6,
    EM_STATUS_DOGATK = 7,
    EM_STATUS_ITEMSET = 8,
    EM_STATUS_LOOK_ME = 9,
    EM_STATUS_DONT_FIRE = 10,
    EM_STATUS_ASHLEY_NO_HELP = 11
};

class cEm : public cModel {
public:
    virtual void setItem(u16 item_id, u16 num, u16 item_flg, u16 auto_item_flg, s8 item_eff);
    virtual void setNoItem();
    virtual int checkThrow();

    s16 hp;               // 0x324
    s16 hp_max;           // 0x326
    cDmgInfo dmg;         // 0x328
    u8 hitInfo[0x378 - 0x344];  // 0x344  YARARE_INFO
    f32 l_pl;             // 0x378  squared distance to the player
    f32 l_sub;            // 0x37C  squared distance to the partner
    PlArc* subArc;        // 0x380
    PlArc* subArc2;       // 0x384
    Vec lockOfs;          // 0x388
    u8 lockParts;         // 0x394
    u8 set;               // 0x395  ESL set: chooses the initial role
    u8 pad_396[2];
    void (*pScenario)(cEm*);  // 0x398
    u8 pad_39C[4];
    u8 emset_no;          // 0x3A0  ESL row index (0xFF when not from a list)
    u8 x3A1;              // 0x3A1
    u8 pad_3A2[2];
    Vec Catch_pos_adj;    // 0x3A4
    Vec Catch_at_adj;     // 0x3B0
    f32 Catch_dir;        // 0x3BC
    cEm* pEmCatch;        // 0x3C0
    u8 RckStat;           // 0x3C4
    s8 RckMy;             // 0x3C5
    s8 RckTo;             // 0x3C6
    s8 RckNear;           // 0x3C7
    s8 RckRno;            // 0x3C8
    u8 RckPad0;           // 0x3C9  PC: set to 1 at init for every type but 6
    u8 pad_3CA[2];
    u32 status;           // 0x3CC  setStatus / clearStatus bits
    u32 flag;             // 0x3D0  ESL flags
    f32 Guard_r;          // 0x3D4
    u8 Character;         // 0x3D8
    u8 itemFlag;          // 0x3D9
    u8 pad_3DA[4];
    u16 Item_id;          // 0x3DE
    u16 Item_num;         // 0x3E0
    u16 Item_flg;         // 0x3E2
    u16 Auto_item_flg;    // 0x3E4
    u8 pad_3E6[2];
    u32 flags_3E8;        // 0x3E8  PC addition
    Vec x3EC;             // 0x3EC  PC addition
    Vec lockOfs1;         // 0x3F8  PC addition (second lock-on point)
    int lockParts1;       // 0x404  PC addition

    DECOMP_METHOD(void, cEm, setStatus, (int id), (id), 0x0040BCE9);
    DECOMP_METHOD(void, cEm, clearStatus, (int id), (id), 0x0040BDDE);
    int checkStatus(int stat) { return (status >> stat) & 1; }
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
DECOMP_OFFSET_CHECK(cEm, l_pl, 0x378);
DECOMP_OFFSET_CHECK(cEm, status, 0x3CC);
DECOMP_OFFSET_CHECK(cModel, prevMat, 0xC4);
DECOMP_OFFSET_CHECK(cModel, pModelInfo, 0x15C);
DECOMP_OFFSET_CHECK(cModel, pMotionB, 0x2A8);
typedef char decomp_check_cModel_size[sizeof(cModel) == 0x324 ? 1 : -1];
typedef char decomp_check_cEm_size[sizeof(cEm) == 0x408 ? 1 : -1];

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

// Effect owners verified on PC. The GameCube ESP_OWNER numbering differs
// from the PC beyond the first entries (the EtcModel port found EFF_ET* =
// GameCube + 4), so only checked values are defined here.
enum {
    EFF_CORE = 0,
    EFF_ROOM = 1,
    EFF_EM10 = 0x10
};
#define ESP_CORE_KIND_NONE 0

DECOMP_FUNC(void, GameAddPoint, (int type), 0x0063AFA0);
#define EFF_EM10 0x10

// --- added by the em25 port ---
// Enemy helpers of game/em_sub.cpp, em_set.cpp, at_mod.cpp, pl_sub.cpp and
// the managers (GameCube prototypes; PC addresses read in the em25 calls).
struct YARARE_INFO;
struct EmAtkInfo;
class cObj;
class cObj16;
struct VibDataTbl;

static inline int EmDeadCk(cEm* em) { return em->dmg.m_Flag || em->dmg.m_Timer; }

DECOMP_FUNC(int, LifeDownSet, (cEm* pEm, int dm_val, int rnd), 0x005BC3E0);
DECOMP_FUNC(int, LifeDownSet2, (cEm* pEm, int dm_val, int rnd, int flag), 0x005B5AF0);
DECOMP_FUNC(void, EmSetDieCnt, (cEm* pEm), 0x005B20B0);
DECOMP_FUNC(void, EmSetDropItem, (cEm* pEm), 0x005BDFA0);
DECOMP_FUNC(void, EmCatchPLSet, (cEm* em, f32 pl_dir, u32 mode, f32 x, f32 y, f32 z, void (*ft)(cPlayer*)),
            0x005B6370);
DECOMP_FUNC(int, EmCatchMotionMove, (cEm* pEm, f32 rot_rate, f32 pos_rate), 0x005B6C30);
DECOMP_FUNC(void, EmDmBloodSet2, (cEm* pEm, u32 est_id, u32 type, u32 mode, u16 esp_core_flg, u32 core_kind),
            0x005B3A20);
DECOMP_FUNC(int, EmGetDmPos, (cEm* pEm, Vec* pPos, Vec* pAng), 0x005B2C70);
DECOMP_FUNC(void, EmPlBloodSet2, (cModel* pEm, Vec* pos, u32 a, u8 eff_id, u8 type), 0x005B3E10);
DECOMP_FUNC(void, EmSubBloodSet, (cEm* pEm, Vec* pPos, u32 type, u8 eff_id, u8 est_id), 0x005B3F30);
DECOMP_FUNC(int, EmAtkHitCk, (EmAtkInfo* info, Vec* pPos, Vec* pPosOld, int noSub), 0x005BDE20);
DECOMP_FUNC(void, EmAtCheck, (cModel* pMod), 0x0058DCA0);
DECOMP_FUNC(int, ObjHitCheck, (Vec* ret_cross, Vec* ret_norm, Vec* pos0, Vec* pos1, u32 flag), 0x0058DA70);
DECOMP_FUNC(int, RouteCkToPos, (cEm* em, Vec* target, Vec* out, int a, int b), 0x006B2950);
DECOMP_FUNC(void, QuakeExec, (u8 id, u16 delay, s16 time, f32 power, u8 axis), 0x006AA9B0);
DECOMP_FUNC(void, VibSetData, (VibDataTbl* t, u32 no, u32 type), 0x00965B00);
DECOMP_FUNC(void, PlSetFace, (int type), 0x00682130);
DECOMP_FUNC(void, PlGachaInit, (), 0x00680D60);
DECOMP_FUNC(void, PlGachaMove, (), 0x00680D80);
DECOMP_FUNC(int, PlGachaGet, (), 0x00680FA0);
DECOMP_FUNC(void, EndPlDamage, (), 0x006811A0);
DECOMP_FUNC(void, PlSetDamageSe, (int se_no), 0x00680FD0);
DECOMP_FUNC(void, EffectEspDelete, (int a, int b, void* c, cModel* pMod), 0x00616570);
DECOMP_FUNC(void, EffectEspgenDelete, (int Core_flg, int Core_kind, void* c), 0x00616030);
DECOMP_FUNC(void, EffectEfmDelete, (int Core_flg, int Core_kind, void* c), 0x00616040);
DECOMP_FUNC(cObj*, SetObj01, (void* bin, void* tpl, Vec* pos, Vec* rot, Vec* v, f32 a, f32 b, int c, int d),
            0x00667780);
DECOMP_FUNC(void, Obj01SetEst, (cObj* pObj, u32 eff, u32 est, u32 action, u32 eff2, u32 est2, u32 eff3, u32 est3,
                                u32 eff4, u32 est4), 0x00666E30);
DECOMP_FUNC(cObj*, SetObj08, (cModel* parent, void* bin, void* tpl, Vec* pos, Vec* rot, int flags, void* atk),
            0x0066AA50);
// PC order: life last (GameCube: obj, spd, life, grav, rad); em25SetPoison.
DECOMP_FUNC(void, SetObj08Spd, (cObj* obj, Vec* spd, f32 grav, f32 rad, int life), 0x00669BC0);
DECOMP_FUNC(void, SetObj08Est, (cObj* obj, int no0, int prm0, int no1, int prm1, int no2, int prm2, int no3,
                                int prm3, u8 flag), 0x00669C90);
DECOMP_FUNC(void, SetObj08Se, (cObj* obj, u16 blk, u16 no), 0x00669D30);
DECOMP_FUNC(void, MotSetObj16, (cObj16* obj, void* mot, int a, int b), 0x00721870);
DECOMP_FUNC(int, GetWepDmVal, (cEm* pEm, u32 wep_no, int near), 0x005B1A90);

// Damage volumes (game/dmg.cpp): DMG_TYPE returned by cDmgMgr::hitCheck.
enum DMG_TYPE {
    DMG_TYPE_NO_HIT = 0,
    DMG_TYPE_FIRE = 1,
    DMG_TYPE_GRENADE_BLAST = 2,
    DMG_TYPE_PUSH = 3,
    DMG_TYPE_FLAME = 4,
    DMG_TYPE_LAMP = 5,
    DMG_TYPE_ENV_LIGHT = 6,
    DMG_TYPE_ENV_FIRE = 7,
    DMG_TYPE_GRENADE = 8,
    DMG_TYPE_GIRL = 9
};
class cDmgMgr {
public:
    DECOMP_METHOD(int, cDmgMgr, hitCheck, (Vec* pPos, Vec* pFrom), (pPos, pFrom), 0x005A8B40);
};
DECOMP_GLOBAL(cDmgMgr, DmgMgr, 0x00BFC624);

// Scenario (SatMgr) and object (EatMgr, a cSatMgr) collision managers.
class cSatMgr {
public:
    DECOMP_METHOD(int, cSatMgr, hitCheck, (Vec* pos0, Vec* pos1, Vec* pCross, Vec* pNorm, int flag, int mask),
                  (pos0, pos1, pCross, pNorm, flag, mask), 0x00594160);
    DECOMP_METHOD(int, cSatMgr, check, (cModel* m, int flag), (m, flag), 0x00594FC0);
    DECOMP_METHOD(int, cSatMgr, checkAir, (cModel* m, int mask), (m, mask), 0x00595090);
    DECOMP_METHOD(f32, cSatMgr, getFloor, (Vec* pos, u32* ppNorm, f32 above_limit, f32 below_limit, int mask),
                  (pos, ppNorm, above_limit, below_limit, mask), 0x00593E60);
};
DECOMP_GLOBAL(cSatMgr, SatMgr, 0x00BF9A7C);
class cEatMgr : public cSatMgr {
};
DECOMP_GLOBAL(cEatMgr, EatMgr, 0x00BF9AA0);

class cEmMgr {
public:
    // Virtual in the game (cEmMgr vtable slot 5); the global object is called directly.
    DECOMP_METHOD(void, cEmMgr, destroy, (cEm* em), (em), 0x005AF570);
};
DECOMP_GLOBAL(cEmMgr, EmMgr, 0x00BFDB04);


#define EFF_EM25 0x1D
#define YAT_FLAG_ON 1
// Motion archive of a model (GameCube em.h).
#ifndef ARC
#define ARC(no) PL_ARC_PTR(em->subArc, no)
#endif
#ifndef EM_ARC
#define EM_ARC(w, no) PL_ARC_PTR((w)->subArc, no)
#endif

// Collision cylinder (cAtariInfo) and damage record methods, through
// subclasses so the em.h structs stay unchanged.
class cAtariInfoM : public cAtariInfo {
public:
    DECOMP_METHOD(void, cAtariInfoM, init,
                  (f32 ox, f32 oy, f32 oz, f32 rs, f32 ro, f32 ra, f32 h, int pno, int flags, int hokan),
                  (ox, oy, oz, rs, ro, ra, h, pno, flags, hokan), 0x00595340);
    DECOMP_METHOD(void, cAtariInfoM, set, (int mode, f32 a, f32 b), (mode, a, b), 0x005953C0);
    DECOMP_METHOD0(void, cAtariInfoM, move, 0x005955B0);
    DECOMP_METHOD(void, cAtariInfoM, setPriority, (int pri), (pri), 0x005956C0);
};
#define ATARI(m) ((cAtariInfoM*) &(m)->atari)
static inline void AtariOn(cAtariInfo* at, u16 b) { at->m_flag |= b; }
static inline void AtariOff(cAtariInfo* at, u16 mask) { at->m_flag &= mask; }

class cDmgInfoM : public cDmgInfo {
public:
    DECOMP_METHOD(void, cDmgInfoM, set, (int flag, int timer), (flag, timer), 0x005AEF70);
};
#define DMG(m) ((cDmgInfoM*) &(m)->dmg)

DECOMP_FUNC(void, YarareInit, (cEm* em, f32 x, f32 y, f32 z, f32 r, f32 h, int parts, int flag), 0x0058B570);
DECOMP_FUNC(void, YarareAdd, (cEm* em, YARARE_INFO* info, f32 x, f32 y, f32 z, f32 r, f32 h, int parts, int flag),
            0x0058B660);

// BEGIN em25 additions
// PC: returns em != *(cEm**)0x00C67594 (the enemy whose route check runs this frame).
DECOMP_FUNC(bool, RouteCkSkip, (cEm* em), 0x006B0F70);
// END em25 additions

#endif
