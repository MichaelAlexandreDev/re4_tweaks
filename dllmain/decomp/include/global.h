#ifndef GLOBAL_H
#define GLOBAL_H

#include "bind.h"
#include "types.h"

// EMI points of the room (enemy info: hide / goto / route spots); Steam
// layout from re4_tweaks SDK EMINFO_WK / EMINFO_DATA, GameCube names.
struct EmiEntry {
    u8 type;              // 0x00
    u8 sub;               // 0x01
    u8 state;             // 0x02
    u8 pad_3;
    Vec pos;              // 0x04
    f32 rotY;             // 0x10
    u8 pad_14[0x40 - 0x14];
};
struct EmiData {
    u32 n;                // 0x0
    u32 id;               // 0x4
    EmiEntry entry[256];  // 0x8
};

struct EmSeInfo {
    Vec pos;              // 0x00
    u8 type;              // 0x0C  2 = bell rung
    u8 pad_0D[3];
};

// Flag numbers (GameCube include/global.h; bit `n` is 0x80000000 >> (n & 31)
// of word n >> 5, which the PC kept: SYS_OMAKE_* test 0xC0000000).
enum DBG_FLAG {
    DBG_TEST_MODE = 0,
    DBG_SCR_TEST = 1,
    DBG_BACK_CLIP = 2,
    DBG_DBG_CAM = 3,
    DBG_SAT_DISP = 4,
    DBG_EAT_DISP = 5,
    DBG_EVENT_TOOL = 6,
    DBG_SLOW_ON = 7,
    DBG_SHADOW_POLYGON = 8,
    DBG_SCE_AT_DISP = 9,
    DBG_SCR2_TEST = 10,
    DBG_SHADOW_FRAME = 11,
    DBG_MIRROR_POLYGON = 12,
    DBG_GROUND_DISP = 13,
    DBG_SKELETON_DISP = 14,
    DBG_ESPTOOL_ONSCR = 15,
    DBG_CINESCO_OFF = 16,
    DBG_RTP_DISP = 17,
    DBG_WIRE_DISP = 18,
    DBG_YARARE_DISP = 19,
    DBG_CAM_AREA_OFF = 20,
    DBG_CLOTH_AT_DISP = 21,
    DBG_WIND_ON = 22,
    DBG_ESPTOOL_MEM_USE = 23,
    DBG_TEX_RENDER_ALL = 24,
    DBG_ESPTOOL_ONEM = 25,
    DBG_EMINFO_DISP = 26,
    DBG_LIGHT_TOOL = 27,
    DBG_EM_COUNT = 28,
    DBG_1d = 29,
    DBG_EM3F_ADA_SADLER = 30,
    DBG_1f = 31,
    DBG_COCKPIT_TOOL = 32,
    DBG_BOUNDING_DISP = 33,
    DBG_ADJUST_CAM = 34,
    DBG_FLAT_FLOOR = 35,
    DBG_OBJ_SKELETON = 36,
    DBG_DRAW_SH_TEX = 37,
    DBG_EM_NO_ATK = 38,
    DBG_NO_EST_CALL = 39,
    DBG_IN_ESP_TOOL = 40,
    DBG_TERM_TOOL = 41,
    DBG_WARN_LEVEL_LOW = 42,
    DBG_ID_TOOL = 43,
    DBG_TITLE_CHECK = 44,
    DBG_CAST_ERR_NO_DISP = 45,
    DBG_EMW_ERR_NO_DISP = 46,
    DBG_TIMER_STOP = 47,
    DBG_SCREEN_SHOT = 48,
    DBG_SERIES_SHOT = 49,
    DBG_ASHLEY_NO_CATCH = 50,
    DBG_33 = 51,
    DBG_34 = 52,
    DBG_35 = 53,
    DBG_36 = 54,
    DBG_37 = 55,
    DBG_38 = 56,
    DBG_39 = 57,
    DBG_3a = 58,
    DBG_3b = 59,
    DBG_3c = 60,
    DBG_3d = 61,
    DBG_3e = 62,
    DBG_3f = 63,
    DBG_ROOMJMP = 64,
    DBG_PROC_BAR = 65,
    DBG_SCA_VIEW = 66,
    DBG_OBA_VIEW = 67,
    DBG_SLOW_MODE = 68,
    DBG_NO_SCE_EXE = 69,
    DBG_SINGLE_DISK = 70,
    DBG_BUGCHECK_MODE = 71,
    DBG_NO_DEATH = 72,
    DBG_INF_BULLET = 73,
    DBG_NO_ENEMY = 74,
    DBG_BGM_STOP = 75,
    DBG_SE_STOP = 76,
    DBG_PL_LOCK_FOLLOW = 77,
    DBG_EM_NO_DEATH = 78,
    DBG_KAIOUKEN = 79,
    DBG_PAD_INFO = 80,
    DBG_UNDER_CONST = 81,
    DBG_EM_WEAK = 82,
    DBG_EM_LIFE_DISP = 83,
    DBG_SHADOW_LIGHT = 84,
    DBG_CAPTION_OFF = 85,
    DBG_TEST_MODE_CK = 86,
    DBG_LIGHT_ERR_CHECK = 87,
    DBG_EST_CALL_CHK = 88,
    DBG_GX_WARN_ALL = 89,
    DBG_GX_WARN_MIDIUM = 90,
    DBG_GX_WARN_SEVERE = 91,
    DBG_PL_NOHIT = 92,
    DBG_SE_ERR_ALL = 93,
    DBG_QUICK_EM_STOP = 94,
    DBG_AV_TEST = 95,
    DBG_INF_BULLET2 = 96,
    DBG_BATTLE_CAM = 97,
    DBG_PL_KLAUSER = 98,
    DBG_MAP_ZOOM_ORG = 99,
    DBG_SCISSOR_OFF = 100,
    DBG_LOG_OFF = 101,
    DBG_SCR_CHECK = 102,
    DBG_OBJ_SERVER = 103,
    DBG_START_ST2 = 104,
    DBG_ERROR_CK = 105,
    DBG_APP_USE_DBMEM = 106,
    DBG_REFRACT_CK = 107,
    DBG_EM_NO_DIE_FLAG = 108,
    DBG_START_ST3 = 109,
    DBG_START_LAST = 110,
    DBG_DOOR_SET_MODE = 111,
    DBG_EFF_NUM_DISP = 112,
    DBG_SET_HITMARK_ALL = 113,
    DBG_FOG_FAR_GREEN = 114,
    DBG_SSCRN_PROC_BAR = 115,
    DBG_NO_ETC_SET = 116,
    DBG_NO_DEATH2 = 117,
    DBG_NO_PARASITE = 118,
    DBG_ESP_CHK = 119,
    DBG_NO_EVENT = 120,
    DBG_NO_LASER_LINE = 121,
    DBG_DATA_113 = 122,
    DBG_SHOP_FULL = 123,
    DBG_ADA_OMAKE_EV = 124,
    DBG_FOG_FAR_COLOR = 125,
    DBG_TOOL_EMINFO_DISP = 126,
    DBG_CHAR_MACHINE = 127,
};

enum STA_FLAG {
    STA_BG_OFF = 0,
    STA_PL_CHECK = 1,
    STA_PL_CHECK2 = 2,
    STA_MOVIE_ON = 3,
    STA_CUTCHG = 4,
    STA_MOVIE2_ON = 5,
    STA_SSCRN_ENABLE = 6,
    STA_CINESCO = 7,
    STA_PL_FIRE = 8,
    STA_09 = 9,
    STA_ACT_DONT_FIRE = 10,
    STA_DIEDEMO = 11,
    STA_BLUR = 12,
    STA_SUB_SCRN = 13,
    STA_CARD_ACCESS = 14,
    STA_PAD_SENSITIVE = 15,
    STA_LOOK_THROUGH = 16,
    STA_PL_ACTION = 17,
    STA_PL_INVISIBLE = 18,
    STA_EVENT = 19,
    STA_ASHLEY_HIDE = 20,
    STA_BINOCULAR = 21,
    STA_WATER_ALIVE = 22,
    STA_CAMERA = 23,
    STA_BLACKOUT = 24,
    STA_SCOPE_CAMERA = 25,
    STA_RIDE_GONDOLA = 26,
    STA_WIND_ON = 27,
    STA_PL_JUMP_OFF = 28,
    STA_MIRROR = 29,
    STA_SAND_ALIVE = 30,
    STA_SELF_SHADOW = 31,
    STA_PL_SE_FOOT = 32,
    STA_PL_SE_WHISTLE = 33,
    STA_SE_BURST = 34,
    STA_SUSPEND = 35,
    STA_TEX_RENDER = 36,
    STA_THERMO_GRAPH = 37,
    STA_CAMERA_IN_ROOM = 38,
    STA_NO_LIGHTMASK = 39,
    STA_PL_SPEAR_SET = 40,
    STA_PL_SWIM = 41,
    STA_PL_BOAT = 42,
    STA_WATER_CAMERA = 43,
    STA_PL_SWIM_CAMERA = 44,
    STA_PL_LADDER = 45,
    STA_CRITICAL = 46,
    STA_TAKEAWAY = 47,
    STA_PL_CATCHED = 48,
    STA_SHADOW_EQCOL = 49,
    STA_PL_CATCHHOLD = 50,
    STA_NEARCLIP_TOUCH = 51,
    STA_CAMERA_SET_ROOM = 52,
    STA_ROOM_RAIN = 53,
    STA_USE_CAST_SHADOW = 54,
    STA_PROC_SHD_TEX = 55,
    STA_ALPHA_DRAW2 = 56,
    STA_SET_BG_COLOR = 57,
    STA_ESPGEN45_SET = 58,
    STA_EFFEM2D_TEXRND = 59,
    STA_SUB_LADDER = 60,
    STA_SUBCHAR_CTRL = 61,
    STA_ITEM_GET = 62,
    STA_LASERSITE_NOADD = 63,
    STA_PL_DONT_FIRE = 64,
    STA_PL_EM_ACTION = 65,
    STA_SUB_CATCHED = 66,
    STA_CUT_CHANGE = 67,
    STA_NO_FENCE = 68,
    STA_SSCRN_REQUEST = 69,
    STA_ESP_COMPULSION_NOSUSPEND = 70,
    STA_PL_MISS_SHOT = 71,
    STA_SUB_BULLDOZER = 72,
    STA_LIT_NO_UPDATE = 73,
    STA_MAP_DISABLE = 74,
    STA_USE_SHADOW_LIGHT = 75,
    STA_EVENT_SYSYTEM = 76,
    STA_INTO_SHOP = 77,
    STA_TIMER_NO_PAUSE = 78,
    STA_EFFAREA_USE_CAM = 79,
    STA_TITLE = 80,
    STA_51 = 81, // PS2: STA_MAP_CK_UNLIMIT, unused in GC?
    STA_52 = 82, // PS2: STA_RIFLE_READY
    STA_53 = 83, // PS2: STA_LOWPOLY_USE
    STA_54 = 84, // PS2: STA_TERMINAL
    STA_55 = 85, // PS2: STA_HIGH_TEMPERATURE_THERMO
    STA_56 = 86,
    STA_57 = 87,
    STA_58 = 88,
    STA_59 = 89,
    STA_5a = 90,
    STA_5b = 91,
    STA_5c = 92,
    STA_5d = 93,
    STA_5e = 94,
    STA_5f = 95,
    STA_SAVEDATA_NO_UPDATE = 96,
    STA_BEHIND_CAM = 97,
    STA_62 = 98,
    STA_SCISSOR = 99,
    STA_SLOW = 100,
    STA_SUB_ASHLEY = 101,
    STA_BIG_MARKER = 102,
    STA_EVENT_CANCEL = 103,
    STA_KLAUSER_TRANSFORM = 104,
    STA_69 = 105, // PS2: STA_ELEVATOR
    STA_6a = 106,
    STA_6b = 107,
    STA_6c = 108,
    STA_6d = 109,
    STA_6e = 110,
    STA_6f = 111,
    STA_70 = 112, // PS2: STA_DIV_EVENT_SET
    STA_71 = 113, // PS2: STA_DIV_EVENT_ING
    STA_72 = 114, // PS2: STA_NOW_LOADING
    STA_73 = 115, // PS2: STA_SCREEN_STOP_RESERVE
    STA_74 = 116, // PS2: STA_TYPEWRITER
    STA_75 = 117,
    STA_76 = 118,
    STA_77 = 119,
    STA_78 = 120,
    STA_79 = 121,
    STA_7a = 122,
    STA_7b = 123,
    STA_7c = 124,
    STA_7d = 125,
    STA_7e = 126,
    STA_7f = 127,
};

enum SYS_FLAG {
    SYS_OMAKE_ADA_GAME = 0,
    SYS_OMAKE_ETC_GAME = 1,
    SYS_EXCEPTION = 2,
    SYS_RENDER_END = 3,
    SYS_SP_USED = 4,
    SYS_SOFT_RESET = 5,
    SYS_DATA_READ = 6,
    SYS_ROOMJUMP = 7,
    SYS_INVISIBLE = 8,
    SYS_DOOR_AFTER = 9,
    SYS_DOORDEMO = 10,
    SYS_TRANS_STOP = 11,
    SYS_CONTINUE = 12,
    SYS_SET_BLACK = 13,
    SYS_SN_PC_READ = 14,
    SYS_SN_PC_READ_TOOL = 15,
    SYS_HARD_RESET = 16,
    SYS_SCREEN_SHOT = 17,
    SYS_NEW_GAME = 18,
    SYS_TYPEWRITER = 19,
    SYS_SCISSOR_ON = 20,
    SYS_SCREEN_STOP = 21,
    SYS_CARD_ACCESS = 22,
    SYS_LOAD_GAME = 23,
    SYS_CONTINUE_AFTER = 24,
    SYS_START_EVT_SKIP = 25,
    SYS_HARD_MODE = 26,
    SYS_MESSAGE_INIT = 27,
    SYS_PUBLICITY_VER = 28,
    SYS_1d = 29, // PS2: SYS_SAVEDATA_EXIST
    SYS_1e = 30, // PS2: SYS_PAL
    SYS_1f = 31, // PS2: SYS_DTV480P
};

#define FlagChk(base, no) (*(u32*) ((((no) >> 5) << 2) + (u32) (base)) & (0x80000000 >> ((no) & 31)))
#define FlagOn(base, no) (*(u32*) ((((no) >> 5) << 2) + (u32) (base)) |= (0x80000000 >> ((no) & 31)))
#define FlagOff(base, no) (*(u32*) ((((no) >> 5) << 2) + (u32) (base)) &= ~(0x80000000 >> ((no) & 31)))
#define FlagChkSignW(flg, no) ((s32) ((flg) << ((no) & 31)) < 0)
#define StaFlagChk(g, n) FlagChk(&(g)->Status_flg, n)
#define StaFlagOn(g, n) FlagOn(&(g)->Status_flg, n)
#define StaFlagOff(g, n) FlagOff(&(g)->Status_flg, n)
#define SysFlagChk(g, n) FlagChk(&(g)->System_flg, n)
// The PC release build has no debug flag checks (em10_R1_Wait: none).
#define DbgFlagChk(g, n) 0
#define DbgFlagOff(g, n) ((void) 0)

// Partial GLOBAL_WK of the Steam build. Only fields with a known offset are
// named; names follow re4_tweaks/dllmain/SDK/global.h.
struct GLOBAL_WK {
    u8 pad_0000[0x20];
    u8 Rno0;                   // 0x20  main routine; 2 = in game
    u8 pad_0021[0x40 - 0x21];
    void* pRoom;               // 0x40  room archive
    void* pWep;                // 0x44
    void* pCore;               // 0x48  current archive (offsets to its sub-files)
    void* pOption;             // 0x4C
    struct PlArc* pPlayer;     // 0x50  player archive
    union {
        u32 flags_SYSTEM[1];   // 0x54  SYS_PS2_ADA_GAME = 0x1000 (Separate Ways)
        u32 System_flg;        // 0x54  (GameCube name)
    };
    u32 Disp_flg;              // 0x58  display flags
    u32 curGameTime;           // 0x5C
    u32 Debug_flg[4];          // 0x60
    f32 deltaTime;             // 0x70  frame time scale of the PC port (timers add deltaTime - 1)
    u8 Camera[0x170 - 0x74];   // 0x74  CAMERA
    u32 Stop_flg;              // 0x170
    u32 Room_flg[4];           // 0x174
    u8 pad_0184[0x4F30 - 0x184];
    EmiData* pEmi;             // 0x4F30
    u8 pad_4F34[0x4F3C - 0x4F34];
    EmSeInfo SeInfo;           // 0x4F3C  last noise enemies react to
    u8 pad_4F4C[0x4F98 - 0x4F4C];
    u8 Game_level;             // 0x4F98  adaptive difficulty rank (GameCube 0x4F88)
    u8 pad_4F99[0x4FAC - 0x4F99];
    union {
        u16 room_id;           // 0x4FAC  stage << 8 | room
        u16 curRoomId;
        struct {
            u8 room_no;        // 0x4FAC  (little endian: the low byte)
            u8 stage_no;       // 0x4FAD
        };
    };
    u8 pad_4FAE[0x4FB4 - 0x4FAE];
    s16 pl_life;               // 0x4FB4  (GameCube 0x4FA4)
    s16 pl_life_max;           // 0x4FB6
    s16 ashley_life;           // 0x4FB8
    s16 ashley_life_max;       // 0x4FBA
    u8 pad_4FBC[0x4FC0 - 0x4FBC];
    u8 weapon_no;              // 0x4FC0
    u8 weapon_type;            // 0x4FC1
    u8 bullet_type;            // 0x4FC2
    u8 weapon_lv_power;        // 0x4FC3
    u8 pad_4FC4[0x4FC8 - 0x4FC4];
    u8 pl_type;                // 0x4FC8  0 Leon, 1 Ashley, 2 Ada, 3 HUNK, 4 Krauser, 5 Wesker
    u8 pad_4FC9[0x4FCB - 0x4FC9];
    u8 subCostume;             // 0x4FCB  Ashley: 0 Normal, 1 Popstar, 2 Armor
    u8 pad_4FCC[0x501C - 0x4FCC];
    u32 Status_flg[4];         // 0x501C  (GameCube 0x500C)
    u8 pad_502C[0x530C - 0x502C];
    u32 Frame_cnt;             // 0x530C  frame counter (re4_tweaks SDK frameCounter_530C)
    u8 pad_5310[0x8690 - 0x5310];
};
typedef char decomp_check_GLOBAL_WK_size[sizeof(GLOBAL_WK) == 0x8690 ? 1 : -1];

#define SYS_PS2_ADA_GAME 0x1000

// As in references/re4-gamecube/include/global.h; using the helper instead of
// `f &= ~b` changes how MSVC schedules the surrounding stores.
static inline void BitOff16(u16& f, u16 b) { f &= ~b; }

DECOMP_GLOBAL(GLOBAL_WK*, pG, 0x00C06F3C);

#endif
