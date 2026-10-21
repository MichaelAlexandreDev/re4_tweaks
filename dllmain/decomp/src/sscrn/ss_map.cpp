// Steam map screen. GameCube guide: references/re4-gamecube/src/Sscrn/ss_map.cpp.
// Verified target region uses x87 (no SSE instructions in 0x00776B90..0x00781100).
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "ss_map.h"
#include "global.h"
#include "snd.h"

// Local subset of the Steam ID system used by the mode-menu close path.
// ECX is &IdSub, unitPtr takes two stack bytes promoted to 32-bit slots,
// and the returned ID_UNIT has rev_flag at +0x8A.
struct MapIdUnit {
    u8 pad_00[0x8A];
    u8 rev_flag;  // 0x8A
};
class MapIdSystem {
public:
    DECOMP_METHOD(MapIdUnit*, MapIdSystem, unitPtr, (u8 mark, u8 group), (mark, group), 0x004095F7);
};
DECOMP_GLOBAL(MapIdSystem, MapIdSub, 0x00C7AB28);

// Zoom-in animation start (10 frames).
// FUNCTION: BIO4 0x0077C4B0
// STATUS: matching
void MapZoomIn::init(SUB_SCREEN* wk)
{
    count = 0;
    SndCall(0, 4, 0, 0, 0, 0);
}

// Zoom-out animation start (10 frames).
// FUNCTION: BIO4 0x0077C4E0
// STATUS: matching
void MapZoomOut::init(SUB_SCREEN* wk)
{
    count = 0;
    SndCall(0, 5, 0, 0, 0, 0);
}

// Map scrolling in MapRead: d-pad/stick (Key bits 24..27) move the camera over the map at a speed
// scaled by the zoom, L/R (bits 22/23) change height between zoomInLimit and zoomOutLimit.
// FUNCTION: BIO4 0x0077DDA0
// STATUS: stub
#if 0 // PARKED: C2227, C2228, CameraSetOrientationUp, Key, PSVECAdd, map_cam_speed, map_cam_speed_base, map_dbg_cam1, memclr_asm, pG, zoomInLimit, zoomOutLimit
static void mapCameraMove(SUB_SCREEN* wk)
{
    Vec d;

    map_cam_speed = map_cam_speed_base * (pG->Camera.Distance / 5000.0f);
    memclr_asm(&d, sizeof(Vec));
    if (Key.on & 0x0C000000) {
        if (Key.on & 0x08000000) {
            d.x = -map_cam_speed;
        }
        if (Key.on & 0x04000000) {
            d.x = map_cam_speed;
        }
    }
    if (Key.on & 0x03000000) {
        if (Key.on & 0x01000000) {
            d.z = -map_cam_speed;
        }
        if (Key.on & 0x02000000) {
            d.z = map_cam_speed;
        }
    }
    if (Key.on & 0x00C00000) {
        if (Key.on & 0x00400000) {
            d.y = map_dbg_cam1[0];
        }
        if (Key.on & 0x00800000) {
            d.y = -map_dbg_cam1[0];
        }
    }
    if (d.x != 0.0f || d.y != 0.0f || d.z != 0.0f) {
        PSVECAdd(&pG->Camera.param.pos, &d, &pG->Camera.param.pos);
        d.y = 0.0f;
        PSVECAdd(&pG->Camera.param.at, &d, &pG->Camera.param.at);
        if (pG->Camera.param.pos.y <= zoomInLimit()) {
            pG->Camera.param.pos.y = zoomInLimit();
        }
        if (pG->Camera.param.pos.y >= zoomOutLimit()) {
            pG->Camera.param.pos.y = zoomOutLimit();
        }
        CameraSetOrientationUp(&pG->Camera);
    }
}
#endif // PARKED

// Whole-stage camera of the stage into `out`; swaps the "zoom in" / "zoom out" button hints.
// FUNCTION: BIO4 0x0077B9C0
// STATUS: stub
#if 0 // PARKED: C2227, C2228, CameraParam, IDC_SSCRN_CKPT_1, IdSub, SUB_SCREEN, map_cam_entire, out
static void mapCameraEntire(SUB_SCREEN* wk, CameraParam* out)
{
    *out = map_cam_entire[(s8) wk->stage_no];
    IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->be_flag |= 8;
    IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->rev_flag &= 0xF0;
    IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
}
#endif // PARKED

// Zoomed camera into `out`: centred between the player and the goal, high enough to frame both
// (4:3), clamped to the zoom limits; swaps the button hints.
// FUNCTION: BIO4 0x0077BAB0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, CameraParam, SUB_SCREEN, markGoalPosition
static void mapCameraZoomIn(SUB_SCREEN* wk, CameraParam* out)
{
    Vec pl;
    Vec goal;
    Vec mid;
    Vec d;
    f32 h;

    pl.x = wk->pl_mat_map[0][3];
    pl.y = wk->pl_mat_map[1][3];
    pl.z = wk->pl_mat_map[2][3];
    markGoalPosition(wk, &goal);
    PSVECAdd(&pl, &goal, &mid);
    PSVECScale(&mid, &mid, 0.5f);
    PSVECSubtract(&pl, &goal, &d);
    d.x = fabsf(d.x);
    d.y = fabsf(d.y);
    d.z = fabsf(d.z);
    if (!(d.z / d.x >= 0.75f)) {
        d.z = d.x * 0.75f;
    }
    h = d.z / tanf(pG->Camera.param.fovy * 0.5f * 3.1415927f / 180.0f);
    if (h <= zoomInLimit()) {
        h = zoomInLimit();
    }
    if (h >= zoomOutLimit()) {
        h = zoomOutLimit();
    }
    out->pos = mid;
    out->at = mid;
    out->pos.y += h;
    IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
    IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->rev_flag &= 0xF0;
    IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->be_flag |= 8;
}
#endif // PARKED

// Camera interpolation from `from` to `to` over `max` frames; 1 when done.
// FUNCTION: BIO4 0x0077BE50
// STATUS: stub
#if 0 // PARKED: C2143, SsMapWork, int, m
int zoomMove(SsMapWork* m, int max, int cnt)
{
    f32 t = (f32) cnt / (f32) max;
    Vec up = {0.0f, 0.0f, -1.0f};
    Vec a;
    Vec b;
    f32 s;

    CameraParam* from = &m->from;
    CameraParam* to = &m->to;

    PSVECScale(&to->pos, &a, t);
    s = 1.0f - t;
    PSVECScale(&from->pos, &b, s);
    PSVECAdd(&a, &b, &pG->Camera.param.pos);
    PSVECScale(&to->at, &a, t);
    PSVECScale(&from->at, &b, s);
    PSVECAdd(&a, &b, &pG->Camera.param.at);
    // pG loads pG separately from the earlier pG loads, so pG is reloaded for the call after the copy
    pG->Camera.Up = up;
    CameraSetOrientationUp(&pG->Camera);
    return t >= 1.0f;
}
#endif // PARKED

// Area data file name: SS/cmn/map_obj<1|2a..2c|3a..3f|4a..4g>.dat for map area 0..17.
// FUNCTION: BIO4 0x0077BFC0
// STATUS: stub
#if 0 // PARKED: sprintf
static void mapAreaFilename(int area, char* name)
{
    switch (area) {
    case 0:
        sprintf(name, "SS/cmn/map_obj1.dat");
        break;
    case 1:
        sprintf(name, "SS/cmn/map_obj1.dat");
        break;
    case 2:
        sprintf(name, "SS/cmn/map_obj2a.dat");
        break;
    case 3:
        sprintf(name, "SS/cmn/map_obj2b.dat");
        break;
    case 4:
        sprintf(name, "SS/cmn/map_obj2c.dat");
        break;
    case 5:
        sprintf(name, "SS/cmn/map_obj3a.dat");
        break;
    case 6:
        sprintf(name, "SS/cmn/map_obj3b.dat");
        break;
    case 7:
        sprintf(name, "SS/cmn/map_obj3c.dat");
        break;
    case 8:
        sprintf(name, "SS/cmn/map_obj3d.dat");
        break;
    case 9:
        sprintf(name, "SS/cmn/map_obj3e.dat");
        break;
    case 10:
        sprintf(name, "SS/cmn/map_obj3f.dat");
        break;
    case 11:
        sprintf(name, "SS/cmn/map_obj4a.dat");
        break;
    case 12:
        sprintf(name, "SS/cmn/map_obj4b.dat");
        break;
    case 13:
        sprintf(name, "SS/cmn/map_obj4c.dat");
        break;
    case 14:
        sprintf(name, "SS/cmn/map_obj4d.dat");
        break;
    case 15:
        sprintf(name, "SS/cmn/map_obj4e.dat");
        break;
    case 16:
        sprintf(name, "SS/cmn/map_obj4f.dat");
        break;
    case 17:
        sprintf(name, "SS/cmn/map_obj4g.dat");
        break;
    }
}
#endif // PARKED

// Map screen loader: skips the previous screen's exit (state 2) when opened directly as
// SS_OPEN_MAP (type 2, the in-game map key).
// FUNCTION: BIO4 0x0077C190
// STATUS: matching
void SsMapInit::init(SUB_SCREEN* wk)
{
    state = wk->open_flag == 2 ? 2 : 0;
    if (pG->flags_SYSTEM[0] & 0xC0001000) {
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pG) + 0x52D4) |= 0x40;
    }
}

// Loads the map screen: state 0 run the previous screen's scrn_out_func and drop its ids, 1 one
// frame wait, 2 hide the HUD, read SS/<lang>/ss_map.dat into the puzzle slot and switch to the map
// model managers (no character model), 3 wait (archive -> pMapCmn, the area archive offset ->
// pMapArea), 4 fade in for SS_OPEN_MAP and transit to SsMapMain.
// FUNCTION: BIO4 0x0077EE90
// STATUS: stub
#if 0 // PARKED: C2050, C2059, C2227, C2228, DVD_READ_N, Dvd, IDC_LIFE_METER, IDC_SSCRN_NEAR_1, IdAllocBuffer, IdFreeBuffer, IdNumErase, IdSub, IdSubErase, IdSys, SS_ARC_PTR, SUB_SCREEN, SsMapInit, mapModelAlloc, map_read_req, map_wait, ssPlModel, ssPlMotion, ssWepModel, ssWepModel2, sscrnDataFilename, sscrnLightClear, sscrnModelFree, state
void SsMapInit::move(SUB_SCREEN* wk)
{
    switch (state) {
    case 0:
        if (wk->scrn_out_func(wk) == 1) {
            if (wk->menu_old == 2) {
                wk->wait_cnt = 1;
            }
            IdSubErase();
            IdNumErase();
            IdFreeBuffer();
            IdSub.set(SS_ARC_PTR(wk->pCmmn, 0xC), 0xFF, IDC_SSCRN_NEAR_1, 0xF, 1, 0);
            map_wait[0] = 0;
            state++;
        }
        break;
    case 1:
        if (--map_wait[0] >= 0) {
            break;
        }
        state++;
        break;
    case 2:
        IdSys.dispSw(IDC_LIFE_METER, 0);
        sscrnDataFilename(wk, "ss_map.dat");
        if (wk->open_flag == 2) {
#line 3250 "D:/Bio4/Prog/ss_map.cpp"
            map_read_req = DVD_READ_N(wk->filename, wk->pSwitchDat, 0, 0, 0, 0x11);
        } else {
#line 3253 "D:/Bio4/Prog/ss_map.cpp"
            map_read_req = DVD_READ_N(wk->filename, wk->pSwitchDat, 0, 0, 0, 0x10);
        }
        if (map_read_req <= 0) {
            break;
        }
        sscrnModelFree(wk);
        mapModelAlloc(wk);
        sscrnLightClear(wk);
        ssPlModel = 0;
        ssWepModel = 0;
        ssPlMotion = 0;
        ssWepModel2 = 0;
        IdAllocBuffer();
        wk->wait_cnt = 0;
        state++;
    case 3: {
        int result;
        int size;

        if (Dvd.ReadCheck(map_read_req, &result, &size, 0) != 1) {
            break;
        }
        wk->pMapDat = wk->pSwitchDat;
        wk->pMapObj = (SsArc*) ((u8*) wk->pSwitchDat + result);
        state++;
    }
    case 4:
        if (wk->open_flag == 2) {
            FadeSetW(0x80000000, 5, 0, 0);
        }
        transit(0, wk);
        break;
    }
}
#endif // PARKED

// Builds the map screen: the zoom/mode widgets (MapFocus -> MapZoomIn <-> MapRead/MapZoomOut ->
// MapEntire, MapModeSelect from Entire/Read), id textures and unit groups (0x19 room colours,
// IdNum 0x14..0x16 typewriter/treasure/medallion marks, 0x14 character marks, 0x10 frame, 0x1D
// button hints; hints for marks not yet available hidden), lights, the SsMapWork, viewport and
// camera; area from the room number; state 2 / step 0 = load the area data.
// FUNCTION: BIO4 0x0077E0B0
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, IDC_SSCRN_0, IDC_SSCRN_1, IDC_SSCRN_2, IDC_SSCRN_FAR_1, IDC_SSCRN_NEAR_0, IDC_SSCRN_NEAR_1, IdNum, IdSub, IdTexDataLoad, MapEntire, MapFocus, MapModeSelect, MapRead, SS_ARC_PTR, SUB_SCREEN, SsMapMain, TEX_OWNER_ID_SSCRN, cur, entire, focus, modeSel, read, zoomIn, zoomOut
void SsMapMain::init(SUB_SCREEN* wk)
{
    focus = new MapFocus;
    entire = new MapEntire;
    zoomIn = new MapZoomIn;
    zoomOut = new MapZoomOut;
    read = new MapRead;
    modeSel = new MapModeSelect;
    focus->connect(0, zoomIn);
    entire->connect(0, zoomIn);
    entire->connect(1, modeSel);
    read->connect(0, zoomIn);
    read->connect(1, zoomOut);
    read->connect(2, modeSel);
    zoomIn->connect(0, read);
    zoomIn->connect(1, zoomOut);
    zoomOut->connect(0, entire);
    zoomOut->connect(1, zoomIn);
    modeSel->connect(0, entire);
    modeSel->connect(1, read);
    cur = focus;
    IdTexDataLoad(SS_ARC_PTR(wk->pMapDat, 4), TEX_OWNER_ID_SSCRN);
    if (!IdSub.setCk(IDC_SSCRN_NEAR_1)) {
        IdSub.set(SS_ARC_PTR(wk->pCmmn, 0xC), 0xFF, IDC_SSCRN_NEAR_1, 0xF, 1, 0);
    }
    IdSub.set(SS_ARC_PTR(wk->pMapDat, 5), 0xFF, IDC_SSCRN_FAR_1, 9, 2, 0);
    IdNum.set(SS_ARC_PTR(wk->pMapDat, 0xA), 0xFF, IDC_SSCRN_2, 0xC, 6, 0);
    IdNum.set(SS_ARC_PTR(wk->pMapDat, 9), 0xFF, IDC_SSCRN_1, 0xC, 6, 0);
    IdNum.set(SS_ARC_PTR(wk->pMapDat, 8), 0xFF, IDC_SSCRN_0, 0xC, 6, 0);
    IdSub.set(SS_ARC_PTR(wk->pMapDat, 6), 0xFF, IDC_SSCRN_0, 0xC, 5, 0);
    IdSub.set(SS_ARC_PTR(wk->pMapDat, 7), 0xFF, IDC_SSCRN_NEAR_0, 0xF, 2, 0);
    IdSub.set(SS_ARC_PTR(wk->pMapDat, 0xB), 0xFF, IDC_SSCRN_CKPT_1, 0x13, 8, 0);
    IdSub.unitPtr(0x10, IDC_SSCRN_NEAR_0)->rev_flag |= 0xF;
    IdSub.unitPtr(0x10, IDC_SSCRN_NEAR_0)->be_flag &= ~8;
    IdSub.unitPtr(0, IDC_SSCRN_NEAR_0)->be_flag &= ~8;
    IdSub.unitPtr(0, IDC_SSCRN_NEAR_0)->rev_flag |= 0xF;
    sscrnLightCreate(wk, (cLit*) SS_ARC_PTR(wk->pCmmn, 0x13));
#line 3409 "D:/Bio4/Prog/ss_map.cpp"
    wk->map = (SsMapWork*) MEM_ALLOC(sizeof(SsMapWork), 1, 0xD);
    mapInitViewport(wk);
    mapCameraInit(wk, &pG->Camera);
    IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    IdSub.unitPtr(2, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    if (!ScfFlagChk(pG, SCF_R104_MEET_MERCHANT)) {
        IdSub.unitPtr(0x12, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    }
    if (!ItemMgr.search(0xA9)) {
        IdSub.unitPtr(0x14, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    }
    if (ItemMgr.num(0xB0) == 0 && !ScfFlagChk(pG, SCF_CONTACT_MERCHANT)) {
        IdSub.unitPtr(0x13, IDC_SSCRN_CKPT_1)->be_flag &= ~8;
    }
    sscrnMainMenuInit(wk, 0);
    markGoalDisp(wk, 0);
    markPlayerDisp(wk, 0);
    markMerchantDisp(wk, 0);
    markTreasureDisp(wk, 0);
    markCoinDisp(wk, 0);
    markSaveDisp(wk, 0);
    wk->map->area = getAreaNo(wk->room_no);
    state = 2;
    step = 0;
    SndCall(0, 0x1E, 0, 0, 0, 0);
}
#endif // PARKED

// Map screen frame. state 0 runs the zoom widget chain (in MapRead: Y / B-or-Z on the in-game map
// exit the sub screen via link 4, B opens the main menu tab row), 1 the tab row (0 items, 1 case,
// 2 back, 3 files, 4 exit), 2 waits for the area load. step 0 reads the area file into pMapArea,
// 1 builds the room/door/mark models, 2 draws them every frame with the marks enabled by the mode
// bits (map_mode) and the scf_check_* availability.
// FUNCTION: BIO4 0x00780AA0
// STATUS: stub
#if 0 // PARKED: C2050, C2227, C2228, DVD_READ_N, Dvd, Key, SUB_SCREEN, SndCall, SsMapMain, Widget, cur, doorModelDisp, doorModelInit, mapAreaFilename, mapChangeViewport, mapModeCheck, mapModelDisp, mapModelInit, markCoinDisp, markCoinInit, markGoalDisp, markGoalInit, markMerchantDisp, markMerchantInit, markPlayerDisp, markSaveDisp, markSaveInit, markTreasureDisp, markTreasureInit, read, readReq, scf_check_merchant, scf_check_submission, scf_check_treasure, scf_check_typewriter, sscrnMainMenu, sscrnMainMenuInit, stageNameDisp, state, step, transit, w
void SsMapMain::move(SUB_SCREEN* wk)
{
    switch (state) {
    case 0: {
        Widget<SUB_SCREEN>* w = cur;

        w->move(wk);
        cur = w->cur;
        if (cur == read) {
            if (wk->open_flag == 2) {
                if (Key.trg & 0x40200000) {
                    wk->close_flag |= 4;
                    transit(4, wk);
                }
            } else {
                if (Key.trg & 0x00100000) {
                    wk->close_flag |= 4;
                    transit(4, wk);
                } else if (Key.trg & 0x40000000) {
                    state = 1;
                    wk->close_flag |= 4;
                    sscrnMainMenuInit(wk, 1);
                    SndCall(0, 0xA, 0, 0, 0, 0);
                }
            }
        }
        break;
    }
    case 1:
        if (sscrnMainMenu(wk)) {
            switch ((s8) wk->menu_no) {
            case 1:
                transit(0, wk);
                break;
            case 0:
                transit(1, wk);
                break;
            case 3:
                transit(3, wk);
                break;
            case 2:
                sscrnMainMenuInit(wk, 0);
                state = 0;
                break;
            case 4:
                transit(4, wk);
                break;
            }
        }
        break;
    case 2:
        if (step > 1) {
            state = 0;
        }
        break;
    }
    switch (step) {
    case 0: {
        char name[64];

        stageNameDisp(wk);
        mapAreaFilename(wk->map->area, name);
#line 3637 "D:/Bio4/Prog/ss_map.cpp"
        readReq = DVD_READ_N(name, wk->pMapObj, 0, 0, 0, 0x10);
        if (readReq <= 0) {
            break;
        }
        step++;
    }
    case 1:
        if (Dvd.ReadCheck(readReq, 0, 0, 0) != 1) {
            break;
        }
        mapModelInit(wk);
        doorModelInit(wk);
        markGoalInit(wk);
        markMerchantInit(wk);
        markTreasureInit(wk);
        markCoinInit(wk);
        markSaveInit(wk);
        step++;
        SndCall(0, 0x1B, 0, 0, 0, 0);
    case 2:
        mapChangeViewport(wk);
        mapModelDisp(wk);
        doorModelDisp(wk);
        markGoalDisp(wk, 1);
        markPlayerDisp(wk, 1);
        if (!mapModeCheck(wk, 0)) {
            if (scf_check_typewriter()) {
                markSaveDisp(wk, 1);
            } else {
                markSaveDisp(wk, 0);
            }
        } else {
            markSaveDisp(wk, 0);
        }
        if (!mapModeCheck(wk, 1)) {
            if (scf_check_merchant()) {
                markMerchantDisp(wk, 1);
            } else {
                markMerchantDisp(wk, 0);
            }
        } else {
            markMerchantDisp(wk, 0);
        }
        if (!mapModeCheck(wk, 2)) {
            if (scf_check_treasure()) {
                markTreasureDisp(wk, 1);
            } else {
                markTreasureDisp(wk, 0);
            }
        } else {
            markTreasureDisp(wk, 0);
        }
        if (!mapModeCheck(wk, 3)) {
            if (scf_check_submission()) {
                markCoinDisp(wk, 1);
            } else {
                markCoinDisp(wk, 0);
            }
        } else {
            markCoinDisp(wk, 0);
        }
        break;
    }
}
#endif // PARKED

// Leaving the map: deletes the widgets and mark models, frees the map work, installs sscrn_map_out.
// FUNCTION: BIO4 0x0077F210
// STATUS: stub
#if 0 // PARKED: C1003, SsMapMain, entire, focus, ssWidgetDelete, zoomIn
void SsMapMain::quit(SUB_SCREEN* wk)
{
    ssWidgetDelete(focus);
    ssWidgetDelete(entire);
    ssWidgetDelete(zoomIn);
    ssWidgetDelete(zoomOut);
    ssWidgetDelete(read);
    ssWidgetDelete(modeSel);
    markGoalQuit(wk);
    markMerchantQuit(wk);
    markTreasureQuit(wk);
    markCoinQuit(wk);
    markSaveQuit(wk);
    Mem_free(wk->map);
    wk->close_flag |= 4;
    sscrn_map_out_init(wk);
    wk->scrn_out_func = sscrn_map_out;
}
#endif // PARKED

// First widget: starts the zoom from the current camera to the player/goal view (MapZoomIn).
// FUNCTION: BIO4 0x0077F3A0
// STATUS: stub
#if 0 // PARKED: C2227, C2228, MapFocus, SUB_SCREEN, SsMapWork, m, mapCameraZoomIn, pG, transit
void MapFocus::move(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;

    m->from = pG->Camera.param;
    mapCameraZoomIn(wk, &wk->map->to);
    transit(0, wk);
}
#endif // PARKED

// Whole-stage view: A/B zoom in (link 0), X (Key bit 17) opens the mark mode menu (link 1, returns
// here).
// FUNCTION: BIO4 0x0077F430
// STATUS: stub
#if 0 // PARKED: C2084, C2227, C2228, Key, MapEntire, SUB_SCREEN, SsMapWork, m, mapCameraZoomIn, pG, transit
void MapEntire::move(SUB_SCREEN* wk)
{
    if (Key.trg & 0xC0000000) {
        SsMapWork* m = wk->map;

        m->from = pG->Camera.param;
        mapCameraZoomIn(wk, &wk->map->to);
        transit(0, wk);
    } else if (Key.trg & 0x00020000) {
        wk->map->modeSel = 0;
        transit(1, wk);
    }
}
#endif // PARKED

// Interpolates the camera to the zoomed view; A reverses into MapZoomOut, done -> MapRead.
// FUNCTION: BIO4 0x0077F520
// STATUS: stub
#if 0 // PARKED: C2227, C2228, Key, SUB_SCREEN, SsMapWork, count, m, mapCameraEntire, move, pG, transit, zoomMove
void MapZoomIn::move(SUB_SCREEN* wk)
{
    if (Key.trg & 0x80000000) {
        SsMapWork* m = wk->map;

        m->from = pG->Camera.param;
        mapCameraEntire(wk, &wk->map->to);
        transit(1, wk);
    } else {
        if (zoomMove(wk->map, 10, count++)) {
            transit(0, wk);
        }
    }
}
#endif // PARKED

// Interpolates the camera to the whole-stage view; B reverses into MapZoomIn, done -> MapEntire.
// FUNCTION: BIO4 0x0077F610
// STATUS: stub
#if 0 // PARKED: C2227, C2228, Key, SUB_SCREEN, SsMapWork, count, m, mapCameraZoomIn, move, pG, transit, zoomMove
void MapZoomOut::move(SUB_SCREEN* wk)
{
    if (Key.trg & 0x40000000) {
        SsMapWork* m = wk->map;

        m->from = pG->Camera.param;
        mapCameraZoomIn(wk, &wk->map->to);
        transit(1, wk);
    } else {
        if (zoomMove(wk->map, 10, count++)) {
            transit(0, wk);
        }
    }
}
#endif // PARKED

// Zoomed map reading: A zooms out (link 1), X opens the mark mode menu (link 2, returns here),
// otherwise the d-pad scrolls the camera (mapCameraMove).
// FUNCTION: BIO4 0x0077F710
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, Key, MapRead, SUB_SCREEN, SsMapWork, m, mapCameraEntire, pG, transit
void MapRead::move(SUB_SCREEN* wk)
{
    if (Key.trg & 0x80000000) {
        SsMapWork* m = wk->map;

        m->from = pG->Camera.param;
        mapCameraEntire(wk, &wk->map->to);
        transit(1, wk);
    } else if (Key.trg & 0x00020000) {
        wk->map->modeSel = 1;
        transit(2, wk);
    } else {
        mapCameraMove(wk);
    }
}
#endif // PARKED

// Mark mode menu open: shows the menu panel (IdSub 0x10/0x10), cursor on row 0 (typewriter), hides
// the rows whose marks are not yet available (merchant before Scenario_flg bit 29, treasure
// without item 0xA9, medallions without 0xB0 / the mission), swaps the button hints.
// FUNCTION: BIO4 0x0077C510
// STATUS: stub
#if 0 // PARKED: C2227, C2228, IDC_SSCRN_CKPT_1, IDC_SSCRN_NEAR_0, IdSub, IdUnit, ItemMgr, MapModeSelect, SCF_CONTACT_MERCHANT, SCF_R104_MEET_MERCHANT, SUB_SCREEN, ScfFlagChk, SndCall, SsMapWork, m, pG, u, u2
void MapModeSelect::init(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;
    IdUnit* u;
    IdUnit* u2;

    IdSub.unitPtr(0x10, IDC_SSCRN_NEAR_0)->be_flag |= 8;
    IdSub.unitPtr(0x10, IDC_SSCRN_NEAR_0)->rev_flag &= 0xF0;
    m->modeCursor = 0;
    u = IdSub.unitPtr(0x20, IDC_SSCRN_NEAR_0);
    u2 = IdSub.unitPtr(0x60, IDC_SSCRN_NEAR_0);
    u->pos0 = u2->pos0;
    u->timer[3] = 0;
    u->timer[2] = 0;
    u->timer[1] = 0;
    u->timer[0] = 0;
    if (ScfFlagChk(pG, SCF_R104_MEET_MERCHANT)) {
        u = IdSub.unitPtr(0x61, IDC_SSCRN_NEAR_0);
        u->be_flag &= ~8;
    }
    if (ItemMgr.num(0xA9)) {
        u = IdSub.unitPtr(0x62, IDC_SSCRN_NEAR_0);
        u->be_flag &= ~8;
    }
    if (ItemMgr.num(0xB0) || (ScfFlagChk(pG, SCF_CONTACT_MERCHANT))) {
        u = IdSub.unitPtr(0x63, IDC_SSCRN_NEAR_0);
        u->be_flag &= ~8;
    }
    IdSub.unitPtr(2, IDC_SSCRN_CKPT_1)->be_flag |= 8;
    IdSub.unitPtr(2, IDC_SSCRN_CKPT_1)->rev_flag &= 0xF0;
    IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
    IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
    SndCall(0, 9, 0, 0, 0, 0);
}
#endif // PARKED

// Mark mode menu: B/X close it back to Entire (modeSel 0) or Read (1); A toggles the mark kind
// under the cursor when available; up/down move the cursor (rows 0x60.. positions, check marks
// 0x50.. shown for enabled kinds).
// FUNCTION: BIO4 0x0077F810
// STATUS: stub
#if 0 // PARKED: C1003, C2227, C2228, IDC_SSCRN_CKPT_1, IdSub, IdUnit, Key, MapModeSelect, SUB_SCREEN, SsMapWork, m, u
void MapModeSelect::move(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;
    IdUnit* u;

    if (Key.trg & 0x40020000) {
        switch (m->modeSel) {
        case 0:
            IdSub.unitPtr(2, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
            IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->be_flag |= 8;
            IdSub.unitPtr(1, IDC_SSCRN_CKPT_1)->rev_flag &= 0xF0;
            transit(0, wk);
            return;
        case 1:
            IdSub.unitPtr(2, IDC_SSCRN_CKPT_1)->rev_flag |= 0xF;
            IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->be_flag |= 8;
            IdSub.unitPtr(0, IDC_SSCRN_CKPT_1)->rev_flag &= 0xF0;
            transit(1, wk);
            return;
        }
    }
    // not `else if`: the switch's fall-out reaches this test too, so its label has two uses and cse
    // reloads Key.trg here
    if (Key.trg & 0x80000000) {
        switch (m->modeCursor) {
        case 1:
            if (!ScfFlagChk(pG, SCF_R104_MEET_MERCHANT)) {
                return;
            }
            break;
        case 3:
            if (ItemMgr.num(0xB0) == 0 && !ScfFlagChk(pG, SCF_CONTACT_MERCHANT)) {
                return;
            }
            break;
        case 2:
            if (ItemMgr.search(0xA9) == 0) {
                return;
            }
            break;
        }
        mapModeChange(wk, m->modeCursor);
        SndCall(0, 0x1D, 0, 0, 0, 0);
    } else {
        int i;
        int old;

        for (i = 0; i < 4; i++) {
            u = IdSub.unitPtr(0x50 + i, IDC_SSCRN_NEAR_0);
            if (mapModeCheck(wk, i)) {
                u->be_flag &= ~8;
            } else {
                u->be_flag |= 8;
            }
        }
        old = m->modeCursor;
        if (Key.rep & 0x01000000) {
            m->modeCursor--;
        }
        if (Key.rep & 0x02000000) {
            m->modeCursor++;
        }
        m->modeCursor = m->modeCursor < 0 ? 3 : (m->modeCursor > 3 ? 0 : m->modeCursor);
        if (old != m->modeCursor) {
            IdUnit* u2;

            u = IdSub.unitPtr(0x20, IDC_SSCRN_NEAR_0);
            u2 = IdSub.unitPtr(0x60 + m->modeCursor, IDC_SSCRN_NEAR_0);
            u->pos0 = u2->pos0;
            u->timer[3] = 0;
            u->timer[2] = 0;
            u->timer[1] = 0;
            u->timer[0] = 0;
            SndCall(0, 6, 0, 0, 0, 0);
        }
    }
}
#endif // PARKED

// Hides the mode menu panel.
// FUNCTION: BIO4 0x0077C920
// STATUS: matching
void MapModeSelect::quit(SUB_SCREEN* wk)
{
    MapIdSub.unitPtr(0x10, 0x10)->rev_flag |= 0xF;
    SndCall(0, 5, 0, 0, 0, 0);
}
