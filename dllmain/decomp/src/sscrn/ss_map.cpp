// Steam map screen. GameCube guide: references/re4-gamecube/src/Sscrn/ss_map.cpp.
// Verified target region uses x87 (no SSE instructions in 0x00776B90..0x00781100).
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "ss_map.h"
#include "global.h"
#include "snd.h"
#include "mem.h"
#include "dvd.h"

// Local subset of the Steam ID system used by the mode-menu close path.
// ECX is &IdSub, unitPtr takes two stack bytes promoted to 32-bit slots,
// and the returned ID_UNIT has rev_flag at +0x8A.
struct MapIdUnit {
    u8 be_flag;
    u8 pad_01[0x7E - 1];
    u16 timer[4]; // 0x7E
    u8 pad_86[4];
    u8 rev_flag; // 0x8A
    u8 pad_8B[0x91-0x8B];
    u8 anim_flags;
    u8 pad_92[2];
    Vec pos0; // 0x94
};
class MapIdSystem {
public:
    DECOMP_METHOD(void, MapIdSystem, set, (void* data, u8 a, u8 group, u8 b, u8 c, u8 d), (data, a, group, b, c, d), 0x00404449);
    DECOMP_METHOD(u8, MapIdSystem, setCk, (u8 group), (group), 0x0040A61E);
    DECOMP_METHOD(void, MapIdSystem, dispSw, (int group, int on), (group, on), 0x004088F5);
    DECOMP_METHOD(MapIdUnit*, MapIdSystem, unitPtr, (u8 mark, u8 group), (mark, group), 0x004095F7);
    DECOMP_METHOD(void, MapIdSystem, setTime, (MapIdUnit* u, s16 time), (u, time), 0x004022A7);
};
DECOMP_GLOBAL(MapIdSystem, MapIdSub, 0x00C7AB28);

// The target calls the ILT thunks; do not reuse the CRT/vector bodies' VAs.
DECOMP_GLOBAL(MapGlobal*, MapGlobals, 0x00C06F3C);
DECOMP_FUNC(void, MapVecScale, (const Vec* src, Vec* dst, f32 scale), 0x004097A0);
DECOMP_FUNC(void, MapVecAdd, (const Vec* a, const Vec* b, Vec* out), 0x00407F63);
DECOMP_FUNC(void, MapCameraOrientationUp, (MapCamera* cam), 0x004050AB);
DECOMP_GLOBAL(SsMapWork*, MapCurrentWork, 0x00C7AFDC);
DECOMP_GLOBAL(unsigned __int64, MapKeyTrg, 0x00C62F10);
DECOMP_GLOBAL_ARRAY(MapCameraParam, mapCameraTable, 26, 0x00B28948);
struct MapMarkAvailability { u8 column[5]; };
DECOMP_GLOBAL_ARRAY(MapMarkAvailability, mapMarkTable, 26, 0x00B28C88);

DECOMP_GLOBAL(u8, MapMenuKey, 0x00C63020);
DECOMP_FUNC(void, MapVecSubtract, (const Vec* a, const Vec* b, Vec* out), 0x0040E3F9);
DECOMP_GLOBAL(f32, MapZoomMinWidth, 0x00C1CF44);
DECOMP_GLOBAL(f32, MapZoomMinScale, 0x00C1CF48);
DECOMP_GLOBAL(f32, MapZoomHeightScale, 0x00C1CF4C);
DECOMP_GLOBAL(f32, MapRangeX, 0x00C1CF50);
DECOMP_GLOBAL(f32, MapRangeZ, 0x00C1CF58);
DECOMP_GLOBAL(f32, MapSpecialRangeX, 0x00C1CF60);
DECOMP_GLOBAL(f32, MapSpecialRangeZ, 0x00C1CF68);
DECOMP_FUNC(int, MapGoalPosition, (), 0x00776B90);
extern "C" double __cdecl tan(double x);
#pragma intrinsic(tan)
static inline f32 MapTan(f32 x) { return (f32) tan(x); }
f32 zoomOutLimit();

// FUNCTION: BIO4 0x0077BA40
// STATUS: matching
f32 zoomInLimit()
{
    return (f32)(MapZoomMinWidth * 0.5 / MapTan((f32)(MapGlobals->Camera.param.fovy * 0.5 * (double)3.1415927f / 180.0)));
}

class MapItemSystem {
public:
    DECOMP_METHOD(u16, MapItemSystem, num, (u16 id), (id), 0x0040EEE9);
    DECOMP_METHOD(u16, MapItemSystem, numType, (u16 id, u8 type), (id, type), 0x00404C4B);
    DECOMP_METHOD(void*, MapItemSystem, searchType, (u16 id, u8 type), (id, type), 0x0040B140);
    DECOMP_METHOD(void*, MapItemSystem, searchCurrent, (u16 id), (id), 0x0040E791);
};
DECOMP_GLOBAL(MapItemSystem, MapItems, 0x00C70FD4);
DECOMP_GLOBAL(u32, MapKeyRep, 0x00C62F20);
DECOMP_GLOBAL(u32, MapViewEnabled, 0x010ED6DC);
DECOMP_FUNC(u8, MapExitOriginal, (SUB_SCREEN* wk), 0x0077E7B0);

// FUNCTION: BIO4 0x0077C460
// STATUS: matching
void sscrn_map_out_init(SUB_SCREEN* wk)
{
    MapIdUnit* u = MapIdSub.unitPtr(0, 0x10);
    u->be_flag |= 8;
    u->rev_flag &= 0xF0;
    MapIdSub.setTime(u, 0);
    MapIdSub.unitPtr(3, 0x1D)->rev_flag |= 0xF;
}

DECOMP_GLOBAL(unsigned __int64, MapKeyOn, 0x00C62F08);
DECOMP_GLOBAL(s8, MapWheel, 0x00C63011);
DECOMP_GLOBAL(f32, MapHorizontalSpeed, 0x00C1D034);
DECOMP_GLOBAL(f32, MapVerticalSpeed, 0x00C1D030);
DECOMP_GLOBAL(f32, MapWheelScale, 0x00C1D02C);
DECOMP_FUNC(f32, MapVecDistance, (const Vec* a, const Vec* b), 0x0040574F);
struct MapCameraBounds { s8 minX, minZ, maxX, maxZ; };
DECOMP_GLOBAL_ARRAY(MapCameraBounds, MapCameraBoundsTable, 26, 0x00C1CEC8);
DECOMP_GLOBAL(MapCameraBounds*, MapCameraBoundsCurrent, 0x00C7BA3C);

// Steam-only clamp. Table stays in the private original image.
// FUNCTION: BIO4 0x0077B8B0
// STATUS: matching
static void mapCameraClamp(int area, Vec* p)
{
    MapCameraBounds* bounds = &MapCameraBoundsTable[area];
    f32 minX = (f32)(bounds->minX * 2000.0);
    f32 maxX = (f32)(bounds->maxX * 2000.0);
    f32 minZ = (f32)(bounds->minZ * 2000.0);
    MapCameraBoundsCurrent = bounds;
    f32 maxZ = (f32)(bounds->maxZ * 2000.0);
    p->x = p->x < minX ? minX : (p->x > maxX ? maxX : p->x);
    p->z = p->z < minZ ? minZ : (p->z > maxZ ? maxZ : p->z);
}

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
// STATUS: matching
static void mapCameraMove(SUB_SCREEN* wk)
{
    f32 distance = MapVecDistance(&MapGlobals->Camera.param.pos, &MapGlobals->Camera.param.at);
    f32 horizontal = (f32)(MapHorizontalSpeed * (distance / 5000.0));
    f32 vertical = (f32)((distance / 5000.0) * MapVerticalSpeed);
    Vec d;
    memclr_asm(&d, sizeof(d));
    if (MapKeyOn & 0x0C000000ULL) {
        if (MapKeyOn & 0x08000000ULL) d.x = -horizontal;
        if (MapKeyOn & 0x04000000ULL) d.x = horizontal;
    }
    if (MapKeyOn & 0x03000000ULL) {
        if (MapKeyOn & 0x01000000ULL) d.z = -horizontal;
        if (MapKeyOn & 0x02000000ULL) d.z = horizontal;
    }
    if (MapKeyOn & 0x00C00000ULL) {
        if (MapKeyOn & 0x00400000ULL) d.y = vertical;
        if (MapKeyOn & 0x00800000ULL) d.y = -vertical;
    } else {
        double wheel = MapWheel;
        if (wheel != 0.0f) d.y = MapWheelScale * vertical * wheel;
    }
    if (d.x != 0.0f || d.y != 0.0f || d.z != 0.0f) {
        MapVecAdd(&MapGlobals->Camera.param.pos, &d, &MapGlobals->Camera.param.pos);
        d.y = 0.0f;
        MapVecAdd(&MapGlobals->Camera.param.at, &d, &MapGlobals->Camera.param.at);
        if (MapGlobals->Camera.param.pos.y <= zoomInLimit()) MapGlobals->Camera.param.pos.y = zoomInLimit();
        if (MapGlobals->Camera.param.pos.y >= zoomOutLimit()) MapGlobals->Camera.param.pos.y = zoomOutLimit();
        mapCameraClamp(wk->map->area, &MapGlobals->Camera.param.pos);
        mapCameraClamp(wk->map->area, &MapGlobals->Camera.param.at);
        MapCameraOrientationUp(&MapGlobals->Camera);
    }
}

// Whole-stage camera of the stage into `out`; swaps the "zoom in" / "zoom out" button hints.
// FUNCTION: BIO4 0x0077B9C0
// STATUS: matching
static void mapCameraEntire(SUB_SCREEN* wk, MapCameraParam* out)
{
    *out = mapCameraTable[wk->map->area];
    MapIdSub.unitPtr(1, 0x1D)->be_flag |= 8;
    MapIdSub.unitPtr(1, 0x1D)->rev_flag &= 0xF0;
    MapIdSub.unitPtr(0, 0x1D)->rev_flag |= 0xF;
}

// Zoomed camera into `out`: centred between the player and the goal, high enough to frame both
// (4:3), clamped to the zoom limits; swaps the button hints.
// FUNCTION: BIO4 0x0077BAB0
// STATUS: stub
static void mapCameraZoomIn(SUB_SCREEN* wk, MapCameraParam* out)
{
    Vec pl;
    Vec goal;
    Vec mid;
    Vec d;
    f32 h;
    if (wk->open_flag == 2 && (wk->attr_flag & 1)) {
        if ((u8)(wk->map->area - 0xB) <= 3) {
            pl.x = wk->pl_mat_map[0][3];
            pl.y = wk->pl_mat_map[1][3];
            pl.z = wk->pl_mat_map[2][3];
            goal = pl;
            goal.x -= MapSpecialRangeX * 0.5;
            goal.z += MapSpecialRangeZ * 0.5;
            pl.x += MapSpecialRangeX * 0.5;
            pl.z -= MapSpecialRangeZ * 0.5;
        } else {
            pl.x = wk->pl_mat_map[0][3];
            pl.y = wk->pl_mat_map[1][3];
            pl.z = wk->pl_mat_map[2][3];
            Vec* goalPtr = &goal;
            // Internal PC convention: ECX=wk, one caller-clean stack slot.
            __asm {
                push goalPtr
                mov ecx, wk
                call MapGoalPosition
                add esp, 4
            }
        }
    } else {
        pl.x = wk->pl_mat_map[0][3];
        pl.y = wk->pl_mat_map[1][3];
        pl.z = wk->pl_mat_map[2][3];
        goal = pl;
        goal.x -= MapRangeX * 0.5;
        goal.z += MapRangeZ * 0.5;
        pl.x += MapRangeX * 0.5;
        pl.z -= MapRangeZ * 0.5;
    }
    MapVecAdd(&pl, &goal, &mid);
    MapVecScale(&mid, &mid, 0.5f);
    MapVecSubtract(&pl, &goal, &d);
    if (d.x < 0.0f) d.x = -d.x;
    if (d.y < 0.0f) d.y = -d.y;
    if (d.z < 0.0f) d.z = -d.z;
    if (!(d.z / d.x >= 0.75)) d.z = (f32)(d.x * 0.75);
    h = (f32)((double)d.z / MapTan((f32)(MapGlobals->Camera.param.fovy * 0.5 * (double)3.1415927f / 180.0)));
    h *= MapZoomHeightScale;
    if (h <= zoomInLimit()) h = zoomInLimit() * MapZoomMinScale;
    if (h >= zoomOutLimit()) h = zoomOutLimit();
    out->pos = mid;
    out->at = mid;
    out->pos.y += h;
    MapIdSub.unitPtr(1, 0x1D)->rev_flag |= 0xF;
    MapIdSub.unitPtr(0, 0x1D)->rev_flag &= 0xF0;
    MapIdSub.unitPtr(0, 0x1D)->be_flag |= 8;
}

// Camera interpolation from `from` to `to` over `max` frames; 1 when done.
// FUNCTION: BIO4 0x0077BE50
// STATUS: matching
u8 zoomMove(SsMapWork* m, int max, int cnt)
{
    f32 t = (f32) cnt / (f32) max;
    Vec up = {0.0f, 0.0f, -1.0f};
    Vec a;
    Vec b;
    f32 s;

    MapCameraParam* from = &m->from;
    MapCameraParam* to = &m->to;

    MapVecScale(&to->pos, &a, t);
    s = 1.0f - t;
    MapVecScale(&from->pos, &b, s);
    MapVecAdd(&a, &b, &MapGlobals->Camera.param.pos);
    MapVecScale(&to->at, &a, t);
    MapVecScale(&from->at, &b, s);
    MapVecAdd(&a, &b, &MapGlobals->Camera.param.at);
    // pG loads pG separately from the earlier pG loads, so pG is reloaded for the call after the copy
    *reinterpret_cast<Vec*>(reinterpret_cast<u8*>(MapGlobals) + 0x138) = up;
    MapCameraOrientationUp(&MapGlobals->Camera);
    if (t >= 1.0f) return 1;
    return 0;
}

// Archive offsets are read from the original loaded archive, never copied into source.
static u8* MapArc(u8* archive, int index) { return archive + ((u32*)archive)[index]; }
DECOMP_FUNC(int, MapSprintf, (char* dst, const char* fmt, ...), 0x00A6E37B);
DECOMP_FUNC(void, MapEasyFilename, (char* name), 0x0040D607);

// Steam supports areas 0..25, including the extra-mode map archives.
// FUNCTION: BIO4 0x0077BFC0
// STATUS: matching
static void mapAreaFilename(s8 area, char* name)
{
    switch (area) {
    case 0: MapSprintf(name, "SS/cmn/map_obj1.dat"); break;
    case 1: MapSprintf(name, "SS/cmn/map_obj1.dat"); break;
    case 2: MapSprintf(name, "SS/cmn/map_obj2a.dat"); break;
    case 3: MapSprintf(name, "SS/cmn/map_obj2b.dat"); break;
    case 4: MapSprintf(name, "SS/cmn/map_obj2c.dat"); break;
    case 5: MapSprintf(name, "SS/cmn/map_obj3a.dat"); break;
    case 6: MapSprintf(name, "SS/cmn/map_obj3b.dat"); break;
    case 7: MapSprintf(name, "SS/cmn/map_obj3c.dat"); break;
    case 8: MapSprintf(name, "SS/cmn/map_obj3d.dat"); break;
    case 9: MapSprintf(name, "SS/cmn/map_obj3e.dat"); break;
    case 10: MapSprintf(name, "SS/cmn/map_obj3f.dat"); break;
    case 11: MapSprintf(name, "SS/cmn/map_obj4a.dat"); break;
    case 12: MapSprintf(name, "SS/cmn/map_obj4b.dat"); break;
    case 13: MapSprintf(name, "SS/cmn/map_obj4c.dat"); break;
    case 14: MapSprintf(name, "SS/cmn/map_obj4d.dat"); break;
    case 15: MapSprintf(name, "SS/cmn/map_obj4e.dat"); break;
    case 16: MapSprintf(name, "SS/cmn/map_obj4f.dat"); break;
    case 17: MapSprintf(name, "SS/cmn/map_obj4g.dat"); break;
    case 18: MapSprintf(name, "SS/cmn/map_obj5a.dat"); break;
    case 19: MapSprintf(name, "SS/cmn/map_obj5b.dat"); break;
    case 20: MapSprintf(name, "SS/cmn/map_obj5c.dat"); break;
    case 21: MapSprintf(name, "SS/cmn/map_obj5d.dat"); break;
    case 22: MapSprintf(name, "SS/cmn/map_obj5e.dat"); break;
    case 23: MapSprintf(name, "SS/cmn/map_obj5f.dat"); break;
    case 24: MapSprintf(name, "SS/cmn/map_obj5g.dat"); break;
    case 25: MapSprintf(name, "SS/cmn/map_obj5h.dat"); break;
    }
    if (!*((u8*)pG + 0x4FA3) && !(pG->flags_SYSTEM[0] & 0xC0001000))
        MapEasyFilename(name + 3);
}

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

// Minimal manager layout confirmed by the three virtual allocations in 0x77EA20.
class MapManager {
public:
    virtual ~MapManager();
    virtual void* memAlloc(u32 bytes);
    virtual void memFree(void* p);
    virtual void memClear(void* p, u32 bytes);
    void* array; int count; u32 size; u32 flags; void* alive;
    void roomInit() { array = 0; count = 0; alive = 0; }
    void arrayAlloc(int n) { array = memAlloc(size*n); count = n; memClear(array, size*n); }
    DECOMP_METHOD0(void, MapManager, move, 0x0040DD82);
};
DECOMP_GLOBAL(MapManager, MapInfoMgr, 0x00C7B5C8);
DECOMP_GLOBAL(MapManager, MapPartsMgr, 0x00C7B5E8);
DECOMP_GLOBAL(MapManager, MapModels, 0x00C16320);
DECOMP_GLOBAL(MapManager*, MapInfoMgrPtr, 0x00C07110);
DECOMP_GLOBAL(MapManager*, MapPartsMgrPtr, 0x00C07114);

// FUNCTION: BIO4 0x0077EA20
// STATUS: matching
static void mapModelAlloc(SUB_SCREEN* wk)
{
    wk->model_flag |= 1;
    MapInfoMgr.roomInit(); MapInfoMgr.arrayAlloc(0x80);
    MapPartsMgr.roomInit(); MapPartsMgr.arrayAlloc(0x100);
    MapInfoMgrPtr = &MapInfoMgr; MapPartsMgrPtr = &MapPartsMgr;
    MapModels.roomInit(); MapModels.arrayAlloc(0x80);
}
DECOMP_GLOBAL(MapIdSystem, MapIdSys, 0x00C59788);
DECOMP_FUNC(void, MapIdSubErase, (), 0x004070EA);
DECOMP_FUNC(void, MapIdNumErase, (), 0x00408A99);
DECOMP_FUNC(void, MapIdFreeBuffer, (), 0x00408CAB);
DECOMP_FUNC(void, MapIdAllocBuffer, (), 0x004039CC);
DECOMP_FUNC(void, MapDataFilename, (SUB_SCREEN*, const char*), 0x00404101);
DECOMP_FUNC(void, MapScreenModelFree, (SUB_SCREEN*), 0x00407CA2);
DECOMP_FUNC(void, MapScreenLightClear, (SUB_SCREEN*), 0x00402AE5);
DECOMP_FUNC(void, MapResourceRequest, (u32, u16), 0x00401915);
DECOMP_FUNC(int, MapResourceReady, (u32), 0x00402CF2);
DECOMP_FUNC(void, MapFadeSetW, (u32, int, u32), 0x0040D0A8);
DECOMP_GLOBAL(int, MapWait, 0x00C7BA60);
DECOMP_GLOBAL(int, MapReadReq, 0x00C7BA5C);
DECOMP_GLOBAL(u32, MapResourceId, 0x00C7B97C);
DECOMP_GLOBAL(u32, MapModel64, 0x00C7BA64);
DECOMP_GLOBAL(u32, MapModel68, 0x00C7BA68);
DECOMP_GLOBAL(u32, MapModel6C, 0x00C7BA6C);
DECOMP_GLOBAL(u32, MapModel70, 0x00C7BA70);
DECOMP_GLOBAL(u32, MapModel74, 0x00C7BA74);
DECOMP_GLOBAL(u32, MapModel78, 0x00C7BA78);

// Steam loader also waits for the resource command associated with the language archive.
// FUNCTION: BIO4 0x0077EE90
// STATUS: stub
void SsMapInit::move(SUB_SCREEN* wk)
{
    switch (state) {
    case 0:
        if (wk->scrn_out_func(wk) == 1) {
            if (wk->menu_old == 2) wk->trans_off = 1;
            MapIdSubErase(); MapIdNumErase(); MapIdFreeBuffer();
            MapIdSub.set(MapArc(wk->pCmmn, 0xC), 0xFF, 0x11, 0xF, 1, 0);
            MapWait = 0; state++;
        }
        break;
    case 1:
        if (--MapWait >= 0) break;
        state++;
        break;
    case 2:
        MapIdSys.dispSw(0x21, 0); MapIdSys.dispSw(0x32, 0);
        MapResourceId = 0;
        MapDataFilename(wk, "ss_map.dat");
        if (wk->open_flag == 2)
            MapReadReq = DvdReadN(wk->filename, wk->pSwitchDat, 0, 0, 0, 0x11, "src\\ss_map.cpp", 0x13F2);
        else
            MapReadReq = DvdReadN(wk->filename, wk->pSwitchDat, 0, 0, 0, 0x10, "src\\ss_map.cpp", 0x13F7);
        if (MapReadReq <= 0) break;
        MapScreenModelFree(wk); mapModelAlloc(wk); MapScreenLightClear(wk);
        MapModel6C=0; MapModel64=0; MapModel70=0; MapModel74=0; MapModel68=0; MapModel78=0;
        MapIdAllocBuffer(); wk->trans_off=0;
        switch ((s8)wk->filename[4]) {
        case 'p': MapResourceId=0x3A000000; break;
        case 'n': MapResourceId=0x3A000001; break;
        case 'r': MapResourceId=0x3A000002; break;
        case 'e': MapResourceId=0x3A000003; break;
        case 't': MapResourceId=0x3A000004; break;
        case 's': MapResourceId=0x3A000005; break;
        case 'h':
            if (wk->filename[5]=='t') MapResourceId=0x3A000006;
            else if (wk->filename[5]=='s') MapResourceId=0x3A000009;
            break;
        }
        MapResourceRequest(MapResourceId, 0); state++;
        // fallthrough
    case 3: {
        u32 result, size;
        if (Dvd.ReadCheckOne(MapReadReq, &result, &size, 0)!=1) break;
        if (!MapResourceReady(MapResourceId)) break;
        wk->pMapDat=wk->pSwitchDat; wk->pMapObj=wk->pSwitchDat+result;
        MapResourceId=0; state++;
        // fallthrough
    }
    case 4:
        if (wk->open_flag==2) MapFadeSetW(0,5,0);
        transit(0,wk);
        break;
    }
}

DECOMP_FUNC(void*, MapNew, (u32 size), 0x0040E386);
DECOMP_GLOBAL(MapIdSystem, MapIdNum, 0x00C7AC78);
DECOMP_FUNC(void, MapIdTexDataLoad, (void*, int), 0x00401F46);
DECOMP_FUNC(void, MapLightCreate, (SUB_SCREEN*, void*), 0x0040DBFC);
DECOMP_FUNC(int, MapGetAreaNo, (u16 room), 0x00408B34);
DECOMP_FUNC(void, MapCameraInit, (SUB_SCREEN*, MapCamera*), 0x0077DCB0);
DECOMP_FUNC(u8, MapColumn2Available, (), 0x0077C260);
DECOMP_FUNC(void, MapMainMenuInit, (SUB_SCREEN*, int), 0x00409B56);
DECOMP_FUNC(bool, MapMainMenu, (SUB_SCREEN*), 0x004074B9);
DECOMP_FUNC(void, MapMerchantDisp, (SUB_SCREEN*, u8), 0x00407176);
DECOMP_FUNC(void, MapTreasureDisp, (SUB_SCREEN*, u8), 0x0040ADF8);
DECOMP_FUNC(void, MapCoinDisp, (SUB_SCREEN*, u8), 0x0040A67D);
DECOMP_FUNC(void, MapSaveDisp, (SUB_SCREEN*, u8), 0x00401EE2);
DECOMP_FUNC(void, MapPlayerDisp, (SUB_SCREEN*, u8), 0x0040EB60);
DECOMP_FUNC(void, MapGoalDisp, (SUB_SCREEN*, u8), 0x0040BEC4);
DECOMP_GLOBAL(MapCamera, MapSavedCamera, 0x00C7B870);
DECOMP_FUNC(void, MapInitViewportRaw, (), 0x00776780);
// Internal helper takes ESI, not a stack argument. MSVC preserves ESI here.
static void MapInitViewport(SUB_SCREEN* wk) {
    __asm { mov esi, wk
            call MapInitViewportRaw }
}
u8 scf_check_merchant();
u8 scf_check_submission();

// The six original constructors retain Steam vtables and allocate their link arrays.
// FUNCTION: BIO4 0x0077E0B0
// STATUS: stub
void SsMapMain::init(SUB_SCREEN* wk)
{
    if (MapGlobals->flags528C & 1) MapGlobals->flags52E0 |= 0x01000000;
    if (MapGlobals->flags5290 & 0x80000000) MapGlobals->flags52E0 |= 0x00800000;
    MapWidget* p;
    p=(MapWidget*)MapNew(0x10); focus=p ? p->constructFocus() : 0;
    p=(MapWidget*)MapNew(0x10); entire=p ? p->constructEntire() : 0;
    p=(MapWidget*)MapNew(0x14); zoomIn=p ? p->constructZoomIn() : 0;
    p=(MapWidget*)MapNew(0x14); zoomOut=p ? p->constructZoomOut() : 0;
    p=(MapWidget*)MapNew(0x10); read=p ? p->constructRead() : 0;
    p=(MapWidget*)MapNew(0x10); modeSel=p ? p->constructModeSelect() : 0;
    focus->connect(0,zoomIn);
    entire->connect(0,zoomIn); entire->connect(1,modeSel);
    read->connect(0,zoomIn); read->connect(1,zoomOut); read->connect(2,modeSel);
    zoomIn->connect(0,read); zoomIn->connect(1,zoomOut);
    zoomOut->connect(0,entire); zoomOut->connect(1,zoomIn);
    modeSel->connect(0,entire); modeSel->connect(1,read);
    cur=focus;
    MapIdTexDataLoad(MapArc(wk->pMapDat,4),9);
    if (!MapIdSub.setCk(0x11)) MapIdSub.set(MapArc(wk->pCmmn,0xC),0xFF,0x11,0xF,1,0);
    MapIdSub.set(MapArc(wk->pMapDat,5),0xFF,0x19,9,2,0);
    MapIdNum.set(MapArc(wk->pMapDat,0xA),0xFF,0x16,0xC,6,0);
    MapIdNum.set(MapArc(wk->pMapDat,9),0xFF,0x15,0xC,6,0);
    MapIdNum.set(MapArc(wk->pMapDat,8),0xFF,0x14,0xC,6,0);
    MapIdSub.set(MapArc(wk->pMapDat,6),0xFF,0x14,0xC,5,0);
    MapIdSub.set(MapArc(wk->pMapDat,7),0xFF,0x10,0xF,2,0);
    MapIdUnit* u=MapIdSub.unitPtr(0xFD,0x10);
    if (wk->room_no==0x22C) u->be_flag|=8; else u->be_flag&=~8;
    MapIdSub.set(MapArc(wk->pMapDat,0xB),0xFF,0x1D,0x13,8,0);
    MapIdSub.unitPtr(0x10,0x10)->rev_flag|=0xF;
    MapIdSub.unitPtr(0x10,0x10)->be_flag&=~8;
    MapIdSub.unitPtr(0,0x10)->be_flag&=~8;
    MapIdSub.unitPtr(0,0x10)->rev_flag|=0xF;
    MapLightCreate(wk,MapArc(wk->pCmmn,0x13));
    wk->map=(SsMapWork*)Mem_alloc(0x1060,0,0,1,0xD);
    MapInitViewport(wk);
    wk->map->area=(s8)MapGetAreaNo(wk->room_no);
    MapCameraInit(wk,&MapGlobals->Camera);
    MapSavedCamera=MapGlobals->Camera;
    MapIdSub.unitPtr(1,0x1D)->be_flag&=~8;
    MapIdSub.unitPtr(0,0x1D)->be_flag&=~8;
    MapIdSub.unitPtr(2,0x1D)->be_flag&=~8;
    if (!MapColumn2Available()) MapIdSub.unitPtr(0x14,0x1D)->be_flag&=~8;
    if (wk->map->area==1) {
        if (!scf_check_merchant()) MapIdSub.unitPtr(0x12,0x1D)->be_flag&=~8;
        if (!scf_check_submission()) MapIdSub.unitPtr(0x13,0x1D)->be_flag&=~8;
    } else if (MapGlobals->flags54 & 0x1000) MapIdSub.unitPtr(0x13,0x1D)->be_flag&=~8;
    MapMainMenuInit(wk,0);
    MapIdSub.unitPtr(3,0x14)->be_flag&=~8;
    MapIdSub.unitPtr(0,0x14)->be_flag&=~8;
    MapIdSub.unitPtr(1,0x14)->be_flag&=~8;
    MapMerchantDisp(wk,0); MapTreasureDisp(wk,0); MapCoinDisp(wk,0); MapSaveDisp(wk,0);
    state=2; step=0;
    SndCall(0,0x1E,0,0,0,0);
    MapViewEnabled=1;
}

DECOMP_FUNC(bool, MapKeyTrgCheck, (unsigned __int64 mask), 0x0040EC0F);
DECOMP_FUNC(void, MapStageNameDisp, (SUB_SCREEN*), 0x00776990);
DECOMP_GLOBAL(u32, MapPendingResource, 0x00C7B980);
DECOMP_FUNC(void, MapModelInitRaw, (), 0x007803F0);
DECOMP_FUNC(void, MapDoorInitRaw, (), 0x0077EBD0);
DECOMP_FUNC(void, MapGridDispRaw, (), 0x0077D780);
DECOMP_FUNC(void, MapDoorDispRaw, (), 0x0077DA30);
DECOMP_FUNC(void, MapMarkCharDispRaw, (), 0x0077CD10);
DECOMP_FUNC(void, MapGoalInit, (SUB_SCREEN*), 0x0040E1C4);
DECOMP_FUNC(void, MapMerchantInit, (SUB_SCREEN*), 0x00405D85);
DECOMP_FUNC(void, MapTreasureInit, (SUB_SCREEN*), 0x0040F213);
DECOMP_FUNC(void, MapCoinInit, (SUB_SCREEN*), 0x00407590);
DECOMP_FUNC(void, MapSaveInit, (SUB_SCREEN*), 0x0040351C);
DECOMP_FUNC(bool, MapPos2ScreenPos, (Vec*, Vec*), 0x0040AE52);
DECOMP_FUNC(void, MapChangeViewport, (SUB_SCREEN*), 0x0077CC60);
struct MapActiveModel { u8 pad_00[8]; MapActiveModel* next; };
class MapLightSystem { public: DECOMP_METHOD(void, MapLightSystem, setModel2, (MapActiveModel* m), (m), 0x004040BB); };
DECOMP_GLOBAL(MapActiveModel*, MapActiveModels, 0x00C16334);
DECOMP_GLOBAL(MapLightSystem, MapLights, 0x00C06B88);
DECOMP_GLOBAL(SUB_SCREEN, MapScreen, 0x00C7ACD8);
struct MapPartnerInfo { u8 pad_00[0x100]; u8 id; };
DECOMP_GLOBAL(MapPartnerInfo*, MapPartner, 0x00C57060);
static void MapModelInit(SUB_SCREEN* wk) { __asm { mov ecx,wk
    call MapModelInitRaw } }
static void MapDoorInit(SUB_SCREEN* wk) { __asm { mov ecx,wk
    call MapDoorInitRaw } }
static void MapGridDisp(SUB_SCREEN* wk) { __asm { mov ecx,wk
    call MapGridDispRaw } }
static void MapDoorDisp(SUB_SCREEN* wk) { __asm { mov eax,wk
    call MapDoorDispRaw } }
static void MapMarkCharDisp(MapIdUnit* u, Mtx* mat) { __asm {
    mov esi,u
    push mat
    call MapMarkCharDispRaw
    add esp,4
} }
static bool MapGetGoalPosition(SUB_SCREEN* wk, Vec* out) {
    bool found;
    __asm { mov ecx,wk
        push out
        call MapGoalPosition
        add esp,4
        mov found,al }
    return found;
}

// Steam separates archive/resource readiness and ID animation readiness (steps 1 and 2).
// FUNCTION: BIO4 0x00780AA0
// STATUS: stub
void SsMapMain::move(SUB_SCREEN* wk)
{
    MapGlobals->Camera=MapSavedCamera;
    switch (state) {
    case 0: {
        MapWidget* w=cur;
        if (w==read) {
            if (wk->open_flag==2) {
                if (MapKeyTrg & 0x0000200040200000ULL) {
                    wk->close_flag|=4; transitOriginal(0,wk); break;
                }
            } else {
                if (MapKeyTrg & 0x100000ULL) {
                    wk->close_flag|=4; transitOriginal(0,wk); break;
                }
                if (MapKeyTrgCheck(0x0000200040000000ULL)) {
                    state=1; wk->close_flag|=4; MapMainMenuInit(wk,1);
                    SndCall(0,0xA,0,0,0,0); break;
                }
            }
        }
        w->move(wk); cur=w->cur;
        break;
    }
    case 1:
        if (MapMainMenu(wk)) {
            switch (wk->menu_no) {
            case 0: transitOriginal(2,wk); break;
            case 1: transitOriginal(1,wk); break;
            case 2: MapMainMenuInit(wk,0); state=0; break;
            case 3: transitOriginal(4,wk); break;
            case 4: transitOriginal(0,wk); break;
            }
        }
        break;
    case 2: if (step>=3) state=0; break;
    }
    switch (step) {
    case 0: {
        char name[64];
        MapStageNameDisp(wk); MapPendingResource=0;
        mapAreaFilename(wk->map->area,name);
        readReq=DvdReadN(name,wk->pMapObj,0,0,0,0x10,"src\\ss_map.cpp",5694);
        if (readReq<=0) break;
        step++;
        switch ((s8)name[3]) {
        case 'c': MapPendingResource=0x1B000007; break;
        case 'e': MapPendingResource=0x1B000008; break;
        case 'j': MapPendingResource=0x1B000000; break;
        }
        MapResourceRequest(MapPendingResource,0);
        // fallthrough
    }
    case 1:
        if (Dvd.ReadCheckOne(readReq,0,0,0)!=1) break;
        if (!MapResourceReady(MapPendingResource)) break;
        step++; MapPendingResource=0;
        // fallthrough
    case 2:
        if (!(MapIdSub.unitPtr(1,0x10)->anim_flags & 1)) break;
        MapModelInit(wk); MapDoorInit(wk);
        MapGoalInit(wk); MapMerchantInit(wk); MapTreasureInit(wk); MapCoinInit(wk); MapSaveInit(wk);
        step++; SndCall(0,0x1B,0,0,0,0);
        // fallthrough
    case 3: {
        MapModels.move();
        MapActiveModel* m=MapActiveModels;
        while (m) { MapActiveModel* p=m; m=m->next; MapLights.setModel2(p); }
        MapDoorDisp(wk); MapGridDisp(wk);
        if (MapScreen.open_flag==2 && (MapScreen.attr_flag & 1) && MapScreen.room_no==0x304)
            MapPlayerDisp(wk,0);
        else {
            MapIdUnit* u=MapIdSub.unitPtr(0,0x14);
            MapMarkCharDisp(u,&wk->pl_mat_map); u->be_flag|=8;
            u=MapIdSub.unitPtr(1,0x14);
            if (MapPartner && MapPartner->id==3) { MapMarkCharDisp(u,&wk->map->subMapMat); u->be_flag|=8; }
        }
        if (mapMarkTable[MapCurrentWork->area].column[0]) {
            Vec pos,screen;
            MapIdUnit* u=MapIdSub.unitPtr(3,0x14); u->be_flag&=~8;
            if (MapGetGoalPosition(wk,&pos) && MapPos2ScreenPos(&pos,&screen)) {
                u->pos0=screen; u->be_flag|=8;
            }
        } else MapGoalDisp(wk,0);
        MapSaveDisp(wk,!(wk->map_mode & 1) && mapMarkTable[MapCurrentWork->area].column[4]);
        MapMerchantDisp(wk,!(wk->map_mode & 2) && scf_check_merchant());
        MapTreasureDisp(wk,!(wk->map_mode & 4) && MapColumn2Available());
        MapCoinDisp(wk,!(wk->map_mode & 8) && scf_check_submission());
        break;
    }
    }
    MapSavedCamera=MapGlobals->Camera;
    MapChangeViewport(wk);
}

// Leaving the map: deletes the widgets and mark models, frees the map work, installs sscrn_map_out.
// FUNCTION: BIO4 0x0077F210
// STATUS: matching
void SsMapMain::quit(SUB_SCREEN* wk)
{
    MapViewEnabled = 0;
    delete focus;
    delete entire;
    delete zoomIn;
    delete zoomOut;
    delete read;
    delete modeSel;
    delete wk->map->pGoal;
    delete wk->map->pMerchant;
    delete wk->map->pTreasure;
    delete wk->map->pCoin;
    delete wk->map->pSave;
    Mem_free(wk->map);
    wk->close_flag |= 4;
    sscrn_map_out_init(wk);
    wk->scrn_out_func = MapExitOriginal;
}

// First widget: starts the zoom from the current camera to the player/goal view (MapZoomIn).
// FUNCTION: BIO4 0x0077F3A0
// STATUS: stub
void MapFocus::move(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;

    m->from = MapGlobals->Camera.param;
    mapCameraZoomIn(wk, &wk->map->to);
    transit(0, wk);
}

// Whole-stage view: A/B zoom in (link 0), X (Key bit 17) opens the mark mode menu (link 1, returns
// here).
// FUNCTION: BIO4 0x0077F430
// STATUS: stub
void MapEntire::move(SUB_SCREEN* wk)
{
    if (MapKeyTrg & 0x2000C0000000ULL) {
        SsMapWork* m = wk->map;

        m->from = MapGlobals->Camera.param;
        mapCameraZoomIn(wk, &wk->map->to);
        transit(0, wk);
    } else if (MapMenuKey & 0x10) {
        wk->map->modeSel = 0;
        transit(1, wk);
    }
}

// Interpolates the camera to the zoomed view; A reverses into MapZoomOut, done -> MapRead.
// FUNCTION: BIO4 0x0077F520
// STATUS: matching
void MapZoomIn::move(SUB_SCREEN* wk)
{
    if (MapKeyTrg & 0x80000000ULL) {
        SsMapWork* m = wk->map;

        m->from = MapGlobals->Camera.param;
        mapCameraEntire(wk, &wk->map->to);
        transit(1, wk);
    } else {
        if (zoomMove(wk->map, 10, count++)) {
            transit(0, wk);
        }
    }
}

// Interpolates the camera to the whole-stage view; B reverses into MapZoomIn, done -> MapEntire.
// FUNCTION: BIO4 0x0077F610
// STATUS: stub
void MapZoomOut::move(SUB_SCREEN* wk)
{
    if (MapKeyTrg & 0x200040000000ULL) {
        SsMapWork* m = wk->map;

        m->from = MapGlobals->Camera.param;
        mapCameraZoomIn(wk, &wk->map->to);
        transit(1, wk);
    } else {
        if (zoomMove(wk->map, 10, count++)) {
            transit(0, wk);
        }
    }
}

// Zoomed map reading: A zooms out (link 1), X opens the mark mode menu (link 2, returns here),
// otherwise the d-pad scrolls the camera (mapCameraMove).
// FUNCTION: BIO4 0x0077F710
// STATUS: stub
void MapRead::move(SUB_SCREEN* wk)
{
    if (MapKeyTrg & 0x80000000ULL) {
        SsMapWork* m = wk->map;

        m->from = MapGlobals->Camera.param;
        mapCameraEntire(wk, &wk->map->to);
        transit(1, wk);
    } else if (MapMenuKey & 0x10) {
        wk->map->modeSel = 1;
        transit(2, wk);
    } else {
        mapCameraMove(wk);
    }
}

// Mark mode menu open: shows the menu panel (IdSub 0x10/0x10), cursor on row 0 (typewriter), hides
// the rows whose marks are not yet available (merchant before scenario bit 14, treasure
// without item 0xA9, medallions without 0xB0 / the mission), swaps the button hints.
// FUNCTION: BIO4 0x0077C510
// STATUS: stub
void MapModeSelect::init(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;
    MapIdUnit* u;
    MapIdUnit* u2;
    MapIdSub.unitPtr(0x10, 0x10)->be_flag |= 8;
    MapIdSub.unitPtr(0x10, 0x10)->rev_flag &= 0xF0;
    m->modeCursor = 0;
    u = MapIdSub.unitPtr(0x20, 0x10);
    u2 = MapIdSub.unitPtr(0x60, 0x10);
    u->pos0 = u2->pos0;
    *reinterpret_cast<u32*>(&u->timer[0]) = 0;
    *reinterpret_cast<u32*>(&u->timer[2]) = 0;
    if (MapGlobals->scenarioFlags & 0x4000) MapIdSub.unitPtr(0x61, 0x10)->be_flag &= ~8;
    if (wk->room_no != 0x22C) {
        if (wk->stage_no == 1 && MapItems.numType(0xA9, 0)) goto showTreasure;
        if (wk->stage_no == 2 && MapItems.numType(0x54, 0)) goto showTreasure;
        if (wk->stage_no == 3 && MapItems.numType(0x55, 0)) goto showTreasure;
        switch (m->area) {
        case 18: if (MapItems.num(0x10B)) goto showTreasure; break;
        case 19: if (MapItems.num(0x10C)) goto showTreasure; break;
        case 20: if (MapItems.num(0x10D)) goto showTreasure; break;
        case 21: case 22: case 23: if (MapItems.num(0x10E)) goto showTreasure; break;
        case 24: case 25: if (MapItems.num(0x10F)) goto showTreasure; break;
        }
    }
    goto checkCoin;
showTreasure:
    MapIdSub.unitPtr(0x62, 0x10)->be_flag &= ~8;
checkCoin:
    if (MapItems.num(0xB0) || (MapGlobals->scenarioFlags & 0x400000)) MapIdSub.unitPtr(0x63, 0x10)->be_flag &= ~8;
    MapIdSub.unitPtr(2, 0x1D)->be_flag |= 8;
    MapIdSub.unitPtr(2, 0x1D)->rev_flag &= 0xF0;
    MapIdSub.unitPtr(1, 0x1D)->rev_flag |= 0xF;
    MapIdSub.unitPtr(0, 0x1D)->rev_flag |= 0xF;
    SndCall(0, 9, 0, 0, 0, 0);
}

// Mark mode menu: B/X close it back to Entire (modeSel 0) or Read (1); A toggles the mark kind
// under the cursor when available; up/down move the cursor (rows 0x60.. positions, check marks
// 0x50.. shown for enabled kinds).
// FUNCTION: BIO4 0x0077F810
// STATUS: stub
void MapModeSelect::move(SUB_SCREEN* wk)
{
    SsMapWork* m = wk->map;
    MapIdUnit* u;
    if ((MapKeyTrg & 0x40000000ULL) || (MapMenuKey & 0x10)) {
        switch (m->modeSel) {
        case 0:
            MapIdSub.unitPtr(2, 0x1D)->rev_flag |= 0xF;
            MapIdSub.unitPtr(1, 0x1D)->be_flag |= 8;
            MapIdSub.unitPtr(1, 0x1D)->rev_flag &= 0xF0;
            transit(0, wk); return;
        case 1:
            MapIdSub.unitPtr(2, 0x1D)->rev_flag |= 0xF;
            MapIdSub.unitPtr(0, 0x1D)->be_flag |= 8;
            MapIdSub.unitPtr(0, 0x1D)->rev_flag &= 0xF0;
            transit(1, wk); return;
        }
    }
    if (MapKeyTrg & 0x80000000ULL) {
        switch (m->modeCursor) {
        case 1: if (!(MapGlobals->scenarioFlags & 0x4000)) return; break;
        case 3: if (!MapItems.num(0xB0) && !(MapGlobals->scenarioFlags & 0x400000)) return; break;
        case 2:
            switch (m->area) {
            case 1: if (!MapItems.searchType(0xA9, 0)) return; break;
            case 2: case 3: case 4: if (!MapItems.searchType(0x54, 0)) return; break;
            case 5: case 6: case 7: case 8: case 9: case 10: if (!MapItems.searchType(0x55, 0)) return; break;
            case 18: if (!MapItems.searchCurrent(0x10B)) return; break;
            case 19: if (!MapItems.searchCurrent(0x10C)) return; break;
            case 20: if (!MapItems.searchCurrent(0x10D)) return; break;
            case 21: case 22: case 23: if (!MapItems.searchCurrent(0x10E)) return; break;
            case 24: case 25: if (!MapItems.searchCurrent(0x10F)) return; break;
            default: return;
            }
            break;
        }
        u8 bit = (u8)(1u << m->modeCursor);
        if (wk->map_mode & bit) wk->map_mode &= (u8)~bit;
        else wk->map_mode |= bit;
        SndCall(0, 0x1D, 0, 0, 0, 0);
    } else {
        int i;
        int old;
        for (i = 0; i < 4; i++) {
            u = MapIdSub.unitPtr((u8)(0x50 + i), 0x10);
            if (wk->map_mode & (1u << i)) u->be_flag &= ~8;
            else u->be_flag |= 8;
        }
        old = m->modeCursor;
        if (MapKeyRep & 0x01000000) m->modeCursor--;
        if (MapKeyRep & 0x02000000) m->modeCursor++;
        m->modeCursor = m->modeCursor < 0 ? 3 : (m->modeCursor > 3 ? 0 : m->modeCursor);
        if (old != m->modeCursor) {
            u = MapIdSub.unitPtr(0x20, 0x10);
            MapIdUnit* u2 = MapIdSub.unitPtr((u8)(0x60 + m->modeCursor), 0x10);
            u->pos0 = u2->pos0;
            *reinterpret_cast<u32*>(&u->timer[0]) = 0;
            *reinterpret_cast<u32*>(&u->timer[2]) = 0;
            SndCall(0, 6, 0, 0, 0, 0);
        }
    }
}

// Hides the mode menu panel.
// FUNCTION: BIO4 0x0077C920
// STATUS: matching
void MapModeSelect::quit(SUB_SCREEN* wk)
{
    MapIdSub.unitPtr(0x10, 0x10)->rev_flag |= 0xF;
    SndCall(0, 5, 0, 0, 0, 0);
}

// Steam uses the current map area rather than the GameCube stage number.
// FUNCTION: BIO4 0x0077B9A0
// STATUS: matching
f32 zoomOutLimit()
{
    return mapCameraTable[MapCurrentWork->area].pos.y;
}

// Return width is AL in the Steam caller and callee.
// FUNCTION: BIO4 0x0077C440
// STATUS: matching
u8 scf_check_typewriter()
{
    return mapMarkTable[MapCurrentWork->area].column[4];
}

// FUNCTION: BIO4 0x0077C220
// STATUS: stub
u8 scf_check_merchant()
{
    SsMapWork* m = MapCurrentWork;
    switch (m->area) {
    case 1: return (u8)(MapGlobals->scenarioFlags >> 14) & 1;
    default: return mapMarkTable[m->area].column[1];
    }
}

// FUNCTION: BIO4 0x0077C3E0
// STATUS: stub
u8 scf_check_submission()
{
    SsMapWork* m = MapCurrentWork;
    switch (m->area) {
    case 1:
        if (MapItems.num(0xB0) != 0 || (MapGlobals->scenarioFlags & 0x400000)) return 1;
        return 0;
    default: return mapMarkTable[m->area].column[3];
    }
}
