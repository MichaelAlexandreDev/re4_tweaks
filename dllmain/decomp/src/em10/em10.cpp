// Ganado enemy (cEm10), Steam bio4.exe. Guide: references/re4-gamecube/src/em10/em10.cpp.
// The original unit uses x87 floating point: no /arch:SSE2.
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "em10.h"
#include "global.h"

DECOMP_FUNC(void, em10SetWaitMotion, (cEm10* em, int a), 0x00432250);

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
// STATUS: functional
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
        Obj12Regist_6700E0(w->pCart);
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
