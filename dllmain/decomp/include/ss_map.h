#ifndef DECOMP_SS_MAP_H
#define DECOMP_SS_MAP_H

#include "types.h"
#include "bind.h"
#include "sscrn.h"

// Steam camera parameters and map work: offsets corroborated by 0x77BE50.
struct MapCameraParam { Vec pos; Vec at; f32 roll; f32 fovy; };
struct MapCamera {
    u8 pad_00[0xA4];
    MapCameraParam param;
    Vec Up;
    u8 pad_D0[0xF8 - 0xD0];
};
struct MapGlobal {
    u8 pad_00[0x54]; u32 flags54; u8 pad_58[0x74-0x58]; MapCamera Camera;
    u8 pad_16C[0x528C-0x16C]; u32 flags528C, flags5290;
    u8 pad_5294[0x52D0-0x5294]; u32 scenarioFlags;
    u8 pad_52D4[0x52E0-0x52D4]; u32 flags52E0;
};
class MapOwnedModel { public: virtual ~MapOwnedModel(); };
struct SsMapWork {
    u8 pad_00[0xFB4];
    MapOwnedModel* pGoal;
    MapOwnedModel* pMerchant;
    MapOwnedModel* pTreasure;
    MapOwnedModel* pCoin;
    MapOwnedModel* pSave;
    u8 pad_FC8[4];
    MapCameraParam from;
    MapCameraParam to;
    s8 area, roomIdx, modeCursor, modeSel; // 0x100C..0x100F
    u8 pad_1010[0x1030-0x1010];
    Mtx subMapMat; // 0x1030
};
typedef char check_map_camera[sizeof(MapCamera) == 0xF8 ? 1 : -1];
typedef char check_map_work[sizeof(SsMapWork) == 0x1060 ? 1 : -1];

// SUB_SCREEN: sscrn.h (shared by every sub screen).

// Steam places the vptr first: num/link/cur follow at 4/8/C.
class MapWidget {
public:
    int num;
    MapWidget** link;
    MapWidget* cur;
    virtual ~MapWidget();
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
    DECOMP_METHOD(void, MapWidget, transitOriginal, (int no, SUB_SCREEN* wk), (no, wk), 0x004023BF);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructFocus, 0x004069DD);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructEntire, 0x004025C7);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructZoomIn, 0x0040EB74);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructZoomOut, 0x00402009);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructRead, 0x0040BC1C);
    DECOMP_METHOD0(MapWidget*, MapWidget, constructModeSelect, 0x00409CC3);
    void connect(int no, MapWidget* target) { if (no < num) link[no] = target; }
    void transit(int no, SUB_SCREEN* wk) {
        if (link[no]) {
            quit(wk);
            cur = link[no];
            cur->init(wk);
            cur->cur = cur;
        }
    }
};
typedef char check_map_widget[sizeof(MapWidget) == 0x10 ? 1 : -1];
class SsMapInit : public MapWidget {
public:
    s32 state; // 0x10
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};
class MapZoomIn : public MapWidget {
public:
    s8 count; // 0x10
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class MapZoomOut : public MapWidget {
public:
    s8 count;  // 0x10
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class MapModeSelect : public MapWidget {
public:
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class SsMapMain : public MapWidget {
public:
    int state, step, readReq;
    MapWidget* focus;
    MapWidget* entire;
    MapWidget* zoomIn;
    MapWidget* zoomOut;
    MapWidget* read;
    MapWidget* modeSel;
    MapWidget* cur;
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
};
class MapRead : public MapWidget { public: virtual void move(SUB_SCREEN* wk); };
class MapFocus : public MapWidget { public: virtual void move(SUB_SCREEN* wk); };
class MapEntire : public MapWidget { public: virtual void move(SUB_SCREEN* wk); };
#endif
