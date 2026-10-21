#ifndef ETC_MODEL_H
#define ETC_MODEL_H

#include "bind.h"
#include "em.h"
#include "global.h"
#include "types.h"

// Room etc models (game/EtcModel.cpp), Steam layouts. Names follow
// references/re4-gamecube/include/etc_model.h; offsets and values are the
// ones the Steam assembly uses (0x00617840..0x0061EC20).

// Room item (game/EtcModel.cpp). Only the flag word is known.
struct EtcItem {
    u32 flags;   // 0x00  0x02: taken
    u8 pad_4[0x70 - 0x04];
    Vec pos;     // 0x70
};

// Etc model id (PS2 ETCMODEL_ID). The Steam build has the two PS2 ids
// (ETC_IRON_DOOR26/27, Et68_init / Et69_init) and 0x6A ids in all.
enum ETCMODEL_ID {
    ETC_WINDOW00 = 0,
    ETC_WOODBOX_SML = 1,
    ETC_WOODBOX_MDL = 2,
    ETC_DOOR00 = 3,
    ETC_TANA00 = 4,
    ETC_TANA01 = 5,
    ETC_HASIGO00 = 6,
    ETC_WINDOW07 = 7,
    ETC_HASIGO01 = 8,
    ETC_DOOR_SP00 = 9,
    ETC_TAIMATU02 = 10,
    ETC_LANTERN_A = 11,
    ETC_DENKYUU = 12,
    ETC_IRON_DOOR00 = 13,
    ETC_SWITCH = 14,
    ETC_BARRED00 = 15,
    ETC_LANTERN_B = 16,
    ETC_WOODBOX_BARREL = 17,
    ETC_DRAM = 18,
    ETC_DOOR01 = 19,
    ETC_TAIMATU01 = 20,
    ETC_TANA_BOX = 21,
    ETC_IRON_DOOR01 = 22,
    ETC_IRON_DOOR02 = 23,
    ETC_DOOR02 = 24,
    ETC_TAIMATU03 = 25,
    ETC_MEDAL00 = 26,
    ETC_BARRED01 = 27,
    ETC_DENKYUU01 = 28,
    ETC_WINDOW1D = 29,
    ETC_WOODBOX_BARREL2 = 30,
    ETC_NEST = 31,
    ETC_DOOR03 = 32,
    ETC_IRON_DOOR03 = 33,
    ETC_DOOR04 = 34,
    ETC_IRON_DOOR_DOWN00 = 35,
    ETC_IRON_DOOR04 = 36,
    ETC_WINDOW25 = 37,
    ETC_BARRED02 = 38,
    ETC_IRON_DOOR05 = 39,
    ETC_BARRED03 = 40,
    ETC_WINDOW29 = 41,
    ETC_IRON_DOOR06 = 42,
    ETC_IRON_DOOR07 = 43,
    ETC_WINDOW2C = 44,
    ETC_BOMB_BARREL = 45,
    ETC_TUBO_A_S = 46,
    ETC_TUBO_A_L = 47,
    ETC_YOROI = 48,
    ETC_IRON_DOOR_DOWN02 = 49,
    ETC_IRON_DOOR11 = 50,
    ETC_IRON_DOOR12 = 51,
    ETC_IRON_DOOR13 = 52,
    ETC_WINDOW35 = 53,
    ETC_WINDOW36 = 54,
    ETC_IRON_DOOR_DOWN01 = 55,
    ETC_DENKYUU02 = 56,
    ETC_IRON_DOOR08 = 57,
    ETC_BARRED04 = 58,
    ETC_IRON_DOOR10 = 59,
    ETC_GUS_BOMBE = 60,
    ETC_DOOR05 = 61,
    ETC_IRON_DOOR14 = 62,
    ETC_IRON_DOOR15 = 63,
    ETC_DOOR06 = 64,
    ETC_IRON_DOOR17 = 65,
    ETC_DEKA_ITA = 66,
    ETC_AUTO_DOOR = 67,
    ETC_WINDOW44 = 68,
    ETC_IRON_DOOR18 = 69,
    ETC_IRON_DOOR19 = 70,
    ETC_IRON_DOOR20 = 71,
    ETC_WINDOW48 = 72,
    ETC_IRON_DOOR21 = 73,
    ETC_WINDOW4A = 74,
    ETC_AUTO_DOOR2 = 75,
    ETC_BARRED05 = 76,
    ETC_IRON_DOOR22 = 77,
    ETC_BARRED06 = 78,
    ETC_AUTO_DOOR3 = 79,
    ETC_WINDOW50 = 80,
    ETC_WINDOW51 = 81,
    ETC_WINDOW52 = 82,
    ETC_WINDOW53 = 83,
    ETC_WINDOW54 = 84,
    ETC_WINDOW55 = 85,
    ETC_WINDOW56 = 86,
    ETC_WINDOW57 = 87,
    ETC_WINDOW58 = 88,
    ETC_IRON_DOOR23 = 89,
    ETC_WINDOW5A = 90,
    ETC_WINDOW5B = 91,
    ETC_WINDOW5C = 92,
    ETC_WINDOW5D = 93,
    ETC_WINDOW5E = 94,
    ETC_WINDOW5F = 95,
    ETC_WINDOW60 = 96,
    ETC_AUTO_DOOR4 = 97,
    ETC_IRON_DOOR25 = 98,
    ETC_IRON_DOOR24 = 99,
    ETC_WINDOW64 = 100,
    ETC_WINDOW65 = 101,
    ETC_ZOU = 102,
    ETC_AUTO_DOOR5 = 103,
    ETC_IRON_DOOR26 = 104,
    ETC_IRON_DOOR27 = 105,
    ETC_ID_MAX = 106
};

// One room etc model record (0x28 bytes). Little endian: the etc number the
// Set* functions take (GameCube `type`, the low byte of `no`) is at 0x02.
struct EtcSetData {
    u16 id;          // 0x00  ETCMODEL_ID
    union {
        u16 no;      // 0x02  g_EtcTbl slot (< 0x40)
        u8 type;     // 0x02  low byte of `no`
    };
    u8 pad_4[0x10 - 0x4];
    Vec ang;         // 0x10
    Vec pos;         // 0x1C
};

// Additive ambient kind (PS2 ETC_AMB_KIND): EtcSetAddAmb `kind`.
enum ETC_AMB_KIND {
    ETC_AMB_TORCH = 0,
    ETC_AMB_BARREL = 1,
    ETC_AMB_DOOR = 2,
    ETC_AMB_BOX = 3,
    ETC_AMB_ITEM = 4,
    ETC_AMB_HIT = 5,
    ETC_AMB_WINDOW00 = 6,
    ETC_AMB_WINDOW90 = 7,
    ETC_AMB_RACK = 8,
    ETC_AMB_NEST = 9,
    ETC_AMB_DRAM = 10,
    ETC_AMB_TUBO = 11,
    ETC_AMB_FALL_LANTERN = 12,
    ETC_AMB_BAR = 13,
    ETC_AMB_WINDOW30 = 14,
    ETC_AMB_WINDOW40 = 15,
    ETC_AMB_WINDOW50 = 16
};

// Which etc_*_rgb table the room uses (PS2 ETC_AMB_TYPE): GetEtcAmbType.
enum ETC_AMB_TYPE {
    ETC_AMB_DAY = 0,
    ETC_AMB_NIGHT = 1,
    ETC_AMB_DAY2 = 2,
    ETC_AMB_DAY3 = 3,
    ETC_AMB_DAY4 = 4
};

// Effect data owners (EspDataLoad `owner`) of the etc models. The Steam build
// numbers them like the PS2: the GameCube value + 4 in this range
// (EFF_ET01 0x59, EFF_OBM2B 0xCD in Et01_init / Et03_init). The setEff
// arguments (effect owner of the model) shift the same way.
enum ESP_OWNER_ETC {
    EFF_ET01 = 89, EFF_ET02 = 90, EFF_ET03 = 91, EFF_ET04 = 92, EFF_ET05 = 93, EFF_ET06 = 94,
    EFF_ET08 = 96, EFF_ET09 = 97, EFF_ET0A = 98, EFF_ET0B = 99, EFF_ET0C = 100, EFF_ET0D = 101,
    EFF_ET0F = 103, EFF_ET10 = 104, EFF_ET11 = 105, EFF_ET12 = 106, EFF_ET13 = 107, EFF_ET14 = 108,
    EFF_ET15 = 109, EFF_ET16 = 110, EFF_ET17 = 111, EFF_ET18 = 112, EFF_ET19 = 113, EFF_ET1A = 114,
    EFF_ET1C = 116, EFF_ET1F = 119, EFF_ET20 = 120, EFF_ET21 = 121, EFF_ET22 = 122, EFF_ET23 = 123,
    EFF_ET24 = 124, EFF_ET27 = 127, EFF_ET2A = 130, EFF_ET2B = 131, EFF_ET2D = 133, EFF_ET2E = 134,
    EFF_ET2F = 135, EFF_ET30 = 136, EFF_ET31 = 137, EFF_ET32 = 138, EFF_ET33 = 139, EFF_ET34 = 140,
    EFF_ET37 = 143, EFF_ET38 = 144, EFF_ET39 = 145, EFF_ET3A = 146, EFF_ET3B = 147, EFF_ET3C = 148,
    EFF_ET3D = 149, EFF_ET3E = 150, EFF_ET3F = 151, EFF_ET40 = 152, EFF_ET41 = 153, EFF_ET42 = 154,
    EFF_ET43 = 155, EFF_ET45 = 157, EFF_ET46 = 158, EFF_ET47 = 159, EFF_ET49 = 161, EFF_ET4B = 163,
    EFF_ET4C = 164, EFF_ET4D = 165, EFF_ET4E = 166, EFF_ET4F = 167, EFF_ET59 = 177, EFF_ET61 = 185,
    EFF_ET62 = 186, EFF_ET63 = 187, EFF_ET66 = 190, EFF_ET67 = 191, EFF_OBM2B = 205, EFF_OBM4C = 207
};

// Debug log (GameCube db_log.h). The Steam release keeps the calls, with
// the arguments, but the target 0x00974040 is an empty function (folded
// with every other empty cdecl function), so the harness needs no binding.
#ifndef pLog
class cDbLog {
public:
#ifdef DECOMP_HARNESS
    static void err(int lv, int kind, const char* fmt, ...) {}
#else
    static void err(int lv, int kind, const char* fmt, ...);
#endif
};
#define pLog ((cDbLog*) 0)
#endif

// Room data (GameCube room_data.h): the global object and the save record
// lookup GetEtcFlgPtr uses.
class cRoomData {
public:
    DECOMP_METHOD(u8*, cRoomData, getRoomSavePtr, (u16 room_no), (room_no), 0x006B0A20);
};
DECOMP_GLOBAL(cRoomData, RoomData, 0x00C67560);

// Room save record as GetEtcFlgPtr sees it: the 64 etc flag words at 0x28.
struct EtcRoomSave {
    u8 pad_0[0x28];
    u16 etcFlag[0x40];   // 0x28
};

// Additive ambient colour per etc kind (EtcSetAddAmb), one table per ambient type.
struct EtcAmbRgb {
    u8 r;
    u8 g;
    u8 b;
};
DECOMP_GLOBAL_ARRAY(EtcAmbRgb, etc_day_rgb, 17, 0x00C038A8);
DECOMP_GLOBAL_ARRAY(EtcAmbRgb, etc_night_rgb, 17, 0x00C038DC);
DECOMP_GLOBAL_ARRAY(EtcAmbRgb, etc_day2_rgb, 17, 0x00C03910);
DECOMP_GLOBAL_ARRAY(EtcAmbRgb, etc_day3_rgb, 17, 0x00C03944);
DECOMP_GLOBAL_ARRAY(EtcAmbRgb, etc_day4_rgb, 17, 0x00C03978);

// Effect system (game/eff_sys.cpp).
DECOMP_FUNC(int, EspDataLoad, (u32 eff_addr, u32 owner, int MultipleOK), 0x005AE6A0);
DECOMP_FUNC(int, EspGetEfmTplAddr, (int id, void** ppTpl), 0x005AD960);

// The etc object classes: only the members the etc list calls.
typedef cModel cUnit;
class cObj : public cModel {
};

class cEmBox : public cEm {
public:
    DECOMP_METHOD(void, cEmBox, setEff, (u8 eff_id), (eff_id), 0x005C2BE0);
    DECOMP_METHOD(void, cEmBox, setBreakModel, (void* bin, void* tpl), (bin, tpl), 0x005C2F30);
};
class cEmDoor : public cEm {
public:
    DECOMP_METHOD(void, cEmDoor, setEff, (u8 eff_id), (eff_id), 0x005C61F0);
    DECOMP_METHOD0(void, cEmDoor, setYarare, 0x005C6210);
    DECOMP_METHOD(void, cEmDoor, setChain, (void* bin, void* tpl), (bin, tpl), 0x005C5E40);
};
class cEmRack : public cEm {
public:
    DECOMP_METHOD(void, cEmRack, setEff, (u8 eff_id), (eff_id), 0x005D4770);
};
class cEmTorch : public cEm {
public:
    DECOMP_METHOD(void, cEmTorch, setEff, (u8 eff_id), (eff_id), 0x005DFA10);
};
class cEmBarred : public cEm {
public:
    DECOMP_METHOD(void, cEmBarred, setEff, (u8 eff_id), (eff_id), 0x005BEC20);
};
class cEmBarrel : public cEm {
public:
    DECOMP_METHOD(void, cEmBarrel, setEff, (u8 eff_id), (eff_id), 0x005C0FB0);
};
class cEmBar : public cEm {
public:
    DECOMP_METHOD(void, cEmBar, setEff, (u8 eff_id), (eff_id), 0x006F92D0);
    DECOMP_METHOD(void, cEmBar, setMotion, (void* mot), (mot), 0x006F92F0);
};
class cEmSwitch : public cEm {
};
class cEmItem : public cEm {
public:
    DECOMP_METHOD(void, cEmItem, setEff, (u8 eff_id), (eff_id), 0x005CEAE0);
};
class cEmWindow : public cEm {
};

// obj13.cpp's ladder object; only the two setters the etc list calls.
class cObjLadder : public cObj {
public:
    DECOMP_METHOD(void, cObjLadder, setLadderInfo, (int num, u8 type), (num, type), 0x0071B180);
    DECOMP_METHOD(void, cObjLadder, setMotion, (void** tbl), (tbl), 0x0071BA60);
};

DECOMP_FUNC(cEmBox*, SetBox, (void* bin, void* tpl, Vec* pos, Vec* rot, u8 type, int etcNo), 0x005C3230);
DECOMP_FUNC(cEmDoor*, SetDoor, (void* bin, void* tpl, Vec* pos, Vec* rot, int type, int flagNo), 0x005CD100);
DECOMP_FUNC(cEmRack*, SetRack, (void* bin, void* tpl, Vec* pos, Vec* rot, u8 type, int etcNo), 0x005D53A0);
DECOMP_FUNC(cEmTorch*, SetTorch, (void* bin, void* tpl, Vec* pos, Vec* rot, int type, int etcNo), 0x005DFE30);
DECOMP_FUNC(cEmBarred*, SetEmBarred, (void* bin, void* tpl, Vec* pos, Vec* rot, int flagNo, int type), 0x005C00D0);
DECOMP_FUNC(cEmBarrel*, SetBarrel, (void* bin, void* tpl, Vec* pos, Vec* rot, u8 type, int etcNo), 0x005C2280);
DECOMP_FUNC(cEmBar*, SetBar, (void* bin, void* tpl, Vec* pos, Vec* rot, int flagNo), 0x006F9950);
DECOMP_FUNC(cEmSwitch*, SetEmSwitch, (void* bin, void* tpl, Vec* pos, Vec* rot, int flagNo), 0x005DEBC0);
DECOMP_FUNC(cEmItem*, SetEmItem, (void* bin, void* tpl, Vec* pos, Vec* rot, int type, int etcNo), 0x005CF160);
DECOMP_FUNC(cObj*, SetLadder, (void* bin, void* tpl, Vec* pos, Vec* rot, int no), 0x0071E2B0);

// One slot of the room etc table (12 bytes, g_EtcTbl at 0x00C58990). The
// Steam build keeps out-of-line copies of the accessors; RegistData is
// called, the others are inlined at every use.
class cEtcTbl {
public:
    int stat;            // 0x00  1: a model is registered
    EtcSetData* pData;   // 0x04  its etc list record
    cModel* m_pPtr;      // 0x08  the model

    void Init();
    int IsUse();
    cModel* GetPtr();
    EtcSetData* GetData();
    int RegistData(EtcSetData* d, cModel* model);
};

DECOMP_GLOBAL(void*, g_addr, 0x00C587E4);        // room etc archive (EtcModelDataLoad)
DECOMP_GLOBAL(u32, g_LastNo, 0x00C587E0);        // highest etc slot the room's list used
DECOMP_GLOBAL_ARRAY(cEtcTbl, g_EtcTbl, 0x40, 0x00C58990);
DECOMP_GLOBAL_ARRAY(u32, g_Etc_das_addr, 0x6A, 0x00C587E8);

// Room etc archive: file count, then the files from 0x20; each file is
// `size` bytes to the next header, name at 0x20, data at 0x40.
struct EtcArcFile {
    u32 size;         // 0x00
    u8 pad_4[0x20 - 0x4];
    char name[0x20];  // 0x20
    u8 data[1];       // 0x40
};
struct EtcArc {
    u32 num;          // 0x00
    u8 pad_4[0x20 - 0x4];
    EtcArcFile file[1];  // 0x20
};

// Room etc list (EtcModelListSet): count, then the 0x28-byte records from 0x10.
struct EtcList {
    u16 num;          // 0x00
    u8 pad_2[0x10 - 0x2];
    EtcSetData data[1];  // 0x10
};

// game/et00.cpp: the window models (one 0x40-byte wrapper each from 0x00617100).
DECOMP_FUNC(int, Et00_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617100);
DECOMP_FUNC(int, Et07_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617140);
DECOMP_FUNC(int, Et1d_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617180);
DECOMP_FUNC(int, Et25_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006171C0);
DECOMP_FUNC(int, Et29_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617200);
DECOMP_FUNC(int, Et2c_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617240);
DECOMP_FUNC(int, Et35_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617280);
DECOMP_FUNC(int, Et36_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006172C0);
DECOMP_FUNC(int, Et44_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617300);
DECOMP_FUNC(int, Et48_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617340);
DECOMP_FUNC(int, Et4a_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617380);
DECOMP_FUNC(int, Et50_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006173C0);
DECOMP_FUNC(int, Et51_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617400);
DECOMP_FUNC(int, Et52_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617440);
DECOMP_FUNC(int, Et53_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617480);
DECOMP_FUNC(int, Et54_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006174C0);
DECOMP_FUNC(int, Et55_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617500);
DECOMP_FUNC(int, Et56_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617540);
DECOMP_FUNC(int, Et57_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617580);
DECOMP_FUNC(int, Et58_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006175C0);
DECOMP_FUNC(int, Et5a_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617600);
DECOMP_FUNC(int, Et5b_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617640);
DECOMP_FUNC(int, Et5c_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617680);
DECOMP_FUNC(int, Et5d_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006176C0);
DECOMP_FUNC(int, Et5e_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617700);
DECOMP_FUNC(int, Et5f_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617740);
DECOMP_FUNC(int, Et60_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617780);
DECOMP_FUNC(int, Et64_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x006177C0);
DECOMP_FUNC(int, Et65_init, (void* arc, EtcSetData* d, cModel** out, int flag), 0x00617800);

#endif
