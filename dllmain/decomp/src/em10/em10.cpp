// Ganado enemy (cEm10), Steam bio4.exe. Guide: references/re4-gamecube/src/em10/em10.cpp.
// The original unit uses x87 floating point: no /arch:SSE2.
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "em10.h"
#include "global.h"


#define EM10_WALK_MOT(base)                    \
    m0 = (MotionData*) PL_ARC_PTR(em->subArc, base); \
    switch (v) {                               \
    case 0:                                    \
    default:                                   \
        m1 = PL_ARC_PTR(em->subArc, base + 1); \
        break;                                 \
    case 1:                                    \
        m1 = PL_ARC_PTR(em->subArc, base + 2); \
        break;                                 \
    case 2:                                    \
        m1 = PL_ARC_PTR(em->subArc, base + 3); \
        break;                                 \
    case 3:                                    \
        m1 = PL_ARC_PTR(em->subArc, base + 4); \
        break;                                 \
    case 4:                                    \
        m1 = PL_ARC_PTR(em->subArc, base + 5); \
        break;                                 \
    }

// FUNCTION: BIO4 0x004323E0
// STATUS: matching
// Walk motion by weapon kind and set-number variant (5 walk styles, water / event overrides).
// Static: the enemy arrives in ESI.
static void em10SetWalkMotion(cEm10* em, int a)
{
    Em10Work* w = EM10_WK(em);
    u32 kind;
    int flag;
    u32 v;
    MotionData* m0;
    void* m1;

    kind = 0;
    if (w->Ganado == 1) {
        kind = 8;
    }
    if (em->type == 6) {
        kind = 7;
    }
    if (w->pWeapon) {
        if (!(w->Be_flg & 0x20000000) && (w->Be_flg & 0x100)) {
            kind = 3;
        }
        if (w->Wep_type == 1) {
            kind = 4;
        }
        if (w->Wep_type == 4) {
            kind = 5;
        }
        if (w->Wep_type == 8 || w->Wep_type == 0xC) {
            kind = 6;
        }
        if (w->Wep_type == 6) {
            kind = 9;
        }
        if (w->Wep_type == 0xC) {
            kind = 0xC;
        }
    }
    if (w->pShield) {
        kind = 0xA;
    }
    if (em->type == 0xA) {
        kind = 0xB;
    }
    if (em->type == 0xD) {
        kind = 0xB;
    }
    if (em->type == 2) {
        kind = 0xD;
    }
    flag = (em->flag & 0x01000000) ? 0x45 : 5;
    v = em->emset_no % 5;
    if (CheckInWater(em, 0)) {
        v = 1;
    }
    if ((pG->flags_SYSTEM[0] & 0x80000000) || (pG->flags_SYSTEM[0] & 0x40000000)) {
        if (v == 2) {
            v = 1;
        }
        if (v == 4) {
            v = 3;
        }
    }
    switch (kind) {
    case 0:
    case 1:
    case 2:
    default:
        switch (w->Walk_type) {
        case 0:
        default:
            EM10_WALK_MOT(8);
            break;
        case 1:
            EM10_WALK_MOT(0xE);
            break;
        }
        break;
    case 3:
        EM10_WALK_MOT(0xD8);
        break;
    case 4:
        EM10_WALK_MOT(0x14B);
        break;
    case 5:
        if (w->Be_flg & 0x100) {
            m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xEF);
            m1 = PL_ARC_PTR(em->subArc, 0xF0);
        } else {
            m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xED);
            m1 = PL_ARC_PTR(em->subArc, 0xEE);
        }
        break;
    case 6:
        EM10_WALK_MOT(0x126);
        if (!(w->Be_flg & 0x100)) {
            m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x124);
            m1 = PL_ARC_PTR(em->subArc, 0x125);
        }
        break;
    case 7:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xA8);
        m1 = PL_ARC_PTR(em->subArc, 0xAB);
        break;
    case 8:
        EM10_WALK_MOT(0xCE);
        break;
    case 9:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x13B);
        m1 = PL_ARC_PTR(em->subArc, 0x13C);
        break;
    case 0xA:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x166);
        m1 = PL_ARC_PTR(em->subArc, 0x167);
        break;
    case 0xB:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x109);
        m1 = PL_ARC_PTR(em->subArc, 0x10A);
        break;
    case 0xC:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x17D);
        m1 = PL_ARC_PTR(em->subArc, 0x17E);
        break;
    case 0xD:
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x190);
        m1 = PL_ARC_PTR(em->subArc, 0x191);
        break;
    }
    {
        u32 fr = (u32) ((f32) (u16) (m0->maxFrame & 0x3FFF) * (f32) em->r_no_3 / 256.0f);
        MotionSetCore(em, MOTION(em), m0, m1, a, flag, (f32) fr, 0);
    }
}
#undef EM10_WALK_MOT

// FUNCTION: BIO4 0x00439120
// STATUS: matching
// Claw Ganado (types 0xA / 0xD): hangs the chain (archive 0x225 / 0x226) from part 0x25.
// The PC build dropped the GameCube "EM10 em10ChainSet failed." log.
static void em10ChainSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;

    if (em->type != 10 && em->type != 13) {
        return;
    }
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    w->pChain = SetChain(PL_ARC_PTR(em->subArc, 0x225), PL_ARC_PTR(em->subArc, 0x226), &pos, &rot);
    if (!w->pChain) {
        return;
    }
    w->Cloth.Num = 4;
    w->Cloth.pCloth = em10_chain_parts;
    w->Cloth.pLeft = 0;
    w->Cloth.pRight = 0;
    w->Cloth.pUpLeft = 0;
    w->Cloth.pUpRight = 0;
    w->Cloth.pParent = em10_chain_up;
    w->Cloth.pChild = em10_chain_down;
    w->Cloth.pMax = 0;
    w->Cloth.pWindSin = 0;
    w->Cloth.pWindRate = 0;
    w->Cloth.pAtset = em10_chain_at;
    w->Cloth.pGravity = 0;
    w->Cloth.pRate = 0;
    w->Cloth.At_num = 5;
    w->Cloth.pEm_at = em;
    w->Cloth.Gravity = 25.0f;
    w->Cloth.Rate = 0.9f;
    w->Cloth.Bundle_num = 10;
    w->Cloth.WindSin = 0.0f;
    w->Cloth.Stretchy = 0.1f;
    w->Cloth.Move_rate = 0.0f;
    w->Cloth.Flag = 0;
    w->Cloth.pPtbl = 0;
    w->pChain->setChain(&w->Cloth);
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    w->pChain->setParent(em, 0x25, &pos, 1);
}

// FUNCTION: BIO4 0x004392E0
// STATUS: matching
// Gatling Ganado (type 2): hangs the ammunition belt (archive 0x22D / 0x22E) from part 0x23.
static void em10BeltSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;

    // `type != 2` compares the byte; the original subtracts from the
    // zero-extended type.
    if (em->type - 2 != 0) {
        return;
    }
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    w->pGunBelt = SetChain(PL_ARC_PTR(em->subArc, 0x22D), PL_ARC_PTR(em->subArc, 0x22E), &pos, &rot);
    if (!w->pGunBelt) {
        return;
    }
    w->Cloth.Num = 8;
    w->Cloth.pCloth = em10_belt_parts;
    w->Cloth.pLeft = 0;
    w->Cloth.pRight = 0;
    w->Cloth.pUpLeft = 0;
    w->Cloth.pUpRight = 0;
    w->Cloth.pParent = em10_belt_up;
    w->Cloth.pChild = em10_belt_down;
    w->Cloth.pMax = em10_belt_max;
    w->Cloth.pWindSin = 0;
    w->Cloth.pWindRate = 0;
    w->Cloth.pAtset = 0;
    w->Cloth.pGravity = 0;
    w->Cloth.pRate = 0;
    w->Cloth.At_num = 0;
    w->Cloth.pEm_at = em;
    w->Cloth.Gravity = 25.0f;
    w->Cloth.Rate = 0.9f;
    w->Cloth.Bundle_num = 10;
    w->Cloth.WindSin = 0.0f;
    w->Cloth.Stretchy = 0.1f;
    w->Cloth.Move_rate = 0.0f;
    w->Cloth.Flag = 0;
    w->Cloth.pPtbl = 0;
    w->pGunBelt->setChain(&w->Cloth);
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    w->pGunBelt->setParent(em, 0x23, &pos, 1);
}

// em10.cpp functions not ported yet (declared so ported callers compile;
// a caller of a static helper stays `stub` until the helper is ported, since
// the original may pass arguments in registers).
// BEGIN unported: generated by decomp/tools/port_gc.py
inline int em10DmgDeadCk(cDmgInfo* d);
void em10DmCk(cEm10* em);
void em10DmSetWep00(cEm10* em);
void em10DmSetWep02(cEm10* em);
void em10DmSetWep03(cEm10* em);
void em10DmSetWep09(cEm10* em);
void em10DmSetWep23(cEm10* em);
void em10BloodSet(cEm10* em, int near);
void em1cBloodSet(cEm10* em, int near);
void em10KickHitMark(cEm10* em);
void __fastcall em10InitRtnSet(cEm10* em);
void em10_R0_Init(cEm10* em);
void em10_R0_Move(cEm10* em);
void em10_R1_br_Dummy(cEm10* em);
void em10_R1_br_Wait(cEm10* em);
void em10_R1_Wait(cEm10* em);
void em10_R1_Keeper(cEm10* em);
inline void em10HideOn(cEm10* em, Em10Work* w);
inline void em10HideOff(cEm10* em, Em10Work* w);
void em10_R1_Hide(cEm10* em);
void em10_R1_HideFall(cEm10* em);
void em10_R1_HideJump(cEm10* em);
void em10_R1_R10CParasite(cEm10* em);
void em10_R1_R10CPCancel(cEm10* em);
void em10_R1_R204Prayer(cEm10* em);
void em10_R1_R222DragonA(cEm10* em);
void em10_R1_R222DragonB(cEm10* em);
void em10_R1_R222DragonC(cEm10* em);
void em10_R1_R227Barrel(cEm10* em);
void em10_R1_R21BTrolleyJump(cEm10* em);
void em10_R1_R21BTrolleyJump2(cEm10* em);
void em10_R1_R303FireDash(cEm10* em);
void em10_R1_R10FGJump(cEm10* em);
void em10_R1_R10FGondola(cEm10* em);
void em10_R1_R209DashSit(cEm10* em);
inline void em10ClawAtkEnd(cEm10* em, Em10Work* w);
void em10_R1_StickClaw(cEm10* em);
void em10_R1_R11DAppear1(cEm10* em);
void em10_R1_R11DAppear2(cEm10* em);
void em10_R1_R212Drill(cEm10* em);
void em10_R1_R209Gatling(cEm10* em);
void em10_R1_R201EventWait(cEm10* em);
void em10_R1_FindLost(cEm10* em);
void em10_R1_R100WalkStay(cEm10* em);
void em10_R1_R202Finger(cEm10* em);
void em10_R1_StayWalk(cEm10* em);
void em10_R1_AttackWait(cEm10* em);
void em10_R1_R100TurnWalk(cEm10* em);
void em10_R1_R100Cliff(cEm10* em);
void em10_R1_R101Bucket(cEm10* em);
void em10_R1_R101Suki(cEm10* em);
void em10_R1_Work(cEm10* em);
void em10_R1_UFOCatch(cEm10* em);
void em10_R1_R300TakeAshley(cEm10* em);
void em10_R1_R30FBullJump(cEm10* em);
void em10_R1_R320Gatling(cEm10* em);
void em10_R1_R321DeadBody(cEm10* em);
void em10_R1_R300Gatling(cEm10* em);
void em10_R1_R101Cart(cEm10* em);
void em10_R1_br_EvtDash(cEm10* em);
void em10_R1_EvtDash(cEm10* em);
void em10_R1_br_EvtWalk(cEm10* em);
void em10_R1_EvtWalk(cEm10* em);
void em10_R1_Pickup(cEm10* em);
void em10_R1_Find(cEm10* em);
void em10_R1_C_SawStart(cEm10* em);
void em10_R1_BombIgnition(cEm10* em);
void em10_R1_br_Walk(cEm10* em);
void em10_R1_Walk(cEm10* em);
void em10_R1_br_Dash(cEm10* em);
void em10_R1_Dash(cEm10* em);
void em10_R1_br_Back(cEm10* em);
void em10_R1_Back(cEm10* em);
void em10_R1_br_Goto(cEm10* em);
void em10_R1_Goto(cEm10* em);
void em10_R1_GuardWalk(cEm10* em);
void em10_R1_Turn180(cEm10* em);
void em10_R1_Threat(cEm10* em);
void em10_R1_SideStep(cEm10* em);
void em10_R1_HideSide(cEm10* em);
void em10_R1_AppearSide(cEm10* em);
void em10_R1_SitDown(cEm10* em);
void em10_R1_Stay(cEm10* em);
void em10_R1_RoofWait(cEm10* em);
void em10_R1_Guard(cEm10* em);
void em10_R1_DownWakeWait(cEm10* em);
void em10_R1_DownWake(cEm10* em);
void em10_R1_Crash(cEm10* em);
void em10_R1_ClimbOver(cEm10* em);
void em10_R1_DoorAtk(cEm10* em);
void em10_R1_RackAtk(cEm10* em);
void em10_R1_WindowAtk(cEm10* em);
void em10_R1_LadderClimb(cEm10* em);
void em10_R1_VLadderClimb(cEm10* em);
void em10_R1_LadderReset(cEm10* em);
void em10_R1_JumpDown(cEm10* em);
void em10_R1_Jump(cEm10* em);
void em10_R1_JumpUp(cEm10* em);
void em10_R1_Trade(cEm10* em);
void em10_R1_Drive(cEm10* em);
void em10_R1_Catapult(cEm10* em);
void em10_R1_RockPush(cEm10* em);
void em10_R1_ParasiteAtk(cEm10* em);
void em10_R1_ShotBowgun(cEm10* em);
void em10_R1_ShotRocket(cEm10* em);
void em10_R1_ShotGatling(cEm10* em);
void em10_R1_ThrowAxe(cEm10* em);
void em10_R1_ThrowBomb(cEm10* em);
void em10_R1_FixBomber(cEm10* em);
void em10_R1_R305Bomber(cEm10* em);
void em10_R1_R408Bomber(cEm10* em);
void em10_R1_RocketWait(cEm10* em);
void em10_R1_AxeAtk(cEm10* em);
void em10_R1_ShieldAtk(cEm10* em);
void em10_R1_TorchFrame(cEm10* em);
void em10_R1_SukiAtk(cEm10* em);
void em10_R1_ScytheAtk(cEm10* em);
void em10_R1_ClawAtk(cEm10* em);
void em10_R1_br_CSawWalkAtk(cEm10* em);
void em10_R1_CSawWalkAtk(cEm10* em);
void em10_R1_ClawWalkAtk(cEm10* em);
void em10_R1_br_ClawCriAtk(cEm10* em);
void em10_R1_ClawCriAtk(cEm10* em);
void em10_R1_ClawCriHit(cEm10* em);
void plem10_ClawCriHit(cPlayer* pl);
void em10_R1_br_C_SawAtk(cEm10* em);
void em10_R1_C_SawAtk(cEm10* em);
void em10_R1_C_SawHit(cEm10* em);
void plem10_C_SawHit(cPlayer* pl);
void em10_R1_br_C_SawCriAtk(cEm10* em);
void em10_R1_C_SawCriAtk(cEm10* em);
void em10_R1_C_SawCriHit(cEm10* em);
void plem10_C_SawCriHit(cPlayer* pl);
void em10_R1_br_Catch(cEm10* em);
void em10_R1_Catch(cEm10* em);
void em10_R1_NeckHang(cEm10* em);
void plem10_NeckHang(cPlayer* pl);
void em10_R1_NeckHang_Luis(cEm10* em);
void subem10_NeckHang_Luis(cSubChar* sub);
void em10_R1_NeckHang_Ashley(cEm10* em);
void subem10_NeckHang_Ashley(cSubChar* sub);
void em10_R1_Backhold(cEm10* em);
void plem10_Backhold(cPlayer* pl);
void em10_R1_Bombhold(cEm10* em);
void plem10_Bombhold(cPlayer* pl);
void em10_R1_br_DashCatch(cEm10* em);
void em10_R1_DashCatch(cEm10* em);
void em10_R1_TakeAway(cEm10* em);
void subem10_TakeAway(cSubChar* sub);
extern "C" void em10CamMoveTakeaway(cEm10* em);
void em10_R0_Damage(cEm10* em);
void em10_R1_Dm_Small(cEm10* em);
void em10_R1_Dm_Head(cEm10* em);
void em10_R1_Dm_Flash(cEm10* em);
void em10_R1_Dm_Claw(cEm10* em);
void em10_R1_Dm_Claw_Big(cEm10* em);
void em10_R1_Dm_Gatling(cEm10* em);
void em10_R1_Dm_FS(cEm10* em);
void em10_R1_Dm_KneeKick(cEm10* em);
void em10_R1_Dm_NeckBreak(cEm10* em);
void em10_R1_Dm_Showtay(cEm10* em);
void em10_R1_Dm_Heel(cEm10* em);
void em10_R1_Dm_DashUp(cEm10* em);
void em10_R1_Dm_DashDown(cEm10* em);
void em10_R1_Dm_Blow(cEm10* em);
void em10_R1_Dm_Fence(cEm10* em);
void em10_R1_Dm_Ladder(cEm10* em);
void em10_R1_Dm_Roof(cEm10* em);
void em10_R1_Dm_KneeDown(cEm10* em);
void em10_R1_Dm_KnockOut(cEm10* em);
void em10_R1_Dm_Down(cEm10* em);
void em10_R1_Dm_Frame(cEm10* em);
void em10_R1_Dm_TakeAway(cEm10* em);
void em10_R0_Die(cEm10* em);
void em10_R1_Die_Cramp(cEm10* em);
void em10_R1_Die_Lost(cEm10* em);
void em10_R1_Die_Down(cEm10* em);
void em10_R1_Die_Normal(cEm10* em);
void em10_R1_Die_RunDown(cEm10* em);
void em10_R1_Die_Bomb(cEm10* em);
void em10RouteCk(cEm10* em);
extern "C" int em10GetGoSub(cEm10* em);
extern "C" int em10RouteTargetSet(cEm10* em);
extern "C" int em10GetWanderRouteEmi(cEm10* em);
u32 em10GetWanderRoute(cEm10* em);
extern "C" void em10GetWanderRoutePos(cEm10* em, Vec* pos);
extern "C" u32 em10WanderRouteUpdate(cEm10* em, int no);
extern "C" int em10SetWanderRoute(cEm10* em);
int em10AtkRtnCk(cEm10* em, int a);
extern "C" int em10CatchPLRtnCk(cEm10* em);
extern "C" int em10CatchSubRtnCk(cEm10* em);
int em10CatchCk(cEm10* em);
int em10CatchSubCk(cEm10* em);
int em10CsawHitCk(cEm10* em);
int em10LostHeadCk(cEm10* em);
int em10LostHead(cEm10* em, int a, int b);
extern "C" void em10CallVoiceSe(cEm10* em, u16 no);
void em10CallVoiceSe2(cEm10* em, u16 no, u16 a);
void em10BreathSe(cEm10* em);
extern "C" void em10CsawSignSe(cEm10* em);
int em10ModelInit(cEm10* em);
extern "C" void Em10SetSeTbl(cEm10* em, int type);
extern "C" void em10WeaponInit(cEm10* em);
extern "C" void em10SetAccesory(cEm10* em);
cEmWep* em10MakeWeapon(cEm10* em, int type);
void em10WeaponSet(cEm10* em);
extern "C" void em10WeaponSet2(cEm10* em);
extern "C" void em10WepSeEffSet(cEm10* em, cEmWep* wep, int type);
extern "C" void em10ShieldSet(cEm10* em);
void em10HandSet(cEm10* em, int type);
void em10HeadSet(cEm10* em, int no);
extern "C" void em10ClothPartsSet(cEm10* em, int no);
extern "C" void em10GoodsPartsSet(cEm10* em, int on);
extern "C" void em10SackSet(cEm10* em);
extern "C" int em10ClimbOverCk2(cEm10* em);
int em10ClimbOverCk(cEm10* em);
extern "C" int em10WindowCk2(cEm10* em);
int em10WindowCk(cEm10* em);
int em10DoorOpenCk(cEm10* em, int kick);
extern "C" int em10AtkDoorCk(cEm10* em);
int em10SetDamageDoor(cEm10* em, int kind);
int em10RackBreakCk(cEm10* em);
extern "C" int em10AtkRackCk(cEm10* em);
void em10SetDamageRack(cEm10* em, int a);
int em10LadderClimbCk(cEm10* em);
int em10VLadderClimbCk(cEm10* em);
int em10LadderResetCk(cEm10* em);
extern "C" int em10JumpDownCk2(cEm10* em);
int em10JumpDownCk(cEm10* em);
int em10JumpCk(cEm10* em);
extern "C" int em10BullJumpCk(cEm10* em);
void em10ReturnStartPosCk(cEm10* em);
extern "C" int em10ParasiteAtkCk(cEm10* em);
extern "C" int em10ShotBowgunCk(cEm10* em);
extern "C" int em10ShotRocketCk(cEm10* em);
extern "C" int em10ShotGatlingCk(cEm10* em);
extern "C" int em10ThrowAxeCk(cEm10* em);
int em10ThrowBombCk(cEm10* em);
extern "C" int em10AxeAtkCk(cEm10* em);
extern "C" int em10ShieldAtkCk(cEm10* em);
extern "C" int em10SukiAtkCk(cEm10* em);
extern "C" int em10ScytheAtkCk(cEm10* em);
extern "C" int em10ClawAtkCk(cEm10* em);
extern "C" int em10ClawCriAtkCk(cEm10* em);
extern "C" int em10CsawAtkCk(cEm10* em);
extern "C" int em10ThreatCk(cEm10* em);
extern "C" int em10PlRunCk(cEm10* em);
extern "C" int em10HeadLockCk(cEm10* em);
void em10NeckMove(cEm10* em);
void em10WaistMove(cEm10* em);
void em10ScaleCompress(cEm10* em);
int em10FindCk(cEm10* em, int a);
int em10FindCk2(cEm10* em);
extern "C" int em10SomebodyFindNowCk(cEm10* em);
int em10FindFloorCk(cEm10* em);
extern "C" int em10SomebodyDamageNowCk(cEm10* em);
int em10SomebodyNearCk(cEm10* em);
void em10FindNotify(cEm10* em);
void em10WalkRtnSet(cEm10* em);
static int em10GotoCk(cEm10* em);
extern "C" int em10BackCk(cEm10* em);
void em10SlopeMove(cEm10* em);
extern "C" void em10CamMove(cEm10* em, int no, f32 rate, int shake);
extern "C" void em10SetCampos2(cEm10* em);
void em10CamMove2(cEm10* em);
extern "C" void em10CamMoveCri(cEm10* em, u32 no, int shake);
extern "C" void em10CamMoveAshley(cEm10* em, u32 no);
void em10SetParasite(cEm10* em);
void em10SetWaitMotion(cEm10* em, int a);
static void em10SetWalkMotion(cEm10* em, int a);
void em10SetDashMotion(cEm10* em);
extern "C" int em10DashCk(cEm10* em);
extern "C" int em10StayCk(cEm10* em);
extern "C" int em10GoSubStayCk(cEm10* em);
void em10ChainSawMove(cEm10* em);
int em10AtkCk(cEm10* em, Vec* a, Vec* b, int no, int parts);
extern "C" int em10GatlingHitCk(cEm10* em);
extern "C" void em10BellAtkCk(cEm10* em, Vec* pos, u32 no);
extern "C" int em10TorchFrameAtkCk(cEm10* em);
extern "C" int em10TorchFrameAtkCkSub(cEm10* em);
void em10DragonFireCk(cEm10* em);
void plemDmFrame(cPlayer* pl);
void plem10DmGondolaShake(cPlayer* pl);
void subem10DmGondolaShake(cSubChar* sub);
void plemDmMStar(cPlayer* pEm);
void plemDmStun(cPlayer* pl);
extern "C" void em10SetRtnFind(cEm10* em);
extern "C" void em10ActEvtSetKick(cEm10* em);
void em10KickAction(cEm10* em);
void em10KneeDownAction(cEm10* em);
void plem10Kick(cPlayer* pl);
void plem10Kick2(cPlayer* pl);
extern "C" void em10ActEvtSetFS(cEm10* em);
void em10FSAction(cEm10* em);
void plem10FS(cPlayer* pl);
void plem10KneeKick(cPlayer* pl);
void plem10NeckBreak(cPlayer* pl);
void plem10NeckBreakCamMove(cPlayer* pl, int a);
void plem10Showtay(cPlayer* pl);
void em10ActEvtSetTrade(cEm10* em);
void em10TradeAction(cEm10* em);
extern "C" void plem10KickCamMove(cPlayer* pl, int a);
void em10SetCrash(cEm10* em, f32 r);
int em10CrashCk(cEm10* em);
void em10MouthPartsReset(cEm10* em);
int em10RoofDmCk(cEm10* em);
void em10FootSe(cEm10* em);
extern "C" int em10DootAtkCk(cEm10* em);
extern "C" int em10ThrowNearCk(cEm10* em);
extern "C" void em10BlendMotSet(cEm10* em, void* m0, void* m1, void* m2, int a, int b, int c, int d);
extern "C" int em10HideRtnCk(cEm10* em);
int em10HideRtnCk2(cEm10* em);
int em10HideToStepCk(cEm10* em, int a);
void em10BowgunMove(cEm10* em);
extern "C" void em10BehindSeCk(cEm10* em);
int em10SetDmVal(cEm10* em);
extern "C" cModel* em10SearchTruck(cEm10* em);
extern "C" int em10SearchParasite(cEm10* em);
extern "C" void em10ParasiteGoOut(cEm10* em);
void em10SetDamageVoice(cEm10* em, u16 a, u16 b);
int em10ChgParasiteCk(cEm10* em);
void em10SetDmWaterEff(cEm10* em, int a);
extern "C" void em10SetTakeawayPos(cEm10* em);
extern "C" void em10SetTakeawayPosUpdate(cEm10* em);
extern "C" int em10ReturnPosCk(cEm10* em);
int em10GotoPosCk(cEm10* em);
void em10SetWaterEff(cEm10* em);
extern "C" int em10ReturnCk(cEm10* em);
void em10BombNeckMove(cEm10* em);
extern "C" int em10ScreenInCk(cEm10* em);
extern "C" int em10ThrowScaCk(cEm10* em);
extern "C" int em10BombThrowScaCk(cEm10* em);
extern "C" void em10SetAtkWait(cEm10* em, int set);
int em10IgnitionCk(cEm10* em);
int em10ClawStickCK(cEm10* em);
int em10FindLostCk(cEm10* em);
extern "C" void em10PlHeadLost();
void em10BombThrow(cEm10* em);
cObjGondola* em10GetGondola(cEm10* em);
static void em10ChainSet(cEm10* em);
static void em10BeltSet(cEm10* em);
void em10ClawMove(cEm10* em);
int em10ArmorCk(cEm10* em, int parts);
void em10CoreBreak(cEm10* em, int a);
extern "C" void em10FallWaterCk(cEm10* em);
extern "C" f32 em10GetPower(cEm10* em);
void em10GatlingRollMove(cEm10* em);
void Em1fClothSet(cModel* m, PlCloth* c);
void Em1fClothMove(cModel* m, PlCloth* c);
void em10SetPoint(cEm10* em);
// END unported

// Inlined everywhere on PC (no own copy in the executable).
static int em10GotoCk(cEm10* em)
{
    if (EM10_WK(em)->Goto_mode) {
        EmRoutineSet(em, 1, 0x13, 0, 0);
        return 1;
    }
    return 0;
}

// FUNCTION: BIO4 0x00428FD0
// STATUS: matching
// PC only: every MotionMove(em, 0) of em10.cpp goes through this wrapper,
// which puts back the scale of parts 0xA and 0x10 when the motion reports
// bit 0. Static: the enemy arrives in ESI.
static u32 em10MotionMove(cEm10* em)
{
    u32 ret = MotionMove(em, 0);
    if (ret & 1) {
        cModel* parts = em->getPartsPtr(0xA);
        parts->scale.x = 1.0f;
        parts->scale.y = 1.0f;
        parts->scale.z = 1.0f;
        parts->r_scale.x = 1.0f;
        parts->r_scale.y = 1.0f;
        parts->r_scale.z = 1.0f;
        parts = em->getPartsPtr(0x10);
        parts->scale.x = 1.0f;
        parts->scale.y = 1.0f;
        parts->scale.z = 1.0f;
        parts->r_scale.x = 1.0f;
        parts->r_scale.y = 1.0f;
        parts->r_scale.z = 1.0f;
    }
    return ret;
}

// Frame counters of the PC port: a count above one frame gets the frame time
// scale of the port added.
static inline void Em10SetTimer(f32& t, int n)
{
    f32 f = (f32) n;
    t = f;
    if (n > 1) {
        t = f + (1.0f - pG->deltaTime);
    }
}

// FUNCTION: BIO4 0x0043AA90
// STATUS: functional
void __fastcall em10InitRtnSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;

    if (em->type != 6) {
        em->RckPad0 = 1;
    }
    switch (em->type) {
    case 2:
    case 5:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xD:
        w->Walk_type = 2;
        break;
    default:
        w->Walk_type = Rnd() & 1;
        break;
    }
    w->Route_type = em->emset_no % 7;
    if (em->Guard_r <= 0.0f) {
        em->Guard_r = 20000.0f;
    }
    w->Be_flg = 0;
    w->DownCnt = Rnd() % 5 + 5;
    Em10SetTimer(w->Atk_wait, 0);
    w->Seid_voice = 0;
    w->Seid_breath = 0;
    w->Seid_csaw = 0;
    w->Neck_dir_x = 0.0f;
    w->Neck_dir_y = 0.0f;
    w->Finger_dir = 0.0f;
    Em10SetTimer(w->Dash_wait, Rnd() % 450 + 150);
    Em10SetTimer(w->Breath_se_wait, Rnd() % 90 + 90);
    Em10SetTimer(w->Back_wait, 0);
    w->Floor_ang.x = 0.0f;
    w->Floor_ang.y = 0.0f;
    w->Floor_ang.z = 0.0f;
    w->L_pl_route = 100000000.0f;
    w->L_pl_guard = 100000000.0f;
    w->L_guard = 100000000.0f;
    w->L_sub_route = 100000000.0f;
    Em10SetTimer(w->HoseiCnt, 0);
    Em10SetTimer(w->Eff_timer, 0);
    w->Hand_type = 0xFF;
    Em10SetTimer(w->Slope_timer, 0);
    w->Slope_spd = 0.0f;
    Em10SetTimer(w->Jcatch_wait, Rnd() % 150);
    w->Compress_y = 1.0f;
    Em10SetTimer(w->Anger_timer, 0);
    w->Csaw_se_id = 0;
    Em10SetTimer(w->Throw_timer, 0);
    Em10SetTimer(w->Atk_no_wait, 0);
    w->Arrow_num = 2;
    w->Now_hide = 0;
    w->Goto_mode = 0;
    Em10SetTimer(w->Frame_timer, 0);
    w->pCore = 0;
    w->pParasite = 0;
    for (i = 0; i < 5; i++) {
        w->pTen[i] = 0;
    }
    w->pTruck = 0;
    Em10SetTimer(w->Parasite_wait, 0);
    w->Reset_enable = 0;
    w->Parasite_on = 0;
    w->Pl_in_ck = 0;
    w->R11c_in_ck = 0;
    w->R11c_in_ck2 = 0;
    w->Sin_neck = 0.0f;
    w->Csaw_regist = Rnd() % 3 + 2;
    Em10SetTimer(w->Water_eff_wait3, 0);
    Em10SetTimer(w->Fire_timer, 0);
    Em10SetTimer(w->Eff_wait, 0);
    Em10SetTimer(w->Csaw_sign_wait, 0);
    Em10SetTimer(w->No_adj_timer, 0);
    w->pSwitch = 0;
    w->pDragon = 0;
    w->pGondola = 0;
    w->Claw_rno_l = 0;
    w->Claw_rno_r = 0;
    Em10SetTimer(w->CriAtk_wait, 450);
    w->Claw_hp = Rnd() % 150 + 150;
    Em10SetTimer(w->Chain_se_wait, 0);
    w->pDrill = 0;
    w->pGatling = 0;
    Em10SetTimer(w->Find_timer, 0);
    Em10SetTimer(w->Wander_timer, 0);
    Em10SetTimer(w->Die_wait, 0);
    Em10SetTimer(w->No_dmg_timer, 0);
    w->Gatling_roll = 0;
    w->Gatling_seid = 0;
    w->Omake_set = 0;
    w->pGunBelt = 0;
    w->pChain = 0;
    for (i = 0; i < 3; i++) {
        void** mot0 = &w->evtMot[0];
        void** mot4 = &w->evtMot[4];
        mot0[i] = 0;
        mot4[i] = 0;
    }
    if (em->type == 0x17 || em->type == 0x19) {
        if (em->type == 0x17) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x8E, 0x800, w->EffKindIdEye, em, 0);
        } else {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x8F, 0x800, w->EffKindIdEye, em, 0);
        }
    } else if (GetEm10EyeEffectEnable()) {
        EstSet(em, -1, 0, 0, EFF_CORE, 0x35, 0x800, w->EffKindIdEye, em, 0);
    }
    if (em->type == 0xA || em->type == 0xD) {
        Vec pos;
        Vec rot;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        w->pCore = SetObj16(PL_ARC_PTR(em->subArc, 0x227), PL_ARC_PTR(em->subArc, 0x228), em, em, 0x24, 4, &pos, &rot);
        if (w->pCore) {
            PlArc* arc = em->subArc;
            w->pCore->setMotData(PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x229),
                                  PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x229),
                                  PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x22A), PL_ARC_PTR(arc, 0x22A),
                                  PL_ARC_PTR(arc, 0x229), PL_ARC_PTR(arc, 0x229));
        }
    }
    switch (em->set) {
    case 0x14:
    case 0x15:
        break;
    default:
        if (w->Wep_type == 7) {
            w->pWeapon->setEffAlways(0x10, 0x14);
            w->pWeapon->setEffFall(0x10, 0x1C);
        }
        if (w->Wep_type == 8 && w->pWeapon) {
            EstSet((cModel*) w->pWeapon, -1, 0, 0, EFF_EM10, 0x1F, 0, w->EffKindIdArrow, (cModel*) w->pWeapon, 0);
        }
        break;
    }
    if (em->set == 0x1D) {
        w->R11c_in_ck = 1;
    }
    if (em->set == 0x21) {
        w->R11c_in_ck2 = 1;
    }
    if ((em->flag & 0x40) && pG->curRoomId == 0x206) {
        em->setFindPL();
    }
    switch (em->set) {
    case 0:
    default:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        if (em->type == 6) {
            EmRoutineSet(em, 1, 0, 0, 0);
        } else {
            switch (em->Character) {
            case 1:
            case 3:
                EmRoutineSet(em, 1, 1, 0, 0);
                break;
            default:
                EmRoutineSet(em, 1, 0, 0, 0);
                break;
            }
        }
        break;
    case 0xE:
    case 0x1D:
    case 0x21:
        em10SetWalkMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        em->setFindPL();
        w->Route_type = Rnd() % 11;
        EmRoutineSet(em, 1, 0x10, 0, 0);
        break;
    case 0x1B:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 5, 0, 0);
        break;
    case 2:
    case 3:
    case 4:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 6, 0, 0);
        break;
    case 5:
    case 6:
    case 0x1A:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 7, 0, 0);
        break;
    case 7:
    case 8:
    case 9:
    case 0xA:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 8, 0, 0);
        break;
    case 0xB:
        if (w->mot[48] && w->mot[49]) {
            w->pCart = SetObj12(w->mot[48], w->mot[49], &em->pos, &em->ang);
        }
        SetCartObj(w->pCart);
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 9, 0, 0);
        break;
    case 0xC:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0xA, 0, 0);
        break;
    case 0x2F:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0xA, 2, 0);
        break;
    case 0xD:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0xB, 0, 0);
        break;
    case 0xF:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x18, 0, 0);
        break;
    case 0x10:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x18, 0, 1);
        break;
    case 0x11:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x1A, 0, 1);
        break;
    case 0x13:
        em10SetWaitMotion(em, 0);
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 2, 0, 0);
        break;
    case 0x14:
        em10SetWaitMotion(em, 0);
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 3, 0, 0);
        break;
    case 0x15:
        em10SetWaitMotion(em, 0);
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 4, 0, 0);
        break;
    case 0x16:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x19, 0, 0);
        break;
    case 0x17:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x19, 0, 1);
        break;
    case 0x18:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x47, 0, 0);
        break;
    case 0x19:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x48, 0, 0);
        break;
    case 0x1C:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x49, 0, 0);
        break;
    case 0x1E:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4A, 0, 0);
        break;
    case 0x30:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4B, 0, 0);
        break;
    case 0x1F:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4C, 0, 0);
        break;
    case 0x20:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4D, 0, 0);
        break;
    case 0x22:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4E, 0, 0);
        break;
    case 0x23:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x4F, 0, 0);
        break;
    case 0x24:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x50, 0, 0);
        break;
    case 0x25:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x50, 0, 1);
        break;
    case 0x2B:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x50, 0, 2);
        break;
    case 0x26:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x51, 0, 0);
        break;
    case 0x27:
        w->pDragon = GetCtrlDragon(0);
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        BitOff16(em->atari.m_flag, 0x200);
        BitOff16(em->atari.m_flag, 0x100);
        EmRoutineSet(em, 1, 0x52, 0, 0);
        break;
    case 0x28:
        w->pDragon = GetCtrlDragon(1);
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        BitOff16(em->atari.m_flag, 0x200);
        BitOff16(em->atari.m_flag, 0x100);
        EmRoutineSet(em, 1, 0x53, 0, 0);
        break;
    case 0x29:
        w->pDragon = GetCtrlDragon(2);
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        BitOff16(em->atari.m_flag, 0x200);
        BitOff16(em->atari.m_flag, 0x100);
        EmRoutineSet(em, 1, 0x54, 0, 0);
        break;
    case 0x2A:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x55, 0, 0);
        break;
    case 0x2C:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x56, 0, 0);
        break;
    case 0x31:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x56, 0, 1);
        break;
    case 0x2D:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x57, 0, 0);
        break;
    case 0x2E:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x58, 0, 0);
        break;
    case 0x32:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x5A, 0, 0);
        break;
    case 0x33:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x5B, 0, 0);
        break;
    case 0x34:
        em10SetWaitMotion(em, 0);
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x5C, 0, 0);
        break;
    case 0x35:
        em10SetWaitMotion(em, 0);
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x5E, 0, 0);
        break;
    case 0x36:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x60, 0, 0);
        break;
    case 0x37:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x61, 0, 0);
        break;
    case 0x38:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x62, 0, 0);
        break;
    case 0x39:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x63, 0, 0);
        break;
    case 0x3A:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x64, 0, 0);
        break;
    case 0x3B:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        w->Scale.x = 1.0f;
        w->Scale.y = 1.0f;
        w->Scale.z = 1.0f;
        EmRoutineSet(em, 1, 0x66, 0, 0);
        break;
    case 0x3C:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x68, 0, 0);
        break;
    case 0x3D:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x69, 0, 0);
        break;
    case 0x41:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x6E, 0, 0);
        break;
    case 0x3E:
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x6A, 0, 0);
        break;
    case 0x3F: {
        MotionData* mot = (MotionData*) PL_ARC_PTR(em->subArc, 0x67);
        MotionSetCore(em, MOTION(em), mot, 0, 0, 1, (f32) (u32) ((mot->maxFrame & 0x3FFF) - 1), 0);
        em->hp = 0;
        em->clearStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x6B, 0, 0);
        break;
    }
    case 0x40:
        w->Be_flg |= 0x100;
        em10SetWaitMotion(em, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        EmRoutineSet(em, 1, 0x6C, 0, 0);
        break;
    }
    if (em->type == 6) {
        em->clearStatus(EM_STATUS_ACTIVE);
    }
    switch (em->type) {
    case 2:
        em10BeltSet(em);
        break;
    case 0xA:
    case 0xD:
        em10ChainSet(em);
        break;
    }
}

// R1 == 0x6B: room 321 corpse: goes straight to the Die_Cramp state (R0 3 / R1 0).
// FUNCTION: BIO4 0x00429180
// STATUS: matching
void em10_R1_R321DeadBody(cEm10* em)
{
    EmRoutineSet(em, 3, 0, 0, 0);
}

// Branch check of Back (0x12): only the goto check while alive.
// FUNCTION: BIO4 0x0043E0B0
// STATUS: stub
void em10_R1_br_Back(cEm10* em)
{
    if (em->hp > 0) {
        em10GotoCk(em);
    }
}

// R0 == 2: damage reaction. Marks the Ganado down (work flag 8) and runs Em10_R1_dmg_tbl[r_no_1].
// FUNCTION: BIO4 0x0042B1E0
// STATUS: matching
void em10_R0_Damage(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    w->Be_flg |= 8;
    Em10_R1_dmg_tbl[em->r_no_1](em);
}

// 1 when the parasite core object (pCore) is out.
// FUNCTION: BIO4 0x00433C20
// STATUS: matching
int cEm10::ckParasite()
{
    return EM10_WK(this)->pCore != 0;
}

// 1 while the dynamite fuse is lit (Fire_timer).
// FUNCTION: BIO4 0x00433C30
// STATUS: matching
int cEm10::ckBombFire()
{
    // PC: the timer is f32 and the test became `> 0`.
    return EM10_WK(this)->Fire_timer > 0.0f;
}

// 1 while the Ganado still carries its shield.
// FUNCTION: BIO4 0x00433C50
// STATUS: matching
int cEm10::ckShiled()
{
    return EM10_WK(this)->pShield != 0;
}

// 1 on the frame the bowgun Ganado fires (Atk_trg), for the room scripts.
// FUNCTION: BIO4 0x00433C60
// STATUS: matching
int cEm10::ckBowgunFire()
{
    Em10Work* w = EM10_WK(this);

    if (w->Wep_type != 8) {
        return 0;
    } else {
        if (w->Atk_trg == 0) {
            return 0;
        }
        return 1;
    }
}

// ===== cEm10 accessors and small helpers =====
// FUNCTION: BIO4 0x00433C80
// STATUS: matching
u32 cEm10::ckGoto()
{
    return EM10_WK(this)->Goto_mode;
}

// Remembers the switch object the room hands the Ganado (pSwitch, R227Barrel / Goto).
// FUNCTION: BIO4 0x00433EF0
// STATUS: matching
void cEm10::setSwitch(cModel* sw)
{
    EM10_WK(this)->pSwitch = sw;
}

// 1 while Throw_timer runs (the Ganado just threw something; cEm::checkThrow for the weapon code).
// FUNCTION: BIO4 0x004361E0
// STATUS: matching
int cEm10::checkThrow()
{
    // PC: the timer is f32 and the test became `> 0`.
    if (!(EM10_WK(this)->Throw_timer > 0.0f)) {
        return 0;
    }
    return 1;
}

// 1 when the room may reset this Ganado (Reset_enable).
// FUNCTION: BIO4 0x00436200
// STATUS: matching
int cEm10::ckResetEnable()
{
    if (EM10_WK(this)->Reset_enable == 0) {
        return 0;
    }
    return 1;
}

// Changes the set number (cEm::set and the St_set copy) from the room script.
// FUNCTION: BIO4 0x00436210
// STATUS: matching
void cEm10::chgSet(u8 no)
{
    EM10_WK(this)->St_set = no;
    set = no;
}

// Room 11D: the chainsaw appearance motion (evtMot[0]) for R11DAppear2.
// FUNCTION: BIO4 0x004362B0
// STATUS: matching
void cEm10::setR11DMotion(void* m0)
{
    EM10_WK(this)->evtMot[0] = m0;
}

// Room 209 gatling mode from the room script (Gatling_mode).
// FUNCTION: BIO4 0x00436380
// STATUS: matching
void cEm10::setGatlingMode(u8 no)
{
    EM10_WK(this)->Gatling_mode = no;
}

// Stores the crane hang / drop motions (evtMot[0/1]) and starts UFOCatch (0x65).
// FUNCTION: BIO4 0x004363A0
// STATUS: matching
void cEm10::setUFOCatch(void* m0, void* m1)
{
    Em10Work* w = EM10_WK(this);

    w->evtMot[0] = m0;
    w->evtMot[1] = m1;
    EmRoutineSet(this, 1, 0x65, 0, 0);
}

// 1 while the Ganado holds a weapon.
// FUNCTION: BIO4 0x00439EB0
// STATUS: matching
int cEm10::ckWeapon()
{
    return EM10_WK(this)->pWeapon != 0;
}

// 1 while the Ganado is carrying Ashley off (work flag 0x4000).
// FUNCTION: BIO4 0x00439EC0
// STATUS: matching
int cEm10::ckTakeAway()
{
    if (EM10_WK(this)->Be_flg & 0x4000) {
        return 1;
    }
    return 0;
}

// Room 305 script: 1 while the bomber (R1 0x6A) is in its waiting step (r_no_2 == 1) and may be told to throw.
// FUNCTION: BIO4 0x00439ED0
// STATUS: matching
int cEm10::ckR305BomberEnable()
{
    if (r_no_0 != 1) {
        return 0;
    }
    if (r_no_1 != 0x6A) {
        return 0;
    }
    return r_no_2 == 1;
}

// Hit mark effect for a melee hit: EstSet 0x25 (0x38 in water) at the hip part 4 facing the damage source.
// FUNCTION: BIO4 0x00429070
// STATUS: stub
void em10KickHitMark(cEm10* em)
{
    cModel* parts;
    Vec rot;

    parts = em->getPartsPtr(4);
    rot.x = 0.0f;
    rot.y = GetXZAngle(&em->dmg.m_PosFrom, &em->pos);
    rot.z = 0.0f;
    if (ChkWaterEffectEnable(&em->pos)) {
        EstSet(0, -1, &parts->world, &rot, EFF_EM10, 0x38, 0, ESP_CORE_KIND_NONE, 0, 0);
    } else {
        EstSet(0, -1, &parts->world, &rot, EFF_EM10, 0x25, 0, ESP_CORE_KIND_NONE, 0, 0);
    }
}

// R0 == 1: normal life. Runs the branch check and the move handler of routine r_no_1 (Em10_R1_move_tbl).
// FUNCTION: BIO4 0x00429120
// STATUS: matching
void em10_R0_Move(cEm10* em)
{
    Em10_R1_move_tbl[em->r_no_1 * 2](em);
    Em10_R1_move_tbl[em->r_no_1 * 2 + 1](em);
}

// R1 == 0x61: room 21B trolley Ganado. On the release flag jumps down at once (JumpDown 0x43 with
// flags 0x10080000: airborne, no scenario adjust).
// FUNCTION: BIO4 0x0043C3E0
// STATUS: stub
void em10_R1_R21BTrolleyJump(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        em10MotionMove(em);
        if (em->flag & 1) {
            w->Target_pos = em->pos;
            em->set = 0;
            w->Be_flg |= 0x10080000;
            em->r_no_0 = 1;
            em->r_no_1 = 0x43;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}

// R1 == 0x64 Work: plain idle at a work post until the player is seen (em10FindCk), then walks.
// FUNCTION: BIO4 0x00479710
// STATUS: stub
void em10_R1_Work(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        em10MotionMove(em);
        break;
    }
    if (!em10FindCk(em, 0)) {
        if (w->Be_flg & 0x100) {
            em10WalkRtnSet(em);
        } else {
            em10HandSet(em, 0);
        }
    }
}

// R1 == 0x66: room 300 Ganado carrying Ashley off: plays motion 0x2A0/0x2A6 once.
// FUNCTION: BIO4 0x0043D8A0
// STATUS: stub
void em10_R1_R300TakeAshley(cEm10* em)
{
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2A0), PL_ARC_PTR(em->subArc, 0x2A6), 0, 5, 0);
        em->r_no_2++;
    case 1:
        em10MotionMove(em);
        break;
    }
    em10HandSet(em, 0);
}

// Branch check of EvtDash (0x0A): the obstacle actions on the way (door, rack, ladders, jump, window).
// FUNCTION: BIO4 0x0045DDD0
// STATUS: stub
void em10_R1_br_EvtDash(cEm10* em)
{
    if (!em10DoorOpenCk(em, 0) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em) && !em10ClimbOverCk(em)) {
        em10WindowCk(em);
    }
}

// Branch check of EvtWalk (0x0B): the obstacle actions on the way (door, rack, ladders, jump, window).
// FUNCTION: BIO4 0x0045DE60
// STATUS: stub
void em10_R1_br_EvtWalk(cEm10* em)
{
    if (!em10DoorOpenCk(em, 0) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em) && !em10ClimbOverCk(em)) {
        em10WindowCk(em);
    }
}

// Branch check of C_SawAtk (0x2F): on the saw's hit frames em10CsawHitCk decides whether the saw
// caught the player (C_SawHit 0x30).
// FUNCTION: BIO4 0x0043EC20
// STATUS: stub
void em10_R1_br_C_SawAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0 && (em->Motion.Seq_old.Free & 1) && w->pWeapon && em10CsawHitCk(em)) {
        EmRoutineSetW(em, 1, 0x30, 0, 0);
    }
}

// Branch check of C_SawCriAtk (0x31): em10CsawHitCk on the hit frames goes to C_SawCriHit (0x32).
// FUNCTION: BIO4 0x0043F140
// STATUS: stub
void em10_R1_br_C_SawCriAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0 && (em->Motion.Seq_old.Free & 1) && w->pWeapon && em10CsawHitCk(em)) {
        if (fabsf(pPL->pos.y - em->pos.y) > 50.0f) {
            EmRoutineSetW(em, 1, 0x30, 0, 0);
        } else {
            EmRoutineSetW(em, 1, 0x32, 0, 0);
        }
    }
}

// R0 == 3: death. Marks the Ganado down (work flag 8) and runs Em10_R1_die_tbl[r_no_1].
// FUNCTION: BIO4 0x0042B210
// STATUS: stub
void em10_R0_Die(cEm10* em)
{
    EM10_WK(em)->Be_flg |= 8;
    Em10_R1_die_tbl[em->r_no_1](em);
}

// Random EMI wander point of the room (entries of type 1 sub 3): its index, or -1 when the room has none.
// FUNCTION: BIO4 0x0042B260
// STATUS: stub
extern "C" int em10GetWanderRouteEmi(cEm10* em)
{
    EmiData* emi = pG->pEmi;
    int cnt;
    int i;
    int r;

    if (!emi) {
        return -1;
    }
    cnt = 0;
    // Byte-offset entry address: the `i*64 + 8` DEST_REG giv has a constant addend, so loop.c makes the
    // later `.sub` address giv (+9) the base (`addi r9,emi,9`, type at -1(r9)); `&emi->entry[i]`
    // gives a zero-addend giv that wins the combine (base +0).
    for (i = 0; i < emi->n; i++) {
        EmiEntry* e = (EmiEntry*) ((u8*) emi + 8 + i * 0x40);
        if (e->type != 1) {
            continue;
        }
        if (e->sub != 3) {
            continue;
        }
        cnt++;
    }
    if (cnt == 0) {
        return -1;
    }
    r = Rnd() % cnt;
    cnt = 0;
    // pG->pRoomEmi re-read here: the loop bound is then a gcse PRE copy (`mr r8, r9`) of the entry test's load.
    for (i = 0; i < (pG->pEmi)->n; i++) {
        EmiEntry* e = &pG->pEmi->entry[i];
        if (e->type != 1) {
            continue;
        }
        if (e->sub != 3) {
            continue;
        }
        if (r == cnt) {
            return i;
        }
        cnt++;
    }
    return -1;
}

// Picks a wander destination for a Ganado that lost the player: an EMI wander point when the room
// has them, else a random route-check point (one in four times the point nearest to the player).
// FUNCTION: BIO4 0x0042B310
// STATUS: stub
u32 em10GetWanderRoute(cEm10* em)
{
    int n = em10GetWanderRouteEmi(em);
    if (n >= 0) {
        return n;
    }
    n = RouteCkGetPointNumber();
    if (n <= 0) {
        return -1;
    }
    n = Rnd() % n;
    if ((Rnd() & 3) == 0) {
        n = RouteCkGetNearPoint(&pPL->pos);
    }
    return n;
}

// Position of the wander destination Wander_route (EMI entry or route-check point).
// FUNCTION: BIO4 0x0042B380
// STATUS: stub
extern "C" void em10GetWanderRoutePos(cEm10* em, Vec* pos)
{
    Em10Work* w = EM10_WK(em);
    EmiData* emi = pG->pEmi;

    if (emi && (int) w->Wander_route >= 0 && (int) w->Wander_route < emi->n) {
        EmiEntry* e = &emi->entry[w->Wander_route];
        if (e->type == 1 && e->sub == 3) {
            *pos = e->pos;
            return;
        }
    }
    RouteCkGetPoint(w->Wander_route, pos);
}

// Keeps wander point `no` while the Ganado is more than 1000 units from it, else picks a new one.
// FUNCTION: BIO4 0x0042B3F0
// STATUS: stub
extern "C" u32 em10WanderRouteUpdate(cEm10* em, int no)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;

    // Early return: the label in front of the main path keeps its `mr r3, r31` (see docs/matching.md
    // "early return merged with the final return").
    if (no <= 0) {
        return em10GetWanderRoute(em);
    }
    em10GetWanderRoutePos(em, &pos);
    f32 d = (em->pos.x - pos.x) * (em->pos.x - pos.x) + (em->pos.z - pos.z) * (em->pos.z - pos.z);
    if (!(d < 2250000.0f)) {
        if (w->HoseiCnt <= 30) {
            return no;
        }
    }
    return em10GetWanderRoute(em);
}

// Whether this Ganado may lose its head to a shot: the plain village types (0/1/3/4/0xB/0xC) only
// while the eye glow (parasite) effect is enabled for the room, every other type always.
// FUNCTION: BIO4 0x0042C3E0
// STATUS: stub
#if 0 // PARKED: C2227, pSys
int em10LostHeadCk(cEm10* em)
{
    if (pSys->eff_country != 0) {
        return 1;
    }
    switch (em->type) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 0xB:
    case 0xC:
        if (!GetEm10EyeEffectEnable()) {
            return 0;
        }
        return 1;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    default:
        return 1;
    }
}
#endif // PARKED

// Chainsaw Ganado: puts the sack over the head (pSack, hides the head part, effect 0x55).
// FUNCTION: BIO4 0x0042E690
// STATUS: stub
#if 0 // PARKED: C2227, C2228, ModInfoMgr, addModel, swapModelInfo
extern "C" void em10SackSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModelInfo* info;

    if (w->Wep_type != 4) {
        return;
    }
    if (!w->mot[21] || !w->mot[22]) {
        return;
    }
    info = ModInfoMgr.create(w->mot[21], w->mot[22]);
    if (info) {
        if (w->pSack) {
            em->swapModelInfo(w->pSack->model_addr, info);
        } else {
            em->addModel(info);
        }
        w->pSack = info;
    }
    if ((u8) (em->type - 11) <= 1) {
        if (w->pHead) {
            w->pHead->be_flag &= ~8;
        }
        EstSet(em, -1, 0, 0, EFF_EM10, 0x55, 0, ESP_CORE_KIND_NONE, em, 0);
    }
}
#endif // PARKED

// Climb-over decision: em10ClimbOverCk2 -> ClimbOver (0x3C) or JumpDown (0x43); 1 when a routine was set.
// FUNCTION: BIO4 0x004538F0
// STATUS: stub
int em10ClimbOverCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    // `case 0: default:` first and no trailing `return 0`: case 2's inline `return 0` block is the
    // one the other return-0 paths jump into (with a trailing return the inline copy is deleted).
    switch ((u32) em10ClimbOverCk2(em)) {
    case 0:
    default:
        return 0;
    case 1:
        EmRoutineSet(em, 1, 0x3C, 0, 0);
        return 1;
    case 2:
        if ((em->flag & 0x400) && w->Goto_mode == 0) {
            return 0;
        }
        EmRoutineSet(em, 1, 0x43, 0, 1);
        return 1;
    }
}

// Wrapper of em10ReturnPosCk (the walk / dash branch checks).
// FUNCTION: BIO4 0x0042EB40
// STATUS: stub
void em10ReturnStartPosCk(cEm10* em)
{
    em10ReturnPosCk(em);
}

// Sight check of the idle routines (a: 1 = also require the player within 2000 units of height,
// 2 = never for the claw types). The Ganado finds the player when it sees him (flag bit0) within
// 15000 (6000 when heading somewhere) and 60 deg, or very close, when another Ganado is being hurt
// nearby, on the bell alarm (Status_flg[1] bit29, SeInfo.pos within 25000), when the room forces the
// alert (Status_flg[0] bit23), or when it is dead / headless. Calls em10SetRtnFind and returns 1.
// FUNCTION: BIO4 0x00426B00
// STATUS: stub
#if 0 // PARKED: C2228, EmDeadCk, STA_PL_FIRE, STA_PL_SE_FOOT, STA_SE_BURST, SeInfo, StaFlagChk, l_pl
int em10FindCk(cEm10* em, int a)
{
    Em10Work* w = EM10_WK(em);
    int find = 0;
    int dead;

    if (w->Be_flg & 0x100) {
        return 0;
    }
    if (em->type == 10 || em->type == 13) {
        if (a == 2) {
            return 0;
        }
    }
    if (w->Be_flg & 1) {
        f32 r;
        switch (w->Goto_mode) {
        case 0:
        case 6:
        case 7:
        case 10:
        case 11:
        case 12:
        case 13:
            r = 225000000.0f;
            break;
        default:
            r = 36000000.0f;
            break;
        }
        if (em->l_pl < r) {
            if (w->Pl_rot < 1.0471976f) {
                find = 1;
            }
        }
        if (StaFlagChk(pG, STA_PL_SE_FOOT)) {
            if (em->l_pl < 25000000.0f) {
                find = 1;
            }
        }
        if (em->l_pl < 12250000.0f) {
            find = 1;
        }
        if (a != 0) {
            if (fabsf(em->pos.y - pPL->pos.y) > 2000.0f) {
                find = 0;
            }
        }
    }
    if (em10SomebodyDamageNowCk(em)) {
        find = 1;
    }
    if (!(em->flag & 0x10)) {
        switch (w->Goto_mode) {
        case 0:
        case 6:
        case 7:
        case 10:
        case 11:
        case 12:
        case 13:
            if (StaFlagChk(pG, STA_SE_BURST)) {
                // em3c bell idiom: three identical arms assigning `r` keep the dispatch compares; the
                // override in the distance block makes the arm sets dead (flow deletes them, the
                // compares stay) and `r` a block-local pseudo loaded at the use (local-alloc gives
                // it the next free FPR, f9, and the high r10 because r9/r11 hold pG).
                f32 r;
                switch (pG->SeInfo.type) {
                case 0:
                    r = 25000.0f;
                    break;
                case 1:
                    r = 25000.0f;
                    break;
                default:
                    r = 25000.0f;
                    break;
                }
                {
                    f32 dx = em->pos.x - pG->SeInfo.pos.x;
                    f32 dy = em->pos.y - pG->SeInfo.pos.y;
                    f32 dz = em->pos.z - pG->SeInfo.pos.z;
                    r = 25000.0f;
                    if (dx * dx + dy * dy + dz * dz < r * r) {
                        if ((w->Be_flg & 1) && w->L_pl_route < r) {
                            find = 1;
                        }
                    }
                }
            }
            if (StaFlagChk(pG, STA_PL_FIRE)) {
                if (w->L_pl_route < 25000.0f) {
                    find = 1;
                }
            }
            break;
        }
    }
    dead = EmDeadCk(em);
    if (dead) {
        find = 1;
    }
    if (em->flag & 0x80) {
        find = 1;
    }
    switch (find) {
    case 0:
        return 0;
    case 1:
        em10SetRtnFind(em);
        return 1;
    default:
        return 0;
    }
}
#endif // PARKED

// 1 when the Ganado is alive and has found the player (work flag 0x100).
// FUNCTION: BIO4 0x00433B30
// STATUS: stub
#if 0 // PARKED: checkStatus
int cEm10::ckFindPL()
{
    if (hp <= 0) {
        return 0;
    }
    if (checkStatus(EM_STATUS_ACTIVE) && type != 6 && (EM10_WK(this)->Be_flg & 0x100)) {
        return 1;
    }
    return 0;
}
#endif // PARKED

// Marks the player found (work flag 0x100, clears the lost flag 0x800000 and Lose_timer); ignored by
// dead / inactive Ganados and the robed type 6.
// FUNCTION: BIO4 0x00433B70
// STATUS: stub
#if 0 // PARKED: checkStatus
void cEm10::setFindPL()
{
    Em10Work* w = EM10_WK(this);

    if (hp > 0 && checkStatus(EM_STATUS_ACTIVE) && type != 6) {
        w->Be_flg |= 0x100;
        w->Be_flg &= ~0x800000;
        w->Lose_timer = 0;
        w->Pl_in_ck = 1;
    }
}
#endif // PARKED

// Forgets the player (clears work flags 0x100 / 0x800000, Lose_timer 0) on an alive active Ganado.
// FUNCTION: BIO4 0x00433BD0
// STATUS: stub
#if 0 // PARKED: checkStatus
void cEm10::clearFindPL()
{
    Em10Work* w = EM10_WK(this);

    if (hp > 0 && checkStatus(EM_STATUS_ACTIVE) && type != 6) {
        w->Be_flg &= ~0x100;
        w->Be_flg &= ~0x800000;
        w->Lose_timer = 0;
    }
}
#endif // PARKED

// Resets the jaw / mouth parts 0x1B..0x21 to their rest angles before a new motion (alive Ganados
// with the head still on).
// FUNCTION: BIO4 0x00435530
// STATUS: stub
void em10MouthPartsReset(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* p;

    if (em->hp > 0 && !(w->Be_flg & 0x80)) {
        p = em->getPartsPtr(0x1B);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x1C);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x1D);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x1E);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x1F);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x20);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
        p = em->getPartsPtr(0x21);
        p->ang.x = 0.0f;
        p->ang.y = 0.0f;
        p->ang.z = 0.0f;
    }
}

// Stores the four event motions the room hands over (evtMot[0..3]) for the event routines.
// FUNCTION: BIO4 0x00436230
// STATUS: matching
void cEm10::setEvtMotion(void* m0, void* m1, void* m2, void* m3)
{
    Em10Work* w = EM10_WK(this);

    w->evtMot[0] = m0;
    w->evtMot[4] = m1;
    w->evtMot[1] = m2;
    w->evtMot[5] = m3;
}

// Room 10F: the gondola jump / land / hack / shake motions (evtMot[0..3]) for R10FGJump.
// FUNCTION: BIO4 0x00436270
// STATUS: matching
void cEm10::setGondolaMotion(void* m0, void* m1, void* m2, void* m3)
{
    Em10Work* w = EM10_WK(this);

    w->evtMot[0] = m0;
    w->evtMot[1] = m1;
    w->evtMot[2] = m2;
    w->evtMot[3] = m3;
}

// Room 212: the drill rider motions (evtMot[0..3]) and the drill object; starts R212Drill (0x5C).
// FUNCTION: BIO4 0x004362D0
// STATUS: matching
void cEm10::setDrill(void* m0, void* m1, void* m2, void* m3)
{
    Em10Work* w = EM10_WK(this);

    w->evtMot[0] = m0;
    w->evtMot[1] = m1;
    w->pDrill = (cObj*) m2;
    w->TmpU32 = (int) m3;
}

// Room 209: mounts the Ganado on the gatling `g` (pGatling, setRide) with its fire / reload / hit /
// die motions (evtMot[0..3]) and starts R209Gatling (0x5F); be_flag 0x10000 keeps it from resetting.
// FUNCTION: BIO4 0x00436310
// STATUS: stub
#if 0 // PARKED: cEm10
void cEm10::setGatling(cObjGatling* g, void* m0, void* m1, void* m2, void* m3)
{
    Em10Work* w = EM10_WK(this);

    if (g) {
        w->evtMot[0] = m0;
        w->evtMot[1] = m1;
        w->evtMot[2] = m2;
        w->evtMot[3] = m3;
        w->pGatling = g;
        w->Gatling_mode = 1;
        // Direct u8 routine stores (not EmRoutineSet): the QImode constant 1 is shared with the
        // gatlingMode store, which keeps the pGatling store after the call-argument moves.
        r_no_0 = 1;
        r_no_1 = 0x5F;
        r_no_2 = 0;
        r_no_3 = 0;
        be_flag |= 0x10000;
        g->setRide(this);
    }
}
#endif // PARKED

// Finds the alive truck enemy (id 0x3B) for the driver (pTruck); returns non-NULL when found.
// FUNCTION: BIO4 0x0044B1F0
// STATUS: stub
#if 0 // PARKED: C2227, C2228, EM3B_WK, EmMgr
extern "C" cModel* em10SearchTruck(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;

    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if ((e->be_flag & 0x201) == 1 && e->id == 0x3B) {
            EM3B_WK(e)->pDriver = em;
            w->pTruck = e;
            return (cModel*) 1;
        }
    }
    return 0;
}
#endif // PARKED

// On a killing hit to a cEm::flag bit20 Ganado: instead of dying the parasite may burst out of the
// head (em10LostHead mode 2, at most two at a time through the ctrl12 CNT_PARASITE count, 30%
// chance). 1 = it did (the damage reaction is skipped).
// FUNCTION: BIO4 0x00457C80
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_CNT_PARASITE, Ctrl12CntCk
int em10ChgParasiteCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (!(em->flag & 0x100000) || em->hp > 0) {
        return 0;
    }
    if (w->Be_flg & 0x80) {
        return 0;
    }
    if (Ctrl12CntCk(w->pCtrlGroup, CTRL12_ID_CNT_PARASITE, 2)) {
        return 0;
    }
    if (em->type != 8) {
        u8 r = Rnd() % 10;
        if (r > 4) {
            return 0;
        }
    }
    if (!em10LostHead(em, 2, 0)) {
        return 0;
    }
    return 1;
}
#endif // PARKED

// Attack cooldown by rank (30 / 45 / 75 frames for rank > 3 / <= 3 / <= 1) into Atk_wait; `set`
// also writes it to the room's ctrl12 slots 6 / 8 (the catch attacks lock everybody).
// FUNCTION: BIO4 0x00438910
// STATUS: stub
#if 0 // PARKED: Ctrl12Set, Game_level
extern "C" void em10SetAtkWait(cEm10* em, int set)
{
    Em10Work* w = EM10_WK(em);
    int t = 30;

    if (pG->Game_level <= 3) {
        t = 45;
    }
    if (pG->Game_level <= 1) {
        t = 75;
    }
    w->Atk_wait = t;
    if (set) {
        Ctrl12Set(w->pCtrlGroup, 6, (s16) t);
        Ctrl12Set(w->pCtrlGroup, 8, (s16) t);
    }
}
#endif // PARKED

// Claw types (0xA / 0xD): when the player is found / the target known, refreshes the sight and goes
// to the critical claw attack or the StickClaw pull-out (0x59). 1 when set.
// FUNCTION: BIO4 0x00438C30
// STATUS: stub
int em10ClawStickCK(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;

    if (em->type != 10 && em->type != 13) {
        return 0;
    }
    // `r` holds the flag test (callee-saved r29, live across the calls) and is reused for the
    // attack-check result; cse writes the zeros of both paths through it.
    r = w->Be_flg & 0x100;
    if (!r) {
        if (!em10FindCk2(em)) {
            return 0;
        }
        em->setFindPL();
        w->Dash_wait = 0;
    }
    if (w->Claw_rno_l == 2 && w->Claw_rno_r == 2) {
        // `||`: the inner label blocks the `li r3, 0` hoist in front of `bne`
        if (!em10FindCk2(em) || (w->Be_flg & 0x08000000)) {
            return 0;
        }
        r = em10ClawCriAtkCk(em);
        if (r) {
            return 1;
        }
        if ((em->pos.x - w->Pl_pos.x) * (em->pos.x - w->Pl_pos.x) + (em->pos.z - w->Pl_pos.z) * (em->pos.z - w->Pl_pos.z) > 16000000.0f &&
            (em->r_no_0 != 1 || em->r_no_1 != 0x11)) {
            em10CallVoiceSe2(em, 0x71, 6);
            w->Route_type = 0;
            EmRoutineSet(em, 1, 0x11, 0, 0);
            return 1;
        }
        return 0;
    }
    if (em10ClawCriAtkCk(em)) {
        return 1;
    }
    EmRoutineSet(em, 1, 0x59, 0, 0);
    return 1;
}

// A claw type that has lost the player for a while (Wander_timer) with no target goes to FindLost (0x5D). 1 when set.
// FUNCTION: BIO4 0x00428F90
// STATUS: stub
int em10FindLostCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->type != 10 && em->type != 13) {
        return 0;
    }
    if (em10FindCk2(em)) {
        w->Find_timer = 150;
    }
    if (w->Find_timer != 0) {
        w->Find_timer--;
        if ((s16) w->Find_timer == 0) {
            EmRoutineSet(em, 1, 0x5D, 0, 0);
            return 1;
        }
    }
    return 0;
}

// The room 10F gondola object (id 0x35) the player rides (ckRide), or NULL.
// FUNCTION: BIO4 0x0044B620
// STATUS: stub
#if 0 // PARKED: C2227, C2228, ObjMgr, cObj, cObjGondola
cObjGondola* em10GetGondola(cEm10* em)
{
    cObj* o;

    for (o = ObjMgr.getActiveWork(); o; o = ObjMgr.getNext(o)) {
        if (o->id == 0x35 && ((cObjGondola*) o)->ckRide()) {
            return (cObjGondola*) o;
        }
    }
    return 0;
}
#endif // PARKED

// Room script: gives an unarmed Ganado the weapon model bin / tpl as Wep_type `type` (created and
// attached like em10WeaponInit's).
// FUNCTION: BIO4 0x00439E10
// STATUS: stub
#if 0 // PARKED: SetWeapon
void cEm10::setWeapon(void* bin, void* tpl, int type)
{
    Em10Work* w = EM10_WK(this);
    Vec pos;
    Vec rot;

    if (!w->pWeapon) {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        w->pWeapon = SetWeapon(bin, tpl, &pos, &rot, 0);
        if (w->pWeapon) {
            w->Wep_type = type;
            em10WepSeEffSet(this, w->pWeapon, w->Wep_type);
            em10WeaponSet(this);
        }
    }
}
#endif // PARKED

// Suspend / resume (be_flag 0x800) the Ganado and every object it owns (weapons, cap, glasses, cart,
// shield, belt, chain, parasite, core, tentacles); pointers to objects that died are dropped here.
// FUNCTION: BIO4 0x00428D30
// STATUS: stub
#if 0 // PARKED: C2227, cObj12, isAlive, setNoSuspend
void cEm10::setNoSuspend(int on)
{
    Em10Work* w = EM10_WK(this);
    u32 i;

    if (on) {
        be_flag |= 0x800;
    } else {
        be_flag &= ~0x800;
    }
    if (w->pWeapon) {
        if (w->pWeapon->isAlive()) {
            w->pWeapon->setNoSuspend(on);
        } else {
            w->pWeapon = 0;
        }
    }
    if (w->pWeapon2) {
        if (w->pWeapon2->isAlive()) {
            w->pWeapon2->setNoSuspend(on);
        } else {
            w->pWeapon2 = 0;
        }
    }
    if (w->pCap) {
        if (w->pCap->isAlive()) {
            w->pCap->setNoSuspend(on);
        } else {
            w->pCap = 0;
        }
    }
    if (w->pGlasses) {
        if (w->pGlasses->isAlive()) {
            w->pGlasses->setNoSuspend(on);
        } else {
            w->pGlasses = 0;
        }
    }
    if (w->pCart) {
        if (w->pCart->isAlive()) {
            w->pCart->setNoSuspend(on);
        } else {
            w->pCart = 0;
        }
    }
    if (w->pShield) {
        if (w->pShield->isAlive()) {
            w->pShield->setNoSuspend(on);
        } else {
            w->pShield = 0;
        }
    }
    if (w->pGunBelt) {
        if (w->pGunBelt->isAlive()) {
            w->pGunBelt->setNoSuspend(on);
        } else {
            w->pGunBelt = 0;
        }
    }
    if (w->pChain) {
        if (w->pChain->isAlive()) {
            w->pChain->setNoSuspend(on);
        } else {
            w->pChain = 0;
        }
    }
    if (w->pParasite) {
        if (w->pParasite->isAlive()) {
            w->pParasite->setNoSuspend(on);
        } else {
            w->pParasite = 0;
        }
    }
    if (w->pCore) {
        if (w->pCore->isAlive()) {
            w->pCore->setNoSuspend(on);
        } else {
            w->pCore = 0;
        }
    }
    for (i = 0; i < 5; i++) {
        if (w->pTen[i]) {
            if (w->pTen[i]->isAlive()) {
                w->pTen[i]->setNoSuspend(on);
            } else {
                w->pTen[i] = 0;
            }
        }
    }
}
#endif // PARKED

// Per-frame damage check, first thing in cEm10::move (r_no_0 != 0). Damage volumes (DmgMgr kind
// 1/4/5/7 = explosions / fire) blow the Ganado away or kill it outright depending on where it is
// (ladder, fence, gondola, down); a weapon hit (dmHit) takes em10SetDmVal off hp, dispatches the
// reaction on the weapon id through Em10DmSetWep_tbl (rifles on a chainsaw Ganado go to the heavy
// reaction), rings the "bell" damage notify (Status_flg[1] bit29 + SeInfo.pos), lights a bowgun
// Ganado's arrow on a hit to its quiver part, and makes the Ganado find the player.
// FUNCTION: BIO4 0x00457D30
// STATUS: stub
#if 0 // PARKED: C2051, C2227, C2228, DMG_TYPE_ENV_FIRE, DMG_TYPE_FIRE, DMG_TYPE_FLAME, DMG_TYPE_LAMP, DmgMgr, EmDeadCk, GameAddPoint, LVADD_CRITICALHIT, LifeDownSet, LifeDownSet2, STA_SE_BURST, SeInfo, StaFlagChk, StaFlagOn, YARARE_INFO, pLog, part
void em10DmCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int dmg;

    if (em10CrashCk(em)) {
        return;
    }
    if ((em->be_flag & 2) && !EmDeadCk(em) && em->hp > 0) {
        switch (DmgMgr.hitCheck(&em->pos, 0)) {
        case DMG_TYPE_FIRE:
        case DMG_TYPE_FLAME:
        case DMG_TYPE_LAMP:
        case DMG_TYPE_ENV_FIRE:
            if (w->Eff_timer == 0) {
                if (w->pGatling) {
                    w->Eff_timer = 120;
                    EstSet(em, -1, 0, 0, EFF_EM10, 0x1E, 0, ESP_CORE_KIND_NONE, em, 0);
                    LifeDownSet(em, 1200, 0);
                    if (em->hp > 0) {
                        if (w->No_dmg_timer == 0) {
                            em->r_no_2 = 4;
                        }
                    } else {
                        em->r_no_2 = 6;
                    }
                    return;
                }
                if ((em->r_no_0 == 1 && em->r_no_1 == 0x5E) && (w->Be_flg & 8)) {
                    w->Eff_timer = 120;
                    EstSet(em, -1, 0, 0, EFF_EM10, 0x1E, 0, ESP_CORE_KIND_NONE, em, 0);
                    return;
                }
                w->Eff_timer = 120;
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1E, 0, ESP_CORE_KIND_NONE, em, 0);
                if (w->Be_flg & 0x4000) {
                    LifeDownSet(em, 500, 0);
                    EmRoutineSet(em, 2, 0xC, 0, 0);
                } else if (w->Be_flg & 0x20000) {
                    LifeDownSet(em, 500, 0);
                    EmRoutineSet(em, 2, 5, 0, 0);
                } else if (w->Be_flg & 0x10000) {
                    LifeDownSet(em, 500, 0);
                    EmRoutineSet(em, 2, 6, 0, 0);
                } else if (w->Be_flg & 0x10000000) {
                    LifeDownSet(em, 500, 0);
                    if (em->hp > 0) {
                        return;
                    }
                    EmRoutineSet(em, 2, 4, 0, 0);
                } else if (w->Be_flg & 0x100000) {
                    LifeDownSet(em, 500, 0);
                    EmRoutineSet(em, 2, 4, 0, 0);
                } else if (em->type == 0xA || em->type == 0xD) {
                    LifeDownSet(em, 200, 0);
                    if (em->hp <= 0) {
                        EmRoutineSet(em, 2, 0xB, 0, 1);
                    } else {
                        em->dmg.m_PosFrom = pPL->pos;
                        EmRoutineSet(em, 2, 0xE, 0, 0);
                    }
                } else if (em->type == 2) {
                    LifeDownSet(em, 200, 0);
                    if (em->hp <= 0) {
                        EmRoutineSet(em, 2, 0xB, 0, 1);
                    } else {
                        EmRoutineSet(em, 2, 0x11, 0, 0);
                    }
                } else if (w->Be_flg & 0x10) {
                    LifeDownSet(em, 500, 0);
                    if (em->hp <= 0) {
                        EmRoutineSet(em, 3, 1, 0, 0);
                    } else {
                        w->WakeTimer = 0;
                        EmRoutineSet(em, 2, 0xA, 0, 0);
                    }
                } else {
                    EmRoutineSet(em, 2, 0xB, 0, 1);
                }
                return;
            }
            break;
        }
    }
    if (em->dmg.m_Flag == 0) {
        return;
    }
    if ((w->Be_flg & 0x200000) && w->pLadder) {
        w->pLadder->setDown2();
        w->pLadder = 0;
    }
    if (w->pTruck) {
        em->dmg.m_Flag = 0;
        if (em->hp <= 0) {
            return;
        }
        em10LostHead(em, 0, 0);
        em->hp = 0;
        GameAddPoint(LVADD_CRITICALHIT);
        return;
    }
    if (w->pDrill) {
        em->dmg.m_Flag = 0;
        if (em->hp <= 0) {
            return;
        }
        if (em->dmg.m_pDamageYarare->parts_no == 5) {
            em10LostHead(em, 0, 0);
        } else {
            em10BloodSet(em, 0);
        }
        em->hp = 0;
        GameAddPoint(LVADD_CRITICALHIT);
        return;
    }
    if ((em->r_no_0 == 1 && em->r_no_1 == 0x5E) && (w->Be_flg & 8)) {
        em10BloodSet(em, 0);
        if (!StaFlagChk(pG, STA_SE_BURST)) {
            StaFlagOn(pG, STA_SE_BURST);
            pG->SeInfo.pos = em->pos;
            pG->SeInfo.type = 0;
        }
        em->dmg.m_Flag = 0;
        return;
    }
    dmg = em10SetDmVal(em);
    LifeDownSet2(em, dmg, 0, 0);
    w->Claw_hp -= dmg;
    if (w->Claw_hp & 0x8000) {
        w->Claw_hp = 0;
    }
    if (em->r_no_0 == 1 && em->r_no_1 == 0x5F) {
        em->dmg.m_Flag = 0;
        em10BloodSet(em, 0);
        if (em->hp > 0) {
            if (w->No_dmg_timer == 0) {
                em->r_no_2 = 4;
            }
        } else {
            em->r_no_2 = 6;
        }
        return;
    }
    if (em->type == 6) {
        if (em->dmg.m_Wep == 0x14) {
            return;
        }
        if (em->dmg.m_Wep == 0x16) {
            return;
        }
        if (em->dmg.m_Wep == 0x17) {
            return;
        }
        if (em->dmg.m_Wep == 0x2A) {
            return;
        }
        if (em->dmg.m_Wep == 0xE) {
            return;
        }
        em->hp = 0;
    }
    if (em->hp <= 0) {
        em10CoreBreak(em, 0);
    }
    if (em->dmg.m_Wep <= 0x2D) {
        u32 no = em->dmg.m_Wep;
        if ((em->dmg.m_Wep == 9 || em->dmg.m_Wep == 10) && w->Wep_type == 4) {
            no = 5;
        }
        Em10DmSetWep_tbl[no](em);
    } else {
        pLog->err(0, 0, "em10DmCk() -> Wep no over max!!!");
    }
    if (em->dmg.m_Wep == 0x10) {
        em->dmg.m_Timer = 0x11;
    }
    if (em10FindCk2(em)) {
        w->Be_flg &= ~0x20000000;
        em->setFindPL();
    }
    if ((s16) w->Anger_timer < 300) {
        w->Anger_timer = 300;
    }
    switch (w->Goto_mode) {
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
        if (em->type == 0xA || em->type == 0xD) {
            if (em10FindCk2(em)) {
                w->Be_flg &= ~0x20000000;
                em->setFindPL();
            }
        } else {
            em->setFindPL();
            w->Goto_mode = 0;
        }
        break;
    }
    if (em->Character == 0 || em->Character == 2) {
        w->Pl_in_ck = 1;
    }
    if (w->Wep_type == 9) {
        int parts = 9;
        YARARE_INFO* part = em->dmg.m_pDamageYarare;
        if (em->flag & 0x1000000) {
            parts = 0xF;
        }
        if (part->parts_no == parts) {
            w->Fire_timer = 3;
            GameAddPoint(LVADD_CRITICALHIT);
        }
    }
    if (!StaFlagChk(pG, STA_SE_BURST)) {
        StaFlagOn(pG, STA_SE_BURST);
        pG->SeInfo.pos = em->pos;
        pG->SeInfo.type = 0;
    }
    em->dmg.m_Flag = 0;
}
#endif // PARKED

// Damage reaction to the melee "hand" weapons (dmWep 0, 0x14, 0x22..0x25 = the player kick / suplex
// hits): hit mark, then the blow (Dm_Blow), knee-down, take-away / fence / ladder / down variants by
// work flag, Wesker's Dm_Heel, or Dm_Showtay for weapon 0x25; a killing blow at the head loses it.
// FUNCTION: BIO4 0x00458800
// STATUS: stub
#if 0 // PARKED: C2227, SndCall, YARARE_INFO, part, set
void em10DmSetWep00(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    YARARE_INFO* part = em->dmg.m_pDamageYarare;

    em->dmg.set(0, 8);
    if ((part->parts_no == 5 || part->parts_no == 0x25) && w->pCore) {
        SndCall(8, 0x83, &em->pos, em->id, 0, em);
    } else {
        SndCall(8, 0xC, &em->pos, em->id, 0, em);
    }
    if (em->dmg.m_Wep == 0x25) {
        EmRoutineSet(em, 2, 0x12, 0, 0);
        return;
    }
    em10KickHitMark(em);
    if ((s16) w->Frame_timer != 0) {
        w->Eff_timer = 120;
        EmRoutineSet(em, 2, 0xB, 0, 0);
    } else if (w->Be_flg & 0x4000) {
        EmRoutineSet(em, 2, 0xC, 0, 0);
    } else if (w->Be_flg & 0x20000) {
        if (em->hp > 0 && em->type == 0x16) {
            return;
        }
        EmRoutineSet(em, 2, 5, 0, 0);
    } else if (w->Be_flg & 0x10000) {
        if (em->hp > 0 && em->type == 0x16) {
            return;
        }
        EmRoutineSet(em, 2, 6, 0, 0);
    } else if (w->Be_flg & 0x10000000) {
        if (em->hp > 0) {
            return;
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x100000) {
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x40000000) {
        if (pG->pl_type == 5) {
            EmRoutineSet(em, 2, 0x13, 0, 0);
        } else {
            EmRoutineSet(em, 2, 4, 0, 1);
        }
    } else if (em->hp <= 0) {
        em10LostHead(em, 0, 0);
        if ((w->Be_flg & 0x40) && (part->parts_no == 0x13 || part->parts_no == 0x17 || part->parts_no == 0x14 || part->parts_no == 0x18)) {
            EmRoutineSet(em, 2, 3, 0, 0);
        } else {
            EmRoutineSet(em, 2, 4, 0, 1);
        }
    } else if (w->Be_flg & 8) {
        w->WakeTimer = 0;
    } else if (w->Be_flg & 0x10) {
        w->WakeTimer = 0;
        EmRoutineSet(em, 2, 0xA, 0, 0);
    } else if (em->dmg.m_Wep == 0x24) {
        EmRoutineSet(em, 2, 0, 0, 0);
    } else if (pG->pl_type == 5) {
        EmRoutineSet(em, 2, 0x13, 0, 0);
    } else {
        EmRoutineSet(em, 2, 4, 0, 1);
    }
}
#endif // PARKED

// Damage reaction to the standard bullets (handguns, TMP, knife 0x10, ...): armour parts (em10ArmorCk)
// do not flinch, chainsaw / parasite Ganados absorb 2..4 hits (EM10_GUARD_CK), a head shot (parts 5)
// gives Dm_Head or, when it kills, the lost-head parasite chance (em10LostHead); legs (parts
// 0x13/0x14/0x17/0x18) trip a dashing Ganado (Dm_DashDown), arm parts 8/0xE can knock it out; the
// robed type 6 dies at once, type 2 / 0xA / 0xD / 0x16 use the gatling / claw reactions.
// FUNCTION: BIO4 0x00458B90
// STATUS: stub
#if 0 // PARKED: C1003, C2227, SndCall, YARARE_INFO, part
void em10DmSetWep02(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    YARARE_INFO* part = em->dmg.m_pDamageYarare;
    int react;
    int ret;
    int type;

    em->dmg.m_Timer = 1;
    if ((part->parts_no == 5 || part->parts_no == 0x25) && w->pCore) {
        SndCall(8, 0x83, &em->pos, em->id, 0, em);
    } else if (em10ArmorCk(em, part->parts_no)) {
        SndCall(8, 0xD, &em->pos, em->id, 0, em);
    } else {
        SndCall(8, 0xC, &em->pos, em->id, 0, em);
    }
    react = em10ArmorCk(em, part->parts_no) == 0;
    if (em->type == 0xA || em->type == 0xD) {
        if (Rnd() & 3) {
            react = 0;
        }
    }
    if ((s16) w->Frame_timer != 0) {
        w->Eff_timer = 120;
        EmRoutineSet(em, 2, 0xB, 0, 0);
    } else if (w->Be_flg & 0x4000) {
        if (em->hp <= 0 && part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (w->Wep_type == 4 || w->pCore || w->pParasite) {
                EM10_GUARD_CK(w);
            }
        }
        if (react) {
            EmRoutineSet(em, 2, 0xC, 0, 0);
        }
    } else if (w->Be_flg & 0x20000) {
        if (em->hp <= 0 && part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
            if (w->Wep_type == 4 || w->pCore || w->pParasite) {
                EM10_GUARD_CK(w);
            }
        }
        if (react) {
            EmRoutineSet(em, 2, 5, 0, 0);
        }
    } else if (w->Be_flg & 0x10000) {
        if (em->hp <= 0 && part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
            if (w->Wep_type == 4 || w->pCore || w->pParasite) {
                EM10_GUARD_CK(w);
            }
        }
        if (react) {
            EmRoutineSet(em, 2, 6, 0, 0);
        }
    } else if (w->Be_flg & 0x10000000) {
        em10BloodSet(em, 0);
        if (em->hp > 0) {
            return;
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x100000) {
        if (em->hp <= 0 && part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
        }
        if (react) {
            EmRoutineSet(em, 2, 4, 0, 0);
        }
    } else if (w->Be_flg & 0x40000000) {
        if (em->hp <= 0 && part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
            if (w->Wep_type == 4 || w->pCore || w->pParasite) {
                EM10_GUARD_CK(w);
            }
        }
        if (react) {
            EmRoutineSet(em, 2, 8, 0, 0);
        }
    } else {
        if (em10ChgParasiteCk(em)) {
            return;
        }
        if (em->hp <= 0) {
            EmSetDie(em);
            EmReserveDropItem(em);
            em10SetPoint(em);
            if (em->type == 0xA || em->type == 0xD || em->type == 2 || em->type == 0x16) {
                em10BloodSet(em, 0);
                EmRoutineSet(em, 3, 2, 0, 0);
                return;
            }
            if (w->Be_flg & 0x10) {
                if (part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
                    em10LostHead(em, 1, 0);
                    GameAddPoint(LVADD_CRITICALHIT);
                } else {
                    em10BloodSet(em, 0);
                }
                EmRoutineSet(em, 3, 1, 0, 0);
                return;
            }
            if ((w->Be_flg & 0x40) && (part->parts_no == 0x13 || part->parts_no == 0x17 || part->parts_no == 0x14 || part->parts_no == 0x18)) {
                EmRoutineSet(em, 2, 3, 0, 0);
                return;
            }
            if (em10RoofDmCk(em)) {
                em10BloodSet(em, 0);
                return;
            }
            if (part->parts_no == 5 && em->dmg.m_Wep != 0x10) {
                u8 r;
                em10LostHead(em, 0, 0);
                GameAddPoint(LVADD_CRITICALHIT);
                r = Rnd() % 3;
                if (r == 0 || !em10LostHeadCk(em)) {
                    EmRoutineSet(em, 2, 4, 0, 0);
                    return;
                }
                if (em->r_no_0 == 1 && (em->r_no_1 == 0x10 || em->r_no_1 == 0x39 || em->r_no_1 == 0x33) && em10LostHeadCk(em) && w->Wep_type != 4) {
                    if (pG->stage_no == 1 && pG->room_no == 0) {
                        em->r_no_0 = 2;
                        em->r_no_1 = 9;
                        em->r_no_3 = em->r_no_2 = 0;
                    }
                    if ((Rnd() & 0xF) == 5) {
                        w->Die_wait = 0x5A;
                        em->hp = 1;
                    }
                    return;
                }
                EmRoutineSet(em, 2, 9, 0, 0);
                return;
            }
            if (part->parts_no == 8 || part->parts_no == 0xE) {
                u8 r = Rnd() % 3;
                if (r != 0) {
                    EmRoutineSet(em, 2, 4, 0, 0);
                    StaFlagOn(pG, STA_CRITICAL);
                    return;
                }
            }
            em->r_no_0 = 3;
            em->r_no_1 = 2;
            em->r_no_3 = em->r_no_2 = 0;
            em10BloodSet(em, 0);
            return;
        }
        if (w->Be_flg & 8) {
            w->WakeTimer = 0;
            em10BloodSet(em, 0);
            return;
        }
        if (w->Be_flg & 0x10) {
            w->WakeTimer = 0;
            em10BloodSet(em, 0);
            if (react) {
                EmRoutineSet(em, 2, 0xA, 0, 0);
            }
            return;
        }
        em10BloodSet(em, 0);
        ret = em10RoofDmCk(em);
        if (ret) {
            return;
        }
        type = em->type;
        if (type == 6) {
            em->r_no_0 = 2;
            em->r_no_1 = 0;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return;
        }
        if (type == 2) {
            switch (em->dmg.m_Wep) {
            default: {
                u8 r;
                if (part->parts_no != 5) {
                    return;
                }
                r = Rnd() % 10;
                if (r != 0) {
                    return;
                }
                em->r_no_0 = 2;
                em->r_no_1 = 1;
                em->r_no_2 = 0;
                em->r_no_3 = 0;
                return;
            }
            case 9:
            case 10:
                if (part->parts_no == 5) {
                    em->r_no_0 = 2;
                    em->r_no_1 = 1;
                    em->r_no_2 = 0;
                    em->r_no_3 = 0;
                    return;
                } else {
                    u8 r = Rnd() % 10;
                    if (r < 5) {
                        return;
                    }
                    em->r_no_0 = 2;
                    em->r_no_1 = 0x11;
                    em->r_no_2 = 0;
                    em->r_no_3 = 0;
                    return;
                }
            }
        }
        if (part->parts_no == 5 && w->pShield == 0) {
            if (w->Wep_type == 4) {
                EM10_GUARD_CK(w);
            }
            if (!react) {
                return;
            }
            if (w->pCore && w->Ganado == 1) {
                if (Rnd() & 3) {
                    EmRoutineSet(em, 2, 0, 0, 0);
                }
                return;
            }
            EmRoutineSet(em, 2, 1, 0, 0);
            return;
        }
        if (part->parts_no == 0x25) {
            EmRoutineSet(em, 2, 0xF, 0, 0);
            return;
        }
        if (w->Be_flg & 0x40) {
            if (w->Wep_type == 4 || w->pCore || w->pParasite) {
                EM10_GUARD_CK(w);
            }
            if (!react) {
                return;
            }
            if (part->parts_no == 0x13 || part->parts_no == 0x17 || part->parts_no == 0x14 || part->parts_no == 0x18) {
                EmRoutineSet(em, 2, 3, 0, 0);
            } else {
                EmRoutineSet(em, 2, 2, 0, 0);
            }
            return;
        }
        if (em->dmg.m_Wep == 0x10 && pG->pl_type != 4) {
            u8 r = Rnd() % 3;
            if (r == 0) {
                return;
            }
            if (!react) {
                return;
            }
        }
        if (em->type == 0xA || em->type == 0xD) {
            return;
        }
        if (w->Wep_type == 4 || w->pCore || w->pParasite) {
            EM10_GUARD_CK(w);
        }
        if (em->type == 0x16) {
            return;
        }
        if (react) {
            EmRoutineSet(em, 2, 0, 0, 0);
        }
    }
}
#endif // PARKED

// Damage reaction to the shotguns (dmWep 7, 8, 0x21): `near` = muzzle within 6000 units (part->rad).
// A near hit blows the Ganado away (Dm_Blow), a far one flinches like a bullet; a near kill at the head
// loses it, chainsaw Ganados only flinch, shield carriers take Dm_Small.
// FUNCTION: BIO4 0x00459860
// STATUS: stub
#if 0 // PARKED: C2227, GameAddPoint, LVADD_CRITICALHIT, SndCall, YARARE_INFO, part, room_no, stage_no
void em10DmSetWep03(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    YARARE_INFO* part = em->dmg.m_pDamageYarare;
    int near = 0;

    em->dmg.m_Timer = 1;
    if (part->len < 36000000.0f) {
        near = 1;
    }
    em10ArmorCk(em, part->parts_no);
    if (em->type == 0xA || em->type == 0xD || em->type == 2 || em->type == 0x16) {
        Rnd();
    }
    if ((part->parts_no == 5 || part->parts_no == 0x25) && w->pCore) {
        SndCall(8, 0x83, &em->pos, em->id, 0, em);
    } else if (em10ArmorCk(em, part->parts_no)) {
        SndCall(8, 0xD, &em->pos, em->id, 0, em);
    } else if (near) {
        SndCall(8, 0x7A, &em->pos, em->id, 0, em);
    } else {
        SndCall(8, 0xC, &em->pos, em->id, 0, em);
    }
    if ((s16) w->Frame_timer != 0) {
        w->Eff_timer = 120;
        EmRoutineSet(em, 2, 0xB, 0, 0);
    } else if (w->Be_flg & 0x4000) {
        EmRoutineSet(em, 2, 0xC, 0, 0);
    } else if (w->Be_flg & 0x20000) {
        if (em->hp <= 0 && part->parts_no == 5) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, near);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x10000) {
        if (em->hp <= 0 && part->parts_no == 5) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, near);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 6, 0, 0);
    } else if (w->Be_flg & 0x10000000) {
        em10BloodSet(em, 0);
        if (em->hp > 0) {
            return;
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x100000) {
        if (em->hp <= 0 && part->parts_no == 5) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, near);
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x40000000) {
        if (em->hp <= 0 && part->parts_no == 5) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, near);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 8, 0, 0);
    } else {
        if (em10ChgParasiteCk(em)) {
            return;
        }
        if (em->hp <= 0) {
            em10BloodSet(em, near);
            if (em->type == 0xA || em->type == 0xD || em->type == 2 || em->type == 0x16) {
                EmRoutineSet(em, 3, 2, 0, 0);
                return;
            }
            if (w->Be_flg & 0x10) {
                if (part->parts_no == 5 && near) {
                    em10LostHead(em, 1, 0);
                    GameAddPoint(LVADD_CRITICALHIT);
                } else {
                    em10BloodSet(em, near);
                }
                EmRoutineSet(em, 3, 1, 0, 0);
                return;
            }
            if (part->parts_no == 5 && near) {
                em10LostHead(em, 0, 0);
                GameAddPoint(LVADD_CRITICALHIT);
                if (em->r_no_0 == 1 && (em->r_no_1 == 0x10 || em->r_no_1 == 0x39 || em->r_no_1 == 0x33) && em10LostHeadCk(em) && w->Wep_type != 4) {
                    if (pG->stage_no == 1 && pG->room_no == 0) {
                        em->r_no_0 = 2;
                        em->r_no_1 = 4;
                        em->r_no_3 = em->r_no_2 = 0;
                    }
                    if ((Rnd() & 0xF) == 5) {
                        w->Die_wait = 0x5A;
                        em->hp = 1;
                    }
                    return;
                }
                EmRoutineSet(em, 2, 4, 0, 0);
                return;
            }
            em10BloodSet(em, near);
            {
                int ret = em10RoofDmCk(em);
                if (ret) {
                    return;
                }
                em->r_no_0 = 2;
                em->r_no_1 = 4;
                em->r_no_2 = 0;
                em->r_no_3 = 0;
                return;
            }
            em->r_no_0 = 2;
            em->r_no_1 = 4;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return;
        }
        if (w->Be_flg & 8) {
            w->WakeTimer = 0;
            em10BloodSet(em, near);
            return;
        }
        if (w->Be_flg & 0x10) {
            w->WakeTimer = 0;
            em10BloodSet(em, near);
            EmRoutineSet(em, 2, 0xA, 0, 0);
            return;
        }
        if (w->pShield) {
            em10BloodSet(em, near);
            EmRoutineSet(em, 2, 0, 0, 0);
            return;
        }
        em10BloodSet(em, near);
        {
        int ret = em10RoofDmCk(em);
        if (ret) {
            return;
        }
        }
        if (part->parts_no == 0x25) {
            em->r_no_0 = 2;
            em->r_no_1 = 0xF;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return;
        }
        if (near && em->type == 2) {
            int type = em->type;
            if (part->parts_no == 5) {
                em->r_no_0 = type;
                em->r_no_1 = 1;
                em->r_no_2 = 0;
                em->r_no_3 = 0;
                return;
            } else {
                u8 r = Rnd() % 10;
                if (r > 4) {
                    em->r_no_0 = type;
                    em->r_no_1 = 0x11;
                    em->r_no_2 = 0;
                    em->r_no_3 = 0;
                    return;
                }
            }
        }
        if (em->type == 0xA || em->type == 0xD || em->type == 2) {
            return;
        }
        if (near) {
            if (em->type == 0x16) {
                u8 r = Rnd() % 10;
                if (r <= 4 && part->parts_no != 5) {
                    return;
                }
                EmRoutineSet(em, 2, 0, 0, 0);
                return;
            }
            EmRoutineSet(em, 2, 4, 0, 0);
            return;
        }
        if (em->type == 0x16) {
            return;
        }
        if (w->Be_flg & 0x40) {
            if (part->parts_no == 0x13 || part->parts_no == 0x17 || part->parts_no == 0x14 || part->parts_no == 0x18) {
                EmRoutineSet(em, 2, 3, 0, 0);
            } else {
                EmRoutineSet(em, 2, 2, 0, 0);
            }
        } else {
            EmRoutineSet(em, 2, 0, 0, 0);
        }
    }
}
#endif // PARKED

// Damage reaction to the heavy weapons (rifles 9/0xA, magnum, rocket 0xD, grenade blasts 0x12/0x13,
// 0x29, mine 0x2D): explosive kinds set `mag` (no lost head, 30-frame damage hold) and a close blast
// on a normal Ganado goes to Die_Bomb (gibbing); a bowgun Ganado's dynamite goes off (Die_Bomb);
// otherwise the Ganado is blown away (Dm_Blow) or reacts like a bullet hit on the gatling / claw types.
// FUNCTION: BIO4 0x0045A110
// STATUS: stub
#if 0 // PARKED: C1003, C2227, G_ROOM_ID, GameAddPoint, LVADD_CRITICALHIT, SndCall, YARARE_INFO, part, set
void em10DmSetWep09(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    YARARE_INFO* part = em->dmg.m_pDamageYarare;
    cModel* parts;
    int mag;
    int type;

    switch (em->dmg.m_Wep) {
    default:
        mag = 0;
        em->dmg.set(0, 5);
        break;
    case 0xD:
    case 0x12:
    case 0x13:
    case 0x29:
    case 0x2D:
        mag = 1;
        em->dmg.set(0, 0x1E);
        break;
    }
    if (em->dmg.m_Wep == 0x2D) {
        em->hp = 0;
        EmRoutineSet(em, 3, 5, 0, 2);
        return;
    }
    parts = em->getPartsPtr(part->parts_no - 1);
    if ((part->parts_no == 5 || part->parts_no == 0x25) && w->pCore) {
        SndCall(8, 0x83, &parts->world, em->id, 0, em);
    } else if (em10ArmorCk(em, part->parts_no)) {
        SndCall(8, 0xD, &parts->world, em->id, 0, em);
    } else if (em->dmg.m_Wep == 0x1C) {
        SndCall(8, 0x81, &parts->world, em->id, 0, em);
    } else {
        SndCall(8, 0xC, &parts->world, em->id, 0, em);
    }
    if (w->Wep_type == 9) {
        switch (em->dmg.m_Wep) {
        case 0xD:
        case 0x12:
        case 0x13:
        case 0x29:
            em->hp = 0;
            EmRoutineSet(em, 3, 5, 0, 0);
            return;
        }
    }
    if ((s16) w->Frame_timer != 0) {
        w->Eff_timer = 120;
        EmRoutineSet(em, 2, 0xB, 0, 0);
    } else if (w->Be_flg & 0x4000) {
        EmRoutineSet(em, 2, 0xC, 0, 0);
    } else if (w->Be_flg & 0x20000) {
        if (em->hp <= 0 && part->parts_no == 5 && mag == 0) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x10000) {
        if (em->hp <= 0 && part->parts_no == 5 && mag == 0) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 6, 0, 0);
    } else if (w->Be_flg & 0x10000000) {
        em10BloodSet(em, 0);
        if (em->hp > 0) {
            return;
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x100000) {
        if (em->hp <= 0 && part->parts_no == 5 && mag == 0) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
        }
        EmRoutineSet(em, 2, 4, 0, 0);
    } else if (w->Be_flg & 0x40000000) {
        if (em->hp <= 0 && part->parts_no == 5 && mag == 0) {
            em10LostHead(em, 1, 0);
            GameAddPoint(LVADD_CRITICALHIT);
        } else {
            em10BloodSet(em, 0);
            if (em->type == 0x16) {
                return;
            }
        }
        EmRoutineSet(em, 2, 8, 0, 0);
    } else {
        if (mag == 0 && em10ChgParasiteCk(em)) {
            return;
        }
        if (em->hp <= 0) {
            if (em->type == 0xA || em->type == 0xD || em->type == 2) {
                em10BloodSet(em, 0);
                EmRoutineSet(em, 3, 2, 0, 0);
                return;
            }
            if (part->len < 1000000.0f && w->Wep_type != 4 && em->type != 0xA && em->type != 0xD && G_ROOM_ID != 0x100 &&
                em->set != 0x39 && em->type != 2) {
                switch (em->dmg.m_Wep) {
                case 0xD:
                case 0x13:
                case 0x29:
                    EmRoutineSet(em, 3, 5, 0, 1);
                    return;
                }
            }
            if (em->hp <= 0 && part->parts_no == 5 && mag == 0) {
                em10LostHead(em, 1, 0);
                GameAddPoint(LVADD_CRITICALHIT);
            } else {
                em10BloodSet(em, 0);
            }
            EmRoutineSet(em, 2, 4, 0, 0);
            return;
        }
        if (w->Be_flg & 8) {
            w->WakeTimer = 0;
            em10BloodSet(em, 0);
            return;
        }
        if (w->Be_flg & 0x10) {
            w->WakeTimer = 0;
            em10BloodSet(em, 0);
            EmRoutineSet(em, 2, 0xA, 0, 0);
            return;
        }
        if (part->parts_no == 0x25) {
            EmRoutineSet(em, 2, 0xF, 0, 0);
            return;
        }
        type = em->type;
        if (type == 0xA || type == 0xD) {
            EmRoutineSet(em, 2, 0xE, 0, 0);
            return;
        }
        if (type == 2) {
            int no;
            em10BloodSet(em, 0);
            no = part->parts_no;
            if (no == 5) {
                no = 1;
            } else {
                no = 0x11;
            }
            em->r_no_0 = type;
            em->r_no_1 = no;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return;
        }
        if (type == 0x16) {
            u8 r = Rnd() % 10;
            if (r <= 4 && part->parts_no != 5) {
                return;
            }
            EmRoutineSet(em, 2, 0, 0, 0);
            return;
        }
        if (em->dmg.m_Wep == 0x1C) {
            if (part->parts_no == 5 && w->pShield == 0) {
                if (w->pCore && w->Ganado == 1) {
                    if (!(Rnd() & 3)) {
                        return;
                    }
                    em10BloodSet(em, 0);
                    EmRoutineSet(em, 2, 4, 0, 0);
                    return;
                }
                EmRoutineSet(em, 2, 1, 0, 0);
                return;
            }
            if (part->parts_no == 0x13 || part->parts_no == 0x17 || part->parts_no == 0x14 || part->parts_no == 0x18) {
                EmRoutineSet(em, 2, 0, 0, 0);
                return;
            }
        }
        em10BloodSet(em, 0);
        EmRoutineSet(em, 2, 4, 0, 0);
    }
}
#endif // PARKED

// Damage reaction to the flash grenade (dmWep 0x17 / 0x2A): kills a Ganado whose parasite is out
// (pCore / pParasite, effect 0x56 + em10CoreBreak) and stuns the others (Dm_Flash) unless they are on
// a gatling, behind a shield, the type 0xA/0xD armoured ones or set 0x19.
// FUNCTION: BIO4 0x0044BC20
// STATUS: stub
#if 0 // PARKED: EmReserveDropItem, EmSetDie
void em10DmSetWep23(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int one = 1;

    em->dmg.m_Timer = one;
    if (em->type == 0xA || em->type == 0xD) {
        return;
    }
    if (em->set == 0x19) {
        return;
    }
    if (w->pGatling) {
        return;
    }
    if (w->pShield) {
        return;
    }
    if (w->pCore || w->pParasite) {
        em->hp = 0;
        EmSetDie(em);
        EmReserveDropItem(em);
        em10SetPoint(em);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x56, 0, ESP_CORE_KIND_NONE, em, 0);
        em10CoreBreak(em, 0);
        if (EM10_WK(em)->Be_flg & 0x10) {
            EmRoutineSet(em, 3, one, 0, 0);
            return;
        }
        if (em10RoofDmCk(em)) {
            return;
        }
        if ((s16) w->Frame_timer != 0) {
            w->Eff_timer = 120;
            EmRoutineSet(em, 2, 0xB, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x4000) {
            EmRoutineSet(em, 2, 0xC, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x20000) {
            EmRoutineSet(em, 2, 5, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x10000) {
            EmRoutineSet(em, 2, 6, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x10000000) {
            EmRoutineSet(em, 2, 4, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x100000) {
            EmRoutineSet(em, 2, 4, 0, 0);
        } else {
            EmRoutineSet(em, 3, 2, 0, 0);
        }
    } else {
        if ((s16) w->Frame_timer != 0) {
            w->Eff_timer = 120;
            EmRoutineSet(em, 2, 0xB, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x4000) {
            EmRoutineSet(em, 2, 0xC, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x20000) {
            EmRoutineSet(em, 2, 5, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x10000) {
            EmRoutineSet(em, 2, 6, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x10000000) {
            if (em->hp > 0) {
                return;
            }
            EmRoutineSet(em, 2, 4, 0, 0);
        } else if (EM10_WK(em)->Be_flg & 0x100000) {
            EmRoutineSet(em, 2, 4, 0, 0);
        } else if (em->hp <= 0) {
            EmSetDie(em);
            EmReserveDropItem(em);
            em10SetPoint(em);
            if (EM10_WK(em)->Be_flg & 0x10) {
                EmRoutineSet(em, 3, one, 0, 0);
            } else {
                if (em10RoofDmCk(em)) {
                    return;
                }
                EmRoutineSet(em, 3, 2, 0, 0);
            }
        } else if (EM10_WK(em)->Be_flg & 8) {
            w->WakeTimer = 0;
        } else if (w->Be_flg & 0x10) {
            EmRoutineSet(em, 2, 0xA, 0, 0);
        } else {
            if (em10RoofDmCk(em)) {
                return;
            }
            EmRoutineSet(em, 2, 0xD, 0, 0);
        }
    }
}
#endif // PARKED

// Damage blood / hit effect by weapon (near: the shot came from close range).
// FUNCTION: BIO4 0x00435780
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, CTRL12_ID_BIGEFF, Camera, Ctrl12Ck, Ctrl12Set, EmDmBloodSet2, EmGetDmPos, EspEstSetSelect, EspGetEstAddr, EspSeqData, Mtx, PSMTXInverse, PSMTXMultVec, YARARE_INFO, cEsp, cam, esp, est, m, part
void em10BloodSet(cEm10* em, int near)
{
    Em10Work* w = EM10_WK(em);
    Camera* cam = &pG->Camera;
    YARARE_INFO* part;
    cModel* parts;
    f32 dist;
    Vec pos;
    Vec dir;
    Vec dir2;
    Mtx m;
    cEsp* esp;
    u32 i;

    if (em->type == 0xA || em->type == 0xD) {
        em1cBloodSet(em, near);
        return;
    }
    parts = em->getPartsPtr(0);
    dist = (cam->param.pos.x - parts->world.x) * (cam->param.pos.x - parts->world.x) +
           (cam->param.pos.y - parts->world.y) * (cam->param.pos.y - parts->world.y) +
           (cam->param.pos.z - parts->world.z) * (cam->param.pos.z - parts->world.z);
    part = em->dmg.m_pDamageYarare;
    if (part == 0) {
        return;
    }
    if (em10ArmorCk(em, part->parts_no)) {
        switch (em->dmg.m_Wep) {
        case 0:
        case 0x14:
        case 0x17:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x2A:
            return;
        case 1:
        case 2:
        case 3:
        case 4:
        case 0x11:
        case 0x19:
        case 0x1C:
        case 0x1F:
        case 0x20:
        case 0x26:
        case 0x2B:
            EmDmBloodSet2(em, 0x10, 0x64, 0, 0, 0);
            return;
        case 0x10:
        case 0x1A:
            EmDmBloodSet2(em, 0x10, 0x64, 0, 0, 0);
            return;
        case 0xB:
        case 0xC:
        case 0x1B:
        case 0x1D:
        case 0x27:
            EmDmBloodSet2(em, 0x10, 0x64, 0, 0, 0);
            return;
        case 7:
        case 8:
        case 0x21:
            if (near) {
                EmDmBloodSet2(em, 0x10, 0x65, 0, 0, 0);
            } else {
                EmDmBloodSet2(em, 0x10, 0x64, 0, 0, 0);
            }
            return;
        case 5:
        case 6:
        case 9:
        case 0xA:
        case 0xD:
        case 0xF:
        case 0x12:
        case 0x13:
        case 0x15:
        case 0x28:
        case 0x29:
        case 0x2C:
        case 0x2D:
            break;
        }
        EmDmBloodSet2(em, 0x10, 0x64, 0, 0, 0);
    } else {
        if (part->parts_no == 5 && (w->pCore || w->pParasite)) {
            EmDmBloodSet2(em, 0x10, 0x26, 0, 0, 0);
            return;
        }
        switch (em->dmg.m_Wep) {
        case 0:
        case 0x14:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x2A:
            return;
        case 1:
        case 2:
        case 3:
        case 4:
        case 0x11:
        case 0x19:
        case 0x1C:
        case 0x1F:
        case 0x20:
        case 0x26:
        case 0x2B:
            if (ChkWaterEffectEnable(&em->pos)) {
                EmDmBloodSet2(em, 0x10, 0x3B, 0, 0, 0);
            } else {
                EmDmBloodSet2(em, 0x10, 1, 0, 0, 0);
            }
            return;
        case 0x10:
        case 0x1A:
            EmDmBloodSet2(em, 0x10, 0x29, 0, 0, 0);
            return;
        case 0xB:
        case 0xC:
        case 0x1B:
        case 0x1D:
        case 0x27:
            if (ChkWaterEffectEnable(&em->pos)) {
                EmDmBloodSet2(em, 0x10, 0x3D, 0, 0, 0);
            } else {
                EmDmBloodSet2(em, 0x10, 0xC, 0, 0, 0);
            }
            if ((Rnd() & 3) == 0) {
                if (EmGetDmPos(em, &pos, &dir)) {
                    EstSet(0, -1, &pos, 0, EFF_EM10, 0xD, 0, ESP_CORE_KIND_NONE, 0, 0);
                }
            }
            return;
        case 7:
        case 8:
        case 0x21:
            if (near) {
                if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BIGEFF)) {
                    if (dist < 16000000.0f) {
                        if (ChkWaterEffectEnable(&em->pos)) {
                            EmDmBloodSet2(em, 0x10, 0x40, 0, 0, 0);
                        } else {
                            EmDmBloodSet2(em, 0x10, 0x37, 0, 0, 0);
                        }
                    } else {
                        if (ChkWaterEffectEnable(&em->pos)) {
                            EmDmBloodSet2(em, 0x10, 0x3E, 0, 0, 0);
                        } else {
                            EmDmBloodSet2(em, 0x10, 0x35, 0, 0, 0);
                        }
                    }
                } else {
                    if (dist < 16000000.0f) {
                        if (ChkWaterEffectEnable(&em->pos)) {
                            EmDmBloodSet2(em, 0x10, 0x3F, 0, 0, 0);
                        } else {
                            EmDmBloodSet2(em, 0x10, 0x36, 0, 0, 0);
                        }
                    } else {
                        if (ChkWaterEffectEnable(&em->pos)) {
                            EmDmBloodSet2(em, 0x10, 0x3C, 0, 0, 0);
                        } else {
                            EmDmBloodSet2(em, 0x10, 2, 0, 0, 0);
                        }
                    }
                    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_BIGEFF, 0x2D);
                }
            } else {
                if (ChkWaterEffectEnable(&em->pos)) {
                    EmDmBloodSet2(em, 0x10, 0x3B, 0, 0, 0);
                } else {
                    EmDmBloodSet2(em, 0x10, 1, 0, 0, 0);
                }
            }
            return;
        case 9:
        case 0xA:
        case 0x28:
            EmDmBloodSet2(em, 0x10, 0x87, 0, 0, 0);
            if (EmGetDmPos(em, &pos, &dir2)) {
                EspSeqData* est = EspGetEstAddr(EFF_EM10, 0x88, 1);
                if (est) {
                    for (i = 0; i < est->num; i++) {
                        if (EspEstSetSelect(EFF_EM10, 0x88, i, &esp, 0)) {
                            esp->m_Pos = pos;
                            esp->parent = em->getPartsPtr(part->parts_no - 1);
                            PSMTXInverse(((cModel*) esp->parent)->mat, m);
                            PSMTXMultVec(m, &esp->m_Pos, &esp->m_Pos);
                            esp->m_Parts_no = ((u8*) &part->parts_no)[1] - 1;
                        }
                    }
                }
            }
            return;
        case 5:
        case 6:
        case 0xD:
        case 0xF:
        case 0x12:
        case 0x13:
        case 0x15:
        case 0x29:
        case 0x2C:
        case 0x2D:
            break;
        }
        {
            if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BIGEFF)) {
                if (dist < 16000000.0f) {
                    if (ChkWaterEffectEnable(&em->pos)) {
                        EmDmBloodSet2(em, 0x10, 0x40, 0, 0, 0);
                    } else {
                        EmDmBloodSet2(em, 0x10, 0x37, 0, 0, 0);
                    }
                } else {
                    if (ChkWaterEffectEnable(&em->pos)) {
                        EmDmBloodSet2(em, 0x10, 0x3E, 0, 0, 0);
                    } else {
                        EmDmBloodSet2(em, 0x10, 0x35, 0, 0, 0);
                    }
                }
            } else {
                if (dist < 16000000.0f) {
                    if (ChkWaterEffectEnable(&em->pos)) {
                        EmDmBloodSet2(em, 0x10, 0x3F, 0, 0, 0);
                    } else {
                        EmDmBloodSet2(em, 0x10, 0x36, 0, 0, 0);
                    }
                } else {
                    if (ChkWaterEffectEnable(&em->pos)) {
                        EmDmBloodSet2(em, 0x10, 0x3C, 0, 0, 0);
                    } else {
                        EmDmBloodSet2(em, 0x10, 2, 0, 0, 0);
                    }
                }
                Ctrl12Set(w->pCtrlGroup, CTRL12_ID_BIGEFF, 0x2D);
            }
        }
    }
}
#endif // PARKED

// Per-frame Ganado update: damage check, route check, the work timers (attack / dash / throw waits,
// Lose_timer), the R0 routine table (Init / Move / Damage / Die / Scenario), then the model post
// processing: scale return, claw / neck / waist / slope / compress / bomb-neck moves, collision and
// scenario check (checkAir while jumping, flag 0x80000), the stuck counter HoseiCnt, shadow fade, work
// effect cleanup per set, cloth, chainsaw idle SE, cart release, bowgun / parasite / water / foot SE,
// the lit dynamite countdown (Fire_timer -> Die_Bomb), and the hit boxes of the core and shield.
// FUNCTION: BIO4 0x0045AE20
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, Camera, EM_STATUS_IK_OFF, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, Em18ClothMove, EmAtCheck, EmMgr, EstSet, Game_level, MotionSetCore, Mtx, PSMTXInverse, PSMTXMultVec, PartsWorldPosCalc, PlWepHitCheck2, STA_PL_CATCHED, STA_PL_CATCHHOLD, SatMgr, Shd_color, SndCall, SndStop, StaFlagChk, VEC_DISTXZ, cObj12, cam, f32, getPartsPtr, hit, hitInfo, isAlive, m, mat, move, pFloor_norm, pParts, partsFixAdjust, partsWorldCalc, pl_life, pos_old, setLost
void cEm10::move()
{
    Em10Work* w = EM10_WK(this);
    f32 dist;
    Mtx m;
    Vec v;

    if (r_no_0) {
        em10DmCk(this);
    }
    w->Be_flg &= 0xACC081A7;
    clearStatus(EM_STATUS_IK_OFF);
    hitInfo.flag |= 1;
    if ((w->Be_flg & 0x80) && w->pCore == 0 && w->pParasite == 0) {
        hitInfo.flag &= ~1;
    }
    em10RouteCk(this);
    {
        u32 lost = w->Be_flg & 0x800000;
        w->Be_flg &= ~0x04000004;
        if (lost) {
            w->Wander_timer++;
        } else {
            w->Wander_timer = 0;
        }
    }
    if (w->Atk_wait) {
        w->Atk_wait--;
    }
    if (pPL->r_no_0 == 1 || (StaFlagChk(pG, STA_PL_CATCHED))) {
        if ((s16) w->Atk_wait <= 4) {
            w->Atk_wait = 5;
        }
    }
    if (StaFlagChk(pG, STA_PL_CATCHHOLD)) {
        w->Atk_wait = 0;
    }
    if (w->Dash_wait) {
        w->Dash_wait--;
    }
    if (pG->Game_level > 6) {
        if (w->Dash_wait) {
            w->Dash_wait--;
        }
    }
    if (pG->Game_level > 9) {
        if (w->Dash_wait) {
            w->Dash_wait--;
        }
    }
    if (w->Anger_timer) {
        w->Anger_timer--;
    }
    if (w->Eff_timer) {
        w->Eff_timer--;
    }
    if (w->Be_flg & 1) {
        w->Lose_timer = 0;
    } else {
        w->Lose_timer++;
    }
    if (w->Jcatch_wait) {
        w->Jcatch_wait--;
    }
    if (w->Throw_timer) {
        w->Throw_timer--;
    }
    if (w->Atk_no_wait) {
        w->Atk_no_wait--;
    }
    if (w->Frame_timer) {
        w->Frame_timer--;
    }
    if (w->Water_eff_wait3) {
        w->Water_eff_wait3--;
    }
    if (w->Esc_timer) {
        w->Esc_timer--;
    }
    if (w->Csaw_sign_wait) {
        w->Csaw_sign_wait--;
    }
    if (w->No_dmg_timer) {
        w->No_dmg_timer--;
    }
    if (w->No_adj_timer) {
        w->No_adj_timer--;
        if (w->No_adj_timer == 0) {
            atari.m_flag &= ~8;
        }
    }
    if (w->Die_wait) {
        w->Die_wait--;
    }
    if ((w->Be_flg & 1) && w->CriAtk_wait) {
        w->CriAtk_wait--;
    }
    if (w->Poison_wait) {
        w->Poison_wait--;
    }
    w->Atk_trg = 0;
    Em10_R0_move_tbl[r_no_0](this);
    if (r_no_0 == 0xFF) {
        EmMgr.destroy(this);
        return;
    }
    if (Motion.Seq_old.Free & 0x80) {
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x01000000;
        setStatus(EM_STATUS_IK_OFF);
    }
    if (w->Be_flg & 0x800) {
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
    } else {
        scale.x = scale.x * 0.9f + w->Scale.x * 0.1f;
        scale.y = scale.y * 0.9f + w->Scale.y * 0.1f;
        scale.z = scale.z * 0.9f + w->Scale.z * 0.1f;
    }
    em10ClawMove(this);
    if (!(w->Be_flg & 0x400000)) {
        em10NeckMove(this);
        em10WaistMove(this);
        em10SlopeMove(this);
        partsWorldCalc();
        em10ScaleCompress(this);
        em10BombNeckMove(this);
        partsFixAdjust();
        PartsWorldPosCalc(this);
        if (!(w->Be_flg & 0x400000)) {
            u16 atFlags;
            f32 moved;
            dist = VEC_DISTXZ(&pos_old, &pos);
            if ((Motion.Seq_old.Free & 0x40) || (w->Be_flg & 0x10091000)) {
                atari.m_flag |= 0x10;
            } else {
                atari.m_flag &= ~0x10;
            }
            atFlags = atari.m_flag;
            if ((Motion.Seq_old.Free & 0x40) || (w->Be_flg & 0x11000)) {
                atari.m_flag &= ~0x100;
            }
            EmAtCheck(this);
            atari.move();
            if (w->Be_flg & 0x80000) {
                SatMgr.checkAir(this, 0x1C2810);
            } else {
                SatMgr.check(this, 0);
            }
            atari.m_flag = atFlags;
            moved = VEC_DISTXZ(&pos, &pos_old);
            if (moved < dist * 0.5f) {
                w->HoseiCnt++;
            } else {
                if (w->HoseiCnt > 60) {
                    w->HoseiCnt = 60;
                }
                if (w->HoseiCnt) {
                    w->HoseiCnt--;
                }
            }
        }
    }
    {
        Camera* cam = &pG->Camera;
        int hide = 0;
        Vec* nrm = pFloor_norm;
        if (nrm == 0 || nrm->y < 0.8f) {
            hide = 1;
        }
        if (cam->param.pos.y < pos.y) {
            hide = 1;
        }
        if ((w->Be_flg & 0x101B0000) || hide) {
            if (Shd_color <= 0xF6) {
                Shd_color += 8;
            } else {
                Shd_color = 0xFF;
            }
        } else {
            if (Shd_color > 8) {
                Shd_color -= 8;
            } else {
                Shd_color = hide;
            }
        }
    }
    switch (set) {
    default:
        break;
    case 5:
    case 6:
    case 0x1A:
        if (r_no_0 != 1 || r_no_1 != 7) {
            EffectEspDelete(0, w->EffKindIdWork, this, 0);
            EffectEspgenDelete(0, w->EffKindIdWork, this);
            EffectEfmDelete(0, w->EffKindIdWork, this);
        }
        break;
    case 7:
    case 8:
    case 9:
        if (r_no_0 != 1 || r_no_1 != 8) {
            EffectEspDelete(0, w->EffKindIdWork, this, 0);
            EffectEspgenDelete(0, w->EffKindIdWork, this);
            EffectEfmDelete(0, w->EffKindIdWork, this);
        }
        break;
    case 0xA:
    case 0xB:
        break;
    }
    em10ChainSawMove(this);
    if (type == 6) {
        Em18ClothMove(this, (PlCloth*) &w->Cloth);
    }
    if (type == 0x16) {
        Em1fClothMove(this, (PlCloth*) &w->Cloth);
    }
    if (w->pWeapon && w->Wep_type == 4 && (w->Be_flg & 0x80000000) && hp > 0 && (s16) pG->pl_life > 0) {
        if (w->Csaw_se_wait) {
            w->Csaw_se_wait--;
            if ((s16) w->Csaw_se_wait == 0) {
                w->Csaw_se_wait = 60;
                w->Csaw_se_id = SndCall(6, 0x4D, &pos, 0, 0, this);
            }
        }
    } else if (w->Csaw_se_id) {
        SndStop(w->Csaw_se_id, 0);
        w->Csaw_se_id = 0;
    }
    if (w->pCart && (r_no_0 != 1 || r_no_1 != 9)) {
        if (w->mot[51]) {
            MotionSetCore(w->pCart, MOTION(w->pCart), w->mot[51], 0, 0, 0, 0);
        }
        w->pCart->atari.init(0.0f, 150.0f, -300.0f, 300.0f, 300.0f, 300.0f, 150.0f, 1, 0x2000, 10);
        w->pCart->atari.m_flag &= ~0x100;
        w->pCart->atari.m_flag |= 0x200;
        w->pCart->atari.m_flag |= 0x10;
        EstSet(w->pCart, -1, 0, 0, EFF_EM10, 0x16, 0, ESP_CORE_KIND_NONE, w->pCart, 0);
        EstSet(w->pCart, -1, 0, 0, EFF_EM10, 0x16, 0, ESP_CORE_KIND_NONE, w->pCart, 0);
        EstSet(w->pCart, -1, 0, 0, EFF_EM10, 0x16, 0, ESP_CORE_KIND_NONE, w->pCart, 0);
        w->pCart = 0;
    }
    em10BowgunMove(this);
    if (w->Parasite_wait) {
        w->Parasite_wait--;
        if (w->Parasite_wait == 0) {
            em10SetParasite(this);
        }
    }
    em10SetWaterEff(this);
    if (w->pParasite && hp > 0) {
        if (w->Eff_wait) {
            w->Eff_wait--;
        } else {
            w->Eff_wait = 0x1D;
            EstSet(this, -1, 0, 0, EFF_EM10, 0x33, 0, ESP_CORE_KIND_NONE, this, 0);
        }
    }
    em10FootSe(this);
    if (w->Wep_type == 9 && w->Fire_timer && w->pWeapon && hp > 0) {
        if (w->Fire_timer <= 999) {
            w->Fire_timer--;
        }
        w->Bomb_se_wait++;
        if (w->Bomb_se_wait % 6 == 0) {
            SndCall(8, 0x95, &pos, id, 0, this);
        }
        if (w->Fire_timer == 0) {
            cModel* parts = w->pWeapon->getPartsPtr(0);
            w->pWeapon->setLost();
            w->pWeapon = 0;
            w->Wep_type = 0;
            if (hp > 0) {
                hp = 0;
                EmRoutineSet(this, 3, 5, 0, 0);
            } else {
                if (w->Be_flg & 0x01400000) {
                    EstSet(0, -1, &parts->world, 0, EFF_EM10, 0x2A, 0, ESP_CORE_KIND_NONE, 0, 0);
                } else {
                    EstSet(this, -1, 0, 0, EFF_EM10, 0x30, 0, ESP_CORE_KIND_NONE, this, 0);
                }
                SndCall(8, 0x96, &pos, id, 0, this);
                SndCall(8, 8, &pos, id, 0, this);
                be_flag &= ~2;
            }
            dmg.m_Timer = 0;
            PlWepHitCheck2(0, &pos, &pos, 0x13, 2, 6000.0f);
        }
    }
    if (type == 0xA || type == 0xD) {
        w->hit[9].flag |= 1;
    } else {
        w->hit[9].flag &= ~1;
        if (w->pCore && w->pCore->pParts && w->pCore->isAlive()) {
            cModel* parts;
            PSMTXInverse(getPartsPtr(4)->mat, m);
            if (w->Ganado == 1) {
                parts = w->pCore->getPartsPtr(9);
            } else {
                parts = w->pCore->getPartsPtr(0x15);
            }
            PSMTXMultVec(m, &parts->world, &v);
            w->hit[9].offset = v;
            w->hit[9].flag |= 1;
        }
        if (w->pParasite && w->pParasite->pParts && w->pParasite->isAlive()) {
            cModel* parts;
            PSMTXInverse(getPartsPtr(4)->mat, m);
            parts = w->pParasite->getPartsPtr(2);
            PSMTXMultVec(m, &parts->world, &v);
            w->hit[9].offset = v;
            w->hit[9].flag |= 1;
        }
    }
    if (w->pShield) {
        if (flag & 0x1000000) {
            w->hit[3].flag &= ~1;
            w->hit[7].flag &= ~1;
        } else {
            w->hit[4].flag &= ~1;
            w->hit[8].flag &= ~1;
        }
    } else {
        w->hit[3].flag |= 1;
        w->hit[7].flag |= 1;
        w->hit[4].flag |= 1;
        w->hit[8].flag |= 1;
    }
    if (w->pShield && w->pShield->hp <= 0) {
        w->pShield = 0;
    }
    em10GatlingRollMove(this);
    if ((flag & 0x40) && !(w->Be_flg & 0x100) && G_ROOM_ID == 0x206) {
        setFindPL();
    }
}
#endif // PARKED

// R0 == 0: creation. Runs the module's Em10SetFunc (motion table), loads the effect data, builds the
// model (em10ModelInit), picks the body scale from emset_no, sets collision, the hit boxes (head 5,
// arms, legs, the extra hit[] boxes), the sound / route / guard defaults and the start routine
// (em10InitRtnSet), then runs the first R0_Move frame.
// FUNCTION: BIO4 0x00461580
// STATUS: stub
#if 0 // PARKED: C2228, DBG_NO_PARASITE, DbgFlagChk, EM_STATUS_ASHLEY_NO_HELP, EM_STATUS_LOCKOFF, Em10SetFunc, Em18ClothSet, EspDataLoad, GetCtrlCtrl11, GetCtrlCtrl12, LightInfo, OSReport, YAT_FLAG_ON, YAT_FLAG_X_AXIS, YAT_FLAG_Z_AXIS, YarareAdd, YarareInit, em10_xflip_tbl, fRand1_1, flip, hit, init, litArea, lockOfs, lockParts, room_no, stage_no
void em10_R0_Init(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    f32 sc;

    if (Em10SetFunc == 0) {
        em->r_no_0 = 0xFF;
        return;
    }
    Em10SetFunc(em);
    EspDataLoad((u32) PL_ARC_PTR(em->subArc, 4), EFF_EM10, 0);
    if (em10ModelInit(em) == 0) {
        em->r_no_0 = 0xFF;
        return;
    }
    switch (em->emset_no & 0xF) {
    default: {
        sc = fRand1_1() * 0.01f + 1.03f;
        const f32 k = 1.01f; // pool order: 1.01 before the 1.1 of case 7 (docs/matching.md const-local idiom)
        break;
    }
    case 7:
        sc = 1.1f;
        break;
    case 0:
    case 4:
    case 9:
    case 0xB:
        sc = fRand1_1() * 0.01f + 1.01f;
        break;
    }
    if (em->type == 6) {
        sc = 1.05f;
    }
    if (em->type == 0xA || em->type == 0xD) {
        sc = 1.25f;
    }
    if (em->type == 2) {
        sc = 1.25f;
    }
    if (em->type == 0x16) {
        sc = 1.3f;
    }
    if (em->type == 0x18) {
        sc = 1.2f;
    }
    switch (em->type) {
    case 0xB:
    case 0xC:
        sc = 1.0f;
        break;
    }
    em->scale.z = sc;
    em->scale.y = sc;
    em->scale.x = sc;
    w->Scale = em->scale;
    em->Motion.flip = em10_xflip_tbl;
    switch (em->type) {
    case 6:
        Em18ClothSet(em, (PlCloth*) &w->Cloth, 0);
        break;
    case 0x16:
        Em1fClothSet(em, (PlCloth*) &w->Cloth);
        break;
    case 3:
    case 5:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0x17:
        break;
    }
    w->pCtrlSe = GetCtrlCtrl11();
    w->pCtrlGroup = GetCtrlCtrl12();
    w->St_pos = em->pos;
    w->St_dir = em->ang.y;
    w->Keep_pos = w->St_pos;
    w->St_set = em->set;
    {
        static const Vec ofs = { 0.0f, 0.0f, 0.0f };
        static const Vec size = { 1000.0f, 1000.0f, 0.0f };
        em->LightInfo.init2(0, 1, &ofs, &size, 2);
    }
    em->lockParts = 2;
    em->lockOfs.x = 0.0f;
    em->lockOfs.y = 0.0f;
    em->lockOfs.z = 0.0f;
    em->atari.init(0.0f, 0.0f, 0.0f, 400.0f, 250.0f, 250.0f, 800.0f, 1, 0x2000, 10);
    if (em->type == 6) {
        em->atari.m_flag |= 8;
        em->setStatus(EM_STATUS_ASHLEY_NO_HELP);
    }
    em->litArea.on(1);
    if (em->type == 6) {
        em->setStatus(EM_STATUS_LOCKOFF);
        if (em->type == 6) {
            em->hp = 1;
        }
    }
    if (em->type == 0xA || em->type == 0xD) {
        YarareInit(em, 0.0f, 300.0f, 50.0f, 150.0f, 110.0f, 3, YAT_FLAG_ON | YAT_FLAG_Z_AXIS);
    } else {
        YarareInit(em, 0.0f, 0.0f, 0.0f, 160.0f, 110.0f, 5, YAT_FLAG_ON);
    }
    YarareAdd(em, &w->hit[0], 0.0f, -30.0f, 0.0f, 210.0f, 290.0f, 2, YAT_FLAG_ON);
    switch (em->type) {
    case 7:
    case 8:
    case 9:
    case 0xB:
    case 0xC:
        YarareAdd(em, &w->hit[1], -20.0f, -400.0f, 0.0f, 180.0f, 400.0f, 0x14, YAT_FLAG_ON);
        YarareAdd(em, &w->hit[2], 20.0f, -400.0f, 0.0f, 180.0f, 400.0f, 0x18, YAT_FLAG_ON);
        break;
    default:
        YarareAdd(em, &w->hit[1], -20.0f, -400.0f, 0.0f, 150.0f, 400.0f, 0x14, YAT_FLAG_ON);
        YarareAdd(em, &w->hit[2], 20.0f, -400.0f, 0.0f, 150.0f, 400.0f, 0x18, YAT_FLAG_ON);
        break;
    }
    if (em->type == 0xA || em->type == 0xD) {
        YarareAdd(em, &w->hit[3], -300.0f, 0.0f, 0.0f, 100.0f, 250.0f, 9, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
        YarareAdd(em, &w->hit[4], 50.0f, 0.0f, 0.0f, 100.0f, 250.0f, 0xF, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
    } else {
        YarareAdd(em, &w->hit[3], -350.0f, 0.0f, 0.0f, 100.0f, 350.0f, 9, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
        YarareAdd(em, &w->hit[4], 0.0f, 0.0f, 0.0f, 100.0f, 350.0f, 0xF, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
    }
    switch (em->type) {
    case 7:
    case 8:
    case 9:
    case 0xB:
    case 0xC:
        YarareAdd(em, &w->hit[5], -20.0f, -300.0f, 0.0f, 200.0f, 300.0f, 0x13, YAT_FLAG_ON);
        YarareAdd(em, &w->hit[6], 20.0f, -300.0f, 0.0f, 200.0f, 300.0f, 0x17, YAT_FLAG_ON);
        break;
    default:
        YarareAdd(em, &w->hit[5], -20.0f, -300.0f, 0.0f, 170.0f, 300.0f, 0x13, YAT_FLAG_ON);
        YarareAdd(em, &w->hit[6], 20.0f, -300.0f, 0.0f, 170.0f, 300.0f, 0x17, YAT_FLAG_ON);
        break;
    }
    YarareAdd(em, &w->hit[7], -300.0f, 0.0f, 0.0f, 120.0f, 300.0f, 8, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
    YarareAdd(em, &w->hit[8], 0.0f, 0.0f, 0.0f, 120.0f, 300.0f, 0xE, YAT_FLAG_ON | YAT_FLAG_X_AXIS);
    if (em->type == 0xA || em->type == 0xD) {
        YarareAdd(em, &w->hit[9], 0.0f, 50.0f, 0.0f, 200.0f, 0.0f, 0x25, YAT_FLAG_ON);
    } else {
        YarareAdd(em, &w->hit[9], 0.0f, 0.0f, 0.0f, 300.0f, 0.0f, 5, 0);
    }
    w->EffKindIdCsaw = 0x2C;
    w->EffKindIdEye = 0x2D;
    w->EffKindIdWork = 0x2E;
    w->EffKindIdArrow = 0x2F;
    w->EffKindIdCore = 0x30;
    if ((pG->stage_no == 1 && pG->room_no == 0) && em->emset_no == 0) {
        em->be_flag |= 0x10000;
    }
    switch (em->set) {
    case 2:
    case 3:
    case 4:
    case 0x18:
    case 0x19:
    case 0x1C:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2C:
    case 0x2D:
    case 0x31:
    case 0x34:
        em->be_flag |= 0x10000;
        break;
    case 0x35:
        break;
    }
    em10InitRtnSet(em);
    if (em->type == 6) {
        em->be_flag |= 0x10000;
    }
    if (DbgFlagChk(pG, DBG_NO_PARASITE)) {
        em->flag &= ~0x100000;
    }
    MotionMove(em, 0);
    em10_R0_Move(em);
    OSReport("em10 free size = 0x%x\n", sizeof(Em10Work));
}
#endif // PARKED

// R1 == 0x00 Wait: idle motion, turns to the player with cEm::flag bit3, drops the weapon once the
// parasite is out; leaves through em10GotoCk (goto modes), em10FindCk (sight), or to Stay (0x1B) when
// the player / Ashley is dead; the robed type 6 only flags itself found.
// FUNCTION: BIO4 0x00478340
// STATUS: stub
#if 0 // PARKED: C2227, DBG_EM_NO_ATK, DbgFlagChk, LIMIT_ANGLE, Muku, PI, ashley_life, pl_life
void em10_R1_Wait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int ret;

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0xA);
        if (w->pCore || w->pParasite) {
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
        }
        em->r_no_2++;
    case 1:
        if (em->flag & 8) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, PI);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        MotionMove(em, 0);
        int one = 1;
        if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
            break;
        }
        ret = em10GotoCk(em);
        if (ret) {
            break;
        }
        if (em->type == 6) {
            em->setFindPL();
            w->Be_flg |= 0x40000;
            break;
        }
        if ((s16) pG->pl_life <= 0 || (pSUB && (s16) pG->ashley_life <= 0)) {
            do { // loop notes keep `one` at its declaration (update_equiv_regs moves single-use constants only outside loops)
                EmRoutineSet(em, one, 0x1B, 0, 0);
            } while (0);
            break;
        }
        if (em10FindCk(em, 0)) {
            break;
        }
        if (em->flag & 1) {
            em->setFindPL();
        }
        if (w->Be_flg & 0x100) {
            em10FindNotify(em);
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10ActEvtSetTrade(em);
}
#endif // PARKED

// R1 == 0x01 Keeper: guard post idle. Far from the player (> 50000 units) the Ganado fades out and
// stops colliding (flag 0x400000); when the player is behind (Pl_rot > 90 deg) it plays the turn
// motion (step 2/3) and attacks from there; leaves the post for em10WalkRtnSet when the player is
// inside Guard_r or the room forces it.
// FUNCTION: BIO4 0x004785A0
// STATUS: stub
#if 0 // PARKED: C1003, LIMIT_ANGLE, Muku, PI
void em10_R1_Keeper(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    f32 ang;

    if ((EM10_WK(em)->Be_flg & 0x100) && em->r_no_2 == 0 && w->Pl_rot > 1.5707964f) {
        em->r_no_2 = 2;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0xA);
        em->r_no_2++;
    case 1:
        if (w->Be_flg & 0x100) {
            w->Be_flg |= 0x40000;
        }
        if (w->Be_flg & 0x100) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.024543693f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        } else if (em->flag & 8) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, PI);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->l_pl < 2500000000.0f || (em->flag & 0x40) || G_ROOM_ID == 0x320) {
            w->Be_flg &= ~0x400000;
            if (!(em->be_flag & 2)) {
                em->be_flag |= 2;
                em->dmg.m_Timer = 0;
            }
            em->atari.m_flag |= 0x300;
            em->invisible_factor = 1.0f;
            MotionMove(em, 0);
            if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
                break;
            }
            if (em10GotoCk(em)) {
                return;
            }
            if (w->Be_flg & 0x100) {
                if (w->L_pl_guard < em->Guard_r || (em->flag & 0x40) || (w->Be_flg & 0x08000000)) {
                    em10WalkRtnSet(em);
                    break;
                }
            } else if (em10FindCk(em, 0)) {
                break;
            }
            if (em->flag & 1) {
                em->setFindPL();
            }
        } else {
            if (em->invisible_factor > 0.0f) {
                em->invisible_factor -= 0.1f;
            } else {
                w->Be_flg |= 0x400000;
                em->invisible_factor = 0.0f;
                em->be_flag &= ~2;
                em->atari.m_flag &= ~0x300;
                em->dmg.m_Timer = 0x80;
            }
        }
        if (em->type == 6) {
            em->setFindPL();
            w->Be_flg |= 0x40000;
        }
        break;
    case 2: {
        void* m0 = PL_ARC_PTR(em->subArc, 0x18);
        void* m1 = PL_ARC_PTR(em->subArc, 0x19);
        int hokan = 1;
        if (w->Go_dir < 0.0f) {
            hokan = 0x41;
        }
        if (w->pShield) {
            m0 = PL_ARC_PTR(em->subArc, 0x16C);
            m1 = PL_ARC_PTR(em->subArc, 0x16D);
            hokan = 1;
            if (em->flag & 0x01000000) {
                hokan = 0x41;
            }
        }
        if (em->type == 0xA || em->type == 0xD) {
            m0 = PL_ARC_PTR(em->subArc, 0x116);
            m1 = PL_ARC_PTR(em->subArc, 0x117);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, hokan, 0);
        w->TmpF = em->ang.y + PI;
        w->Timer = 60;
        em->r_no_2++;
    }
    case 3:
        if (em->Motion.Seq_old.Free & 8) {
            ang = Muku(&em->pos, &pPL->pos, w->TmpF, 0.09817477f);
            w->TmpF += ang;
            w->TmpF = LIMIT_ANGLE(w->TmpF);
            em->ang.y += ang;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        } else {
            em10AtkRtnCk(em, 0);
        }
        break;
    }
    em10HandSet(em, 0);
    em10ActEvtSetTrade(em);
}
#endif // PARKED

// R1 == 0x02 Hide: invisible until cEm::flag bit0 (the room releases it), then appears in place and
// starts walking (150-frame dash delay in stage 1).
// FUNCTION: BIO4 0x00462140
// STATUS: stub
#if 0 // PARKED: room_no, stage_no
void em10_R1_Hide(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWalkMotion(em, 0);
        MotionMove(em, 0);
        em10HideOn(em, w);
        em->r_no_2++;
    case 1:
        if (!(em->flag & 1)) {
            break;
        }
        w->Be_flg |= 4;
        em10RouteCk(em);
        em->r_no_2++;
    case 2:
        em10SetWalkMotion(em, 0);
        MotionMove(em, 0);
        em10HideOff(em, w);
        if (pG->stage_no == 1 && pG->room_no == 0) {
            w->Dash_wait = 150;
        }
        em10WalkRtnSet(em);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x03 HideFall: hidden until released, then appears and drops to the floor below (JumpDown 0x43
// with Keep_pos at the floor height).
// FUNCTION: BIO4 0x0043BD90
// STATUS: stub
#if 0 // PARKED: C2228, SatMgr
void em10_R1_HideFall(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        MotionMove(em, 0);
        em10HideOn(em, w);
        em->r_no_2++;
    case 1:
        if (!(em->flag & 1)) {
            break;
        }
        em->r_no_2++;
    case 2:
        em10HideOff(em, w);
        {
            f32 y = SatMgr.getFloor(&em->pos, 0, 600.0f, 100000.0f, 0);
            w->Keep_pos = em->pos;
            w->Keep_pos.y = y;
        }
        w->Target_pos = em->pos;
        EmRoutineSet(em, 1, 0x43, 0, 0);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x04 HideJump: hidden until released, then dashes towards the player until the floor 200
// units ahead drops away by 350, and jumps down there (JumpDown 0x43).
// FUNCTION: BIO4 0x0043BFE0
// STATUS: stub
#if 0 // PARKED: C2228, LIMIT_ANGLE, Muku, PI, PSMTXMultVec, RouteCkToPos, SatMgr, l_pl, mat
void em10_R1_HideJump(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    f32 y;

    switch (em->r_no_2) {
    case 0:
        em10SetWalkMotion(em, 0);
        MotionMove(em, 0);
        em10HideOn(em, w);
        em->r_no_2++;
    case 1:
        if (!(em->flag & 1)) {
            break;
        }
        em->r_no_2++;
    case 2:
        em10HideOff(em, w);
        em->r_no_3 = 0;
        em10SetDashMotion(em);
        w->Route_type = 0;
        RouteCkToPos(em, &pPL->pos, &w->Pl_pos, 0, 0);
        w->Pl_dir = Muku(&em->pos, &w->Pl_pos, em->ang.y, PI);
        w->Pl_rot = fabsf(w->Pl_dir);
        w->Go_pos = w->Pl_pos;
        w->Go_dir = w->Pl_dir;
        w->Go_rot = w->Pl_rot;
        w->L_go = em->l_pl;
        em->r_no_2++;
    case 3:
        em->dmg.m_Timer = 0xA;
        w->Be_flg |= 0x1000;
        em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.15707964f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 200.0f;
        PSMTXMultVec(em->mat, &v, &v);
        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        if (y < em->pos.y - 350.0f) {
            w->Keep_pos = em->pos;
            w->Keep_pos.y = y;
            em->dmg.m_Timer = 0xA;
            w->Target_pos = em->pos;
            EmRoutineSet(em, 1, 0x43, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x4A: room 10C scripted head burst. Waits for the release flag, plays the event motion and
// at frame 201 the parasite bursts out of the head (em10LostHead mode 2), then walks (motion 7).
// FUNCTION: BIO4 0x0045C060
// STATUS: stub
void em10_R1_R10CParasite(cEm10* em)
{
    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->r_no_2++;
        }
        break;
    case 2:
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        } else if (em->Motion.Seq_frame > 200.7f && em->Motion.Seq_frame < 201.3f) {
            em->flag |= 0x100000;
            em10LostHead(em, 2, 0);
        }
        break;
    case 3:
        em10SetWalkMotion(em, 7);
        em->r_no_2++;
    case 4:
        MotionMove(em, 0);
        break;
    }
    em10HandSet(em, 0);
}

// R1 == 0x4B: room 10C parasite cancel. Puts the parasite out at once (em10SetParasite, head model
// swap, hides the hood / accessories), marks the Ganado found and goes to the walk routine.
// FUNCTION: BIO4 0x00462350
// STATUS: stub
#if 0 // PARKED: C2227, Ctrl12CntAdd, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete
void em10_R1_R10CPCancel(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        w->Be_flg |= 0x80;
        w->Parasite_on = 1;
        Ctrl12CntAdd(w->pCtrlGroup, 4, -1);
        Ctrl12CntAdd(w->pCtrlGroup, 4, 1);
        em10SetParasite(em);
        em10HeadSet(em, 1);
        EffectEspDelete(0, w->EffKindIdEye, em, 0);
        EffectEspgenDelete(0, w->EffKindIdEye, em);
        EffectEfmDelete(0, w->EffKindIdEye, em);
        if (w->pHood) {
            w->pHood->be_flag &= ~8;
        }
        if (w->pWhood) {
            w->pWhood->be_flag &= ~8;
        }
        if (w->pAccesory[0]) {
            w->pAccesory[0]->be_flag &= ~8;
        }
        if (w->pAccesory[1]) {
            w->pAccesory[1]->be_flag &= ~8;
        }
        if (w->pAccesory[3]) {
            w->pAccesory[3]->be_flag &= ~8;
        }
        if (w->pAccesory[4]) {
            w->pAccesory[4]->be_flag &= ~8;
        }
        if (w->pAccesory[5]) {
            w->pAccesory[5]->be_flag &= ~8;
        }
        em->flag |= 0x100000;
        em->setFindPL();
        em10WalkRtnSet(em);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x51: room 204 praying zealot. Idle motion until the player is seen (em10FindCk mode 1) or
// alerted / given a goto, then walks after a 10-frame delay.
// FUNCTION: BIO4 0x00478C60
// STATUS: stub
#if 0 // PARKED: C2105, int
void em10_R1_R204Prayer(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 10;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em10FindCk(em, 1)) {
            break;
        }
        if ((w->Be_flg & 0x100) || w->Goto_mode) {
            if (w->Timer) {
                w->Timer--;
            } else if (!em10GotoCk(em)) {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x52: room 222 dragon statue rider A. Sits on the dragon (pDragon, cCtrlDragon), moves to
// the firing seat, aims the statue at the player (setAngleX/Y, addAngle) and fires in 150-frame
// bursts sweeping the flame (em10DragonFireCk); a kill puts it into Dm_Roof (fall off, Target_dir).
// FUNCTION: BIO4 0x0045C180
// STATUS: stub
#if 0 // PARKED: C1003, C2105, C2227, EM10_DRAGON, EM_STATUS_IK_OFF, Mtx, Muku, PI, PSMTXInverse, PSMTXMultVec, PosToPos, SINF, Timer2, Timer3, int, m, minv, throughOn
void em10_R1_R222DragonA(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Mtx minv;
    Vec pl;
    Vec v;
    f32 x;
    f32 y;
    f32 ay;
    f32 ang;

    em->atari.throughOn();
    em->setStatus(EM_STATUS_IK_OFF);
    switch (em->r_no_2) {
    case 0:
        EM10_DRAGON(w)->getMtx(m, 1);
        v.x = 0.0f;
        v.y = -1000.0f;
        v.z = -1000.0f;
        PSMTXMultVec(m, &v, &em->pos);
        em->dmg.m_Timer = 0x1E;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xAE), PL_ARC_PTR(em->subArc, 0xAF), 3, 5, 0);
        w->TmpU32 = 0;
        w->Timer = 90;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->pDragon == 0) {
            break;
        }
        EM10_DRAGON(w)->getMtx(m, 1);
        v.x = 0.0f;
        v.y = -1000.0f;
        v.z = 2000.0f;
        PSMTXMultVec(m, &v, &v);
        if (w->TmpU32) {
            if (w->Timer) {
                w->Timer--;
                PosToPos(&em->pos, &v, &em->pos, 0.1f);
            } else {
                em->r_no_2++;
            }
        } else if ((em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.z - v.z) * (em->pos.z - v.z) < 10000.0f) {
            em10SetWaitMotion(em, 0xA);
            w->TmpU32 = 1;
            if (w->pDragon) {
                EM10_DRAGON(w)->setFire();
            }
        }
        break;
    case 2:
        em10SetWaitMotion(em, 0xA);
        w->Timer = 90;
        em->r_no_2++;
    case 3:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            EM10_DRAGON(w)->getMtx(m, 0);
            PSMTXInverse(m, minv);
            PSMTXMultVec(minv, &pPL->pos, &pl);
            x = pl.x;
            if (x > 50.0f) {
                x = 50.0f;
            }
            if (x < -50.0f) {
                x = -50.0f;
            }
            EM10_DRAGON(w)->setAngleX(x);
            pl.y += 2000.0f;
            y = pl.y;
            ay = fabsf(y);
            if (y > 50.0f) {
                y = 50.0f;
            }
            if (y < -50.0f) {
                y = -50.0f;
            }
            EM10_DRAGON(w)->setAngleY(y);
            EM10_DRAGON(w)->setTarget(&pl);
            ang = EM10_DRAGON(w)->getAngle();
            ang = Muku(&pl, &pPL->pos, ang, 0.0015339808f);
            EM10_DRAGON(w)->addAngle(ang);
            if (w->Timer == 0) {
                if (ay < 150.0f && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) < 0.5235988f &&
                    (em->pos.x - pPL->pos.x) * (em->pos.x - pPL->pos.x) + (em->pos.y - pPL->pos.y) * (em->pos.y - pPL->pos.y) +
                            (em->pos.z - pPL->pos.z) * (em->pos.z - pPL->pos.z) <
                        400000000.0f) {
                    em->r_no_2++;
                    break;
                }
            } else {
                w->Timer--;
            }
        }
        MotionMove(em, 0);
        break;
    case 4:
        em10SetWaitMotion(em, 0);
        w->Timer = 150;
        w->TmpU32 = 0;
        EM10_DRAGON(w)->setFire();
        w->TmpF = 0.0f;
        w->TmpF2 = EM10_DRAGON(w)->getFireAngle();
        w->Timer2 = 30;
        w->Timer3 = 15;
        em->r_no_2++;
    case 5:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            if (w->Timer > 30) {
                if (w->Timer2) {
                    w->Timer2--;
                } else {
                    ang = SINF(w->TmpF) * 0.19634955f + w->TmpF2;
                    EM10_DRAGON(w)->setFireAngle(ang);
                    w->TmpF += 0.06981317f;
                    if (w->Timer3) {
                        w->Timer3--;
                    } else {
                        em10DragonFireCk(em);
                    }
                }
            }
        }
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_2 = 2;
        }
        break;
    }
    if (em->hp <= 0) {
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        EmRoutineSet(em, 2, 7, 0, 1);
    } else {
        em10HandSet(em, 0);
    }
}
#endif // PARKED

// R1 == 0x53: room 222 dragon rider B, same as DragonA with the aim held high while the player is
// still east of x = -32000 (the far side of the hall).
// FUNCTION: BIO4 0x0045CB60
// STATUS: stub
#if 0 // PARKED: C2105, C2227, EM10_DRAGON, EM_STATUS_IK_OFF, LIMIT_ANGLE, Mtx, Muku, PI, PSMTXInverse, PSMTXMultVec, PosToPos, SINF, Timer2, Timer3, int, m, minv, throughOn
void em10_R1_R222DragonB(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Mtx minv;
    Vec pl;
    Vec v;
    f32 x;
    f32 y;
    f32 ay;
    f32 ang;

    em->atari.throughOn();
    em->setStatus(EM_STATUS_IK_OFF);
    switch (em->r_no_2) {
    case 0:
        EM10_DRAGON(w)->getMtx(m, 1);
        v.x = 0.0f;
        v.y = -1000.0f;
        v.z = -1000.0f;
        PSMTXMultVec(m, &v, &em->pos);
        em->dmg.m_Timer = 0x1E;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xAE), PL_ARC_PTR(em->subArc, 0xAF), 3, 5, 0);
        w->TmpU32 = 0;
        w->Timer = 90;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->pDragon == 0) {
            break;
        }
        EM10_DRAGON(w)->getMtx(m, 1);
        v.x = 0.0f;
        v.y = -1000.0f;
        v.z = 2000.0f;
        PSMTXMultVec(m, &v, &v);
        if (w->TmpU32) {
            if (w->Timer) {
                w->Timer--;
                PosToPos(&em->pos, &v, &em->pos, 0.1f);
            } else {
                em->r_no_2++;
            }
        } else if ((em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.z - v.z) * (em->pos.z - v.z) < 10000.0f) {
            em10SetWaitMotion(em, 0xA);
            w->TmpU32 = 1;
            if (w->pDragon) {
                EM10_DRAGON(w)->setFire();
            }
        }
        break;
    case 2:
        em10SetWaitMotion(em, 0xA);
        w->Timer = 90;
        em->r_no_2++;
    case 3:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            EM10_DRAGON(w)->getMtx(m, 0);
            PSMTXInverse(m, minv);
            PSMTXMultVec(minv, &pPL->pos, &pl);
            x = pl.x;
            if (x > 50.0f) {
                x = 50.0f;
            }
            if (x < -50.0f) {
                x = -50.0f;
            }
            EM10_DRAGON(w)->setAngleX(x);
            pl.y += 2000.0f;
            y = pl.y;
            ay = fabsf(y);
            if (y > 50.0f) {
                y = 50.0f;
            }
            if (y < -50.0f) {
                y = -50.0f;
            }
            if (pPL->pos.x > -32000.0f) {
                y = 50.0f;
                if (w->Timer <= 29) {
                    w->Timer = 30;
                }
            }
            EM10_DRAGON(w)->setAngleY(y);
            EM10_DRAGON(w)->setTarget(&pl);
            ang = EM10_DRAGON(w)->getAngle();
            ang = Muku(&pl, &pPL->pos, ang, 0.0015339808f);
            EM10_DRAGON(w)->addAngle(ang);
            if (w->Timer == 0) {
                if (ay < 150.0f && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) < 0.5235988f &&
                    (em->pos.x - pPL->pos.x) * (em->pos.x - pPL->pos.x) + (em->pos.y - pPL->pos.y) * (em->pos.y - pPL->pos.y) +
                            (em->pos.z - pPL->pos.z) * (em->pos.z - pPL->pos.z) <
                        400000000.0f) {
                    em->r_no_2++;
                    break;
                }
            } else {
                w->Timer--;
            }
        }
        MotionMove(em, 0);
        break;
    case 4:
        em10SetWaitMotion(em, 0);
        w->Timer = 150;
        w->TmpU32 = 0;
        EM10_DRAGON(w)->setFire();
        w->TmpF = 0.0f;
        w->TmpF2 = EM10_DRAGON(w)->getFireAngle();
        w->Timer2 = 30;
        w->Timer3 = 15;
        em->r_no_2++;
    case 5:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            if (w->Timer > 30) {
                if (w->Timer2) {
                    w->Timer2--;
                } else {
                    ang = SINF(w->TmpF) * 0.19634955f + w->TmpF2;
                    EM10_DRAGON(w)->setFireAngle(ang);
                    w->TmpF += 0.06981317f;
                    if (w->Timer3) {
                        w->Timer3--;
                    } else {
                        em10DragonFireCk(em);
                    }
                }
            }
        }
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_2 = 2;
        }
        break;
    }
    if (em->hp <= 0) {
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        EmRoutineSet(em, 2, 7, 0, 1);
    } else {
        em10HandSet(em, 0);
    }
}
#endif // PARKED

// R1 == 0x54: room 222 dragon rider C: keeps the statue at home until the player passes x = -53000
// (Room_flg[0] bit31 clear), then tracks and fires like the others.
// FUNCTION: BIO4 0x0045D5C0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, EM10_DRAGON, EM_STATUS_IK_OFF, Mtx, PSMTXInverse, PSMTXMultVec, Room_flg, int, m, minv, throughOn
void em10_R1_R222DragonC(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Mtx minv;
    Vec pl;
    Vec v;
    f32 y;
    f32 ay;
    f32 ang;

    em->atari.throughOn();
    em->setStatus(EM_STATUS_IK_OFF);
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 90;
        em->r_no_2++;
    case 1:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            EM10_DRAGON(w)->getMtx(m, 0);
            PSMTXInverse(m, minv);
            PSMTXMultVec(minv, &pPL->pos, &pl);
            pl.y += 2000.0f;
            y = pl.y;
            ay = fabsf(y);
            if (y > 200.0f) {
                y = 200.0f;
            }
            if (y < -200.0f) {
                y = -200.0f;
            }
            if (pPL->pos.x > -53000.0f && !(pG->Room_flg[0] & 0x80000000)) {
                y = 200.0f;
                if (w->Timer <= 99) {
                    w->Timer = 100;
                }
                EM10_DRAGON(w)->setHome();
            } else {
                EM10_DRAGON(w)->setTarget(&pl);
                ang = EM10_DRAGON(w)->getAngle();
                ang = Muku(&pl, &pPL->pos, ang, 0.0061359233f);
                EM10_DRAGON(w)->addAngle(ang);
            }
            EM10_DRAGON(w)->setAngleY(y);
            if (w->Timer == 0) {
                if (ay < 150.0f && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) < 1.1170107f &&
                    (em->pos.x - pPL->pos.x) * (em->pos.x - pPL->pos.x) + (em->pos.y - pPL->pos.y) * (em->pos.y - pPL->pos.y) +
                            (em->pos.z - pPL->pos.z) * (em->pos.z - pPL->pos.z) <
                        400000000.0f) {
                    em->r_no_2++;
                    break;
                }
            } else {
                w->Timer--;
            }
        }
        MotionMove(em, 0);
        break;
    case 2:
        em10SetWaitMotion(em, 0);
        w->Timer = 150;
        w->TmpU32 = 0;
        EM10_DRAGON(w)->setFire();
        w->TmpF = 0.0f;
        w->TmpF2 = EM10_DRAGON(w)->getFireAngle();
        w->Timer2 = 30;
        w->Timer3 = 15;
        em->r_no_2++;
    case 3:
        if (w->pDragon) {
            EM10_DRAGON(w)->getMtx(m, 1);
            v.x = 0.0f;
            v.y = -1000.0f;
            v.z = 2000.0f;
            PSMTXMultVec(m, &v, &em->pos);
            if (w->Timer > 30) {
                if (w->Timer2) {
                    w->Timer2--;
                } else {
                    ang = SINF(w->TmpF) * 0.19634955f + w->TmpF2;
                    EM10_DRAGON(w)->setFireAngle(ang);
                    w->TmpF += 0.06981317f;
                    if (w->Timer3) {
                        w->Timer3--;
                    } else {
                        em10DragonFireCk(em);
                    }
                }
            }
        }
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_2 = 0;
        }
        break;
    }
    if (em->hp <= 0) {
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        EmRoutineSet(em, 2, 7, 0, 1);
    } else {
        em10HandSet(em, 0);
    }
}
#endif // PARKED

// R1 == 0x55: room 227 Ganado at the switch. Waits for the release flag and a 150-frame timer, then
// plays motion 0x9C and closes the emswitch (pSwitch) on the motion's seFlags bit0; leaves the post
// when the player gets within 2000 units of height.
// FUNCTION: BIO4 0x004624E0
// STATUS: stub
#if 0 // PARKED: C2059, C2105, cEmSwitch, int
void em10_R1_R227Barrel(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 150;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->pos.y < pPL->pos.y + 2000.0f) {
            em10WalkRtnSet(em);
        } else if (!(em->flag & 1)) {
            w->Timer = 0;
        } else if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_2++;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9C), PL_ARC_PTR(em->subArc, 0x9D), 30, 1, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        } else if ((em->Motion.Seq_old.Free & 1) && w->pSwitch) {
            ((cEmSwitch*) w->pSwitch)->setClose();
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x62: room 21B trolley Ganado, second kind: on release starts the torch / bowgun effects and
// dashes until em10JumpDownCk finds an edge to jump off.
// FUNCTION: BIO4 0x0043C4A0
// STATUS: stub
#if 0 // PARKED: EstSet
void em10_R1_R21BTrolleyJump2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->r_no_2++;
        }
        break;
    case 2:
        if (w->Wep_type == 7) {
            w->pWeapon->setEffAlways(0x10, 0x14);
            w->pWeapon->setEffFall(0x10, 0x1C);
        }
        if (w->Wep_type == 8 && w->pWeapon) {
            EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x1F, 0, w->EffKindIdArrow, w->pWeapon, 0);
        }
        em->r_no_3 = 0;
        em10SetDashMotion(em);
        w->Route_type = 0;
        em->r_no_2++;
    case 3:
        em->dmg.m_Timer = 0xA;
        MotionMove(em, 0);
        if (em10JumpDownCk(em)) {
            w->Target_pos = em->pos;
            em->set = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x63: room 303 burning Ganado. Starts dead-red (colour 0x80/0x30/0x30, burning cap) with no
// collision; on release it comes alive with full hp and dash-catches the player (DashCatch 0x39).
// FUNCTION: BIO4 0x0043C620
// STATUS: stub
#if 0 // PARKED: C2105, C2227, SndCall, Timer2, cModelInfo, cObj12, pModelInfo
void em10_R1_R303FireDash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0: {
        cModelInfo* info;
        em10SetWaitMotion(em, 0);
        em->be_flag &= ~2;
        em->hp = 0;
        for (info = em->pModelInfo; info; info = info->pList) {
            info->color[0] = 0x80;
            info->color[1] = 0x30;
            info->color[2] = 0x30;
        }
        if (w->pCap) {
            w->pCap->setBurn();
        }
        w->Timer2 = 10;
        em->r_no_2++;
    }
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->be_flag |= 2;
            em->hp = em->hp_max;
            SndCall(6, 3, &em->pos, 0, 0, em);
            if (w->Timer) {
                w->Timer--;
            } else {
                EmRoutineSet(em, 1, 0x39, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x56: room 10F Ganado jumping onto the player's gondola. Jumps (evtMot[0]), lands on the car
// (evtMot[1], shakes the player / Ashley with plem10DmGondolaShake), then hacks at it (evtMot[2]):
// the first hit damages the gondola, the second breaks it (cObjGondola setDamage / setBreak).
// FUNCTION: BIO4 0x0044C7A0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, EFF_ROOM, PSMTXMultVec, PSMTXMultVecSR, SetPlDamage, SetSubDamage, SndCall, atan2f, cObjGondola, int, mat, pl_life, throughOn
void em10_R1_R10FGJump(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    Vec dir;
    Vec dir2;

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 90;
        em->atari.throughOn();
        em->hp = 1;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->evtMot[0] && w->evtMot[1] && w->evtMot[2]) {
            cObjGondola* g = em10GetGondola(em);
            if (g) {
                f32 fr;
                if (em->set == 0x2C) {
                    fr = 682.0f;
                } else {
                    fr = 1286.0f;
                }
                if (g->Motion.Seq_frame == fr) {
                    w->pGondola = g;
                    em->r_no_2++;
                }
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 3, 1, 0);
        w->Timer = 10;
        em->r_no_2++;
    case 3:
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Be_flg |= 0x100000;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), w->evtMot[1], 0, 3, 1, 0);
        if (w->pGondola) {
            w->pGondola->setVib();
            if ((s16) pG->pl_life > 0) {
                SetPlDamage(em, plem10DmGondolaShake);
            }
            if (pSUB && pSUB->hp > 0) {
                SetSubDamage(em, (void (*)()) subem10DmGondolaShake);
            }
        }
        SndCall(6, 0xC, &em->pos, 0, 0, em);
        em->r_no_2++;
    case 5:
        w->Be_flg |= 0x100000;
        if (w->pGondola) {
            cModel* parts = w->pGondola->getPartsPtr(1);
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 1.0f;
            PSMTXMultVecSR(parts->mat, &dir, &dir);
            em->ang.y = atan2f(dir.x, dir.z);
            v.x = 51.9f;
            v.y = -2757.56f;
            v.z = -1419.8f;
            PSMTXMultVec(parts->mat, &v, &em->pos);
            if (em->Motion.Seq_frame > 41.7f && em->Motion.Seq_frame < 42.3f) {
                SndCall(6, 0xD, &em->pos, 0, 0, em);
                EstSet(em, -1, 0, 0, EFF_ROOM, 2, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 6:
        MotionSetCore(em, MOTION(em), w->evtMot[2], 0, 3, 5, 0);
        w->Timer = 5;
        em->r_no_2++;
    case 7:
        w->Be_flg |= 0x100000;
        if (w->pGondola) {
            cModel* parts = w->pGondola->getPartsPtr(1);
            dir2.x = 0.0f;
            dir2.y = 0.0f;
            dir2.z = 1.0f;
            PSMTXMultVecSR(parts->mat, &dir2, &dir2);
            em->ang.y = atan2f(dir2.x, dir2.z);
            v.x = 51.9f;
            v.y = -2757.56f;
            v.z = -1419.8f;
            PSMTXMultVec(parts->mat, &v, &em->pos);
        }
        MotionMove(em, 0);
        if (em->Motion.Seq_frame > 32.7f && em->Motion.Seq_frame < 33.3f) {
            SndCall(6, 0xD, &em->pos, 0, 0, em);
            EstSet(em, -1, 0, 0, EFF_ROOM, 2, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        if (w->Timer && em->Motion.Seq_frame > 32.7f && em->Motion.Seq_frame < 33.3f && w->pGondola) {
            w->Timer--;
            if (w->Timer > 0) {
                w->pGondola->setDamage();
                w->pGondola->setVib();
            } else {
                w->pGondola->setBreak();
                w->pGondola->setVib();
                if ((s16) pG->pl_life > 0) {
                    SetPlDamage(em, plem10DmGondolaShake);
                    pPL->r_no_3 = 1;
                }
                if (pSUB && pSUB->hp > 0) {
                    SetSubDamage(em, (void (*)()) subem10DmGondolaShake);
                    pSUB->r_no_3 = 1;
                }
                em->dmg.m_Timer = 0x80;
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x57: room 10F Ganado on the opposite gondola. Faces the player's car and throws its weapon
// (axe / dynamite / scythe via cEmWep::setThrow, Em10AtkTbl[5] / [6]), aimed at the player's predicted
// position, then re-arms with Wep_type2 (em10MakeWeapon) and throws again.
// FUNCTION: BIO4 0x0044CE80
// STATUS: stub
#if 0 // PARKED: Em10AtkTbl, LIMIT_ANGLE, Mtx, Muku, PI, PSMTXMultVecSR, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, SQRTF, atan2f, fRand1_1, int, l_pl, m, mat, setThrow, throughOn
void em10_R1_R10FGondola(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec tgt = { 26400.0f, 10279.0f, -32116.0f };
    Vec pos;
    Vec d;
    Mtx m;
    Vec wpos;
    Vec spd;
    Vec diff;
    Vec plPos;
    Vec wpos2;
    cModel* parts;
    f32 len;
    f32 t;
    f32 dist;

    parts = pPL->getPartsPtr(0);
    PSVECSubtract(&parts->world, &parts->world_old2, &d);
    len = SQRTF(d.x * d.x + d.y * d.y + d.z * d.z);
    PSVECScale(&d, &d, len / 350.0f + 10.0f);
    PSVECAdd(&pPL->pos, &d, &pos);
    pos.y += 1300.0f;
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->Motion.Mot_attr &= ~1;
        em->atari.throughOn();
        w->Timer = 90;
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        dist = (em->pos.x - tgt.x) * (em->pos.x - tgt.x) + (em->pos.y - tgt.y) * (em->pos.y - tgt.y) + (em->pos.z - tgt.z) * (em->pos.z - tgt.z);
        if (dist < 64000000.0f) {
            EmRoutineSet(em, 3, 3, 0, 0);
            return;
        }
        if (w->pWeapon == 0) {
            em->r_no_2 = 4;
        } else {
            // `dist` (the tgt distance above) reused for the limit: the multi-set pseudo is global and
            // takes f13 for both the `fmadds` result and the limit loads.
            switch (em->emset_no % 3) {
            default:
                dist = 400000000.0f;
                break;
            case 1:
                dist = 324000000.0f;
                break;
            case 2:
                dist = 256000000.0f;
                break;
            }
            if (em->l_pl < dist && em->pos.y < pPL->pos.y) {
                em->r_no_2 = 2;
            }
        }
        break;
    case 2: {
        int hokan = 0;
        if (em->flag & 0x01000000) {
            hokan = 0x40;
        }
        if (w->Wep_type != 6) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, hokan, 0x10);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x13D), PL_ARC_PTR(em->subArc, 0x13F), 10, hokan, 0);
        }
        w->Timer = 10;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        em->r_no_2++;
    }
    case 3:
        em->ang.y += Muku(&em->pos, &pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0)) {
            em->r_no_2++;
            break;
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            wpos.x = w->pWeapon->mat[0][3];
            wpos.y = w->pWeapon->mat[1][3];
            wpos.z = w->pWeapon->mat[2][3];
            PSVECSubtract(&pos, &wpos, &diff);
            len = SQRTF(diff.x * diff.x + diff.z * diff.z) * 0.0028571428f;
            t = diff.y / ((len + 1.0f) * 0.5f * len);
            spd.y = t * len;
            switch (Rnd() % 5) {
            case 0:
            default:
                spd.x = fRand1_1() * 10.0f;
                break;
            case 1:
                spd.x = 5.0f;
                break;
            case 2:
                spd.x = -5.0f;
                break;
            case 3:
                spd.x = 10.0f;
                break;
            case 4:
                spd.x = -10.0f;
                break;
            }
            spd.z = 350.0f;
            PSMTXRotRad(m, 'y', atan2f(diff.x, diff.z));
            PSMTXMultVecSR(m, &spd, &spd);
            if (w->Wep_type != 6) {
                w->pWeapon->setThrow(&spd, &Em10AtkTbl[5], t);
            } else {
                plPos = pPL->pos;
                plPos.y += 1500.0f;
                wpos2.x = w->pWeapon->mat[0][3];
                wpos2.y = w->pWeapon->mat[1][3];
                wpos2.z = w->pWeapon->mat[2][3];
                PSVECSubtract(&plPos, &wpos2, &spd);
#line 6995 "D:/Bio4/Prog/em10.cpp"
                VECNormalize(&spd, &spd);
                PSVECScale(&spd, &spd, 250.0f);
                w->pWeapon->setThrowScythe(&spd, &Em10AtkTbl[6]);
            }
            w->pWeapon = 0;
            w->Wep_type = 0;
            w->Throw_timer = 10;
        }
        break;
    case 4: {
        int hokan = 0;
        if (em->flag & 0x01000000) {
            hokan = 0x40;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x76), PL_ARC_PTR(em->subArc, 0x77), 10, hokan, 0);
        em->r_no_2++;
    }
    case 5:
        em->ang.y += Muku(&em->pos, &pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if ((em->Motion.Seq_old.Free & 4) && w->pWeapon2) {
            w->pWeapon = em10MakeWeapon(em, w->Wep_type2);
            if (w->pWeapon) {
                w->Wep_type = w->Wep_type2;
            } else {
                w->Wep_type = 0;
            }
            em10WeaponSet(em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x58: room 209 Ganado sitting down after the dash (motion 0xAE), 70 frames, then SitDown 0x1A.
// FUNCTION: BIO4 0x0043C790
// STATUS: stub
#if 0 // PARKED: C2105, int
void em10_R1_R209DashSit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xAE), PL_ARC_PTR(em->subArc, 0xAF), 0, 5, 0);
        w->Timer = 70;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            EmRoutineSet(em, 1, 0x1A, 0, 1);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x59: the room 201 claw Ganado pulls its claws out of the ground (motion 0x115, effect 0x76)
// and turns towards the player (0x116); then walks / dashes (em10ClawAtkEnd) or attacks.
// FUNCTION: BIO4 0x004626B0
// STATUS: stub
#if 0 // PARKED: Muku, PI, Timer2, int
void em10_R1_StickClaw(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->r_no_2 == 0 && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 1.9198622f) {
        em->r_no_2 = 2;
    }
    if (em10FindCk2(em)) {
        w->Find_timer = 150;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x115), 0, 3, 1, 0);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x76, 0, ESP_CORE_KIND_NONE, em, 0);
        w->Timer = 25;
        w->Timer2 = 46;
        if (em->r_no_3 == 0) {
            em10CallVoiceSe2(em, 0x71, 6);
        }
        if (w->Goto_mode == 0) {
            u8 r = Rnd() % 10;
            if (r > 4) {
                w->Goto_pos = pPL->pos;
            }
            w->Find_timer = 150;
        }
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_frame > 9.7f && em->Motion.Seq_frame < 10.3f) {
            w->Claw_rno_l = 1;
            w->Claw_rno_r = 1;
        }
        if (MotionMove(em, 0) && !em10AtkRtnCk(em, 0)) {
            em10ClawAtkEnd(em, w);
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x116), PL_ARC_PTR(em->subArc, 0x117), 10, 1, 0);
        em10CallVoiceSe2(em, 0x71, 6);
        em->r_no_3 = 1;
        if (w->Goto_mode == 0) {
            u8 r = Rnd() % 10;
            if (r > 4) {
                w->Goto_pos = pPL->pos;
            }
            w->Find_timer = 150;
        }
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0) || (em->Motion.Seq_old.Free & 1)) {
            if (!em10AtkRtnCk(em, 0)) {
                em10ClawAtkEnd(em, w);
            }
        } else if (em->Motion.Seq_frame > 9.7f && em->Motion.Seq_frame < 10.3f) {
            w->Claw_rno_l = 1;
            w->Claw_rno_r = 1;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x5A: room 11D chainsaw Ganado appearance 1. Plays the appear motion 0xE7 with the effect
// 0x2B, starts the chainsaw (work flag 0x80000000, SE 0x4C), notifies the others and dashes
// (jumping down / climbing over obstacles on the way).
// FUNCTION: BIO4 0x0044D800
// STATUS: stub
#if 0 // PARKED: EstSet, SndCall, int
void em10_R1_R11DAppear1(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        if (em->flag & 0x01000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 0x41, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 1, 0);
        }
        EstSet(em, -1, 0, 0, EFF_EM10, 0x2B, 0, ESP_CORE_KIND_NONE, em, 0);
        w->Timer = 30;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 4) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = Rnd() % 150 + 150;
            SndCall(6, 0x4C, &em->pos, 0, 0, em);
            w->Csaw_se_wait = 60;
            em10FindNotify(em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        em10SetDashMotion(em);
        em->r_no_2++;
    case 3:
        MotionMove(em, 0);
        if (!em10JumpDownCk(em)) {
            em10ClimbOverCk(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x5B: room 11D chainsaw Ganado appearance 2: waits for the release flag, revs the chainsaw
// (SE 0x50) through the event motion (setR11DMotion evtMot[0]), screams at frame 40, then walks.
// FUNCTION: BIO4 0x00462F40
// STATUS: stub
#if 0 // PARKED: EstSet, SndCall, SndStop
void em10_R1_R11DAppear2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        EM10_WK(em)->Be_flg |= 0x80000000;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->evtMot[0] == 0) {
            break;
        }
        em->r_no_2++;
    case 2:
        MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 0, 1, 0);
        SndStop(w->Seid_csaw, 0);
        w->Seid_csaw = SndCall(6, 0x50, &em->pos, 0, 0, em);
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        w->Be_flg |= 0x80000000;
        EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
        w->Be_flg |= 0x80000000;
        w->Csaw_fake_timer = Rnd() % 150 + 150;
        w->Csaw_se_wait = 60;
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_frame > 39.7f && em->Motion.Seq_frame < 40.3f) {
            SndStop(w->Seid_voice, 0);
            SndStop(w->Seid_breath, 0);
            w->Seid_voice = SndCall(6, 0x3D, &em->pos, 0, 0, em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x5C: room 212 Ganado riding the drill (pDrill; EM10_DRILL_FOLLOW keeps it on the root
// part). Idle, then the drive motion (evtMot[0]); when killed plays the die motion evtMot[1] and
// scores the kill (em10SetPoint).
// FUNCTION: BIO4 0x0043C8A0
// STATUS: stub
#if 0 // PARKED: AtariOff, EM10_DRILL_FOLLOW, EmReserveDropItem, EmSetDie
void em10_R1_R212Drill(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        AtariOff(&em->atari, 0xFCFF);
        w->Scale.x = 1.0f;
        w->Scale.y = 1.0f;
        w->Scale.z = 1.0f;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (!w->pDrill || !w->evtMot[0] || !w->evtMot[1]) {
            break;
        }
        em->setStatus(EM_STATUS_ACTIVE);
        em->r_no_2++;
    case 2:
        MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 0, 5, 0);
        em->r_no_2++;
    case 3:
        EM10_DRILL_FOLLOW;
        MotionMove(em, 0);
        if (em->hp <= 0) {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), w->evtMot[1], 0, 3, 1, 0);
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->r_no_2++;
    case 5:
        EM10_DRILL_FOLLOW;
        if (MotionMove(em, 0)) {
            EmSetDie(em);
            EmReserveDropItem(em);
            em10SetPoint(em);
            em->clearStatus(EM_STATUS_ACTIVE);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Room 209: Ganado on the mounted gatling (evtMot[0..3] = fire / reload / hit / die, setGatling).
// FUNCTION: BIO4 0x0044DA70
// STATUS: stub
#if 0 // PARKED: AtariOff, C2227, C2228, Ctrl12CntAdd, EmSetDie, PSMTXMultVec, cObjGatling, initializing
void em10_R1_R209Gatling(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cObjGatling* g = w->pGatling;
    Vec ofs;

    if (g) {
        ofs.x = 8.46f;
        ofs.y = 63.6f;
        ofs.z = -778.04f;
        PSMTXMultVec(g->mat, &ofs, &em->pos);
        em->ang.y = g->ang.y;
    }
    switch (em->r_no_2) {
    case 0:
        if (w->evtMot[0]) {
            MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 5, 5, 0);
        } else {
            em10SetWaitMotion(em, 0);
        }
        AtariOff(&em->atari, 0xFCFF);
        w->Scale.x = 1.0f;
        w->Scale.y = 1.0f;
        w->Scale.z = 1.0f;
        if (!(em->flag & 1) || !w->Gatling_mode) {
            MotionMove(em, 0);
            break;
        }
        if (w->pGatling) {
            w->pGatling->setFire();
        }
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->pGatling) {
            if (w->pGatling->ckReload()) {
                em->r_no_2++;
            } else if (w->pGatling && w->pGatling->ckBreak()) {
                em->r_no_2 = 6;
            }
        }
        break;
    case 2:
        if (w->evtMot[1]) {
            MotionSetCore(em, MOTION(em), w->evtMot[1], 0, 5, 1, 0);
        } else {
            em10SetWaitMotion(em, 0);
        }
        w->pGatling->stopFire();
        w->pGatling->setReload();
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            if (w->pGatling) {
                em->r_no_2 = 0;
            }
        } else if (w->pGatling && w->pGatling->ckBreak()) {
            em->r_no_2 = 6;
        }
        break;
    case 4:
        if (w->evtMot[2]) {
            MotionSetCore(em, MOTION(em), w->evtMot[2], 0, 5, 1, 0);
        } else {
            em10SetWaitMotion(em, 0);
        }
        w->pGatling->stopFire();
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        w->No_dmg_timer = 90;
        em->r_no_2++;
    case 5:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        } else if (w->pGatling && w->pGatling->ckBreak()) {
            em->r_no_2 = 6;
        }
        break;
    case 6:
        if (w->evtMot[3]) {
            MotionSetCore(em, MOTION(em), w->evtMot[3], 0, 5, 1, 0);
        } else {
            em10SetWaitMotion(em, 0);
        }
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        if (w->Parasite_on) {
            w->Parasite_on = 0;
            Ctrl12CntAdd(w->pCtrlGroup, 4, -1);
        }
        em10CoreBreak(em, 0);
        w->pGatling->stopFire();
        em->r_no_2++;
    case 7:
        if (MotionMove(em, 0)) {
            EmSetDie(em);
            em10SetPoint(em);
            em->clearStatus(EM_STATUS_ACTIVE);
            em->r_no_2++;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x5E: room 201 claw Ganado lying in wait (motion 0x108, work flag 8 = down so bullets do
// not react): on the event motions (setEvtMotion evtMot[0/1]) it stands up, is marked found and
// goes to StickClaw (0x59).
// FUNCTION: BIO4 0x0043CB80
// STATUS: stub
#if 0 // PARKED: C2105, SndCall, clrFlag100, int, setFlag100
void em10_R1_R201EventWait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x108), 0, 0, 5, 0);
        em->atari.clrFlag100();
        em->r_no_2++;
    case 1:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        w->Be_flg |= 8;
        MotionMove(em, 0);
        if (!w->evtMot[0] || !w->evtMot[1]) {
            break;
        }
        MotionMove(em, 0);
        em->r_no_2++;
    case 2:
        em->pos.x = 34588.95f;
        em->pos.y = -2003.77f;
        em->pos.z = -41650.69f;
        em->ang.y = -1.5707964f;
        MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 0, 5, 0);
        em->r_no_2++;
    case 3:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        w->Be_flg |= 8;
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), w->evtMot[1], 0, 0, 5, 0);
        em->setStatus(EM_STATUS_ACTIVE);
        em->setFindPL();
        w->Goto_pos = pPL->pos;
        w->Timer = 30;
        em10CallVoiceSe2(em, w->Se_tbl[4], 8);
        em->r_no_2++;
    case 5:
        if (w->Timer) {
            w->Timer--;
            w->Be_flg |= 8;
            if (w->Timer == 0) {
                em->atari.setFlag100();
            }
        }
        if (MotionMove(em, 0)) {
            em->atari.setFlag100();
            EmRoutineSet(em, 1, 0x59, 0, 0);
        } else if (MOTION(em)->Seq_frame > 14.7f && MOTION(em)->Seq_frame < 15.3f) {
            SndCall(6, 0x11, &em->pos, 0, 0, em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x5D: lost sight of the player. Idle motion; when the timer runs out the Ganado marks the
// player lost (flag 0x800000, clears found 0x100), picks a wander route and walks; seeing the player
// (em10FindCk2) goes straight back to the walk.
// FUNCTION: BIO4 0x004631C0
// STATUS: stub
void em10_R1_FindLost(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            w->Be_flg |= 0x800000;
            w->Be_flg &= ~0x100;
            w->Wander_route = em10GetWanderRoute(em);
            w->Wander_timer = 0;
            if (w->Claw_rno_l != 4) {
                w->Claw_rno_l = 3;
            }
            if (w->Claw_rno_r != 4) {
                w->Claw_rno_r = 3;
            }
            em10WalkRtnSet(em);
        }
        if (em10FindCk2(em)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}

// R1 == 0x4C: room 100 villager walking to its post: walks the route, then idles and returns to
// Wait (0) or Keeper (1) depending on the set.
// FUNCTION: BIO4 0x0043CF20
// STATUS: stub
void em10_R1_R100WalkStay(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        em10SetWalkMotion(em, 0);
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        em10SetWaitMotion(em, 10);
        em->r_no_2++;
    case 3:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->flag &= ~1;
            w->Be_flg &= ~0x100;
            if (em->Character != 1 && em->Character != 3) {
                EmRoutineSet(em, 1, 0, 0, 0);
            } else {
                EmRoutineSet(em, 1, 1, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}

// R1 == 0x4D: room 202 zealot pointing at the player (work flag 0x8000 keeps the finger / neck
// tracking on): plays the point motion and turns to face him, then walks when he comes close.
// FUNCTION: BIO4 0x0043D040
// STATUS: stub
void em10_R1_R202Finger(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;

    w->Be_flg |= 0x8000;
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        em->dmg.m_Timer = 2;
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->r_no_2++;
        }
        break;
    case 2:
        m0 = PL_ARC_PTR(em->subArc, 0x73);
        m1 = PL_ARC_PTR(em->subArc, 0x74);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    case 3:
        em->dmg.m_Timer = 2;
        if (em->Motion.Seq_old.Free & 8) {
            w->Be_flg |= 0x2000000;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 4:
        em10SetWaitMotion(em, 10);
        em->r_no_2++;
    case 5:
        MotionMove(em, 0);
        break;
    }
    em10HandSet(em, 0);
}

// R1 == 0x4E StayWalk: idle at the post (chainsaw revving if it carries one) for up to 600 frames or
// until a goto arrives, then marks the player found and walks (em10WalkRtnSet).
// FUNCTION: BIO4 0x004632A0
// STATUS: stub
#if 0 // PARKED: C2105, EstSet, int
void em10_R1_StayWalk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 600;
        if (w->Wep_type == 4) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = (u8) (Rnd() % 150) + 150;
            w->Csaw_se_wait = 60;
        }
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        r = em10GotoCk(em);
        if (r) {
            break;
        }
        if (w->Be_flg & 1) {
            w->Timer = r;
        }
        if (em->flag & 1) {
            w->Timer = r;
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            em->setFindPL();
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Waits for the player to come in front of the Ganado, then picks the weapon's attack routine.
// FUNCTION: BIO4 0x004634C0
// STATUS: stub
#if 0 // PARKED: EstSet, PSMTXMultVec, PSVECAdd, PSVECScale, PSVECSubtract, int, mat
void em10_R1_AttackWait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    Vec c;
    cModel* p;
    f32 d2;
    f32 dy;
    int no;

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Timer = 600;
        if (w->Wep_type == 4) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = (u8) (Rnd() % 150) + 150;
            w->Csaw_se_wait = 60;
        }
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em10GotoCk(em)) {
            return;
        }
        if ((w->Be_flg & 1) && w->Pl_rot > 1.5707964f) {
            em->setFindPL();
            em10WalkRtnSet(em);
            break;
        }
        p = pPL->getPartsPtr(0);
        PSVECSubtract(&p->world, &p->world_old2, &a);
        PSVECScale(&a, &a, 30.0f);
        PSVECAdd(&pPL->pos, &a, &b);
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 1000.0f;
        PSMTXMultVec(em->mat, &a, &c);
        d2 = (b.x - c.x) * (b.x - c.x) + (b.z - c.z) * (b.z - c.z);
        dy = b.y - c.y;
        dy = fabsf(dy);
        if (d2 < 1000000.0f && dy < 1000.0f) {
            no = 0;
            em->set = no;
            if (em->type == 0xA || em->type == 0xD) {
                EmRoutineSet(em, 1, 0x2B, 0, 0xA);
                return;
            }
            if (em->type == 2) {
                EmRoutineSet(em, 1, 0x23, 0, 0);
                return;
            }
            switch (w->Wep_type) {
            default:
            case 0:
                if (w->pShield) {
                    EmRoutineSet(em, 1, 0x27, 0, 0);
                    break;
                }
            case 9:
                EmRoutineSet(em, 1, 0x33, 0, 0);
                break;
            case 1:
                EmRoutineSet(em, 1, 0x29, 0, 0);
                break;
            case 2:
            case 3:
            case 7:
            case 0xA:
            case 0xB:
            case 0xF:
            case 0x10:
                EmRoutineSet(em, 1, 0x26, 0, 0);
                break;
            case 4:
                EmRoutineSet(em, 1, 0x2F, 0, 0);
                break;
            case 6:
                EmRoutineSet(em, 1, 0x2A, 0, 0xA);
                break;
            case 8:
                EmRoutineSet(em, 1, 0x21, 0, 0);
                break;
            case 0xC:
                EmRoutineSet(em, 1, 0x22, 0, 0);
                break;
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x05: room 100 villager: idles 90 frames, plays the turn motion (shield / claw variants),
// then walks off with motion 7; damage is only held (dmg.m_Timer 0x80).
// FUNCTION: BIO4 0x0043D1C0
// STATUS: stub
#if 0 // PARKED: C2105, int
void em10_R1_R100TurnWalk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    PlArc* arc;
    void* m0;
    void* m1;
    int flag;

    em->dmg.m_Timer = 0x80;
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        w->Timer = 90;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
            break;
        }
        em->r_no_2++;
    case 2:
        arc = em->subArc;
        m0 = PL_ARC_PTR(arc, 0x18);
        m1 = PL_ARC_PTR(arc, 0x19);
        flag = (w->Go_dir < 0.0f) ? 0x41 : 1;
        // direct `em->subArc` reads below: cse copies the `arc` load into a second pseudo (`mr r10, r11`)
        if (w->pShield) {
            m0 = PL_ARC_PTR(em->subArc, 0x16C);
            m1 = PL_ARC_PTR(em->subArc, 0x16D);
            flag = (em->flag & 0x1000000) ? 0x41 : 1;
        }
        if (em->type == 10 || em->type == 13) {
            m0 = PL_ARC_PTR(em->subArc, 0x116);
            m1 = PL_ARC_PTR(em->subArc, 0x117);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 4:
        em10SetWalkMotion(em, 7);
        em->r_no_2++;
    case 5:
        MotionMove(em, 0);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x06: room 100 cliff-top villager: dead-on-arrival (hp 0) playing the fall motion 5 on its
// own or in sync with the other set 2/3/4 Ganados, then fades out (invisible_factor) and goes inactive.
// FUNCTION: BIO4 0x0043D3F0
// STATUS: stub
#if 0 // PARKED: C2105, C2228, EmMgr, EmSetDie, SndCall, int, invisible_factor, l_pl
void em10_R1_R100Cliff(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    u32 i;

    em->dmg.m_Timer = 0x80;
    switch (em->r_no_2) {
    case 0:
        if (w->evtMot[0]) {
            m0 = w->evtMot[0];
            m1 = w->evtMot[4];
        } else {
            m0 = PL_ARC_PTR(em->subArc, 5);
            m1 = 0;
        }
        MotionSetCore(em, MOTION(em), m0, m1, 0, 5, 0);
        MotionMove(em, 0);
        em->hp = 0;
        em->atari.m_flag &= ~0x200;
        em->r_no_2++;
        break;
    case 1:
        if (w->evtMot[0]) {
            m0 = w->evtMot[0];
            m1 = w->evtMot[4];
        } else {
            m0 = PL_ARC_PTR(em->subArc, 5);
            m1 = 0;
        }
        MotionSetCore(em, MOTION(em), m0, m1, 0, 5, 0);
        MotionMove(em, 0);
        w->Timer = (*(u16*) m0 & 0x3FFF) - 10;
        if (em->l_pl < 784000000.0f || (em->flag & 1)) {
            em->r_no_2++;
            em->flag |= 1;
            if (em->set == 2) {
                SndCall(6, 4, &em->pos, 0, 0, em);
            }
            for (i = 0; i < EmMgr.getArrayNum(); i++) {
                cEm* e = EmMgr.fastAt(i);
                if ((e->be_flag & 0x201) != 1) {
                    continue;
                }
                if (e->id <= 0xF) {
                    continue;
                }
                if (e->id > 0x20) {
                    continue;
                }
                if (e->hp <= 0) {
                    continue;
                }
                if (e == em) {
                    continue;
                }
                switch (e->set) {
                case 2:
                case 3:
                case 4:
                    e->flag |= 1;
                    break;
                }
            }
        }
        break;
    case 2:
        EmSetDie(em);
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->invisible_factor -= 0.1f;
            if (em->invisible_factor <= 0.0f) {
                em->invisible_factor = 0.0f;
                em->atari.m_flag &= 0xFCFF;
                em->be_flag &= ~2;
                em->clearStatus(EM_STATUS_ACTIVE);
                em->r_no_2++;
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Room 101: the Ganado carrying a bucket between the two positions of its route (x38D picks it).
// FUNCTION: BIO4 0x00478D80
// STATUS: stub
#if 0 // PARKED: C1003, C2228, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, Frame_cnt, LIMIT_ANGLE, Motion, MotionSetCore, Muku, RouteCkToPos, SndCall, ang, em10_r101_bucket_pos2
void em10_R1_R101Bucket(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* p;
    Vec* rp;
    Vec* rp2;
    int no;
    f32 d2;

    no = em->set == 6;
    if (em->set == 0x1A) {
        no = 2;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1AF), PL_ARC_PTR(em->subArc, 0x1B0), 3, 1, 0);
        if (w->Wep_type == 5 && w->pWeapon && w->mot[0x2D]) {
            w->pWeapon->ang.x = 0.0f;
            w->pWeapon->ang.y = 0.0f;
            w->pWeapon->ang.z = 0.0f;
            MotionSetCore(w->pWeapon, MOTION(w->pWeapon), w->mot[0x2D], 0, 0, 0, 0);
        }
        EstSet(em, -1, 0, 0, EFF_EM10, 0x12, 0, w->EffKindIdWork, em, 0);
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(6, 0xF, &em->pos, 0, 0, em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1B1), PL_ARC_PTR(em->subArc, 0x1B2), 3, 5, 0);
        if (w->Wep_type == 5 && w->pWeapon && w->mot[0x2E]) {
            MotionSetCore(w->pWeapon, MOTION(w->pWeapon), w->mot[0x2E], 0, 0, 4, 0);
        }
        w->Target_pos = em10_r101_bucket_pos2[no];
        em->r_no_2++;
    case 3:
        if ((pG->Frame_cnt & 7) == (em->emset_no & 7)) {
            RouteCkToPos(em, &em10_r101_bucket_pos2[no], &w->Target_pos, 0, 0);
        }
        em->ang.y += Muku(&em->pos, &w->Target_pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        rp2 = &em10_r101_bucket_pos2[no];
        d2 = (em->pos.x - rp2->x) * (em->pos.x - rp2->x) + (em->pos.z - rp2->z) * (em->pos.z - rp2->z);
        if (d2 < 250000.0f) {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1B3), PL_ARC_PTR(em->subArc, 0x1B4), 10, 1, 0);
        if (w->Wep_type == 5 && w->pWeapon && w->mot[0x2E]) {
            MotionSetCore(w->pWeapon, MOTION(w->pWeapon), w->mot[0x2F], 0, 0, 0, 0);
        }
        EffectEspDelete(0, w->EffKindIdWork, em, 0);
        EffectEspgenDelete(0, w->EffKindIdWork, em);
        EffectEfmDelete(0, w->EffKindIdWork, em);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x13, 0, w->EffKindIdWork, em, 0);
        em->r_no_2++;
    case 5:
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(6, 0xF, &em->pos, 0, 0, em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 6:
        EffectEspDelete(0, w->EffKindIdWork, em, 0);
        EffectEspgenDelete(0, w->EffKindIdWork, em);
        EffectEfmDelete(0, w->EffKindIdWork, em);
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 8), PL_ARC_PTR(em->subArc, 0xB), 5, 5, 0);
        w->Target_pos = em10_r101_bucket_pos[no];
        em->r_no_2++;
    case 7:
        if ((pG->Frame_cnt & 7) == (em->emset_no & 7)) {
            RouteCkToPos(em, &em10_r101_bucket_pos[no], &w->Target_pos, 0, 0);
        }
        em->ang.y += Muku(&em->pos, &w->Target_pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        p = em->getPartsPtr(10);
        p->ang.x = 0.017601645f;
        p->ang.y = 0.05197416f;
        p->ang.z = 0.04245688f;
        RotMatrix(p->l_mat, &p->ang);
        TransMatrix(p->l_mat, &p->pos);
        ScaleMatrix(p->l_mat, &p->scale);
        rp = &em10_r101_bucket_pos[no];
        d2 = (em->pos.x - rp->x) * (em->pos.x - rp->x) + (em->pos.z - rp->z) * (em->pos.z - rp->z);
        if (d2 < 90000.0f) {
            em->r_no_2 = 0;
        }
        break;
    }
    if (!em10FindCk(em, 0)) {
        if (w->Be_flg & 0x100) {
            em10WalkRtnSet(em);
        } else {
            em10HandSet(em, 0);
        }
    }
}
#endif // PARKED

// R1 == 0x08: room 101 villager digging with the hoe (motions 0x1B5..0x1B7, dirt effects 0xF/0x11/
// 0x15, SE 0x10) 5..9 strokes, then straightens up; breaks off for the player (em10FindCk).
// FUNCTION: BIO4 0x00479490
// STATUS: stub
#if 0 // PARKED: C2105, SndCall, int
void em10_R1_R101Suki(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1B5), PL_ARC_PTR(em->subArc, 0x1B6), 3, 5, 0);
        w->Timer = Rnd() % 5 + 5;
        EstSet(em, -1, 0, 0, EFF_EM10, 0x15, 0, w->EffKindIdWork, em, 0);
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(6, 0x10, &em->pos, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 1) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0xF, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        if (em->Motion.Seq_old.Free & 4) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x11, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        if (MotionMove(em, 0)) {
            if (w->Timer) {
                w->Timer--;
                EstSet(em, -1, 0, 0, EFF_EM10, 0x15, 0, w->EffKindIdWork, em, 0);
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1B7), 0, 3, 1, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    if (!em10FindCk(em, 0)) {
        if (w->Be_flg & 0x100) {
            em10WalkRtnSet(em);
        } else {
            em10HandSet(em, 0);
        }
    }
}
#endif // PARKED

// R1 == 0x65 UFOCatch: the Ganado grabbed by the crane / claw event (setUFOCatch evtMot[0/1]):
// collision off, hangs (evtMot[0]), is dropped (evtMot[1] or motion 0x21) and disappears (setLost).
// FUNCTION: BIO4 0x0043D730
// STATUS: stub
#if 0 // PARKED: throughOn
void em10_R1_UFOCatch(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        em->atari.throughOn();
        if (w->evtMot[0]) {
            MotionSetCore(em, MOTION(em), w->evtMot[0], 0, 10, 5, 0);
        } else {
            em10SetWaitMotion(em, 0);
        }
        em->flag &= ~1;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->r_no_2++;
        }
        break;
    case 2:
        if (w->evtMot[1]) {
            MotionSetCore(em, MOTION(em), w->evtMot[1], 0, 3, 1, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x21), 0, 3, 1, 0);
        }
        em10SetDamageVoice(em, w->Se_tbl[14], w->Se_tbl[14]);
        em->flag &= ~1;
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em->setLost();
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x67: room 30F Ganado jumping off the bulldozer (room archive motion 0x39, airborne flags
// 0x10080000): lands on the floor found below with SE 8/5 and the landing motion 0x25, dust effect
// 0x31 / 0x17, then walks with a 15-frame attack delay.
// FUNCTION: BIO4 0x00463990
// STATUS: stub
#if 0 // PARKED: C2227, C2228, EM_STATUS_IK_OFF, SatMgr, SndCall, pRoom, pos_old
void em10_R1_R30FBullJump(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    int end;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR((PlArc*) pG->pRoom, 0x39), PL_ARC_PTR((PlArc*) pG->pRoom, 0x3A), 3, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[17], 8);
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x10080000;
        em->setStatus(EM_STATUS_IK_OFF);
        end = MotionMove(em, 0);
        if (em->Motion.Seq_old.Free & 4) {
            f32 y;
            v = em->pos;
            v.y = em->pos_old.y;
            y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
            // The landing tail is repeated in both arms (jump2 cross-jumps it): with the MotionSetCore call
            // in the SndCall's block, the SndCall arg `li r3, 8` is issued after `mr r8` like the target.
            if (em->pos.y < y) {
                em->pos.y = y;
                w->Spd.y = 0.0f;
                SndCall(8, 5, &em->pos, em->id, 0, em);
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x25), 0, 3, 1, 0);
                MotionMove(em, 0);
                em->r_no_2 = 2;
            } else if (end) {
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x25), 0, 3, 1, 0);
                MotionMove(em, 0);
                em->r_no_2 = 2;
            }
        }
        break;
    case 2:
        if (ChkWaterEffectEnable(&em->pos)) {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, em, 0);
        } else {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x68: room 320 gatling-room Ganado: idles already alerted (flag 0x100) and turns to the
// player until it sees him (em10FindCk) or the room's goto arrives, then walks / attacks.
// FUNCTION: BIO4 0x00479790
// STATUS: stub
#if 0 // PARKED: DBG_EM_NO_ATK, DbgFlagChk, LIMIT_ANGLE, Muku, PI, ashley_life, l_pl, pl_life
void em10_R1_R320Gatling(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Be_flg |= 0x100;
        em->r_no_2++;
    case 1:
        int one = 1;
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if (!DbgFlagChk(pG, DBG_EM_NO_ATK) && !em10GotoCk(em)) {
            if (em->type == 6) {
                em->setFindPL();
                w->Be_flg |= 0x40000;
            } else if ((s16) pG->pl_life <= 0 || (pSUB && (s16) pG->ashley_life <= 0)) {
                do { // loop notes keep `one` at its declaration (update_equiv_regs moves single-use constants only outside loops)
                    EmRoutineSet(em, one, 0x1B, 0, 0);
                } while (0);
            } else if (!em10FindCk(em, 0)) {
                if (em->l_pl < 9000000.0f) {
                    em->set = 0;
                    em10WalkRtnSet(em);
                } else {
                    em10AtkRtnCk(em, 0);
                }
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x69: room 300 Ganado at the gatling: waits for the release flag, plays the event motion
// (setEvtMotion evtMot[0]/[4]) with its SE 0xB3 and effect, then walks.
// FUNCTION: BIO4 0x00463C70
// STATUS: stub
#if 0 // PARKED: EFF_ROOM, SndCall
void em10_R1_R300Gatling(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 0);
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (!w->evtMot[0] || !w->evtMot[4]) {
            break;
        }
        em->r_no_2++;
    case 2:
        MotionSetCore(em, MOTION(em), w->evtMot[0], w->evtMot[4], 0, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        SndCall(8, 0xB3, &em->pos, em->id, 0, em);
        EstSet(em, -1, 0, 0, EFF_ROOM, 0x11, 0, ESP_CORE_KIND_NONE, em, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 9, &em->pos, 0, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x09: room 101 villager pushing the cart (pCart) along em10_r101_cart_route, turning at the
// points, with the cart creak SE / effect 0x16 every 15..45 frames; drops it for the player.
// FUNCTION: BIO4 0x00479B40
// STATUS: stub
#if 0 // PARKED: C2105, C2227, EstSet, LIMIT_ANGLE, MotionSetCore, Muku, PI, PSMTXMultVec, RouteCkToPos, Timer2, cObj12, em10_r101_cart_route, int, mat
void em10_R1_R101Cart(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1B8), PL_ARC_PTR(em->subArc, 0x1B9), 3, 5, 0);
        if (w->pCart && w->mot[50]) {
            MotionSetCore(w->pCart, MOTION(w->pCart), w->mot[50], 0, 0, 4, 0);
        }
        w->Timer = 0;
        w->Timer2 = Rnd() % 15 + 5;
        em->r_no_2++;
    case 1:
        RouteCkToPos(em, &em10_r101_cart_route[w->Timer], &w->Go_pos, 0, 0);
        w->Go_dir = Muku(&em->pos, &w->Go_pos, em->ang.y, PI);
        w->Go_rot = fabsf(w->Go_dir);
        {
            Vec* rp = &em10_r101_cart_route[w->Timer];
            f32 dx = em->pos.x - rp->x;
            f32 dz = em->pos.z - rp->z;
            w->L_go = dx * dx + dz * dz;
        }
        em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.012271847f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (w->L_go < 1000000.0f) {
            w->Timer++;
            if (w->Timer > 6) {
                w->Timer = 0;
            }
        }
        MotionMove(em, 0);
        break;
    }
    if (!em10FindCk(em, 0)) {
        if (w->Be_flg & 0x100) {
            em10WalkRtnSet(em);
        } else {
            em10HandSet(em, 0);
            if (w->pCart) {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = 1700.0f;
                PSMTXMultVec(em->mat, &v, &w->pCart->pos);
                w->pCart->ang = em->ang;
            }
            if (w->Timer2) {
                w->Timer2--;
            } else {
                w->Timer2 = Rnd() % 30 + 15;
                EstSet(w->pCart, -1, 0, 0, EFF_EM10, 0x16, 0, ESP_CORE_KIND_NONE, w->pCart, 0);
            }
        }
    }
}
#endif // PARKED

// R1 == 0x0A EvtDash: scripted dash (starts the chainsaw if it carries one): runs 120 frames on a
// random route then hands over to the normal Dash (0x11) with a random Route_type; step 2/3 is the
// chainsaw appear motion 0xE7.
// FUNCTION: BIO4 0x0043D930
// STATUS: stub
#if 0 // PARKED: C2105, EstSet, FlagChkSignW, SndCall, int
void em10_R1_EvtDash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em->r_no_3 = Rnd() % 256;
        em10SetDashMotion(em);
        if (w->Wep_type == 4 && FlagChkSignW(w->Be_flg, 0)) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = Rnd() % 150 + 150;
            SndCall(6, 0x4C, &em->pos, 0, 0, em);
            w->Csaw_se_wait = 60;
        }
        w->Timer = 120;
        em->r_no_3 = 0;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_0 = 1;
            em->r_no_1 = 0x11;
            em->r_no_2 = 0;
            em->r_no_3 = (u8) (MOTION(em)->Seq_frame * 255.0f / (f32) MOTION(em)->Seq_frame_num);
            w->Route_type = Rnd() % 7;
        }
        break;
    case 2:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 0x41, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 1, 0);
        }
        EstSet(em, -1, 0, 0, EFF_EM10, 0x2B, 0, ESP_CORE_KIND_NONE, em, 0);
        w->Timer = 30;
        em->r_no_2++;
    case 3:
        if (em->Motion.Seq_old.Free & 4) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = Rnd() % 150 + 150;
            SndCall(6, 0x4C, &em->pos, 0, 0, em);
            w->Csaw_se_wait = 60;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x0B EvtWalk: scripted walk (motion 7, chainsaw started) for 120 frames, then the normal
// Walk (0x10) with a random Route_type.
// FUNCTION: BIO4 0x0043DDE0
// STATUS: stub
#if 0 // PARKED: C2105, EstSet, SndCall, int
void em10_R1_EvtWalk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em->r_no_3 = Rnd() % 256;
        em10SetWalkMotion(em, 7);
        if (w->Wep_type == 4 && (w->Be_flg & 0x80000000)) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = Rnd() % 150 + 150;
            SndCall(6, 0x4C, &em->pos, 0, 0, em);
            w->Csaw_se_wait = 60;
        }
        w->Timer = 120;
        em->r_no_3 = 0;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_0 = 1;
            em->r_no_1 = 0x10;
            em->r_no_2 = 0;
            em->r_no_3 = (u8) (MOTION(em)->Seq_frame * 255.0f / (f32) MOTION(em)->Seq_frame_num);
            w->Route_type = Rnd() % 11;
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x0C Pickup: picks the weapon up from the ground / takes the second weapon (Wep_type2 ->
// pWep via em10MakeWeapon or pWeapon2, em10WeaponSet), then starts the chainsaw (C_SawStart 0x0E),
// lights the dynamite (BombIgnition 0x0F) or walks.
// FUNCTION: BIO4 0x00463FA0
// STATUS: stub
#if 0 // PARKED: setTransMode
void em10_R1_Pickup(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    if ((w->Be_flg & 0x100) && (em->flag & 0x2000)) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x76), PL_ARC_PTR(em->subArc, 0x77), 10, flag, 0);
        em->r_no_2++;
    case 1:
        if ((em->Motion.Seq_old.Free & 4) && w->pWeapon2) {
            if (em->flag & 0x10000) {
                w->pWeapon = em10MakeWeapon(em, w->Wep_type2);
                if (w->pWeapon) {
                    w->Wep_type = w->Wep_type2;
                } else {
                    w->Wep_type = 0;
                }
            } else {
                w->pWeapon2->setTransMode(1);
                w->pWeapon = w->pWeapon2;
                w->Wep_type = w->Wep_type2;
                w->pWeapon2 = 0;
                w->Wep_type2 = 0;
            }
            em10WeaponSet(em);
        }
        if (MotionMove(em, 0)) {
            if (w->pWeapon && w->Wep_type == 4 && !(w->Be_flg & 0x80000000)) {
                EmRoutineSet(em, 1, 0xE, 0, 0);
                break;
            }
            if (w->pWeapon && w->Wep_type == 9 && w->Fire_timer == 0 && (w->L_pl_route < 15000.0f || em->Character == 2) && (w->Be_flg & 1)) {
                if (em->flag & 0x10000) {
                    EmRoutineSet(em, 1, 0xF, 0, 0);
                    break;
                }
                if (em->pos.y > pPL->pos.y - 500.0f) {
                    EmRoutineSet(em, 1, 0xF, 0, 0);
                    break;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x0D Find: the "spotted the player" reaction: turns to him with the point / shout motion
// 0x73, calls the find voice (Se_tbl[4]), notifies the others (em10FindNotify) and dashes or walks.
// FUNCTION: BIO4 0x00464260
// STATUS: stub
#if 0 // PARKED: C2105, LIMIT_ANGLE, Muku, int
void em10_R1_Find(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        m0 = PL_ARC_PTR(em->subArc, 0x73);
        m1 = PL_ARC_PTR(em->subArc, 0x74);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        w->Timer = 30;
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (em->Motion.Seq_old.Free & 0x20) {
            em10CallVoiceSe2(em, w->Se_tbl[4], 8);
        }
        if (em->Motion.Seq_old.Free & 8) {
            w->Be_flg |= 0x2000000;
        }
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                em10FindNotify(em);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->flag & 0x2000000) {
                EmRoutineSet(em, 1, 0x11, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 2);
    em10GotoCk(em);
}
#endif // PARKED

// R1 == 0x0E C_SawStart: the chainsaw Ganado starts the saw (motion 0xE7, effect 0x2B, SE 0x4C,
// work flag 0x80000000 = saw running), notifies the others and dashes / walks / turns (Turn180).
// FUNCTION: BIO4 0x00464470
// STATUS: stub
#if 0 // PARKED: Em10AtkTbl, LIMIT_ANGLE, Muku, PI, PSVECScale, VECNormalize, setThrowScythe
void em10_R1_C_SawStart(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 0x41, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE7), PL_ARC_PTR(em->subArc, 0xE8), 10, 1, 0);
        }
        EstSet(em, -1, 0, 0, EFF_EM10, 0x2B, 0, ESP_CORE_KIND_NONE, em, 0);
        w->Timer = 30;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 4) {
            if (em->type == 0x16) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x96, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            } else {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 9, 0, w->EffKindIdCsaw, w->pWeapon, 0);
            }
            w->Be_flg |= 0x80000000;
            w->Csaw_fake_timer = Rnd() % 150 + 150;
            SndCall(6, 0x4C, &em->pos, 0, 0, em);
            w->Csaw_se_wait = 60;
            em10FindNotify(em);
        }
        if (MotionMove(em, 0)) {
            if (em->flag & 0x2000000) {
                EmRoutineSet(em, 1, 0x11, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        } else if ((em->Motion.Seq_old.Free & 1) && w->Go_rot > 1.9634955f) {
            EmRoutineSet(em, 1, 0x15, 0, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x0F BombIgnition: the dynamite Ganado lights the fuse (motion 0x7E, fuse effects 0x2D /
// 0x2F, SE 0x94) and goes to the throw check (em10ThrowBombCk) or the dash.
// FUNCTION: BIO4 0x0045DEF0
// STATUS: stub
#if 0 // PARKED: C2105, int
void em10_R1_BombIgnition(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec ofs;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x7E), PL_ARC_PTR(em->subArc, 0x7F), 10, 0x41, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x7E), PL_ARC_PTR(em->subArc, 0x7F), 10, 1, 0);
        }
        w->Timer = 30;
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 1, 0x11, 0, 0);
        } else if (em->Motion.Seq_old.Free & 1) {
            w->Fire_timer = 9999;
            w->pWeapon->setEffAlways(0x10, 0x2D);
            ofs.x = 0.0f;
            ofs.y = 40.0f;
            ofs.z = 60.0f;
            w->pWeapon->setEffAlways2(0x10, 0x2F, 0, &ofs, 3);
            SndCall(8, 0x94, &em->pos, em->id, 0, em);
            em10ThrowBombCk(em);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// Branch check of Walk (0x10): a dead Ganado goes to Dm_Small; else goto / obstacle actions (door,
// rack, ladders, jumps, climb over, window), the return-to-start check, RoofWait (0x1C) when stuck
// above the player, the dynamite ignition / claw stick / lost checks, back to Keeper near Keep_pos,
// and em10GotoPosCk.
// FUNCTION: BIO4 0x0045E0D0
// STATUS: stub
#if 0 // PARKED: DBG_EM_NO_ATK, DbgFlagChk, Muku, PI, l_pl
void em10_R1_br_Walk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0) {
        int one = 1; // kept in a callee-saved reg across the calls (docs/matching.md)
        if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
            EmRoutineSet(em, one, 0, 0, 0);
        } else if (!em10GotoCk(em) && !em10DoorOpenCk(em, 0) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em)) {
            em10ReturnStartPosCk(em);
            if (!em10ClimbOverCk(em) && !em10WindowCk(em)) {
                if ((em->flag & 0x400) && w->Goto_mode == 0 && w->HoseiCnt > 30 && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) < 0.5235988f) {
                    EmRoutineSet(em, one, 0x1C, 0, 0);
                } else if (!em10IgnitionCk(em) && !em10ClawStickCK(em) && !em10FindLostCk(em)) {
                    if (w->Be_flg & 0x20000000) {
                        f32 dx = em->pos.x - w->Keep_pos.x;
                        f32 dz = em->pos.z - w->Keep_pos.z;
                        if (dx * dx + dz * dz < 4000000.0f) {
                            w->Be_flg &= ~0x20000000;
                            EmRoutineSet(em, 1, 1, 0, 0);
                            return;
                        }
                        if (em->l_pl < 4000000.0f && w->L_guard < em->Guard_r) {
                            w->Be_flg &= ~0x20000000;
                        }
                    }
                    em10GotoPosCk(em);
                }
            }
        }
    }
}
#endif // PARKED

// R1 == 0x10 Walk: walks the route towards the player (Go_pos) with the walk motion (Route_type
// variants), turning at 0.157 rad/frame; a killed Ganado goes to Dm_KnockOut, a dead player to Stay;
// checks attack / turn-around (Turn180 when the target is behind) / stay / dash / threat / head lock
// (Guard 0x1D or GuardWalk 0x14) / sight, the type 0xA claw Ganado gives up into FindLost; runs the
// breath, chainsaw and behind-player SEs and the chainsaw walk attack.
// FUNCTION: BIO4 0x00479F80
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_BACKSIGN, Ctrl12Ck, LIMIT_ANGLE, Muku, Muku2, PI, Timer2, Timer3, ashley_life, int, l_pl, pl_life
void em10_R1_Walk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;
    int r;
    f32 a;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10MouthPartsReset(em);
        em10SetWalkMotion(em, 7);
        w->Timer = Rnd() % 2;
        w->Timer2 = 0;
        w->Timer3 = 15;
        w->TmpF2 = 0.0f;
        if (w->Be_flg & 0x100) {
            w->Pl_in_ck = 1;
        }
        em->r_no_2++;
    case 1:
        if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
            a = Muku(&em->pos, &pPL->pos, em->ang.y, PI);
        } else {
            a = Muku(&em->pos, &w->Go_pos, em->ang.y, PI);
        }
        w->TmpF2 = a * 0.3f;
        w->TmpF2 = Muku2(0.0f, w->TmpF2, 0.15707964f);
        em->ang.y += w->TmpF2;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        end = MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            if (end) {
                EmRoutineSet(em, 2, 9, 0, 0);
            }
            break;
        }
        if ((s16) pG->pl_life <= 0) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (pSUB && (s16) pG->ashley_life <= 0) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (em10AtkRtnCk(em, 0)) {
            break;
        }
        if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
            if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 2.7488937f && em->l_pl > 36000000.0f) {
                EmRoutineSet(em, 1, 0x15, 0, 0);
                break;
            }
        } else if (w->Go_rot > 2.7488937f && em->l_pl > 36000000.0f) {
            EmRoutineSet(em, 1, 0x15, 0, 0);
            break;
        }
        if (end) {
            if (em10StayCk(em)) {
                break;
            }
            if (em10DashCk(em)) {
                break;
            }
            if ((u8) (Rnd() % 10) == 0 && !Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BACKSIGN)) {
                em10CallVoiceSe2(em, w->Se_tbl[15], 8);
            }
        }
        if (w->Timer == 0) {
            if (em10ThreatCk(em)) {
                break;
            }
        }
        if (w->Timer) {
            if (em10HeadLockCk(em)) {
                if (++w->Timer2 > 10) {
                    if ((u8) (Rnd() % 3)) {
                        EmRoutineSet(em, 1, 0x1D, 0, 0);
                    } else {
                        EmRoutineSet(em, 1, 0x14, 0, 0);
                    }
                    break;
                }
            } else {
                w->Timer2 = 0;
            }
        }
        r = em10FindCk(em, 2);
        if (!r && (w->Be_flg & 0x800000) && end && (Rnd() & 1) && (s16) w->Wander_timer > 150 && em->type == 0xA) {
            EmRoutineSet(em, 1, 0x5D, 0, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    em10CsawSignSe(em);
    em10BehindSeCk(em);
    if (em->type == 0x16) {
        EmRoutineSet(em, 1, 0x6D, 0, 0);
    }
}
#endif // PARKED

// Branch check of Dash (0x11): like br_Walk (goto, obstacle actions, return, ignition, claw stick,
// lost, Keeper near Keep_pos, GotoPosCk) without the RoofWait case.
// FUNCTION: BIO4 0x0045E370
// STATUS: stub
#if 0 // PARKED: DBG_EM_NO_ATK, DbgFlagChk, l_pl
void em10_R1_br_Dash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0) {
        int one = 1; // kept in a callee-saved reg across the calls (docs/matching.md)
        if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
            EmRoutineSet(em, one, 0, 0, 0);
        } else if (!em10GotoCk(em) && !em10DoorOpenCk(em, 0) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em)) {
            em10ReturnStartPosCk(em);
            if (!em10ClimbOverCk(em) && !em10WindowCk(em) && !em10IgnitionCk(em) && !em10ClawStickCK(em) && !em10FindLostCk(em)) {
                if (w->Be_flg & 0x20000000) {
                    f32 dx = em->pos.x - w->Keep_pos.x;
                    f32 dz = em->pos.z - w->Keep_pos.z;
                    if (dx * dx + dz * dz < 4000000.0f) {
                        w->Be_flg &= ~0x20000000;
                        EmRoutineSet(em, one, one, 0, 0);
                        return;
                    }
                    if (em->l_pl < 4000000.0f && w->L_guard < em->Guard_r) {
                        w->Be_flg &= ~0x20000000;
                    }
                }
                em10GotoPosCk(em);
            }
        }
    }
}
#endif // PARKED

// R1 == 0x11 Dash: runs at the player (work flag 0x40 = dashing, not for chainsaw / shield carriers)
// for 5..7 route steps then drops back to Walk with a new Dash_wait; attack checks, Turn180 when the
// target is behind, the chainsaw walk attack (0x6D), and the SEs.
// FUNCTION: BIO4 0x0047A720
// STATUS: stub
#if 0 // PARKED: C1003, int
void em10_R1_Dash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;
    f32 lim;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    w->Be_flg |= 0x40;
    if (w->Wep_type == 4 || w->pShield) {
        w->Be_flg &= ~0x40;
    }
    switch (em->r_no_2) {
    case 0:
        em->flag &= ~0x80;
        em10SetDashMotion(em);
        w->Timer = (u8) (Rnd() % 3) + 5;
        if (w->Ganado) {
            w->Dash_wait = (int) Rnd() % 450 + 150;
        } else {
            w->Dash_wait = (u8) (Rnd() % 150) + 150;
        }
        if (em->type == 0xA) {
            w->Timer3 = 60;
        } else {
            w->Timer3 = 15;
        }
        if (w->Be_flg & 0x100) {
            w->Pl_in_ck = 1;
        }
        em->r_no_2++;
    case 1:
        if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
        } else {
            em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.19634955f);
        }
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        end = MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            if (end) {
                EmRoutineSet(em, 2, 9, 0, 0);
            }
            break;
        }
        if ((s16) pG->pl_life <= 0) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (pSUB && (s16) pG->ashley_life <= 0) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (em10AtkRtnCk(em, 0)) {
            break;
        }
        if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
            if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 2.7488937f && em->l_pl > 36000000.0f) {
                EmRoutineSet(em, 1, 0x15, 0, 0);
                break;
            }
        } else if (w->Go_rot > 1.9634955f) {
            EmRoutineSet(em, 1, 0x15, 0, 0);
            break;
        }
        if (w->Esc_timer) {
            break;
        }
        if (end || w->HoseiCnt > 60) {
            if (--w->Timer <= 0) {
                em->r_no_0 = 1;
                em->r_no_1 = 0x10;
                em->r_no_2 = 0;
                em->r_no_3 = (u8) (MOTION(em)->Seq_frame * 255.0f / (f32) MOTION(em)->Seq_frame_num);
                w->Route_type = Rnd() % 11;
                break;
            }
        }
        lim = 2500.0f;
        if (pG->Game_level <= 3) {
            lim = 5000.0f;
        }
        if (em->type == 0xA || em->type == 0xD) {
            lim = 0.0f;
        }
        if (em->type == 0x16 && w->L_pl_route < 5000.0f) {
            EmRoutineSet(em, 1, 0x6D, 0, 0);
            break;
        }
        if (w->L_pl_route < lim && w->Timer) {
            em->r_no_0 = 1;
            em->r_no_1 = 0x10;
            em->r_no_2 = 0;
            em->r_no_3 = (u8) (MOTION(em)->Seq_frame * 255.0f / (f32) MOTION(em)->Seq_frame_num);
            w->Route_type = Rnd() % 11;
            break;
        }
        em10FindCk(em, 2);
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    em10CsawSignSe(em);
    em10BehindSeCk(em);
    if (em->type == 0xA || em->type == 0xD) {
        DmgMgr.set(DMG_TYPE_PUSH, 2, &em->pos, 1500.0f, 1000.0f);
    }
}
#endif // PARKED

// R1 == 0x12 Back: steps backwards (weapon-specific back motions, EM10_BACK_MOT) facing Go_pos, then
// attacks (em10AtkRtnCk mode 1) or goes to Stay; a killed Ganado goes to Dm_KnockOut.
// FUNCTION: BIO4 0x00464730
// STATUS: stub
#if 0 // PARKED: EM10_BACK_MOT, LIMIT_ANGLE, Muku, ashley_life, l_pl, pl_life
void em10_R1_Back(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        if (w->pShield) {
            EM10_BACK_MOT(0x16A, 0x16B);
        } else {
            switch (w->Wep_type) {
            case 0:
                EM10_BACK_MOT(0x15, 0x14);
                break;
            default:
                EM10_BACK_MOT(0xE4, 0xE5);
                break;
            case 1:
                EM10_BACK_MOT(0x157, 0x158);
                break;
            case 6:
                EM10_BACK_MOT(0x142, 0x143);
                break;
            }
        }
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        end = MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            if (end) {
                EmRoutineSet(em, 2, 9, 0, 0);
            }
            break;
        }
        if ((s16) pG->pl_life <= 0 || (pSUB && (s16) pG->ashley_life <= 0)) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (em10AtkRtnCk(em, 1)) {
            break;
        }
        if (em->r_no_3) {
            if (end) {
                EmRoutineSet(em, 1, 0x1B, 0, 0);
            }
        } else if (end || em->l_pl > 16000000.0f) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    em10CsawSignSe(em);
    em10BehindSeCk(em);
}
#endif // PARKED

// Branch check of Goto (0x13): obstacle actions, return check, and Turn180 (r_no_3 2) when the goto
// point is more than 90 deg behind (not for goto mode 8).
// FUNCTION: BIO4 0x0045E560
// STATUS: stub
void em10_R1_br_Goto(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0 && !em10DoorOpenCk(em, 1) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em)) {
        em10ReturnStartPosCk(em);
        if (!em10ClimbOverCk(em) && !em10WindowCk(em) && em->r_no_2 == 0 && w->Goto_mode != 8 && w->Go_rot > 1.9198622f) {
            EmRoutineSet(em, 1, 0x15, 0, 2);
        }
    }
}

// R1 == 0x13 Goto: walks / dashes to Route_target (Goto_mode picks the motion and the arrival action:
// idle, Find, open / close the switch pSwitch (motion 0x9C), ladder reset, the die-at-target modes,
// SitDown, the point motion); EM10_GOTO_ARRIVE_RTN picks the room-specific routine by cEm::set.
// FUNCTION: BIO4 0x0047B030
// STATUS: stub
#if 0 // PARKED: C2059, C2227, EM10_GOTO_ARRIVE_RTN, EM10_GOTO_ARRIVE_WAIT, LIMIT_ANGLE, Muku, cEmSwitch, l_pl
void em10_R1_Goto(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;
    f32 d2;
    f32 dy;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    w->Be_flg |= 0x40;
    if (w->Wep_type == 4 || w->pShield) {
        w->Be_flg &= ~0x40;
    }
    if (em->r_no_2 == 0 && w->Goto_mode == 8) {
        em->r_no_2 = 6;
    }
    switch (em->r_no_2) {
    case 0:
        switch (w->Goto_mode) {
        default:
            em->r_no_3 = 0;
            em10SetDashMotion(em);
            break;
        case 6:
        case 9:
        case 0xA:
            em10SetWalkMotion(em, 7);
            break;
        }
        em->r_no_2++;
    case 1:
        if (w->Goto_mode == 6 || w->Goto_mode == 0xA) {
            em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
        } else {
            em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.2617994f);
        }
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            EmRoutineSet(em, 2, 9, 0, 0);
            break;
        }
        d2 = (em->pos.x - w->Goto_pos.x) * (em->pos.x - w->Goto_pos.x) + (em->pos.y - w->Goto_pos.y) * (em->pos.y - w->Goto_pos.y) + (em->pos.z - w->Goto_pos.z) * (em->pos.z - w->Goto_pos.z);
        if (d2 < 250000.0f) {
            switch (w->Goto_mode) {
            case 0:
                break;
            case 1:
            case 0xA:
            case 0xB:
            case 0xD:
                w->Goto_mode = 0;
                w->Keep_pos = em->pos;
                EM10_GOTO_ARRIVE_RTN(EM10_GOTO_ARRIVE_WAIT);
                break;
            case 2:
                w->Goto_mode = 0;
                w->Keep_pos = em->pos;
                EmRoutineSet(em, 1, 0xD, 0, 0);
                break;
            case 3:
            case 4:
                if (w->pSwitch) {
                    em->r_no_2++;
                } else {
                    w->Goto_mode = 0;
                    EM10_GOTO_ARRIVE_RTN(EM10_GOTO_ARRIVE_WAIT);
                }
                break;
            case 5:
                em->r_no_2 = 4;
                break;
            case 9:
                w->Goto_mode = 0;
                if (em10LadderResetCk(em)) {
                    return;
                }
                em10WalkRtnSet(em);
                break;
            case 6:
            case 7:
                em->r_no_2 = 8;
                break;
            case 0xC:
                em->setFindPL();
                w->Goto_mode = 0;
                em10WalkRtnSet(em);
                break;
            case 0xE:
                w->Goto_mode = 0;
                EmRoutineSet(em, 1, 0x1A, 0, 1);
                break;
            }
        } else {
            if (w->Go_rot > 1.9198622f) {
                EmRoutineSet(em, 1, 0x15, 0, 0);
                break;
            }
            switch (w->Goto_mode) {
            case 6:
            case 7:
            case 0xA:
            case 0xB:
                em10FindCk(em, 2);
                if (w->Be_flg & 0x100) {
                    w->Goto_mode = 0;
                    w->Keep_pos = em->pos;
                    w->Route_type = Rnd() % 11;
                    em10FindNotify(em);
                    EM10_GOTO_ARRIVE_RTN(EmRoutineSet(em, 1, 0x10, 0, 0));
                }
                break;
            case 0xC:
            case 0xD:
                dy = pPL->pos.y - em->pos.y;
                dy = fabsf(dy);
                if ((w->Be_flg & 1) && dy < 1500.0f && em->l_pl < 16000000.0f) {
                    em->setFindPL();
                    w->Goto_mode = 0;
                    em10WalkRtnSet(em);
                    break;
                }
                if (pSUB && (w->Be_flg & 0x8000000)) {
                    dy = pSUB->pos.y - em->pos.y;
                    dy = fabsf(dy);
                    if ((w->Be_flg & 2) && dy < 1500.0f && w->L_sub < 16000000.0f) {
                        em->setFindPL();
                        w->Goto_mode = 0;
                        em10WalkRtnSet(em);
                    }
                }
                break;
            case 0xE: // default-grouped case after [C-D]: shapes the compare tree (`ble` into the arm, `b` to default)
                break;
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9C), PL_ARC_PTR(em->subArc, 0x9D), 5, 1, 0);
        em->r_no_2++;
    case 3:
        em->ang.y += Muku(&em->pos, &w->pSwitch->pos, em->ang.y, 0.19634955f);
        if (MotionMove(em, 0)) {
            EM10_GOTO_ARRIVE_RTN(EM10_GOTO_ARRIVE_WAIT);
            w->pSwitch = 0;
        } else if (em->Motion.Seq_old.Free & 1) {
            switch (w->Goto_mode) {
            case 3:
                ((cEmSwitch*) w->pSwitch)->setOpen();
                break;
            case 4:
                ((cEmSwitch*) w->pSwitch)->setClose();
                break;
            }
            w->Goto_mode = 0;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x23), PL_ARC_PTR(em->subArc, 0x24), 3, 1, 0);
        em->ang.y = GetXZAngle(&em->pos, &w->Route_target);
        w->Goto_mode = 0;
        em->hp = 0;
        em->atari.m_flag &= 0xFCFF;
        em->r_no_2++;
    case 5:
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 3, 3, 0, 0);
        }
        break;
    case 6: {
        void* m0 = PL_ARC_PTR(em->subArc, 0x73);
        void* m1 = PL_ARC_PTR(em->subArc, 0x74);
        int flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    }
    case 7:
        w->TmpV = w->Route_target;
        em->ang.y += Muku(&em->pos, &w->TmpV, em->ang.y, 0.19634955f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (em->Motion.Seq_old.Free & 0x20) {
            em10CallVoiceSe2(em, w->Se_tbl[4], 8);
        }
        if (em->Motion.Seq_old.Free & 8) {
            w->Be_flg |= 0x2000000;
        }
        if (MotionMove(em, 0)) {
            w->Goto_mode = 0;
            em10WalkRtnSet(em);
        }
        break;
    case 8:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2AA), PL_ARC_PTR(em->subArc, 0x2AB), 10, 1, 0);
        em->r_no_2++;
    case 9:
        end = MotionMove(em, 0);
        if (end) {
            w->Goto_mode = 0;
            w->Keep_pos = em->pos;
            EmRoutineSet(em, 1, 0, 0, 0);
            break;
        }
        em10FindCk(em, 2);
        if (w->Be_flg & 0x100) {
            w->Goto_mode = 0;
            w->Keep_pos = em->pos;
            w->Route_type = Rnd() % 11;
            EM10_GOTO_ARRIVE_RTN(EmRoutineSet(em, 1, 0x10, 0, 0));
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x14 GuardWalk: walks with the arms up guarding the head (motion 6/7, hit box off) towards the
// player / Go_pos for 120..240 frames, then back to Walk; a killed Ganado goes to Dm_KnockOut.
// FUNCTION: BIO4 0x004649E0
// STATUS: stub
#if 0 // PARKED: C2105, C2228, LIMIT_ANGLE, Muku, ashley_life, hitInfo, int, pl_life
void em10_R1_GuardWalk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    PlArc* arc;
    int end;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    em->hitInfo.flag &= ~1;
    switch (em->r_no_2) {
    case 0:
        arc = em->subArc;
        if (em->r_no_3) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(arc, 6), PL_ARC_PTR(arc, 7), 30, 5, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(arc, 6), PL_ARC_PTR(arc, 7), 30, 0x45, 0);
        }
        em->r_no_3 = Rnd() & 1;
        w->Timer = Rnd() % 120 + 120;
        em->r_no_2++;
    case 1:
        if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        } else {
            em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
        }
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        end = MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            if (end) {
                EmRoutineSet(em, 2, 9, 0, 0);
            }
        } else if ((s16) pG->pl_life <= 0 || (pSUB && (s16) pG->ashley_life <= 0)) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
        } else if (!em10AtkRtnCk(em, 0)) {
            if (w->Timer) {
                w->Timer--;
            } else {
                u8 f = (u8) (MOTION(em)->Seq_frame * 255.0f / (f32) MOTION(em)->Seq_frame_num);
                w->Route_type = Rnd() % 11;
                em->r_no_0 = 1;
                em->r_no_1 = 0x10;
                em->r_no_2 = 0;
                em->r_no_3 = f;
            }
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x15 Turn180: turns around on the spot (motion 0x18/0x19, 0x16/0x17 in the other direction,
// shield / claw variants) towards the player or Go_pos, then Find (r_no_3 0: notify), Goto (r_no_3 2),
// Dash / Walk, or the attack / stay / dash checks.
// FUNCTION: BIO4 0x00464F80
// STATUS: stub
#if 0 // PARKED: C2105, LIMIT_ANGLE, Muku, PI, ashley_life, int, pl_life
void em10_R1_Turn180(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;
    f32 a;

    switch (em->r_no_2) {
    case 0:
        if (em->r_no_3 != 1) {
            m0 = PL_ARC_PTR(em->subArc, 0x18);
            m1 = PL_ARC_PTR(em->subArc, 0x19);
        } else {
            m0 = PL_ARC_PTR(em->subArc, 0x16);
            m1 = PL_ARC_PTR(em->subArc, 0x17);
        }
        flag = 1;
        if (w->Go_dir < 0.0f) {
            flag = 0x41;
        }
        if (w->pShield) {
            m0 = PL_ARC_PTR(em->subArc, 0x16C);
            m1 = PL_ARC_PTR(em->subArc, 0x16D);
            flag = 1;
            if (em->flag & 0x1000000) {
                flag = 0x41;
            }
        }
        if (em->type == 0xA || em->type == 0xD) {
            m0 = PL_ARC_PTR(em->subArc, 0x116);
            m1 = PL_ARC_PTR(em->subArc, 0x117);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        w->TmpF = em->ang.y + PI;
        w->TmpF = LIMIT_ANGLE(w->TmpF);
        w->Timer = 60;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 8) {
            if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
                a = Muku(&em->pos, &pPL->pos, w->TmpF, 0.09817477f);
            } else {
                a = Muku(&em->pos, &w->Go_pos, w->TmpF, 0.09817477f);
            }
            w->TmpF += a;
            w->TmpF = LIMIT_ANGLE(w->TmpF);
            em->ang.y += a;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            } else {
                em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
            }
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if ((em->Motion.Seq_old.Free & 1) && (w->Be_flg & 0x100)) {
            w->Be_flg |= 0x40000;
        }
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                em10FindNotify(em);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->r_no_3 == 1) {
                EmRoutineSet(em, 1, 0, 0, 0);
                break;
            }
            if (em->r_no_3 == 2 && w->Goto_mode) {
                EmRoutineSet(em, 1, 0x13, 0, 0);
                break;
            }
            if (em->flag & 0x80) {
                EmRoutineSet(em, 1, 0x11, 0, 0);
                break;
            }
            if (em10AtkRtnCk(em, 0)) {
                break;
            }
            if (em10StayCk(em)) {
                break;
            }
            if (em10DashCk(em)) {
                break;
            }
            if (em->type == 0x16) {
                EmRoutineSet(em, 1, 0x6D, 0, 0);
                return;
            }
            w->Route_type = Rnd() % 11;
            EmRoutineSet(em, 1, 0x10, 0, 0);
        } else {
            if (em->r_no_3 != 2 && em10GotoCk(em)) {
                break;
            }
            if ((s16) pG->pl_life <= 0 || (pSUB && (s16) pG->ashley_life <= 0)) {
                EmRoutineSet(em, 1, 0x1B, 0, 0);
                break;
            }
            if (em->type != 0xA && em->type != 0xD) {
                em10AtkRtnCk(em, 0);
            }
        }
        break;
    }
    em10CsawSignSe(em);
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x16 Threat: the threatening shout (motion 0x71, mirrored when the player faces away) while
// turning to the player, then walks or attacks.
// FUNCTION: BIO4 0x00465990
// STATUS: stub
#if 0 // PARKED: C2105, LIMIT_ANGLE, Muku, PI, int
void em10_R1_Threat(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        if (Muku(&pPL->pos, &em->pos, pPL->ang.y, PI) > 0.0f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x71), PL_ARC_PTR(em->subArc, 0x72), 30, 1, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x71), PL_ARC_PTR(em->subArc, 0x72), 30, 0x41, 0);
        }
        w->Timer = 10;
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else {
            em10AtkRtnCk(em, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x17 SideStep: dodges sideways (r_no_3: 0/1 step 0x2AC left / right, 2/3 the 0x98 variant),
// then attacks, hides again (em10HideRtnCk2) or walks.
// FUNCTION: BIO4 0x00465D70
// STATUS: stub
void em10_R1_SideStep(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        switch (em->r_no_3) {
        case 0:
        default:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2AC), PL_ARC_PTR(em->subArc, 0x2AD), 5, 1, 0);
            break;
        case 1:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2AC), PL_ARC_PTR(em->subArc, 0x2AD), 5, 0x41, 0);
            break;
        case 2:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x98), PL_ARC_PTR(em->subArc, 0x99), 5, 1, 0);
            break;
        case 3:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x98), PL_ARC_PTR(em->subArc, 0x99), 5, 0x41, 0);
            break;
        }
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            w->Atk_no_wait = 1;
            if (!em10AtkRtnCk(em, 0) && !em10HideRtnCk2(em)) {
                em10WalkRtnSet(em);
            }
        } else if (em->Motion.Seq_old.Free & 1) {
            w->Atk_no_wait = 1;
            em10AtkRtnCk(em, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}

// R1 == 0x18 HideSide: waits out of sight for 90..240 frames, then steps out (em10HideToStepCk ->
// SideStep 0x17) with the find voice, or walks when the player is already close.
// FUNCTION: BIO4 0x00466290
// STATUS: stub
#if 0 // PARKED: C2105, int
void em10_R1_HideSide(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Timer = Rnd() % 150 + 90;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->flag &= ~1;
            em->setFindPL();
            em->r_no_0 = 1;
            em->r_no_1 = 0x17;
            em->r_no_2 = 0;
        } else {
            if (w->Timer == 0) {
                if (em10HideToStepCk(em, em->r_no_3)) {
                    if (!(w->Be_flg & 0x100)) {
                        em->setFindPL();
                        em10CallVoiceSe2(em, w->Se_tbl[4], 8);
                        if (w->Timer) {
                            w->Timer--;
                            if (w->Timer == 0) {
                                em10FindNotify(em);
                            }
                        }
                    }
                    break;
                }
            } else {
                w->Timer--;
            }
            if (w->L_pl_route < 4000.0f) {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x19 AppearSide: like HideSide without the step check: after the wait marks the player found
// and side-steps (0x17) or walks.
// FUNCTION: BIO4 0x004664F0
// STATUS: stub
#if 0 // PARKED: int
void em10_R1_AppearSide(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Timer = Rnd() % 150 + 90;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->setFindPL();
            em->r_no_0 = 1;
            em->r_no_1 = 0x17;
            em->r_no_2 = 0;
        } else if (w->L_pl_guard < em->Guard_r) {
            em->setFindPL();
            em10WalkRtnSet(em);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x1A SitDown: sits down (motion 0x9A), a bowgun Ganado re-arms its arrow, waits 120..210
// frames turning to the player, stands up (0x9B) and walks / attacks; a killed one goes to Dm_KnockOut.
// FUNCTION: BIO4 0x00466670
// STATUS: stub
#if 0 // PARKED: C2105, EstSet, LIMIT_ANGLE, Muku, int
void em10_R1_SitDown(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9A), 0, 5, 1, 0);
        em->flag &= ~1;
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em->r_no_2++;
            if (w->Wep_type == 8) {
                if (w->Arrow_num == 0 && w->pWeapon) {
                    EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x1F, 0, w->EffKindIdArrow, w->pWeapon, 0);
                }
                w->Arrow_num = 2;
            }
        }
        break;
    case 2:
        w->Timer = (u8) (Rnd() % 90) + 120;
        em->r_no_2++;
    case 3:
        MotionMove(em, 0);
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (em->r_no_3) {
            if (w->L_pl_route < 3000.0f) {
                em->setFindPL();
                em->r_no_2++;
            } else if (em->flag & 1) {
                em->setFindPL();
                em->r_no_2++;
            } else if (w->Goto_mode) {
                em->setFindPL();
                em->r_no_2++;
            }
        } else if (w->L_pl_route < 3000.0f) {
            em->setFindPL();
            em->r_no_2++;
        } else if (w->Timer) {
            w->Timer--;
        } else {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9B), 0, 5, 1, 0);
        em->r_no_2++;
    case 5:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0)) {
            w->Atk_no_wait = 1;
            if (em10AtkRtnCk(em, 0)) {
                if (em->r_no_1 == 0x21) {
                    em->r_no_2 = 2;
                }
            } else {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x1B Stay: stands facing the player (slow turn) when it must not approach (ctrl12 NOT_NEAR /
// ATK locks, other Ganados attacking): every 35 frames re-checks return (em10ReturnCk), attack, stay
// and walk; turns around first (step 2/3) when the player is more than 135 deg behind.
// FUNCTION: BIO4 0x00466C20
// STATUS: stub
#if 0 // PARKED: C2227, CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_NOT_NEAR, Ctrl12Ck, DBG_EM_NO_ATK, DbgFlagChk, LIMIT_ANGLE, Muku, PI, Timer2, ashley_life, int, l_pl, pl_life
void em10_R1_Stay(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;
    int one;
    f32 a;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    if (em->r_no_2 == 0 && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 2.3561945f) {
        em->r_no_2 = 2;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        if (w->pCore || w->pParasite) {
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
        }
        w->Timer2 = 0x23;
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.024543693f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if ((s16) pG->pl_life <= 0) {
            break;
        }
        if (pSUB && (s16) pG->ashley_life <= 0) {
            break;
        }
        one = 1;  // shared SImode constant: the routine kind below and the xFE reset of the wait re-entry
        if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
            EmRoutineSet(em, one, 0, 0, 0);
            return;
        }
        if (em10GotoCk(em)) {
            break;
        }
        if (w->Timer2 == 0 || --w->Timer2 == 0) {
            if (!Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR) && !Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_ATK) && w->Atk_wait == 0) {
                if (em10ReturnCk(em)) {
                    break;
                }
                if (em10AtkRtnCk(em, 0)) {
                    break;
                }
                if (em->Character == 0 || em->Character == 2) {
                    if (em10StayCk(em)) {
                        if (em->r_no_0 == 1 && em->r_no_1 == 0x1B) {
                            em->r_no_2 = one;
                            w->Timer2 = 0x23;
                        }
                        break;
                    }
                    em10WalkRtnSet(em);
                    break;
                }
                if (w->L_pl_guard < em->Guard_r || (em->flag & 0x40)) {
                    em10WalkRtnSet(em);
                    break;
                }
            }
        }
        if ((w->Be_flg & 0x20000000) && (!(w->Be_flg & 1) || em->l_pl > 225000000.0f || w->L_guard > em->Guard_r + 10000.0f)) {
            w->Route_type = Rnd() % 3;
            EmRoutineSet(em, 1, 0x11, 0, 0);
        } else {
            em10GotoPosCk(em);
        }
        break;
    case 2:
        m0 = PL_ARC_PTR(em->subArc, 0x18);
        m1 = PL_ARC_PTR(em->subArc, 0x19);
        flag = 1;
        if (w->Go_dir < 0.0f) {
            flag = 0x41;
        }
        if (w->pShield) {
            m0 = PL_ARC_PTR(em->subArc, 0x16C);
            m1 = PL_ARC_PTR(em->subArc, 0x16D);
            flag = 1;
            if (em->flag & 0x1000000) {
                flag = 0x41;
            }
        }
        if (em->type == 0xA || em->type == 0xD) {
            m0 = PL_ARC_PTR(em->subArc, 0x116);
            m1 = PL_ARC_PTR(em->subArc, 0x117);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        w->TmpF = em->ang.y + PI;
        w->Timer = 60;
        em->r_no_2++;
    case 3:
        if (em->Motion.Seq_old.Free & 8) {
            a = Muku(&em->pos, &pPL->pos, w->TmpF, 0.09817477f);
            w->TmpF += a;
            w->TmpF = LIMIT_ANGLE(w->TmpF);
            em->ang.y += a;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        } else {
            em10AtkRtnCk(em, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    } else {
        em10CsawSignSe(em);
    }
}
#endif // PARKED

// R1 == 0x1C RoofWait: stuck above the player (br_Walk): idles turning to him, walks again once he is
// more than 45 deg off, attacks when possible.
// FUNCTION: BIO4 0x00467640
// STATUS: stub
#if 0 // PARKED: C1003, C2105, LIMIT_ANGLE, Muku, PI, Timer2, ashley_life, int, pl_life
void em10_R1_RoofWait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Timer = 10;
        w->Timer2 = 0x23;
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        MotionMove(em, 0);
        if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 0.7853982f) {
            if ((s16) pG->pl_life > 0 && (!pSUB || (s16) pG->ashley_life > 0)) {
                em10WalkRtnSet(em);
            }
        } else {
            em10AtkRtnCk(em, 0);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x1D Guard: covers the head against the aimed weapon (motion 0x78 / 0x2A8 variants picked by
// the free side, em10HeadLockCk keeps it up), then the threat shout (0x71) and back to the walk.
// FUNCTION: BIO4 0x00467A50
// STATUS: stub
#if 0 // PARKED: C2105, C2228, Muku, PI, PSMTXMultVec, SatMgr, Timer2, int, mat
void em10_R1_Guard(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    u32 mode;

    if (em->r_no_2 == 0 && (Rnd() & 1)) {
        em->r_no_2 = 2;
    }
    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        mode = Rnd() % 2;
        if ((Rnd() & 1) == 0) {
            a.x = 0.0f;
            a.y = 250.0f;
            a.z = -1000.0f;
            PSMTXMultVec(em->mat, &a, &a);
            PSMTXMultVec(em->mat, &b, &b);
            if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0) == 0) {
                mode = 2;
            } else {
                a.x = 0.0f;
                a.y = 250.0f;
                a.z = 1000.0f;
                PSMTXMultVec(em->mat, &b, &b);
                if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0) == 0) {
                    mode = 3;
                }
            }
        }
        switch (mode) {
        case 0:
        default:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x78), 0, 30, 1, 0);
            break;
        case 1:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x78), 0, 30, 0x41, 0);
            break;
        case 2:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2A8), PL_ARC_PTR(em->subArc, 0x2A9), 30, 1, 0);
            break;
        case 3:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2A8), PL_ARC_PTR(em->subArc, 0x2A9), 30, 0x41, 0);
            break;
        }
        w->Timer = 15;
        w->Timer2 = 0;
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
            break;
        }
        if (w->Timer) {
            w->Timer--;
            break;
        }
        if (em10HeadLockCk(em)) {
            if (++w->Timer2 > 10) {
                em->r_no_2 = 2;
            }
        } else {
            w->Timer2 = 0;
        }
        break;
    case 2:
        if (Muku(&pPL->pos, &em->pos, pPL->ang.y, PI) > 0.0f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x71), PL_ARC_PTR(em->subArc, 0x72), 30, 1, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x71), PL_ARC_PTR(em->subArc, 0x72), 30, 0x41, 0);
        }
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x1E DownWakeWait: lies on the floor (work flags 0x10 | 0x1000000, IK off) for WakeTimer
// 60..120 frames, a parasite core may attack from there, then DownWake (0x1F).
// FUNCTION: BIO4 0x0043E0E0
// STATUS: stub
#if 0 // PARKED: EM_STATUS_IK_OFF, ckAtkEnable, l_pl, setAtk
void em10_R1_DownWakeWait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        w->WakeTimer = Rnd() % 60 + 60;
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        MotionMove(em, 0);
        if (em->l_pl > 6250000.0f && w->pCore && w->Ganado != 1 && w->pCore->ckAtkEnable()) {
            w->pCore->setAtk(1);
        }
        if ((s16) w->WakeTimer != 0) {
            w->WakeTimer--;
        } else {
            EmRoutineSet(em, 1, 0x1F, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x1F DownWake: gets up (motion 0x6D / 0xAA / 0x6F by weapon, voice Se_tbl[11]); the down
// flags stay set for the first frames, then the walk routine.
// FUNCTION: BIO4 0x00467EB0
// STATUS: stub
#if 0 // PARKED: EM_STATUS_IK_OFF, ckAtkEnable, l_pl, setAtk
void em10_R1_DownWake(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        flag = (MOTION(em)->Mot_attr & 0x40) ? 0x41 : 1;
        if (w->Be_flg & 0x20) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x6D), PL_ARC_PTR(em->subArc, 0x6E), 15, flag, 0);
        } else if (em->type == 6) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xAA), PL_ARC_PTR(em->subArc, 0xAD), 15, flag, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x6F), PL_ARC_PTR(em->subArc, 0x70), 15, flag, 0);
        }
        if (!(w->Be_flg & 0x80)) {
            em10CallVoiceSe2(em, w->Se_tbl[11], 8);
        }
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x18;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        } else {
            w->Be_flg &= ~0x10;
            w->Be_flg &= ~0x1000000;
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else {
            if (em->l_pl > 6250000.0f && w->pCore && w->Ganado != 1 && w->pCore->ckAtkEnable()) {
                w->pCore->setAtk(1);
            }
            if (em->Motion.Seq_old.Free & 2) {
                em10SetDmWaterEff(em, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x3B Crash: knocked over by another Ganado falling into it (em10CrashCk: the kind 3 damage
// volume em10SetCrash registers): the stumble motion 0x1B / 0x1D (random start frame), then walks on;
// pushes others in turn (flag 0x2000, em10SetCrash 500).
// FUNCTION: BIO4 0x00468070
// STATUS: stub
void em10_R1_Crash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;

    switch (em->r_no_2) {
    case 0:
        flag = (Rnd() & 1) ? 0x41 : 1;
        if (em->r_no_3) {
            m0 = PL_ARC_PTR(em->subArc, 0x1D);
            m1 = PL_ARC_PTR(em->subArc, 0x1E);
        } else {
            m0 = PL_ARC_PTR(em->subArc, 0x1B);
            m1 = PL_ARC_PTR(em->subArc, 0x1C);
        }
        {
            // Rnd() before the call statement: the `addi r4, em, 0x1d8` is then computed after the
            // call and regmove ties it to r4 (issued after `mr r3`); m0 declared before m1 for r30/r29.
            int r = Rnd() % 5;
            MotionSetCore(em, MOTION(em), m0, m1, 6, flag, r);
        }
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x2000;
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    w->Be_flg |= 0x2000;
    em10SetCrash(em, 500.0f);
}

// R1 == 0x3C ClimbOver: climbs over the low obstacle found by em10ClimbOverCk (motion 0x1F), sliding
// the remaining Target_pos offset in over the motion (flag 0x20000 = on the fence), then walks.
// FUNCTION: BIO4 0x004681C0
// STATUS: stub
#if 0 // PARKED: EM_STATUS_IK_OFF, Mtx, PSMTXInverse, PSMTXMultVecSR, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, m
void em10_R1_ClimbOver(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Vec v;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1F), PL_ARC_PTR(em->subArc, 0x20), 10, 1, 0);
        PSVECSubtract(&w->Target_pos, &em->pos, &w->Target_pos);
        PSMTXRotRad(m, 'y', em->ang.y);
        PSMTXInverse(m, m);
        PSMTXMultVecSR(m, &w->Target_pos, &w->Target_pos);
        w->Target_pos.y = 0.0f;
        em->r_no_2++;
    case 1:
        PSVECScale(&w->Target_pos, &v, 0.2f);
        PSVECSubtract(&w->Target_pos, &v, &w->Target_pos);
        PSMTXRotRad(m, 'y', em->ang.y);
        PSMTXMultVecSR(m, &v, &v);
        PSVECAdd(&em->pos, &v, &em->pos);
        if (em->Motion.Seq_old.Free & 4) {
            w->Be_flg |= 0x20000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
            w->R11c_in_ck = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x3D DoorAtk: bashes the door in front (motion 0x79, chainsaw 0xE9 + SE 0x50): the hit frame
// damages the door (em10SetDamageDoor kind 1, 2 for the chainsaw) or the rack; a second swing 0x7B
// when the door still stands, then walks with Atk_wait 15.
// FUNCTION: BIO4 0x00468380
// STATUS: stub
#if 0 // PARKED: SndCall
void em10_R1_DoorAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        m0 = PL_ARC_PTR(em->subArc, 0x79);
        m1 = PL_ARC_PTR(em->subArc, 0x7A);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        if (w->pWeapon && w->Wep_type == 4) {
            m0 = PL_ARC_PTR(em->subArc, 0xE9);
            m1 = PL_ARC_PTR(em->subArc, 0xEA);
            SndCall(6, 0x50, &em->pos, 0, 0, em);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        } else if ((em->Motion.Seq_old.Free & 0x20) || (em->Motion.Seq_old.Free & 1)) {
            if (em->Motion.Seq_old.Free & 1) {
                if (w->Wep_type == 4) {
                    em10SetDamageDoor(em, 2);
                    em10SetDamageRack(em, 2);
                    break;
                }
                if (em10SetDamageDoor(em, 1) != 1) {
                    em10SetDamageRack(em, 2);
                    break;
                }
                em10SetDamageRack(em, 0);
            } else {
                em10SetDamageDoor(em, 0);
                em10SetDamageRack(em, 0);
            }
        } else if (em->Motion.Seq_old.Free & 4) {
            int ok = em10AtkDoorCk(em) == 0;
            if (em10AtkRackCk(em)) {
                ok = 0;
            }
            if (ok) {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        m0 = PL_ARC_PTR(em->subArc, 0x79);
        m1 = PL_ARC_PTR(em->subArc, 0x7B);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x3E RackAtk: bashes the rack / crate barricade (motion 0x7C, chainsaw 0xE9): the hit frame
// calls em10SetDamageRack / em10SetDamageDoor kind 2, second swing 0x7B, then walks.
// FUNCTION: BIO4 0x004686E0
// STATUS: stub
#if 0 // PARKED: SndCall
void em10_R1_RackAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        m0 = PL_ARC_PTR(em->subArc, 0x7C);
        m1 = PL_ARC_PTR(em->subArc, 0x7D);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        if (w->pWeapon && w->Wep_type == 4) {
            m0 = PL_ARC_PTR(em->subArc, 0xE9);
            m1 = PL_ARC_PTR(em->subArc, 0xEA);
            SndCall(6, 0x50, &em->pos, 0, 0, em);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_old.Free & 1) {
            em10SetDamageDoor(em, 2);
            em10SetDamageRack(em, 2);
        }
        break;
    case 2:
        m0 = PL_ARC_PTR(em->subArc, 0x79);
        m1 = PL_ARC_PTR(em->subArc, 0x7B);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x3F WindowAtk: smashes the window pWindow (motion 0x79, chainsaw 0xE9): the hit frame breaks
// it when its hp is 1 or the chainsaw does it, else takes one hp; then walks (Atk_wait 15).
// FUNCTION: BIO4 0x00468960
// STATUS: stub
#if 0 // PARKED: C2227, EM10_WINDOW, SndCall
void em10_R1_WindowAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;
    int ok;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        m0 = PL_ARC_PTR(em->subArc, 0x79);
        m1 = PL_ARC_PTR(em->subArc, 0x7A);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        if (w->pWeapon && w->Wep_type == 4) {
            m0 = PL_ARC_PTR(em->subArc, 0xE9);
            m1 = PL_ARC_PTR(em->subArc, 0xEA);
            SndCall(6, 0x50, &em->pos, 0, 0, em);
        }
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        } else if ((em->Motion.Seq_old.Free & 0x20) || (em->Motion.Seq_old.Free & 1)) {
            if (em->Motion.Seq_old.Free & 1) {
                ok = 0;
                if (EM10_WINDOW(w) && EM10_WINDOW(w)->hp > 0) {
                    if (EM10_WINDOW(w)->hp <= 1 || w->Wep_type == 4) {
                        EM10_WINDOW(w)->SetBreakAll(&em->pos, 0, 0);
                        w->pWindow = 0;
                        ok = 1;
                    } else {
                        EM10_WINDOW(w)->SetShake();
                    }
                }
                if (w->Wep_type == 4 || ok) {
                    em10SetDamageDoor(em, 2);
                    em10SetDamageRack(em, 2);
                } else {
                    em10SetDamageRack(em, 0);
                }
            } else {
                if (EM10_WINDOW(w) && EM10_WINDOW(w)->hp > 0) {
                    EM10_WINDOW(w)->SetShake();
                }
                em10SetDamageDoor(em, 0);
                em10SetDamageRack(em, 0);
            }
        } else if (em->Motion.Seq_old.Free & 4) {
            ok = 1;
            if (EM10_WINDOW(w) && EM10_WINDOW(w)->hp > 0) {
                ok = 0;
            }
            if (em10AtkDoorCk(em)) {
                ok = 0;
            }
            if (em10AtkRackCk(em)) {
                ok = 0;
            }
            if (ok) {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        m0 = PL_ARC_PTR(em->subArc, 0x79);
        m1 = PL_ARC_PTR(em->subArc, 0x7B);
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), m0, m1, 10, flag, 0);
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x40 LadderClimb: climbs the ladder pLadder found by em10LadderClimbCk: mount (motion 0xBC),
// rungs (0xBE, one per Timer = ladder rung count), dismount (0xC0 / 0xC2 by ladder type), with the
// climb SEs; work flag 0x10000 = on the ladder; the player kicking the ladder top (Status_flg[1]
// bit18 with his hip part near) throws the Ganado off into Dm_Ladder.
// FUNCTION: BIO4 0x00468D10
// STATUS: stub
#if 0 // PARKED: C1003, C2106, C2227, C2228, EM_STATUS_IK_OFF, LIMIT_ANGLE, MotionAddSpeed, MotionGetSpeed, Mtx, PI, PSMTXMultVec, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, STA_PL_LADDER, SatMgr, StaFlagChk, TransMatrix, mat
void em10_R1_LadderClimb(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec tmp;
    Vec spd;
    Vec rot;
    Mtx mat;
    Vec v;
    cModel* p;
    int flag;
    int st;
    f32 d2;
    f32 fl;

    switch (em->r_no_2) {
    case 0:
        PSMTXRotRad(mat, 'y', w->pLadder->ang.y);
        TransMatrix(mat, &w->pLadder->pos);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 300.0f;
        PSMTXMultVec(mat, &v, &v);
        PSVECSubtract(&v, &em->pos, &w->Target_pos);
        em->ang.y = w->pLadder->ang.y + PI;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        em->r_no_3 = Rnd() % 2;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xBC), PL_ARC_PTR(em->subArc, 0xBD), 10, em->r_no_3 ? 0 : 0x40, 0);
        w->Timer = w->pLadder->getLadderNum();
        w->TmpV = em->pos;
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x10000;
        em->setStatus(EM_STATUS_IK_OFF);
        em->pos.x = w->TmpV.x;
        em->pos.z = w->TmpV.z;
        PSVECScale(&w->Target_pos, &tmp, 0.3f);
        PSVECAdd(&em->pos, &tmp, &em->pos);
        PSVECSubtract(&w->Target_pos, &tmp, &w->Target_pos);
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Timer -= 4;
            if (w->Timer <= 0) {
                em->r_no_2 = 4;
            } else {
                em->r_no_2++;
            }
        } else {
            w->TmpV = em->pos;
        }
        break;
    case 2:
        flag = 0x44;
        if (em->r_no_3) {
            flag = 4;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xBE), PL_ARC_PTR(em->subArc, 0xBF), 10, flag, 0);
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10000;
        em->setStatus(EM_STATUS_IK_OFF);
        em->pos.x = w->TmpV.x;
        em->pos.z = w->TmpV.z;
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Timer -= 2;
            if (w->Timer <= 0) {
                em->r_no_2 = 4;
            }
        } else {
            if (StaFlagChk(pG, STA_PL_LADDER)) {
                p = pPL->getPartsPtr(4);
                v = p->world;
                d2 = (em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.y - v.y) * (em->pos.y - v.y) + (em->pos.z - v.z) * (em->pos.z - v.z);
                if (d2 < 90000.0f) {
                    em->dmg.m_PosFrom = pPL->pos;
                    EmRoutineSet(em, 2, 6, 0, 0);
                    break;
                }
            }
            w->TmpV = em->pos;
        }
        break;
    case 4:
        flag = em->r_no_3 ? 0 : 0x40;
        if (w->pLadder->getType() == 1) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xC2), PL_ARC_PTR(em->subArc, 0xC3), 10, flag, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xC0), PL_ARC_PTR(em->subArc, 0xC1), 10, flag, 0);
        }
        em->r_no_2++;
    case 5:
        if (!(em->Motion.Seq_old.Free & 4)) {
            w->Be_flg |= 0x10000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 8) {
            fl = SatMgr.getFloor(&em->pos, 0, 600.0f, 100000.0f, 0);
            if (em->pos.y < fl) {
                em->pos.y = fl;
            }
        }
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        } else if (StaFlagChk(pG, STA_PL_LADDER) && !(em->Motion.Seq_old.Free & 4)) {
            p = pPL->getPartsPtr(4);
            v = p->world;
            d2 = (em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.y - v.y) * (em->pos.y - v.y) + (em->pos.z - v.z) * (em->pos.z - v.z);
            if (d2 < 90000.0f) {
                em->dmg.m_PosFrom = pPL->pos;
                EmRoutineSet(em, 2, 6, 0, 0);
            }
        }
        break;
    }
    st = w->pLadder->getStatus();
    if ((w->Be_flg & 0x10000) && st == 3) {
        em->dmg.m_PosFrom = pPL->pos;
        EmRoutineSet(em, 2, 6, 0, 0);
        return;
    }
    em10HandSet(em, 0);
    if (em->r_no_3) {
        if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 0x46, &em->pos, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 2) {
            SndCall(6, 0x45, &em->pos, 0, 0, em);
        }
    } else {
        if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 0x45, &em->pos, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 2) {
            SndCall(6, 0x46, &em->pos, 0, 0, em);
        }
    }
}
#endif // PARKED

// R1 == 0x41 VLadderClimb: the vertical-wall ladder variant of LadderClimb (motions 0xC4 / 0xC6 /
// 0xC8, SEs 0x62/0x63, turns to Target_dir while mounting); kicked off the same way into Dm_Ladder.
// FUNCTION: BIO4 0x00469600
// STATUS: stub
#if 0 // PARKED: C2106, EM_STATUS_IK_OFF, LIMIT_ANGLE, MotionAddSpeed, MotionGetSpeed, Mtx, Muku2, PI, PSMTXMultVec, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, STA_PL_LADDER, SndCall, StaFlagChk, TransMatrix, mat, u8
void em10_R1_VLadderClimb(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec tmp;
    Vec spd;
    Vec rot;
    Mtx mat;
    Vec v;
    cModel* p;
    int flag;
    f32 d2;

    switch (em->r_no_2) {
    case 0:
        PSMTXRotRad(mat, 'y', w->Target_dir + PI);
        TransMatrix(mat, &w->Target_pos);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 100.0f;
        PSMTXMultVec(mat, &v, &v);
        PSVECSubtract(&v, &em->pos, &w->Target_pos);
        w->Timer = em->r_no_3;
        em->r_no_3 = Rnd() % 2;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xC4), PL_ARC_PTR(em->subArc, 0xC5), 10, em->r_no_3 ? 0 : 0x40, 0);
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x10000;
        em->setStatus(EM_STATUS_IK_OFF);
        PSVECScale(&w->Target_pos, &tmp, 0.3f);
        PSVECAdd(&em->pos, &tmp, &em->pos);
        PSVECSubtract(&w->Target_pos, &tmp, &w->Target_pos);
        em->ang.y += Muku2(em->ang.y, w->Target_dir, 0.3926991f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Timer -= 2;
            if (w->Timer <= 0) {
                em->r_no_2 = 4;
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        flag = 0x44;
        if (em->r_no_3) {
            flag = 4;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xC6), PL_ARC_PTR(em->subArc, 0xC7), 10, flag, 0);
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10000;
        em->setStatus(EM_STATUS_IK_OFF);
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Timer -= 1;
            if (w->Timer <= 0) {
                em->r_no_2 = 4;
            }
        } else if (StaFlagChk(pG, STA_PL_LADDER)) {
            p = pPL->getPartsPtr(4);
            v = p->world;
            d2 = (em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.y - v.y) * (em->pos.y - v.y) + (em->pos.z - v.z) * (em->pos.z - v.z);
            if (d2 < 90000.0f) {
                em->dmg.m_PosFrom = pPL->pos;
                EmRoutineSet(em, 2, 6, 0, 0);
            }
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xC8), PL_ARC_PTR(em->subArc, 0xC9), 10, em->r_no_3 ? 0 : 0x40, 0);
        em->r_no_2++;
    case 5:
        if (!(em->Motion.Seq_old.Free & 4)) {
            w->Be_flg |= 0x10000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        } else if (StaFlagChk(pG, STA_PL_LADDER) && !(em->Motion.Seq_old.Free & 4)) {
            p = pPL->getPartsPtr(4);
            v = p->world;
            d2 = (em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.y - v.y) * (em->pos.y - v.y) + (em->pos.z - v.z) * (em->pos.z - v.z);
            if (d2 < 90000.0f) {
                em->dmg.m_PosFrom = pPL->pos;
                EmRoutineSet(em, 2, 6, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    if (em->r_no_3) {
        if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 0x63, &em->pos, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 2) {
            SndCall(6, 0x62, &em->pos, 0, 0, em);
        }
    } else {
        if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 0x62, &em->pos, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 2) {
            SndCall(6, 0x63, &em->pos, 0, 0, em);
        }
    }
}
#endif // PARKED

// R1 == 0x42 LadderReset: puts a knocked-down ladder back up (motion 0xBA facing it, cObjLadder
// setReset at frame 31), work flag 0x200000 while the ladder is held, then walks.
// FUNCTION: BIO4 0x00469D40
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, LIMIT_ANGLE, Mtx, Muku2, PI, PSMTXMultVec, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, Timer2, TransMatrix, int, m
void em10_R1_LadderReset(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    Mtx m;
    Vec ofs;
    int flag;

    switch (em->r_no_2) {
    case 0:
        PSMTXRotRad(m, 'y', w->pLadder->ang.y);
        TransMatrix(m, &w->pLadder->pos);
        if (Muku2(w->pLadder->ang.y, em->ang.y, PI) > 0.0f) {
            ofs.x = -615.27f;
            ofs.y = 0.0f;
            ofs.z = 1432.8f;
            flag = 1;
            w->Target_dir = w->pLadder->ang.y + 1.5707964f;
        } else {
            ofs.x = 615.27f;
            ofs.y = 0.0f;
            ofs.z = 1432.8f;
            flag = 0x41;
            w->Target_dir = w->pLadder->ang.y - 1.5707964f;
        }
        w->Target_dir = Muku2(em->ang.y, w->Target_dir, PI);
        PSMTXMultVec(m, &ofs, &ofs);
        PSVECSubtract(&ofs, &em->pos, &w->Target_pos);
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xBA), 0, 10, flag, 0);
        w->Timer = 0x1F;
        w->Timer2 = 0x71;
        em->r_no_2++;
    case 1:
        PSVECScale(&w->Target_pos, &v, 0.2f);
        PSVECAdd(&em->pos, &v, &em->pos);
        PSVECSubtract(&w->Target_pos, &v, &w->Target_pos);
        {
            f32 d = w->Target_dir * 0.2f;
            em->ang.y += d;
            w->Target_dir -= d;
        }
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                w->pLadder->setReset(1);
            }
        }
        if (w->Timer2) {
            w->Timer2--;
            if (w->Timer == 0) {
                w->Be_flg |= 0x200000;
            }
        } else if (w->pLadder) {
            w->pLadder = 0;
        }
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x43 JumpDown: drops off an edge / down to Keep_pos (motion 0x23 with the Target_pos run-off, 0x21
// when starting from a stand): falls with flags 0x10080000 (airborne, no adjust), water splash on
// the way (em10FallWaterCk), lands with the snap / SE 5 / motion 0x25 and a dust effect, then walks.
// FUNCTION: BIO4 0x0046A130
// STATUS: stub
#if 0 // PARKED: C2228, EM10_FALL_WATER_EFFECT, EM10_JUMP_DOWN_LAND, EM_STATUS_IK_OFF, PSVECAdd, PSVECScale, PSVECSubtract, SatMgr, pos_old
void em10_R1_JumpDown(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec tmp;
    int end;
    f32 fl;

    switch (em->r_no_2) {
    case 0:
        if (em->r_no_3) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x23), PL_ARC_PTR(em->subArc, 0x24), 3, 1, 0);
            PSVECSubtract(&w->Target_pos, &em->pos, &w->Target_pos);
            w->Target_pos.y = 0.0f;
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x21), PL_ARC_PTR(em->subArc, 0x22), 3, 1, 0);
            w->Target_pos.x = 0.0f;
            w->Target_pos.y = 0.0f;
            w->Target_pos.z = 0.0f;
        }
        w->TmpU32 = 0;
        em10CallVoiceSe2(em, w->Se_tbl[18], 8);
        em->r_no_2++;
    case 1:
        PSVECScale(&w->Target_pos, &tmp, 0.2f);
        PSVECAdd(&em->pos, &tmp, &em->pos);
        PSVECSubtract(&w->Target_pos, &tmp, &w->Target_pos);
        w->Be_flg |= 0x10080000;
        em->setStatus(EM_STATUS_IK_OFF);
        end = MotionMove(em, 0);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                EM10_FALL_WATER_EFFECT;
            }
        }
        if (em->Motion.Seq_old.Free & 0x40) {
            break;
        }
        tmp = em->pos;
        tmp.y = em->pos_old.y;
        fl = SatMgr.getFloor(&tmp, 0, 600.0f, 100000.0f, 0);
        if (em->pos.y < fl) {
            EM10_JUMP_DOWN_LAND;
        } else if (end) {
            em->r_no_2++;
        }
        break;
    case 2:
        w->Spd.x = 0.0f;
        w->Spd.y = -400.0f;
        w->Spd.z = 0.0f;
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10080000;
        em->setStatus(EM_STATUS_IK_OFF);
        PSVECAdd(&em->pos, &w->Spd, &em->pos);
        w->Spd.y -= 20.0f;
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                EM10_FALL_WATER_EFFECT;
            }
        }
        MotionMove(em, 0);
        tmp = em->pos;
        tmp.y = em->pos_old.y;
        fl = SatMgr.getFloor(&tmp, 0, 600.0f, 100000.0f, 0);
        if (em->pos.y < fl) {
            EM10_JUMP_DOWN_LAND;
        }
        break;
    case 4:
        if (CheckInWater(em, 0)) {
            em10FallWaterCk(em);
            if (w->TmpU32 == 0) {
                EM10_FALL_WATER_EFFECT;
            }
        } else if (ChkWaterEffectEnable(&em->pos)) {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, em, 0);
        } else {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        em->r_no_2++;
    case 5:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R1 == 0x44 Jump: jumps over a gap found by em10JumpCk (motion 0x8C towards the far floor) and
// falls / lands like JumpDown (flags 0x00181000 during the leap, then 0x10080000).
// FUNCTION: BIO4 0x0046A880
// STATUS: stub
#if 0 // PARKED: C1003, Mtx, PSMTXMultVec, PSMTXRotRad, TransMatrix, mat
void em10_R1_Jump(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx mat;
    Vec tmp;
    Vec v;
    Vec spd;
    Vec rot;
    f32 fl;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x8C), PL_ARC_PTR(em->subArc, 0x8D), 10, 0, 0);
        PSMTXRotRad(mat, 'y', em->ang.y);
        TransMatrix(mat, &em->pos);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 4000.0f;
        PSMTXMultVec(mat, &v, &v);
        fl = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        w->TmpF = fl - em->pos.y;
        if (fl > 1000.0f) {
            w->TmpF = 0.0f;
        }
        if (fl < 0.0f) {
            w->TmpF = 0.0f;
        }
        w->TmpU32 = 0;
        em10CallVoiceSe2(em, w->Se_tbl[18], 8);
        em->r_no_2++;
    case 1:
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        if (em->Motion.Seq_old.Free & 4) {
            w->Be_flg |= 0x00181000;
            fl = w->TmpF * 0.1f;
            em->pos.y += fl;
            w->TmpF -= fl;
            PSVECScale(&spd, &spd, 1.0f / em->scale.x);
        }
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        em->setStatus(EM_STATUS_IK_OFF);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                EM10_FALL_WATER_EFFECT;
            }
        }
        if (em->Motion.Seq_old.Free & 1) {
            tmp = em->pos;
            tmp.y = em->pos_old.y;
            fl = SatMgr.getFloor(&tmp, 0, 600.0f, 100000.0f, 0);
            if (em->pos.y < fl) {
                EM10_JUMP_DOWN_LAND;
                break;
            }
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        w->Spd.x = 0.0f;
        w->Spd.y = -400.0f;
        w->Spd.z = 0.0f;
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10080000;
        em->setStatus(EM_STATUS_IK_OFF);
        PSVECAdd(&em->pos, &w->Spd, &em->pos);
        w->Spd.y -= 20.0f;
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                EM10_FALL_WATER_EFFECT;
            }
        }
        MotionMove(em, 0);
        tmp = em->pos;
        tmp.y = em->pos_old.y;
        fl = SatMgr.getFloor(&tmp, 0, 600.0f, 100000.0f, 0);
        if (em->pos.y < fl) {
            EM10_JUMP_DOWN_LAND;
        }
        break;
    case 4:
        if (CheckInWater(em, 0)) {
            em10FallWaterCk(em);
            if (w->TmpU32 == 0) {
                EM10_FALL_WATER_EFFECT;
            }
        } else if (ChkWaterEffectEnable(&em->pos)) {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, em, 0);
        } else {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        em->r_no_2++;
    case 5:
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R1 == 0x45 JumpUp: climbs up a ledge (motion 0x1A0, TmpV = the position delta to Target_pos fed in over
// the motion, work flag 0x180000), then walks with Atk_wait 15.
// FUNCTION: BIO4 0x0046B140
// STATUS: stub
#if 0 // PARKED: Mtx, PSMTXMultVec, PSMTXRotRad, PSVECAdd, PSVECScale, PSVECSubtract, RotMatrix, ScaleMatrix, TransMatrix, mat
void em10_R1_JumpUp(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx mat;
    Vec a;
    Vec b;
    Vec tmp;
    f32 f;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1A0), PL_ARC_PTR(em->subArc, 0x1A1), 10, 1, 0);
        f = w->Target_pos.y - em->pos.y + 500.0f;
        w->TmpF = f;
        w->TmpF *= 0.010989011f;
        w->Spd.x = 0.0f;
        w->Spd.y = w->TmpF * 13.0f;
        w->Spd.z = 0.0f;
        em->ang.y = w->Target_dir;
        PSMTXRotRad(mat, 'y', w->Target_dir);
        TransMatrix(mat, &w->Target_pos);
        ScaleMatrix(mat, &em->scale);
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 1448.59f;
        PSMTXMultVec(mat, &a, &a);
        RotMatrix(em->mat, &em->ang);
        TransMatrix(em->mat, &em->pos);
        ScaleMatrix(em->mat, &em->scale);
        b.x = 0.0f;
        b.y = 0.0f;
        b.z = 1448.59f;
        PSMTXMultVec(em->mat, &b, &b);
        PSVECSubtract(&a, &b, &w->TmpV);
        w->TmpV.y = 0.0f;
        em10CallVoiceSe2(em, w->Se_tbl[17], 8);
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x40) {
            w->Be_flg |= 0x00180000;
            if (em->Motion.Seq_old.Free & 4) {
                PSVECAdd(&em->pos, &w->Spd, &em->pos);
                w->Spd.y -= 33.333332f;
            } else {
                em->dmg.m_Timer = 2;
                PSVECAdd(&em->pos, &w->Spd, &em->pos);
                w->Spd.y -= w->TmpF;
            }
            PSVECScale(&w->TmpV, &tmp, 0.1f);
            PSVECAdd(&em->pos, &tmp, &em->pos);
            PSVECSubtract(&w->TmpV, &tmp, &w->TmpV);
        }
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R1 == 0x46 Trade: the robed type 6 Ganado's merchant routine: opens the coat (motion 0xA5, cloth
// and goods parts shown), waits for the shop sub screen (SubScreenOpen SS_OPEN_SHOP) to close, closes
// it again (0xA7) and returns to Wait.
// FUNCTION: BIO4 0x0043E250
// STATUS: stub
#if 0 // PARKED: KeyStop, LIMIT_ANGLE, Muku, SS_OPEN_SHOP, SndCall, Stop_flg, SubScreenOpen
void em10_R1_Trade(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xA5), 0, 10, 1, 0);
        KeyStop(0xEFCF0000);
        w->Seid_voice = SndCall(8, 0x84, &em->pos, em->id, 0, em);
        em->r_no_2++;
    case 1:
        em->dmg.m_Timer = 2;
        if (MOTION(em)->Seq_frame > 35.7f && MOTION(em)->Seq_frame < 36.3f) {
            em10ClothPartsSet(em, 1);
            SndCall(8, 0x86, &em->pos, em->id, 0, em);
        }
        if (MOTION(em)->Seq_frame > 40.7f && MOTION(em)->Seq_frame < 41.3f) {
            em10GoodsPartsSet(em, 1);
        }
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        if (SubScreenOpen(SS_OPEN_SHOP, 0)) {
            em->r_no_2++;
        }
        break;
    case 3:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xA7), 0, 10, 1, 0);
        SndCall(8, 0x86, &em->pos, em->id, 0, em);
        w->Seid_voice = SndCall(8, 0x9A, &em->pos, em->id, 0, em);
        pG->Stop_flg &= 0x7FFFFFFF; // load stays below the x5B8 store
        em->r_no_2++;
    case 4:
        if (MOTION(em)->Seq_frame > 33.7f && MOTION(em)->Seq_frame < 34.3f) {
            em10ClothPartsSet(em, 0);
        }
        if (MOTION(em)->Seq_frame > 26.7f && MOTION(em)->Seq_frame < 27.3f) {
            em10GoodsPartsSet(em, 0);
        }
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 1, 0, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x47 Drive: the truck driver: sits in the driving motion 0x1BA on the truck found by
// em10SearchTruck (pTruck, follows its root part); when shot or when the truck crashes (flag bit1) it
// slumps (0x1BB, hp 0, inactive) and stays fixed to the truck.
// FUNCTION: BIO4 0x0044DF50
// STATUS: stub
#if 0 // PARKED: PSMTXConcat, RotMatrix, ScaleMatrix, TransMatrix, mat
void em10_R1_Drive(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* truck;

    em->atari.m_flag &= 0xFCFF;
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1BA), 0, 0, 5, 0);
        w->pTruck = 0;
        w->Scale.x = 1.0f;
        w->Scale.y = 1.0f;
        w->Scale.z = 1.0f;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em10SearchTruck(em)) {
            em->r_no_2++;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1BA), 0, 0, 5, 0);
        em->r_no_2++;
    case 3:
        em->pos.x = 0.0f;
        em->pos.y = 0.0f;
        em->pos.z = 0.0f;
        em->ang.x = 0.0f;
        em->ang.y = 0.0f;
        em->ang.z = 0.0f;
        RotMatrix(em->mat, &em->ang);
        TransMatrix(em->mat, &em->pos);
        ScaleMatrix(em->mat, &em->scale);
        PSMTXConcat(w->pTruck->getPartsPtr(0)->mat, em->mat, em->mat);
        MOTION(em)->Mot_flag |= 0x40000000;
        MotionMove(em, 0);
        if (em->hp <= 0 || (w->pTruck->flag & 2)) {
            em->r_no_2++;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1BB), 0, 3, 1, 0);
        em->hp = 0;
        em->clearStatus(EM_STATUS_ACTIVE);
        em->r_no_2++;
    case 5:
        em->pos.x = 0.0f;
        em->pos.y = -100.0f;
        em->pos.z = 0.0f;
        em->ang.x = 0.0f;
        em->ang.y = 0.0f;
        em->ang.z = 0.0f;
        truck = w->pTruck->getPartsPtr(0);
        RotMatrix(em->mat, &em->ang);
        TransMatrix(em->mat, &em->pos);
        ScaleMatrix(em->mat, &em->scale);
        PSMTXConcat(truck->mat, em->mat, em->mat);
        MOTION(em)->Mot_flag |= 0x40000000;
        MotionMove(em, 0);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x48 Catapult: the catapult crew Ganado (set 0x19 arrives here from Goto): loops motion 5
// from a random frame with hp 1, neck tracking on (flag 0x8000); only leaves through a goto.
// FUNCTION: BIO4 0x0043E5C0
// STATUS: stub
void em10_R1_Catapult(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 n;
    u32 r;

    w->Be_flg |= 0x8000;
    switch (em->r_no_2) {
    case 0:
        n = *(u16*) PL_ARC_PTR(em->subArc, 5) & 0x3FFF;
        r = Rnd() % n;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 5), 0, 10, 5, (u16) r);
        em->hp = 1;
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x40000;
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->flag &= ~1;
            em->r_no_2++;
        }
        break;
    case 2:
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    if (!em10GotoCk(em)) {
        em10HandSet(em, 0);
    }
}

// R1 == 0x49 RockPush: the Ganado pushing the boulder (motion 5, no collision): comes alive on the
// release flag, then 150 frames later hides again (invisible, flag 0x400000) or dies out of sight.
// FUNCTION: BIO4 0x0043E710
// STATUS: stub
#if 0 // PARKED: C2105, int, invisible_factor, throughOn
void em10_R1_RockPush(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Be_flg |= 0x8000;
    em->atari.throughOn();
    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 5), 0, 0, 5, 0);
        em->be_flag &= ~2;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em->flag & 1) {
            em->flag &= ~1;
            em->be_flag |= 2;
            em->r_no_2++;
        }
        break;
    case 2:
        w->Timer = 150;
        em->r_no_2++;
    case 3:
        if (w->Timer) {
            w->Timer--;
        } else {
            em->invisible_factor = 0.0f;
            em->be_flag &= ~2;
            w->Be_flg |= 0x400000;
        }
        if (MotionMove(em, 0)) {
            em->hp = 0;
            em->invisible_factor = 0.0f;
            em->be_flag &= ~2;
            w->Be_flg |= 0x400000;
            em->r_no_2++;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x20 ParasiteAtk: the exposed parasite attacks: the em25 partner (pParasite v68/v78, aimed
// at Ashley when it can) or the core object (pCore setAtk 0/1 or setCritical), while the body plays
// motion 0x28E and drops its weapon / shield; ends when the parasite reports the hit, Atk_wait by rank.
// FUNCTION: BIO4 0x0046B4C0
// STATUS: stub
#if 0 // PARKED: C2227, Game_level, ckAtkHit, int, l_pl, setAtk, setCritical, v68, v70, v78, vC8
void em10_R1_ParasiteAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;
    int flag;

    switch (em->r_no_2) {
    case 0:
        flag = 0;
        if (w->pParasite) {
            f32 d;
            flag = 0x1E;
            if (w->pParasite->vC8() && pSUB) {
                d = w->L_sub;
            } else {
                d = em->l_pl;
            }
            if (d < 2560000.0f) {
                w->pParasite->v68();
            } else {
                w->pParasite->v78();
                r = Rnd();
                w->Poison_wait = r % 300 + 300;
            }
        }
        if (w->pCore) {
            if (w->Ganado != 1) {
                flag = 0x1E;
                if (w->Be_flg & 0x8000000) {
                    w->pCore->setAtk(1);
                } else {
                    w->pCore->setAtk(0);
                }
            } else {
                w->pCore->setCritical();
            }
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x28E), 0, 10, 1, flag);
        w->Atk_ck = 0;
        w->Timer = 120;
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em->r_no_2++;
    case 1:
        if (w->pCore && w->pCore->ckAtkHit()) {
            w->Atk_ck = 1;
        }
        if (w->pParasite && w->pParasite->v70()) {
            w->Atk_ck = 1;
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x21 ShotBowgun: the bowgun Ganado: raise (0x12C), aim at the player's chest blending the
// up / down poses (em10BlendMotSet, EM10_BOWGUN_AIM_RATE) with the fire lock (ctrl12 EM10_THROW,
// screen-in, scenario check), shoot (0x12F..: the arrow cEmWep setShot with Em10AtkTbl[7]), lower and
// re-arm (0x131, effect 0x1F), or side-step when the player closes in; flag 0x200 = aiming.
// FUNCTION: BIO4 0x0046B800
// STATUS: stub
#if 0 // PARKED: C1003, C2105, CTRL12_ID_EM10_THROW, Ctrl12Ck, EM10_BOWGUN_AIM_RATE, EmDeadCk, Game_level, LIMIT_ANGLE, Muku, PSMTXMultVec, PSMTXMultVecSR, SetWeapon, be_flag, fRand0_1, fRand1_1, int, mat
void em10_R1_ShotBowgun(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec d;
    Vec tgt;
    Vec ofs;
    Vec spd;
    cEmWep* wep;
    f32 rate;
    f32 len;
    int flag;

    w->Be_flg |= 0x200;
    if (w->Be_flg & 1) {
        tgt = pPL->getPartsPtr(2)->world;
    } else {
        tgt = pPL->pos;
        tgt.y += 1300.0f;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x12C), PL_ARC_PTR(em->subArc, 0x12D), 10, flag, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.3926991f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0) || (em->Motion.Seq_old.Free & 4)) {
            if (w->Goto_mode != 0 || !w->pWeapon) {
                em->r_no_2 = 8;
            } else if (w->Arrow_num) {
                em->r_no_2++;
            } else {
                em->r_no_2 = 6;
            }
        }
        break;
    case 2:
        EM10_BOWGUN_AIM_RATE;
        w->Hokan = 10;
        w->Frame = 0;
        w->Blend = rate;
        {
            u8 r = Rnd() % 30;
            w->Timer = r + 30;
        }
        em->r_no_2++;
    case 3:
        w->Throw_timer = 2;
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        EM10_BOWGUN_AIM_RATE;
        w->Blend = w->Blend * 0.9f + rate * 0.1f;
        flag = (em->flag & 0x1000000) ? 0x45 : 5;
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x12E), PL_ARC_PTR(em->subArc, 0x133), PL_ARC_PTR(em->subArc, 0x136), 0, 0, 0, flag);
        MotionMove(em, 0);
        if (w->Goto_mode != 0) {
            em->r_no_2 = 8;
            break;
        }
        if (em10ThrowNearCk(em)) {
            em->r_no_2 = 0;
            em->r_no_0 = 1;
            em->r_no_1 = 0x17;
            em->r_no_3 = Rnd() & 1;
            break;
        }
        if (!em10ThrowScaCk(em) || !w->pWeapon) {
            em->r_no_2 = 8;
            break;
        }
        if (w->Timer) {
            w->Timer--;
            break;
        }
        if (!(em->flag & 1)) {
            if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_THROW)) {
                break;
            }
            if (EmDeadCk(pPL)) {
                break;
            }
            if (pG->Game_level <= 4) {
                if (!em10ScreenInCk(em)) {
                    break;
                }
            }
            if (pG->Game_level <= 3) {
                u8 r = Rnd() % 10;
                if (r > 4) {
                    u8 r2 = Rnd() % 30;
                    w->Timer = r2 + 30;
                    break;
                }
            }
        }
        if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, 3.1415927f)) < 0.19634955f) {
            em->flag &= ~1;
            em->r_no_2++;
        } else {
            em->r_no_2 = 8;
        }
        break;
    case 4:
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 1;
        if (pG->Game_level <= 3) {
            w->Timer = 0;
        }
        w->Hokan = 0;
        w->Frame = 0;
        em->r_no_2++;
    case 5:
        EM10_BOWGUN_AIM_RATE;
        w->Blend = w->Blend * 0.9f + rate * 0.1f;
        flag = (em->flag & 0x1000000) ? 0x45 : 5;
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x12F), PL_ARC_PTR(em->subArc, 0x134), PL_ARC_PTR(em->subArc, 0x137),
                        (int) PL_ARC_PTR(em->subArc, 0x130), (int) PL_ARC_PTR(em->subArc, 0x135), (int) PL_ARC_PTR(em->subArc, 0x138), flag);
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (em->type != 6) {
                w->Arrow_num--;
            }
            w->Throw_timer = 10;
            w->Atk_trg = 1;
            if (w->pWeapon && w->mot[75] && w->mot[76]) {
                ofs.x = 0.0f;
                ofs.y = 57.4f;
                ofs.z = 232.98f;
                PSMTXMultVec(w->pWeapon->mat, &ofs, &ofs);
                wep = SetWeapon(w->mot[75], w->mot[76], &ofs, &em->ang, 0);
                if (wep) {
                    spd.x = fRand1_1() * 50.0f;
                    spd.y = fRand0_1() * 50.0f;
                    spd.z = fRand1_1() * 50.0f + 1000.0f;
                    wep->be_flag |= 0x4000;
                    PSMTXMultVecSR(w->pWeapon->mat, &spd, &spd);
                    wep->setShot(&spd, &Em10AtkTbl[7]);
                    wep->setSeDamage(8, 0x3F, em->id);
                    wep->setSeHit(8, 0x81, em->id);
                    wep->setSeHitWall(8, 0x82, em->id);
                    wep->setSeThrow(8, 0x80, em->id, 0xFF);
                    wep->setEffDamage(0, 0x18);
                    wep->setEffHit(0x10, 0xB);
                    wep->setEffWater(1, 0x37);
                    wep->setEffAlways(0x10, 0x20);
                    SndCall(8, 0x7F, &em->pos, em->id, 0, em);
                }
                if (w->Arrow_num == 0) {
                    EffectEspDelete(0, w->EffKindIdArrow, w->pWeapon, 0);
                    EffectEspgenDelete(0, w->EffKindIdArrow, w->pWeapon);
                    EffectEfmDelete(0, w->EffKindIdArrow, w->pWeapon);
                }
            }
        }
        if (MotionMove(em, 0)) {
            if (!em10ThrowScaCk(em) || !w->pWeapon) {
                em->r_no_2 = 8;
            } else if (w->Arrow_num == 0) {
                if ((Rnd() & 3) && em10HideRtnCk(em)) {
                    break;
                }
                if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, 3.1415927f)) < 0.19634955f) {
                    em->r_no_2++;
                } else {
                    em->r_no_2 = 8;
                }
            } else if (w->Timer == 0) {
                em->r_no_2 = 2;
            } else {
                w->Timer--;
                if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_THROW) || EmDeadCk(pPL)) {
                    em->r_no_2 = 2;
                }
            }
        }
        break;
    case 6:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x131), PL_ARC_PTR(em->subArc, 0x132), 10, flag, 0);
        em->r_no_2++;
    case 7:
        if (em->Motion.Seq_old.Free & 1) {
            w->Arrow_num = 2;
            if (w->Wep_type == 8 && w->pWeapon) {
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x1F, 0, w->EffKindIdArrow, w->pWeapon, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, 3.1415927f)) < 0.19634955f) {
                em->r_no_2 = 2;
            } else if (!em10HideRtnCk(em)) {
                em->r_no_2++;
            }
        }
        break;
    case 8:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x139), 0, 10, flag, 0);
        em->r_no_2++;
    case 9:
        if (MotionMove(em, 0)) {
            if (w->Goto_mode) {
                em10WalkRtnSet(em);
            } else if (!em10HideRtnCk(em)) {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x22 ShotRocket: the rocket launcher Ganado: kneels (0x181, flag 0x40000000), aims the
// blended pose at the player, fires (SetWeapon of the player archive's rocket model, cEmWep setRocket
// with Em10AtkTbl[7], SE 0xB1), recovers (0x18A) and drops the empty launcher (setWeaponFall).
// FUNCTION: BIO4 0x0046C5D0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, CTRL12_ID_EM10_THROW, Ctrl12Ck, EM10_ROCKET_AIM_RATE, Em10AtkTbl, EmDeadCk, Game_level, LIMIT_ANGLE, Muku, PSMTXMultVec, PSMTXMultVecSR, SetWeapon, SndCall, be_flag, getPartsPtr, int, mat, pPlayer, setEffDamage, setEffHit, setEffWater, setRocket, setSeDamage, setSeHit, setSeHitWall, setSeThrow
void em10_R1_ShotRocket(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec d;
    Vec tgt;
    Vec ofs;
    Vec spd;
    cEmWep* wep;
    cModel* p;
    f32 rate;
    f32 len;

    w->Be_flg |= 0x200;
    if (w->Be_flg & 1) {
        tgt = pPL->getPartsPtr(2)->world;
    } else {
        tgt = pPL->pos;
        tgt.y += 1300.0f;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x181), PL_ARC_PTR(em->subArc, 0x182), 10, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 4) {
            w->Be_flg |= 0x40000000;
        }
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0)) {
            if (w->Goto_mode != 0) {
                em10WalkRtnSet(em);
            } else if (!w->pWeapon) {
                em->r_no_2 = 6;
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        EM10_ROCKET_AIM_RATE;
        w->Hokan = 10;
        w->Frame = 0;
        w->Blend = rate;
        {
            u8 r = Rnd() % 30;
            w->Timer = r + 30;
        }
        em->r_no_2++;
    case 3:
        w->Throw_timer = 2;
        w->Be_flg |= 0x40000000;
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        EM10_ROCKET_AIM_RATE;
        w->Blend = w->Blend * 0.9f + rate * 0.1f;
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x183), PL_ARC_PTR(em->subArc, 0x186), PL_ARC_PTR(em->subArc, 0x188), 0, 0, 0, 5);
        MotionMove(em, 0);
        if (w->Goto_mode != 0) {
            em10WalkRtnSet(em);
            break;
        }
        if (!w->pWeapon) {
            em->r_no_2 = 6;
            break;
        }
        if (!(em->flag & 1)) {
            if (em10ThrowNearCk(em)) {
                em->r_no_2 = 0;
                em->r_no_0 = 1;
                em->r_no_1 = 0x17;
                em->r_no_3 = Rnd() & 1;
                break;
            }
            if (!em10ThrowScaCk(em)) {
                em10WalkRtnSet(em);
                break;
            }
        }
        if (w->Timer) {
            w->Timer--;
            break;
        }
        if (!(em->flag & 1)) {
            if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_THROW)) {
                break;
            }
            if (EmDeadCk(pPL)) {
                break;
            }
        }
        if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, 3.1415927f)) < 0.19634955f) {
            em->flag &= ~1;
            em->r_no_2++;
        } else {
            em10WalkRtnSet(em);
        }
        break;
    case 4:
        w->Be_flg |= 0x40000000;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 1;
        if (pG->Game_level <= 3) {
            w->Timer = 0;
        }
        w->Hokan = 0;
        w->Frame = 0;
        ofs.x = 0.0f;
        ofs.y = 57.4f;
        ofs.z = 232.98f;
        PSMTXMultVec(w->pWeapon->mat, &ofs, &ofs);
        wep = SetWeapon(PL_ARC_PTR(pG->pPlayer, 0x70), PL_ARC_PTR(pG->pPlayer, 0x71), &ofs, &em->ang, 0);
        if (wep) {
            spd.x = -500.0f;
            spd.y = 0.0f;
            spd.z = 0.0f;
            wep->be_flag |= 0x4000;
            PSMTXMultVecSR(w->pWeapon->mat, &spd, &spd);
            wep->setRocket(em, &spd, &Em10AtkTbl[7]);
            wep->setSeDamage(8, 0x3F, em->id);
            wep->setSeHit(8, 0x81, em->id);
            wep->setSeHitWall(8, 0x82, em->id);
            wep->setSeThrow(8, 0xB2, em->id, 0xFF);
            wep->setEffDamage(0, 0x18);
            wep->setEffHit(0x10, 0xB);
            wep->setEffWater(1, 0x37);
            wep->setEffAlways(0x10, 0x9F);
            SndCall(8, 0xB1, &em->pos, em->id, 0, em);
            p = w->pWeapon->getPartsPtr(2);
            p->scale.x = 0.0f;
            p->scale.y = 0.0f;
            p->scale.z = 0.0f;
        }
        em->r_no_2++;
    case 5:
        EM10_ROCKET_AIM_RATE;
        w->Blend = w->Blend * 0.9f + rate * 0.1f;
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x184), PL_ARC_PTR(em->subArc, 0x187), PL_ARC_PTR(em->subArc, 0x189),
                        (int) PL_ARC_PTR(em->subArc, 0x185), 0, 0, 1);
        if (MotionMove(em, 0)) {
            em->r_no_2 = 6;
        }
        break;
    case 6:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x18A), PL_ARC_PTR(em->subArc, 0x18B), 10, 1, 0);
        em->r_no_2++;
    case 7:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_old.Free & 1) {
            em->setWeaponFall();
        }
        break;
    }
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x23 ShotGatling: the hand-held gatling Ganado: spin-up (0x198, SE 0xB3), aims the torso at
// the player with a distance-dependent turn limit, fires in bursts (blend poses 0x19A..0x19D,
// em10GatlingHitCk for the player hit every 5..8 frames), stops (0x139, SE 0xB4) and walks / hides.
// FUNCTION: BIO4 0x0046D0B0
// STATUS: stub
#if 0 // PARKED: C2105, C2228, C2450, EM10_GATLING_TURN_LIMIT, EatMgr, EmDeadCk, Game_level, LIMIT_ANGLE, Muku, Muku2, PSVECSubtract, SQRTF, SndCall, Timer2, Timer3, Timer4, atan2f, int, partsFixMemory, partsWorldCalc
void em10_R1_ShotGatling(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec d;
    Vec tgt;
    cModel* p;
    f32 ang;
    f32 lim;
    f32 rate;
    f32 len;
    f32 sv;

    tgt = pPL->pos;
    tgt.y += 1200.0f;
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x198), PL_ARC_PTR(em->subArc, 0x199), 10, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        SndCall(8, 0xB3, &em->pos, em->id, 0, em);
        w->Timer = 15;
        w->Blend = 0.0f;
        w->Timer3 = 0;
        w->Timer4 = 0;
        em->r_no_2++;
    case 1:
        p = em->getPartsPtr(10);
        ang = em->ang.y;
        ang = LIMIT_ANGLE(ang + (Muku(&p->world, &pPL->pos, ang, 3.1415927f) + -0.34906584f));
        em->ang.y += Muku2(em->ang.y, ang, 0.05235988f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (MotionMove(em, 0)) {
            if (w->Goto_mode != 0) {
                em->r_no_2 = 6;
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        w->Hokan = 10;
        w->Frame = 0;
        w->Timer2 = 50;
        em->r_no_2++;
    case 3:
        w->Gatling_roll = 1;
        w->Throw_timer = 2;
        EM10_GATLING_TURN_LIMIT(0.034906585f);
        em->getPartsPtr(10);
        p = em->getPartsPtr(10);
        ang = em->ang.y;
        ang = LIMIT_ANGLE(ang + (Muku(&p->world, &pPL->pos, ang, 3.1415927f) + -0.34906584f));
        em->ang.y += Muku2(em->ang.y, ang, lim);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        w->Blend = w->Blend * 0.95f + 6.4f;
        em->partsFixMemory(0x15);
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x19A), PL_ARC_PTR(em->subArc, 0x19C), PL_ARC_PTR(em->subArc, 0x19E), 0, 0, 0, 1);
        MotionMove(em, 0);
        if (w->Goto_mode != 0) {
            em->r_no_2 = 6;
            break;
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            if (!(em->flag & 1)) {
                if (EmDeadCk(pPL)) {
                    break;
                }
                if (pG->Game_level <= 4) {
                    if (!em10ScreenInCk(em)) {
                        break;
                    }
                }
                if (pG->Game_level <= 3) {
                    u8 r = Rnd() % 10;
                    if (r > 4) {
                        u8 r2 = Rnd() % 30;
                        w->Timer = r2 + 30;
                        break;
                    }
                }
            }
            em->flag &= ~1;
            em->r_no_2++;
            break;
        }
        em10AxeAtkCk(em);
        break;
    case 4:
        w->Timer = 1;
        if (pG->Game_level <= 3) {
            w->Timer = 0;
        }
        w->Hokan = 0;
        w->Frame = 0;
        w->Timer = 2;
        em->r_no_2++;
    case 5:
        w->Gatling_roll = 1;
        EM10_GATLING_TURN_LIMIT(0.02268928f);
        em->ang.y += Muku(&em->getPartsPtr(10)->world, &pPL->pos, em->ang.y, lim);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        PSVECSubtract(&tgt, &em->getPartsPtr(0)->world, &d);
        len = SQRTF(d.x * d.x + d.z * d.z);
        lim = -atan2f(d.y, len);
        rate = lim * 488.92398f;
        if (rate > 255.0f) {
            rate = 255.0f;
        }
        if (rate < -255.0f) {
            rate = -255.0f;
        }
        w->Blend = w->Blend * 0.95f + rate * 0.05f;
        em->partsFixMemory(0x15);
        em10BlendMotSet(em, PL_ARC_PTR(em->subArc, 0x19B), PL_ARC_PTR(em->subArc, 0x19D), PL_ARC_PTR(em->subArc, 0x19F), 0, 0, 0, 1);
        if (MotionMove(em, 0)) {
            if (!em10ThrowScaCk(em)) {
                em->r_no_2 = 6;
                break;
            }
            if (EatMgr.hitCheck(&em->getPartsPtr(10)->world, &tgt, 0, 0, 0, 0x404000)) {
                em->r_no_2 = 6;
                break;
            }
            if (w->Pl_rot > 0.7853982f) {
                em->r_no_2 = 6;
                break;
            }
            PSVECSubtract(&pPL->pos, &em->pos, &d);
            len = SQRTF(d.x * d.x + d.z * d.z);
            lim = -atan2f(d.y, len);
            lim = fabsf(lim);
            if (lim > 0.43633232f) {
                em->r_no_2 = 6;
            } else {
                w->Timer = 60;
                em->r_no_2 = 2;
            }
        } else {
            switch (w->Timer) {
            case 2:
                sv = w->Waist_dir_y;
                em10WaistMove(em);
                em->partsWorldCalc();
                if (em10GatlingHitCk(em)) {
                    if (w->Timer2 > 8) {
                        w->Timer2 = 8;
                    }
                }
                w->Waist_dir_y = sv;
            default:
                w->Timer--;
                break;
            case 0:
                if (w->Pl_rot > 2.0943952f) {
                    if (w->Timer2 > 5) {
                        w->Timer2 = 5;
                    }
                }
                if (w->Timer2) {
                    w->Timer2--;
                    em->r_no_2 = 4;
                }
                break;
            }
        }
        break;
    case 6:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x139), 0, 10, 1, 0);
        SndCall(8, 0xB4, &em->pos, em->id, 0, em);
        em->r_no_2++;
    case 7:
        if (MotionMove(em, 0)) {
            if (w->Goto_mode) {
                em10WalkRtnSet(em);
            } else if (!em10HideRtnCk(em)) {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
        em->hp = 0;
    }
    if (em->hp <= 0) {
        EmRoutineSet(em, 2, 9, 0, 0);
    }
}
#endif // PARKED

// R1 == 0x24 ThrowAxe: throws the hand weapon at the player (motion 0x82, scythe 0x13D): the release
// frame hands the weapon to cEmWep::setThrow (Em10AtkTbl[5]) / setThrowScythe ([6]) aimed at the
// player's chest with a random miss chance by rank; the eye glow effect marks a sure hit.
// FUNCTION: BIO4 0x0046DCF0
// STATUS: stub
#if 0 // PARKED: C1003, C2105, Game_level, LIMIT_ANGLE, Muku, PSVECSubtract, SQRTF, Timer2, atan2, fRand1_1, int, sinf
void em10_R1_ThrowAxe(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec spd;
    Vec d;
    Vec plPos;
    Vec wpos;
    f32 len;
    f32 t;
    f32 v;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        if (w->Wep_type != 6) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, flag, 0x10);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x13D), PL_ARC_PTR(em->subArc, 0x13F), 10, flag, 0);
        }
        w->Timer = 10;
        w->Timer2 = 0;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (w->pWeapon) {
                spd.x = fRand1_1() * 50.0f + 20.0f;
                spd.y = fRand1_1() * 10.0f + 75.0f;
                spd.z = fRand1_1() * 10.0f + 350.0f;
                if (pG->Game_level <= 3) {
                    if ((u8) (Rnd() % 10) > 4) {
                        if ((u8) (Rnd() % 10) > 4) {
                            spd.x = 50.0f;
                        } else {
                            spd.x = -10.0f;
                        }
                    }
                }
                if (pG->Game_level <= 1) {
                    if ((u8) (Rnd() % 10) > 1) {
                        if ((u8) (Rnd() % 10) > 4) {
                            spd.x = 50.0f;
                        } else {
                            spd.x = -10.0f;
                        }
                    }
                }
                PSVECSubtract(&pPL->pos, &em->pos, &d);
                len = SQRTF(d.x * d.x + d.z * d.z);
                t = -atan2(d.y, len);
                if (len < 3000.0f) {
                    if (len < 1500.0f) {
                        v = sinf(t) * -700.0f;
                    } else {
                        v = sinf(t) * -500.0f;
                    }
                } else {
                    v = sinf(t) * -400.0f;
                }
                spd.y += v;
                PSMTXMultVecSR(em->mat, &spd, &spd);
                if (w->Wep_type != 6) {
                    w->pWeapon->setThrow(&spd, &Em10AtkTbl[5], 15.0f);
                } else {
                    plPos = pPL->pos;
                    plPos.y += 1500.0f;
                    wpos.x = w->pWeapon->mat[0][3];
                    wpos.y = w->pWeapon->mat[1][3];
                    wpos.z = w->pWeapon->mat[2][3];
                    PSVECSubtract(&plPos, &wpos, &spd);
#line 15148 "D:/Bio4/Prog/em10.cpp"
                    VECNormalize(&spd, &spd);
                    PSVECScale(&spd, &spd, 250.0f);
                    w->pWeapon->setThrowScythe(&spd, &Em10AtkTbl[6]);
                }
                w->pWeapon = 0;
                w->Wep_type = 0;
                w->Throw_timer = 10;
            }
        }
        if (w->pWeapon) {
            if (GetEm10EyeEffectEnable()) {
                if (w->Timer2) {
                    w->Timer2--;
                } else {
                    w->Timer2 = 14;
                    switch (w->Wep_type) {
                    case 2:
                        EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x89, 0, ESP_CORE_KIND_NONE, w->pWeapon, 0);
                        break;
                    case 3:
                        EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x8A, 0, ESP_CORE_KIND_NONE, w->pWeapon, 0);
                        break;
                    }
                }
            }
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x25 ThrowBomb: throws the lit dynamite (motion 0x82, em10BombThrow on the release frame),
// then walks.
// FUNCTION: BIO4 0x0046E3C0
// STATUS: stub
#if 0 // PARKED: C2106, LIMIT_ANGLE, Muku, int
void em10_R1_ThrowBomb(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0x41, 0x10);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 1, 0x10);
        }
        w->Timer = 10;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        if (w->Fire_timer > 90) {
            w->Fire_timer = 90;
        }
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer -= 1;
            em->ang.y = em->ang.y + Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            em10BombThrow(em);
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x50 FixBomber: the stationary dynamite thrower (sets 0x24/0x25/0x2B): faces the player, when
// the throw is clear (em10BombThrowScaCk) lights the fuse (0x7E), throws (0x82, em10BombThrow), takes
// the next stick from the spare (EM10_WEP2_TAKE, 0x76), sits down (0x9A) between throws and stands
// up (0x9B) when the player is seen again.
// FUNCTION: BIO4 0x0044E290
// STATUS: stub
#if 0 // PARKED: C2105, EM10_BOMB_FIRE_EFFECT, EM10_WEP2_TAKE, EmDeadCk, LIMIT_ANGLE, Mtx, Muku, Muku2, PSMTXInverse, PSMTXMultVec, int, inv, mat, pl_life
void em10_R1_FixBomber(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec ofs;
    Mtx inv;
    f32 f;
    f32 ang;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->TmpF = em->ang.y;
        em->r_no_2++;
    case 1:
        if (em->r_no_3) {
            f = Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        } else {
            ang = Muku(&em->pos, &pPL->pos, w->TmpF, 3.1415927f);
            if (ang > 0.17453292f) {
                ang = 0.17453292f;
            }
            if (ang < -0.17453292f) {
                ang = -0.17453292f;
            }
            ang = LIMIT_ANGLE(ang + w->TmpF);
            f = Muku2(em->ang.y, ang, 0.09817477f);
        }
        em->ang.y += f;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if (em10GotoCk(em)) {
            break;
        }
        if (w->pWeapon == 0) {
            em->r_no_2 = 6;
            break;
        }
        if (em->flag & 1) {
            em->flag &= ~1;
            EM10_BOMB_FIRE_EFFECT();
            em->r_no_2 = 4;
            break;
        }
        if (w->L_pl_guard < em->Guard_r) {
            em->set = 0;
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            break;
        }
        if (em->r_no_3 && !(w->Be_flg & 1)) {
            break;
        }
        if (EmDeadCk(pPL)) {
            break;
        }
        if ((s16) pG->pl_life <= 0) {
            break;
        }
        PSMTXInverse(em->mat, inv);
        PSMTXMultVec(inv, &pPL->pos, &ofs);
        if (ofs.x > -2000.0f && ofs.x < 2000.0f && ofs.y < 1000.0f && ofs.z > 1000.0f && ofs.z < 20000.0f) {
            if (em10BombThrowScaCk(em)) {
                if (w->Fire_timer) {
                    em->r_no_2 = 4;
                } else {
                    em->r_no_2 = 2;
                }
            }
        }
        break;
    case 2:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x7E), PL_ARC_PTR(em->subArc, 0x7F), 10, 0x40, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x7E), PL_ARC_PTR(em->subArc, 0x7F), 10, 0, 0);
        }
        em->r_no_2++;
    case 3:
        MotionMove(em, 0);
        if (em->Motion.Seq_old.Free & 1) {
            EM10_BOMB_FIRE_EFFECT();
            if (em10BombThrowScaCk(em)) {
                em->r_no_2++;
            } else {
                em->r_no_2 = 0;
            }
        }
        break;
    case 4:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0x40, 0x12);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0, 0x12);
        }
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 10;
        em->r_no_2++;
    case 5:
        if (w->Timer) {
            w->Timer--;
        } else if (em->r_no_3) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            em10BombThrow(em);
        }
        if (MotionMove(em, 0)) {
            if (em->r_no_3 == 2) {
                em->r_no_2 = 8;
            } else {
                em->r_no_2 = 0;
            }
        } else if ((em->Motion.Seq_old.Free & 2) && em->r_no_3 == 2) {
            em->r_no_2 = 8;
        }
        break;
    case 6:
        flag = (em->flag & 0x1000000) ? 0x40 : 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x76), PL_ARC_PTR(em->subArc, 0x77), 10, flag, 0);
        em->r_no_2++;
    case 7:
        if ((em->Motion.Seq_old.Free & 4) && w->pWeapon2) {
            EM10_WEP2_TAKE();
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    case 8:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9A), 0, 5, 1, 0);
        em->r_no_2++;
    case 9:
        if (MotionMove(em, 0)) {
            em->r_no_2++;
            if (w->pWeapon2) {
                EM10_WEP2_TAKE();
            }
        }
        break;
    case 10:
        w->Timer = (u8) (Rnd() % 90) + 90;
        em->r_no_2++;
    case 11:
        MotionMove(em, 0);
        if (w->L_pl_route < 3000.0f) {
            em->setFindPL();
            em->r_no_2++;
        } else if (w->Be_flg & 1) {
            if (w->Timer) {
                w->Timer--;
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 12:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x9B), 0, 5, 1, 0);
        EM10_BOMB_FIRE_EFFECT();
        em->r_no_2++;
    case 13:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 4;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x6A: room 305 dynamite thrower (set 0x3E): faces the player, throws (0x82 + em10BombThrow)
// whenever the room allows (ckR305BomberEnable), re-arms from the spare (0x76) and repeats.
// FUNCTION: BIO4 0x0044EEA0
// STATUS: stub
#if 0 // PARKED: C2105, EM10_BOMB_FIRE_EFFECT, EM10_WEP2_TAKE, LIMIT_ANGLE, Muku, int
void em10_R1_R305Bomber(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec ofs;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if (em10GotoCk(em)) {
            break;
        }
        if (w->pWeapon == 0) {
            em->r_no_2 = 4;
            break;
        }
        if (em->flag & 1) {
            em->flag &= ~1;
            EM10_BOMB_FIRE_EFFECT();
            em->r_no_2 = 2;
        }
        break;
    case 2:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0x40, 0x12);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0, 0x12);
        }
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 10;
        em->r_no_2++;
    case 3:
        if (w->Timer) {
            w->Timer--;
        } else if (em->r_no_3) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            em10BombThrow(em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 4;
        }
        break;
    case 4:
        flag = (em->flag & 0x1000000) ? 0x40 : 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x76), PL_ARC_PTR(em->subArc, 0x77), 10, flag, 0);
        em->r_no_2++;
    case 5:
        if ((em->Motion.Seq_old.Free & 4) && w->pWeapon2) {
            EM10_WEP2_TAKE();
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x6C: room 408 dynamite thrower (set 0x40): waits 60..90 frames between throws (0x82 +
// em10BombThrow), re-arms from the spare (0x76); walks when out of dynamite.
// FUNCTION: BIO4 0x0044F3A0
// STATUS: stub
#if 0 // PARKED: C2105, EM10_BOMB_FIRE_EFFECT, EM10_WEP2_TAKE, LIMIT_ANGLE, Muku, int
void em10_R1_R408Bomber(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec ofs;
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->Timer = (u8) (Rnd() % 30) + 60;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (em10GotoCk(em)) {
            break;
        }
        if (w->pWeapon == 0) {
            em->r_no_2 = 4;
            break;
        }
        if (w->L_pl_guard < em->Guard_r) {
            em->set = 0;
            EmRoutineSet(em, 1, 0x10, 0, 0);
            break;
        }
        if (w->Timer) {
            w->Timer--;
            break;
        }
        EM10_BOMB_FIRE_EFFECT();
        em->r_no_2 = 2;
        break;
    case 2:
        if (em->flag & 0x1000000) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0x40, 0x12);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x82), PL_ARC_PTR(em->subArc, 0x83), 10, 0, 0x12);
        }
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 10;
        em->r_no_2++;
    case 3:
        if (w->Timer) {
            w->Timer--;
        } else if (em->r_no_3) {
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 4) {
            w->Throw_timer = 2;
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            em10BombThrow(em);
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 4;
        }
        break;
    case 4:
        flag = (em->flag & 0x1000000) ? 0x40 : 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x76), PL_ARC_PTR(em->subArc, 0x77), 10, flag, 0);
        em->r_no_2++;
    case 5:
        if ((em->Motion.Seq_old.Free & 4) && w->pWeapon2) {
            EM10_WEP2_TAKE();
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x60 RocketWait: the rocket Ganado (set 0x36) idles facing the player until the release flag
// (cEm::flag bit0), then fires (ShotRocket 0x22).
// FUNCTION: BIO4 0x0043E8E0
// STATUS: stub
#if 0 // PARKED: LIMIT_ANGLE, Muku
void em10_R1_RocketWait(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        em10SetWaitMotion(em, 10);
        w->TmpF = em->ang.y;
        em->r_no_2++;
    case 1:
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if (!em10GotoCk(em) && (em->flag & 1)) {
            em->set = 0;
            EmRoutineSet(em, 1, 0x22, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x26 AxeAtk: the melee swing with the hand weapon (motion 0x80; 0x178 for Wep_type 0xB,
// 0x1A5 for 0xF, 0x170 with a shield, 0x1A2 bare-handed with the effect 0x98): turns to the target
// (Ashley when carrying her), swing SE by weapon, and on the hit frames sweeps em10AtkCk along the
// weapon / hand parts (EM10_AXE_SWEEP_CK segments); Atk_wait 15/45/90 by rank afterwards.
// FUNCTION: BIO4 0x0046E610
// STATUS: stub
#if 0 // PARKED: C2105, EM10_AXE_SWEEP_CK, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, PSMTXMultVec, SndCall, getPartsPtr, int, mat
void em10_R1_AxeAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    cModel* p;
    void* m0;
    int m1;
    int flag;
    int atk;

    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        m0 = PL_ARC_PTR(em->subArc, 0x80);
        m1 = (int) PL_ARC_PTR(em->subArc, 0x81);
        if (w->Wep_type == 0xB) {
            m0 = PL_ARC_PTR(em->subArc, 0x178);
            m1 = (int) PL_ARC_PTR(em->subArc, 0x179);
        }
        if (w->Wep_type == 0xF) {
            m0 = PL_ARC_PTR(em->subArc, 0x1A5);
            m1 = (int) PL_ARC_PTR(em->subArc, 0x1A6);
        }
        if (w->pShield) {
            m0 = PL_ARC_PTR(em->subArc, 0x170);
            m1 = (int) PL_ARC_PTR(em->subArc, 0x171);
        }
        if (em->type == 2) {
            m0 = PL_ARC_PTR(em->subArc, 0x1A2);
            m1 = (int) PL_ARC_PTR(em->subArc, 0x1A3);
        }
        if (em->type == 0x18) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x98, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        MotionSetCore(em, MOTION(em), m0, (void*) m1, 10, flag, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        if ((w->Timer && --w->Timer) || (em->Motion.Seq_old.Free & 8)) {
            f32 ang = (em->Motion.Seq_old.Free & 8) ? 0.049087387f : 0.19634955f;
            if ((w->Be_flg & 0x8000000) && pSUB) {
                em->ang.y += Muku(&em->pos, &pSUB->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            } else {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            }
        }
        if (em->Motion.Seq_old.Free & 0x20) {
            switch (w->Wep_type) {
            case 7:
                SndCall(8, 0x42, &em->pos, em->id, 0, em);
                break;
            case 0xF:
                SndCall(8, 0x42, &em->pos, em->id, 0, em);
                break;
            default:
                SndCall(8, 0x3D, &em->pos, em->id, 0, em);
                break;
            }
            if (em->type == 2) {
                SndCall(8, 0xB3, &em->pos, em->id, 0, em);
            }
        }
        if (em->Motion.Seq_old.Free & 1) {
            atk = (w->Wep_type == 0xA) ? 1 : 0;
            if (w->Wep_type == 0xB) {
                atk = 2;
            }
            if (w->Wep_type == 0xF) {
                atk = 3;
            }
            if (em->type == 2) {
                atk = 0x10;
            }
            if (em->type == 0x18) {
                atk = 0x12;
            }
            if (em->type == 2) {
                p = em->getPartsPtr(0x22);
                em10AtkCk(em, &p->world, &p->world_old2, 0x10, 0);
                p = em->getPartsPtr(0x23);
                em10AtkCk(em, &p->world, &p->world_old2, 0x10, 0);
                p = em->getPartsPtr(0x24);
                em10AtkCk(em, &p->world, &p->world_old2, 0x10, 0);
                p = em->getPartsPtr(0x10);
                em10AtkCk(em, &p->world, &p->world_old2, 0x10, 0);
            }
            if (em->type == 0x18) {
                p = em->getPartsPtr(10);
                v.x = 1000.0f;
                v.y = 0.0f;
                v.z = -1000.0f;
                PSMTXMultVec(p->mat, &v, &v);
                EM10_AXE_SWEEP_CK(p->mat, 200.0f, 0.0f, -200.0f, &v);
                EM10_AXE_SWEEP_CK(p->mat, -50.0f, 0.0f, 50.0f, &v);
                EM10_AXE_SWEEP_CK(p->mat, -300.0f, 0.0f, 300.0f, &v);
                EM10_AXE_SWEEP_CK(p->mat, -550.0f, 0.0f, 550.0f, &v);
            }
            if (w->pWeapon) {
                if (w->Wep_type == 0xB) {
                    cModel* q = w->pWeapon->getPartsPtr(10);
                    em10AtkCk(em, &q->world, &q->world_old2, atk, 0);
                    EM10_AXE_SWEEP_CK(w->pWeapon->mat, 0.0f, 0.0f, 150.0f, &em->pos);
                } else {
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = -1000.0f;
                    PSMTXMultVec(w->pWeapon->mat, &v, &v);
                    EM10_AXE_SWEEP_CK(w->pWeapon->mat, 0.0f, 0.0f, -200.0f, &v);
                    EM10_AXE_SWEEP_CK(w->pWeapon->mat, 0.0f, 0.0f, 50.0f, &v);
                    if (w->Wep_type == 0x10) {
                        EM10_AXE_SWEEP_CK(w->pWeapon->mat, 0.0f, 0.0f, 300.0f, &v);
                        EM10_AXE_SWEEP_CK(w->pWeapon->mat, 0.0f, 0.0f, 550.0f, &v);
                    }
                }
            }
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
                if (w->Wep_type == 0xF) {
                    w->Atk_wait = 90;
                }
                if (em->type == 2) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x27 ShieldAtk: the shield carrier's bash (motion 0x16E): the hit frames check em10AtkCk at
// the shield / hand part (attack kind 4), then Atk_wait by rank and back to the walk.
// FUNCTION: BIO4 0x0046EFA0
// STATUS: stub
#if 0 // PARKED: C1003, C2105, Game_level, LIMIT_ANGLE, Muku, SndCall, int
void em10_R1_ShieldAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    cModel* parts;
    int flag;

    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x16E), PL_ARC_PTR(em->subArc, 0x16F), 10, flag, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        if ((w->Timer && --w->Timer) || (em->Motion.Seq_old.Free & 8)) {
            f32 ang = (em->Motion.Seq_old.Free & 8) ? 0.049087387f : 0.09817477f;
            // LIMIT_ANGLE in both arms (cross-jumped): its argument is the summed register, not a reload.
            if ((w->Be_flg & 0x8000000) && pSUB) {
                em->ang.y += Muku(&em->pos, &pSUB->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            } else {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            }
        }
        if (em->Motion.Seq_old.Free & 0x20) {
            if (w->Wep_type == 7) {
                SndCall(8, 0x42, &em->pos, em->id, 0, em);
            } else {
                SndCall(8, 0x3D, &em->pos, em->id, 0, em);
            }
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (em->flag & 0x1000000) {
                parts = em->getPartsPtr(10);
            } else {
                parts = em->getPartsPtr(16);
            }
            v = parts->world;
            em10AtkCk(em, &v, &parts->world_old2, 4, 0);
            v.y -= 500.0f;
            em10AtkCk(em, &v, &parts->world_old2, 4, 0);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x28 TorchFrame: the torch Ganado swings the flame (motion 0x84, SE 0x8C): on the hit frame
// em10TorchFrameAtkCk / Sub set the player / partner on fire, the flame effect 0x23 and SE 0x8D play.
// FUNCTION: BIO4 0x0046F400
// STATUS: stub
#if 0 // PARKED: C2105, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, SndCall, Timer2, int
void em10_R1_TorchFrame(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        w->Timer2 = 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x84), PL_ARC_PTR(em->subArc, 0x85), 10, flag, 0);
        w->Frame_timer = 60;
        w->Timer2 = 1;
        SndCall(8, 0x8C, &em->pos, em->id, 0, em);
        w->Timer = 10;
        w->Atk_ck = 0;
        w->Atk_ck2 = 0;
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            if ((w->Be_flg & 0x8000000) && pSUB) {
                em->ang.y += Muku(&em->pos, &pSUB->pos, em->ang.y, 0.09817477f);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            } else {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            }
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            em10TorchFrameAtkCk(em);
            em10TorchFrameAtkCkSub(em);
        }
        if (em->Motion.Seq_old.Free & 2) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x23, 0, w->EffKindIdWork, em, 0);
        }
        if (em->Motion.Seq_old.Free & 0x20) {
            w->Seid_frame = SndCall(8, 0x8D, &em->pos, em->id, 0, em);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x29 SukiAtk: the hoe / pitchfork Ganado's swing (one of three motions 0x15D / 0x15F /
// 0x161): turns to the target, and on the hit frame sweeps em10AtkCk along the tool (attack kind 8,
// EM10_SUKI_SWEEP_CK), sparks 0x44 when the tool hits a wall; Atk_wait by rank afterwards.
// FUNCTION: BIO4 0x0046F7D0
// STATUS: stub
#if 0 // PARKED: C2105, EM10_SUKI_SWEEP_CK, EstSet, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, PSMTXMultVec, SndCall, int, mat
void em10_R1_SukiAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    f32 dy;
    u32 sel;
    int flag;
    int hit;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        sel = 0;
        dy = pPL->pos.y - em->pos.y;
        if (dy > -100.0f) {
            sel = 1;
        }
        if (dy < -800.0f) {
            sel = 2;
        }
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        switch (sel) {
        case 0:
        default:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x15D), PL_ARC_PTR(em->subArc, 0x15E), 10, flag, 0);
            break;
        case 1:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x15F), PL_ARC_PTR(em->subArc, 0x160), 10, flag, 0);
            break;
        case 2:
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x161), PL_ARC_PTR(em->subArc, 0x162), 10, flag, 0);
            break;
        }
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->Atk_ck = 0;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(8, 0x3D, &em->pos, em->id, 0, em);
        }
        if (w->Timer) {
            w->Timer--;
            if ((w->Be_flg & 0x8000000) && pSUB) {
                em->ang.y += Muku(&em->pos, &pSUB->pos, em->ang.y, 0.19634955f);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            } else {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            }
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            hit = w->Atk_ck;
            if (hit == 0) {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = -2000.0f;
                PSMTXMultVec(w->pWeapon->mat, &v, &v);
                EM10_SUKI_SWEEP_CK(-1000.0f);
                EM10_SUKI_SWEEP_CK(-700.0f);
                EM10_SUKI_SWEEP_CK(-400.0f);
                EM10_SUKI_SWEEP_CK(-100.0f);
                EM10_SUKI_SWEEP_CK(300.0f);
                EM10_SUKI_SWEEP_CK(600.0f);
                if (w->Atk_ck) {
                    EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x44, 0, ESP_CORE_KIND_NONE, w->pWeapon, (void*) hit);
                }
            }
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x2A ScytheAtk: the scythe swing (0x148 overhead when the player is close / 0x13D sweep):
// swing effect 0x43 after 31 frames, the hit frame sweeps em10AtkCk along the blade
// (EM10_SCYTHE_SWEEP_CK), then Atk_wait by rank.
// FUNCTION: BIO4 0x0046FE60
// STATUS: stub
#if 0 // PARKED: C2105, EM10_SCYTHE_SWEEP_CK, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, PSMTXMultVec, Timer2, int, mat
void em10_R1_ScytheAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    cModel* p;
    int flag;
    int atk;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        p = pPL->getPartsPtr(4);
        if (p->world.y < em->pos.y + 1300.0f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x148), PL_ARC_PTR(em->subArc, 0x149), 10, flag, em->r_no_3);
            em->r_no_3 = 1;
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x13D), PL_ARC_PTR(em->subArc, 0x13E), 10, flag, em->r_no_3);
            em->r_no_3 = 0;
        }
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->Atk_ck = 0;
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer2 = 31;
        em->r_no_2++;
    case 1:
        if ((w->Timer && --w->Timer) || (em->Motion.Seq_old.Free & 8)) {
            f32 ang = (em->Motion.Seq_old.Free & 8) ? 0.049087387f : 0.19634955f;
            if ((w->Be_flg & 0x8000000) && pSUB) {
                em->ang.y += Muku(&em->pos, &pSUB->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            } else {
                em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, ang);
                em->ang.y = LIMIT_ANGLE(em->ang.y);
            }
        }
        if (w->Timer2 && --w->Timer2 == 0) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x43, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        if ((em->Motion.Seq_old.Free & 1) && w->pWeapon) {
            atk = em->r_no_3 ? 10 : 9;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = -1000.0f;
            PSMTXMultVec(w->pWeapon->mat, &v, &v);
            EM10_SCYTHE_SWEEP_CK(200.0f);
            EM10_SCYTHE_SWEEP_CK(400.0f);
            EM10_SCYTHE_SWEEP_CK(600.0f);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x2B ClawAtk: the claw (type 0xA/0xD) Ganado's double swipe (motion 0x11C): the hit frames
// sweep em10AtkCk along both claw parts (kind 0xD, EM10_CLAW_SWEEP_CK); afterwards it may go for the
// critical claw attack (em10ClawCriAtkCk) or, having lost the player, FindLost.
// FUNCTION: BIO4 0x00470440
// STATUS: stub
#if 0 // PARKED: C2105, EM10_CLAW_SWEEP_CK, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Mtx, Muku, PSMTXMultVec, SndCall, int, m, mat
void em10_R1_ClawAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    int r;

    if (em10FindCk2(em)) {
        w->Find_timer = 150;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x11C), PL_ARC_PTR(em->subArc, 0x11D), 10, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Find_timer = 0;
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->TmpU32 = 0;
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        if ((w->Timer && --w->Timer) || (em->Motion.Seq_old.Free & 8)) {
            f32 ang = (em->Motion.Seq_old.Free & 8) ? 0.049087387f : 0.19634955f;
            em->ang.y += Muku(&em->pos, &w->Pl_pos, em->ang.y, ang);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(8, 0x3D, &em->pos, em->id, 0, em);
        }
        if (em->Motion.Seq_old.Free & 3) {
            w->Atk_ck = 0;
            if (em->Motion.Seq_old.Free & 2) {
                v.x = -500.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                Mtx* m = &em->getPartsPtr(0x10)->mat;
                PSMTXMultVec(*m, &v, &v);
                EM10_CLAW_SWEEP_CK(-300.0f, 0x10);
                EM10_CLAW_SWEEP_CK(0.0f, 0x10);
                EM10_CLAW_SWEEP_CK(300.0f, 0x10);
                EM10_CLAW_SWEEP_CK(600.0f, 0x10);
            } else {
                v.x = 500.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                Mtx* m = &em->getPartsPtr(10)->mat;
                PSMTXMultVec(*m, &v, &v);
                EM10_CLAW_SWEEP_CK(300.0f, 10);
                EM10_CLAW_SWEEP_CK(0.0f, 10);
                EM10_CLAW_SWEEP_CK(-300.0f, 10);
                EM10_CLAW_SWEEP_CK(-600.0f, 10);
            }
            if (w->Atk_ck) {
                w->TmpU32++;
            }
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            if ((s16) w->Find_timer == 0) {
                EmRoutineSet(em, 1, 0x5D, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        } else if ((em->Motion.Seq_old.Free & 4) && w->Atk_ck == 0) {
            u8 rnd;
            if ((s16) w->Find_timer != 0 && (rnd = Rnd() % 10, rnd > 4)) {
                r = em10ClawCriAtkCk(em);
                if (r == 0) {
                    if ((em->pos.x - w->Pl_pos.x) * (em->pos.x - w->Pl_pos.x) + (em->pos.z - w->Pl_pos.z) * (em->pos.z - w->Pl_pos.z) > 16000000.0f &&
                        (em->r_no_0 != 1 || em->r_no_1 != 0x11)) {
                        em10CallVoiceSe2(em, 0x71, 6);
                        w->Route_type = 0;
                        EmRoutineSet(em, 1, 0x11, 0, 0);
                    }
                }
            } else {
                w->Find_timer = 0;
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Branch check of CSawWalkAtk (0x6D): the same obstacle / goto / return / RoofWait / Keeper checks as br_Walk.
// FUNCTION: BIO4 0x0045E6E0
// STATUS: stub
#if 0 // PARKED: DBG_EM_NO_ATK, DbgFlagChk, Muku, PI, l_pl
void em10_R1_br_CSawWalkAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0) {
        int one = 1; // kept in a callee-saved reg across the calls (docs/matching.md)
        if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
            EmRoutineSet(em, one, 0, 0, 0);
        } else if (!em10GotoCk(em) && !em10DoorOpenCk(em, 0) && !em10RackBreakCk(em) && !em10LadderClimbCk(em) && !em10VLadderClimbCk(em) && !em10LadderResetCk(em) && !em10JumpDownCk(em) && !em10JumpCk(em)) {
            em10ReturnStartPosCk(em);
            if (!em10ClimbOverCk(em) && !em10WindowCk(em)) {
                if ((em->flag & 0x400) && w->Goto_mode == 0 && w->HoseiCnt > 30 && fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) < 0.5235988f) {
                    EmRoutineSet(em, one, 0x1C, 0, 0);
                } else if (!em10IgnitionCk(em) && !em10ClawStickCK(em) && !em10FindLostCk(em)) {
                    if (w->Be_flg & 0x20000000) {
                        f32 dx = em->pos.x - w->Keep_pos.x;
                        f32 dz = em->pos.z - w->Keep_pos.z;
                        if (dx * dx + dz * dz < 4000000.0f) {
                            w->Be_flg &= ~0x20000000;
                            EmRoutineSet(em, 1, 1, 0, 0);
                            return;
                        }
                        if (em->l_pl < 4000000.0f && w->L_guard < em->Guard_r) {
                            w->Be_flg &= ~0x20000000;
                        }
                    }
                    em10GotoPosCk(em);
                }
            }
        }
    }
}
#endif // PARKED

// R1 == 0x6D CSawWalkAtk: the chainsaw Ganado walking with the saw held out (motion 0x2BC), steering
// towards Go_pos: the saw sweeps em10AtkCk every frame (kind 0xC, EM10_CSAW_SWEEP_CK), with the same
// Turn180 / Stay / death exits as Walk.
// FUNCTION: BIO4 0x0045E980
// STATUS: stub
#if 0 // PARKED: C1003, Game_level, int
void em10_R1_CSawWalkAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    int end;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2BC), PL_ARC_PTR(em->subArc, 0x2BD), 5, 5, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Timer = 20;
        if (pG->Game_level <= 3) {
            w->Timer = 5;
        }
        if (pG->Game_level > 6) {
            w->Timer = 30;
        }
        w->TmpU32 = 0;
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        w->TmpF2 = Muku(&em->pos, &w->Go_pos, em->ang.y, PI) * 0.3f;
        w->TmpF2 = Muku2(0.0f, w->TmpF2, 0.15707964f);
        em->ang.y += w->TmpF2;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        end = MotionMove(em, 0);
        if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
            em->hp = 0;
        }
        if (em->hp <= 0) {
            if (end) {
                EmRoutineSet(em, 2, 9, 0, 0);
            }
            break;
        }
        if (end || (em->Motion.Seq_old.Free & 4)) {
            if ((s16) pG->pl_life <= 0) {
                EmRoutineSet(em, 1, 0x1B, 0, 0);
                break;
            }
            if (pSUB && (s16) pG->ashley_life <= 0) {
                EmRoutineSet(em, 1, 0x1B, 0, 0);
                break;
            }
            if ((em->flag & 0x400) && w->Goto_mode == 0 && em->pos.y > pPL->pos.y + 500.0f) {
                if (fabsf(Muku(&em->pos, &pPL->pos, em->ang.y, PI)) > 2.7488937f && em->l_pl > 36000000.0f) {
                    EmRoutineSet(em, 1, 0x15, 0, 0);
                    break;
                }
            } else if (w->Go_rot > 2.7488937f && em->l_pl > 36000000.0f) {
                EmRoutineSet(em, 1, 0x15, 0, 0);
                break;
            }
            if (w->Atk_ck) {
                EmRoutineSet(em, 1, 0x1B, 0, 0);
                break;
            }
        }
        if (end) {
            if ((u8) (Rnd() % 10) == 0 && !Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BACKSIGN)) {
                em10CallVoiceSe2(em, w->Se_tbl[15], 8);
            }
        }
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(8, 0x3D, &em->pos, em->id, 0, em);
        }
        if (em->Motion.Seq_old.Free & 1) {
            Mtx* m;
            w->Atk_ck = 0;
            v.x = 500.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            m = &em->getPartsPtr(10)->mat;
            PSMTXMultVec(*m, &v, &v);
            EM10_CSAW_SWEEP_CK(300.0f);
            EM10_CSAW_SWEEP_CK(0.0f);
            EM10_CSAW_SWEEP_CK(-300.0f);
            EM10_CSAW_SWEEP_CK(-600.0f);
        }
        break;
    }
    em10BreathSe(em);
    em10HandSet(em, 0);
    em10CsawSignSe(em);
    em10BehindSeCk(em);
}
#endif // PARKED

// R1 == 0x2C ClawWalkAtk: the claw Ganado swiping while walking (motion 0x121), 10..15 frames of
// approach then the claw sweeps (kind 0xD); step 2/3 pulls the stuck claws out again (0x115).
// FUNCTION: BIO4 0x00470B80
// STATUS: stub
#if 0 // PARKED: C2105, EM10_CLAW_SWEEP_CK, Game_level, LIMIT_ANGLE, Mtx, Muku, Muku2, PI, PSMTXMultVec, SndCall, Timer2, int, m, mat
void em10_R1_ClawWalkAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v2;
    Vec v;
    int end;

    if (em->r_no_2 == 0 && (w->Claw_rno_l == 4 || w->Claw_rno_r == 4)) {
        em->r_no_2 = 2;
    }
    if (em10FindCk2(em)) {
        w->Find_timer = 150;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x121), PL_ARC_PTR(em->subArc, 0x122), 10, 5, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Find_timer = 0;
        w->Timer = (u8) (Rnd() % 5) + 10;
        w->TmpU32 = 0;
        w->Atk_ck = 0;
        em->r_no_2++;
    case 1:
        if ((em->pos.x - w->Pl_pos.x) * (em->pos.x - w->Pl_pos.x) + (em->pos.y - w->Pl_pos.y) * (em->pos.y - w->Pl_pos.y) +
                (em->pos.z - w->Pl_pos.z) * (em->pos.z - w->Pl_pos.z) <
            1000000.0f) {
            w->Timer = 0;
        } else {
            w->TmpF2 = Muku(&em->pos, &w->Go_pos, em->ang.y, PI) * 0.3f;
            w->TmpF2 = Muku2(0.0f, w->TmpF2, 0.19634955f);
            em->ang.y += w->TmpF2;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        end = MotionMove(em, 0);
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(8, 0x3D, &em->pos, em->id, 0, em);
        }
        if (em->Motion.Seq_old.Free & 3) {
            if (em->Motion.Seq_old.Free & 2) {
                Mtx* m;
                v.x = -500.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                m = &em->getPartsPtr(0x10)->mat;
                PSMTXMultVec(*m, &v, &v);
                EM10_CLAW_SWEEP_CK(-300.0f, 0);
                EM10_CLAW_SWEEP_CK(0.0f, 0);
                EM10_CLAW_SWEEP_CK(300.0f, 0);
                EM10_CLAW_SWEEP_CK(600.0f, 0);
            } else {
                Mtx* m;
                v.x = 500.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                m = &em->getPartsPtr(10)->mat;
                PSMTXMultVec(*m, &v, &v);
                EM10_CLAW_SWEEP_CK(300.0f, 0);
                EM10_CLAW_SWEEP_CK(0.0f, 0);
                EM10_CLAW_SWEEP_CK(-300.0f, 0);
                EM10_CLAW_SWEEP_CK(-600.0f, 0);
            }
        }
        if (end || (em->Motion.Seq_old.Free & 4)) {
            if (w->Atk_ck) {
                w->Timer = 0;
            }
            if (w->Timer) {
                w->Timer--;
            } else {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
                if ((s16) w->Find_timer == 0) {
                    EmRoutineSet(em, 1, 0x5D, 0, 0);
                } else {
                    em10WalkRtnSet(em);
                }
                break;
            }
            if (w->Go_rot > 1.9634955f) {
                EmRoutineSet(em, 1, 0x15, 0, 0);
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x115), 0, 3, 1, 0);
        w->Timer = 25;
        w->Timer2 = 46;
        SndCall(6, 0x71, &em->getPartsPtr(0)->world, 0, 0, em);
        em->r_no_2++;
    case 3:
        if (em->Motion.Seq_frame > 9.7f && em->Motion.Seq_frame < 10.3f) {
            w->Claw_rno_l = 1;
            w->Claw_rno_r = 1;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 0;
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Branch check of ClawCriAtk (0x2D): sweeps the claw points with attack kind 0xE (the decapitating
// critical); when it connected on a player with hp 0 (Leon / HUNK / Wesker in the overseas version)
// the player is kept at 1 hp and the ClawCriHit (0x2E) cut scene plays instead.
// FUNCTION: BIO4 0x0045EFD0
// STATUS: stub
#if 0 // PARKED: C2227, Mtx, PSMTXMultVec, m, mat, pSys, pl_life
void em10_R1_br_ClawCriAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    Vec p;
    Mtx* m;

    if (em->hp <= 0) {
        return;
    }
    if (em->r_no_2 != 5) {
        return;
    }
    if (!(em->Motion.Seq_old.Free & 1)) {
        return;
    }
    {
        p.x = 500.0f;
        p.y = 0.0f;
        p.z = 0.0f;
        m = &em->getPartsPtr(10)->mat;
        PSMTXMultVec(*m, &p, &p);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        PSMTXMultVec(*m, &v, &v);
        em10AtkCk(em, &v, &p, 0xE, 0);
        v.x = -300.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        PSMTXMultVec(*m, &v, &v);
        em10AtkCk(em, &v, &p, 0xE, 0);
        v.x = -600.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        PSMTXMultVec(*m, &v, &v);
        em10AtkCk(em, &v, &p, 0xE, 0);
        switch (pG->pl_type) {
        case 0:
        case 3:
        case 5:
            if (pSys->eff_country && w->Atk_ck && (s16) pG->pl_life <= 0) {
                pG->pl_life = 1;
                EmRoutineSet(em, 1, 0x2E, 0, 0);
            }
            break;
        }
    }
}
#endif // PARKED

// R1 == 0x2D ClawCriAtk: the claw Ganado's critical combo: wind-up (0x118, CriAtk_wait 450), the
// lunge (0x10B) turning towards Pl_pos while the way is clear, the overhead swipe (0x11E) with the
// wall hit sparks 0x7C, the claws stuck in the ground (0x11A, effect 0x7E, SEs 0x73/0x74), then walk.
// FUNCTION: BIO4 0x00471350
// STATUS: stub
#if 0 // PARKED: C1003, C2105, C2228, EatMgr, GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Mtx, Muku, PI, PSMTXInverse, PSMTXMultVec, SndCall, Timer2, atan2f, int, inv, m, mat
void em10_R1_ClawCriAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    Mtx inv;
    Vec lp;
    Vec hit;
    Vec nrm;
    Vec rot;

    if (em10FindCk2(em)) {
        w->Find_timer = 150;
    }
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x118), PL_ARC_PTR(em->subArc, 0x119), 10, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->Atk_ck = 0;
        w->CriAtk_wait = 0x1C2;
        w->Find_timer = 0;
        w->Timer = 80;
        w->Timer2 = 15;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_frame > 14.7f && em->Motion.Seq_frame < 15.3f) {
            if (w->Claw_rno_l != 2) {
                w->Claw_rno_l = 1;
            }
            if (w->Claw_rno_r != 2) {
                w->Claw_rno_r = 1;
            }
        }
        em->ang.y += Muku(&em->pos, &w->Pl_pos, em->ang.y, 0.3926991f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        if (w->Timer == 0) {
            PSMTXInverse(em->mat, inv);
            PSMTXMultVec(inv, &pPL->pos, &lp);
            if (lp.x > -300.0f && lp.x < 300.0f && lp.y > -500.0f && lp.y < 500.0f && lp.z > 0.0f && lp.z < 2500.0f) {
                MotionMove(em, 0);
                em->r_no_2 = 4;
                break;
            }
        } else {
            w->Timer--;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x10B), PL_ARC_PTR(em->subArc, 0x10C), 3, 5, 3);
        w->TmpU32 = 1;
        em->r_no_2++;
    case 3:
        if ((w->Pl_pos.x - em->pos.x) * (w->Pl_pos.x - em->pos.x) + (w->Pl_pos.y - em->pos.y) * (w->Pl_pos.y - em->pos.y) +
                    (w->Pl_pos.z - em->pos.z) * (w->Pl_pos.z - em->pos.z) >
                49000000.0f &&
            w->TmpU32 && w->Pl_rot < 0.5235988f) {
            em->ang.y += Muku(&em->pos, &w->Pl_pos, em->ang.y, 0.049087387f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        } else {
            w->TmpU32 = 0;
        }
        MotionMove(em, 0);
        a.x = 0.0f;
        a.y = 500.0f;
        a.z = 0.0f;
        b.x = 0.0f;
        b.y = 500.0f;
        b.z = 2800.0f;
        PSMTXMultVec(em->mat, &a, &a);
        PSMTXMultVec(em->mat, &b, &b);
        if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
            em->r_no_2 = 4;
            break;
        }
        PSMTXInverse(em->mat, inv);
        PSMTXMultVec(inv, &pPL->pos, &lp);
        if (lp.x > -300.0f && lp.x < 300.0f && lp.y > -500.0f && lp.y < 500.0f && lp.z > 0.0f && lp.z < 2500.0f) {
            em->r_no_2 = 4;
            break;
        }
        if (w->HoseiCnt > 5) {
            em->r_no_2 = 4;
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x11E), PL_ARC_PTR(em->subArc, 0x11F), 3, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        w->TmpU32 = 0;
        em->r_no_2++;
    case 5:
        if (em->Motion.Seq_old.Free & 0x20) {
            SndCall(8, 0x3D, &em->pos, em->id, 0, em);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            if ((s16) w->Find_timer == 0) {
                EmRoutineSet(em, 1, 0x5D, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        } else {
            if ((em->Motion.Seq_old.Free & 2) && w->TmpU32 == 0) {
                Mtx* m;
                a.x = 0.0f;
                a.y = 0.0f;
                a.z = -100.0f;
                b.x = -1800.0f;
                b.y = 0.0f;
                b.z = -100.0f;
                m = &em->getPartsPtr(10)->mat;
                PSMTXMultVec(*m, &a, &a);
                PSMTXMultVec(*m, &b, &b);
                rot = em->ang;
                rot.y += PI;
                rot.y = LIMIT_ANGLE(rot.y);
                if (EatMgr.hitCheck(&a, &b, &hit, &nrm, 0, 0x4000)) {
                    b = hit;
                    rot.y = atan2f(nrm.x, nrm.z);
                    EstSet(0, -1, &b, &rot, EFF_EM10, 0x7C, 0, ESP_CORE_KIND_NONE, 0, 0);
                    w->TmpU32 = 1;
                    SndCall(6, 0x72, &em->getPartsPtr(0)->world, 0, 0, em);
                }
            }
            if ((em->Motion.Seq_old.Free & 4) && w->TmpU32) {
                em->r_no_2 = 6;
            }
        }
        break;
    case 6:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x11A), PL_ARC_PTR(em->subArc, 0x11B), 3, 1, 0);
        em10CallVoiceSe2(em, w->Se_tbl[12], 8);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x7E, 0, ESP_CORE_KIND_NONE, em, 0);
        w->Timer = 120;
        em->r_no_2++;
    case 7:
        if (w->Timer) {
            w->Timer--;
            em->atari.m_flag |= 8;
            w->No_adj_timer = 2;
        }
        if (em->Motion.Seq_old.Free & 1) {
            SndCall(6, 0x73, &em->getPartsPtr(0)->world, 0, 0, em);
        }
        if (em->Motion.Seq_old.Free & 4) {
            SndCall(6, 0x74, &em->getPartsPtr(0)->world, 0, 0, em);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            if (w->Atk_ck) {
                w->Atk_wait = 15;
                if (pG->Game_level <= 3) {
                    w->Atk_wait = 45;
                }
                if (pG->Game_level <= 1) {
                    w->Atk_wait = 90;
                }
            }
            if (em->l_pl > 6250000.0f) {
                EmRoutineSet(em, 1, 0x5D, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x2E ClawCriHit: the decapitation cut scene on the Ganado side (motion 0x120, effect 0x8C):
// kills the player at frame 124; plem10_ClawCriHit runs the player half.
// FUNCTION: BIO4 0x0043EA20
// STATUS: stub
#if 0 // PARKED: C2105, Catch_at_adj, DiedemoExec, EmCatchMotionMove, EmCatchPLSet, int, pl_life
void em10_R1_ClawCriHit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x120), 0, 10, 1, 0);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x8C, 0, ESP_CORE_KIND_NONE, em, 0);
        EmCatchPLSet(em, 0.0f, 2, 241.15f, 0.0f, 975.16f, plem10_ClawCriHit);
        w->Timer = 10;
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        em->r_no_2++;
    case 1:
        if (MOTION(em)->Seq_frame > 123.7f && MOTION(em)->Seq_frame < 124.3f) {
            w->Claw_rno_l = 1;
            pG->pl_life = 0;
            DiedemoExec(2, 0);
        }
        if (w->Timer) {
            w->Timer--;
            EmCatchMotionMove(em, 0.3f, 0.2f);
        } else {
            MotionMove(em, 0);
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x2F C_SawAtk: the chainsaw swing (motion 0xE9, saw SE 0x50 + roar 0x3D) steering towards
// Go_pos; a miss goes back to the walk; the swing also breaks doors / racks in the way (kind 2).
// FUNCTION: BIO4 0x00471F30
// STATUS: stub
#if 0 // PARKED: C2105, GameAddPoint, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, SndCall, SndStop, int
void em10_R1_C_SawAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xE9), PL_ARC_PTR(em->subArc, 0xEA), 10, flag, 0x1E);
        w->Atk_ck = 0;
        w->Csaw_regist = 0;
        w->Timer = 5;
        SndStop(w->Seid_csaw, 0);
        w->Seid_csaw = SndCall(6, 0x50, &em->pos, 0, 0, em);
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        w->Seid_voice = SndCall(6, 0x3D, &em->pos, 0, 0, em);
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_old.Free & 1) {
            em10SetDamageDoor(em, 2);
            em10SetDamageRack(em, 2);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x30 C_SawHit: the chainsaw caught the player: the saw goes into his neck (0xFB, blood
// effect 0x50, the escape button mash PlGacha), locks the other Ganados (ctrl12 ATK / THROW /
// NOT_NEAR) and holds the camera (em10CamMoveCri); the player dies when the mash fails (pl_life 0,
// plem10_C_SawHit shows the decapitation) or breaks free (0xFE / 0xFD) with Atk_wait by rank.
// FUNCTION: BIO4 0x004721A0
// STATUS: stub
#if 0 // PARKED: C2059, C2105, C2227, CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_NOT_NEAR, CTRL12_ID_EM10_THROW, Catch_at_adj, Ctrl12Set, EmCatchMotionMove, EmCatchPLSet, EstSet, GameAddPoint, Game_level, LVADD_PL_DAMAGE, PSMTXMultVec, PlGachaGet, PlGachaInit, PlSetDamageSe, SndCall, SndStop, Timer2, Timer3, VibDataTbl, VibSetData, int, mat, pSys, pl_life, set
void em10_R1_C_SawHit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    int flag;

    w->Be_flg |= 0x800;
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 30);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 120);
    em->dmg.set(0, 2);
    switch (em->r_no_2) {
    case 0:
        if (pG->pl_type != 2) {
            EmCatchPLSet(em, 0.0f, 2, -15.17f, 0.0f, 853.48f, plem10_C_SawHit);
        } else {
            EmCatchPLSet(em, 0.0f, 2, 8.56f, 0.0f, 520.49f, plem10_C_SawHit);
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xFB), 0, 5, 1, 0);
        w->Timer2 = 0;
        PlSetDamageSe(0);
        SndStop(w->Seid_csaw, 0);
        w->TmpU32 = SndCall(6, 0x4F, &em->pos, 0, 0, em);
        if (pSys->eff_country == 0 && (u8) (Rnd() % 10) > 4) {
            em->r_no_3 = 1;
        } else {
            em->r_no_3 = 3;
        }
        PlGachaInit();
        w->Timer = 19;
        w->Timer2 = 41;
        GameAddPoint(LVADD_PL_DAMAGE);
        w->Timer3 = 0;
        VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 0xD, 1);
        em->r_no_2++;
    case 1:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        if (w->pWeapon) {
            if (w->Timer3) {
                w->Timer3--;
            } else {
                w->Timer3 = 2;
                EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x50, 0, ESP_CORE_KIND_NONE, w->pWeapon, 0);
            }
        }
        if (w->Timer) {
            w->Timer--;
        } else if (w->Timer2) {
            w->Timer2--;
        } else {
            pG->pl_life = 0;
        }
        if ((s16) pG->pl_life > 0) {
            if (w->Timer2) {
                w->Timer2--;
            } else {
                w->Timer2 = 10;
                if (w->pWeapon) {
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 250.0f;
                    PSMTXMultVec(w->pWeapon->mat, &v, &v);
                    EstSet(0, -1, &v, &em->ang, EFF_EM10, 0x10, 0, ESP_CORE_KIND_NONE, 0, 0);
                }
            }
        } else {
            em->dmg.m_Timer = 2;
            em->r_no_2 = 4;
            SndStop(w->TmpU32, 0);
            SndCall(6, 0x54, &em->pos, 0, 0, em);
            break;
        }
        if (EmCatchMotionMove(em, 0.3f, 0.2f) || (u32) PlGachaGet() > 30) {
            em->dmg.m_Timer = 2;
            em->r_no_2++;
            SndStop(w->TmpU32, 0);
            SndCall(6, 0x54, &em->pos, 0, 0, em);
        }
        break;
    case 2:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        em->atari.m_flag &= ~8;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xFE), PL_ARC_PTR(em->subArc, 0xFF), 5, flag, 0);
        em->r_no_2++;
    case 3:
        em->dmg.m_Timer = 2;
        w->Be_flg &= ~0x800;
        if (MotionMove(em, 0)) {
            w->Atk_wait = 15;
            if (pG->Game_level <= 3) {
                w->Atk_wait = 45;
            }
            if (pG->Game_level <= 1) {
                w->Atk_wait = 90;
            }
            em10WalkRtnSet(em);
        }
        break;
    case 4:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        em->atari.m_flag &= ~8;
        w->Timer = 30;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xFD), 0, 5, flag, 0);
        em->r_no_2++;
    case 5:
        em->dmg.m_Timer = 2;
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Be_flg &= ~0x800;
        }
        MotionMove(em, 0);
        break;
    }
    em->Catch_at_adj = em->pos;
    if (w->Be_flg & 0x800) {
        em10CamMoveCri(em, em->r_no_3, 1);
    }
    w->Be_flg |= 0x2000;
    em10SetCrash(em, 800.0f);
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x31 C_SawCriAtk: the chainsaw overhead critical swing (motion 0xEB, saw SE 0x50) turning to
// the player; a miss returns to the walk.
// FUNCTION: BIO4 0x004729B0
// STATUS: stub
#if 0 // PARKED: C2105, GameAddPoint, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, SndCall, SndStop, int
void em10_R1_C_SawCriAtk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    if (w->Be_flg & 0x100) {
        w->Be_flg |= 0x40000;
    }
    switch (em->r_no_2) {
    case 0:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xEB), PL_ARC_PTR(em->subArc, 0xEC), 10, flag, 0);
        w->Atk_ck = 0;
        w->Csaw_regist = 0;
        w->Timer = 5;
        SndStop(w->Seid_csaw, 0);
        w->Seid_csaw = SndCall(6, 0x50, &em->pos, 0, 0, em);
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        w->Seid_voice = SndCall(6, 0x3D, &em->pos, 0, 0, em);
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            if (w->Atk_ck == 0) {
                GameAddPoint(LVADD_ESCAPEATTACK);
            }
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x32 C_SawCriHit: the chainsaw critical connected: the one-shot decapitation (motion 0x100,
// no escape) with the blood effect 0x4F and the critical camera, then Stay.
// FUNCTION: BIO4 0x0044F8A0
// STATUS: stub
#if 0 // PARKED: C1003, C2059, C2105, C2227, CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_NOT_NEAR, CTRL12_ID_EM10_THROW, Ctrl12Set, EmCatchPLSet, GameAddPoint, LVADD_PL_DAMAGE, SndCall, SndStop, Timer2, Timer3, VibDataTbl, VibSetData, int, pSys
void em10_R1_C_SawCriHit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Be_flg |= 0x800;
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 30);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 120);
    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x100), 0, 5, 1, 0);
        if (pG->pl_type != 2) {
            EmCatchPLSet(em, 0.0f, 2, 24.27f, 0.0f, 805.87f, plem10_C_SawCriHit);
        } else {
            EmCatchPLSet(em, 0.0f, 2, 24.27f, 0.0f, 805.87f, plem10_C_SawCriHit);
        }
        w->Timer2 = 0;
        SndStop(w->Seid_csaw, 0);
        w->TmpU32 = SndCall(6, 0x4F, &em->pos, 0, 0, em);
        em->r_no_3 = 1;
        w->Timer3 = 0;
        w->Timer = 18;
        GameAddPoint(LVADD_PL_DAMAGE);
        if (pSys->eff_country == 0) {
            SndCall(6, 0x5C, &pPL->pos, 0, 0, pPL);
        }
        VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 0xD, 1);
        em->r_no_2++;
    case 1:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        if (w->Timer) {
            w->Timer--;
            if (w->pWeapon) {
                if (w->Timer3) {
                    w->Timer3--;
                } else {
                    w->Timer3 = 2;
                    EstSet(w->pWeapon, -1, 0, 0, EFF_EM10, 0x4F, 0, ESP_CORE_KIND_NONE, w->pWeapon, 0);
                }
            }
        }
        if (em->Motion.Seq_frame > 17.7f && em->Motion.Seq_frame < 18.3f) {
            SndStop(w->TmpU32, 0);
            SndCall(6, 0x54, &em->pos, 0, 0, em);
        }
        if (EmCatchMotionMove(em, 0.3f, 0.2f)) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    if (w->Be_flg & 0x800) {
        if (w->Timer) {
            em10CamMoveCri(em, em->r_no_3, 1);
        } else {
            em10CamMoveCri(em, em->r_no_3, 0);
        }
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// Branch check of Catch (0x33): on the catch frame (Motion.Seq_old.Free bit1) em10CatchCk decides the grab:
// a dynamite Ganado with the fuse lit goes to Bombhold (0x38), one behind the player with others
// around or Ashley present to Backhold (0x37), else NeckHang (0x34); em10CatchSubCk grabs the partner.
// FUNCTION: BIO4 0x0044FC70
// STATUS: stub
#if 0 // PARKED: Muku2, PI
void em10_R1_br_Catch(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0 && (em->Motion.Seq_old.Free & 2)) {
        if (em10CatchCk(em)) {
            if (pG->pl_type == 1) {
                EmRoutineSetW(em, 1, 0x36, 0, 0);
            } else if ((fabsf(Muku2(em->ang.y, pPL->ang.y, PI)) < 1.5707964f && (em10SomebodyNearCk(em) || pSUB)) || w->Wep_type == 9) {
                if (w->Wep_type == 9 && w->Fire_timer) {
                    EmRoutineSetW(em, 1, 0x38, 0, 0);
                } else {
                    EmRoutineSetW(em, 1, 0x37, 0, 0);
                }
            } else {
                EmRoutineSetW(em, 1, 0x34, 0, 0);
            }
        } else {
            em10CatchSubCk(em);
        }
    }
}
#endif // PARKED

// R1 == 0x33 Catch: the grab attempt: lunge motion 0x86 / 0x88 / 0x8A picked by the angle to the
// target (player or Ashley), turning with the TmpF limit; on a miss back to the walk (escape point).
// FUNCTION: BIO4 0x00472BF0
// STATUS: stub
#if 0 // PARKED: GameAddPoint, Game_level, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, PI
void em10_R1_Catch(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    f32 a;

    switch (em->r_no_2) {
    case 0: {
        f32 ang;
        if (em->r_no_3 && pSUB) {
            ang = Muku(&em->pos, &pSUB->pos, em->ang.y, PI);
        } else {
            ang = Muku(&em->pos, &pPL->pos, em->ang.y, PI);
        }
        a = fabsf(ang);
        if (a < 1.3089969f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x86), PL_ARC_PTR(em->subArc, 0x87), 10, 1, 0);
            w->TmpU32 = 0;
        } else if (a < 1.9634955f) {
            if (ang < 0.0f) {
                w->TmpU32 = 1;
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x88), PL_ARC_PTR(em->subArc, 0x89), 10, 0x41, 0);
            } else {
                w->TmpU32 = 2;
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x88), PL_ARC_PTR(em->subArc, 0x89), 10, 1, 0);
            }
        } else {
            w->TmpU32 = 3;
            if (ang < 0.0f) {
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x8A), PL_ARC_PTR(em->subArc, 0x8B), 10, 0x41, 0);
            } else {
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x8A), PL_ARC_PTR(em->subArc, 0x8B), 10, 1, 0);
            }
        }
        w->TmpF = em->ang.y;
        switch ((u32) w->TmpU32) {
        case 0:
        default:
            w->TmpF = em->ang.y;
            break;
        case 1:
            w->TmpF = em->ang.y - 1.5707964f;
            break;
        case 2:
            w->TmpF = em->ang.y + 1.5707964f;
            break;
        case 3:
            w->TmpF = em->ang.y + PI;
            break;
        }
        w->TmpF = LIMIT_ANGLE(w->TmpF);
        w->TmpF2 = 0.31415927f;
        if (pG->Game_level <= 3) {
            w->TmpF2 = 0.10471976f;
        }
        em10CallVoiceSe2(em, w->Se_tbl[5], 8);
        em->r_no_2++;
    }
    case 1:
        if (em->Motion.Seq_old.Free & 8) {
            if (em->r_no_3 && pSUB) {
                a = Muku(&em->pos, &pSUB->pos, w->TmpF, w->TmpF2);
            } else {
                a = Muku(&em->pos, &pPL->pos, w->TmpF, w->TmpF2);
            }
            w->TmpF += a;
            w->TmpF = LIMIT_ANGLE(w->TmpF);
            em->ang.y += a;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
                em->hp = 0;
            }
            if (em->hp <= 0) {
                EmRoutineSet(em, 2, 9, 0, 0);
            } else {
                GameAddPoint(LVADD_ESCAPEATTACK);
                em10WalkRtnSet(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x34 NeckHang: the front strangle hold on the player (motion 0x28F, damage hold SE 0x85):
// drains the player's hp every frame (em10GetPower rate) while he mashes the button (PlGacha); he
// either dies (pl_life 0), or throws the Ganado off (0x290 / 0x292: kick to the head, damage on the
// Ganado, a mash above 30 lands a critical) into DownWakeWait, with the cut-in camera em10CamMove.
// FUNCTION: BIO4 0x00473050
// STATUS: stub
#if 0 // PARKED: C2105, CTRL12_ID_EM10_NOT_NEAR, Catch_at_adj, Ctrl12Set, EmCatchMotionMove, EmCatchPLSet, GameAddPoint, LVADD_CRITICALHIT, LVADD_PL_DAMAGE, LifeDownSet2, PlGachaGet, PlGachaInit, PlGachaMove, PlSetDamageSe, QuakeExec, SYS_HARD_MODE, SndCall, SndStop, SysFlagChk, Timer2, Timer3, int, pl_life, room_id, set, shooting_mode
void em10_R1_NeckHang(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int dmg;

    w->Be_flg |= 0x800;
    em10SetAtkWait(em, 1);
    switch (em->r_no_2) {
    case 0:
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x28F), 0, 5, 1, 0);
        PlSetDamageSe(0);
        if (pG->pl_type != 2) {
            EmCatchPLSet(em, 0.0f, 2, -180.0f, 0.0f, 470.18f, plem10_NeckHang);
        } else {
            EmCatchPLSet(em, 0.0f, 2, -172.81f, 0.0f, 428.66f, plem10_NeckHang);
        }
        em->dmg.set(0, 0);
        w->Timer = 0xF;
        w->Timer2 = 0x28;
        w->Timer3 = (s16) pG->pl_life;
        SndCall(8, 0x85, &em->pos, em->id, 0, em);
        w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        em->r_no_3 = 0;
        PlGachaInit();
        GameAddPoint(LVADD_PL_DAMAGE);
        em->r_no_2++;
    case 1:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        em10CamMove(em, em->r_no_3, 0.1f, 0);
        PlGachaMove();
        if (em->Motion.Seq_frame > 43.7f && em->Motion.Seq_frame < 44.3f) {
            SndStop(w->TmpU32, 0);
            w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        }
        dmg = 10;
        dmg *= em10GetPower(em);
        if ((em->flag & 4) && !SysFlagChk(pG, SYS_HARD_MODE)) {
            dmg = dmg / 2 + 1;
        }
        LifeDownSet2(pPL, dmg, 0, 1);
        if (EmCatchMotionMove(em, 0.3f, 0.2f) || (em->Motion.Seq_frame > 34.7f && em->Motion.Seq_frame < 35.3f)) {
            em->dmg.m_Timer = 2;
            if ((u32) PlGachaGet() <= 9 || ((s16) pG->pl_life <= 1 && w->Timer3 <= 0xC7)) {
                if ((s16) pG->pl_life <= 1) {
                    pG->pl_life = 0;
                }
                em->r_no_2 = 2;
            } else {
                em->r_no_2 = 4;
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x290), PL_ARC_PTR(em->subArc, 0x291), 5, 1, 0);
        w->Timer = 0x30;
        SndStop(w->TmpU32, 0);
        em10CallVoiceSe2(em, w->Se_tbl[13], 8);
        if (pG->room_id != 0x21B) {
            em10SetCampos2(em);
        }
        em->r_no_2++;
    case 3: {
        int r;
        em->dmg.m_Timer = 2;
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        if (w->Timer) {
            w->Timer--;
            r = EmCatchMotionMove(em, 0.3f, 0.2f);
            if (w->Timer == 0) {
                QuakeExec(0, 0, 10, 8.0f, 2);
            }
        } else {
            r = MotionMove(em, 0);
        }
        if (r) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            if ((w->Be_flg & 0x80) && w->Die_wait == 0 && em->hp == 1) {
                em->hp = 0;
            }
            if (em->hp <= 0) {
                EmRoutineSet(em, 2, 9, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        } else if (pG->room_id != 0x21B) {
            em10CamMove2(em);
        } else {
            em10CamMove(em, em->r_no_3, 0.1f, 0);
        }
        break;
    }
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x292), PL_ARC_PTR(em->subArc, 0x293), 5, 1, 0);
        w->Timer = 0xE;
        if (pG->pl_type == 2) {
            EmCatchPLSet(em, 0.0f, 2, -280.09f, 0.0f, 364.05f, plem10_NeckHang);
            pPL->r_no_2 = 4;
        }
        SndStop(w->TmpU32, 0);
        dmg = 100;
        em->r_no_3 = 1;
        w->Timer = 0x37;
        if (!(w->Be_flg & 0x80)) {
            if ((u32) PlGachaGet() > 0x1E) {
                dmg = 9999;
            }
            if ((u32) PlGachaGet() > 0x14 && (Rnd() & 1)) {
                dmg = 9999;
            }
            if (pG->shooting_mode) {
                dmg = 9999;
            }
            if (w->Be_flg & 0x80) {
                dmg = 0;
            }
            if (dmg == 9999) {
                GameAddPoint(LVADD_CRITICALHIT);
            }
        }
        LifeDownSet2(em, dmg, 0, 0);
        if (em->hp <= 0) {
            SndCall(1, 0x35, &pPL->pos, 0, 0, pPL);
        } else {
            SndCall(1, 0x3D, &pPL->pos, 0, 0, pPL);
            EstSet(em, -1, 0, 0, EFF_EM10, 0x4B, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        EstSet(pPL, -1, 0, 0, EFF_EM10, 0x4C, 0, ESP_CORE_KIND_NONE, pPL, 0);
        pPL->dmg.m_Timer = 2;
        w->Be_flg |= 0x20;
        em->r_no_2++;
    case 5:
        em->dmg.m_Timer = 2;
        if (w->Timer) {
            w->Timer--;
            em10CamMove(em, em->r_no_3, 1.0f, 0);
        } else {
            w->Be_flg &= ~0x800;
            w->Be_flg |= 0x2000;
        }
        if (MotionMove(em, 0)) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            em10SetAtkWait(em, 1);
            w->Be_flg |= 0x20;
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else {
            if (em->Motion.Seq_old.Free & 2) {
                em10SetDmWaterEff(em, 1);
            }
            if (em->Motion.Seq_old.Free & 1) {
                if (!(w->Be_flg & 0x80) && em->hp <= 0 && !em10ChgParasiteCk(em)) {
                    em10LostHead(em, 0, 1);
                }
                SndCall(1, 0x3A, &pPL->pos, 0, 0, pPL);
                SndCall(1, 0x3B, &pPL->pos, 0, 0, pPL);
                w->Se_no = w->Se_tbl[8];
                em10SetDamageVoice(em, w->Se_no, w->Se_tbl[0]);
            }
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 1);
    if (em->Motion.Seq_old.Free & 0x10) {
        em10SetCrash(em, 800.0f);
    }
}
#endif // PARKED

// R1 == 0x35 NeckHang_Luis: strangles Luis (pSUB) instead: motion 0x28F with subem10_NeckHang_Luis on
// the partner (EmCatchSubSet), then the throw-off 0x292 and DownWakeWait; no button mash.
// FUNCTION: BIO4 0x0043FDC0
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_EM10_NOT_NEAR, Catch_at_adj, Ctrl12Set, EmCatchMotionMove, EmCatchSubSet, SndCall, SndStop, Timer2, int, set
void em10_R1_NeckHang_Luis(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Be_flg |= 0x800;
    em10SetAtkWait(em, 1);
    switch (em->r_no_2) {
    case 0:
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x28F), 0, 5, 1, 0);
        EmCatchSubSet(em, pSUB, 0.0f, 2, -180.0f, 0.0f, 470.18f, subem10_NeckHang_Luis);
        em->dmg.set(0, 0);
        w->Timer = 0xF;
        w->Timer2 = 0x28;
        SndCall(8, 0x85, &em->pos, em->id, 0, em);
        w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        em->r_no_2++;
    case 1:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        if (em->Motion.Seq_frame > 43.7f && em->Motion.Seq_frame < 44.3f) {
            SndStop(w->TmpU32, 0);
            w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        }
        if (EmCatchMotionMove(em, 0.3f, 0.2f)) {
            em->r_no_2 = 2;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x292), PL_ARC_PTR(em->subArc, 0x293), 5, 1, 0);
        w->Timer = 0xE;
        SndStop(w->TmpU32, 0);
        w->Be_flg |= 0x20;
        em->r_no_2++;
    case 3:
        if (MotionMove(em, 0)) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            em10SetAtkWait(em, 1);
            w->Be_flg |= 0x20;
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else if (em->Motion.Seq_old.Free & 1) {
            SndCall(1, 0x3A, &pPL->pos, 0, 0, pPL);
            SndCall(1, 0x3B, &pPL->pos, 0, 0, pPL);
            w->Se_no = w->Se_tbl[8];
            em10SetDamageVoice(em, w->Se_no, w->Se_tbl[0]);
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 1);
    if (em->Motion.Seq_old.Free & 0x10) {
        em10SetCrash(em, 800.0f);
    }
}
#endif // PARKED

// R1 == 0x36 NeckHang_Ashley: the strangle hold when Ashley is the player (pl_type 1): motion 0x1A8
// with the Ashley camera (em10CamMoveAshley), her hp drains while she mashes; the throw-off 0x1A9,
// then DownWakeWait.
// FUNCTION: BIO4 0x00440300
// STATUS: stub
#if 0 // PARKED: C1003, CTRL12_ID_EM10_NOT_NEAR, Ctrl12Set, EmCatchPLSet, GameAddPoint, LVADD_PL_DAMAGE, PlGachaInit, PlSetFace, SndCall, SndStop, Timer2, Timer3, int, pl_life, set
void em10_R1_NeckHang_Ashley(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int dmg;
    int r;

    w->Be_flg |= 0x800;
    em10SetAtkWait(em, 1);
    switch (em->r_no_2) {
    case 0:
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1A8), 0, 5, 1, 0);
        PlSetFace(1);
        EmCatchPLSet(em, 0.0f, 2, -150.33f, 0.0f, 415.26f, (void (*)(cPlayer*)) subem10_NeckHang_Ashley);
        em->dmg.set(0, 0);
        w->Timer2 = 0x28;
        w->Timer = 0xF;
        w->Timer3 = (s16) pG->pl_life;
        SndCall(8, 0x85, &em->pos, em->id, 0, em);
        w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        SndCall(1, 7, &em->pos, 0, 0, em);
        em->r_no_3 = 0;
        PlGachaInit();
        GameAddPoint(LVADD_PL_DAMAGE);
        w->Timer = 0x28;
        em->r_no_2++;
    case 1:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 5);
        em10CamMove(em, em->r_no_3, 0.1f, 0);
        if (em->Motion.Seq_frame > 39.7f && em->Motion.Seq_frame < 40.3f) {
            SndStop(w->TmpU32, 0);
            w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            PlGachaMove();
            dmg = 10;
            if ((em->flag & 4) && !SysFlagChk(pG, SYS_HARD_MODE)) {
                dmg = 6;
            }
            LifeDownSet2(pPL, dmg, 0, 0);
        }
        if (EmCatchMotionMove(em, 0.3f, 0.2f) || (u32) PlGachaGet() > 10 || (s16) pG->pl_life <= 0) {
            em->dmg.m_Timer = 2;
            if ((s16) pG->pl_life <= 0) {
                em->r_no_2 = 2;
            } else {
                em->r_no_2 = 4;
            }
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1AB), 0, 5, 1, 0);
        SndStop(w->TmpU32, 0);
        em10CallVoiceSe2(em, w->Se_tbl[13], 8);
        SndCall(1, 0xD, &em->pos, 0, 0, em);
        em->r_no_3 = Rnd() & 1;
        em->r_no_2++;
    case 3:
        em10CamMoveAshley(em, em->r_no_3);
        em->dmg.m_Timer = 2;
        MotionMove(em, 0);
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x1A9), PL_ARC_PTR(em->subArc, 0x1AA), 5, 1, 0);
        w->Timer = 0xE;
        SndStop(w->TmpU32, 0);
        em->r_no_3 = (Rnd() & 1) + 1;
        w->Timer = 0x37;
        SndCall(1, 0x35, &em->pos, 0, 0, em);
        pPL->dmg.m_Timer = 2;
        em->r_no_2++;
    case 5:
        em->dmg.m_Timer = 2;
        if (w->Timer) {
            w->Timer--;
            em10CamMoveAshley(em, em->r_no_3);
        }
        if (w->Timer) {
            w->Timer--;
            r = EmCatchMotionMove(em, 0.3f, 0.2f);
        } else {
            r = MotionMove(em, 0);
        }
        if (r) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            em10SetAtkWait(em, 1);
            w->Be_flg |= 0x20;
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else {
            if (em->Motion.Seq_old.Free & 2) {
                em10SetDmWaterEff(em, 1);
            }
            if (em->Motion.Seq_old.Free & 1) {
                SndCall(1, 0x3A, &em->pos, 0, 0, em);
            }
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 1);
    if (em->Motion.Seq_old.Free & 0x10) {
        em10SetCrash(em, 800.0f);
    }
}
#endif // PARKED

// R1 == 0x37 Backhold: grabs the player from behind (motion 0x294) and holds him for the other
// Ganados (Status_flg[1] bit13 = held): the button mash (PlGacha) frees him with the elbow / throw
// 0x295 (damage on the Ganado, critical over 40), the hold ends by itself into the release 0x57.
// FUNCTION: BIO4 0x00473CE0
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_NOT_NEAR, CTRL12_ID_EM10_THROW, Catch_at_adj, Ctrl12Set, EmCatchMotionMove, EmCatchPLSet, GameAddPoint, LVADD_CRITICALHIT, LifeDownSet2, PI, PlGachaGet, PlGachaInit, PlGachaMove, PlSetDamageSe, STA_PL_CATCHED, STA_PL_CATCHHOLD, SndCall, SndStop, StaFlagChk, StaFlagOn, Timer2, int, set, shooting_mode
void em10_R1_Backhold(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* p = pPL->getPartsPtr(4);
    int dmg;

    w->Be_flg |= 0x800;
    em10SetAtkWait(em, 0);
    switch (em->r_no_2) {
    case 0:
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 0);
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 0);
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 0);
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x294), 0, 5, 1, 0);
        PlSetDamageSe(0);
        if (pG->pl_type != 2) {
            EmCatchPLSet(em, PI, 2, 178.63f, 0.0f, -190.03f, plem10_Backhold);
        } else {
            EmCatchPLSet(em, PI, 2, 178.53f, 0.0f, -190.03f, plem10_Backhold);
        }
        StaFlagOn(pG, STA_PL_CATCHED);
        em->dmg.set(0, 0);
        w->Timer = 0xF;
        if (w->Fire_timer) {
            w->Fire_timer = 0x1E;
        }
        SndCall(8, 0x85, &p->world, em->id, 0, pPL);
        w->TmpU32 = SndCall(8, 0x85, &p->world, em->id, 0, pPL);
        em->r_no_3 = 0;
        PlGachaInit();
        StaFlagOn(pG, STA_PL_CATCHHOLD);
        w->Timer2 = 0x2D;
        em->r_no_2++;
    case 1:
        em10CamMove(em, em->r_no_3, 0.1f, 0);
        PlGachaMove();
        if (w->Timer2) {
            w->Timer2--;
        }
        if (EmCatchMotionMove(em, 0.3f, 0.2f) || w->Timer2 == 0) {
            em->dmg.m_Timer = 2;
            em->r_no_2 = 2;
        } else if (!StaFlagChk(pG, STA_PL_CATCHHOLD)) {
            em->r_no_2 = 4;
        }
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x295), PL_ARC_PTR(em->subArc, 0x296), 5, 1, 0);
        dmg = 100;
        SndStop(w->TmpU32, 0);
        if (!(w->Be_flg & 0x80)) {
            if ((u32) PlGachaGet() > 0x28) {
                dmg = 9999;
            }
            if ((u32) PlGachaGet() > 0x1E && (Rnd() & 1)) {
                dmg = 9999;
            }
            if (pG->shooting_mode) {
                dmg = 9999;
            }
            if (w->Be_flg & 0x80) {
                dmg = 0;
            }
            if (dmg == 9999) {
                GameAddPoint(LVADD_CRITICALHIT);
            }
        }
        LifeDownSet2(em, dmg, 0, 0);
        if (em->hp <= 0) {
            SndCall(1, 0x35, &p->world, 0, 0, pPL);
        } else {
            SndCall(1, 0x3D, &p->world, 0, 0, pPL);
            EstSet(em, -1, 0, 0, EFF_EM10, 0x4A, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        EstSet(pPL, -1, 0, 0, EFF_EM10, 0x4C, 0, ESP_CORE_KIND_NONE, pPL, 0);
        pPL->dmg.m_Timer = 2;
        em->r_no_2++;
    case 3:
        em->dmg.m_Timer = 2;
        if (MotionMove(em, 0)) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            em10SetAtkWait(em, 1);
            w->Be_flg &= ~0x20;
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else {
            if (em->Motion.Seq_old.Free & 2) {
                em10SetDmWaterEff(em, 1);
            }
            if (em->Motion.Seq_old.Free & 1) {
                if (!(w->Be_flg & 0x80) && em->hp <= 0 && !em10ChgParasiteCk(em)) {
                    em10LostHead(em, 0, 1);
                }
                SndCall(1, 0x3A, &p->world, 0, 0, pPL);
                SndCall(1, 0x3B, &p->world, 0, 0, pPL);
                w->Se_no = w->Se_tbl[8];
                em10SetDamageVoice(em, w->Se_no, w->Se_tbl[0]);
            }
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x57), PL_ARC_PTR(em->subArc, 0x58), 5, 1, 0);
        SndStop(w->TmpU32, 0);
        w->Be_flg &= ~0x800;
        em->atari.m_flag &= ~8;
        em->r_no_2++;
    case 5:
        if (MotionMove(em, 0)) {
            em10SetAtkWait(em, 0);
            em10WalkRtnSet(em);
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 1);
    if (em->Motion.Seq_old.Free & 0x10) {
        em10SetCrash(em, 800.0f);
    }
}
#endif // PARKED

// R1 == 0x38 Bombhold: the dynamite Ganado grabs the player from behind (0x294) with the lit stick:
// if the fuse runs out the bomb goes off killing both (weapon lost, core broken, player hp 0 via
// plem10_Bombhold), a mash above 15 breaks free (0x295) into DownWakeWait; the Ganado hides itself
// after the blast (flag 0x400000).
// FUNCTION: BIO4 0x0044FDD0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, Camera, EM_STATUS_ITEMSET, EmCatchMotionMove, EmCatchPLSet, EmReserveDropItem, EmSetDie, PI, PlGachaGet, PlGachaInit, PlGachaMove, PlSetDamageSe, STA_PL_CATCHED, STA_PL_CATCHHOLD, SndCall, SndStop, StaFlagOn, Timer2, cam, int, set, setLost, setReset
void em10_R1_Bombhold(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* p;
    Camera* cam;
    Vec rot;

    w->Be_flg |= 0x800;
    em10SetAtkWait(em, 1);
    switch (em->r_no_2) {
    case 0:
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x294), 0, 5, 1, 0);
        PlSetDamageSe(0);
        if (pG->pl_type != 2) {
            EmCatchPLSet(em, PI, 2, 178.63f, 0.0f, -190.03f, plem10_Bombhold);
        } else {
            EmCatchPLSet(em, PI, 2, 178.53f, 0.0f, -190.03f, plem10_Bombhold);
        }
        StaFlagOn(pG, STA_PL_CATCHED);
        em->dmg.set(0, 0);
        w->Timer = 0xF;
        if (w->Fire_timer) {
            w->Fire_timer = 0x78;
        }
        SndCall(8, 0x85, &em->pos, em->id, 0, em);
        w->TmpU32 = SndCall(8, 0x85, &em->pos, em->id, 0, em);
        em->r_no_3 = 0;
        PlGachaInit();
        StaFlagOn(pG, STA_PL_CATCHHOLD);
        w->Timer2 = 0x50;
        em->r_no_2++;
    case 1:
        em10CamMove(em, em->r_no_3, 0.1f, 0);
        PlGachaMove();
        if (w->Timer2) {
            w->Timer2--;
            if (w->Timer2 == 0) {
                if (w->Fire_timer) {
                    w->Fire_timer = 0;
                }
                if (w->pWeapon) {
                    w->pWeapon->setLost();
                    w->pWeapon = 0;
                    w->Wep_type = 0;
                }
                em10CoreBreak(em, 1);
                if (w->pParasite) {
                    w->pParasite->setReset();
                    w->pParasite = 0;
                }
                em->hp = 0;
                em->atari.m_flag = (em->atari.m_flag & ~0x300) | 0x10;
                EmSetDie(em);
                EmReserveDropItem(em);
                em10SetPoint(em);
                em->clearStatus(EM_STATUS_ACTIVE);
                em->setStatus(EM_STATUS_ITEMSET);
                SndStop(w->TmpU32, 0);
                SndCall(8, 0x96, &em->pos, em->id, 0, em);
                cam = &pG->Camera;
                p = em->getPartsPtr(0);
                if ((cam->param.pos.x - p->world.x) * (cam->param.pos.x - p->world.x) +
                        (cam->param.pos.y - p->world.y) * (cam->param.pos.y - p->world.y) +
                        (cam->param.pos.z - p->world.z) * (cam->param.pos.z - p->world.z) <
                    4000000.0f) {
                    rot.x = 0.0f;
                    rot.y = GetXZAngle(&p->world, &cam->param.pos);
                    rot.z = 0.0f;
                    EstSet(0, -1, &em->pos, &rot, EFF_EM10, 0x47, 0, ESP_CORE_KIND_NONE, 0, 0);
                } else {
                    EstSet(em, -1, 0, 0, EFF_EM10, 0x30, 0, ESP_CORE_KIND_NONE, em, 0);
                }
                w->Timer = 3;
                MotionMove(em, 0);
                em->r_no_2 = 2;
                break;
            }
        }
        EmCatchMotionMove(em, 0.3f, 0.2f);
        if ((u32) PlGachaGet() > 0xF) {
            em->r_no_2 = 4;
        }
        break;
    case 2:
        em->r_no_2++;
    case 3:
        MotionMove(em, 0);
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                w->Reset_enable = 1;
                w->Be_flg |= 0x400000;
                em->be_flag &= ~2;
            }
        }
        em10CamMove(em, em->r_no_3, 0.1f, 0);
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x295), PL_ARC_PTR(em->subArc, 0x296), 5, 1, 0);
        SndStop(w->TmpU32, 0);
        SndCall(1, 0x3D, &pPL->pos, 0, 0, pPL);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x4A, 0, ESP_CORE_KIND_NONE, em, 0);
        EstSet(pPL, -1, 0, 0, EFF_EM10, 0x4C, 0, ESP_CORE_KIND_NONE, pPL, 0);
        pPL->dmg.m_Timer = 2;
        em->r_no_2++;
    case 5:
        em->dmg.m_Timer = 2;
        if (MotionMove(em, 0)) {
            w->Be_flg &= ~0x800;
            em->atari.m_flag &= ~8;
            em10SetAtkWait(em, 1);
            w->Be_flg &= ~0x20;
            EmRoutineSet(em, 1, 0x1E, 0, 0);
        } else {
            if (em->Motion.Seq_old.Free & 1) {
                cModel* q = pPL->getPartsPtr(4);
                SndCall(1, 0x3A, &q->world, 0, 0, pPL);
                SndCall(1, 0x3B, &q->world, 0, 0, pPL);
                w->Se_no = w->Se_tbl[8];
                em10SetDamageVoice(em, w->Se_no, w->Se_tbl[0]);
            }
            if (em->Motion.Seq_old.Free & 2) {
                em10SetDmWaterEff(em, 1);
            }
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 1);
    if (em->Motion.Seq_old.Free & 0x10) {
        em10SetCrash(em, 800.0f);
    }
}
#endif // PARKED

// Branch check of DashCatch (0x39): the running grab; on the catch frame the same Bombhold / Backhold /
// NeckHang choice as br_Catch.
// FUNCTION: BIO4 0x004506C0
// STATUS: stub
#if 0 // PARKED: Muku2, PI
void em10_R1_br_DashCatch(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp > 0 && (em->Motion.Seq_old.Free & 2) && em10CatchCk(em)) {
        if (fabsf(Muku2(em->ang.y, pPL->ang.y, PI)) < 1.5707964f && (em10SomebodyNearCk(em) || pSUB || w->Wep_type == 9)) {
            if (w->Wep_type == 9 && w->Fire_timer) {
                EmRoutineSetW(em, 1, 0x38, 0, 0);
            } else {
                EmRoutineSetW(em, 1, 0x37, 0, 0);
            }
        } else {
            EmRoutineSetW(em, 1, 0x34, 0, 0);
        }
    }
}
#endif // PARKED

// R1 == 0x39 DashCatch: the running lunge at the player (motion 0x8E, voice Se_tbl[5]) turning
// towards him; a miss ends in the fall / splash effect (0x33 in water, 0x34 dust) and the walk.
// FUNCTION: BIO4 0x004745A0
// STATUS: stub
#if 0 // PARKED: C2105, EFF_ROOM, GameAddPoint, LIMIT_ANGLE, LVADD_ESCAPEATTACK, Muku, SndCall, int
void em10_R1_DashCatch(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;
    int r;

    switch (em->r_no_2) {
    case 0:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x8E), PL_ARC_PTR(em->subArc, 0x8F), 10, 1, 0);
        em->r_no_3 = 0;
        em10CallVoiceSe2(em, w->Se_tbl[5], 8);
        w->Timer = 15;
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.19634955f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        end = MotionMove(em, 0);
        if (end) {
            r = Rnd();
            w->Jcatch_wait = r % 300 + 300;
            GameAddPoint(LVADD_ESCAPEATTACK);
            em10WalkRtnSet(em);
        } else if ((em->Motion.Seq_old.Free & 1) && CheckInWater(em, 0)) {
            EstSet(em, -1, 0, 0, EFF_ROOM, 0x33, 0, ESP_CORE_KIND_NONE, em, 0);
            SndCall(6, 0x16, &em->pos, 0, 0, em);
        } else if ((em->Motion.Seq_old.Free & 0x20) && CheckInWater(em, 0)) {
            EstSet(em, -1, 0, 0, EFF_ROOM, 0x34, 0, ESP_CORE_KIND_NONE, em, 0);
            SndCall(6, 0x11, &em->pos, 0, 0, em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R1 == 0x3A TakeAway: carries Ashley off (motion 0x29F pick-up with subem10_TakeAway on her, then
// 0x2A0 running with her over the shoulder, flags 0x4800 | 0x4000000): runs the escape route
// (em10SetTakeawayPos) through windows / doors / racks / climb-overs / jumps (0x92..0x97), stops when
// the player frees her (Status_flg[1] bit16 clear); reaching the exit fades both out with the
// take-away camera (em10CamMoveTakeaway) and sets the "Ashley taken" bit Status_flg[1] bit6.
// FUNCTION: BIO4 0x00474850
// STATUS: stub
#if 0 // PARKED: C2105, C2143, C2227, C2228, Catch_at_adj, DBG_FOG_FAR_GREEN, DPF_SCR, DbgFlagOff, DpfFlagOn, EFF_PL01, EM10_FALL_WATER_EFFECT, EM_STATUS_IK_OFF, EffectDeleteAll, EmCatchMotionMove, EmCatchSubSet, EmMgr, GXColor, LIMIT_ANGLE, Muku, ObjMgr, PSMTXMultVec, PSVECAdd, PSVECScale, PSVECSubtract, STA_SET_BG_COLOR, STA_TAKEAWAY, SatMgr, SceEventStart, SndCall, StaFlagChk, StaFlagOn, Timer2, Timer3, ashley_life, bio4_GXSetCopyClear, cObj, int, invisible_factor, isAlive, mat, pos_old, set, throughOn
void em10_R1_TakeAway(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;
    u32 i;
    Vec v;

    w->Be_flg |= 0x4800;
    switch (em->r_no_2) {
    case 0:
        w->TmpV = em->scale;
        em->scale.x = 1.0f;
        em->scale.y = 1.0f;
        em->scale.z = 1.0f;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x29F), 0, 5, 1, 0);
        EmCatchSubSet(em, pSUB, 0.0f, 2, -194.22f, 0.0f, 582.05f, subem10_TakeAway);
        em->dmg.set(0, 0);
        em->flag &= ~0x400;
        w->Timer2 = 0x28;
        em10SetTakeawayPos(em);
        em10SetTakeawayPosUpdate(em);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (em->Character == 5) {
            em->Character = 0;
        }
        w->Timer = 10;
        em->r_no_2++;
    case 1: {
        int end;
        w->Be_flg |= 0x4000000;
        if (w->Timer) {
            w->Timer--;
            end = EmCatchMotionMove(em, 0.3f, 0.2f);
        } else {
            end = MotionMove(em, 0);
        }
        if (end) {
            em->r_no_2++;
        }
        break;
    }
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2A0), 0, 5, 5, 0);
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x4000000;
        em10SetTakeawayPosUpdate(em);
        em->ang.y += Muku(&em->pos, &w->Go_pos, em->ang.y, 0.09817477f);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        MotionMove(em, 0);
        if (!StaFlagChk(pG, STA_TAKEAWAY)) {
            em10WalkRtnSet(em);
            break;
        }
        switch ((u32) em10WindowCk2(em)) {
        case 0:
        case 3:
        case 4:
        default:
            break;
        case 1:
            em->r_no_3 = 1;
            em->r_no_2 = 4;
            return;
        case 2:
            em->r_no_2 = 6;
            return;
        }
        em10DoorOpenCk(em, 0);
        em10RackBreakCk(em);
        switch ((u32) em10ClimbOverCk2(em)) {
        case 0:
        default:
            break;
        case 1:
            em->r_no_2 = 4;
            return;
        case 2:
            em->r_no_2 = 6;
            em->r_no_3 = 1;
            return;
        }
        if (em10JumpDownCk2(em)) {
            em->r_no_2 = 6;
            em->r_no_3 = 0;
            return;
        }
        if ((em->pos.x - w->Route_target.x) * (em->pos.x - w->Route_target.x) + (em->pos.z - w->Route_target.z) * (em->pos.z - w->Route_target.z) <
            640000.0f) {
            em->r_no_2 = 0xE;
            break;
        }
        if (!StaFlagChk(pG, STA_TAKEAWAY)) {
            em10WalkRtnSet(em);
        }
        break;
    case 4:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x92), PL_ARC_PTR(em->subArc, 0x93), 10, 1, 0);
        PSVECSubtract(&w->Target_pos, &em->pos, &w->Target_pos);
        w->Target_pos.y = 0.0f;
        em->r_no_2++;
    case 5:
        em->setStatus(EM_STATUS_IK_OFF);
        PSVECScale(&w->Target_pos, &v, 0.2f);
        PSVECAdd(&em->pos, &v, &em->pos);
        PSVECSubtract(&w->Target_pos, &v, &w->Target_pos);
        if (em->Motion.Seq_old.Free & 4) {
            w->Be_flg |= 0x20000;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2 = 2;
        }
        break;
    case 6:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x94), PL_ARC_PTR(em->subArc, 0x95), 3, 1, 0);
        w->TmpU32 = 0;
        em->r_no_2++;
    case 7:
        em->dmg.m_Timer = 2;
        w->Be_flg |= 0x10080000;
        em->setStatus(EM_STATUS_IK_OFF);
        MotionMove(em, 0);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                EM10_FALL_WATER_EFFECT
            }
        }
        if (!(em->Motion.Seq_old.Free & 0x40)) {
            f32 y;
            v = em->pos;
            v.y = em->pos_old.y;
            y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
            if (!(em->pos.y > y)) {
                em->pos.y = y;
                w->Spd.y = 0.0f;
                em->r_no_2++;
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x96), 0, 3, 1, 0);
                MotionMove(em, 0);
            }
        }
        break;
    case 8:
        SndCall(8, 0x77, &em->pos, em->id, 0, em);
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x96), 0, 3, 1, 0);
        r = CheckInWater(em, 0);
        if (r) {
            em10FallWaterCk(em);
            if (w->TmpU32 == 0) {
                EM10_FALL_WATER_EFFECT
            }
        } else if (ChkWaterEffectEnable(&em->pos)) {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, 0, 0);
        } else {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, 0, 0);
        }
        em->r_no_2++;
    case 9:
        if (MotionMove(em, 0)) {
            em->r_no_2 = 2;
        }
        break;
    case 0xE:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x97), 0, 3, 5, 0);
        w->Timer3 = 0x1E;
        em->r_no_2++;
    case 0xF:
        MotionMove(em, 0);
        if (!StaFlagChk(pG, STA_TAKEAWAY)) {
            em10WalkRtnSet(em);
            break;
        }
        if (w->Timer3) {
            w->Timer3--;
            if (w->Timer3 == 0) {
                SceEventStart(0);
                pPL->dmg.set(0, 0x80);
                pPL->setNoSuspend(1);
                em->setNoSuspend(1);
                em->dmg.m_Timer = 0x80;
                pG->ashley_life = 0;
                em->atari.throughOn();
                if (pSUB) {
                    pSUB->atari.throughOn();
                    pSUB->setNoSuspend(1);
                }
                em->r_no_2++;
            }
        }
        break;
    case 0x10: {
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2A0), 0, 5, 5, 0);
        EffectDeleteAll();
        {
            Vec v2 = {0.0f, 0.0f, 2000.0f};
            Vec rot = em->ang;
            PSMTXMultVec(em->mat, &v2, &v2);
            EstSet(0, -1, &v2, &rot, EFF_PL01, 0, 1, ESP_CORE_KIND_NONE, 0, 0);
        }
        DpfFlagOn(pG, DPF_SCR);
        pPL->setNoSuspend(0);
        DbgFlagOff(pG, DBG_FOG_FAR_GREEN);
        StaFlagOn(pG, STA_SET_BG_COLOR);
        bio4_GXSetCopyClear(GXColor(), 0xFFFFFF);
        for (i = 0; i < EmMgr.getArrayNum(); i++) {
            cEm* e = EmMgr.fastAt(i);
            if (e && e != em && pSUB && e != pSUB && e->isAlive()) {
                e->setNoSuspend(0);
            }
        }
        for (i = 0; i < ObjMgr.getArrayNum(); i++) {
            cObj* o = ObjMgr.fastAt(i);
            if (o && o->isAlive()) {
                o->setNoSuspend(0);
            }
        }
        w->Timer3 = 0x32;
        em->r_no_2++;
    }
    case 0x11:
        if (w->Timer3) {
            w->Timer3--;
        } else {
            em->invisible_factor -= 0.04f;
            if (em->invisible_factor < 0.0f) {
                em->invisible_factor = 0.0f;
            }
            if (pSUB) {
                pSUB->invisible_factor = pSUB->invisible_factor - 0.04f;
                if (pSUB->invisible_factor < 0.0f) {
                    pSUB->invisible_factor = 0.0f;
                }
            }
        }
        MotionMove(em, 0);
        em10CamMoveTakeaway(em);
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 0);
}
#endif // PARKED

// Take-away camera: installs the work's Camera (Cam) as the cut-in camera looking from Campos at the
// Ganado carrying Ashley off (em10_R1_TakeAway steps 0x10/0x11).
// FUNCTION: BIO4 0x0042B060
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, GlobalWork, PSMTXMultVec, PosToPos, g, mat
extern "C" void em10CamMoveTakeaway(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    GlobalWork* g = pG;
    Vec a;
    Vec b;

    a.x = 250.0f;
    a.y = 579.0f;
    a.z = -2530.0f;
    b.x = 0.0f;
    b.y = 1075.0f;
    b.z = 0.0f;
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    PosToPos(&g->Camera.param.pos, &a, &w->Cam.param.pos, 1.0f);
    PosToPos(&g->Camera.param.at, &b, &w->Cam.param.at, 1.0f);
    w->Cam.Up.x = 0.0f;
    w->Cam.Up.y = 1.0f;
    w->Cam.Up.z = 0.0f;
    {
        f32 dx = w->Cam.param.pos.x - w->Cam.param.at.x;
        f32 dy = w->Cam.param.pos.y - w->Cam.param.at.y;
        f32 dz = w->Cam.param.pos.z - w->Cam.param.at.z;
        w->Cam.Distance = SQRTF(dx * dx + dy * dy + dz * dz);
    }
    w->Cam.param.fovy = 55.0f;
    CameraSetOrientationUp(&w->Cam);
    CamCtrl.m_pExtraCamera = (s32) &w->Cam;
}
#endif // PARKED

// R0 2 / R1 == 0x00 Dm_Small: the flinch. Picks the motion from the hit zone (arm parts 8/0xE, hands
// 9/0xF, thighs 0x13/0x17, shins 0x14/0x18, front or back hit; a leg hit may drop the Ganado to its
// knees, flag 0x20 = knee), with weapon-specific variants (a hand hit drops the weapon), then turns
// to the player, offers the melee prompt (em10ActEvtSetKick / FS by character and hit zone) and
// returns to the walk or to DownWakeWait; may hide again (em10HideRtnCk) after 10 frames.
// FUNCTION: BIO4 0x00475800
// STATUS: stub
#if 0 // PARKED: C1003, C2143, C2227, DM_SMALL_RND2, DM_SMALL_RND3, DM_SMALL_WEP_MOT, DM_SMALL_WEP_MOT_SE, Muku, PI, Timer2, Timer3, Timer5, YARARE_INFO, hit, int, switch, w
void em10_R1_Dm_Small(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    void* m0;
    void* m1;
    int flag;
    u32 type;
    int r;
    u32 r3;

    switch (em->r_no_2) {
    case 0: {
        YARARE_INFO* hit = em->dmg.m_pDamageYarare;
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        if (fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI)) < 1.5707964f) {
            type = hit->parts_no == 8 ? 1 : 0;
            if (hit->parts_no == 9) {
                type = 2;
            }
            if (hit->parts_no == 0xE) {
                type = 3;
            }
            if (hit->parts_no == 0xF) {
                type = 4;
            }
            if (hit->parts_no == 0x13) {
                type = 5;
            }
            if (hit->parts_no == 0x17) {
                type = 6;
            }
            if (hit->parts_no == 0x14) {
                type = 8;
            }
            if (hit->parts_no == 0x18) {
                type = 9;
            }
        } else {
            type = 7;
            if (hit->parts_no == 0x14) {
                type = 8;
            }
            if (hit->parts_no == 0x18) {
                type = 9;
            }
            if (hit->parts_no == 9) {
                type = 0xA;
            }
            if (hit->parts_no == 0xF) {
                type = 0xB;
            }
        }
        if (w->Wep_type == 0xC) {
            type = 0xC;
        }
        w->Timer = 0;
        w->Timer2 = 0;
        w->Timer3 = 2;
        w->Timer5 = 10;
        w->TmpU32 = 0;
        if (em->type == 6 || em->type == 0x16) {
            if (type == 8) {
                type = 5;
            }
            if (type == 9) {
                type = 6;
            }
        }
        w->Se_no = w->Se_tbl[8];
        flag = 1;
        m1 = 0;
        switch (type) {
        case 0:
        default:
            switch (Rnd() % 3) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x26);
                m1 = PL_ARC_PTR(em->subArc, 0x27);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x28);
                m1 = PL_ARC_PTR(em->subArc, 0x29);
                break;
            case 2:
                m0 = PL_ARC_PTR(em->subArc, 0x28);
                m1 = PL_ARC_PTR(em->subArc, 0x29);
                break;
            }
            flag = (em->Motion.Mot_attr & 0x40) ? 1 : 0x41;
            if (w->Wep_type == 1) {
                DM_SMALL_WEP_MOT(0x159, 0x15A)
            }
            if (w->Wep_type == 6) {
                DM_SMALL_WEP_MOT(0x144, 0x145)
            }
            if (w->Wep_type == 4) {
                DM_SMALL_WEP_MOT(0xF7, 0xF8)
            }
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x176, 0x177)
            }
            break;
        case 1:
            DM_SMALL_RND2()
            switch (r) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x28);
                m1 = PL_ARC_PTR(em->subArc, 0x29);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x33);
                m1 = PL_ARC_PTR(em->subArc, 0x34);
                w->Timer = 10;
                w->Timer2 = 0x3C;
                w->Se_no = w->Se_tbl[1];
                break;
            }
            em->setWeaponFall();
            flag = 1;
            if (w->Wep_type == 1) {
                DM_SMALL_WEP_MOT_SE(0x159, 0x15A)
            }
            if (w->Wep_type == 6) {
                DM_SMALL_WEP_MOT_SE(0x144, 0x145)
            }
            if (w->Wep_type == 4) {
                DM_SMALL_WEP_MOT_SE(0xF7, 0xF8)
            }
            if (w->pShield) {
                DM_SMALL_WEP_MOT_SE(0x176, 0x177)
            }
            break;
        case 2:
            DM_SMALL_RND2()
            switch (r) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x32);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x33);
                m1 = PL_ARC_PTR(em->subArc, 0x34);
                w->Timer = 10;
                w->Timer2 = 0x3C;
                w->Se_no = w->Se_tbl[1];
                break;
            }
            if (w->pWeapon && !(em->flag & 0x1000000)) {
                m0 = PL_ARC_PTR(em->subArc, 0x55);
                m1 = PL_ARC_PTR(em->subArc, 0x56);
                w->Se_no = w->Se_tbl[8];
                em->setWeaponFall();
            }
            flag = 1;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x176, 0x177)
            }
            break;
        case 3:
            DM_SMALL_RND2()
            switch (r) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x2A);
                m1 = PL_ARC_PTR(em->subArc, 0x2B);
                flag = 1;
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x33);
                m1 = PL_ARC_PTR(em->subArc, 0x34);
                w->Timer = 10;
                w->Timer2 = 0x3C;
                w->Se_no = w->Se_tbl[1];
                flag = 0x41;
                break;
            }
            em->setWeaponFall();
            if (w->Wep_type == 1) {
                DM_SMALL_WEP_MOT_SE(0x159, 0x15A)
            }
            if (w->Wep_type == 6) {
                DM_SMALL_WEP_MOT_SE(0x144, 0x145)
            }
            if (w->Wep_type == 4) {
                DM_SMALL_WEP_MOT_SE(0xF7, 0xF8)
            }
            if (w->pShield) {
                DM_SMALL_WEP_MOT_SE(0x176, 0x177)
            }
            break;
        case 4:
            DM_SMALL_RND2()
            switch (r) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x32);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x33);
                m1 = PL_ARC_PTR(em->subArc, 0x34);
                w->Timer = 10;
                w->Timer2 = 0x3C;
                w->Se_no = w->Se_tbl[1];
                break;
            }
            if (w->pWeapon && (em->flag & 0x1000000)) {
                m0 = PL_ARC_PTR(em->subArc, 0x55);
                m1 = PL_ARC_PTR(em->subArc, 0x56);
                w->Se_no = w->Se_tbl[8];
                em->setWeaponFall();
            }
            flag = 0x41;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x176, 0x177)
            }
            break;
        case 5:
            DM_SMALL_RND3()
            switch (r3) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x35);
                m1 = PL_ARC_PTR(em->subArc, 0x36);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x37);
                m1 = PL_ARC_PTR(em->subArc, 0x38);
                w->Se_no = w->Se_tbl[3];
                break;
            case 2:
                m0 = PL_ARC_PTR(em->subArc, 0x51);
                m1 = PL_ARC_PTR(em->subArc, 0x52);
                w->Be_flg |= 0x20;
                w->Timer = 999;
                break;
            }
            w->Timer5 = 0;
            flag = 1;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x174, 0x175)
                w->Timer = 0;
            }
            break;
        case 6:
            DM_SMALL_RND3()
            switch (r3) {
            case 0:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x35);
                m1 = PL_ARC_PTR(em->subArc, 0x36);
                break;
            case 1:
                m0 = PL_ARC_PTR(em->subArc, 0x37);
                m1 = PL_ARC_PTR(em->subArc, 0x38);
                w->Se_no = w->Se_tbl[3];
                break;
            case 2:
                m0 = PL_ARC_PTR(em->subArc, 0x51);
                m1 = PL_ARC_PTR(em->subArc, 0x52);
                w->Be_flg |= 0x20;
                w->Timer = 999;
                break;
            }
            w->Timer5 = 0;
            flag = 0x41;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x174, 0x175)
                w->Timer = 0;
            }
            break;
        case 7:
            m0 = PL_ARC_PTR(em->subArc, 0x45);
            m1 = PL_ARC_PTR(em->subArc, 0x46);
            flag = (em->Motion.Mot_attr & 0x40) ? 1 : 0x41;
            if (w->Wep_type == 1) {
                DM_SMALL_WEP_MOT(0x15B, 0x15C)
            }
            if (w->Wep_type == 6) {
                DM_SMALL_WEP_MOT(0x146, 0x147)
            }
            if (w->Wep_type == 4) {
                DM_SMALL_WEP_MOT(0xF9, 0xFA)
            }
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x172, 0x173)
            }
            w->Timer5 = 0;
            w->Timer = 10;
            break;
        case 8:
            switch (Rnd() % 3) {
            case 0:
            case 1:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x39);
                m1 = PL_ARC_PTR(em->subArc, 0x3A);
                w->Timer = 10;
                w->Timer5 = 0;
                w->TmpU32 = 1;
                break;
            case 2:
                m0 = PL_ARC_PTR(em->subArc, 0x51);
                m1 = PL_ARC_PTR(em->subArc, 0x52);
                w->Be_flg |= 0x20;
                w->Timer = 999;
                break;
            }
            flag = 1;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x174, 0x175)
                w->Timer = 0;
                w->TmpU32 = 2;
            }
            w->Timer5 = 0;
            break;
        case 9:
            switch (Rnd() % 3) {
            case 0:
            case 1:
            default:
                m0 = PL_ARC_PTR(em->subArc, 0x39);
                m1 = PL_ARC_PTR(em->subArc, 0x3A);
                w->Timer = 10;
                w->Timer5 = 0;
                w->TmpU32 = 1;
                break;
            case 2:
                m0 = PL_ARC_PTR(em->subArc, 0x51);
                m1 = PL_ARC_PTR(em->subArc, 0x52);
                w->Be_flg |= 0x20;
                w->Timer = 999;
                break;
            }
            flag = 0x41;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x174, 0x175)
                w->Timer = 0;
                w->TmpU32 = 2;
            }
            w->Timer5 = 0;
            break;
        case 0xA:
            m0 = PL_ARC_PTR(em->subArc, 0x47);
            m1 = PL_ARC_PTR(em->subArc, 0x48);
            if (w->pWeapon && !(em->flag & 0x1000000)) {
                m0 = PL_ARC_PTR(em->subArc, 0x55);
                m1 = PL_ARC_PTR(em->subArc, 0x56);
                em->setWeaponFall();
            }
            w->Timer5 = 0;
            flag = 1;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x176, 0x177)
            }
            break;
        case 0xB:
            m0 = PL_ARC_PTR(em->subArc, 0x47);
            m1 = PL_ARC_PTR(em->subArc, 0x48);
            if (w->pWeapon && (em->flag & 0x1000000)) {
                m0 = PL_ARC_PTR(em->subArc, 0x55);
                m1 = PL_ARC_PTR(em->subArc, 0x56);
                em->setWeaponFall();
            }
            w->Timer5 = 0;
            flag = 0x41;
            if (w->pShield) {
                DM_SMALL_WEP_MOT(0x176, 0x177)
            }
            break;
        case 0xC:
            m0 = PL_ARC_PTR(em->subArc, 0x51);
            m1 = PL_ARC_PTR(em->subArc, 0x52);
            w->Be_flg |= 0x20;
            w->Timer = 999;
            w->Timer5 = 0;
            break;
        }
        MotionSetCore(em, MOTION(em), m0, m1, 6, flag, 0);
        w->Timer4 = 10;
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        if (w->pShield) {
            w->Timer = 0;
        }
        em->r_no_2++;
    }
    case 1:
        if (w->Timer3) {
            w->Timer3--;
            if (w->Timer3 == 0) {
                em10SetDamageVoice(em, w->Se_no, w->Se_tbl[0]);
            }
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Be_flg &= ~8;
        }
        if (w->Timer2) {
            w->Timer2--;
            em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, 0.09817477f);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (em->Motion.Seq_old.Free & 2) {
            w->Be_flg |= 0x40000000;
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0) || (em->Motion.Seq_old.Free & 4)) {
            if (em->Motion.Seq_old.Free & 0x80) {
                w->Be_flg |= 0x10;
                w->Be_flg |= 0x1000000;
                em->setStatus(EM_STATUS_IK_OFF);
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            } else {
                em10WalkRtnSet(em);
            }
        } else if (!(em->Motion.Seq_old.Free & 0x80) && w->Timer5) {
            w->Timer5--;
            if (w->Timer5 == 0) {
                em10HideRtnCk(em);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    switch ((u32) w->TmpU32) {
    case 0:
    default:
        break;
    case 1:
        switch (pG->pl_type) {
        case 2:
            em10ActEvtSetKick(em);
            break;
        case 3:
            em10ActEvtSetKick(em);
            break;
        case 4:
            em10ActEvtSetFS(em);
            break;
        case 5:
            em10ActEvtSetKick(em);
            break;
        default:
            if (w->Ganado) {
                em10ActEvtSetFS(em);
            } else {
                em10ActEvtSetKick(em);
            }
            break;
        }
        break;
    case 2:
        switch (pG->pl_type) {
        case 2:
            em10ActEvtSetKick(em);
            break;
        case 4:
            em10ActEvtSetKick(em);
            break;
        case 5:
            em10ActEvtSetKick(em);
            break;
        default:
            em10ActEvtSetKick(em);
            break;
        case 3:
            w->Be_flg |= 0x40000000;
            em10ActEvtSetKick(em);
            w->Be_flg &= ~0x40000000;
            break;
        }
        break;
    }
}
#endif // PARKED

// R0 2 / R1 == 0x01 Dm_Head: the head shot stagger (motions 0x2C / 0x2E / 0x30 by side): the cap
// (cObj12 pCap) or glasses may fly off, the head-hit voice plays, and the kick prompt is offered;
// back to the walk when the motion ends.
// FUNCTION: BIO4 0x00476E20
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, Mtx, Muku, ObjMgr, PI, PSMTXMultVecSR, PSMTXRotRad, SndStop, Timer3, cObj12, ckAtkEnable, int, m, mat, setDamage, vB8, vC0
void em10_R1_Dm_Head(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;
    Mtx m;
    Vec spd;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        if (fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI)) < 1.5707964f) {
            switch ((u8) (Rnd() % 3)) {
            case 1:
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2E), PL_ARC_PTR(em->subArc, 0x2F), 6, flag, 0);
                break;
            case 2:
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x30), PL_ARC_PTR(em->subArc, 0x31), 6, flag, 0);
                break;
            case 0:
            default:
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2C), PL_ARC_PTR(em->subArc, 0x2D), 6, flag, 0);
                break;
            }
        } else {
            switch (Rnd() & 1) {
            case 0:
            default:
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2C), PL_ARC_PTR(em->subArc, 0x2D), 6, flag, 0);
                break;
            case 1:
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2E), PL_ARC_PTR(em->subArc, 0x2F), 6, flag, 0);
                break;
            }
        }
        w->Timer = 15;
        w->Timer3 = 2;
        if (w->pCap) {
            if ((u8) (Rnd() % 10) > 4 || em->type == 2) {
                switch (w->Cap_type) {
                case 0:
                    break;
                default:
                    PSMTXRotRad(m, 'y', GetXZAngle(&pPL->pos, &em->pos));
                    spd.x = 0.0f;
                    spd.y = 40.0f;
                    spd.z = -50.0f;
                    PSMTXMultVecSR(em->mat, &spd, &spd);
                    w->pCap->setFall(&spd, 2);
                    w->pCap = 0;
                    w->Cap_type = 0;
                    break;
                case 3:
                case 4:
                    ObjMgr.destroy(w->pCap);
                    w->pCap = 0;
                    w->Cap_type = 0;
                    break;
                }
            }
        }
        if (w->pGlasses) {
            PSMTXRotRad(m, 'y', GetXZAngle(&pPL->pos, &em->pos));
            spd.x = 0.0f;
            spd.y = 40.0f;
            spd.z = -50.0f;
            PSMTXMultVecSR(em->mat, &spd, &spd);
            w->pGlasses->setFall(&spd, 3);
            w->pGlasses = 0;
        }
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        if (w->Timer3) {
            w->Timer3--;
            if (w->Timer3 == 0) {
                em10SetDamageVoice(em, w->Se_tbl[2], w->Se_tbl[0]);
            }
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Be_flg &= ~8;
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10ActEvtSetKick(em);
}
#endif // PARKED

// R0 2 / R1 == 0x0D Dm_Flash: stunned by the flash grenade: the blinded motion 0x2AE (random start),
// the recover motions 0x2B0 / 0x2B2, the kick prompt available throughout; then the walk.
// FUNCTION: BIO4 0x004773D0
// STATUS: stub
#if 0 // PARKED: C2105, SndStop, Timer3, ckAtkEnable, int, setDamage, vB8, vC0
void em10_R1_Dm_Flash(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2AE), PL_ARC_PTR(em->subArc, 0x2AF), 6, (u16) flag,
                      (u8) (Rnd() % 5));
        w->Timer = 15;
        w->Timer3 = 2;
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        if (w->Timer3) {
            w->Timer3--;
            if (w->Timer3 == 0) {
                em10SetDamageVoice(em, w->Se_tbl[2], w->Se_tbl[0]);
            }
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Be_flg &= ~8;
        }
        if (MotionMove(em, 0)) {
            em->r_no_2++;
        }
        break;
    case 2:
        flag = (em->flag & 0x1000000) ? 0x45 : 5;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B0), PL_ARC_PTR(em->subArc, 0x2B1), 6, flag, 0);
        w->Timer = (u8) (Rnd() % 5) + 5;
        em->r_no_2++;
    case 3:
        w->Be_flg &= ~8;
        if (MotionMove(em, 0)) {
            if (w->Timer) {
                w->Timer--;
            } else {
                em->r_no_2++;
            }
        }
        break;
    case 4:
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B2), PL_ARC_PTR(em->subArc, 0x2B3), 6, flag, 0);
        em->r_no_2++;
    case 5:
        w->Be_flg &= ~8;
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
    em10ActEvtSetKick(em);
}
#endif // PARKED

// R0 2 / R1 == 0x0E Dm_Claw: the claw Ganado's (type 0xA/0xD) flinch (0x10F front / 0x111 back),
// then the walk; sets the 150..300 damage Claw_hp and the 450 frame Find_timer.
// FUNCTION: BIO4 0x004777E0
// STATUS: stub
#if 0 // PARKED: C2228, Muku, PI, SeInfo
void em10_R1_Dm_Claw(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Claw_hp = Rnd() % 150 + 150;
    w->Be_flg &= ~8;
    em10FindCk2(em);
    w->Be_flg |= 0x100;
    w->Find_timer = 0x1C2;
    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        if (fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI)) < 1.5707964f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x10F), PL_ARC_PTR(em->subArc, 0x110), 3, 1, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x111), PL_ARC_PTR(em->subArc, 0x112), 3, 1, 0);
        }
        em10SetDamageVoice(em, w->Se_tbl[1], w->Se_tbl[0]);
        w->Goto_pos = pG->SeInfo.pos;
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        } else if (em->Motion.Seq_old.Free & 1) {
            em10CallVoiceSe2(em, w->Se_tbl[11], 8);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x0F Dm_Claw_Big: the hit to the exposed claw / parasite part 0x25 (motion 0x113,
// core damage effect 0x77), turning towards Go_pos, then the walk.
// FUNCTION: BIO4 0x00477A40
// STATUS: stub
#if 0 // PARKED: C2228, EstSet, LIMIT_ANGLE, Muku, PI, SeInfo, ckAtkEnable, setDamage
void em10_R1_Dm_Claw_Big(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    f32 a;

    w->Claw_hp = Rnd() % 150 + 150;
    em10FindCk2(em);
    w->Be_flg |= 0x100;
    w->Find_timer = 0x1C2;
    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x113), PL_ARC_PTR(em->subArc, 0x114), 3, 1, 0);
        if (w->pCore) {
            if (w->pCore->ckAtkEnable()) {
                w->pCore->setDamage();
            }
            EstSet(w->pCore, -1, 0, 0, EFF_EM10, 0x77, 0, ESP_CORE_KIND_NONE, w->pCore, 0);
        }
        em10CallVoiceSe(em, w->Se_tbl[1]);
        w->Goto_pos = pG->SeInfo.pos;
        w->TmpF = em->ang.y + PI;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 8) {
            a = Muku(&em->pos, &w->Go_pos, w->TmpF, 0.09817477f);
            w->TmpF += a;
            w->TmpF = LIMIT_ANGLE(w->TmpF);
            em->ang.y += a;
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x11 Dm_Gatling: the gatling Ganado's (type 2) flinch (0x194 front / 0x196 back),
// then the walk.
// FUNCTION: BIO4 0x00477CE0
// STATUS: stub
#if 0 // PARKED: Muku, PI
void em10_R1_Dm_Gatling(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Claw_hp = Rnd() % 150 + 150;
    em10FindCk2(em);
    w->Be_flg |= 0x100;
    w->Find_timer = 0x1C2;
    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        if (fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI)) < 1.5707964f) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x194), PL_ARC_PTR(em->subArc, 0x195), 3, 1, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x196), PL_ARC_PTR(em->subArc, 0x197), 3, 1, 0);
        }
        em10SetDamageVoice(em, w->Se_tbl[1], w->Se_tbl[0]);
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x10 Dm_FS: hit by the player's suplex (em10FSAction): the Ganado is slammed head
// first (motion 0xD4 facing the player, drops the weapon / shield): 40% chance of instant death, else
// 300 damage; the head bursts on the landing when killed (em10LostHead 3), then Die_Cramp or
// DownWakeWait.
// FUNCTION: BIO4 0x0045F1A0
// STATUS: stub
#if 0 // PARKED: C2227, EFF_ROOM, EM_STATUS_IK_OFF, EstSetEm10WaterFall, LIMIT_ANGLE, LifeDownSet, Muku, PI, PSMTXMultVec, RotMatrix, SndCall, SndStop, Timer3, TransMatrix, ckAtkEnable, mat, room_id, set, setDamage, vB8, vC0
void em10_R1_Dm_FS(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        em->r_no_3 = 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xD4), PL_ARC_PTR(em->subArc, 0xD5), 0, 1, 0);
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        pPL->ang.y = em->ang.y + PI;
        pPL->ang.y = LIMIT_ANGLE(pPL->ang.y);
        v.x = -2.01f;
        v.y = 0.0f;
        v.z = 628.03f;
        RotMatrix(em->mat, &em->ang);
        TransMatrix(em->mat, &em->pos);
        PSMTXMultVec(em->mat, &v, &pPL->pos);
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        w->TmpU32 = 0;
        w->Landing_ck = 0;
        w->Timer3 = 0;
        if ((u8) (Rnd() % 100) < 40) {
            LifeDownSet(em, 9999, 0);
        } else {
            LifeDownSet(em, 300, 0);
        }
        if (em->hp > 0) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x84, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        w->Be_flg |= 0x20;
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        if (w->Landing_ck == 0) {
            em->atari.m_flag |= 8;
            w->No_adj_timer = 2;
        }
        em->dmg.set(0, 2);
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            em10FallWaterCk(em);
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
            if (em->hp <= 0) {
                em->hp = 0;
                em10LostHead(em, 3, 1);
                SndCall(1, 0x12, &em->pos, 0, 0, em);
            } else {
                EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x83, 0, ESP_CORE_KIND_NONE, 0, 0);
                SndCall(8, 0xB0, &em->pos, em->id, 0, em);
            }
        }
        if (em->Motion.Seq_old.Free & 2) {
            em10SetDmWaterEff(em, 1);
        }
        if (w->TmpU32 == 0 && CheckInWater(em, 0)) {
            w->TmpU32 = 1;
            if (pG->room_id == 0x311) {
                EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                SndCall(6, 0xA, &em->pos, 0, 0, em);
            } else {
                EstSetEm10WaterFall((Vec*) em);
                SndCall(6, 0x16, &em->pos, 0, 0, em);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 1200.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x14 Dm_KneeKick: hit by the player's knee kick (plem10KneeKick): motion 0x2B7 facing
// him, 40% instant death else 1000 damage, weapon dropped; lands down (head lost when killed), then
// Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x0045F750
// STATUS: stub
#if 0 // PARKED: C1003, C2227, LIMIT_ANGLE, Muku, PI, PSMTXMultVec, RotMatrix, SndStop, Timer3, TransMatrix, mat
void em10_R1_Dm_KneeKick(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;

    em->dmg.m_Timer = 2;
    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        em->r_no_3 = 0;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B7), PL_ARC_PTR(em->subArc, 0x2B8), 0, 1, 0);
        em->ang.y += Muku(&em->pos, &pPL->pos, em->ang.y, PI);
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        pPL->ang.y = em->ang.y + PI;
        pPL->ang.y = LIMIT_ANGLE(pPL->ang.y);
        v.x = -28.49f;
        v.y = 0.0f;
        v.z = 1382.54f;
        RotMatrix(em->mat, &em->ang);
        TransMatrix(em->mat, &em->pos);
        PSMTXMultVec(em->mat, &v, &pPL->pos);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        w->TmpU32 = 0;
        w->Landing_ck = 0;
        w->Timer3 = 0;
        if ((u8) (Rnd() % 100) < 40) {
            LifeDownSet(em, 9999, 0);
        } else {
            LifeDownSet(em, 1000, 0);
        }
        w->Be_flg |= 0x20;
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        em->dmg.set(0, 2);
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            em10FallWaterCk(em);
            if (em->hp <= 0) {
                em->hp = 0;
                em10LostHead(em, 0, 1);
            } else {
                SndCall(8, 0xB0, &em->pos, em->id, 0, em);
            }
        }
        if (em->Motion.Seq_old.Free & 2) {
            em10SetDmWaterEff(em, 1);
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x15 Dm_NeckBreak: the player's neck-break finisher (plem10NeckBreak): motion 0x2BA
// with effect 0x90, always fatal (hp 0); the body drops to the floor into Die_Cramp and scores.
// FUNCTION: BIO4 0x00440B50
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, Catch_at_adj, EM_STATUS_IK_OFF, EmCatchMotionMove, EmCatchPLSet, SatMgr, SndCall, SndStop, int, set
void em10_R1_Dm_NeckBreak(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int end;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2BA), PL_ARC_PTR(em->subArc, 0x2BB), 0, 1, 0);
        EstSet(em, -1, 0, 0, EFF_EM10, 0x90, 0, ESP_CORE_KIND_NONE, em, 0);
        EmCatchPLSet(em, 0.0f, 2, -75.21f, 0.0f, 1076.1f, plem10NeckBreak);
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        em->hp = 0;
        w->Be_flg &= ~0x20;
        w->Timer = 30;
        em->r_no_2++;
    case 1:
        em->dmg.set(0, 2);
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        } else {
            em->pos.y = pPL->pos.y;
            w->Be_flg |= 0x80000;
        }
        if (em->Motion.Seq_old.Free & 2) {
            em10SetDmWaterEff(em, 1);
        }
        if (w->Timer) {
            w->Timer--;
            end = EmCatchMotionMove(em, 1.0f, 1.0f);
        } else {
            end = MotionMove(em, 0);
        }
        if (end) {
            em->pos.y = SatMgr.getFloor(&em->pos, 0, 600.0f, 100000.0f, 0);
            EmRoutineSet(em, 3, 0, 0, 1);
        } else if (em->Motion.Seq_old.Free & 1) {
            SndCall(1, 0x4C, &em->getPartsPtr(4)->world, 0, 0, em);
            em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
            em10SetPoint(em);
        }
        break;
    }
    em->Catch_at_adj = em->pos;
    em10HandSet(em, 0);
    em10SetCrash(em, 1500.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x12 Dm_Showtay: hit by the palm strike (dmWep 0x25, plem10Showtay): flies back
// (motion 0x2B5, airborne flag 0x80000, falls off ledges with the splash / dust effects), the landing
// 0x2B6 takes extra damage, then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00440ED0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, DMG_TYPE_PUSH, DmgMgr, EFF_ROOM, EM_STATUS_IK_OFF, EstSetEm10WaterFall, LIMIT_ANGLE, LifeDownSet2, PI, SatMgr, SndCall, SndStop, Timer3, ckAtkEnable, int, pos_old, room_id, set, setDamage, vB8, vC0
void em10_R1_Dm_Showtay(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    f32 y;
    int dmg;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B5), 0, 3, 1, 2);
        em->ang.y = pPL->ang.y + PI;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em10SetDmWaterEff(em, 1);
        w->Be_flg |= 0x20;
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        w->Timer = 12;
        w->TmpU32 = 0;
        w->Landing_ck = 0;
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        w->Timer3++;
        em->dmg.set(0, 2);
        w->Be_flg |= 0x80000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                w->TmpU32 = 1;
                if (pG->room_id == 0x311) {
                    EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                    SndCall(6, 0xA, &em->pos, 0, 0, em);
                } else {
                    EstSetEm10WaterFall((Vec*) em);
                    SndCall(6, 0x16, &em->pos, 0, 0, em);
                }
            }
        }
        if (w->Timer) {
            w->Timer--;
            v = em->pos;
            v.y = em->pos_old.y;
            y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
            if (em->pos.y < y + 50.0f) {
                em->pos.y = y;
            }
        } else {
            v = em->pos;
            v.y = em->pos_old.y;
            y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
            v.x += 10.0f;
            if (em->pos.y < y) {
                em->pos.y = y;
                w->Spd.y = 0.0f;
                em->r_no_2++;
                MotionMove(em, 0);
                if (w->Timer3 > 30) {
                    em->hp = 0;
                } else {
                    SndCall(8, 0x77, &em->pos, em->id, 0, em);
                    if (ChkWaterEffectEnable(&em->pos)) {
                        EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, 0, 0);
                    } else {
                        EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, 0, 0);
                    }
                }
                w->Be_flg &= ~0x80000;
                break;
            }
        }
        DmgMgr.set(DMG_TYPE_PUSH, 2, &em->pos, 1500.0f, 800.0f);
        break;
    case 2:
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B6), 0, 3, 1, 0);
        dmg = w->Timer3 * 50;
        if (em->type == 6) {
            dmg = 0;
        }
        LifeDownSet2(em, dmg, 0, 0);
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x13 Dm_Heel: Wesker's heel kick (pl_type 5): motion 0x2B7, a kill loses the head
// and drops the weapon / shield, scores a critical; then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00460050
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, GameAddPoint, LVADD_CRITICALHIT, SndStop, Timer4, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_Heel(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x2B7), PL_ARC_PTR(em->subArc, 0x2B8), 3, 1, 0);
        w->Be_flg &= ~0x20;
        if (em->hp <= 0) {
            em10LostHead(em, 1, 0);
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
        }
        if (!(w->Be_flg & 0x80)) {
            em10CallVoiceSe2(em, w->Se_tbl[10], 8);
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        GameAddPoint(LVADD_CRITICALHIT);
        w->Timer4 = 10;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 0);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x02 Dm_DashUp: shot in the body while dashing: the stumble 0x57 (mirrored at
// random) and straight back to the walk.
// FUNCTION: BIO4 0x00477ED0
// STATUS: stub
#if 0 // PARKED: SndStop, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_DashUp(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = (Rnd() & 1) ? 1 : 0x41;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x57), PL_ARC_PTR(em->subArc, 0x58), 6, flag, 0);
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            em10WalkRtnSet(em);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x03 Dm_DashDown: shot in the legs while dashing: trips and falls (motion 0x59, dust /
// splash on landing), then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00441660
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, SndStop, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_DashDown(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = 1;
        if (em->dmg.m_pDamageYarare->parts_no == 0x13) {
            flag = 0x41;
        }
        if (em->dmg.m_pDamageYarare->parts_no == 0x14) {
            flag = 0x41;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x59), PL_ARC_PTR(em->subArc, 0x5A), 3, flag, 0);
        w->Be_flg &= ~0x20;
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x04 Dm_Blow: blown off the feet (shotgun near hit, heavy weapons, melee, explosions):
// picks a forward / backward fall (0x3B..0x4E by hit direction, hp and chance), airborne (0x80000)
// until the floor, falls over ledges with the water / dust effects (0x5B.. drop motions, landing
// damage), then Die_Cramp or DownWakeWait; Landing_ck marks the landing.
// FUNCTION: BIO4 0x00441890
// STATUS: stub
#if 0 // PARKED: C1003, C2227, EFF_ROOM, EM_STATUS_IK_OFF, LIMIT_ANGLE, MotionAddSpeed, MotionGetSpeed, Muku, PI, PSVECScale, SndCall, SndStop, Timer3, ckAtkEnable, int, room_id, set, setDamage, vB8, vC0
void em10_R1_Dm_Blow(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;
    f32 ang;
    Vec v;
    Vec spd;
    Vec rot;
    f32 y;
    cModel* p;
    int dmg;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        w->Timer = 0;
        if (em->r_no_3) {
            w->Timer = 1;
        }
        em->r_no_3 = Rnd() & 1;
        flag = em->r_no_3 ? 1 : 0x41;
        ang = fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI));
        w->TmpF = 1.0f;
        if ((u8) (Rnd() % 10) > 6 || em->hp > 0) {
            if (ang < 1.5707964f) {
                if ((Rnd() & 1) || (w->Be_flg & 0x80)) {
                    MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x43), PL_ARC_PTR(em->subArc, 0x44), 3, flag, 0);
                    em->ang.y += Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI);
                    w->Be_flg &= ~0x20;
                    w->Spd.x = 0.0f;
                    w->Spd.y = -100.0f;
                    w->Spd.z = -100.0f;
                } else {
                    MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x3F), PL_ARC_PTR(em->subArc, 0x40), 3, flag, 0);
                    em->ang.y += Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI);
                    w->Be_flg |= 0x20;
                    w->Spd.x = 0.0f;
                    w->Spd.y = -100.0f;
                    w->Spd.z = -100.0f;
                }
            } else {
                if (Rnd() & 1) {
                    MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x4B), PL_ARC_PTR(em->subArc, 0x4C), 3, flag, 0);
                    em->ang.y += Muku(&em->dmg.m_PosFrom, &em->pos, em->ang.y, PI);
                    w->Be_flg |= 0x20;
                    w->Spd.x = 0.0f;
                    w->Spd.y = -100.0f;
                    w->Spd.z = 100.0f;
                } else {
                    MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x4D), PL_ARC_PTR(em->subArc, 0x4E), 3, flag, 0);
                    em->ang.y += Muku(&em->dmg.m_PosFrom, &em->pos, em->ang.y, PI);
                    w->Be_flg &= ~0x20;
                    w->Spd.x = 0.0f;
                    w->Spd.y = -100.0f;
                    w->Spd.z = 100.0f;
                }
            }
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        } else {
            if (ang < 1.5707964f && (u8) (Rnd() % 10) > 6) {
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x41), PL_ARC_PTR(em->subArc, 0x42), 3, flag, 0);
                w->Be_flg &= ~0x20;
                w->Spd.x = 0.0f;
                w->Spd.y = -200.0f;
                w->Spd.z = -100.0f;
            } else {
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x3B), PL_ARC_PTR(em->subArc, 0x3C), 3, flag, 0);
                w->Be_flg |= 0x20;
                if (em->r_no_3 == 1) {
                    w->TmpF = 2.0f;
                }
                w->Spd.x = 0.0f;
                w->Spd.y = -200.0f;
                w->Spd.z = -100.0f;
            }
            em->ang.y += Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI);
            em->ang.y = LIMIT_ANGLE(em->ang.y);
        }
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        w->TmpU32 = 0;
        w->Landing_ck = 0;
        w->Timer3 = 0;
        em->r_no_2++;
    case 1:
        if (w->Landing_ck == 0) {
            em->atari.m_flag |= 8;
            w->No_adj_timer = 2;
            em->dmg.set(0, 2);
        }
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 0x10) {
            w->Be_flg |= 0x80000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (em->Motion.Seq_old.Free & 2) {
            em10SetDmWaterEff(em, 1);
        }
        MotionGetSpeed(em, MOTION(em), 0, &spd, &rot);
        PSVECScale(&spd, &spd, w->TmpF);
        MotionAddSpeed(em, MOTION(em), &spd, &rot);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                w->TmpU32 = 1;
                if (pG->room_id == 0x311) {
                    EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                    SndCall(6, 0xA, &em->pos, 0, 0, em);
                } else {
                    EstSetEm10WaterFall((Vec*) em);
                    SndCall(6, 0x16, &em->pos, 0, 0, em);
                }
            }
        }
        v = em->pos;
        v.y = em->pos_old.y;
        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        if (w->Be_flg & 0x80000) {
            if (w->Landing_ck == 0) {
                if (em->pos.y < y + 50.0f) {
                    em->pos.y = y;
                }
            }
        }
        if (w->Landing_ck) {
            w->Be_flg &= ~0x80000;
        }
        if (MotionMove(em, 0)) {
            if (em->pos.y < -99000.0f) {
                em->hp = 0;
            }
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else {
            if (w->Timer) {
                p = em->getPartsPtr(0);
                if (!(em->Motion.Seq_old.Free & 0x80)) {
                    p->l_mat[1][3] *= 1.5f;
                }
            }
            if (em->Motion.Seq_old.Free & 4) {
                if (em->pos.y > y + 300.0f) {
                    em->r_no_2++;
                } else {
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = -100.0f;
                    PSMTXMultVec(em->mat, &v, &v);
                    v.y = em->pos_old.y;
                    y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
                    if (em->pos.y > y + 300.0f) {
                        em->r_no_2++;
                    } else {
                        v.x = 50.0f;
                        v.y = 0.0f;
                        v.z = -50.0f;
                        PSMTXMultVec(em->mat, &v, &v);
                        v.y = em->pos_old.y;
                        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
                        if (em->pos.y > y + 300.0f) {
                            em->r_no_2++;
                        } else {
                            w->Landing_ck = 1;
                            w->Be_flg &= ~0x80000;
                        }
                    }
                }
            }
        }
        break;
    case 2:
        flag = em->r_no_3 ? 1 : 0x41;
        if (w->Be_flg & 0x20) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x5B), PL_ARC_PTR(em->subArc, 0x5E), 3, flag, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x53), 0, 3, flag, 0);
        }
        em->r_no_2++;
    case 3:
        em->atari.m_flag |= 8;
        w->No_adj_timer = 2;
        w->Timer3++;
        em->dmg.set(0, 2);
        if (em->Motion.Seq_old.Free & 0x10) {
            w->Be_flg |= 0x80000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        w->Spd.y -= 20.0f;
        PSMTXMultVecSR(em->mat, &w->Spd, &v);
        PSVECAdd(&em->pos, &v, &em->pos);
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                w->TmpU32 = 1;
                if (pG->room_id == 0x311) {
                    EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                    SndCall(6, 0xA, &em->pos, 0, 0, em);
                } else {
                    EstSetEm10WaterFall((Vec*) em);
                    SndCall(6, 0x16, &em->pos, 0, 0, em);
                }
            }
        }
        v = em->pos;
        v.y = em->pos_old.y;
        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        v.x += 10.0f;
        if (em->pos.y < y) {
            em->pos.y = y;
            w->Spd.y = 0.0f;
            em->r_no_2++;
            MotionMove(em, 0);
            if (w->Timer3 > 60) {
                em->hp = 0;
            } else {
                SndCall(8, 0x77, &em->pos, em->id, 0, em);
                if (ChkWaterEffectEnable(&em->pos)) {
                    EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, 0, 0);
                } else {
                    EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, 0, 0);
                }
            }
            w->Be_flg &= ~0x80000;
        } else {
            MotionMove(em, 0);
        }
        break;
    case 4:
        flag = em->r_no_3 ? 1 : 0x41;
        if (w->Be_flg & 0x20) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x5B), PL_ARC_PTR(em->subArc, 0x5D), 3, flag, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x54), 0, 3, flag, 0);
        }
        dmg = w->Timer3 * 50;
        if (em->type == 6) {
            dmg = 0;
        }
        LifeDownSet2(em, dmg, 0, 0);
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        em->r_no_2++;
    case 5:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else if (em->r_no_3 == 2) {
            p = em->getPartsPtr(0);
            if (!(em->Motion.Seq_old.Free & 0x80)) {
                p->l_mat[1][3] *= 1.5f;
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x05 Dm_Fence: shot while climbing over a fence / window (work flag 0x20000): falls
// off (motion 0x5B/0x5C), then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00442B00
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, SndStop, ckAtkEnable, set, setDamage, vB8, vC0
void em10_R1_Dm_Fence(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        em->r_no_3 = Rnd() & 1;
        flag = em->r_no_3 ? 1 : 0x41;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x5B), PL_ARC_PTR(em->subArc, 0x5C), 3, flag, 0);
        w->Be_flg |= 0x20;
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        em->r_no_2++;
    case 1:
        em->dmg.set(0, 2);
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x06 Dm_Ladder: shot or kicked off the ladder (work flag 0x10000): falls backwards
// (motion 0xCC, scream Se_tbl[14]) to the floor found below, the landing 0x5B/0x5D takes damage,
// scores a critical; then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00442DA0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, C2296, EM_STATUS_IK_OFF, GameAddPoint, LVADD_CRITICALHIT, LifeDownSet2, PSMTXMultVec, SatMgr, SndCall, SndStop, ckAtkEnable, int, mat, pos_old, set, setDamage, vB8, vC0
void em10_R1_Dm_Ladder(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;
    Vec v;
    Vec a;
    Vec b;
    f32 y;
    int dmg;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        em->r_no_3 = Rnd() & 1;
        flag = em->r_no_3 ? 1 : 0x41;
        a.x = 0.0f;
        a.y = -1000.0f;
        a.z = -1500.0f;
        b.x = 2000.0f;
        b.y = -1000.0f;
        b.z = -1500.0f;
        PSMTXMultVec(em->mat, &a, &a);
        PSMTXMultVec(em->mat, &b, &b);
        if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
            flag = 1;
        }
        a.x = 0.0f;
        a.y = -1000.0f;
        a.z = -1500.0f;
        b.x = -2000.0f;
        b.y = -1000.0f;
        b.z = -1500.0f;
        PSMTXMultVec(em->mat, &a, &a);
        PSMTXMultVec(em->mat, &b, &b);
        if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
            flag = 0x41;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0xCC), PL_ARC_PTR(em->subArc, 0xCD), 3, flag, 0);
        w->Be_flg |= 0x20;
        em10SetDamageVoice(em, w->Se_tbl[14], w->Se_tbl[14]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        w->Timer = 0;
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        GameAddPoint(LVADD_CRITICALHIT);
        w->TmpU32 = 0;
        em->r_no_2++;
    case 1:
        em->dmg.set(0, 2);
        w->Be_flg |= 0x80000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (MotionMove(em, 0)) {
            w->TmpU32 = 1;
        }
        if (!(em->Motion.Seq_old.Free & 4)) {
            break;
        }
        if (w->TmpU32) {
            em->pos.y -= 1000.0f;
        }
        w->Timer++;
        v = em->pos;
        v.y = em->pos_old.y;
        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        if (em->pos.y > y) {
            break;
        }
        em->pos.y = y;
        w->Spd.y = 0.0f;
        em->r_no_2++;
    case 2:
        flag = em->r_no_3 ? 1 : 0x41;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x5B), PL_ARC_PTR(em->subArc, 0x5D), 3, flag, 0);
        dmg = w->Timer * 50;
        if (em->type == 6) {
            dmg = 0;
        }
        LifeDownSet2(em, dmg, 0, 0);
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        SndCall(8, 0x77, &em->pos, em->id, 0, em);
        if (ChkWaterEffectEnable(&em->pos)) {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, 0, 0);
        } else {
            EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, 0, 0);
        }
        em->r_no_2++;
    case 3:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (em->Motion.Seq_old.Free & 1) {
            em10FallWaterCk(em);
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x07 Dm_Roof: falls off the roof / gondola / dragon statue it stood on (motion 0x5F /
// 0x61 by r_no_3, turned to Target_dir): flies with flags 0x81000, leaves the gondola (setGetOffEm),
// takes 50 damage per airborne frame on landing (0x63), then Die_Cramp or DownWakeWait; the room
// 10F fall sets Status_flg[1] bit17.
// FUNCTION: BIO4 0x004433D0
// STATUS: stub
#if 0 // PARKED: C1003, C2105, C2227, C2228, C2296, EFF_ROOM, EM_STATUS_IK_OFF, EstSetEm10WaterFall, LIMIT_ANGLE, LifeDownSet2, Muku2, PI, PSVECAdd, PSVECSubtract, STA_CRITICAL, SatMgr, SndCall, SndStop, StaFlagOn, Timer2, Timer3, ckAtkEnable, int, pos_old, room_id, set, setDamage, vB8, vC0
void em10_R1_Dm_Roof(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;
    Vec v;
    Vec d;
    f32 y;
    f32 a;
    int mv;
    cModel* p;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        if (em->r_no_3) {
            em->r_no_3 = Rnd() & 1;
            flag = em->r_no_3 ? 1 : 0x41;
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x61), PL_ARC_PTR(em->subArc, 0x62), 3, flag, 0);
        } else {
            em->r_no_3 = Rnd() & 1;
            flag = em->r_no_3 ? 1 : 0x41;
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x5F), PL_ARC_PTR(em->subArc, 0x60), 3, flag, 0);
        }
        w->Be_flg &= ~0x20;
        em10SetDamageVoice(em, w->Se_tbl[14], w->Se_tbl[14]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->flag &= ~0x400;
        w->Timer2 = 15;
        w->Timer = 0;
        w->TmpU32 = 0;
        em10SetDmWaterEff(em, 1);
        SndStop(w->Seid_csaw, 0);
        w->TmpF = Muku2(em->ang.y, w->Target_dir, PI);
        w->Timer3 = 20;
        em->r_no_2++;
    case 1:
        em->dmg.set(0, 2);
        em->flag &= ~0x400;
        a = w->TmpF * 0.1f;
        em->ang.y += a;
        em->ang.y = LIMIT_ANGLE(em->ang.y);
        w->TmpF -= a;
        if (w->Timer2) {
            w->Timer2--;
            if (w->Timer2 == 0) {
                StaFlagOn(pG, STA_CRITICAL);
            }
        }
        if (w->Timer3) {
            w->Timer3--;
            if (em->r_no_3 == 2 && w->pGondola) {
                p = w->pGondola->getPartsPtr(0);
                PSVECSubtract(&p->world, &p->world_old2, &d);
                d.y = 0.0f;
                PSVECAdd(&em->pos, &d, &em->pos);
            }
        } else if (w->pGondola) {
            w->pGondola->setGetOffEm(em);
            w->pGondola = 0;
        }
        w->Be_flg |= 0x80000;
        w->Be_flg |= 0x1000;
        em->setStatus(EM_STATUS_IK_OFF);
        mv = MotionMove(em, 0);
        if (em->Motion.Seq_old.Free & 4) {
            w->Timer++;
            v = em->pos;
            v.y = em->pos_old.y;
            y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
            if (w->TmpU32 == 0) {
                em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                w->TmpU32 = 1;
                if (pG->room_id == 0x311) {
                    EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                    SndCall(6, 0xA, &em->pos, 0, 0, em);
                } else {
                    EstSetEm10WaterFall((Vec*) em);
                    SndCall(6, 0x16, &em->pos, 0, 0, em);
                }
            }
            }
            if (em->pos.y < y) {
                em->pos.y = y;
                w->Spd.y = 0.0f;
                flag = em->r_no_3 ? 1 : 0x41;
                MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x63), PL_ARC_PTR(em->subArc, 0x64), 3, flag, 0);
                MotionMove(em, 0);
                em->r_no_2 = 4;
            } else if (mv) {
                em->r_no_2++;
            }
        }
        break;
    case 2:
        w->Spd.x = 0.0f;
        w->Spd.y = -300.0f;
        w->Spd.z = 0.0f;
        if (w->pGondola) {
            w->pGondola->setGetOffEm(em);
            w->pGondola = 0;
        }
        em->r_no_2++;
    case 3:
        w->Timer++;
        em->dmg.set(0, 2);
        w->Be_flg |= 0x80000;
        w->Be_flg |= 0x1000;
        em->setStatus(EM_STATUS_IK_OFF);
        PSVECAdd(&em->pos, &w->Spd, &em->pos);
        w->Spd.y -= 20.0f;
        if (w->TmpU32 == 0) {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                w->TmpU32 = 1;
                if (pG->room_id == 0x311) {
                    EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                    SndCall(6, 0xA, &em->pos, 0, 0, em);
                } else {
                    EstSetEm10WaterFall((Vec*) em);
                    SndCall(6, 0x16, &em->pos, 0, 0, em);
                }
            }
            if (pG->room_id == 0x222 && em->pos.y <= -9900.0f) {
                d = em->pos;
                d.y = -9900.0f;
                EstSet(0, -1, &d, 0, EFF_ROOM, 6, 0, ESP_CORE_KIND_NONE, 0, 0);
                w->TmpU32 = 1;
            }
        }
        MotionMove(em, 0);
        v = em->pos;
        v.y = em->pos_old.y;
        y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
        if (em->pos.y < y) {
            em->pos.y = y;
            w->Spd.y = 0.0f;
            flag = em->r_no_3 ? 1 : 0x41;
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x63), PL_ARC_PTR(em->subArc, 0x64), 3, flag, 0);
            MotionMove(em, 0);
            em->r_no_2 = 4;
        }
        break;
    case 4:
        LifeDownSet2(em, w->Timer * 50, 0, 0);
        if (em->pos.y < -99000.0f) {
            em->hp = 0;
        }
        if (w->pGondola) {
            w->pGondola->setGetOffEm(em);
            w->pGondola = 0;
        }
        if (w->Timer > 60) {
            em->hp = 0;
        } else {
            em10FallWaterCk(em);
            if (CheckInWater(em, 0)) {
                if (w->TmpU32 == 0) {
                    w->TmpU32 = 1;
                    if (pG->room_id == 0x311) {
                        EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
                        SndCall(6, 0xA, &em->pos, 0, 0, em);
                    } else {
                        EstSetEm10WaterFall((Vec*) em);
                        SndCall(6, 0x16, &em->pos, 0, 0, em);
                    }
                }
            } else {
                SndCall(8, 0x77, &em->pos, em->id, 0, em);
                if (ChkWaterEffectEnable(&em->pos)) {
                    EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x31, 0, ESP_CORE_KIND_NONE, 0, 0);
                } else {
                    EstSet(0, -1, &em->pos, &em->ang, EFF_EM10, 0x17, 0, ESP_CORE_KIND_NONE, 0, 0);
                }
            }
        }
        em->r_no_2++;
    case 5:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
    em10SetCrash(em, 800.0f);
}
#endif // PARKED

// R0 2 / R1 == 0x08 Dm_KneeDown: the kneeling Ganado (flag 0x40000000, rocket aim) shot down:
// motion 0x3D (front) / 0x49 (back), scores a critical, then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00443F60
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, GameAddPoint, LVADD_CRITICALHIT, Muku, PI, SndStop, Timer4, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_KneeDown(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        if (fabsf(Muku(&em->pos, &em->dmg.m_PosFrom, em->ang.y, PI)) < 1.5707964f) {
            em->r_no_3 = Rnd() & 1;
            flag = (MOTION(em)->Mot_attr & 0x40) ? 0x41 : 1;
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x3D), PL_ARC_PTR(em->subArc, 0x3E), 15, flag, 0);
            w->Be_flg |= 0x20;
        } else {
            em->r_no_3 = Rnd() & 1;
            flag = (MOTION(em)->Mot_attr & 0x40) ? 0x41 : 1;
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x49), PL_ARC_PTR(em->subArc, 0x4A), 15, flag, 0);
            w->Be_flg &= ~0x20;
        }
        if (em->hp <= 0) {
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
        }
        if (!(w->Be_flg & 0x80)) {
            em10CallVoiceSe2(em, w->Se_tbl[10], 8);
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        GameAddPoint(LVADD_CRITICALHIT);
        w->Timer4 = 10;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 0);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x09 Dm_KnockOut: collapses on the spot (motion 0x69: a Ganado dying while walking or
// an arm-shot knock-out), dust / splash on the ground, then Die_Cramp (dead) or DownWakeWait.
// FUNCTION: BIO4 0x00444290
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, SndStop, Timer4, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_KnockOut(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        em->r_no_3 = Rnd() & 1;
        flag = em->r_no_3 ? 1 : 0x41;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x69), PL_ARC_PTR(em->subArc, 0x6A), 15, flag, 0);
        w->Be_flg &= ~0x20;
        if (em->hp <= 0) {
            em->setWeaponFall();
            if (w->pShield) {
                w->pShield->setFall(0, 20.0f);
                w->pShield = 0;
            }
        }
        if (!(w->Be_flg & 0x80)) {
            em10CallVoiceSe2(em, w->Se_tbl[10], 8);
        }
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        w->Timer4 = 10;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 0);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x0A Dm_Down: shot while already down (work flag 0x10): the twitch motion 0x4F / 0x50,
// then DownWake (0x1F).
// FUNCTION: BIO4 0x004445E0
// STATUS: stub
#if 0 // PARKED: EM_STATUS_IK_OFF, ckAtkEnable, setDamage, vB8, vC0
void em10_R1_Dm_Down(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = (MOTION(em)->Mot_attr & 0x40) ? 0x41 : 1;
        if (w->Be_flg & 0x20) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x4F), 0, 5, flag, 0);
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x50), 0, 5, flag, 0);
        }
        if (w->pCore && w->pCore->ckAtkEnable()) {
            w->pCore->setDamage();
        }
        if (w->pParasite && w->pParasite->vB8()) {
            w->pParasite->vC0();
        }
        em->r_no_2++;
    case 1:
        w->Be_flg |= 0x10;
        w->Be_flg |= 0x1000000;
        em->setStatus(EM_STATUS_IK_OFF);
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 1, 0x1F, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x0B Dm_Frame: on fire (torch / incendiary, Frame_timer): burning motion 0x65 with
// the flame effect 0x24, 500 damage then 10 per frame; a burnt corpse burns its cap / core /
// tentacles too; then Die_Cramp or DownWakeWait.
// FUNCTION: BIO4 0x00444750
// STATUS: stub
#if 0 // PARKED: C2105, C2227, EM_STATUS_IK_OFF, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, LifeDownSet, LifeDownSet2, SndCall, SndStop, cModelInfo, cObj12, int, pModelInfo, setBurn
void em10_R1_Dm_Frame(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag;
    cModelInfo* info;
    u32 i;

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        flag = (em->flag & 0x1000000) ? 0x41 : 1;
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x65), PL_ARC_PTR(em->subArc, 0x66), 3, (u16) flag,
                      (u8) (Rnd() % 5));
        LifeDownSet(em, 500, 0);
        if (em->r_no_3 == 0) {
            EffectEspDelete(0, w->EffKindIdWork, em, 0);
            EffectEspgenDelete(0, w->EffKindIdWork, em);
            EffectEfmDelete(0, w->EffKindIdWork, em);
            EstSet(em, -1, 0, 0, EFF_EM10, 0x24, 0, ESP_CORE_KIND_NONE, em, 0);
        }
        SndStop(w->Seid_frame, 0);
        SndCall(8, 0x8E, &em->pos, em->id, 0, em);
        SndCall(8, 0x90, &em->pos, em->id, 0, em);
        em10CallVoiceSe2(em, w->Se_tbl[2], 8);
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        w->Timer = 50;
        w->Be_flg &= ~0x20;
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        } else {
            w->Be_flg &= ~8;
        }
        if (w->Timer) {
            w->Timer--;
            LifeDownSet2(em, 10, 0, 0);
        }
        if (em->hp <= 0) {
            for (info = em->pModelInfo; info; info = info->pList) {
                if (info->color[0] > 0x20) {
                    info->color[0] -= 0x20;
                }
                info->color[2] = info->color[1] = info->color[0];
            }
            if (w->pCap) {
                w->pCap->setBurn();
            }
            if (w->pCore) {
                w->pCore->setBurn();
            }
            for (i = 0; i < 5; i++) {
                if (w->pTen[i]) {
                    ((cObj16*) w->pTen[i])->setBurn();
                }
            }
        }
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 2 / R1 == 0x0C Dm_TakeAway: shot while carrying Ashley (flag 0x4000): drops her and falls
// (motion 0x59), then Die_Cramp / Die_Normal or DownWakeWait.
// FUNCTION: BIO4 0x00444BA0
// STATUS: stub
#if 0 // PARKED: SndStop
void em10_R1_Dm_TakeAway(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        w->Be_flg &= ~0x8000000;
        em10MouthPartsReset(em);
        if (w->Claw_rno_l != 4) {
            w->Claw_rno_l = 3;
        }
        if (w->Claw_rno_r != 4) {
            w->Claw_rno_r = 3;
        }
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x59), PL_ARC_PTR(em->subArc, 0x5A), 3, 1, 0);
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        w->Be_flg &= ~0x20;
        em->r_no_2++;
    case 1:
        if (MotionMove(em, 0)) {
            if (em->hp <= 0) {
                EmRoutineSet(em, 3, 0, 0, 1);
            } else {
                EmRoutineSet(em, 1, 0x1E, 0, 0);
            }
        } else if ((em->Motion.Seq_old.Free & 4) && em->hp <= 0) {
            EmRoutineSet(em, 3, 2, 0, 0);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 3 / R1 == 0x00 Die_Cramp: dead on the floor: scores the kill (em10SetPoint), drops the item
// (ITEMSET status), releases the weapon / shield / parasite (em10ParasiteGoOut, em10CoreBreak), then
// twitches until the corpse fades (Die_Lost 3, staggered by the ctrl12 EM10_LOST timer) or, with a
// lit dynamite, explodes (Die_Bomb 5).
// FUNCTION: BIO4 0x004507E0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, CTRL12_ID_EM10_LOST, Ctrl12Ck, Ctrl12CntAdd, Ctrl12Set, EM_STATUS_IK_OFF, EM_STATUS_ITEMSET, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, EmReserveDropItem, EmSetDie, EmSetDieCnt, EmSetDropItem, int, room_no, setLostWait, stage_no
void em10_R1_Die_Cramp(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int f;

    w->Be_flg |= 0x10;
    w->Be_flg |= 0x1000000;
    em->setStatus(EM_STATUS_IK_OFF);
    switch (em->r_no_2) {
    case 0:
        em10MouthPartsReset(em);
        em10SetPoint(em);
        EmSetDie(em);
        if (em->type != 6) {
            EmSetDieCnt(em);
            EmReserveDropItem(em);
        }
        em->clearStatus(EM_STATUS_ACTIVE);
        em->setStatus(EM_STATUS_ITEMSET);
        EmSetDropItem(em);
        em->atari.m_flag &= ~0x200;
        w->Timer = 0;
        if (w->pParasite) {
            w->Timer = 60;
        }
        if (w->Parasite_on) {
            w->Parasite_on = 0;
            Ctrl12CntAdd(w->pCtrlGroup, 4, -1);
        }
        if (em->type == 0xA || em->type == 0xD) {
            if (w->pCore) {
                EffectEspDelete(0, w->EffKindIdCore, w->pCore, 0);
                EffectEspgenDelete(0, w->EffKindIdCore, w->pCore);
                EffectEfmDelete(0, w->EffKindIdCore, w->pCore);
                w->pCore->setLostWait(0);
                w->pCore = 0;
            }
        }
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em10ParasiteGoOut(em);
        em10CoreBreak(em, 0);
        em->r_no_2++;
    case 1:
        MotionMove(em, 0);
        if (w->Fire_timer) {
            EmRoutineSet(em, 3, 5, 0, 0);
            return;
        }
        if (w->Timer) {
            w->Timer--;
        } else if (!Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_LOST)) {
            f = 0;
            if (pG->stage_no == 1 && pG->room_no == 0) {
                f = 1;
            }
            if (em->flag & 0x10000000) {
                f = 1;
            }
            if (w->Ganado == 1 && (em->flag & 0x100)) {
                f = 1;
            }
            if (em->type == 6) {
                f = 1;
            }
            if (em->type == 2) {
                f = 1;
            }
            if (em->type == 0xA) {
                f = 1;
            }
            if (em->type == 0xD) {
                f = 1;
            }
            if (em->set == 0x39) {
                f = 1;
            }
            if (pG->stage_no > 3) {
                f = 0;
            }
            if (f) {
                em->r_no_2++;
            } else {
                EmRoutineSet(em, 3, 3, 0, 0);
                Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_LOST, (u8) (Rnd() % 10) + 15);
            }
        }
        break;
    case 2:
        MotionMove(em, 0);
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 3 / R1 == 0x03 Die_Lost: the corpse dissolves: the death effect (0 / 0x58 by model type, 0x19 /
// 0x59 for the burnt variant, 0x35 in water), then fades out (invisible_factor -0.1 per frame), drops
// the weapons, breaks the core and hides itself (flag 0x400000, no collision).
// FUNCTION: BIO4 0x00450C80
// STATUS: stub
#if 0 // PARKED: C1003, C2105, EFF_ROOM, EM_STATUS_ITEMSET, EmReserveDropItem, EmSetDie, SndCall, Timer2, Timer3, TransMatrix, int, invisible_factor, mat, setLost, setReset
void em10_R1_Die_Lost(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Be_flg |= 0x400;
    switch (em->r_no_2) {
    case 0:
        em->atari.m_flag &= ~0x300;
        em->atari.m_flag |= 0x10;
        em10SetPoint(em);
        EmSetDie(em);
        EmReserveDropItem(em);
        em->clearStatus(EM_STATUS_ACTIVE);
        if (CheckInWater(em, 0)) {
            EstSet(em, -1, 0, 0, EFF_ROOM, 0x35, 0, ESP_CORE_KIND_NONE, em, 0);
        } else if (w->Be_flg & 0x20) {
            switch (em->type) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 0xA:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x17:
            case 0x18:
            case 0x19:
                EstSet(em, -1, 0, 0, EFF_EM10, 0, 0, ESP_CORE_KIND_NONE, em, 0);
                break;
            case 2:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0xB:
            case 0xC:
                EstSet(em, -1, 0, 0, EFF_EM10, 0x58, 0, ESP_CORE_KIND_NONE, em, 0);
                break;
            }
        } else {
            switch (em->type) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 0xA:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x17:
            case 0x18:
            case 0x19:
                EstSet(em, -1, 0, 0, EFF_EM10, 0x19, 0, ESP_CORE_KIND_NONE, em, 0);
                break;
            case 2:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0xB:
            case 0xC:
                EstSet(em, -1, 0, 0, EFF_EM10, 0x59, 0, ESP_CORE_KIND_NONE, em, 0);
                break;
            }
        }
        em10CoreBreak(em, 1);
        if (w->pParasite) {
            w->pParasite->setReset();
            w->pParasite = 0;
        }
        w->Timer = 15;
        w->Timer2 = 0x35;
        w->Timer3 = 1;
        if (w->Wep_type == 4) {
            w->Timer3 = 60;
        }
        SndCall(8, 0x44, &em->pos, em->id, 0, em);
        w->Compress_y = 1.0f;
        em->r_no_2++;
    case 1:
        if (w->Timer3) {
            w->Timer3--;
            if (w->Timer3 == 0) {
                em->setStatus(EM_STATUS_ITEMSET);
            }
        }
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Compress_y -= 0.028f;
            if (w->Compress_y < 0.1f) {
                w->Compress_y = 0.1f;
            }
            em->pos.y -= 6.0f;
        }
        TransMatrix(em->mat, &em->pos);
        if (w->Timer2) {
            w->Timer2--;
        } else {
            em->invisible_factor -= 0.1f;
            if (em->invisible_factor <= 0.0f) {
                em->invisible_factor = 0.0f;
                if (w->pWeapon) {
                    w->pWeapon->setLost();
                    w->pWeapon = 0;
                    w->Wep_type = 0;
                }
                if (w->pWeapon2) {
                    w->pWeapon2->setLost();
                    w->pWeapon2 = 0;
                    w->Wep_type2 = 0;
                }
                em10CoreBreak(em, 1);
                if (w->pParasite) {
                    w->pParasite->setReset();
                    w->pParasite = 0;
                }
                em->be_flag &= ~2;
                w->Reset_enable = 1;
                w->Be_flg |= 0x400000;
                em->r_no_2++;
            }
        }
        break;
    }
}
#endif // PARKED

// R0 3 / R1 == 0x01 Die_Down: dies while lying down (motion 0x6B / 0x6C by side), weapon dropped,
// then Die_Cramp after the death voice.
// FUNCTION: BIO4 0x00444D00
// STATUS: stub
#if 0 // PARKED: C2105, C2227, EM_STATUS_IK_OFF, SndCall, int
void em10_R1_Die_Down(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Be_flg |= 0x10;
    w->Be_flg |= 0x1000000;
    em->setStatus(EM_STATUS_IK_OFF);
    switch (em->r_no_2) {
    case 0:
        em10MouthPartsReset(em);
        em10SetPoint(em);
        if (w->Be_flg & 0x20) {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x6B), 0, 6, 1, 0);
            w->Timer = 0x46;
        } else {
            MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x6C), 0, 6, 1, 0);
            w->Timer = 0x4A;
        }
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                SndCall(8, 4, &em->pos, em->id, 0, em);
            }
        }
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 3, 0, 0, 1);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 3 / R1 == 0x02 Die_Normal: the standing death (collapse motion 0x67, death voice, weapon and
// shield dropped, the core's lost wait); lands with the dust / splash effect and goes to Die_Cramp
// (or waits 60 frames as a fixed corpse for the special sets).
// FUNCTION: BIO4 0x00444EF0
// STATUS: stub
#if 0 // PARKED: C2105, C2227, EM_STATUS_IK_OFF, EM_STATUS_ITEMSET, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, EmReserveDropItem, EmSetDropItem, LIMIT_ANGLE, SndCall, SndStop, fRand1_1, int, setLostWait
void em10_R1_Die_Normal(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec v;

    switch (em->r_no_2) {
    case 0:
        em10MouthPartsReset(em);
        em10SetPoint(em);
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x67), PL_ARC_PTR(em->subArc, 0x68), 15, 1, 0);
        w->Be_flg &= ~0x20;
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        if (em->type != 6) {
            EmReserveDropItem(em);
        }
        w->Timer = 0x49;
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        if (em->type == 0xA || em->type == 0xD) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x85, 0, ESP_CORE_KIND_NONE, em, 0);
            SndCall(8, 0x44, &em->pos, em->id, 0, em);
            if (w->pCore) {
                EffectEspDelete(0, w->EffKindIdCore, w->pCore, 0);
                EffectEspgenDelete(0, w->EffKindIdCore, w->pCore);
                EffectEfmDelete(0, w->EffKindIdCore, w->pCore);
                w->pCore->setLostWait(100);
                w->pCore = 0;
            }
        }
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            v = em->ang;
            v.y += fRand1_1() * 3.1415927f;
            v.y = LIMIT_ANGLE(v.y);
            EstSet(0, -1, &em->pos, &v, EFF_EM10, 0x18, 0, ESP_CORE_KIND_NONE, 0, 0);
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 3, 0, 0, 1);
        } else if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                em->clearStatus(EM_STATUS_ACTIVE);
                em->setStatus(EM_STATUS_ITEMSET);
                EmSetDropItem(em);
                em->be_flag |= 0x10000;
            }
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 3 / R1 == 0x04 Die_RunDown: dies while dashing (the trip motion 0x59), lands with the ground
// effect, then Die_Cramp.
// FUNCTION: BIO4 0x00445330
// STATUS: stub
#if 0 // PARKED: C2227, EM_STATUS_IK_OFF, SndStop
void em10_R1_Die_RunDown(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    switch (em->r_no_2) {
    case 0:
        em10MouthPartsReset(em);
        em10SetPoint(em);
        MotionSetCore(em, MOTION(em), PL_ARC_PTR(em->subArc, 0x59), PL_ARC_PTR(em->subArc, 0x5A), 3, 1, 0);
        em->hp = 0;
        w->Be_flg &= ~0x20;
        em10SetDamageVoice(em, w->Se_tbl[8], w->Se_tbl[0]);
        em10SetDmWaterEff(em, 0);
        SndStop(w->Seid_csaw, 0);
        em->setWeaponFall();
        if (w->pShield) {
            w->pShield->setFall(0, 20.0f);
            w->pShield = 0;
        }
        em->r_no_2++;
    case 1:
        if (em->Motion.Seq_old.Free & 0x80) {
            w->Be_flg |= 0x10;
            w->Be_flg |= 0x1000000;
            em->setStatus(EM_STATUS_IK_OFF);
        }
        if (em->Motion.Seq_old.Free & 1) {
            if (ChkWaterEffectEnable(&em->pos)) {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x32, 0, ESP_CORE_KIND_NONE, em, 0);
            } else {
                EstSet(em, -1, 0, 0, EFF_EM10, 0x1A, 0, ESP_CORE_KIND_NONE, em, 0);
            }
        }
        if (MotionMove(em, 0)) {
            EmRoutineSet(em, 3, 0, 0, 1);
        }
        break;
    }
    em10HandSet(em, 0);
}
#endif // PARKED

// R0 3 / R1 == 0x05 Die_Bomb: blown up (dynamite / explosive weapon): drops everything, scores, and
// after the timer bursts (effect 0x9B and the gib effects 0x2A / 0x47 / 0x30, SEs 0x96 / 8) with a
// 6000-unit blast on the player (PlWepHitCheck2 0x13); the body is hidden at once.
// FUNCTION: BIO4 0x00451120
// STATUS: stub
#if 0 // PARKED: C2105, C2227, C2228, Camera, EM_STATUS_ITEMSET, EmReserveDropItem, EmSetDie, EmSetDieCnt, EmSetDropItem, ObjMgr, PlWepHitCheck2, SndCall, SndStop, Timer2, cam, int, isAlive, setLost, setReset
void em10_R1_Die_Bomb(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cModel* p;
    Camera* cam;
    f32 dx, dy;
    Vec rot;

    switch (em->r_no_2) {
    case 0:
        em10SetPoint(em);
        if (w->Fire_timer) {
            w->Fire_timer = 0;
        }
        if (w->pWeapon) {
            w->pWeapon->setLost();
            w->pWeapon = 0;
            w->Wep_type = 0;
        }
        if (w->pParasite) {
            w->pParasite->setReset();
            w->pParasite = 0;
        }
        em->hp = 0;
        SndStop(w->Seid_csaw, 0);
        em->atari.m_flag &= ~0x300;
        em->atari.m_flag |= 0x10;
        EmSetDie(em);
        EmReserveDropItem(em);
        em10SetPoint(em);
        if (em->type != 6) {
            EmSetDieCnt(em);
        }
        em->clearStatus(EM_STATUS_ACTIVE);
        em->setStatus(EM_STATUS_ITEMSET);
        EmSetDropItem(em);
        w->Timer = 1;
        w->Timer2 = 3;
        SndCall(8, 0x96, &em->pos, em->id, 0, em);
        SndCall(8, 8, &em->pos, em->id, 0, em);
        MotionMove(em, 0);
        em->r_no_2++;
    case 1:
        if (w->Timer) {
            w->Timer--;
            if (w->Timer == 0) {
                if (em->r_no_3 == 2) {
                    EstSet(em, -1, 0, 0, EFF_EM10, 0x9B, 0, ESP_CORE_KIND_NONE, em, 0);
                } else {
                    p = em->getPartsPtr(0);
                    if (w->Be_flg & 0x1400000) {
                        EstSet(0, -1, &p->world, 0, EFF_EM10, 0x2A, 0, ESP_CORE_KIND_NONE, 0, 0);
                    } else {
                        cam = &pG->Camera;
                        dx = cam->param.pos.x - p->world.x;
                        dy = cam->param.pos.y - p->world.y;
                        if (dx * dx + dy * dy + (cam->param.pos.z - p->world.z) * (cam->param.pos.z - p->world.z) < 4000000.0f) {
                            rot.x = 0.0f;
                            rot.y = GetXZAngle(&p->world, &cam->param.pos);
                            rot.z = 0.0f;
                            EstSet(0, -1, &em->pos, &rot, EFF_EM10, 0x47, 0, ESP_CORE_KIND_NONE, 0, 0);
                        } else {
                            EstSet(em, -1, 0, 0, EFF_EM10, 0x30, 0, ESP_CORE_KIND_NONE, em, 0);
                        }
                    }
                }
                if (em->r_no_3 == 0) {
                    PlWepHitCheck2(0, &em->pos, &em->pos, 0x13, 2, 6000.0f);
                }
            }
        }
        if (w->Timer2) {
            w->Timer2--;
        } else {
            em10CoreBreak(em, 2);
            if (w->pParasite) {
                w->pParasite->setReset();
                w->pParasite = 0;
            }
            if (w->pGunBelt) {
                if (w->pGunBelt->isAlive()) {
                    ObjMgr.destroy((cObj*) w->pGunBelt);
                }
                w->pGunBelt = 0;
            }
            if (w->pChain) {
                if (w->pChain->isAlive()) {
                    ObjMgr.destroy((cObj*) w->pChain);
                }
                w->pChain = 0;
            }
            w->Be_flg |= 0x400000;
            em->be_flag &= ~2;
            em->r_no_2++;
        }
        break;
    case 2:
        w->Timer = 90;
        em->r_no_2++;
    case 3:
        if (w->Timer) {
            w->Timer--;
        } else {
            w->Reset_enable = 1;
        }
        break;
    }
}
#endif // PARKED

// Every frame from cEm10::move. Computes the route points and angles to the player (Pl_pos / Pl_dir /
// Pl_rot, L_pl_route), the partner (Sub_*), the guard post (L_pl_guard / L_guard from Keep_pos) and
// the goto target (Go_* from Route_target, the wander route or the lock-on target), the line of sight
// (flag bit0 player seen, bit1 partner seen) alternating the probe side, and picks the route target
// (em10RouteTargetSet: flag 0x08000000 = after the partner); Route_type offsets the approach point.
// Route bookkeeping run every frame: distances / angles to the player, partner and goto point.
// FUNCTION: BIO4 0x00451630
// STATUS: stub
#if 0 // PARKED: C2228, EM10_ROUTE_LOCKON, EM10_ROUTE_SIGHT_CK, Frame_cnt, Muku, PSMTXMultVec, PSVECSubtract, SQRTF, SatMgr, l_pl, mat
void em10RouteCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec p;
    Vec q;
    Vec b;
    Vec c;
    Vec r;
    Vec nrm;
    Vec dbg;
    f32 d;
    f32 ang;
    f32 aa;
    f32 dy;
    int fl;
    int t;

    if (em->hp <= 0) {
        return;
    }
    if (em->type == 0xA || em->type == 0xD) {
        w->Be_flg |= 4;
    }
    if (em->r_no_0 != 0 && !(w->Be_flg & 4) && (pG->Frame_cnt & 7) != (em->emset_no & 7)) {
        em10RouteTargetSet(em);
        EM10_ROUTE_LOCKON();
        return;
    }
    w->Go_target = 0;
    if (w->Be_flg & 0x8000) {
        w->Be_flg &= ~0x08000000;
        w->Pl_pos = pPL->pos;
        w->Pl_dir = Muku(&em->pos, &w->Pl_pos, em->ang.y, 3.1415927f);
        w->Pl_rot = fabsf(w->Pl_dir);
        EM10_ROUTE_SIGHT_CK();
        w->Go_pos = w->Pl_pos;
        w->Go_dir = w->Pl_dir;
        w->Go_rot = w->Pl_rot;
        w->L_go = em->l_pl;
        em10RouteTargetSet(em);
        return;
    }
    d = SQRTF(em->l_pl);
    if (d > 4000.0f) {
        d = 4000.0f;
    }
    d *= 0.00025f;
    if (w->pShield || em->type == 0xA || em->type == 0xD) {
        switch ((u8) (em->emset_no % 3)) {
        case 0:
        default:
            w->Route_type = 0;
            break;
        case 1:
            w->Route_type = 7;
            break;
        case 2:
            w->Route_type = 8;
            break;
        }
    }
    switch (w->Route_type) {
    case 0:
    default:
        p.x = 0.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 1:
        p.x = d * 2000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 2:
        p.x = d * -2000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 3:
        p.x = d * 3000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 4:
        p.x = d * -3000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 5:
        p.x = d * 4000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 6:
        p.x = d * -4000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 7:
        p.x = d * 1000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 8:
        p.x = d * -1000.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 9:
        p.x = d * 2500.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    case 10:
        p.x = d * -2500.0f;
        p.y = 500.0f;
        p.z = 0.0f;
        break;
    }
    PSMTXMultVec(pPL->mat, &p, &p);
    r = pPL->pos;
    r.y += 500.0f;
    if (SatMgr.hitCheck(&r, &p, &q, 0, 0, 0)) {
        PSVECSubtract(&r, &q, &nrm);
#line 25754 "D:/Bio4/Prog/em10.cpp"
        VECNormalize(&nrm, &nrm);
        PSVECScale(&nrm, &nrm, 350.0f);
        PSVECAdd(&q, &nrm, &p);
        if (DbgFlagChk(pG, DBG_RTP_DISP)) {
            Draw_line3d(&r, &q, 0xFFFFFFFF, 0);
            Draw_line3d(&r, &p, 0xFF00FF00, 0);
        }
    }
    r = p;
    if (em->l_pl < 12250000.0f && (s16) w->Anger_timer == 0) {
        if ((s16) w->Back_wait <= 0x12B) {
            ang = Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f);
            if (ang > 0.0f) {
                if (ang > 0.7853982f) {
                    ang -= 0.7853982f;
                } else {
                    ang = 0.0f;
                }
            } else {
                if (ang < -0.7853982f) {
                    ang += 0.7853982f;
                } else {
                    ang = 0.0f;
                }
            }
            aa = fabsf(ang);
            r.x = ang * 3500.0f;
            r.y = 500.0f;
            r.z = aa * 3000.0f;
            PSMTXMultVec(pPL->mat, &r, &r);
            p = pPL->pos;
            p.y += 500.0f;
            if (SatMgr.hitCheck(&p, &r, &q, 0, 0, 0)) {
                PSVECSubtract(&p, &q, &nrm);
#line 25789 "D:/Bio4/Prog/em10.cpp"
                VECNormalize(&nrm, &nrm);
                PSVECScale(&nrm, &nrm, 350.0f);
                PSVECAdd(&q, &nrm, &r);
            }
            if (aa < 0.3926991f) {
                w->Back_wait = 0;
            } else {
                w->Back_wait++;
            }
        }
    } else {
        w->Back_wait = 0;
    }
    w->L_pl_route = RouteCkPosToPosDis(&em->pos, &pPL->pos);
    w->L_pl_guard = RouteCkPosToPosDis(&w->Keep_pos, &pPL->pos);
    w->L_guard = RouteCkPosToPosDis(&w->Keep_pos, &em->pos);
    if ((em->type == 0xA || em->type == 0xD) && w->Goto_mode == 0 && (w->Be_flg & 0x100)) {
        r = w->Goto_pos;
    }
    w->Be_flg &= ~3;
    fl = 0;
    if (pPL->pos.y > em->pos.y + 1000.0f) {
        fl = 1;
    }
    RouteCkToPos(em, &r, &w->Pl_pos, fl, &w->Route_h);
    w->Pl_dir = Muku(&em->pos, &w->Pl_pos, em->ang.y, 3.1415927f);
    w->Pl_rot = fabsf(w->Pl_dir);
    if (em->r_no_0 == 0) {
        w->Pl_dir = 0.0f;
        w->Pl_rot = 0.0f;
        em->l_pl = 100000000.0f;
    }
    EM10_ROUTE_SIGHT_CK();
    if (em->Character == 5 && (w->Be_flg & 1) && w->L_pl_route < 5000.0f) {
        em->Character = 0;
    }
    if (pSUB) {
        RouteCkToEm(em, pSUB, &w->Sub_pos, 0);
        w->L_sub_route = RouteCkPosToPosDis(&em->pos, &pSUB->pos);
        w->L_sub = (em->pos.x - pSUB->pos.x) * (em->pos.x - pSUB->pos.x) + (em->pos.z - pSUB->pos.z) * (em->pos.z - pSUB->pos.z);
        w->Sub_dir = Muku(&em->pos, &w->Sub_pos, em->ang.y, 3.1415927f);
        w->Sub_rot = fabsf(w->Sub_dir);
        if (em->r_no_0 == 0) {
            w->Sub_dir = 0.0f;
            w->Sub_rot = 0.0f;
            w->L_sub = 100000000.0f;
        }
        b.x = em->pos.x;
        b.y = em->pos.y + 1500.0f;
        b.z = em->pos.z;
        c.x = pSUB->pos.x;
        c.y = pSUB->pos.y + 1500.0f;
        c.z = pSUB->pos.z;
        if (!EatMgr.hitCheck(&b, &c, 0, 0, 0, 0)) {
            w->Be_flg |= 2;
        }
    } else {
        w->Be_flg &= ~0x08000000;
        w->Sub_pos = w->Pl_pos;
        w->L_sub_route = 100000000.0f;
        w->L_sub = 10000000000000000.0f;
        w->Sub_dir = 0.0f;
        w->Sub_rot = 0.0f;
    }
    if (w->Be_flg & 0x20000000) {
        w->Route_target = w->Keep_pos;
        w->Be_flg |= 0x04000000;
    }
    if (w->Goto_mode != 0) {
        w->Route_target = w->Goto_pos;
        w->Be_flg |= 0x04000000;
    }
    if (!(w->Be_flg & 0x04000000)) {
        if (w->Be_flg & 0x00800000) {
            if (em10SetWanderRoute(em)) {
                em10RouteTargetSet(em);
                return;
            }
            w->Lose_timer = 0;
            w->Be_flg &= ~0x00800000;
        }
    }
    t = em10RouteTargetSet(em);
    switch (t) {
    case 0:
    default:
        w->Be_flg &= ~0x08000000;
        w->Go_pos = w->Pl_pos;
        w->Go_dir = w->Pl_dir;
        w->Go_rot = w->Pl_rot;
        w->L_go = em->l_pl;
        if (w->Esc_timer != 0) {
            if (em->l_pl > 36000000.0f) {
                w->Esc_timer = 0;
            }
            RouteCkEscEm(em, pPL, &w->Go_pos);
        }
        break;
    case 1:
        w->Be_flg |= 0x08000000;
        w->Go_pos = w->Sub_pos;
        w->Go_dir = w->Sub_dir;
        w->Go_rot = w->Sub_rot;
        w->L_go = w->L_sub;
        break;
    }
    if (w->R11c_in_ck) {
        w->Be_flg |= 0x04000000;
        w->Route_target.x = 109672.0f;
        w->Route_target.y = 500.0f;
        w->Route_target.z = -48196.0f;
        d = (em->pos.x - w->Route_target.x) * (em->pos.x - w->Route_target.x) + (em->pos.z - w->Route_target.z) * (em->pos.z - w->Route_target.z);
        if (d < 4000000.0f) {
            w->R11c_in_ck = 0;
        }
        p = em->pos;
        p.y += 500.0f;
        if (SatMgr.hitCheck(&p, &w->Route_target, 0, 0, 0, 0) == 0) {
            w->R11c_in_ck = 0;
        }
    }
    if (w->R11c_in_ck2) {
        w->Be_flg |= 0x04000000;
        w->Route_target.x = 113323.0f;
        w->Route_target.y = 500.0f;
        w->Route_target.z = -51441.0f;
        d = (em->pos.x - w->Route_target.x) * (em->pos.x - w->Route_target.x) + (em->pos.z - w->Route_target.z) * (em->pos.z - w->Route_target.z);
        if (d < 4000000.0f) {
            w->R11c_in_ck2 = 0;
        }
        p = em->pos;
        p.y += 500.0f;
        if (SatMgr.hitCheck(&p, &w->Route_target, 0, 0, 0, 0) == 0) {
            w->R11c_in_ck2 = 0;
        }
    }
    if (w->Be_flg & 0x04000000) {
        RouteCkToPos(em, &w->Route_target, &w->Go_pos, 1, &w->Route_h);
        w->Go_dir = Muku(&em->pos, &w->Go_pos, em->ang.y, 3.1415927f);
        w->Go_rot = fabsf(w->Go_dir);
        w->L_go = (em->pos.x - w->Route_target.x) * (em->pos.x - w->Route_target.x) + (em->pos.z - w->Route_target.z) * (em->pos.z - w->Route_target.z);
        w->Go_target = 1;
        if (DbgFlagChk(pG, DBG_RTP_DISP)) {
            dbg = em->pos;
            dbg.y += 250.0f;
            Draw_line3d(&dbg, &w->Route_target, 0xFFFF0000, 0);
        }
    }
    EM10_ROUTE_LOCKON();
    if (DbgFlagChk(pG, DBG_RTP_DISP)) {
        dbg = em->pos;
        dbg.y += 250.0f;
        Draw_line3d(&dbg, &w->Go_pos, 0xFFFFFF40, 0);
        Draw_line3d(&dbg, &r, 0xFF0000FF, 0);
    }
}
#endif // PARKED

// Wander route step for a Ganado that lost the player: updates Wander_route and sets Go_pos / Go_dir /
// L_go towards it through the route check; 0 when there is no wander point.
// FUNCTION: BIO4 0x0042B4A0
// STATUS: stub
#if 0 // PARKED: DBG_RTP_DISP, DbgFlagChk, Draw_line3d, Muku, RouteCkToPos
extern "C" int em10SetWanderRoute(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec p2;
    int n;

    n = em10WanderRouteUpdate(em, w->Wander_route);
    w->Wander_route = n;
    if (n < 0) {
        return 0;
    }
    em10GetWanderRoutePos(em, &pos);
    RouteCkToPos(em, &pos, &w->Go_pos, 0, 0);
    w->Go_dir = Muku(&em->pos, &w->Go_pos, em->ang.y, 3.1415927f);
    w->Go_rot = fabsf(w->Go_dir);
    w->L_go = (em->pos.x - w->Go_pos.x) * (em->pos.x - w->Go_pos.x) + (em->pos.z - w->Go_pos.z) * (em->pos.z - w->Go_pos.z);
    if (DbgFlagChk(pG, DBG_RTP_DISP)) {
        p2 = em->pos;
        p2.y += 250.0f;
        Draw_line3d(&p2, &pos, 0xFFFF0000, 0);
        Draw_line3d(&p2, &w->Go_pos, 0xFFFFFF40, 0);
    }
    return 1;
}
#endif // PARKED

// Attack decision from the walk / dash / stay / turn routines (a = 1 from Back: allows the back-step
// check first). A dead Ganado goes to Dm_Small; otherwise, unless the ctrl12 EM10_ATK lock or the
// player's action scene forbids it, tries in order: the parasite, shield, axe, hoe, scythe, claw,
// claw critical, bowgun, rocket, gatling, throw axe, throw dynamite, chainsaw, and the catch of the
// player / partner. Returns 1 when a routine was set.
// FUNCTION: BIO4 0x00460AF0
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_EM10_ATK, Ctrl12Ck, DBG_EM_NO_ATK, DbgFlagChk, STA_NO_FENCE, STA_PL_BOAT, StaFlagChk, room_id
int em10AtkRtnCk(cEm10* em, int a)
{
    Em10Work* w = EM10_WK(em);

    if (DbgFlagChk(pG, DBG_EM_NO_ATK)) {
        em->r_no_1 = 0;
        em->r_no_2 = 0;
        em->r_no_3 = 0;
        em->r_no_0 = 1;
        return 1;
    }
    if (a == 0) {
        if (em10BackCk(em)) {
            return 1;
        }
    }
    if (w->Esc_timer != 0) {
        return 0;
    }
    if (w->Atk_no_wait == 0) {
        if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_ATK)) {
            return 0;
        }
    }
    if (StaFlagChk(pG, STA_PL_BOAT)) {
        return 0;
    }
    if (em10ParasiteAtkCk(em)) {
        return 1;
    }
    if (w->pParasite) {
        return 0;
    }
    if (em->type != 10 && em->type != 13) {
        if (w->pCore) {
            return 0;
        }
    }
    if (em->type == 0x16) {
        return 0;
    }
    if (em10ShieldAtkCk(em)) {
        return 1;
    }
    if (em10AxeAtkCk(em)) {
        return 1;
    }
    if (em10SukiAtkCk(em)) {
        return 1;
    }
    if (em10ScytheAtkCk(em)) {
        return 1;
    }
    if (em10ClawAtkCk(em)) {
        return 1;
    }
    if (em10ClawCriAtkCk(em)) {
        return 1;
    }
    if (em10ShotBowgunCk(em)) {
        return 1;
    }
    if (em10ShotRocketCk(em)) {
        return 1;
    }
    if (em10ShotGatlingCk(em)) {
        return 1;
    }
    if (em10ThrowAxeCk(em)) {
        return 1;
    }
    if (em10ThrowBombCk(em)) {
        return 1;
    }
    if (pG->room_id == 0x21B) {
        if (StaFlagChk(pG, STA_NO_FENCE)) {
            return 0;
        }
    }
    if (em10CsawAtkCk(em)) {
        return 1;
    }
    if (em10CatchPLRtnCk(em)) {
        return 1;
    }
    if (em10CatchSubRtnCk(em)) {
        return 1;
    }
    return 0;
}
#endif // PARKED

// Catch-frame test of Catch / DashCatch (br_Catch): the player must be alive, not held, within the
// grab box in front of the Ganado and reachable (three scenario line probes); on success locks the
// other Ganados' attacks for 30 / 120 frames (ctrl12 EM10_ATK / EM10_THROW). 1 = caught.
// FUNCTION: BIO4 0x0042B7F0
// STATUS: stub
#if 0 // PARKED: C2059, C2228, CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_THROW, Ctrl12Set, EatMgr, EmDeadCk, Mtx, PSMTXInverse, PSMTXMultVec, PSMTXRotRad, STA_PL_CATCHED, StaFlagChk, TransMatrix, VibDataTbl, VibSetData, inv, m, mat, pl_life, set
int em10CatchCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx inv;
    Vec v;
    Vec a;
    Vec b;
    Mtx m;

    if (EmDeadCk(pPL)) {
        return 0;
    }
    if ((s16) pG->pl_life <= 0) {
        return 0;
    }
    if (em->hp <= 0) {
        return 0;
    }
    if (!(em->Motion.Seq_old.Free & 2)) {
        return 0;
    }
    if (!(w->Be_flg & 1)) {
        return 0;
    }
    if (w->Die_wait > 0x2D) {
        return 0;
    }
    if (StaFlagChk(pG, STA_PL_CATCHED)) {
        return 0;
    }
    if ((w->Be_flg & 0x80) && (em->flag & 0x100000)) {
        return 0;
    }
    PSMTXInverse(em->mat, inv);
    PSMTXMultVec(inv, &pPL->pos, &v);
    if (v.y < -500.0f || v.y > 500.0f) {
        return 0;
    }
    if (v.z < 0.0f || v.z > 900.0f) {
        return 0;
    }
    if (!(v.x > -400.0f)) {
        return 0;
    }
    if (!(v.x < 400.0f)) {
        return 0;
    }
    a = em->pos;
    b = pPL->pos;
    a.y += 500.0f;
    b.y += 500.0f;
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    PSMTXRotRad(m, 'y', GetXZAngle(&em->pos, &pPL->pos));
    TransMatrix(m, &em->pos);
    a.x = 300.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    b.x = 300.0f;
    b.y = 500.0f;
    b.z = 500.0f;
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(m, &b, &b);
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    a.x = 300.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    b.x = 300.0f;
    b.y = 500.0f;
    b.z = 500.0f;
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(m, &b, &b);
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    pPL->dmg.set(0, 2);
    em->dmg.set(0, 2);
    VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 7, 1);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 30);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 120);
    return 1;
}
#endif // PARKED

// Catch-frame test on the partner: like em10CatchCk for Ashley (or Luis); on success sets the
// "partner held" bits (Status_flg[1] bit16, Status_flg[2] bit29) and goes to NeckHang_Ashley /
// NeckHang_Luis (or TakeAway). 1 = caught.
// FUNCTION: BIO4 0x0042BC20
// STATUS: stub
#if 0 // PARKED: C1003, C2228, EatMgr, EmDeadCk, Mtx, PSMTXInverse, PSMTXMultVec, PSMTXRotRad, STA_ASHLEY_HIDE, STA_SUB_BULLDOZER, STA_SUB_LADDER, STA_TAKEAWAY, SatMgr, StaFlagChk, StaFlagOn, TransMatrix, inv, m, mat, set
int em10CatchSubCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx inv;
    Vec v;
    Vec a;
    Vec b;
    Mtx m;

    if (pSUB == 0) {
        return 0;
    }
    if (EmDeadCk(pSUB)) {
        return 0;
    }
    if (pSUB->hp <= 0) {
        return 0;
    }
    if (em->hp <= 0) {
        return 0;
    }
    if (!(em->Motion.Seq_old.Free & 2)) {
        return 0;
    }
    if (!(w->Be_flg & 2)) {
        return 0;
    }
    if (StaFlagChk(pG, STA_TAKEAWAY)) {
        return 0;
    }
    if (StaFlagChk(pG, STA_SUB_BULLDOZER)) {
        return 0;
    }
    if (StaFlagChk(pG, STA_ASHLEY_HIDE)) {
        return 0;
    }
    if (StaFlagChk(pG, STA_SUB_LADDER)) {
        return 0;
    }
    if (w->Be_flg & 0x80) {
        return 0;
    }
    PSMTXInverse(em->mat, inv);
    PSMTXMultVec(inv, &pSUB->pos, &v);
    if (v.y < -500.0f || v.y > 500.0f) {
        return 0;
    }
    if (v.z < 0.0f || v.z > 900.0f) {
        return 0;
    }
    if (!(v.x > -400.0f && v.x < 400.0f)) {
        return 0;
    }
    pSUB->dmg.set(0, 2);
    em->dmg.set(0, 2);
    a = em->pos;
    b = pSUB->pos;
    a.y += 500.0f;
    b.y += 500.0f;
    if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    PSMTXRotRad(m, 'y', GetXZAngle(&em->pos, &pPL->pos));
    TransMatrix(m, &em->pos);
    a.x = 300.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    b.x = 300.0f;
    b.y = 500.0f;
    b.z = 500.0f;
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(m, &b, &b);
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    a.x = 300.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    b.x = 300.0f;
    b.y = 500.0f;
    b.z = 500.0f;
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(m, &b, &b);
    if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    if (pSUB->id == 4) {
        StaFlagOn(pG, STA_TAKEAWAY);
        StaFlagOn(pG, STA_SUB_CATCHED);
        EmRoutineSetW(em, 1, 0x35, 0, 0);
    } else {
        StaFlagOn(pG, STA_TAKEAWAY);
        StaFlagOn(pG, STA_SUB_CATCHED);
        EmRoutineSetW(em, 1, 0x3A, 0, 0);
    }
    return 1;
}
#endif // PARKED

// Chainsaw hit test on the saw's hit frames (Motion.Seq_old.Free bit0): a capsule along the saw blade
// (Em10AtkTbl[12]) against the player and the partner (EmAtkHitCk). A player hit leaves him at 1 hp
// with the damage hold set and returns 1 (the C_SawHit routine finishes him); a partner hit kills
// her outright.
// FUNCTION: BIO4 0x0042C0D0
// STATUS: stub
#if 0 // PARKED: C2059, Em10AtkTbl, EmAtkHitCk, EmAtkInfo, LifeDownSet, PSMTXMultVec, VibDataTbl, VibSetData, info, mat, pl_life, set
int em10CsawHitCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    EmAtkInfo info;
    Vec a;
    Vec b;
    int hit;

    if (!(em->Motion.Seq_old.Free & 1)) {
        return 0;
    }
    if (w->pWeapon == 0) {
        return 0;
    }
    info = Em10AtkTbl[12];
    a.x = 0.0f;
    a.y = 0.0f;
    a.z = 500.0f;
    b.x = 0.0f;
    b.y = 0.0f;
    b.z = -1000.0f;
    PSMTXMultVec(w->pWeapon->mat, &a, &a);
    PSMTXMultVec(w->pWeapon->mat, &b, &b);
    hit = EmAtkHitCk(&info, &a, &b, 0);
    if (hit & 1) {
        pG->pl_life = 1;
        pPL->dmg.set(0, 0x80);
        em->dmg.set(0, 0x80);
        VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 7, 1);
        return 1;
    } else if (hit & 2) {
        LifeDownSet(pSUB, 9999, 0);
        VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 7, 1);
        return 0;
    } else {
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 0.0f;
        b.x = 0.0f;
        b.y = 0.0f;
        b.z = -1000.0f;
        PSMTXMultVec(w->pWeapon->mat, &a, &a);
        PSMTXMultVec(w->pWeapon->mat, &b, &b);
        hit = EmAtkHitCk(&info, &a, &b, 0);
        if (hit & 1) {
            pG->pl_life = 1;
            pPL->dmg.set(0, 0x80);
            em->dmg.set(0, 0x80);
            VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 7, 1);
            return 1;
        } else if (hit & 2) {
            LifeDownSet(pSUB, 9999, 0);
            VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 7, 1);
            return 0;
        } else {
            a.x = 0.0f;
            a.y = 0.0f;
            a.z = -200.0f;
            b.x = 0.0f;
            b.y = 0.0f;
            b.z = -1000.0f;
            PSMTXMultVec(w->pWeapon->mat, &a, &a);
            PSMTXMultVec(w->pWeapon->mat, &b, &b);
            a.y = em->pos.y + 500.0f;
            hit = EmAtkHitCk(&info, &a, &b, 0);
            if (hit & 1) {
                pG->pl_life = 1;
                pPL->dmg.set(0, 0x80);
                em->dmg.set(0, 0x80);
                VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 0xD, 1);
                return 1;
            } else if (hit & 2) {
                LifeDownSet(pSUB, 9999, 0);
                VibSetData((VibDataTbl*) (pG->pCore->ofs_1C + (u32) pG->pCore), 0xD, 1);
                return 0;
            }
        }
    }
    return 0;
}
#endif // PARKED

// Head loss: a = 0/1 shot off (checked), 2 parasite emerges, 3 kick; b = 1 suppresses the blood burst
// when the head cannot be lost. Returns 1 when the head is gone.
// FUNCTION: BIO4 0x00452BC0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, CTRL12_ID_BIGEFF, CTRL12_ID_CNT_PARASITE, Ctrl12Ck, Ctrl12CntAdd, Ctrl12CntCk, Ctrl12Set, EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, EmReserveDropItem, EmSetDie, Mtx, ObjMgr, PSMTXMultVecSR, PSMTXRotRad, SndCall, SndStop, cObj12, m, mat, room_no, stage_no
int em10LostHead(cEm10* em, int a, int b)
{
    Em10Work* w = EM10_WK(em);
    int hit;
    int paras;
    int no;
    Mtx m;
    Vec spd;

    if ((a == 0 || a == 1) && !(hit = em10LostHeadCk(em))) {
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        SndCall(8, 7, &em->pos, em->id, 0, em);
        if (a == 3) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x78, 0, ESP_CORE_KIND_NONE, em, (void*) hit);
        } else {
            EstSet(em, -1, 0, 0, EFF_EM10, 0xA0, 0, ESP_CORE_KIND_NONE, em, (void*) hit);
        }
        return 0;
    }
    no = em->type == 6;
    if (em->type == 0xA) {
        no = 1;
    }
    if (em->type == 0xD) {
        no = 1;
    }
    if (em->type == 2) {
        no = 1;
    }
    if (pG->stage_no == 1 && pG->room_no == 0) {
        no = 1;
    }
    if (w->Wep_type == 4) {
        no = 1;
    }
    if (em->flag & 0x200) {
        if (w->Ganado == 1) {
            no = 1;
        }
        if (w->Ganado == 2) {
            no = 1;
        }
    }
    if ((w->Be_flg & 0x80) && !w->pCore) {
        no = 1;
    }
    if (no) {
        if (b == 0) {
            em10BloodSet(em, 0);
        }
        return 0;
    }
    paras = 0;
    if (w->pCore) {
        paras = 1;
    }
    switch ((u32) a) {
    case 0:
    default:
        // Dead test falling through into case 1 (the store is deleted by flow, the compare stays
        // live because gcse PRE reuses it for the `a == 3` after em10HeadSet): its block gives the
        // case-1 label a second predecessor where the compare is already computed, so PRE inserts
        // `cmpwi cr4, a, 3` at the end of the dispatch block and of the case-2 else arm and deletes
        // the join's compare. Any dead local store works here; the tree still needs case 0 and
        // case 1 as separate nodes (`cmpwi 1; beq; cmplwi 1; blt`).
        if (a == 3) {
            paras = 0;
        }
    case 1:
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        SndCall(8, 7, &em->pos, em->id, 0, em);
        w->Be_flg |= 0x80;
        EmSetDie(em);
        EmReserveDropItem(em);
        em10SetPoint(em);
        break;
    case 2:
        SndStop(w->Seid_voice, 0);
        SndStop(w->Seid_breath, 0);
        SndCall(8, 7, &em->pos, em->id, 0, em);
        w->Be_flg |= 0x80;
        if ((em->flag & 0x00100000) && !paras && !Ctrl12CntCk(w->pCtrlGroup, CTRL12_ID_CNT_PARASITE, 2)) {
            u8 r;
            w->Parasite_on = 1;
            Ctrl12CntAdd(w->pCtrlGroup, CTRL12_ID_CNT_PARASITE, 1);
            r = Rnd() % 3;
            w->Parasite_wait = r * 30 + 1;
            em->hp = 3000;
        } else {
            EmSetDie(em);
            EmReserveDropItem(em);
            em10SetPoint(em);
        }
        break;
    }
    em10HeadSet(em, 1);
    if (a == 3) {
        EstSet(em, -1, 0, 0, EFF_EM10, 0x78, 0, ESP_CORE_KIND_NONE, em, 0);
    } else {
        hit = Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BIGEFF);
        if (hit) {
            EstSet(em, -1, 0, 0, EFF_EM10, 0x34, 0, ESP_CORE_KIND_NONE, em, 0);
        } else {
            Ctrl12Set(w->pCtrlGroup, CTRL12_ID_BIGEFF, 0x2D);
            EstSet(em, -1, 0, 0, EFF_EM10, 3, 0, ESP_CORE_KIND_NONE, em, (void*) hit);
        }
    }
    EstSet(em, -1, 0, 0, EFF_EM10, 6, 0, ESP_CORE_KIND_NONE, em, 0);
    EffectEspDelete(0, w->EffKindIdEye, em, 0);
    EffectEspgenDelete(0, w->EffKindIdEye, em);
    EffectEfmDelete(0, w->EffKindIdEye, em);
    if (w->pHood) {
        w->pHood->be_flag &= ~8;
    }
    if (w->pWhood) {
        w->pWhood->be_flag &= ~8;
    }
    if (w->pAccesory[0]) {
        w->pAccesory[0]->be_flag &= ~8;
    }
    if (w->pAccesory[1]) {
        w->pAccesory[1]->be_flag &= ~8;
    }
    if (w->pAccesory[3]) {
        w->pAccesory[3]->be_flag &= ~8;
    }
    if (w->pAccesory[4]) {
        w->pAccesory[4]->be_flag &= ~8;
    }
    if (w->pAccesory[5]) {
        w->pAccesory[5]->be_flag &= ~8;
    }
    if (w->pCap) {
        switch (w->Cap_type) {
        case 0:
            break;
        default:
            PSMTXRotRad(m, 'y', GetXZAngle(&pPL->pos, &em->pos));
            spd.x = 0.0f;
            spd.y = 40.0f;
            spd.z = -50.0f;
            PSMTXMultVecSR(em->mat, &spd, &spd);
            w->pCap->setFall(&spd, 2);
            w->pCap = 0;
            w->Cap_type = 0;
            break;
        case 3:
        case 4:
            ObjMgr.destroy(w->pCap);
            w->pCap = 0;
            w->Cap_type = 0;
            break;
        }
    }
    if (w->pGlasses) {
        PSMTXRotRad(m, 'y', GetXZAngle(&pPL->pos, &em->pos));
        spd.x = 0.0f;
        spd.y = 40.0f;
        spd.z = -50.0f;
        PSMTXMultVecSR(em->mat, &spd, &spd);
        w->pGlasses->setFall(&spd, 3);
        w->pGlasses = 0;
    }
    em->setWeaponFall();
    if (w->pShield) {
        w->pShield->setFall(0, 20.0f);
        w->pShield = 0;
    }
    return 1;
}
#endif // PARKED

// Plays voice `no` through the room's ctrl11 SE control with the voice index of the model type
// (each type has its own voice bank slot), stopping the current voice; resets Breath_se_wait.
// FUNCTION: BIO4 0x0042C4A0
// STATUS: stub
#if 0 // PARKED: Ctrl11StopAndSetSe, SndStop
extern "C" void em10CallVoiceSe(cEm10* em, u16 no)
{
    Em10Work* w = EM10_WK(em);
    int idx;

    SndStop(w->Seid_voice, 0);
    SndStop(w->Seid_breath, 0);
    if (w->Be_flg & 0x80) {
        return;
    }
    switch (em->type) {
    case 0:
        idx = 0;
        break;
    default:
        idx = 0;
        break;
    case 1:
        idx = 1;
        break;
    case 2:
        idx = 2;
        break;
    case 3:
        idx = 3;
        break;
    case 4:
        idx = 4;
        break;
    case 5:
        idx = 2;
        break;
    case 6:
        idx = 5;
        break;
    case 7:
        idx = 2;
        break;
    case 8:
        idx = 2;
        break;
    case 9:
        idx = 2;
        break;
    case 10:
        idx = 2;
        break;
    case 13:
        idx = 2;
        break;
    case 14:
        idx = 2;
        break;
    case 15:
        idx = 2;
        break;
    case 16:
        idx = 2;
        break;
    case 17:
        idx = 2;
        break;
    case 18:
        idx = 2;
        break;
    case 19:
        idx = 2;
        break;
    case 20:
        idx = 2;
        break;
    case 21:
        idx = 2;
        break;
    case 23:
        idx = 2;
        break;
    case 24:
        idx = 2;
        break;
    case 25:
        idx = 2;
        break;
    }
    w->Seid_voice = Ctrl11StopAndSetSe(w->pCtrlSe, em, Rnd() % 20 + 20, no, idx);
    w->Breath_se_wait = Rnd() % 120 + 120;
}
#endif // PARKED

// Voice `no` on ctrl11 bank `a` without stopping the current voice (only while none plays).
// FUNCTION: BIO4 0x0042C640
// STATUS: stub
#if 0 // PARKED: Ctrl11SetSe2, SndStop
void em10CallVoiceSe2(cEm10* em, u16 no, u16 a)
{
    Em10Work* w = EM10_WK(em);

    SndStop(w->Seid_voice, 0);
    SndStop(w->Seid_breath, 0);
    if (!(w->Be_flg & 0x80)) {
        w->Seid_voice = Ctrl11SetSe2(w->pCtrlSe, em, Rnd() % 20 + 20, no, 6, a);
        w->Breath_se_wait = Rnd() % 120 + 120;
    }
}
#endif // PARKED

// Breathing / grunt SE (Se_tbl[7]) every 120..240 frames while walking.
// FUNCTION: BIO4 0x0042C710
// STATUS: stub
#if 0 // PARKED: Ctrl11SetSe
void em10BreathSe(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Breath_se_wait) {
        w->Breath_se_wait--;
    } else {
        w->Breath_se_wait = Rnd() % 120 + 120;
        if (!(w->Be_flg & 0x80)) {
            w->Seid_breath = Ctrl11SetSe(w->pCtrlSe, em, Rnd() % 20 + 20, w->Se_tbl[7], 6);
        }
    }
}
#endif // PARKED

// Chainsaw Ganado tell: revs the saw (Csaw_sign_wait) or plays the far-away "fake" rev every
// Csaw_fake_timer frames so the player hears it coming; nothing when the player is dead.
// FUNCTION: BIO4 0x0042C7F0
// STATUS: stub
#if 0 // PARKED: FlagChkSignW, SndCall, SndStop, l_pl, pl_life
extern "C" void em10CsawSignSe(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (!FlagChkSignW(w->Be_flg, 0)) {
        return;
    }
    if (w->Wep_type != 4) {
        return;
    }
    if ((s16) pG->pl_life <= 0) {
        return;
    }
    if (em->l_pl > 25000000.0f) {
        if (w->Csaw_fake_timer) {
            w->Csaw_fake_timer--;
            return;
        }
        w->Csaw_fake_timer = (u8) (Rnd() % 150) + 150;
        w->Csaw_sign_wait = 150;
        w->Seid_csaw = SndCall(6, 0x4B, &em->pos, 0, 0, em);
    } else {
        if (w->Csaw_sign_wait != 0) {
            return;
        }
        SndStop(w->Seid_csaw, 0);
        w->Seid_csaw = SndCall(6, 0x3E, &em->pos, 0, 0, em);
        w->Csaw_sign_wait = 150;
        w->Csaw_fake_timer = (u8) (Rnd() % 150) + 150;
    }
}
#endif // PARKED

// Builds the Ganado model at creation: cModel::modelInit, the robe / cloth / goods parts of type 6,
// head, hands, accessories, weapon, shield and the chainsaw Ganado's sack. 0 when modelInit fails.
// FUNCTION: BIO4 0x00460920
// STATUS: stub
#if 0 // PARKED: C2227, C2228, Em10_fs_tbl, ModInfoMgr, addModel, modelInit, pFsdTbl, pLog
int em10ModelInit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (!em->modelInit(w->mot[1], w->mot[0])) {
        pLog->err(0, 0, "EM10 pEm->modelInit() failed.");
        return 0;
    }
    w->pRobe = 0;
    w->pCloth = 0;
    w->pGoods = 0;
    if (em->type == 6) {
        if (w->mot[16] && w->mot[17]) {
            w->pRobe = ModInfoMgr.create(w->mot[16], w->mot[17]);
            if (w->pRobe) {
                em->addModel(w->pRobe);
            }
        }
        em10ClothPartsSet(em, 0);
        em10GoodsPartsSet(em, 0);
    }
    w->pHead = 0;
    em10HeadSet(em, 0);
    w->pRHand = 0;
    w->pLHand = 0;
    em10HandSet(em, 0);
    em->pFsdTbl = &Em10_fs_tbl;
    w->pCart = 0;
    em10SetAccesory(em);
    em10WeaponInit(em);
    em10ShieldSet(em);
    em10SackSet(em);
    return 1;
}
#endif // PARKED

// Picks the weapon in hand from the cEm::flag bits and the module's motion table: 1 hoe (bit31),
// 2 hatchet / 0xB flail (bit29), 3 sickle (bit27), 0xA pitchfork (bit14), 4 chainsaw (bit28), 6 scythe /
// 0xF stun rod (bit26), 7 torch / 0x10 (bit11), 8 bowgun (bit15), 9 dynamite (bit17), 5 bucket / 0xC
// rocket launcher (bit30); bit13 means "carried as the spare" (em10WeaponSet2). Creates it with
// em10MakeWeapon and attaches it. Claw / gatling types (0xA, 0xD, 2) carry nothing.
// FUNCTION: BIO4 0x00453280
// STATUS: stub
extern "C" void em10WeaponInit(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    w->Wep_type = 0;
    w->Wep_type2 = 0;
    w->pWeapon = 0;
    w->pWeapon2 = 0;
    if (em->type == 0xA || em->type == 0xD || em->type == 2) {
        return;
    }
    if (em->flag & 0x40000000) {
        if (w->Ganado == 0) {
            if (w->mot[43] && w->mot[44]) {
                w->Wep_type = 5;
            }
        } else {
            w->Wep_type = 0xC;
        }
    }
    if (w->mot[69] && w->mot[70] && (em->flag & 0x04000000) && w->pWeapon == 0) {
        if (w->Ganado == 2) {
            w->Wep_type = 0xF;
        } else {
            w->Wep_type = 6;
        }
    }
    if (w->mot[41] && w->mot[42] && w->Ganado == 0 && (em->flag & 0x80000000) && w->pWeapon == 0) {
        w->Wep_type = 1;
    }
    if (w->mot[65] && w->mot[66] && (em->flag & 0x20000000) && w->pWeapon == 0) {
        if (w->Ganado == 0) {
            if (!(em->flag & 0x2000)) {
                w->Wep_type = 2;
            }
        } else {
            em->flag &= ~0x2000;
            w->Wep_type = 0xB;
        }
    }
    if (w->mot[63] && w->mot[64] && (em->flag & 0x08000000) && w->pWeapon == 0 && !(em->flag & 0x2000)) {
        if (w->Ganado == 2) {
            w->Wep_type = 2;
        } else {
            w->Wep_type = 3;
        }
    }
    if (w->mot[77] && w->mot[78] && (em->flag & 0x4000) && w->pWeapon == 0 && !(em->flag & 0x2000)) {
        w->Wep_type = 0xA;
    }
    if (w->mot[67] && w->mot[68] && (em->flag & 0x10000000) && w->pWeapon == 0) {
        w->Wep_type = 4;
    }
    if (w->mot[71] && w->mot[72] && (em->flag & 0x800) && w->pWeapon == 0) {
        if (w->Ganado == 2) {
            w->Wep_type = 0x10;
        } else {
            w->Wep_type = 7;
        }
    }
    if (w->mot[73] && w->mot[74] && em->type == 6) {
        em->flag |= 0x0001A000;
    }
    if ((em->flag & 0x8000) && w->pWeapon == 0) {
        w->Arrow_num = 2;
        if (!(em->flag & 0x2000)) {
            w->Wep_type = 8;
        }
    }
    if ((em->flag & 0x00020000) && w->pWeapon == 0) {
        if (!(em->flag & 0x2000)) {
            w->Wep_type = 9;
        }
        em->flag &= ~0x00100000;
    }
    w->pWeapon = em10MakeWeapon(em, w->Wep_type);
    if (w->pWeapon == 0) {
        w->Wep_type = 0;
    }
    em10WeaponSet(em);
    em10WeaponSet2(em);
}

// Creates the accessories the cEm::flag bits ask for per model type: hanging cObj12 objects (sack,
// lantern, bucket ... as pCap, glasses as pGlasses) and extra parts (hood, hats, belts as pHood /
// pWhood / pAccesory[]); the head part is hidden under a hood.
// Sets up the accessory objects / parts selected by the flags_3C8 bits (per enemy type).
// FUNCTION: BIO4 0x0042CAF0
// STATUS: stub
#if 0 // PARKED: C2227, EM10_ACC_OBJ12, EM10_ACC_PARTS, cObj12
extern "C" void em10SetAccesory(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;
    Vec rot2;
    cModelInfo* info;

    w->pCap = 0;
    w->Cap_type = 0;
    w->pGlasses = 0;
    w->pHood = 0;
    w->pWhood = 0;
    w->pAccesory[0] = 0;
    w->pAccesory[1] = 0;
    w->pAccesory[2] = 0;
    w->pAccesory[3] = 0;
    w->pAccesory[4] = 0;
    w->pAccesory[5] = 0;
    w->pAccesory[6] = 0;
    if (em->type == 0xA || em->type == 0xD) {
        return;
    }
    switch (em->type) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 6:
    default:
        if (w->mot[52] && w->mot[53] && (em->flag & 0x00800000) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[52], w->mot[53], rot, 125.7f, 20.0f, 1);
        }
        if (w->mot[52] && w->mot[54] && (em->flag & 0x00080000) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[52], w->mot[54], rot2, 125.7f, 20.0f, 2);
        }
        if (w->mot[55] && w->mot[56] && (em->flag & 0x00400000) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[55], w->mot[56], rot, 5.0f, 0.0f, 3);
        }
        if (w->mot[55] && w->mot[57] && (em->flag & 0x00040000) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[55], w->mot[57], rot, 5.0f, 0.0f, 4);
        }
        if (w->mot[58] && w->mot[59] && (em->flag & 0x200) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[58], w->mot[59], rot, 87.0f, 11.0f, 5);
        }
        if (w->mot[58] && w->mot[60] && (em->flag & 0x100) && !w->pCap) {
            EM10_ACC_OBJ12(w->mot[58], w->mot[60], rot, 87.0f, 11.0f, 6);
        }
        if (w->mot[61] && w->mot[62] && (em->flag & 0x00200000)) {
            pos.x = 0.0f;
            pos.y = 54.71f;
            pos.z = 72.73f;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            w->pGlasses = SetObj12(w->mot[61], w->mot[62], &pos, &rot);
            if (w->pGlasses) {
                w->pGlasses->setParent(em, 4, 1);
            }
        }
        if (w->mot[23] && w->mot[24] && (em->flag & 0x00400000)) {
            EM10_ACC_PARTS(w->mot[23], w->mot[24], w->pWhood);
        }
        if (w->mot[23] && w->mot[25] && (em->flag & 0x00040000)) {
            EM10_ACC_PARTS(w->mot[23], w->mot[25], w->pWhood);
        }
        break;
    case 5:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xD:
        if ((em->flag & 0x00040002) && !(em->flag & 0x00081000)) {
            em->flag |= 0x00080000;
        }
        if (em->flag & 0x300) {
            em->flag &= ~0x00080000;
            em->flag |= 0x1000;
        }
        if ((em->flag & 0x00080000) && w->mot[26] && w->mot[0]) {
            EM10_ACC_PARTS(w->mot[26], w->mot[0], w->pHood);
        }
        if ((em->flag & 0x1000) && w->mot[27] && w->mot[0]) {
            EM10_ACC_PARTS(w->mot[27], w->mot[0], w->pHood);
            if (w->pHead) {
                w->pHead->be_flag &= ~8;
            }
            em->flag &= ~0x00204000;
        }
        if ((em->flag & 0x00800000) && w->mot[28] && w->mot[29]) {
            EM10_ACC_PARTS(w->mot[28], w->mot[29], w->pAccesory[0]);
        }
        if ((em->flag & 0x00400000) && w->mot[30] && w->mot[31]) {
            EM10_ACC_PARTS(w->mot[30], w->mot[31], w->pAccesory[1]);
        }
        if ((em->flag & 0x00040000) && w->mot[32] && w->mot[33]) {
            EM10_ACC_PARTS(w->mot[32], w->mot[33], w->pAccesory[2]);
        }
        if ((em->flag & 2) && !w->pAccesory[2] && w->mot[32]) {
            EM10_ACC_PARTS(w->mot[32], PL_ARC_PTR(em->subArc, 0x220), w->pAccesory[2]);
        }
        if ((em->flag & 0x200) && w->mot[34] && w->mot[35]) {
            EM10_ACC_PARTS(w->mot[34], w->mot[35], w->pAccesory[3]);
            if (w->pHead) {
                w->pHead->be_flag &= ~8;
            }
        }
        if ((em->flag & 0x100) && w->mot[36] && w->mot[37]) {
            EM10_ACC_PARTS(w->mot[36], w->mot[37], w->pAccesory[4]);
        }
        if ((em->flag & 0x00200000) && w->mot[38] && w->mot[39]) {
            EM10_ACC_PARTS(w->mot[38], w->mot[39], w->pAccesory[5]);
        }
        if ((em->flag & 0x4000) && !w->pAccesory[5] && w->mot[38] && w->mot[40]) {
            EM10_ACC_PARTS(w->mot[38], w->mot[40], w->pAccesory[5]);
        }
        break;
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x17:
    case 0x18:
        if ((em->flag & 0x00800000) && !w->pCap) {
            EM10_ACC_OBJ12(PL_ARC_PTR(em->subArc, 0x265), PL_ARC_PTR(em->subArc, 0x266), rot, 80.0f, 0.0f, 9);
        }
        if ((em->flag & 0x00200000) && !w->pCap) {
            EM10_ACC_OBJ12(PL_ARC_PTR(em->subArc, 0x262), PL_ARC_PTR(em->subArc, 0x263), rot, 80.0f, 0.0f, 9);
        }
        if ((em->flag & 0x00080000) && !w->pCap) {
            EM10_ACC_OBJ12(PL_ARC_PTR(em->subArc, 0x262), PL_ARC_PTR(em->subArc, 0x263), rot, 80.0f, 0.0f, 9);
        }
        if (em->flag & 0x200) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x24F), PL_ARC_PTR(em->subArc, 0x250), w->pAccesory[3]);
            if (w->pHead) {
                w->pHead->be_flag &= ~8;
            }
        }
        if (em->flag & 0x100) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x243), PL_ARC_PTR(em->subArc, 0x244), w->pAccesory[2]);
        }
        if ((em->flag & 2) && !w->pAccesory[2]) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x245), w->mot[0], w->pAccesory[2]);
        }
        if ((em->flag & 0x00040000) && !w->pHood) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x23D), PL_ARC_PTR(em->subArc, 0x23E), w->pHood);
        }
        if ((em->flag & 0x4000) && !w->pHood) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x23F), PL_ARC_PTR(em->subArc, 0x240), w->pHood);
        }
        if ((em->flag & 0x1000) && !w->pHood) {
            EM10_ACC_PARTS(PL_ARC_PTR(em->subArc, 0x241), PL_ARC_PTR(em->subArc, 0x242), w->pHood);
        }
        break;
    case 2:
        EM10_ACC_OBJ12(PL_ARC_PTR(em->subArc, 0x25F), PL_ARC_PTR(em->subArc, 0x260), rot, 155.38f, 23.4f, 0xA);
        break;
    }
}
#endif // PARKED

// Creates the cEmWep of weapon `type` from the motion table's model pair (mot[41..] by type; the
// rocket launcher and the dynamite come from the archive), with its SE / effect set
// (em10WepSeEffSet); heavy weapons get be_flag 0x4000. NULL for type 0.
// FUNCTION: BIO4 0x00445C90
// STATUS: stub
#if 0 // PARKED: SetWeapon, be_flag, setCloth, setSeAlways
cEmWep* em10MakeWeapon(cEm10* em, int type)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;
    cEmWep* wep = 0;

    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    switch (type) {
    case 0:
    default:
        break;
    case 1:
        if (w->mot[41] && w->mot[42]) {
            wep = SetWeapon(w->mot[41], w->mot[42], &pos, &rot, 0);
        }
        break;
    case 2:
        if (w->Ganado == 2) {
            if (w->mot[63] && w->mot[64]) {
                wep = SetWeapon(w->mot[63], w->mot[64], &pos, &rot, 0);
            }
        } else {
            if (w->mot[65] && w->mot[66]) {
                wep = SetWeapon(w->mot[65], w->mot[66], &pos, &rot, 0);
            }
        }
        break;
    case 0xB:
        if (w->mot[65] && w->mot[66]) {
            wep = SetWeapon(w->mot[65], w->mot[66], &pos, &rot, 0);
            if (wep) {
                wep->setCloth(em);
            }
        }
        break;
    case 3:
        if (w->mot[63] && w->mot[64]) {
            wep = SetWeapon(w->mot[63], w->mot[64], &pos, &rot, 0);
        }
        break;
    case 0xA:
        if (w->mot[77] && w->mot[78]) {
            wep = SetWeapon(w->mot[77], w->mot[78], &pos, &rot, 0);
        }
        break;
    case 4:
        if (w->mot[67] && w->mot[68]) {
            wep = SetWeapon(w->mot[67], w->mot[68], &pos, &rot, 0);
        }
        break;
    case 5:
        if (w->mot[43] && w->mot[44]) {
            wep = SetWeapon(w->mot[43], w->mot[44], &pos, &rot, 0);
        }
        break;
    case 6:
        if (w->mot[69] && w->mot[70]) {
            wep = SetWeapon(w->mot[69], w->mot[70], &pos, &rot, 0);
        }
        break;
    case 0xF:
        if (w->mot[69] && w->mot[70]) {
            wep = SetWeapon(w->mot[69], w->mot[70], &pos, &rot, 0);
            if (wep) {
                wep->setEffAlways(0xCD, 0);
                wep->setSeAlways(8, 0x60, em->id, 0x13);
            }
        }
        break;
    case 7:
        if (w->mot[71] && w->mot[72]) {
            wep = SetWeapon(w->mot[71], w->mot[72], &pos, &rot, 0);
        }
        break;
    case 0x10:
        if (w->mot[71] && w->mot[72]) {
            wep = SetWeapon(w->mot[71], w->mot[72], &pos, &rot, 0);
        }
        break;
    case 8:
        if (w->mot[73] && w->mot[74]) {
            wep = SetWeapon(w->mot[73], w->mot[74], &pos, &rot, 0);
        }
        break;
    case 0xC:
        wep = SetWeapon(PL_ARC_PTR(em->subArc, 0x18C), PL_ARC_PTR(em->subArc, 0x18D), &pos, &rot, 0);
        break;
    case 9:
        wep = SetWeapon(PL_ARC_PTR(em->subArc, 0xA2), PL_ARC_PTR(em->subArc, 0xA3), &pos, &rot, 0);
        break;
    }
    if (wep) {
        em10WepSeEffSet(em, wep, type);
        switch (type) {
        case 4:
        case 5:
        case 8:
        case 0xB:
        case 0xC:
            break;
        default:
            wep->be_flag |= 0x4000;
            break;
        }
    }
    return wep;
}
#endif // PARKED

// Places the weapon in hand: position / rotation offsets per weapon type (mirrored for a left-handed
// Ganado, cEm::flag bit24) and the parent hand part (0x10 left / 0xA right).
// FUNCTION: BIO4 0x0042D880
// STATUS: stub
#if 0 // PARKED: ang, pos, setParent
void em10WeaponSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;
    u32 f;

    if (!w->pWeapon) {
        return;
    }
    switch (w->Wep_type) {
    case 0:
    default:
        return;
    case 1:
        pos.x = -843.58f;
        pos.y = 5.81f;
        pos.z = 866.87f;
        rot.x = 0.0f;
        rot.y = -0.7166758f;
        rot.z = 0.0f;
        break;
    case 6:
        pos.x = -403.0f;
        pos.y = -120.94f;
        pos.z = 296.24f;
        rot.x = 0.0f;
        rot.y = -0.73521996f;
        rot.z = 0.0f;
        w->pWeapon->setEffAlways(0x10, 0x49);
        break;
    case 5:
        pos.x = -80.0f;
        pos.y = -35.0f;
        pos.z = -10.0f;
        rot.x = 0.0f;
        rot.y = -2.0943952f;
        rot.z = 0.0f;
        break;
    case 2:
    case 3:
    case 7:
    case 9:
    case 0xA:
    case 0xF:
    case 0x10:
        pos.x = -313.85f;
        pos.y = -21.2f;
        pos.z = 102.21f;
        rot.x = 0.0f;
        rot.y = -1.0402162f;
        rot.z = 0.0f;
        break;
    case 0xB:
        pos.x = -86.0f;
        pos.y = -28.0f;
        pos.z = -6.0f;
        rot.x = 0.0f;
        rot.y = -0.7853982f;
        rot.z = 0.0f;
        break;
    case 4:
        pos.x = -304.24f;
        pos.y = -17.86f;
        pos.z = -57.43f;
        rot.x = 1.4835298f;
        rot.y = 0.0f;
        rot.z = -1.5707964f;
        break;
    case 8:
        pos.x = -384.25f;
        pos.y = -24.01f;
        pos.z = 17.94f;
        rot.x = 1.5707964f;
        rot.y = 0.0f;
        rot.z = -1.5707964f;
        w->Arrow_num = 2;
        break;
    case 0xC:
        pos.x = 220.0f;
        pos.y = 120.0f;
        pos.z = 25.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        w->Arrow_num = 2;
        break;
    case 0xD:
        pos.x = -540.0f;
        pos.y = -25.0f;
        pos.z = 5.0f;
        rot.x = 0.0f;
        rot.y = -1.5707964f;
        rot.z = 0.0f;
        break;
    case 0xE:
        pos.x = -199.0f;
        pos.y = -27.0f;
        pos.z = 459.0f;
        rot.x = 0.0f;
        rot.y = -0.2617994f;
        rot.z = 0.0f;
        break;
    }
    f = em->flag;
    if (f & 0x01000000) {
        switch (w->Wep_type) {
        case 4:
        case 6:
            em->flag = f & ~0x01000000;
            break;
        case 1:
        case 8:
        case 0xC:
            pos.x = -pos.x;
            rot.y = -rot.y;
            rot.z = -rot.z;
            break;
        default:
            pos.x = -pos.x;
            rot.z = -rot.z + 3.1415927f;
            break;
        }
    }
    w->pWeapon->pos = pos;
    w->pWeapon->ang = rot;
    if (em->flag & 0x01000000) {
        w->pWeapon->setParent(em, 0x10, 0);
    } else {
        w->pWeapon->setParent(em, 0xA, 0);
    }
}
#endif // PARKED

// Creates the spare weapon (cEm::flag bit13): the second hatchet / flail / sickle / pitchfork /
// bowgun / dynamite as pWeapon2 on the back part 0x11, hidden until Pickup takes it.
// FUNCTION: BIO4 0x004460D0
// STATUS: stub
#if 0 // PARKED: ang, pos, setParent, setTransMode
extern "C" void em10WeaponSet2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    Vec rot;

    if (w->pWeapon2) {
        return;
    }
    if (!(em->flag & 0x2000)) {
        return;
    }
    if (em->flag & 0x20000000) {
        if (w->Ganado == 0) {
            w->Wep_type2 = 2;
        } else {
            w->Wep_type2 = 0xB;
        }
    }
    if (em->flag & 0x08000000) {
        int t = w->Ganado;
        if (t != 2) {
            t = 3;
        }
        w->Wep_type2 = t;
    }
    if (em->flag & 0x4000) {
        w->Wep_type2 = 0xA;
    }
    if (em->flag & 0x8000) {
        w->Wep_type2 = 8;
    }
    if (em->flag & 0x20000) {
        w->Wep_type2 = 9;
    }
    w->pWeapon2 = em10MakeWeapon(em, w->Wep_type2);
    if (!w->pWeapon2) {
        w->Wep_type2 = 0;
        return;
    }
    if (em->type == 6) {
        w->pWeapon2->setTransMode(0);
    }
    if (em->flag & 0x01000000) {
        pos.x = -140.0f;
        pos.y = -30.0f;
        pos.z = -150.0f;
        rot.x = -1.5707964f;
        rot.y = 0.0f;
        rot.z = 1.2566371f;
    } else {
        pos.x = 140.0f;
        pos.y = -30.0f;
        pos.z = -150.0f;
        rot.x = 1.5707964f;
        rot.y = 0.0f;
        rot.z = 1.8849555f;
    }
    w->pWeapon2->pos = pos;
    w->pWeapon2->ang = rot;
    w->pWeapon2->setParent(em, 0x11, 0);
}
#endif // PARKED

// Zealot / soldier shield (Ganado 1 / 2 with cEm::flag bit31): creates the cEmShield from the archive
// (0x163 / 0x164) and attaches it to the off hand (0x10, or 0xA for a left-handed Ganado, flag bit24).
// FUNCTION: BIO4 0x0042DEE0
// STATUS: stub
#if 0 // PARKED: C2227, FlagChkSignW, SetShield, cEmShield, s
extern "C" void em10ShieldSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cEmShield* s;
    Vec pos;
    Vec rot;

    w->pShield = 0;
    if (w->Ganado == 0) {
        return;
    }
    if (!FlagChkSignW(em->flag, 0)) {
        return;
    }
    if (em->type == 10 || em->type == 13 || em->type == 2 || em->type == 22 || em->type == 24) {
        return;
    }
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    s = SetShield(PL_ARC_PTR(em->subArc, 0x163), PL_ARC_PTR(em->subArc, 0x164), &pos, &rot);
    if (!s) {
        return;
    }
    pos.x = -237.0f;
    pos.y = 0.02f;
    pos.z = 14.0f;
    rot.x = 0.0f;
    rot.y = -1.5707964f;
    rot.z = 0.0f;
    if (em->flag & 0x01000000) {
        pos.x = pos.x * -1.0f;
        rot.y = 1.5707964f;
    }
    s->pos = pos;
    s->ang = rot;
    if (em->flag & 0x01000000) {
        s->setParent(em, 0xA, 0);
    } else {
        s->setParent(em, 0x10, 0);
    }
    w->pShield = s;
}
#endif // PARKED

// Per frame from the routines: swaps both hand models (pRHand / pLHand from mot[6..15]) to the pose
// `type`: 0 relaxed, 1 open, 2 pointing, 3 = holding the weapon (forced while pWep is set; the hoe /
// chainsaw / scythe grips differ, left-handed Ganados mirror). Skipped for the claw / gatling types.
// FUNCTION: BIO4 0x0042E0B0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, ModInfoMgr, addModel, swapModelInfo
void em10HandSet(cEm10* em, int type)
{
    Em10Work* w = EM10_WK(em);
    cEmWep* wep;
    void* bin;
    void* tpl;
    cModelInfo* info;

    if (em->type == 10 || em->type == 13 || em->type == 2 || em->type == 22 || em->type == 24) {
        return;
    }
    if (w->pWeapon) {
        type = 3;
    }
    if (w->pRHand && w->pLHand && w->Hand_type == type) {
        if (type != 0) {
            return;
        }
        if (!w->pWeapon) {
            return;
        }
    }
    wep = w->pWeapon;
    switch ((u32) type) {
    case 0:
    default:
        bin = w->mot[6];
        tpl = w->mot[11];
        break;
    case 1:
        bin = w->mot[7];
        tpl = w->mot[12];
        break;
    case 2:
        if (em->flag & 0x01000000) {
            bin = w->mot[6];
            tpl = w->mot[13];
        } else {
            bin = w->mot[8];
            tpl = w->mot[11];
        }
        break;
    }
    if (wep) {
        if (em->flag & 0x01000000) {
            tpl = w->mot[14];
        } else {
            bin = w->mot[9];
        }
        if (w->Wep_type == 1 || w->Wep_type == 4) {
            bin = w->mot[9];
            tpl = w->mot[14];
        }
        if (w->Wep_type == 6) {
            tpl = w->mot[15];
        }
    }
    if (w->Wep_type == 12) {
        bin = PL_ARC_PTR(em->subArc, 0x18E);
    }
    info = ModInfoMgr.create(bin, w->mot[0]);
    if (info) {
        if (w->pRHand) {
            em->swapModelInfo(w->pRHand->model_addr, info);
        } else {
            em->addModel(info);
        }
        w->pRHand = info;
    }
    info = ModInfoMgr.create(tpl, w->mot[0]);
    if (info) {
        if (w->pLHand) {
            em->swapModelInfo(w->pLHand->model_addr, info);
        } else {
            em->addModel(info);
        }
        w->pLHand = info;
    }
    w->Hand_type = type;
}
#endif // PARKED

// Swaps one hand model (`no` 0 left, 1 right) to pose `type` (0 relaxed, 1 open, 2 pointing, 3 the
// weapon grip; mot[10] for the scythe); used by the event / cut-scene code outside em10.
// FUNCTION: BIO4 0x0042E2D0
// STATUS: stub
#if 0 // PARKED: C2059, C2227, C2228, ModInfoMgr, addModel, swapModelInfo
void cEm10::setHand(int no, int type)
{
    Em10Work* w = EM10_WK(this);
    void* tpl;
    void* bin;
    cModelInfo* info;

    if (type == 10 || type == 13 || type == 2 || type == 22 || type == 24) {
        return;
    }
    if (w->pRHand && w->pLHand && w->Hand_type == type) {
        return;
    }
    switch ((u32) type) {
    case 0:
    default:
        bin = w->mot[6];
        tpl = w->mot[11];
        break;
    case 1:
        bin = w->mot[7];
        tpl = w->mot[12];
        break;
    case 2:
        bin = w->mot[8];
        tpl = w->mot[13];
        break;
    case 3:
        // Target block: `lbz wepType; lwz tpl; lwz bin; cmpwi; bne`. tpl first = LUID order of the two
        // loads; the barrier keeps the byte compare after `lwz bin` (rank_for_schedule prefers the
        // compare: equal priority, weight 0 vs +1, then more dependents -- no plain form ranks the
        // load first). tpl loaded first would cost it r4 (global-alloc priority 2*5/16 vs bin's
        // 2*6/19), so the tpl create below is weighted with a do{}while(0). `no` is r10 because of
        // the dead x184 test below.
        {
            u8 wt = w->Wep_type;
            tpl = w->mot[14];
            bin = w->mot[9];
            asm volatile(""); // COMPILER-DIFF: candidate #14 (sched1 rank of `lwz bin` vs the byte compare)
            if (wt == 6) {
                bin = w->mot[10];
            }
        }
        break;
    }
    // Dead test (`type` is dead here; flow deletes the store, jump2 deletes the jump-to-next and,
    // through delete_computation, the compare and the load). Its load temp holds r11 through
    // global alloc, so `no` takes r10 (alloc order r0, r9, r11, r10). Only this load reproduces it.
    if (w->pRHand == 0) {
        type = 0;
    }
    if (no) {
        info = ModInfoMgr.create(bin, w->mot[0]);
        if (!info) {
            return;
        }
        if (w->pRHand) {
            swapModelInfo(w->pRHand->model_addr, info);
        } else {
            addModel(info);
        }
        w->pRHand = info;
    } else {
        do { info = ModInfoMgr.create(tpl, w->mot[0]); } while (0); // tpl's 6th weighted ref: r4
        if (!info) {
            return;
        }
        if (w->pLHand) {
            swapModelInfo(w->pLHand->model_addr, info);
        } else {
            addModel(info);
        }
        w->pLHand = info;
    }
}
#endif // PARKED

// Head model (pHead, hidden by be_flag bit3 until shown): `no` 0 the normal head (mot[2]), 1 the
// lost-head stump (mot[3]) or, for a living zealot without hood, the bent-neck parasite head
// (mot[4], part 0x24 tilted). Skipped for the claw / gatling types.
// FUNCTION: BIO4 0x0042E480
// STATUS: stub
#if 0 // PARKED: C2227, C2228, ModInfoMgr, addModel, swapModelInfo
void em10HeadSet(cEm10* em, int no)
{
    Em10Work* w = EM10_WK(em);
    cModelInfo* info;
    void* bin;

    if (em->type == 10 || em->type == 13 || em->type == 2 || em->type == 22) {
        return;
    }
    // Separate case 0 / default arms (cross-jumped after allocation): the extra w reference decides r30 for w.
    switch (no) {
    case 0:
        bin = w->mot[2];
        break;
    default:
        bin = w->mot[2];
        break;
    case 1:
        if (w->Ganado == 1 && em->hp > 0 && (Rnd() & 3) && !(em->flag & 0x204000)) {
            em->getPartsPtr(0x24)->ang.x = -0.6981317f;
            bin = w->mot[4];
        } else {
            bin = w->mot[3];
        }
        break;
    }
    info = ModInfoMgr.create(bin, w->mot[5]);
    if (info) {
        if (w->pHead) {
            em->swapModelInfo(w->pHead->model_addr, info);
        } else {
            em->addModel(info);
        }
        w->pHead = info;
        w->pHead->be_flag |= 8;
    }
}
#endif // PARKED

// Low obstacle test in front (scenario flag 0x20 wall): 0 none, 1 climb over (Target_pos = the landing
// point behind it), 2 the floor behind is far below (jump down instead). Not for Character 5.
// FUNCTION: BIO4 0x004535C0
// STATUS: stub
#if 0 // PARKED: C2228, Mtx, PSMTXMultVec, PSMTXRotRad, STA_NO_FENCE, SatMgr, StaFlagChk, TransMatrix, atan2f, f32, m, mat
extern "C" int em10ClimbOverCk2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    Vec c;
    Vec n;
    Mtx m;
    Vec d;
    Vec e1;
    Vec e2;
    int side;
    f32 ang;
    f32 y;

    if (StaFlagChk(pG, STA_NO_FENCE)) {
        return 0;
    }
    if (w->HoseiCnt % 15 != 11) {
        return 0;
    }
    if (w->Goto_mode == 0 && em->Character == 5) {
        return 0;
    }
    a.x = 0.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    b.x = 0.0f;
    b.y = 500.0f;
    b.z = 800.0f;
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    if (!(SatMgr.hitCheck(&a, &b, &c, &n, 0, 0) & 0x20)) {
        return 0;
    }
    side = 0;
    ang = atan2f(-n.x, -n.z);
    em->ang.y = ang;
    w->Target_pos = em->pos;
    PSMTXRotRad(m, 'y', ang);
    TransMatrix(m, &em->pos);
    e1.x = 300.0f;
    e1.y = 1200.0f;
    e1.z = 0.0f;
    e2.x = 300.0f;
    e2.y = 1200.0f;
    e2.z = 800.0f;
    PSMTXMultVec(m, &e1, &e1);
    PSMTXMultVec(m, &e2, &e2);
    if (SatMgr.hitCheck(&e1, &e2, 0, 0, 0, 0x20)) {
        side = 1;
    }
    e1.x = -300.0f;
    e1.y = 1200.0f;
    e1.z = 0.0f;
    e2.x = -300.0f;
    e2.y = 1200.0f;
    e2.z = 800.0f;
    PSMTXMultVec(m, &e1, &e1);
    PSMTXMultVec(m, &e2, &e2);
    if (SatMgr.hitCheck(&e1, &e2, 0, 0, 0, 0x20)) {
        side |= 2;
    }
    if (side == 3) {
        return 0;
    }
    if (side & 1) {
        d.x = -300.0f;
        d.y = 0.0f;
        d.z = 0.0f;
        PSMTXMultVec(m, &d, &w->Target_pos);
    }
    if (side & 2) {
        d.x = 300.0f;
        d.y = 0.0f;
        d.z = 0.0f;
        PSMTXMultVec(m, &d, &w->Target_pos);
    }
    d.x = 0.0f;
    d.y = 0.0f;
    d.z = 1000.0f;
    PSMTXMultVec(m, &d, &d);
    y = SatMgr.getFloor(&d, 0, 600.0f, 100000.0f, 0);
    if (y < em->pos.y - 500.0f) {
        return 2;
    }
    return 1;
}
#endif // PARKED

// Window / low wall test in front: 0 none, 1 climb through (Target_pos set), 2 drop behind, 3 a window
// object (pWindow) to smash, 4 a door to bash (em10DootAtkCk).
// FUNCTION: BIO4 0x0044AA30
// STATUS: stub
#if 0 // PARKED: C2227, C2228, ChkWindow, EmRackCk, Mtx, PSMTXMultVec, PSMTXRotRad, SatMgr, TransMatrix, atan2f, f32, m, mat
extern "C" int em10WindowCk2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    Vec dir;
    Vec pos;
    u16 status;
    f32 ang;
    f32 y;

    if (w->HoseiCnt % 15 != 10) {
        return 0;
    }
    a.x = 0.0f;
    a.y = 400.0f;
    a.z = 0.0f;
    b.x = 0.0f;
    b.y = 400.0f;
    b.z = 1000.0f;
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    if (!ChkWindow(em, &a, &b, 1, &status, &dir, &pos, &w->pWindow)) {
        return 0;
    }
    if (!w->pWindow->ChkEnableDamage()) {
        return 0;
    }
    Mtx m;
    Vec tmp;
    Vec v;
    ang = atan2f(-dir.x, -dir.z);
    em->ang.y = ang;
    PSMTXRotRad(m, 'y', ang);
    TransMatrix(m, &pos);
    a.x = 0.0f;
    a.y = 0.0f;
    a.z = -400.0f;
    PSMTXMultVec(m, &a, &w->Target_pos);
    w->Target_pos.y = em->pos.y;
    tmp = em->pos;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 1000.0f;
    PSMTXMultVec(m, &v, &v);
    y = SatMgr.getFloor(&v, 0, 600.0f, 100000.0f, 0);
    if (y < em->pos.y - 500.0f) {
        return 2;
    }
    if (!(status & 1)) {
        if ((w->Be_flg & 0x4000) && w->pWindow) {
            w->pWindow->SetBreakAll(&em->pos, 0, 0);
            w->pWindow = 0;
            return 0;
        }
        if (em10DootAtkCk(em) == 0) {
            return 0;
        }
        return 3;
    }
    if (EmRackCk(em, &em->pos, ang)) {
        return 1;
    }
    if (em10DootAtkCk(em) == 0) {
        return 0;
    }
    return 4;
}
#endif // PARKED

// Window decision: em10WindowCk2 -> ClimbOver (0x3C), JumpDown (0x43), WindowAtk (0x3F) or DoorAtk (0x3D).
// FUNCTION: BIO4 0x004539A0
// STATUS: stub
#if 0 // PARKED: room_no, stage_no
int em10WindowCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if ((pG->stage_no == 1 && pG->room_no == 1) && (w->Be_flg & 0x00800000)) {
        return 0;
    }
    switch ((u32) em10WindowCk2(em)) {
    case 0:
    default:
        return 0;
    case 1:
        EmRoutineSet(em, 1, 0x3C, 0, 0);
        return 1;
    case 2:
        EmRoutineSet(em, 1, 0x43, 0, 1);
        return 1;
    case 3:
        EmRoutineSet(em, 1, 0x3F, 0, 0);
        return 1;
    case 4:
        EmRoutineSet(em, 1, 0x3D, 0, 0);
        return 1;
    }
}
#endif // PARKED

// Door in front (cEmDoor list, within 45 deg and reach): opens it when it can (setOpen from the
// Ganado's side), goes to DoorAtk (0x3D) to bash a closed / locked one (kick: also when the door is
// only stuck). 1 when a routine was set.
// FUNCTION: BIO4 0x00453D60
// STATUS: stub
#if 0 // PARKED: C1003, C2059, C2227, C2228, EmDoorWork, EmMgr, cEmDoor, dw, e, f32
int em10DoorOpenCk(cEm10* em, int kick)
{
    Em10Work* w = EM10_WK(em);
    Vec v;
    u32 i;
    f32 ang;

    if (w->HoseiCnt % 15 != 9) {
        return 0;
    }
    if (w->Esc_timer != 0) {
        kick = 1;
    }
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEmDoor* e = (cEmDoor*) EmMgr.fastAt(i);
        EmDoorWork* dw;
        if ((e->be_flag & 0x201) != 1) {
            continue;
        }
        if (e->id != 0x41) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        if ((em->pos.x - e->pos.x) * (em->pos.x - e->pos.x) + (em->pos.y - e->pos.y) * (em->pos.y - e->pos.y) + (em->pos.z - e->pos.z) * (em->pos.z - e->pos.z) > 6250000.0f) {
            continue;
        }
        dw = EMDOOR_WK(e);
        ang = fabsf(Muku2(dw->base_dir, em->ang.y, 3.1415927f));
        if (ang > 0.7853982f && ang < 2.3561945f) {
            continue;
        }
        PSMTXMultVec(dw->base_im, &em->pos, &v);
        if (ang < 1.5707964f) {
            if (v.z > 0.0f || v.z < -800.0f) {
                continue;
            }
        } else {
            if (v.z < 0.0f || v.z > 800.0f) {
                continue;
            }
        }
        if (v.x > dw->Width || v.x < -dw->Width) {
            continue;
        }
        if (v.y > 500.0f || v.y < -500.0f) {
            continue;
        }
        switch (e->ckOpen()) {
        case 0:
        default:
            if (w->Be_flg & 0x4000) {
                e->setOpen(&em->pos, 0, 0, 0);
                return 1;
            }
            if (em->l_pl < 25000000.0f && kick == 0) {
                if (!em10DootAtkCk(em)) {
                    return 0;
                }
                em->r_no_0 = 1;
                em->r_no_1 = 0x3D;
                em->r_no_2 = 0;
                em->r_no_3 = 0;
                return 1;
            }
            e->setOpen(&em->pos, 0, 0, 0);
            return 0;
        case 2:
            if (w->Be_flg & 0x4000) {
                e->setOpen(&em->pos, 0, 0, 0);
                return 1;
            }
            if (em10DootAtkCk(em)) {
                EmRoutineSet(em, 1, 0x3D, 0, 0);
                return 1;
            }
            return 0;
        case 1:
        case 3:
            return 0;
        }
    }
    return 0;
}
#endif // PARKED

// Is a bashable door (alive, closed, within 45 deg in front) still there: 1 = yes (DoorAtk's second swing).
// FUNCTION: BIO4 0x004465B0
// STATUS: stub
#if 0 // PARKED: C2059, C2227, C2228, EMDOOR_WK, EmDoorWork, EmMgr, Mtx, Muku, PSMTXInverse, PSMTXMultVec, PSMTXRotRad, TransMatrix, cEmDoor, d, dw, m
extern "C" int em10AtkDoorCk(cEm10* em)
{
    u32 i;
    Mtx m;
    Vec v;

    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEmDoor* d = (cEmDoor*) EmMgr.fastAt(i);
        EmDoorWork* dw;
        f32 hw;
        if (!(d->be_flag & 1)) {
            continue;
        }
        if (!(d->be_flag & 0x20)) {
            continue;
        }
        if (d->id != 0x41) {
            continue;
        }
        if (d->hp <= 0) {
            continue;
        }
        {
            f32 dx = em->pos.x - d->pos.x;
            f32 dy = em->pos.y - d->pos.y;
            f32 dz = em->pos.z - d->pos.z;
            if (dx * dx + dy * dy + dz * dz > 4000000.0f) {
                continue;
            }
        }
        dw = EMDOOR_WK(d);
        PSMTXRotRad(m, 'y', dw->base_dir);
        TransMatrix(m, &d->pos);
        v.x = -dw->Width * 0.5f;
        v.y = 0.0f;
        v.z = 0.0f;
        PSMTXMultVec(m, &v, &v);
        TransMatrix(m, &v);
        if (fabsf(Muku(&em->pos, &v, em->ang.y, 3.1415927f)) > 0.7853982f) {
            continue;
        }
        PSMTXInverse(m, m);
        PSMTXMultVec(m, &em->pos, &v);
        hw = dw->Width * 0.5f + 300.0f;
        if (v.x > hw) {
            continue;
        }
        if (v.x < -hw) {
            continue;
        }
        if (v.y > 500.0f || v.y < -500.0f) {
            continue;
        }
        if (v.z > 800.0f || v.z < -800.0f) {
            continue;
        }
        if (d->ckOpen() == 1) {
            continue;
        }
        return 1;
    }
    return 0;
}
#endif // PARKED

// Rack / barricade in front (cEmRack list within 45 deg): pushes it over at once (setDown) when it
// can, else goes to RackAtk (0x3E) / DoorAtk (0x3D) by rack type. 1 when a routine was set.
// FUNCTION: BIO4 0x00446EB0
// STATUS: stub
#if 0 // PARKED: C1003, C2059, C2227, C2228, EmMgr, Mtx, Muku, PSMTXInverse, PSMTXMultVec, cEmRack, e, f32, inv
int em10RackBreakCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;
    Mtx inv;
    Vec v;
    Vec a;
    Vec b;

    if (w->HoseiCnt % 15 != 8) {
        return 0;
    }
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEmRack* e = (cEmRack*) EmMgr.fastAt(i);
        if (!(e->be_flag & 1)) {
            continue;
        }
        if (!(e->be_flag & 0x20)) {
            continue;
        }
        if (e->id != 0x45) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        if ((em->pos.x - e->pos.x) * (em->pos.x - e->pos.x) + (em->pos.y - e->pos.y) * (em->pos.y - e->pos.y) + (em->pos.z - e->pos.z) * (em->pos.z - e->pos.z) > 4000000.0f) {
            continue;
        }
        if (fabsf(Muku(&em->pos, &e->pos, em->ang.y, 3.1415927f)) > 0.7853982f) {
            continue;
        }
        PSMTXInverse(e->mat, inv);
        PSMTXMultVec(inv, &em->pos, &v);
        if (v.x > 1000.0f || v.x < -1000.0f) {
            continue;
        }
        if (v.y > 500.0f || v.y < -500.0f) {
            continue;
        }
        if (v.z > 1200.0f || v.z < -1200.0f) {
            continue;
        }
        if (w->Be_flg & 0x4000) {
            e->setDown(&em->pos);
            return 1;
        }
        if (!em10DootAtkCk(em)) {
            return 0;
        }
        a = em->pos;
        b = e->pos;
        b.y = a.y = em->pos.y + 1500.0f;
        if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
            continue;
        }
        {
            // `type` loaded before the loop notes: the LOOP_BEG barrier keeps the ternary's hoisted
            // `li 0x3D` behind the compare, so the temp shares r0 with the loaded byte (and does not
            // inherit e's r3/r11 preferences). Two do { } while (0) levels put the four routine
            // stores at loop depth 4: em then has 34 weighted refs and outranks e (em r31, e r30);
            // the loop notes also keep `li r3, 1` below the stores so the hitCheck result (known 0)
            // stays in r3 for the xFE/xFF zeros.
            int type = e->type;
            do {
                do {
                    EmRoutineSet(em, 1, type == 0 ? 0x3E : 0x3D, 0, 0);
                } while (0);
            } while (0);
        }
        return 1;
    }
    return 0;
}
#endif // PARKED

// Is a bashable rack still in front: 1 = yes.
// FUNCTION: BIO4 0x00446C50
// STATUS: stub
#if 0 // PARKED: C2228, EmMgr, Mtx, Muku, PSMTXInverse, PSMTXMultVec, inv, mat
extern "C" int em10AtkRackCk(cEm10* em)
{
    u32 i;
    Mtx inv;
    Vec v;

    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if (!(e->be_flag & 1)) {
            continue;
        }
        if (!(e->be_flag & 0x20)) {
            continue;
        }
        if (e->id != 0x45) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        {
            f32 dx = em->pos.x - e->pos.x;
            f32 dy = em->pos.y - e->pos.y;
            f32 dz = em->pos.z - e->pos.z;
            if (dx * dx + dy * dy + dz * dz > 4000000.0f) {
                continue;
            }
        }
        if (fabsf(Muku(&em->pos, &e->pos, em->ang.y, 3.1415927f)) > 0.7853982f) {
            continue;
        }
        PSMTXInverse(e->mat, inv);
        PSMTXMultVec(inv, &em->pos, &v);
        if (v.x > 1000.0f || v.x < -1000.0f) {
            continue;
        }
        if (v.y > 500.0f || v.y < -500.0f) {
            continue;
        }
        if (v.z > 1800.0f || v.z < -1800.0f) {
            continue;
        }
        return 1;
    }
    return 0;
}
#endif // PARKED

// Ladder (cObjLadder list) in front when the goto target is more than 1000 units up: takes it
// (setClimb) and goes to LadderClimb (0x40). Not for Character 5.
// FUNCTION: BIO4 0x00447160
// STATUS: stub
#if 0 // PARKED: C2059, C2227, C2228, Mtx, Muku, PSMTXInverse, PSMTXMultVec, PSMTXRotRad, TransMatrix, cObjLadder, f32, m, o, pos_old
int em10LadderClimbCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cObjLadder* o;
    Mtx m;
    Vec v;

    if (w->HoseiCnt % 15 != 4) {
        return 0;
    }
    if (w->Goto_mode == 0 && em->Character == 5) {
        return 0;
    }
    if (w->Go_pos.y - em->pos.y < 1000.0f) {
        return 0;
    }
    // A while loop with the `o = o->next` step repeated before every `continue` (jump2 cross-jumps
    // the copies into one): the copies keep the loop at 82 real insns in loop pass 2, above the
    // 71-insn invariant threshold, so the Muku PI `lis` stays inside the loop like the target.
    o = (cObjLadder*) ObjMgr.getActiveWork();
    while (o) {
        if (o->id != 0x13) {
            o = (cObjLadder*) o->pNext;
            continue;
        }
        if (!o->ckClimb()) {
            o = (cObjLadder*) o->pNext;
            continue;
        }
        {
            f32 dx = em->pos_old.x - o->pos.x;
            f32 dy = em->pos_old.y - o->pos.y;
            f32 dz = em->pos_old.z - o->pos.z;
            if (dx * dx + dy * dy + dz * dz > 4000000.0f) {
                o = (cObjLadder*) o->pNext;
                continue;
            }
        }
        if (fabsf(Muku(&em->pos_old, &o->pos, em->ang.y, 3.1415927f)) > 1.5707964f) {
            o = (cObjLadder*) o->pNext;
            continue;
        }
        PSMTXRotRad(m, 'y', o->ang.y);
        TransMatrix(m, &o->pos);
        PSMTXInverse(m, m);
        PSMTXMultVec(m, &em->pos, &v);
        if (v.z > 1000.0f || v.z < -500.0f) {
            o = (cObjLadder*) o->pNext;
            continue;
        }
        if (v.x > 800.0f || v.x < -800.0f) {
            o = (cObjLadder*) o->pNext;
            continue;
        }
        if (!(fabsf(v.y) > 500.0f)) {
            w->pLadder = o;
            o->setClimb();
            EmRoutineSet(em, 1, 0x40, 0, 0);
            return 1;
        }
        o = (cObjLadder*) o->pNext;
    }
    return 0;
}
#endif // PARKED

// Vertical wall-ladder in front (the room's climb objects, Ganado ids only): sets Target_pos at its top
// and goes to VLadderClimb (0x41, r_no_3 = level) or JumpUp (0x45) for a one-level ledge.
// FUNCTION: BIO4 0x004475C0
// STATUS: stub
#if 0 // PARKED: C2228, EmMgr, Muku2, SceAtSearchLadder, checkStatus, f32, room_no, stage_no
int em10VLadderClimbCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec pos;
    s8 level;
    f32 ang;
    u32 i;

    if ((pG->stage_no == 1 && pG->room_no == 1) || (pG->stage_no == 1 && pG->room_no == 0x11) || (pG->stage_no == 4 && pG->room_no == 0)) {
        return 0;
    }
    switch (em->id) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x16:
    case 0x17:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
    case 0x20:
        break;
    default:
        return 0;
    }
    if (w->HoseiCnt % 15 != 5) {
        return 0;
    }
    if (w->Goto_mode == 0 && em->Character == 5) {
        return 0;
    }
    if (w->Go_pos.y - em->pos.y < 1000.0f) {
        return 0;
    }
    if (!SceAtSearchLadder(em, &pos, &ang, (u8*) &level)) {
        return 0;
    }
    if ((em->pos.x - pos.x) * (em->pos.x - pos.x) + (em->pos.y - pos.y) * (em->pos.y - pos.y) + (em->pos.z - pos.z) * (em->pos.z - pos.z) > 1000000.0f) {
        return 0;
    }
    if (fabsf(Muku2(em->ang.y, ang, 3.1415927f)) > 1.5707964f) {
        return 0;
    }
    if ((em->pos.x - pPL->pos.x) * (em->pos.x - pPL->pos.x) + (em->pos.y - pPL->pos.y) * (em->pos.y - pPL->pos.y) + (em->pos.z - pPL->pos.z) * (em->pos.z - pPL->pos.z) < 4000000.0f) {
        return 0;
    }
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* o = EmMgr.fastAt(i);
        if ((o->be_flag & 0x201) != 1) {
            continue;
        }
        if (o->id <= 0xF) {
            continue;
        }
        if (o->id > 0x20) {
            continue;
        }
        if (o->hp <= 0) {
            continue;
        }
        if (o == em) {
            continue;
        }
        if (!o->checkStatus(EM_STATUS_ACTIVE)) {
            continue;
        }
        if (o->r_no_0 != 1) {
            continue;
        }
        if (o->r_no_1 != 0x41) {
            continue;
        }
        if (!((em->pos.x - o->pos.x) * (em->pos.x - o->pos.x) + (em->pos.y - o->pos.y) * (em->pos.y - o->pos.y) + (em->pos.z - o->pos.z) * (em->pos.z - o->pos.z) > 4000000.0f)) {
            return 0;
        }
    }
    w->Target_dir = ang;
    w->Target_pos = pos;
    if (em->type == 2 || em->type == 0x16) {
        w->Target_dir = ang;
        w->Target_pos = pos;
        w->Target_pos.y += (f32) level * 1000.0f;
        EmRoutineSet(em, 1, 0x45, 0, 0);
        return 1;
    }
    EmRoutineSet(em, 1, 0x41, 0, level);
    return 1;
}
#endif // PARKED

// A knocked-down ladder (ckReset) in front when the target is above: walks to its foot (setGoto
// mode 9) or, when there, reserves it and goes to LadderReset (0x42).
// FUNCTION: BIO4 0x0042EF30
// STATUS: stub
#if 0 // PARKED: C1003, cObjLadder, o
int em10LadderResetCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    cObjLadder* o;
    Vec v;

    if (w->Go_pos.y - em->pos.y < 1000.0f) {
        return 0;
    }
    for (o = (cObjLadder*) ObjMgr.getActiveWork(); o; o = (cObjLadder*) ObjMgr.getNext(o)) {
        f32 d;
        if (o->id != 0x13) {
            continue;
        }
        if (!o->ckReset()) {
            continue;
        }
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 1432.0f;
        PSMTXMultVec(o->mat, &v, &v);
        d = (em->pos.x - v.x) * (em->pos.x - v.x) + (em->pos.z - v.z) * (em->pos.z - v.z);
        if (d > 1210000.0f) {
            if (d < 9000000.0f) {
                em->setGoto(&v, 9);
                return 1;
            }
        } else {
            int zero = 0;
            w->pLadder = o;
            o->setResetReserve();
            EmRoutineSet(em, 1, 0x42, zero, zero);
            return 1;
        }
    }
    return 0;
}
#endif // PARKED

// Edge test ahead (two probes, shield carriers further out): 0 no drop, 1 a low drop, 2 a drop the
// Ganado should jump down.
// FUNCTION: BIO4 0x00447750
// STATUS: stub
#if 0 // PARKED: C2228, Muku2, PSMTXMultVec, SatMgr, atan2f, f32, l_pl, mat
extern "C" int em10JumpDownCk2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec n;
    Vec a;
    Vec b;
    f32 ang;
    int hit;

    if (w->HoseiCnt % 15 != 6) {
        return 0;
    }
    if (w->Go_target == 0) {
        if ((w->Be_flg & 0x08000000) && pSUB != 0) {
            if (w->L_sub < 25000000.0f) {
                if (pSUB->pos.y > em->pos.y - 500.0f) {
                    return 0;
                }
            }
        } else {
            if (em->l_pl < 25000000.0f && pPL->pos.y > em->pos.y - 500.0f) {
                return 0;
            }
        }
    }
    a.x = 100.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    if (w->pShield) {
        b.x = 100.0f;
        b.y = 500.0f;
        b.z = 700.0f;
    } else {
        b.x = 100.0f;
        b.y = 500.0f;
        b.z = 500.0f;
    }
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    hit = SatMgr.hitCheck(&a, &b, 0, &n, 0, 0);
    if (hit & 0x142010) {
        ang = atan2f(-n.x, -n.z);
        if (fabsf(Muku2(em->ang.y, ang, 3.1415927f)) < 0.5235988f) {
            goto found;
        }
    }
    a.x = -100.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    if (w->pShield) {
        b.x = -100.0f;
        b.y = 500.0f;
        b.z = 700.0f;
    } else {
        b.x = -100.0f;
        b.y = 500.0f;
        b.z = 500.0f;
    }
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    hit = SatMgr.hitCheck(&a, &b, 0, &n, 0, 0);
    if (hit & 0x142010) {
        ang = atan2f(-n.x, -n.z);
        if (fabsf(Muku2(em->ang.y, ang, 3.1415927f)) < 0.5235988f) {
found:
            em->ang.y = ang;
            if (hit & 0x40000) {
                return 2;
            }
            return 1;
        }
    }
    return 0;
}
#endif // PARKED

// Jump-down decision: from a roof (cEm::flag bit10) when the floor is below, or em10JumpDownCk2 ->
// JumpDown (0x43); 1 when a routine was set.
// FUNCTION: BIO4 0x004540F0
// STATUS: stub
#if 0 // PARKED: C2228, Frame_cnt, SatMgr
int em10JumpDownCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;

    if ((em->flag & 0x400) && w->Goto_mode == 0) {
        return 0;
    }
    if ((pG->Frame_cnt & 3) == (em->emset_no & 3)) {
        f32 y = SatMgr.getFloor(&em->pos, 0, 600.0f, 100000.0f, 0);
        if (y < em->pos.y - 350.0f) {
            w->Target_pos = em->pos;
            EmRoutineSet(em, 1, 0x43, 0, 0);
            return 1;
        }
    }
    if (w->Route_h < 600.0f) {
        return 0;
    }
    r = em10JumpDownCk2(em);
    switch ((u32) r) {
    case 0:
    default:
        break;
    case 1:
        w->Target_pos = em->pos;
        EmRoutineSet(em, r, 0x43, 0, 0);
        return 1;
    case 2:
        w->Target_pos = em->pos;
        em->r_no_1 = 0x43;
        em->r_no_2 = 0;
        em->r_no_0 = 1;
        em->r_no_3 = 1;
        return 1;
    }
    return 0;
}
#endif // PARKED

// Gap-jump decision: the bulldozer ride (em10BullJumpCk) or a scenario jump wall (flag 0x80000) with
// a floor behind -> Jump (0x44). 1 when a routine was set.
// FUNCTION: BIO4 0x00437E00
// STATUS: stub
#if 0 // PARKED: C2228, Mtx, PSMTXMultVec, PSMTXRotRad, SatMgr, TransMatrix, atan2f, f32, m, mat
int em10JumpCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec n;
    Vec a;
    Vec b;
    Mtx m;
    f32 ang;
    f32 y;

    if (em10BullJumpCk(em)) {
        return 1;
    }
    if (w->HoseiCnt % 15 != 7) {
        return 0;
    }
    a.x = 0.0f;
    a.y = 500.0f;
    a.z = 0.0f;
    if (w->pShield) {
        b.x = 0.0f;
        b.y = 500.0f;
        b.z = 700.0f;
    } else {
        b.x = 0.0f;
        b.y = 500.0f;
        b.z = 500.0f;
    }
    PSMTXMultVec(em->mat, &a, &a);
    PSMTXMultVec(em->mat, &b, &b);
    if (!(SatMgr.hitCheck(&a, &b, 0, &n, 0, 0) & 0x80000)) {
        return 0;
    }
    ang = atan2f(-n.x, -n.z);
    PSMTXRotRad(m, 'y', ang);
    TransMatrix(m, &em->pos);
    a.x = 0.0f;
    a.y = 500.0f;
    a.z = 4500.0f;
    PSMTXMultVec(m, &a, &a);
    y = SatMgr.getFloor(&a, 0, 600.0f, 100000.0f, 0);
    if (fabsf(em->pos.y - y) > 1000.0f) {
        return 0;
    }
    em->ang.y = ang;
    EmRoutineSet(em, 1, 0x44, 0, 0);
    return 1;
}
#endif // PARKED

// Critical claw attack check: after CriAtk_wait ran out and one chance in four, when the way to the
// player is clear -> ClawCriAtk (0x2D) from further than 3000 units (half the time) or ClawWalkAtk
// (0x2C). 1 when set.
// FUNCTION: BIO4 0x0042F790
// STATUS: stub
#if 0 // PARKED: C2228, CTRL12_ID_EM10_ATK, CTRL12_ID_EM10_THROW, Ctrl12Ck, Ctrl12Set, Game_level, STA_SE_BURST, SatMgr, StaFlagChk, l_pl, stage_no
extern "C" int em10ClawCriAtkCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;
    u8 r;

    if (em->type != 0xA && em->type != 0xD) {
        return 0;
    }
    if (!(w->Be_flg & 0x100)) {
        return 0;
    }
    if (w->Atk_wait != 0) {
        return 0;
    }
    if ((s16) w->CriAtk_wait != 0) {
        return 0;
    }
    if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_EM10_ATK)) {
        return 0;
    }
    if ((w->Be_flg & 0x08000000) && pSUB != 0) {
        return 0;
    }
    if (fabsf(em->pos.y - pPL->pos.y) > 1500.0f) {
        return 0;
    }
    if (em->l_pl < 12250000.0f || em->l_pl > 225000000.0f) {
        return 0;
    }
    if (!(Rnd() & 3) && !StaFlagChk(pG, STA_SE_BURST)) {
        w->CriAtk_wait = 150;
        return 0;
    }
    a = em->pos;
    b = pPL->pos;
    a.y += 1500.0f;
    b.y += 1500.0f;
    if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return 0;
    }
    if (em->l_pl > 25000000.0f) {
        EmRoutineSet(em, 1, 0x2D, 0, 0);
    } else if (em->l_pl > 9000000.0f && (r = Rnd() % 10, r > 4)) {
        EmRoutineSet(em, 1, 0x2D, 0, 0);
    } else {
        EmRoutineSet(em, 1, 0x2C, 0, 0);
    }
    if (pG->Game_level <= 3) {
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 60);
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 120);
    } else if (pG->stage_no <= 2 && pG->Game_level <= 9) {
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_ATK, 30);
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_THROW, 120);
    }
    return 1;
}
#endif // PARKED

// "Is there a target to go to" check used by the found Ganados: the bell alarm position (SeInfo.type
// 1 / 2, within 20000 units), the player when the alert is on (Status_flg[1] bit31) and near, when
// he is within 1000 units, or when the room forces it; stores the target in Goto_pos. 1 = target set.
// FUNCTION: BIO4 0x004305E0
// STATUS: stub
#if 0 // PARKED: C2228, FlagChkSign, STA_PL_FIRE, STA_PL_SE_FOOT, STA_SE_BURST, SeInfo, StaFlagChk, Status_flg, l_pl
int em10FindCk2(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (w->Goto_mode) {
        return 0;
    }
    if (StaFlagChk(pG, STA_SE_BURST)) {
        if (pG->SeInfo.type == 2) {
            f32 dx = em->pos.x - pG->SeInfo.pos.x;
            f32 dy = em->pos.y - pG->SeInfo.pos.y;
            f32 dz = em->pos.z - pG->SeInfo.pos.z;
            if (dx * dx + dy * dy + dz * dz < 400000000.0f) {
                w->Goto_pos = pG->SeInfo.pos;
                w->CriAtk_wait = 0;
                return 1;
            }
        }
        if (pG->SeInfo.type == 1) {
            f32 dx = em->pos.x - pG->SeInfo.pos.x;
            f32 dy = em->pos.y - pG->SeInfo.pos.y;
            f32 dz = em->pos.z - pG->SeInfo.pos.z;
            if (dx * dx + dy * dy + dz * dz < 400000000.0f) {
                w->Goto_pos = pG->SeInfo.pos;
                return 1;
            }
        }
    }
    if (FlagChkSign(pG->Status_flg, STA_PL_SE_FOOT)) {
        f32 r = 12250000.0f;
        if (pPL->r_no_0 == 0 && pPL->r_no_1 == 3) {
            r = 64000000.0f;
        }
        if (em->l_pl < r) {
            w->Goto_pos = pPL->pos;
            return 1;
        }
    }
    if (em->l_pl < 1000000.0f) {
        w->Goto_pos = pPL->pos;
        return 1;
    }
    if (StaFlagChk(pG, STA_PL_FIRE) && w->L_pl_route < 25000.0f) {
        w->Goto_pos = pPL->pos;
        return 1;
    }
    return 0;
}
#endif // PARKED

// 1 when another active Ganado stands within 3000 units (the grab from behind is allowed then).
// FUNCTION: BIO4 0x00449380
// STATUS: stub
#if 0 // PARKED: C1003, C2228, EmMgr, checkStatus
int em10SomebodyNearCk(cEm10* em)
{
    u32 i;

    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if ((e->be_flag & 0x201) != 1) {
            continue;
        }
        if (e->id <= 0xF) {
            continue;
        }
        if (e->id > 0x20) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        if (e == em) {
            continue;
        }
        if (!e->checkStatus(EM_STATUS_ACTIVE)) {
            continue;
        }
        {
            f32 dx = em->pos.x - e->pos.x;
            f32 dy = em->pos.y - e->pos.y;
            f32 dz = em->pos.z - e->pos.z;
            if (dx * dx + dy * dy + dz * dz < 9000000.0f) {
                return 1;
            }
        }
    }
    return 0;
}
#endif // PARKED

// Picks the movement routine of a Ganado that knows where the player is: the room-specific post
// routines by cEm::set (FixBomber, RocketWait, Catapult, the bombers, AttackWait, R320Gatling), Pickup
// when a spare weapon should be taken, ignition / claw stick, Stay when the player is dead or too many
// are already attacking, Turn180 when he is behind, the ranged weapons' keep-distance rule, then
// Back / Stay / chainsaw walk attack / Dash checks, and finally Walk with a random Route_type.
// FUNCTION: BIO4 0x00478050
// STATUS: stub
#if 0 // PARKED: C2227, C2228, Game_level, getPartsPtr, l_pl, pl_life
void em10WalkRtnSet(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;

    if (w->Wep_type == 0xC) {
        if (w->pWeapon->getPartsPtr(2)->scale.x == 0.0f) {
            em->setWeaponFall();
        }
    }
    if (em10GotoCk(em)) {
        return;
    }
    switch (em->set) {
    case 0x24:
        EmRoutineSet(em, 1, 0x50, 0, 0);
        return;
    case 0x25:
        EmRoutineSet(em, 1, 0x50, 0, 1);
        return;
    case 0x2B:
        EmRoutineSet(em, 1, 0x50, 0, 2);
        return;
    case 0x36:
        EmRoutineSet(em, 1, 0x60, 0, 0);
        return;
    case 0x19:
        EmRoutineSet(em, 1, 0x48, 0, 0);
        return;
    case 0x3E:
        EmRoutineSet(em, 1, 0x6A, 0, 0);
        return;
    case 0x40:
        EmRoutineSet(em, 1, 0x6C, 0, 0);
        return;
    case 0x23:
        if (!(w->L_pl_guard < em->Guard_r)) {
            EmRoutineSet(em, 1, 0x4F, 0, 0);
            return;
        }
        em->set = 0;
        break;
    case 0x3C:
        if (!(w->L_pl_guard < em->Guard_r)) {
            EmRoutineSet(em, 1, 0x68, 0, 0);
            return;
        }
        em->set = 0;
        break;
    }
    if (w->pWeapon2 != 0 && w->pCore == 0 && w->pParasite == 0 && (w->pWeapon == 0 || w->Wep_type == 5)) {
        em->setWeaponFall();
        EmRoutineSet(em, 1, 0xC, 0, 0);
        return;
    }
    if (em10IgnitionCk(em)) {
        return;
    }
    if (em10ClawStickCK(em)) {
        return;
    }
    if ((s16) pG->pl_life <= 0) {
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return;
    }
    if (em->flag & 0x80) {
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return;
    }
    if (em->Character == 3) {
        if (em->r_no_0 == 1 && em->r_no_1 == 0x1B) {
            return;
        }
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return;
    }
    if (w->Go_rot > 2.7488937f) {
        EmRoutineSet(em, 1, 0x15, 0, 0);
        return;
    }
    if (w->Wep_type == 8 || w->Wep_type == 0xC) {
        if (em->l_pl < 9000000.0f && pG->Game_level <= 9 && !(em->be_flag & 0x20000000)) {
            w->Esc_timer = 120;
            EmRoutineSet(em, 1, 0x11, 0, 0);
            return;
        }
        if (w->Be_flg & 1) {
            EmRoutineSet(em, 1, 0x1B, 0, 0);
            return;
        }
    }
    if (em10BackCk(em)) {
        return;
    }
    if (em10StayCk(em)) {
        return;
    }
    if (em->type == 0x16) {
        EmRoutineSet(em, 1, 0x6D, 0, 0);
        return;
    }
    r = em10DashCk(em);
    if (r) {
        return;
    }
    w->Route_type = Rnd() % 11;
    EmRoutineSet(em, 1, 0x10, 0, 0);
}
#endif // PARKED

// When a goto order is pending (Goto_mode != 0) switches to Goto (0x13) and returns 1.
// FUNCTION: BIO4 0x004389C0
// STATUS: stub
#if 0 // PARKED: C2084
int em10GotoCk(cEm10* em)
{
    if (EM10_WK(em)->Goto_mode) {
        EmRoutineSet(em, 1, 0x13, 0, 0);
        return 1;
    }
    return 0;
}
#endif // PARKED

// Critical-hit (head burst / kick) cut-in camera: fixed offsets from the player matrix, optional shake.
// FUNCTION: BIO4 0x00431910
// STATUS: stub
#if 0 // PARKED: C2227, C2228, Camera, PSMTXMultVec, PSVECAdd, PSVECSubtract, PosToPos, c, fRand1_1, mat, pSys
extern "C" void em10CamMoveCri(cEm10* em, u32 no, int shake)
{
    Em10Work* w = EM10_WK(em);
    Camera* c = &pG->Camera;
    Vec a;
    Vec b;
    Vec hit;
    Vec d;

    switch (no) {
    case 0:
    default:
        a.x = 1716.6f;
        a.y = 3074.7f;
        a.z = 266.8f;
        b.x = -124.6f;
        b.y = 748.4f;
        b.z = -23.5f;
        w->Cam.param.fovy = 50.0f;
        break;
    case 1:
        if (pSys->eff_country == 0) {
            a.x = 762.0f;
            a.y = 1953.0f;
            a.z = 263.0f;
            b.x = -110.0f;
            b.y = 1083.0f;
            b.z = 212.0f;
        } else {
            a.x = -651.2f;
            a.y = 2688.1f;
            a.z = 147.3f;
            b.x = 174.0f;
            b.y = 887.6f;
            b.z = -17.1f;
        }
        w->Cam.param.fovy = 50.0f;
        break;
    case 2:
        a.x = 120.0f;
        a.y = 1689.9f;
        a.z = 1118.5f;
        b.x = 9.2f;
        b.y = 1518.8f;
        b.z = -149.1f;
        w->Cam.param.fovy = 30.0f;
        break;
    case 3:
        if (pSys->eff_country == 0) {
            a.x = -1120.0f;
            a.y = 1269.0f;
            a.z = -494.1f;
            b.x = -16.0f;
            b.y = 1288.0f;
            b.z = 90.0f;
        } else {
            a.x = -87.3f;
            a.y = 1451.2f;
            a.z = -543.1f;
            b.x = 15.6f;
            b.y = 1250.6f;
            b.z = 293.6f;
        }
        w->Cam.param.fovy = 50.0f;
        break;
    case 4:
        a.x = -957.6f;
        a.y = 1741.7f;
        a.z = -87.0f;
        b.x = 104.5f;
        b.y = 1234.5f;
        b.z = 120.06f;
        w->Cam.param.fovy = 50.0f;
        break;
    }
    PSMTXMultVec(pPL->mat, &a, &a);
    PSMTXMultVec(pPL->mat, &b, &b);
    if (shake) {
        PosToPos(&c->param.at, &b, &w->Cam.param.at, 1.0f);
        PosToPos(&c->param.pos, &a, &w->Cam.param.pos, 1.0f);
        a.x = fRand1_1() * 10.0f;
        a.y = fRand1_1() * 10.0f;
        a.z = fRand1_1() * 10.0f;
        PSVECAdd(&w->Cam.param.pos, &a, &w->Cam.param.pos);
        PSVECAdd(&w->Cam.param.at, &a, &w->Cam.param.at);
    } else {
        PosToPos(&c->param.at, &b, &w->Cam.param.at, 1.0f);
        PosToPos(&c->param.pos, &a, &w->Cam.param.pos, 1.0f);
    }
    {
        f32 dx = w->Cam.param.pos.x - w->Cam.param.at.x;
        f32 dy = w->Cam.param.pos.y - w->Cam.param.at.y;
        f32 dz = w->Cam.param.pos.z - w->Cam.param.at.z;
        if (dx * dx + dy * dy + dz * dz > 100.0f) {
            PSVECSubtract(&w->Cam.param.pos, &w->Cam.param.at, &d);
#line 32943 "D:/Bio4/Prog/em10.cpp"
            VECNormalize(&d, &d);
            PSVECScale(&d, &d, 250.0f);
            PSVECAdd(&w->Cam.param.pos, &d, &w->Cam.param.pos);
            if (EatMgr.hitCheck(&w->Cam.param.at, &w->Cam.param.pos, &hit, 0, 0x8000, 0)) {
                w->Cam.param.pos = hit;
            }
            PSVECSubtract(&w->Cam.param.pos, &d, &w->Cam.param.pos);
        }
    }
    w->Cam.Up.x = 0.0f;
    w->Cam.Up.y = 1.0f;
    w->Cam.Up.z = 0.0f;
    {
        f32 dx = w->Cam.param.pos.x - w->Cam.param.at.x;
        f32 dy = w->Cam.param.pos.y - w->Cam.param.at.y;
        f32 dz = w->Cam.param.pos.z - w->Cam.param.at.z;
        w->Cam.Distance = SQRTF(dx * dx + dy * dy + dz * dz);
    }
    CameraSetOrientationUp(&w->Cam);
    CamCtrl.m_pExtraCamera = (s32) &w->Cam;
}
#endif // PARKED

// Grow the parasite (Plaga) out of the neck: the body object and the four head/tentacle objects.
// FUNCTION: BIO4 0x00455100
// STATUS: stub
#if 0 // PARKED: EstSet, MotSetObj16, SetObj16, SndCall, setDieEff, setPlDmgMot
void em10SetParasite(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    PlArc* arc;
    Vec pos;
    Vec rot;
    cObj* o;
    u32 n;
    void* bin;
    void* tpl;
    void* m0;
    void* m1;
    void* m2;
    void* m3;
    void* m4;
    void* m5;
    void* m6;
    void* m7;
    void* m8;
    void* m9;
    void* m10;
    int type;
    int heads;

    if (em->hp <= 0) {
        return;
    }
    em->flag |= 0x20;
    if (w->Ganado == 1) {
        EstSet(em, -1, 0, 0, EFF_EM10, 0x5B, 0, ESP_CORE_KIND_NONE, em, 0);
    } else {
        EstSet(em, -1, 0, 0, EFF_EM10, 8, 0, ESP_CORE_KIND_NONE, em, 0);
    }
    if (em10SearchParasite(em)) {
        return;
    }
    arc = em->subArc;
    n = (((MotionData*) PL_ARC_PTR(arc, 0x253))->maxFrame & 0x3FFF) / 4;
    if (w->Ganado != 1) {
        m5 = PL_ARC_PTR(arc, 0x283);
        type = 2;
        heads = 1;
        bin = PL_ARC_PTR(arc, 0x279);
        tpl = PL_ARC_PTR(arc, 0x27A);
        m0 = PL_ARC_PTR(arc, 0x27B);
        m1 = PL_ARC_PTR(arc, 0x27C);
        m2 = PL_ARC_PTR(arc, 0x280);
        m3 = PL_ARC_PTR(arc, 0x282);
        m7 = PL_ARC_PTR(arc, 0x27D);
        m8 = PL_ARC_PTR(arc, 0x281);
        m9 = PL_ARC_PTR(arc, 0x27E);
        m10 = PL_ARC_PTR(arc, 0x27F);
        m4 = m5;
        m6 = m5;
    } else {
        m1 = PL_ARC_PTR(arc, 0x286);
        m8 = PL_ARC_PTR(arc, 0x289);
        m10 = PL_ARC_PTR(arc, 0x28B);
        type = 3;
        heads = 0;
        bin = PL_ARC_PTR(arc, 0x284);
        tpl = PL_ARC_PTR(arc, 0x285);
        m2 = PL_ARC_PTR(arc, 0x287);
        m3 = PL_ARC_PTR(arc, 0x288);
        m4 = PL_ARC_PTR(arc, 0x28C);
        m5 = PL_ARC_PTR(arc, 0x28A);
        m6 = PL_ARC_PTR(arc, 0x28D);
        m0 = m1;
        m7 = m8;
        m9 = m10;
    }
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    w->pCore = (cObj16*) SetObj16(bin, tpl, em, em, 3, type, &pos, &rot);
    if (w->pCore) {
        w->pCore->setDieEff();
        w->pCore->setMotData(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10);
        EstSet(w->pCore, -1, 0, 0, EFF_EM10, 0xE, 0, w->EffKindIdCore, w->pCore, 0);
        w->pCore->setPlDmgMot(PL_ARC_PTR(em->subArc, 0x17A), PL_ARC_PTR(em->subArc, 0x17B));
    }
    if (heads) {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = -0.6632251f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        o = SetObj16(PL_ARC_PTR(em->subArc, 0x251), PL_ARC_PTR(em->subArc, 0x252), em, w->pCore, 0x16, 1, &pos, &rot);
        if (o) {
            MotSetObj16(o, PL_ARC_PTR(em->subArc, 0x253), 4, 0);
            w->pTen[0] = (cEm*) o;
        }
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.61086524f;
        o = SetObj16(PL_ARC_PTR(em->subArc, 0x251), PL_ARC_PTR(em->subArc, 0x252), em, w->pCore, 0x17, 1, &pos, &rot);
        if (o) {
            MotSetObj16(o, PL_ARC_PTR(em->subArc, 0x253), 4, n);
            w->pTen[1] = (cEm*) o;
        }
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = -0.5235988f;
        o = SetObj16(PL_ARC_PTR(em->subArc, 0x251), PL_ARC_PTR(em->subArc, 0x252), em, w->pCore, 0x18, 1, &pos, &rot);
        if (o) {
            MotSetObj16(o, PL_ARC_PTR(em->subArc, 0x253), 4, n * 2);
            w->pTen[2] = (cEm*) o;
        }
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        o = SetObj16(PL_ARC_PTR(em->subArc, 0x251), PL_ARC_PTR(em->subArc, 0x252), em, w->pCore, 0x19, 1, &pos, &rot);
        if (o) {
            MotSetObj16(o, PL_ARC_PTR(em->subArc, 0x253), 4, n * 3);
            w->pTen[3] = (cEm*) o;
        }
    }
    SndCall(8, 0x8A, &em->pos, em->id, 0, em);
    w->Atk_wait = 0x2D;
}
#endif // PARKED

// Starts the idle motion matching the weapon in hand (hoe, chainsaw, flail / torch / dynamite,
// scythe, rocket, shield, claw, gatling variants) at a random frame, `a` = blend-in frames.
// FUNCTION: BIO4 0x00432250
// STATUS: stub
void em10SetWaitMotion(cEm10* em, int a)
{
    Em10Work* w = EM10_WK(em);
    int flag = (em->flag & 0x01000000) ? 0x45 : 5;
    void* mot = PL_ARC_PTR(em->subArc, 5);
    u32 n;
    int r;

    if (w->pWeapon) {
        if (w->Wep_type == 1) {
            mot = PL_ARC_PTR(em->subArc, 0x14A);
        }
        if (w->Wep_type == 4) {
            mot = PL_ARC_PTR(em->subArc, 0xE6);
        }
        if (w->Wep_type == 11) {
            mot = PL_ARC_PTR(em->subArc, 0xD7);
        }
        if (w->Wep_type == 7) {
            mot = PL_ARC_PTR(em->subArc, 0xD7);
        }
        if (w->Wep_type == 9) {
            mot = PL_ARC_PTR(em->subArc, 0xD7);
        }
        if (w->Wep_type == 6) {
            mot = PL_ARC_PTR(em->subArc, 0x13A);
        }
        if (w->Wep_type == 12) {
            mot = PL_ARC_PTR(em->subArc, 0x17C);
        }
    }
    if (w->pShield) {
        mot = PL_ARC_PTR(em->subArc, 0x165);
    }
    if (em->type == 6) {
        mot = PL_ARC_PTR(em->subArc, 0xA4);
    }
    if (em->type == 10) {
        mot = PL_ARC_PTR(em->subArc, 0x108);
    }
    if (em->type == 13) {
        mot = PL_ARC_PTR(em->subArc, 0x108);
    }
    if (em->type == 2) {
        mot = PL_ARC_PTR(em->subArc, 0x18F);
    }
    n = ((MotionData*) mot)->maxFrame & 0x3FFF;
    r = Rnd();
    MotionSetCore(em, MOTION(em), mot, 0, (u8) a, flag, (u16) (r % n));
}

// Starts the run motion for the weapon in hand (plain, chainsaw, hoe, scythe, torch / flail / dynamite,
// shield, claw, rocket, gatling variants); the r_no_3 low bits pick one of five run styles.
// FUNCTION: BIO4 0x00432A00
// STATUS: stub
#if 0 // PARKED: FlagChkSignW, SYS_OMAKE_ADA_GAME, SYS_OMAKE_ETC_GAME, SysFlagChk, System_flg
void em10SetDashMotion(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int flag = 5;
    u32 v;
    MotionData* m0;
    void* m1;

    if (em->flag & 0x01000000) {
        flag = 0x45;
    }
    v = em->emset_no % 5;
    if (CheckInWater(em, 0)) {
        v = 1;
    }
    if (FlagChkSignW(pG->System_flg, SYS_OMAKE_ADA_GAME) || (SysFlagChk(pG, SYS_OMAKE_ETC_GAME))) {
        if (v == 2) {
            v = 1;
        }
        if (v == 4) {
            v = 3;
        }
    }
    m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xAE);
    switch (v) {
    default:
        m1 = PL_ARC_PTR(em->subArc, 0xAF);
        break;
    case 1:
        m1 = PL_ARC_PTR(em->subArc, 0xB0);
        break;
    case 2:
        m1 = PL_ARC_PTR(em->subArc, 0xB1);
        break;
    case 3:
        m1 = PL_ARC_PTR(em->subArc, 0xB2);
        break;
    case 4:
        m1 = PL_ARC_PTR(em->subArc, 0xB3);
        break;
    }
    if (w->pWeapon && w->Wep_type == 4) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xF1);
        switch (v) {
        default:
            m1 = PL_ARC_PTR(em->subArc, 0xF2);
            break;
        case 1:
            m1 = PL_ARC_PTR(em->subArc, 0xF3);
            break;
        case 2:
            m1 = PL_ARC_PTR(em->subArc, 0xF4);
            break;
        case 3:
            m1 = PL_ARC_PTR(em->subArc, 0xF5);
            break;
        case 4:
            m1 = PL_ARC_PTR(em->subArc, 0xF6);
            break;
        }
    }
    if (w->pWeapon && w->Wep_type == 1) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x151);
        switch (v) {
        default:
            m1 = PL_ARC_PTR(em->subArc, 0x152);
            break;
        case 1:
            m1 = PL_ARC_PTR(em->subArc, 0x153);
            break;
        case 2:
            m1 = PL_ARC_PTR(em->subArc, 0x154);
            break;
        case 3:
            m1 = PL_ARC_PTR(em->subArc, 0x155);
            break;
        case 4:
            m1 = PL_ARC_PTR(em->subArc, 0x156);
            break;
        }
    }
    if (w->pWeapon && w->Wep_type == 6) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x140);
        m1 = PL_ARC_PTR(em->subArc, 0x141);
    }
    if (w->pWeapon && (w->Wep_type == 7 || w->Wep_type == 0xB || w->Wep_type == 9)) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xDE);
        switch (v) {
        default:
            m1 = PL_ARC_PTR(em->subArc, 0xDF);
            break;
        case 1:
            m1 = PL_ARC_PTR(em->subArc, 0xE0);
            break;
        case 2:
            m1 = PL_ARC_PTR(em->subArc, 0xE1);
            break;
        case 3:
            m1 = PL_ARC_PTR(em->subArc, 0xE2);
            break;
        case 4:
            m1 = PL_ARC_PTR(em->subArc, 0xE3);
            break;
        }
    }
    if (em->type == 6) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0xA9);
        m1 = PL_ARC_PTR(em->subArc, 0xAC);
    }
    if (w->pShield) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x168);
        m1 = PL_ARC_PTR(em->subArc, 0x169);
    }
    if (em->type == 0xA || em->type == 0xD) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x10D);
        m1 = PL_ARC_PTR(em->subArc, 0x10E);
    }
    if (w->pWeapon && w->Wep_type == 0xC) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x17F);
        m1 = PL_ARC_PTR(em->subArc, 0x180);
    }
    if (em->type == 2) {
        m0 = (MotionData*) PL_ARC_PTR(em->subArc, 0x192);
        m1 = PL_ARC_PTR(em->subArc, 0x193);
    }
    {
        u16 fr = (u32) ((f32) (m0->maxFrame & 0x3FFF) * (f32) em->r_no_3 / 256.0f);
        MotionSetCore(em, MOTION(em), m0, m1, 5, flag, fr);
    }
}
#endif // PARKED

// Should the walking Ganado start running: not for claw / parasite types, roofs, low rank, or while
// Dash_wait runs; a headless one always dashes, Character 3 stays instead; otherwise when the player
// is far enough (3500 units, 2000 above rank 6), seen, and (below rank 7) looking away; also no more
// than N others already dashing. Sets Dash (0x11) and returns 1.
// FUNCTION: BIO4 0x00449590
// STATUS: stub
#if 0 // PARKED: C2228, EmMgr, Game_level, Muku, checkStatus, l_pl
extern "C" int em10DashCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;
    u32 cnt;
    u32 lim;
    f32 d;
    f32 ang;

    if (w->Wep_type == 9 && w->Fire_timer != 0) {
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return 1;
    }
    if (em->type == 0xA || em->type == 0xD) {
        return 0;
    }
    if (w->pParasite != 0) {
        return 0;
    }
    if (w->pCore != 0) {
        return 0;
    }
    if ((em->flag & 0x400) && w->Goto_mode == 0) {
        return 0;
    }
    if (em->flag & 0x40) {
        return 0;
    }
    if (pG->Game_level <= 1) {
        return 0;
    }
    if (em->flag & 0x80) {
        w->Route_type = Rnd() % 3;
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return 1;
    }
    if (em->Character == 3) {
        if (em->r_no_0 == 1 && em->r_no_1 == 0x1B) {
            return 1;
        }
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return 1;
    }
    if (w->Be_flg & 0x20000000) {
        w->Route_type = Rnd() % 3;
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return 1;
    }
    d = 12250000.0f;
    if (pG->Game_level > 6) {
        d = 4000000.0f;
    }
    if (em->l_pl < d) {
        return 0;
    }
    if (w->Dash_wait != 0) {
        return 0;
    }
    if (!(w->Be_flg & 1)) {
        return 0;
    }
    ang = fabsf(Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f));
    if (pG->Game_level <= 6 && ang > 0.3926991f) {
        return 0;
    }
    cnt = 0;
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* o = EmMgr.fastAt(i);
        if ((o->be_flag & 0x201) != 1) {
            continue;
        }
        if (o->id <= 0xF) {
            continue;
        }
        if (o->id > 0x20) {
            continue;
        }
        if (o->hp <= 0) {
            continue;
        }
        if (o == em) {
            continue;
        }
        if (!o->checkStatus(EM_STATUS_ACTIVE)) {
            continue;
        }
        if (o->l_pl < 12250000.0f) {
            cnt++;
        }
    }
    lim = 0;
    if (pG->Game_level > 3) {
        lim = 1;
    }
    if (w->Ganado != 0) {
        lim = 2;
    }
    if (pG->Game_level > 6) {
        lim = 3;
    }
    if (pG->Game_level > 9) {
        lim = 6;
    }
    if (cnt > lim) {
        return 0;
    }
    {
        int zero = 0; // shared zero pseudo: lands in r7 ahead of the 1 / 0x11 constants
        w->Route_type = Rnd() % 3;
        EmRoutineSet(em, 1, 0x11, zero, zero);
    }
    return 1;
}
#endif // PARKED

// Should the Ganado stop and stand off (Stay 0x1B): Character 3 always, when the player is out of
// the guard range (Character 0), when the partner-chasers are enough (em10GoSubStayCk), or when too
// many others are already close to the player (rank-dependent count); the tower rooms (101 / 111 /
// 400) send it to one of four waiting points instead. Headless / lit-dynamite Ganados dash. 1 when set.
// FUNCTION: BIO4 0x004556E0
// STATUS: stub
#if 0 // PARKED: C2228, EmMgr, Game_level, checkStatus, l_pl, room_no, stage_no
extern "C" int em10StayCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;
    u32 n;

    if (em10GotoCk(em)) {
        return 1;
    }
    if (em->flag & 0x40) {
        return 0;
    }
    if (w->Be_flg & 0x08000000) {
        return 0;
    }
    if (em->type == 0x16) {
        return 0;
    }
    if (em->flag & 0x80) {
        w->Route_type = Rnd() % 3;
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return 1;
    }
    if (w->Wep_type == 9 && w->Fire_timer != 0) {
        EmRoutineSet(em, 1, 0x11, 0, 0);
        return 1;
    }
    if (((pG->stage_no == 1 && pG->room_no == 1) || (pG->stage_no == 1 && pG->room_no == 0x11) ||
         (pG->stage_no == 4 && pG->room_no == 0)) &&
        pPL->pos.y > 6000.0f && em->l_pl < 144000000.0f) {
        if (em->l_pl < 36000000.0f) {
            Vec tbl[4] = {
                { 13913.0f, 215.0f, 322.0f },
                { 10172.0f, 215.0f, 1311.0f },
                { 14208.0f, 215.0f, -11076.0f },
                { 23407.0f, 1215.0f, -1110.0f },
            };
            em->setGoto(&tbl[Rnd() & 3], 12);
            return 0;
        }
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return 1;
    }
    if (em->Character == 3) {
        if (em->r_no_0 == 1 && em->r_no_1 == 0x1B) {
            return 1;
        }
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return 1;
    }
    if (em10ReturnCk(em)) {
        return 1;
    }
    if (em->Character == 0 && w->Pl_in_ck == 0 && w->L_pl_guard > em->Guard_r + 2000.0f) {
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return 1;
    }
    if (w->Be_flg & 0x08000000) {
        if (em10GoSubStayCk(em)) {
            return 1;
        }
        return 0;
    }
    if (em->type == 0xA || em->type == 0xD) {
        return 0;
    }
    n = 0;
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if ((e->be_flag & 0x201) == 1 && e->id > 0xF && e->id <= 0x20 && e->hp > 0 && e != em &&
            e->checkStatus(EM_STATUS_ACTIVE) && EM10_WK(e)->L_pl_route < w->L_pl_route) {
            n++;
        }
    }
    if (pG->Game_level <= 1 && n == 0) {
        return 0;
    }
    if (pG->Game_level <= 3 && n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 0;
    }
    if (em->Character == 2) {
        if (pG->Game_level <= 3) {
            if (w->L_pl_route > 6000.0f) {
                return 0;
            }
        } else {
            if (w->L_pl_route > 4000.0f) {
                return 0;
            }
        }
    }
    if (n <= 7) {
        if (w->L_pl_route > 8000.0f) {
            return 0;
        }
    }
    if (w->L_pl_route > 12000.0f) {
        return 0;
    }
    EmRoutineSet(em, 1, 0x1B, 0, 0);
    return 1;
}
#endif // PARKED

// Torch swing hit on the player: the flame within Em10AtkTbl[0x11] range in front sets him on fire
// (plemDmFrame damage routine, ctrl12 NOT_NEAR 30). 1 = hit.
// FUNCTION: BIO4 0x00456F30
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_EM10_NOT_NEAR, Ctrl12Set, EmDeadCk, Mtx, PSMTXInverse, PSMTXMultVec, SetPlDamage, SndCall, m, mat, pl_life, set
extern "C" int em10TorchFrameAtkCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Vec v;

    if ((s16) pG->pl_life <= 0) {
        return 0;
    }
    if (EmDeadCk(pPL)) {
        return 0;
    }
    if (w->Atk_ck) {
        return 0;
    }
    PSMTXInverse(em->mat, m);
    PSMTXMultVec(m, &pPL->pos, &v);
    if (!(v.x > 500.0f) && !(v.x < -500.0f) && !(v.z > 3500.0f) && !(v.z < 0.0f) && !(v.y > 1500.0f) && !(v.y < -500.0f)) {
        w->Atk_ck = 1;
        SndCall(8, 0x8F, &pPL->pos, em->id, 0, pPL);
        Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 30);
        SetPlDamage(em, plemDmFrame);
        pPL->dmg.set(0, 30);
        return 1;
    }
    return 0;
}
#endif // PARKED

// Torch swing hit on the partner: registers a burn damage (kind 0x18) on her cDmgInfo. 1 = hit.
// FUNCTION: BIO4 0x004331A0
// STATUS: stub
#if 0 // PARKED: C2227, CTRL12_ID_EM10_NOT_NEAR, Ctrl12Set, Em10AtkTbl, EmAtkHitSubCk2, EmDeadCk, Mtx, PSMTXInverse, PSMTXMultVec, SndCall, YARARE_INFO, hit, inv, mat, set
extern "C" int em10TorchFrameAtkCkSub(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx inv;
    Vec v;
    YARARE_INFO* hit;
    u8 old;

    if (!pSUB) {
        return 0;
    }
    if (pSUB->hp <= 0) {
        return 0;
    }
    if (EmDeadCk(pSUB)) {
        return 0;
    }
    old = w->Atk_ck2;
    if (old) {
        return 0;
    }
    PSMTXInverse(em->mat, inv);
    PSMTXMultVec(inv, &pSUB->pos, &v);
    if (v.x > 500.0f || v.x < -500.0f) {
        return 0;
    }
    if (v.z > 3500.0f || v.z < 0.0f) {
        return 0;
    }
    if (v.y > 1500.0f || v.y < -500.0f) {
        return 0;
    }
    w->Atk_ck2 = 1;
    SndCall(8, 0x8F, &pSUB->pos, em->id, 0, pSUB);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_EM10_NOT_NEAR, 0x1E);
    EstSet(pSUB, -1, 0, 0, EFF_EM10, 0x28, 0, ESP_CORE_KIND_NONE, pSUB, (void*) old);
    v = pSUB->pos;
    cDmgInfo* dmg = &pSUB->dmg; // held across the call (r31)
    v.y += 1300.0f;
    hit = EmAtkHitSubCk2(&Em10AtkTbl[17], &v, &em->pos);
    if (hit) {
        dmg->set(0, 0xA, 0x18, &em->pos, hit->len, hit);
    }
    return 1;
}
#endif // PARKED

// Routine set the moment the Ganado finds the player (em10FindCk): marks him found (setFindPL),
// re-arms the bowgun / drops the spare, ignition / claw checks, then the Find shout (0x0D, only one
// Ganado at a time and only when the player looks at it), Turn180 when he is behind, else the walk.
// FUNCTION: BIO4 0x00460F60
// STATUS: stub
#if 0 // PARKED: Muku, l_pl
extern "C" void em10SetRtnFind(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    int r;
    f32 ang;

    em->setFindPL();
    if (w->pShield) {
        return;
    }
    if (em->type == 10 || em->type == 13 || em->type == 2) {
        return;
    }
    if (w->Wep_type == 8) {
        return;
    }
    if (w->Be_flg & 0x80) {
        return;
    }
    if (w->pWeapon2 && !w->pCore && !w->pParasite && (!w->pWeapon || w->Wep_type == 5)) {
        em->setWeaponFall();
        EmRoutineSet(em, 1, 0xC, 0, 0);
        return;
    }
    if (em10IgnitionCk(em)) {
        return;
    }
    r = em10ClawStickCK(em);
    if (r) {
        return;
    }
    if (em10SomebodyFindNowCk(em)) {
        if (w->Pl_rot < 1.5707964f) {
            ang = fabsf(Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f));
            if (em->l_pl < 9000000.0f || (em->l_pl < 49000000.0f && ang < 0.7853982f)) {
                em10WalkRtnSet(em);
            } else {
                EmRoutineSet(em, 1, 0xD, 0, 0);
            }
        } else {
            em->r_no_1 = 0x15;
            em->r_no_2 = r;
            em->r_no_0 = 1;
            em->r_no_3 = 1;
        }
    } else {
        em10WalkRtnSet(em);
    }
}
#endif // PARKED

// Drops the weapon in hand (cEmWep::setFall) unless the room forbids it: heavy weapons only fall when
// hp is below 300, the chainsaw only from a dead Ganado (its effects and running flag are cleared).
// FUNCTION: BIO4 0x00433940
// STATUS: stub
#if 0 // PARKED: EffectEfmDelete, EffectEspDelete, EffectEspgenDelete, room_no, setFall, stage_no
void cEm10::setWeaponFall()
{
    Em10Work* w = EM10_WK(this);
    u8 wtype;

    if (!w->pWeapon) {
        return;
    }
    if ((pG->stage_no == 1 && pG->room_no == 0) && hp > 0) {
        return;
    }
    if (type == 6) {
        return;
    }
    wtype = w->Wep_type;
    if (wtype == 9) {
        return;
    }
    if (!(w->Be_flg & 0x80) && !(flag & 0x40)) {
        if (wtype == 8) {
            return;
        }
        if (type == 0x18) {
            return;
        }
    }
    switch (w->Wep_type) {
    case 6:
    case 7:
    case 0xB:
    case 0xF:
    case 0x10:
        if (hp > 300) {
            return;
        }
        break;
    }
    EffectEspDelete(0, w->EffKindIdWork, this, 0);
    EffectEspgenDelete(0, w->EffKindIdWork, this);
    EffectEfmDelete(0, w->EffKindIdWork, this);
    if (w->Wep_type == 4) {
        if (hp > 0) {
            return;
        }
        if (!(w->Be_flg & 0x80000000)) {
            return;
        }
        EffectEspDelete(0, w->EffKindIdCsaw, w->pWeapon, 0);
        EffectEspgenDelete(0, w->EffKindIdCsaw, w->pWeapon);
        EffectEfmDelete(0, w->EffKindIdCsaw, w->pWeapon);
        w->Be_flg &= 0x7FFFFFFF;
    } else {
        w->pWeapon->setFall(0, 0, 20.0f);
        w->pWeapon = 0;
        w->Wep_type = 0;
    }
}
#endif // PARKED

// Sends the Ganado to `pos` (Goto_pos snapped to the floor; Goto_mode = `range`, the goto kind the Goto
// routine interprets) and marks it heading somewhere (work flags 0x04000004); some kinds forget the player.
// FUNCTION: BIO4 0x00433C90
// STATUS: stub
#if 0 // PARKED: C2228, SatMgr
void cEm10::setGoto(Vec* pos, int range)
{
    Em10Work* w = EM10_WK(this);
    f32 y;

    if (w->Be_flg & 0x4000) {
        return;
    }
    w->Goto_mode = range;
    w->Goto_pos = *pos;
    y = SatMgr.getFloor(&w->Goto_pos, 0, 600.0f, 100000.0f, 0);
    if (y != -100000.0f) {
        w->Goto_pos.y = y + 50.0f;
    }
    w->Route_target = *pos;
    w->Be_flg |= 0x04000004;
    if (!(flag & 0x40)) {
        w->Be_flg &= ~0x100;
    }
    w->Be_flg &= ~0x800000;
}
#endif // PARKED

// Sends the Ganado to the switch `sw` to operate it: Goto_mode 3 (open, `near`) or 4 (close) at `pos`.
// FUNCTION: BIO4 0x00433DA0
// STATUS: stub
#if 0 // PARKED: C2228, SatMgr
void cEm10::setGotoSwitch(cModel* sw, int near, Vec* pos)
{
    Em10Work* w = EM10_WK(this);
    f32 y;

    if (w->Be_flg & 0x4000) {
        return;
    }
    w->Goto_mode = near ? 3 : 4;
    if (pos) {
        w->Goto_pos = *pos;
    } else {
        w->Goto_pos = sw->pos;
    }
    y = SatMgr.getFloor(&w->Goto_pos, 0, 600.0f, 100000.0f, 0);
    if (y != -100000.0f) {
        w->Goto_pos.y = y + 50.0f;
    }
    w->pSwitch = sw;
    w->Route_target = w->Goto_pos;
    w->Be_flg |= 0x04000004;
    if (!(flag & 0x40)) {
        w->Be_flg &= ~0x100;
    }
    w->Be_flg &= ~0x800000;
}
#endif // PARKED

// Offers the melee action button (kick / suplex ...) on a staggered Ganado: alive, the player facing
// it within 60 deg at melee range with a clear line; the button kind and reach depend on the player
// character (Leon / Ada / HUNK / Krauser / Wesker) and on a parasite being out.
// FUNCTION: BIO4 0x00461170
// STATUS: stub
#if 0 // PARKED: C1003, l_pl
extern "C" void em10ActEvtSetKick(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Vec a;
    Vec b;

    if (em->hp <= 0) {
        return;
    }
    switch (pG->pl_type) {
    default:
        if (em->l_pl > 2250000.0f) {
            return;
        }
        break;
    case 5:
        if (em->l_pl > 2250000.0f) {
            return;
        }
        break;
    case 4:
        if (em->l_pl > 4000000.0f) {
            return;
        }
        break;
    }
    if (fabsf(Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f)) > 1.0471976f) {
        return;
    }
    if (em->type == 10 || em->type == 13 || em->type == 2 || em->type == 0x16) {
        return;
    }
    if (fabsf(em->pos.y - pPL->pos.y) > 700.0f) {
        return;
    }
    EM10_KICK_LINE(a, b, em);
    if (SatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
        return;
    }
    if (pG->pl_type == 3) {
        EM10_KICK_LINE(a, b, em);
        if (EatMgr.hitCheck(&a, &b, 0, 0, 0, 0)) {
            return;
        }
    }
    switch (pG->pl_type) {
    default:
        if (w->Be_flg & 0x40000000) {
            ActBtn.set(ACT_KICK, 0xB, (void*) em10KneeDownAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        } else {
            ActBtn.set(ACT_KICK, 0xB, (void*) em10KickAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        }
        break;
    case 5:
        if (w->Be_flg & 0x40000000) {
            ActBtn.set(ACT_NERICHAGI, 0xB, (void*) em10KneeDownAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        } else {
            ActBtn.set(ACT_PALM_SHOCK, 0xB, (void*) em10KickAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        }
        break;
    case 2:
        if (w->Be_flg & 0x40000000) {
            ActBtn.set(ACT_BACKKICK, 0xB, (void*) em10KneeDownAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        } else {
            ActBtn.set(ACT_SENPUU, 0xB, (void*) em10KickAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        }
        break;
    case 3:
        if (w->Be_flg & 0x40000000) {
            ActBtn.set(ACT_KICK, 0xB, (void*) em10KneeDownAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        } else if (w->pCore != 0 || w->pParasite != 0 || (w->Be_flg & 0x80)) {
            ActBtn.set(ACT_KICK, 0xB, (void*) em10KneeDownAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        } else {
            ActBtn.set(ACT_EXECUTE, 0xB, (void*) em10KickAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        }
        break;
    case 4:
        ActBtn.set(ACT_KICK, 0xB, (void*) em10KickAction, em, ACTCTR_WEP_SET_IGNORE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
        break;
    }
}
#endif // PARKED

// Robed type 6 Ganado (the merchant model): offers the "trade" action button when the player faces
// him inside the talk box (bigger box in the shop rooms 20F / 301 / 305) and no other Ganado is
// within 10000 units.
// FUNCTION: BIO4 0x0044A6B0
// STATUS: stub
#if 0 // PARKED: ACTCTR_NONE, ACT_FUNC_NORMAL, ACT_TALK, ActBtn, C2228, DISP_A_NORMAL, EmMgr, Mtx, Muku, PSMTXInverse, PSMTXMultVec, checkStatus, inv, l_pl, mat, room_id
void em10ActEvtSetTrade(cEm10* em)
{
    Mtx inv;
    Vec v;
    u32 i;
    f32 a;

    if (em->type != 6) {
        return;
    }
    if (em->hp <= 0) {
        return;
    }
    if (fabsf(Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f)) > 0.7853982f) {
        return;
    }
    PSMTXInverse(pPL->mat, inv);
    PSMTXMultVec(inv, &em->pos, &v);
    switch (pG->room_id) {
    default:
        if (v.z > 1500.0f) {
            return;
        }
        if (v.z < 0.0f) {
            return;
        }
        if (v.x > 700.0f) {
            return;
        }
        if (v.x < -700.0f) {
            return;
        }
        a = fabsf(v.y);
        if (a > 700.0f) {
            return;
        }
        break;
    case 0x20F:
    case 0x301:
    case 0x305:
        if (em->l_pl > 2250000.0f) {
            if (v.z > 3000.0f) {
                return;
            }
            if (v.z < 0.0f) {
                return;
            }
            if (v.x > 700.0f) {
                return;
            }
            if (v.x < -700.0f) {
                return;
            }
        }
        a = fabsf(v.y);
        if (a > 700.0f) {
            return;
        }
        break;
    }
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if ((e->be_flag & 0x201) != 1) {
            continue;
        }
        if (e->id <= 0xF) {
            continue;
        }
        if (e->id > 0x20) {
            continue;
        }
        if (e == em) {
            continue;
        }
        if (!e->checkStatus(EM_STATUS_ACTIVE)) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        {
            f32 dx = em->pos.x - e->pos.x;
            f32 dy = em->pos.y - e->pos.y;
            f32 dz = em->pos.z - e->pos.z;
            if (dx * dx + dy * dy + dz * dz < 100000000.0f) {
                return;
            }
        }
    }
    ActBtn.set(ACT_TALK, 2, (void*) em10TradeAction, em, ACTCTR_NONE, DISP_A_NORMAL, ACT_FUNC_NORMAL, 0);
}
#endif // PARKED

// Knocked against a roof / wall: pick the wall direction (x5DC) and start damage routine 2-7.
// FUNCTION: BIO4 0x0044BF80
// STATUS: stub
#if 0 // PARKED: C2228, EM10_ROOF_PROBE, Game_level, LIMIT_ANGLE, Mtx, PSMTXMultVec, PSMTXRotRad, SatMgr, TransMatrix, atan2f, m, mat
int em10RoofDmCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Vec a;
    Vec b;
    Vec nrm;
    u32 mask = 0x00182810;

    switch (em->set) {
    case 0x37:
        w->Target_dir = em->ang.y;
        em->be_flag |= 0x10000;
        em->hp = 0;
        EmRoutineSet(em, 2, 7, 0, 1);
        return 1;
    case 0x2D:
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        em->be_flag |= 0x10000;
        em->hp = 0;
        EmRoutineSet(em, 2, 7, 0, 1);
        return 1;
    case 0x2C:
    case 0x31:
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        em->hp = 0;
        em->be_flag |= 0x10000;
        if (em->r_no_2 < 4) {
            EmRoutineSet(em, 2, 7, 0, 1);
        } else {
            EmRoutineSet(em, 2, 7, 0, 2);
        }
        return 1;
    case 0x27:
    case 0x28:
    case 0x29:
        w->Target_dir = em->ang.y;
        if (Rnd() & 1) {
            w->Target_dir += 1.5707964f;
        } else {
            w->Target_dir -= 1.5707964f;
        }
        w->Target_dir = LIMIT_ANGLE(w->Target_dir);
        em->be_flag |= 0x10000;
        em->hp = 0;
        EmRoutineSet(em, 2, 7, 0, 1);
        return 1;
    default:
        break;
    }
    if (w->Wep_type == 4) {
        u8 r = Rnd() % 3;
        if (r != 0) {
            return 0;
        }
    }
    if (em->type == 0xA || em->type == 0xD || em->type == 2 || em->type == 0x16) {
        return 0;
    }
    if (pG->Game_level > 6) {
        u8 r = Rnd() % 100;
        if (r > 0x4B) {
            return 0;
        }
    }
    if (pG->Game_level == 10) {
        return 0;
    }
    a = em->pos;
    a.y += 500.0f;
    EM10_ROOF_PROBE(0.0f, 500.0f, 800.0f);
    EM10_ROOF_PROBE(800.0f, 500.0f, 0.0f);
    EM10_ROOF_PROBE(-800.0f, 500.0f, 0.0f);
    b.x = 0.0f;
    b.y = 500.0f;
    b.z = -800.0f;
    PSMTXMultVec(em->mat, &b, &b);
    if (SatMgr.hitCheck(&a, &b, 0, &nrm, 0, 0) & mask) {
        PSMTXRotRad(m, 'y', atan2f(-nrm.x, -nrm.z));
        TransMatrix(m, &em->pos);
        b.x = 0.0f;
        b.y = 500.0f;
        b.z = 800.0f;
        PSMTXMultVec(m, &b, &b);
        if (SatMgr.hitCheck(&a, &b, 0, &nrm, 0, 0) & mask) {
            w->Target_dir = atan2f(nrm.x, nrm.z);
            EmRoutineSet(em, 2, 7, 0, 0);
            return 1;
        }
    }
    return 0;
}
#endif // PARKED

// 1 when another active Ganado stands in the throwing lane (500 wide, 3000 high) between this one and
// the player: the thrower / shooter side-steps instead.
// FUNCTION: BIO4 0x0044AB60
// STATUS: stub
#if 0 // PARKED: C2228, EmMgr, Mtx, PSMTXInverse, PSMTXMultVec, PSMTXRotRad, TransMatrix, checkStatus, l_pl, m
extern "C" int em10ThrowNearCk(cEm10* em)
{
    Mtx m;
    Vec v;
    u32 i;

    PSMTXRotRad(m, 'y', GetXZAngle(&em->pos, &pPL->pos));
    TransMatrix(m, &em->pos);
    PSMTXInverse(m, m);
    for (i = 0; i < EmMgr.getArrayNum(); i++) {
        cEm* e = EmMgr.fastAt(i);
        if ((e->be_flag & 0x201) != 1) {
            continue;
        }
        if (e->id <= 0xF) {
            continue;
        }
        if (e->id > 0x20) {
            continue;
        }
        if (e->hp <= 0) {
            continue;
        }
        if (e == em) {
            continue;
        }
        if (!e->checkStatus(EM_STATUS_ACTIVE)) {
            continue;
        }
        PSMTXMultVec(m, &e->pos, &v);
        if (em->l_pl < v.z * v.z) {
            continue;
        }
        if (v.z < 0.0f) {
            continue;
        }
        if (v.y > 1500.0f) {
            continue;
        }
        if (v.y < -1500.0f) {
            continue;
        }
        if (v.x > 250.0f) {
            continue;
        }
        if (v.x < -250.0f) {
            continue;
        }
        return 1;
    }
    return 0;
}
#endif // PARKED

// Resets the Ganado to its start state for a room re-entry: visible, full hp, head back on, body
// scale from emset_no, the accessories hidden again, weapon / shield recreated, Keep_pos = St_pos,
// the start routine (em10InitRtnSet) and one R0_Move frame.
// FUNCTION: BIO4 0x004577A0
// STATUS: stub
#if 0 // PARKED: C2227, C2228, cModelInfo, fRand1_1, hitInfo, invisible_factor, pModelInfo, pParts, partsWorldCalc, pos_old
void cEm10::setReset()
{
    Em10Work* w = EM10_WK(this);
    f32 sc;
    cModel* p;
    cModelInfo* info;

    invisible_factor = 1.0f;
    be_flag = (be_flag | 2) & ~0x10000;
    atari.m_flag |= 0x300;
    atari.m_flag &= ~0x10;
    em10HeadSet(this, 0);
    switch (emset_no & 0xF) {
    default: {
        sc = fRand1_1() * 0.01f + 1.03f;
        const f32 k = 1.01f; // pool order (see em10_R0_Init)
        break;
    }
    case 7:
        sc = 1.1f;
        break;
    case 0:
    case 4:
    case 9:
    case 0xB:
        sc = fRand1_1() * 0.01f + 1.01f;
        break;
    }
    if (type == 6) {
        sc = 1.05f;
    }
    scale.z = sc;
    scale.y = sc;
    scale.x = sc;
    w->Scale = scale;
    p = getPartsPtr(0x1B);
    p->scale.x = 1.0f;
    p->scale.y = 1.0f;
    p->scale.z = 1.0f;
    pos = w->St_pos;
    pos_old = pos;
    ang.x = 0.0f;
    ang.y = w->St_dir;
    ang.z = 0.0f;
    hp = hp_max;
    w->Keep_pos = w->St_pos;
    set = w->St_set;
    if (w->pHood) {
        w->pHood->be_flag |= 8;
    }
    if (w->pWhood) {
        w->pWhood->be_flag |= 8;
    }
    if (w->pAccesory[0]) {
        w->pAccesory[0]->be_flag |= 8;
    }
    if (w->pAccesory[1]) {
        w->pAccesory[1]->be_flag |= 8;
    }
    if (w->pAccesory[3]) {
        w->pAccesory[3]->be_flag |= 8;
    }
    if (w->pAccesory[4]) {
        w->pAccesory[4]->be_flag |= 8;
    }
    if (w->pAccesory[5]) {
        w->pAccesory[5]->be_flag |= 8;
    }
    if (w->pAccesory[6]) {
        w->pAccesory[6]->be_flag |= 8;
    }
    em10WeaponInit(this);
    em10ShieldSet(this);
    for (info = pModelInfo; info; info = info->pList) {
        info->color[0] = 0xFF;
        info->color[1] = 0xFF;
        info->color[2] = 0xFF;
    }
    em10InitRtnSet(this);
    hitInfo.flag = 1;
    MotionMove(this, 0);
    em10_R0_Move(this);
    partsWorldCalc();
    for (p = pParts; p; p = p->pParts) {
        p->world_old = p->world;
        p->world_old2 = p->world_old;
    }
}
#endif // PARKED

// Removes the Ganado from play without a death: collision off, parasite / core / weapons lost,
// invisible and inactive (work flag 0x400000).
// FUNCTION: BIO4 0x00457B70
// STATUS: stub
#if 0 // PARKED: EmReserveDropItem, EmSetDie, setLost, setReset, throughOn
void cEm10::setLost()
{
    Em10Work* w = EM10_WK(this);

    atari.throughOn();
    EmSetDie(this);
    EmReserveDropItem(this);
    clearStatus(EM_STATUS_ACTIVE);
    em10CoreBreak(this, 1);
    if (w->pParasite) {
        w->pParasite->setReset();
        w->pParasite = 0;
    }
    w->Compress_y = 1.0f;
    if (w->pWeapon) {
        w->pWeapon->setLost();
        w->pWeapon = 0;
        w->Wep_type = 0;
    }
    if (w->pWeapon2) {
        w->pWeapon2->setLost();
        w->pWeapon2 = 0;
        w->Wep_type2 = 0;
    }
    be_flag &= ~2;
    w->Be_flg |= 0x400000;
    w->Reset_enable = 1;
}
#endif // PARKED

// Looks for an EMI hide point (type 1 entries: sub 0 right / 1 left cover, 2 a bowgun perch) near the
// Ganado (1500..2500 units) that faces the player from more than 5000 away: takes it and hides there
// (HideSide 0x18 / SitDown 0x1A). 1 when set.
// FUNCTION: BIO4 0x00436570
// STATUS: stub
#if 0 // PARKED: C1003, EMI_DATA, Mtx, m
extern "C" int em10HideRtnCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    Mtx m;
    Vec v;
    u32 i;
    EmiEntry* e;
    f32 ang;
    f32 a;

    if (pG->pEmi == 0) {
        return 0;
    }
    for (i = 0; i < EMI_DATA->n; i++) {
        e = &EMI_DATA->entry[i];
        if (e->type != 1) {
            continue;
        }
        switch (e->sub) {
        case 0:
            {
                f32 dx = e->pos.x - em->pos.x;
                f32 dy = e->pos.y - em->pos.y;
                f32 dz = e->pos.z - em->pos.z;
                f32 d = dx * dx + dy * dy + dz * dz;
                if (d > 6250000.0f) {
                    continue;
                }
                if (d < 2250000.0f) {
                    continue;
                }
            }
            ang = Muku(&e->pos, &pPL->pos, e->rotY, 3.1415927f);
            a = fabsf(ang);
            if (a > 1.0471976f) {
                continue;
            }
            {
                f32 dx = e->pos.x - pPL->pos.x;
                f32 dy = e->pos.y - pPL->pos.y;
                f32 dz = e->pos.z - pPL->pos.z;
                if (dx * dx + dy * dy + dz * dz < 25000000.0f) {
                    continue;
                }
            }
            PSMTXRotRad(m, 'y', e->rotY);
            TransMatrix(m, &e->pos);
            PSMTXMultVec(m, &em10_hide_ofs_r, &v);
            {
                f32 dx = v.x - em->pos.x;
                f32 dy = v.y - em->pos.y;
                f32 dz = v.z - em->pos.z;
                if (dx * dx + dy * dy + dz * dz > 1440000.0f) {
                    continue;
                }
            }
            em->ang.y = e->rotY;
            em->r_no_0 = 1;
            em->r_no_1 = 0x17;
            em->r_no_2 = 0;
            em->r_no_3 = 1;
            return 1;
        case 1:
            {
                f32 dx = e->pos.x - em->pos.x;
                f32 dy = e->pos.y - em->pos.y;
                f32 dz = e->pos.z - em->pos.z;
                f32 d = dx * dx + dy * dy + dz * dz;
                if (d > 6250000.0f) {
                    continue;
                }
                if (d < 2250000.0f) {
                    continue;
                }
            }
            ang = Muku(&e->pos, &pPL->pos, e->rotY, 3.1415927f);
            a = fabsf(ang);
            if (a > 1.0471976f) {
                continue;
            }
            {
                f32 dx = e->pos.x - pPL->pos.x;
                f32 dy = e->pos.y - pPL->pos.y;
                f32 dz = e->pos.z - pPL->pos.z;
                if (dx * dx + dy * dy + dz * dz < 25000000.0f) {
                    continue;
                }
            }
            PSMTXRotRad(m, 'y', e->rotY);
            TransMatrix(m, &e->pos);
            PSMTXMultVec(m, &em10_hide_ofs_l, &v);
            {
                f32 dx = v.x - em->pos.x;
                f32 dy = v.y - em->pos.y;
                f32 dz = v.z - em->pos.z;
                if (dx * dx + dy * dy + dz * dz > 1440000.0f) {
                    continue;
                }
            }
            em->ang.y = e->rotY;
            em->r_no_0 = 1;
            em->r_no_1 = 0x17;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return 1;
        case 2:
            if (w->Wep_type != 8) {
                continue;
            }
            {
                f32 dx = e->pos.x - em->pos.x;
                f32 dy = e->pos.y - em->pos.y;
                f32 dz = e->pos.z - em->pos.z;
                if (dx * dx + dy * dy + dz * dz > 2250000.0f) {
                    continue;
                }
            }
            ang = Muku(&e->pos, &pPL->pos, e->rotY, 3.1415927f);
            a = fabsf(ang);
            if (a > 1.0471976f) {
                continue;
            }
            {
                f32 dx = e->pos.x - pPL->pos.x;
                f32 dy = e->pos.y - pPL->pos.y;
                f32 dz = e->pos.z - pPL->pos.z;
                if (dx * dx + dy * dy + dz * dz < 25000000.0f) {
                    continue;
                }
            }
            em->ang.y = e->rotY;
            em->r_no_0 = 1;
            em->r_no_1 = 0x1A;
            em->r_no_2 = 0;
            em->r_no_3 = 0;
            return 1;
        }
    }
    return 0;
}
#endif // PARKED

// From HideSide: steps out of cover (SideStep 0x17, r_no_3 = `a` side) when the player is in front
// of the cover point and the way is clear. 1 when set.
// FUNCTION: BIO4 0x00436FC0
// STATUS: stub
#if 0 // PARKED: C2228, EatMgr, Frame_cnt, Muku, PSMTXMultVec, mat
int em10HideToStepCk(cEm10* em, int a)
{
    Vec v;
    Vec p;
    int hit;

    if ((pG->Frame_cnt & 7) != (em->emset_no & 7)) {
        return 0;
    }
    if (a) {
        v.x = -2000.0f;
        v.y = 2000.0f;
        v.z = 0.0f;
        PSMTXMultVec(em->mat, &v, &v);
    } else {
        v.x = -2000.0f;
        v.y = 2000.0f;
        v.z = 0.0f;
        PSMTXMultVec(em->mat, &v, &v);
    }
    if (fabsf(Muku(&v, &pPL->pos, em->ang.y, 3.1415927f)) > 1.5707964f) {
        return 0;
    }
    {
        f32 dz = v.z - pPL->pos.z;
        f32 dx = v.x - pPL->pos.x;
        if (dx * dx + dz * dz > 169000000.0f) {
            return 0;
        }
    }
    p = pPL->pos;
    p.y += 2000.0f;
    hit = EatMgr.hitCheck(&v, &p, 0, 0, 0, 0);
    if (hit) {
        return 0;
    }
    EmRoutineSet(em, 1, 0x17, hit, a);
    return 1;
}
#endif // PARKED

// "Behind you" tell: a Ganado within 3000 units in front of itself but behind the player's back,
// who sees him, plays Se_tbl[16] and locks the tell for 300 frames (ctrl12 BACKSIGN) for all.
// FUNCTION: BIO4 0x004372D0
// STATUS: stub
#if 0 // PARKED: CTRL12_ID_BACKSIGN, Ctrl12Ck, Ctrl12Set, Muku, l_pl
extern "C" void em10BehindSeCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (Ctrl12Ck(w->pCtrlGroup, CTRL12_ID_BACKSIGN)) {
        return;
    }
    if ((s16) w->Atk_wait != 0) {
        return;
    }
    if (em->l_pl > 9000000.0f) {
        return;
    }
    if (w->Pl_rot > 1.5707964f) {
        return;
    }
    if (w->Wep_type == 4) {
        return;
    }
    if (fabsf(Muku(&pPL->pos, &em->pos, pPL->ang.y, 3.1415927f)) < 1.5707964f) {
        return;
    }
    if (!(w->Be_flg & 1)) {
        return;
    }
    em10CallVoiceSe2(em, w->Se_tbl[16], 8);
    Ctrl12Set(w->pCtrlGroup, CTRL12_ID_BACKSIGN, 300);
    w->Breath_se_wait = (u8) (Rnd() % 120) + 120;
}
#endif // PARKED

// Damage voice: `b` (the death cry) when the Ganado is dead, the parasite screech 0x89 when the
// parasite is out, else `a`.
// FUNCTION: BIO4 0x00437480
// STATUS: stub
#if 0 // PARKED: Ctrl11SetSe2, SndStop
void em10SetDamageVoice(cEm10* em, u16 a, u16 b)
{
    Em10Work* w = EM10_WK(em);

    if (em->hp <= 0) {
        if (!(w->Be_flg & 0x80)) {
            em10CallVoiceSe2(em, b, 8);
        }
        if (em->type != 10 && em->type != 13 && (w->pParasite || w->pCore)) {
            SndStop(w->Seid_voice, 0);
            SndStop(w->Seid_breath, 0);
            w->Seid_voice = Ctrl11SetSe2(w->pCtrlSe, em, Rnd() % 20 + 20, 0x89, 6, 8);
            w->Breath_se_wait = Rnd() % 120 + 120;
        }
    } else if (!(w->Be_flg & 0x80)) {
        em10CallVoiceSe(em, a);
    }
}
#endif // PARKED

// Splash effect at the root part when the Ganado takes a hit / falls in water (`a` 0 small, 1 big),
// at most every Water_eff_wait3 frames.
// FUNCTION: BIO4 0x004375D0
// STATUS: stub
#if 0 // PARKED: EFF_ROOM, SndCall
void em10SetDmWaterEff(cEm10* em, int a)
{
    Em10Work* w = EM10_WK(em);
    cModel* p;
    s8 wait;

    if (CheckInWater(em, 4)) {
        wait = w->Water_eff_wait3;
        if (wait == 0) {
            p = em->getPartsPtr(0);
            switch (a) {
            case 0:
            default:
                EstSet(em, -1, 0, 0, EFF_ROOM, 0x34, 0, (int) wait, em, 0);
                SndCall(6, 0x11, &p->world, 0, 0, em);
                w->Water_eff_wait3 = 5;
                break;
            case 1:
                EstSet(em, -1, 0, 0, EFF_ROOM, 0x33, 0, (int) wait, em, 0);
                SndCall(6, 0x16, &p->world, 0, 0, em);
                w->Water_eff_wait3 = 10;
                break;
            }
        }
    }
}
#endif // PARKED

// While carrying Ashley: passing an EMI type 5 sub 1 waypoint (within 4000, same height) redirects
// Route_target to the exit (sub 0) of the same route state.
// FUNCTION: BIO4 0x00437CA0
// STATUS: stub
extern "C" void em10SetTakeawayPosUpdate(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;
    u32 j;
    f32 d;
    f32 dy;

    if (!pG->pEmi) {
        return;
    }
    for (i = 0; i < (pG->pEmi)->n; i++) {
        EmiEntry* e = &(pG->pEmi)->entry[i];
        if (e->type != 5) {
            continue;
        }
        if (e->sub == 0) {
            continue;
        }
        if (e->sub != 1) {
            continue;
        }
        d = (em->pos.x - e->pos.x) * (em->pos.x - e->pos.x) + (em->pos.z - e->pos.z) * (em->pos.z - e->pos.z);
        dy = fabsf(em->pos.y - e->pos.y);
        if (!(d < 16000000.0f && dy < 1000.0f)) {
            continue;
        }
        for (j = 0; j < (pG->pEmi)->n; j++) {
            EmiEntry* e2 = &(pG->pEmi)->entry[j];
            if (e2->type != 5) {
                continue;
            }
            if (e2->sub != 0) {
                continue;
            }
            if (e2->state != e->state) {
                continue;
            }
            w->Route_target = e2->pos;
            return;
        }
    }
}

// Guard Ganados (Character 1 / 3): reaching an EMI type 0xC "return" point within 3000 units makes
// them stop there (Stay 0x1B, flag 0x20000000 = returned, Return_ck_pos). 1 when set.
// FUNCTION: BIO4 0x0042E750
// STATUS: stub
extern "C" int em10ReturnPosCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    EmiData* emi;
    EmiEntry* e;
    u32 i;

    if (pG->pEmi == 0) {
        return 0;
    }
    if (w->Be_flg & 0x4000) {
        return 0;
    }
    if (em->Character != 1 && em->Character != 3) {
        return 0;
    }
    emi = pG->pEmi;
    for (i = 0; i < emi->n; i++) {
        f32 dx;
        f32 dy;
        f32 dz;
        e = &emi->entry[i];
        if (e->type != 0xC) {
            continue;
        }
        dx = em->pos.x - e->pos.x;
        dy = em->pos.y - e->pos.y;
        dz = em->pos.z - e->pos.z;
        if (dx * dx + dy * dy + dz * dz > 9000000.0f) {
            continue;
        }
        w->Be_flg |= 0x20000000;
        w->Return_ck_pos = em->pos;
        EmRoutineSet(em, 1, 0x1B, 0, 0);
        return 1;
    }
    return 0;
}

// EMI type 0xF ambush groups: a Ganado at a group's start point (sub 0) whose trigger point (sub 2)
// the player is near, once enough Ganados (state) gather, is sent to the group's goal (sub 1) with
// goto mode 0xC (forgetting the player unless flag bit6). 1 when set.
// FUNCTION: BIO4 0x0044B330
// STATUS: stub
#if 0 // PARKED: C2227, C2228, EMI_DATA, EmMgr
int em10GotoPosCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    u32 i;
    u32 j;
    int found;
    u32 cnt;
    EmiEntry* e;
    EmiEntry* f;
    f32 d;

    if (pG->pEmi == 0) {
        return 0;
    }
    if (w->Goto_mode != 0) {
        return 0;
    }
    for (i = 0; i < EMI_DATA->n; i++) {
        e = &EMI_DATA->entry[i];
        if (e->type != 0xF) {
            continue;
        }
        if (e->sub != 0) {
            continue;
        }
        {
            f32 dx = em->pos.x - e->pos.x;
            f32 dy = em->pos.y - e->pos.y;
            f32 dz = em->pos.z - e->pos.z;
            if (dx * dx + dy * dy + dz * dz > 9000000.0f) {
                continue;
            }
        }
        found = 0;
        for (j = 0; j < EMI_DATA->n; j++) {
            f = &EMI_DATA->entry[j];
            if (f->type != 0xF) {
                continue;
            }
            if (f->sub != 2) {
                continue;
            }
            if (f->pad_3 != e->pad_3) {
                continue;
            }
            {
                f32 dx = f->pos.x - pPL->pos.x;
                f32 dy = f->pos.y - pPL->pos.y;
                f32 dz = f->pos.z - pPL->pos.z;
                d = dx * dx + dy * dy + dz * dz;
                if (d < 9000000.0f) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            continue;
        }
        cnt = 0;
        for (j = 0; j < EmMgr.getArrayNum(); j++) {
            cEm* o = EmMgr.fastAt(j);
            if ((o->be_flag & 0x201) != 1) {
                continue;
            }
            if (o->id <= 0xF) {
                continue;
            }
            if (o->id > 0x20) {
                continue;
            }
            if (o->hp <= 0) {
                continue;
            }
            if (o == em) {
                continue;
            }
            {
                f32 dx = o->pos.x - e->pos.x;
                f32 dy = o->pos.y - e->pos.y;
                f32 dz = o->pos.z - e->pos.z;
                d = dx * dx + dy * dy + dz * dz;
                if (d > 25000000.0f) {
                    continue;
                }
            }
            if (o->r_no_0 == 1 && o->r_no_1 == 0x13) {
                continue;
            }
            cnt++;
        }
        if (cnt < e->state) {
            return 0;
        }
        for (j = 0; j < EMI_DATA->n; j++) {
            f = &EMI_DATA->entry[j];
            if (f->type != 0xF) {
                continue;
            }
            if (f->sub != 1) {
                continue;
            }
            if (f->pad_3 != e->pad_3) {
                continue;
            }
            if (!(em->flag & 0x40)) {
                w->Be_flg &= ~0x100;
            }
            em->setGoto(&f->pos, 0xC);
            return 1;
        }
    }
    return 0;
}
#endif // PARKED

// Is the throw / shot line from the Ganado's chest to the player's chest (at most 5000 units) clear of
// scenario walls (attribute mask 0x404000): 1 = clear.
// FUNCTION: BIO4 0x00438620
// STATUS: stub
#if 0 // PARKED: PSVECSubtract
extern "C" int em10ThrowScaCk(cEm10* em)
{
    Vec a;
    Vec b;
    Vec d;

    a.x = em->pos.x;
    a.y = em->pos.y + 1500.0f;
    a.z = em->pos.z;
    b.x = pPL->pos.x;
    b.y = pPL->pos.y + 1500.0f;
    b.z = pPL->pos.z;
    if ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z) > 25000000.0f) {
        PSVECSubtract(&b, &a, &d);
#line 38900 "D:/Bio4/Prog/em10.cpp"
        VECNormalize(&d, &d);
        PSVECScale(&d, &d, 5000.0f);
        PSVECAdd(&a, &d, &b);
    }
    return EatMgr.hitCheck(&a, &b, 0, 0, 0, 0x404000) == 0;
}
#endif // PARKED

// From the walk routines: a chainsaw not yet running -> C_SawStart (0x0E); dynamite not lit and the
// player near enough (or the fixed bombers) -> BombIgnition (0x0F). 1 when set.
// FUNCTION: BIO4 0x00438AD0
// STATUS: stub
int em10IgnitionCk(cEm10* em)
{
    Em10Work* w = EM10_WK(em);

    if (!(w->Be_flg & 0x100)) {
        return 0;
    }
    if (w->pWeapon) {
        if (w->Wep_type == 4 && !(w->Be_flg & 0x80000000)) {
            EmRoutineSet(em, 1, 0xE, 0, 0);
            return 1;
        }
        if (w->pWeapon && w->Wep_type == 9 && w->Fire_timer == 0) {
            if (w->L_pl_route < 15000.0f || em->Character == 2) {
                if (w->Be_flg & 1) {
                    if (em->flag & 0x10000) {
                        EmRoutineSet(em, 1, 0xF, 0, 0);
                        return 1;
                    }
                    if (em->pos.y > pPL->pos.y - 500.0f) {
                        EmRoutineSet(em, 1, 0xF, 0, 0);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// Throws the lit dynamite at the player: hands the weapon object to cEmWep::setBombThrow with a fuse
// of 50..85 frames (longer at low rank), then forgets it (Wep_type 0, Throw_timer).
// FUNCTION: BIO4 0x00438EF0
// STATUS: stub
#if 0 // PARKED: PSMTXMultVecSR, SQRTF, fRand1_1, l_pl, mat, setBombThrow
void em10BombThrow(cEm10* em)
{
    Em10Work* w = EM10_WK(em);
    f32 d;
    int fuse;
    Vec spd;

    if (!w->pWeapon) {
        return;
    }
    if (w->Wep_type != 9) {
        return;
    }
    d = SQRTF(em->l_pl);
    if (d < 4000.0f) {
        d = 4000.0f;
    }
    fuse = (u8) (Rnd() % 10) + 50;
    if (pPL->pos.y < em->pos.y - 1000.0f) {
        if (pPL->pos.y < em->pos.y - 3000.0f) {
            d *= 0.015384615f;
            fuse = (u8) (Rnd() % 15) + 70;
        } else {
            d *= 0.018181818f;
            fuse = (u8) (Rnd() % 15) + 60;
        }
    } else {
        d *= 0.022222223f;
    }
    spd.x = fRand1_1() * 5.0f + 20.0f;
    spd.y = fRand1_1() * 5.0f + 50.0f;
    spd.z = fRand1_1() * 5.0f + d;
    PSMTXMultVecSR(em->mat, &spd, &spd);
    w->pWeapon->setBombThrow(&spd, fuse);
    w->pWeapon = 0;
    w->Fire_timer = 0;
    w->Wep_type = 0;
    w->Throw_timer = 10;
}
#endif // PARKED

// During a fall: when the path pos_old -> pos crosses a water surface (EatMgr effect attribute)
// plays the water entry effect / SE there.
// FUNCTION: BIO4 0x00439F00
// STATUS: stub
#if 0 // PARKED: AtEffInfo, C2227, C2228, EFF_ROOM, EatGetEffectType, EatMgr, EstSetEm10WaterFall, SndCall, info, pos_old, room_id
extern "C" void em10FallWaterCk(cEm10* em)
{
    Vec hit;
    u32 attr;
    AtEffInfo* info;

    attr = EatMgr.hitCheck(&em->pos_old, &em->pos, &hit, 0, 0, 0);
    if (!attr) {
        return;
    }
    info = EatMgr.getEffInfo(EatGetEffectType(attr));
    if (!info) {
        return;
    }
    if (info->flag & 1) {
        if (pG->room_id == 0x311) {
            EstSet(0, -1, &em->pos, 0, EFF_ROOM, 3, 0, ESP_CORE_KIND_NONE, 0, 0);
            SndCall(6, 0xA, &em->pos, 0, 0, em);
        } else {
            EstSetEm10WaterFall((Vec*) em);
            SndCall(6, 0x16, &em->pos, 0, 0, em);
        }
    }
}
#endif // PARKED
