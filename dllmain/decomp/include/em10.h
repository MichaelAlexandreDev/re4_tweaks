#ifndef EM10_H
#define EM10_H

#include "bind.h"
#include "em.h"
#include "types.h"

// Ganado enemy (class cEm10, 0xC64 bytes on Steam). The PC port moved the
// work fields of the GameCube Em10Work (references/re4-gamecube/include/
// em10.h) around and turned most frame counters into f32. Offsets agree with
// re4_tweaks/dllmain/SDK/em10.h and with em10InitRtnSet (0x0043AA90); names
// are the GameCube ones, with the SDK name noted where it differs.

class cEmWep {
public:
    DECOMP_METHOD(void, cEmWep, setEffAlways, (int kind, int no), (kind, no), 0x0040A114);
    DECOMP_METHOD(void, cEmWep, setEffFall, (int kind, int no), (kind, no), 0x0040D909);
};

class cObj12;
class cObj16 {
public:
    DECOMP_METHOD(void, cObj16, setMotData,
                  (void* m0, void* m1, void* m2, void* m3, void* m4, void* m5, void* m6, void* m7, void* m8, void* m9,
                   void* m10),
                  (m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10), 0x0040385F);
};
class cCtrl;
class cObjGondola;
class PlCloth;
class cObjGatling;
class cObj;
class cModelInfo;
class cSubChar;
class cEm10;
typedef void (*Em10Func)(cEm10*);

// CLOTH_INFO (0x60 bytes): chain / cloth simulation setup (re4_tweaks SDK cloth.h).
struct ClothAtSet {
    u8 pad_00[0x24];   // CLOTH_AT_SET: collision capsule of a chain link
};
struct ClothInfo {
    u32 Num;              // 0x00
    u8* pCloth;           // 0x04
    u8* pLeft;            // 0x08
    u8* pRight;           // 0x0C
    u8* pUpLeft;          // 0x10
    u8* pUpRight;         // 0x14
    u8* pParent;          // 0x18
    u8* pChild;           // 0x1C
    f32* pGravity;        // 0x20
    f32* pRate;           // 0x24
    f32* pMax;            // 0x28
    f32* pWindSin;        // 0x2C
    f32* pWindRate;       // 0x30
    ClothAtSet* pAtset;   // 0x34
    u32 At_num;           // 0x38
    f32 Gravity;          // 0x3C
    f32 Rate;             // 0x40
    u32 Bundle_num;       // 0x44
    f32 WindSin;          // 0x48
    f32 Stretchy;         // 0x4C
    f32 Move_rate;        // 0x50
    void** pPtbl;         // 0x54
    cEm* pEm_at;          // 0x58
    u32 Flag;             // 0x5C
};

class cObjChain {
public:
    DECOMP_METHOD(void, cObjChain, setChain, (ClothInfo* info), (info), 0x0040B9C4);
    DECOMP_METHOD(void, cObjChain, setParent, (cEm* em, int parts, Vec* pos, int a), (em, parts, pos, a), 0x0040F349);
};

// Work of the Ganado, overlaid on cEm10 from 0x408 like the GameCube Em10Work
// (offsets relative to the work; the cEm10 offset in parentheses).
struct Em10Work {
    Vec TmpV;                 // 0x000 (0x408)
    Vec St_pos;               // 0x00C (0x414)
    Vec Keep_pos;             // 0x018 (0x420)
    Vec Route_target;         // 0x024 (0x42C)
    Vec Return_ck_pos;        // 0x030 (0x438)
    Vec Pl_pos;               // 0x03C (0x444)
    Vec Sub_pos;              // 0x048 (0x450)
    Vec Go_pos;               // 0x054 (0x45C)
    Vec Floor_ang;            // 0x060 (0x468)
    Vec Spd;                  // 0x06C (0x474)
    Vec Target_pos;           // 0x078 (0x480)
    Vec Goto_pos;             // 0x084 (0x48C)
    Vec Scale;                // 0x090 (0x498)
    Vec Campos;               // 0x09C (0x4A4)
    u32 Be_flg;               // 0x0A8 (0x4B0)
    f32 Timer[5];             // 0x0AC (0x4B4)
    f32 TmpF;                 // 0x0C0 (0x4C8)
    f32 TmpF2;                // 0x0C4 (0x4CC)
    u32 TmpU32;               // 0x0C8 (0x4D0)
    void* mot[79];            // 0x0CC (0x4D4)  motion data (Em10Set / Em10WeaponSet)
    cEmWep* pWeapon;          // 0x208 (0x610)
    cEmWep* pWeapon2;         // 0x20C (0x614)
    void* pShield;            // 0x210 (0x618)
    cObj12* pCap;             // 0x214 (0x61C)
    cObj12* pGlasses;         // 0x218 (0x620)
    cObj12* pCart;            // 0x21C (0x624)
    void* pRHand;             // 0x220 (0x628)
    void* pLHand;             // 0x224 (0x62C)
    void* pHead;              // 0x228 (0x630)
    void* pRobe;              // 0x22C (0x634)
    void* pCloth;             // 0x230 (0x638)
    void* pGoods;             // 0x234 (0x63C)
    void* pSack;              // 0x238 (0x640)
    void* pHood;              // 0x23C (0x644)
    void* pWhood;             // 0x240 (0x648)
    void* pAccesory[7];       // 0x244 (0x64C)
    f32 St_dir;               // 0x260 (0x668)
    void* pLadder;            // 0x264 (0x66C)
    void* pSwitch;            // 0x268 (0x670)
    f32 Pl_dir;               // 0x26C (0x674)
    f32 Pl_rot;               // 0x270 (0x678)
    f32 Sub_dir;              // 0x274 (0x67C)
    f32 Sub_rot;              // 0x278 (0x680)
    f32 L_sub;                // 0x27C (0x684)
    f32 Go_dir;               // 0x280 (0x688)
    f32 Go_rot;               // 0x284 (0x68C)
    f32 L_go;                 // 0x288 (0x690)
    f32 L_pl_route;           // 0x28C (0x694)
    f32 L_sub_route;          // 0x290 (0x698)
    f32 L_pl_guard;           // 0x294 (0x69C)
    f32 L_guard;              // 0x298 (0x6A0)
    void* pWindow;            // 0x29C (0x6A4)
    cEm* pTruck;              // 0x2A0 (0x6A8)
    void* pGondola;           // 0x2A4 (0x6AC)
    void* pDrill;             // 0x2A8 (0x6B0)
    void* pGatling;           // 0x2AC (0x6B4)
    cCtrl* pDragon;           // 0x2B0 (0x6B8)
    cObj16* pCore;            // 0x2B4 (0x6BC)
    cEm* pTen[5];             // 0x2B8 (0x6C0)
    cEm* pParasite;           // 0x2CC (0x6D4)
    cObjChain* pGunBelt;      // 0x2D0 (0x6D8)
    cObjChain* pChain;        // 0x2D4 (0x6DC)
    void* pCtrlGroup;         // 0x2D8 (0x6E0)
    void* pCtrlSe;            // 0x2DC (0x6E4)
    u32 Seid_voice;           // 0x2E0 (0x6E8)
    u32 Seid_breath;          // 0x2E4 (0x6EC)
    u32 Seid_frame;           // 0x2E8 (0x6F0)
    u32 Seid_csaw;            // 0x2EC (0x6F4)
    f32 Neck_dir_x;           // 0x2F0 (0x6F8)
    f32 Neck_dir_y;           // 0x2F4 (0x6FC)
    f32 Finger_dir;           // 0x2F8 (0x700)
    f32 Waist_dir_y;          // 0x2FC (0x704)
    f32 Compress_y;           // 0x300 (0x708)
    f32 Target_dir;           // 0x304 (0x70C)
    u32 Goto_mode;            // 0x308 (0x710)
    void* evtMot[8];          // 0x30C (0x714)  SDK Mot_tbl[4] + Seq_tbl[4]
    f32 HoseiCnt;             // 0x32C (0x734)
    f32 Lose_timer;           // 0x330 (0x738)
    u32 Wander_route;         // 0x334 (0x73C)
    f32 Fire_timer;           // 0x338 (0x740)  SDK Bomb_wait
    f32 Sin_neck;             // 0x33C (0x744)
    f32 Route_h;              // 0x340 (0x748)
    f32 Parasite_wait;        // 0x344 (0x74C)
    f32 Jcatch_wait;          // 0x348 (0x750)
    f32 Slope_spd;            // 0x34C (0x754)
    f32 Dash_wait;            // 0x350 (0x758)
    u32 Csaw_se_id;           // 0x354 (0x75C)
    f32 Eff_timer;            // 0x358 (0x760)  SDK Fire_timer
    f32 x764;                 // 0x35C (0x764)  SDK Eff_timer
    u32 Gatling_seid;         // 0x360 (0x768)
    f32 Blend;                // 0x364 (0x76C)
    f32 Hokan;                // 0x368 (0x770)
    f32 Frame;                // 0x36C (0x774)
    f32 Esc_timer;            // 0x370 (0x778)
    f32 CriAtk_wait;          // 0x374 (0x77C)
    f32 Poison_wait;          // 0x378 (0x780)
    f32 Find_timer;           // 0x37C (0x784)
    f32 Wander_timer;         // 0x380 (0x788)
    f32 Csaw_fake_timer;      // 0x384 (0x78C)
    f32 Csaw_sign_wait;       // 0x388 (0x790)
    f32 Anger_timer;          // 0x38C (0x794)
    f32 Slope_timer;          // 0x390 (0x798)
    f32 Throw_timer;          // 0x394 (0x79C)
    f32 Dash_timer;           // 0x398 (0x7A0)
    f32 WakeTimer;            // 0x39C (0x7A4)
    f32 Atk_wait;             // 0x3A0 (0x7A8)
    f32 Breath_se_wait;       // 0x3A4 (0x7AC)
    f32 Back_wait;            // 0x3A8 (0x7B0)
    f32 Frame_timer;          // 0x3AC (0x7B4)
    f32 Csaw_se_wait;         // 0x3B0 (0x7B8)
    u16 Claw_hp;              // 0x3B4 (0x7BC)
    u8 St_set;                // 0x3B6 (0x7BE)
    u8 Gatling_mode;          // 0x3B7 (0x7BF)
    f32 Eff_wait;             // 0x3B8 (0x7C0)
    u8 Eff_type;              // 0x3BC (0x7C4)
    u8 pad_3BD[3];
    f32 Atk_no_wait;          // 0x3C0 (0x7C8)
    u8 Atk_ck;                // 0x3C4 (0x7CC)
    u8 Atk_ck2;               // 0x3C5 (0x7CD)
    u8 Bend_rno;              // 0x3C6 (0x7CE)
    u8 Walk_type;             // 0x3C7 (0x7CF)
    u8 DownCnt;               // 0x3C8 (0x7D0)
    u8 EffKindIdCsaw;         // 0x3C9 (0x7D1)
    u8 EffKindIdEye;          // 0x3CA (0x7D2)
    u8 EffKindIdWork;         // 0x3CB (0x7D3)
    u8 EffKindIdArrow;        // 0x3CC (0x7D4)
    u8 EffKindIdCore;         // 0x3CD (0x7D5)
    u8 pad_3CE[2];
    f32 Water_eff_wait;       // 0x3D0 (0x7D8)
    f32 Water_eff_wait2;      // 0x3D4 (0x7DC)
    f32 Water_eff_wait3;      // 0x3D8 (0x7E0)
    f32 Bomb_se_wait;         // 0x3DC (0x7E4)
    u8 Look_cnt;              // 0x3E0 (0x7E8)
    u8 Gatling_roll;          // 0x3E1 (0x7E9)
    u8 Route_type;            // 0x3E2 (0x7EA)
    u8 Hand_type;             // 0x3E3 (0x7EB)
    u8 Parasite_on;           // 0x3E4 (0x7EC)
    u8 Go_target;             // 0x3E5 (0x7ED)
    u8 Trade_ck;              // 0x3E6 (0x7EE)
    u8 Se_no;                 // 0x3E7 (0x7EF)
    u8 Wep_type;              // 0x3E8 (0x7F0)
    u8 Wep_type2;             // 0x3E9 (0x7F1)
    u8 Cap_type;              // 0x3EA (0x7F2)
    u8 Arrow_num;             // 0x3EB (0x7F3)
    u8 Now_hide;              // 0x3EC (0x7F4)
    u8 Reset_enable;          // 0x3ED (0x7F5)
    u8 Pl_in_ck;              // 0x3EE (0x7F6)
    u8 R11c_in_ck;            // 0x3EF (0x7F7)
    u8 R11c_in_ck2;           // 0x3F0 (0x7F8)
    u8 Csaw_regist;           // 0x3F1 (0x7F9)
    u8 pad_3F2[2];
    f32 No_adj_timer;         // 0x3F4 (0x7FC)
    u8 Landing_ck;            // 0x3F8 (0x800)
    u8 Claw_rno_l;            // 0x3F9 (0x801)
    u8 Claw_rno_r;            // 0x3FA (0x802)
    u8 pad_3FB;
    f32 Chain_se_wait;        // 0x3FC (0x804)
    f32 Die_wait;             // 0x400 (0x808)
    u8 Atk_trg;               // 0x404 (0x80C)
    u8 pad_405[3];
    f32 No_dmg_timer;         // 0x408 (0x810)
    u8 Omake_set;             // 0x40C (0x814)
    u8 pad_40D[3];
    f32 R180Timer;            // 0x410 (0x818)
    u8 Ganado;                // 0x414 (0x81C)
    u8 Se_tbl[19];            // 0x415 (0x81D)
    u8 JumpMode;              // 0x428 (0x830)
    u8 pad_429[0x42C - 0x429];
    u8 YarareTbl[0x634 - 0x42C];  // 0x42C (0x834)  YARARE_INFO[10]
    u8 Cam[0x72C - 0x634];        // 0x634 (0xA3C)
    ClothInfo Cloth;              // 0x72C (0xB34)
    u8 Sub_mot[0x85C - 0x78C];    // 0x78C (0xB94)  MOTION_INFO
};

class cEm10 : public cEm {
public:
    // Overrides of cModel / cEm virtuals.
    virtual void move();
    virtual void setNoSuspend(int onoff);
    virtual int checkThrow();
    // cEm10 virtuals, in vtable order (GameCube em10.h, re4_tweaks SDK).
    virtual void setHand(int no, int type);
    virtual void setWeaponFall();
    virtual int ckFindPL();
    virtual void setFindPL();
    virtual void clearFindPL();
    virtual int ckParasite();
    virtual u32 ckGoto();
    virtual void setGoto(Vec* pos, int range);
    virtual void setGotoSwitch(cModel* sw, int near, Vec* pos);
    virtual int ckResetEnable();
    virtual void setReset();
    virtual void chgSet(u8 no);
    virtual void setEvtMotion(void* m0, void* m1, void* m2, void* m3);
    virtual void setGondolaMotion(void* m0, void* m1, void* m2, void* m3);
    virtual void setR11DMotion(void* m0);
    virtual void setDrill(void* m0, void* m1, void* m2, void* m3);
    virtual void setGatling(void* g, void* m0, void* m1, void* m2, void* m3);
    virtual void setGatlingMode(u8 mode);
    virtual int ckBombFire();
    virtual int ckShiled();
    virtual int ckBowgunFire();
    virtual void setSwitch(cModel* sw);
    virtual void setLost();
    virtual void setWeapon(void* bin, void* tpl, int type);
    virtual int ckWeapon();
    virtual int ckTakeAway();
    virtual void setUFOCatch(void* m0, void* m1);
    virtual int ckR305BomberEnable();

    u8 free[0xC64 - 0x408];   // 0x408  Em10Work
};

#define EM10_WK(em) ((Em10Work*) (((cEm10*) (em))->free))

DECOMP_OFFSET_CHECK(Em10Work, TmpV, 0x000);
DECOMP_OFFSET_CHECK(Em10Work, Be_flg, 0x0A8);
DECOMP_OFFSET_CHECK(Em10Work, pWeapon, 0x208);
DECOMP_OFFSET_CHECK(Em10Work, L_pl_route, 0x28C);
DECOMP_OFFSET_CHECK(Em10Work, Seid_voice, 0x2E0);
DECOMP_OFFSET_CHECK(Em10Work, evtMot, 0x30C);
DECOMP_OFFSET_CHECK(Em10Work, Claw_hp, 0x3B4);
DECOMP_OFFSET_CHECK(Em10Work, Walk_type, 0x3C7);
DECOMP_OFFSET_CHECK(Em10Work, Wep_type, 0x3E8);
DECOMP_OFFSET_CHECK(Em10Work, No_adj_timer, 0x3F4);
DECOMP_OFFSET_CHECK(Em10Work, Omake_set, 0x40C);
DECOMP_OFFSET_CHECK(Em10Work, JumpMode, 0x428);
DECOMP_OFFSET_CHECK(Em10Work, Cloth, 0x72C);
typedef char decomp_check_ClothInfo_size[sizeof(ClothInfo) == 0x60 ? 1 : -1];
typedef char decomp_check_cEm10_size[sizeof(cEm10) == 0xC64 ? 1 : -1];
typedef char decomp_check_Em10Work_size[sizeof(Em10Work) == 0xC64 - 0x408 ? 1 : -1];

DECOMP_FUNC(int, GetEm10EyeEffectEnable, (), 0x00401CEE);
DECOMP_FUNC(cObj16*, SetObj16, (void* m0, void* m1, cEm* parent, cEm* target, int a4, int a5, Vec* pos, Vec* rot),
            0x0040C5C7);
DECOMP_FUNC(cObj12*, SetObj12, (void* m0, void* m1, Vec* pos, Vec* ang), 0x00407130);
// PC addition: keeps the r101 cart object (0x00C65F84) so 0x006700F0 can release it.
DECOMP_FUNC(void, SetCartObj, (cObj12* obj), 0x004068A7);
DECOMP_FUNC(cCtrl*, GetCtrlDragon, (int no), 0x0040A55B);
DECOMP_FUNC(int, CheckInWater, (cModel* m, int a), 0x0040846D);
// Engine functions used by em10.cpp (real addresses; the game calls them
// through the incremental-link table).
DECOMP_FUNC(f32, GetXZAngle, (Vec* v0, Vec* v1), 0x006CFE10);
DECOMP_FUNC(void, RouteCkGetPoint, (int no, Vec* out), 0x006B0F90);
DECOMP_FUNC(int, RouteCkGetPointNumber, (), 0x006B1010);
DECOMP_FUNC(int, RouteCkGetNearPoint, (Vec* pos), 0x006B2260);
DECOMP_FUNC(int, ChkWaterEffectEnable, (Vec* pos), 0x00616260);

extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
static inline f32 fabsf(f32 x) { return (f32) fabs(x); }
DECOMP_FUNC(cObjChain*, SetChain, (void* m0, void* m1, Vec* pos, Vec* rot), 0x0040B749);

// Per-module set function the family prolog stores (EmXXSet), run by em10_R0_Init.
DECOMP_GLOBAL(Em10Func, Em10SetFunc, 0x00C5017C);

// Routine tables of em10.cpp (function pointers, bound by address).
DECOMP_GLOBAL_ARRAY(Em10Func, Em10_R0_move_tbl, 5, 0x00BF39B0);
DECOMP_GLOBAL_ARRAY(Em10Func, Em10_R1_move_tbl, 0xDE, 0x00BF39C8);
DECOMP_GLOBAL_ARRAY(Em10Func, Em10_R1_dmg_tbl, 22, 0x00BF3D40);
DECOMP_GLOBAL_ARRAY(Em10Func, Em10_R1_die_tbl, 6, 0x00BF3D98);
DECOMP_GLOBAL_ARRAY(Em10Func, Em10DmSetWep_tbl, 46, 0x00BF3DB0);

// Chain and belt tables of em10.cpp (game data, bound by address).
DECOMP_GLOBAL_ARRAY(u8, em10_chain_parts, 4, 0x00BF39C4);
DECOMP_GLOBAL_ARRAY(u8, em10_chain_up, 4, 0x00BF3E74);
DECOMP_GLOBAL_ARRAY(u8, em10_chain_down, 4, 0x00BF4048);
DECOMP_GLOBAL_ARRAY(ClothAtSet, em10_chain_at, 5, 0x00BF4070);
DECOMP_GLOBAL_ARRAY(u8, em10_belt_parts, 8, 0x00BF4124);
DECOMP_GLOBAL_ARRAY(u8, em10_belt_up, 8, 0x00BF412C);
DECOMP_GLOBAL_ARRAY(u8, em10_belt_down, 8, 0x00BF4134);
DECOMP_GLOBAL_ARRAY(f32, em10_belt_max, 8, 0x00BF413C);

#endif
