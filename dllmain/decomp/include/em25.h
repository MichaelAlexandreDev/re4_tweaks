#ifndef EM25_H
#define EM25_H

#include "bind.h"
#include "em.h"
#include "em10.h"
#include "etc_model.h"
#include "global.h"
#include "math_sub.h"
#include "snd.h"
#include "types.h"

// Las Plagas parasite (class cEm25), Steam bio4.exe. Guide:
// references/re4-gamecube/src/em25/em25.cpp and include/em25.h.
//
// The work overlays cEm25 from 0x408 (GameCube 0x3E0): the offsets inside
// the work are the GameCube ones up to Eff_wait2, every frame counter became
// f32 (the PC 60 fps port), and one byte was added before Die_ck (0x3AC,
// cEm25 0x7B4: an atari flag 4 countdown in cEm25::move).

struct YARARE_INFO {
    Vec ofs;              // 0x00
    Vec cross;            // 0x0C
    f32 r;                // 0x18
    f32 h;                // 0x1C
    f32 extent;           // 0x20
    u16 flag;             // 0x24
    s16 parts_no;         // 0x26
    f32 len;              // 0x28
    f32 c_dis;            // 0x2C
    YARARE_INFO* pList;   // 0x30
};
typedef char decomp_check_YARARE_INFO_size[sizeof(YARARE_INFO) == 0x34 ? 1 : -1];

struct EmAtkInfo {
    f32 range;   // 0x00
    int type;    // 0x04  PL_DM_TYPE
    u16 dmg;     // 0x08
    u16 flag;    // 0x0A
    u16 dm_cnt;  // 0x0C
    u16 x0E;     // 0x0E
};

class cObj16;

struct Em25Work {
    u32 Be_flg;            // 0x000 (0x408)
    f32 Timer;             // 0x004 (0x40C)
    f32 Timer2;            // 0x008 (0x410)
    u32 TmpU32;            // 0x00C (0x414)  bite SE handle
    YARARE_INFO hit[3];    // 0x010 (0x418)
    u8 pad_AC[0x218 - 0xAC];
    f32 Pl_dir;            // 0x218 (0x620)
    f32 Pl_rot;            // 0x21C (0x624)
    u8 pad_220[8];
    f32 Go_dir;            // 0x228 (0x630)
    f32 Go_rot;            // 0x22C (0x634)
    f32 L_go;              // 0x230 (0x638)
    Vec Pl_pos;            // 0x234 (0x63C)
    u8 pad_240[0xC];
    Vec Go_pos;            // 0x24C (0x654)
    cEm* pEm;              // 0x258 (0x660)
    cEm* pEm_oya;          // 0x25C (0x664)
    int oya_parts;         // 0x260 (0x668)
    u8 pad_264[0x378 - 0x264];
    cObj16* pParasite[3];  // 0x378 (0x780)
    u32 Wm_no;             // 0x384 (0x78C)
    f32 Alive_timer;       // 0x388 (0x790)
    f32 Compress_y;        // 0x38C (0x794)
    int Mode;              // 0x390 (0x798)
    f32 Atk_wait;          // 0x394 (0x79C)
    f32 Fire_timer;        // 0x398 (0x7A0)
    u8 EffKindId;          // 0x39C (0x7A4)
    u8 pad_39D[3];
    f32 Se_breath_wait;    // 0x3A0 (0x7A8)
    f32 Eff_wait1;         // 0x3A4 (0x7AC)
    f32 Eff_wait2;         // 0x3A8 (0x7B0)
    u8 Atari_wait;         // 0x3AC (0x7B4)  PC addition: frames until atari flag 4 is cleared
    u8 Die_ck;             // 0x3AD (0x7B5)
    u8 Atk_ck;             // 0x3AE (0x7B6)
    u8 Atk_enable;         // 0x3AF (0x7B7)
};

class cEm25 : public cEm {
public:
    virtual void move();
    virtual int ckParent();
    virtual void setParent(cEm* pMod, int parts, Vec* pos0, Vec* ang0);
    virtual void setWait();
    virtual void setAtk();
    virtual int ckAtkHit();
    virtual void setPoison();
    virtual int ckAtkEnd();
    virtual void setDie();
    virtual int ckDie();
    virtual void setGoOut(int flag);
    virtual void setHide();
    virtual int ckHide();
    virtual void setBirth(Vec* pos, f32 ang);
    virtual int ckAtkEnable();
    virtual void setDamage();
    virtual int ckLock();

    // cModel methods (not in em.h yet), called on the parasite.
    DECOMP_METHOD(int, cEm25, modelInit, (void* bin, void* tpl), (bin, tpl), 0x0065EBA0);
    DECOMP_METHOD0(void, cEm25, partsWorldCalc, 0x0065BED0);
    DECOMP_METHOD(void, cEm25, setPos, (Vec* pos), (pos), 0x0065C3F0);

    u8 free[0xE08 - 0x408];   // 0x408  Em25Work (class size not confirmed on PC)
};

#define EM25_WK(em) ((Em25Work*) (((cEm25*) (em))->free))

DECOMP_OFFSET_CHECK(Em25Work, Pl_dir, 0x218);
DECOMP_OFFSET_CHECK(Em25Work, pEm_oya, 0x25C);
DECOMP_OFFSET_CHECK(Em25Work, pParasite, 0x378);
DECOMP_OFFSET_CHECK(Em25Work, Eff_wait2, 0x3A8);
DECOMP_OFFSET_CHECK(Em25Work, Atk_enable, 0x3AF);

typedef void (*Em25Func)(cEm25*);

// Routine tables and data of em25.cpp (game data, bound by address).
DECOMP_GLOBAL_ARRAY(Em25Func, Em25_R0_move_tbl, 5, 0x00BF4E28);
DECOMP_GLOBAL_ARRAY(Em25Func, Em25_R2_move_tbl, 5, 0x00BF4E3C);
DECOMP_GLOBAL_ARRAY(Em25Func, Em25_R1_move_tbl, 24, 0x00BF4E50);
DECOMP_GLOBAL_ARRAY(Em25Func, Em25_R3_move_tbl, 3, 0x00BF4EB0);
DECOMP_GLOBAL_ARRAY(u16, em25_flip_tbl, 80, 0x00BF4EC0);
DECOMP_GLOBAL_ARRAY(EmAtkInfo, em25_atk_tbl, 2, 0x00BF4F60);
DECOMP_GLOBAL_ARRAY(EmAtkInfo, em25_poison_atk, 1, 0x00BF4F80);

// Frame counter helpers of the PC port (__thiscall on the f32 counter,
// folded by identical-code merging).
class F32Timer {
public:
    f32 t;
    // *this > 0
    DECOMP_METHOD0(bool, F32Timer, Active, 0x004078BF);
    // the counter is within one frame of n
    DECOMP_METHOD(bool, F32Timer, Eq, (int n), (n), 0x0040E1E7);
};
#define F32T(x) ((F32Timer*) &(x))

// cLightInfo of a model (cModel 0x164).
class cLightInfoM {
public:
    DECOMP_METHOD(void, cLightInfoM, init2, (int a, int b, const Vec* ofs, const Vec* size, int c),
                  (a, b, ofs, size, c), 0x0070E660);
};
#define LIGHT_INFO(m) ((cLightInfoM*) ((u8*) (m) + 0x164))
#define LIGHT_ENABLE_MASK(m) (*(u8*) ((u8*) (m) + 0x1B4))

// cModelInfo list of a model (PC offsets from em25_R1_Hide).
struct ModelInfoList {
    u8 pad_00[0x14];
    ModelInfoList* pList;   // 0x14
    u8 pad_18[0x8C - 0x18];
    u8 color[3];            // 0x8C
};

class cObj16M : public cObj16 {
public:
    DECOMP_METHOD0(void, cObj16M, clearLostWait, 0x00721920);
};

// cPlayer virtuals up to setHead (vtable slot 17, 0x44).
class cPlayerV : public cEm {
public:
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void setHead(int no);
};

// cEm::cEm (out of line; the cEm25 constructor is inlined into Em25Init).
class cEmC : public cEm {
public:
    DECOMP_METHOD0(void, cEmC, ctor, 0x0040295A);
};
DECOMP_GLOBAL(void*, cEm25_vftable, 0x00B10EDC);

DECOMP_FUNC(void, PSVECNormalize, (const Vec* src, Vec* dst), 0x00975FE0);
DECOMP_FUNC(u8, EspPullCoreKind, (), 0x005ADBD0);


// PC: the status flag test is a function (cdecl, bit number on the stack, reads pG->Status_flg).
DECOMP_FUNC(u32, StaFlagChkFn, (int n), 0x00428BE0);

// PC: VECNormalize is inlined (a zero vector stays zero).
static inline void Em25VecNormalize(Vec* src, Vec* dst)
{
    if (src->x == 0.0f && src->y == 0.0f && src->z == 0.0f) {
        dst->x = dst->y = dst->z = 0.0f;
        return;
    }
    PSVECNormalize(src, dst);
}

// Absolute value as the PC code computes it (compare and negate, no fabs).
static inline f32 Em25Abs(f32 x)
{
    if (x < 0.0f) {
        x = -x;
    }
    return x;
}
// Same, with two returns (the compiler then merges the paths through a temporary, as em25RouteCk does).
static inline f32 Em25AbsRet(f32 x)
{
    if (x < 0.0f) {
        return -x;
    }
    return x;
}

// The PC release keeps some debug flag checks (cEm25::move: DBG_EM_NO_DEATH).
#define DbgFlagChkPC(g, n) FlagChk(&(g)->Debug_flg, n)

// Frame counters of the PC port: a count above one frame gets the frame
// time scale added (the constant is added as a double).
static inline void Em25SetTimer(f32& t, int n)
{
    f32 f = (f32) n;
    t = f;
    if (n > 1) {
        t = f + (1.0f - pG->deltaTime);
    }
}
#define EM25_SET_TIMER(t, n) Em25SetTimer(t, n)
#define EM25_DEC_TIMER(t) \
    if ((t) > 0.0f) {     \
        (t) -= pG->deltaTime; \
    }

#endif
