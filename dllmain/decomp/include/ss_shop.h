#ifndef SS_SHOP_H
#define SS_SHOP_H

#include "bind.h"
#include "dvd.h"
#include "em.h"
#include "mem.h"
#include "snd.h"
#include "types.h"
#include "math_sub.h"
#include "sscrn.h"

// Merchant shop screen of the sub screen (Sscrn/ss_shop.cpp). Layouts are the
// Steam ones: offsets agree with re4_tweaks/dllmain/SDK (sscrn.h, ID.h,
// message.h, cManager.h) and with the shop functions' assembly; names follow
// the GameCube decompilation (references/re4-gamecube/include).

#define DECOMP_SHOP_CHECK(type, member, offset) \
    typedef char decomp_check_##type##_##member[((u32) & ((type*) 0)->member == (offset)) ? 1 : -1]

class pzlPlayer;
class Merchant;
struct ShopWork;

// Sub screen data archive: byte offsets of its sub-files.
// SsArc, SS_ARC_PTR and SUB_SCREEN: sscrn.h (shared by every sub screen).
DECOMP_SHOP_CHECK(SUB_SCREEN, puzzlePlayer, 0x2AC);
DECOMP_SHOP_CHECK(SUB_SCREEN, shop, 0x310);

// Item slot (cItem, 0xE bytes; re4_tweaks SDK item.h).
struct ItemWork {
    u16 id;                   // 0x0
    u16 num;                  // 0x2
    u8 be_flag;               // 0x4
    u8 type;                  // 0x5
    union {
        u16 lv;               // 0x6  tune levels, one nibble each
        s8 lv8[2];
        struct {
            u16 fire : 4;     // PC order: fire, speed, reload, capacity
            u16 speed : 4;
            u16 reload : 4;
            u16 cap : 4;
        } tune;
    };
    union {
        u16 lv2;              // 0x8
        struct {
            u16 x8 : 3;
            u16 bullet : 13;  // loaded rounds
        } ammo;
    };
    s8 x;                     // 0xA
    s8 y;                     // 0xB
    s8 orient;                // 0xC
    u8 board;                 // 0xD

    DECOMP_METHOD0(bool, ItemWork, specialTuned, 0x007723F0);
};
typedef char decomp_check_ItemWork_size[sizeof(ItemWork) == 0xE ? 1 : -1];

// Shop screen state (SUB_SCREEN::shop, 0x48 bytes).
struct ShopWork {
    int num;         // 0x00  entries of the current list
    int top;         // 0x04  first entry shown
    int cursor;      // 0x08
    int count;       // 0x0C  pieces to sell / buy
    u16 buyId;       // 0x10
    ItemWork buy;    // 0x12  slot template of the item being bought (case placement)
    int placed;      // 0x20  the bought piece was put on the case
    ItemWork* item;  // 0x24  item being sold / tuned
    int price;       // 0x28  tune-up price
    int lvType;      // 0x2C  tune type (0 fire, 1 magazine, 2 speed, 3 exclusive, 4 all)
    int lv[4];       // 0x30  tune levels after the purchase
    int noRoom;      // 0x40  the bought piece did not fit
    u8 coat;         // 0x44  the merchant's coat is open (bool on PC)
    u8 pad_45[3];
};
typedef char decomp_check_ShopWork_size[sizeof(ShopWork) == 0x48 ? 1 : -1];

// ID_UNIT of the Steam build (0x144 bytes).
struct IdUnit {
    u8 be_flag;               // 0x000  8: visible
    u8 unitNo;                // 0x001
    u8 classNo;               // 0x002
    u8 markNo;                // 0x003
    u8 pad_004[4];
    Mtx mat;                  // 0x008
    Mtx l_mat;                // 0x038
    IdUnit* pParent;          // 0x068
    u8 pad_06C[0x7A - 0x6C];
    u8 texNo;                 // 0x07A
    u8 pad_07B[0x8A - 0x7B];
    u8 rev_flag;              // 0x08A
    u8 tex_flag;              // 0x08B
    u8 pad_08C[0x91 - 0x8C];
    u8 anima_state;           // 0x091
    u8 pad_092[2];
    Vec pos0;                 // 0x094
    Vec pos;                  // 0x0A0
    u8 pad_0AC[0xEC - 0xAC];
    u8 col0[4];               // 0x0EC
    u8 pad_0F0[0x144 - 0xF0];
};
typedef char decomp_check_IdUnit_size[sizeof(IdUnit) == 0x144 ? 1 : -1];
DECOMP_SHOP_CHECK(IdUnit, pos0, 0x94);

enum {
    IDC_SSCRN_PESETA = 0x2,
    IDC_SSCRN_NEAR_0 = 0x10,
    IDC_SSCRN_0 = 0x14,
    IDC_SSCRN_CKPT_0 = 0x1C,
    IDC_SSCRN_CKPT_1 = 0x1D,
    IDC_SSCRN_CKPT_2 = 0x1E,
    IDC_SSCRN_CKPT_3 = 0x1F,
};

class IdSystem {
public:
    DECOMP_METHOD(void, IdSystem, kill, (u8 markNo, int classNo), (markNo, classNo), 0x00641A30);
    DECOMP_METHOD(IdUnit*, IdSystem, unitPtr, (u8 markNo, int classNo), (markNo, classNo), 0x00643580);
};
DECOMP_GLOBAL(IdSystem, IdSub, 0x00C7AB28);

// cManager<T> (0x1C bytes; re4_tweaks SDK cManager.h): work array of fixed-size blocks. The PC
// inline helpers lost the GameCube logging, arrayFree and the peak count.
template <class T>
class cManager {
public:
    virtual ~cManager();
    virtual void* memAlloc(u32 size);
    virtual void memFree(void* p);
    virtual void memClear(T* p, u32 size);
    virtual void log(const char* fmt, ...);
    virtual void destroy(T* p);
    virtual int construct(T* p, u32 id);

    T* m_pArray;              // 0x04
    u32 m_nArray;             // 0x08
    u32 m_blockSize;          // 0x0C
    u8 m_DieTimelag;          // 0x10
    T* m_pAlive;              // 0x14  head of the active list (cUnit::pNext)
    T* m_pAliveBack;          // 0x18

    T* getWork(u32 no) { return no >= m_nArray ? 0 : (T*) ((u8*) m_pArray + no * m_blockSize); }
    void roomInit() {
        m_pArray = 0;
        m_nArray = 0;
        m_pAlive = 0;
    }
    void arrayAlloc(u32 n) {
        m_pArray = (T*) memAlloc(m_blockSize * n);
        m_nArray = n;
        if (n) {
            memClear(m_pArray, m_blockSize * n);
        }
    }
    void addListFront(T* p) {
        T* q;
        for (q = m_pAlive; q; q = (T*) q->pNext) {
            if (q == p) {
                return;
            }
        }
        p->pNext = m_pAlive;
        m_pAlive = p;
    }
    T* create(int id, u32 no) {
        T* p;
        if (no >= m_nArray) {
            return 0;
        }
        p = (T*) ((u8*) m_pArray + m_blockSize * no);
        if (p->be_flag & 0x601) {
            return 0;
        }
        memClear(p, m_blockSize);
        construct(p, id);
        addListFront(p);
        return p;
    }
};

// Map / item model (cMap derives from cModel; LightInfo is the cLightInfo at 0x164).
class cLightInfo;
class cMap : public cModel {
public:
    DECOMP_METHOD(void, cMap, modelInit, (void* mdl, void* tex), (mdl, tex), 0x0065EBA0);
    cLightInfo* lightInfo() { return (cLightInfo*) LightInfo; }
};
DECOMP_GLOBAL(cManager<cMap>, MapMgr, 0x00C16320);
class cModelInfo;
// Sub screen model info / parts managers and the model's manager pointers (cModel::mm / pm).
DECOMP_GLOBAL(cManager<cModelInfo>, ssModInfoMgr, 0x00C7B5C8);
DECOMP_GLOBAL(cManager<cModel>, ssPartsMgr, 0x00C7B5E8);
DECOMP_GLOBAL(void*, cModel_mm, 0x00C07110);
DECOMP_GLOBAL(void*, cModel_pm, 0x00C07114);

// Message (0x100 bytes) of MessageControl.
struct MesQue;
struct Message {
    u8 pad_00[0x10];
    u32 m_state;              // 0x010  2: the text is shown completely
    u8 pad_14[0x20 - 0x14];
    u16 m_ot_type;            // 0x020
    u16 m_ot_no;              // 0x022
    u8 pad_24[0xD0 - 0x24];
    MesQue* qbase;            // 0x0D0
    u8 pad_D4[0xF9 - 0xD4];
    s8 m_sel;                 // 0x0F9  answer of a yes/no message (1 yes, 2 no)
    u8 m_cur;                 // 0x0FA  selection cursor
    u8 pad_FB[0x100 - 0xFB];
};

class MessageControl {
public:
    void* vtbl;               // 0x0000
    u8 pad_4[4];
    Message m_Msg[16];        // 0x0008
    DECOMP_METHOD(int, MessageControl, MesSet, (u32 id, int x, int y, u32 attr, int wk, int col, int a6),
                  (id, x, y, attr, wk, col, a6), 0x00718DE0);
    DECOMP_METHOD(void, MessageControl, Delete, (int wk), (wk), 0x00716F60);
    DECOMP_METHOD(void, MessageControl, setLayout, (int wk, int type), (wk, type), 0x00718900);
    Message* getMes(int no) { return &m_Msg[no]; }
};
DECOMP_GLOBAL(MessageControl, cMes, 0x00C17830);

// Sound streams (GameCube SndStrReq overloads: stop / play).
DECOMP_FUNC(u32, SndStrStop, (u32 id, int mode, int a2, int a3), 0x0096EA40);
DECOMP_FUNC(u32, SndStrReq, (int blk, int no, int mode, int a3, int a4, f32 vol), 0x0096FB40);

// pSys (SYSTEM_SAVE_WORK, language at +0x8) is declared in global.h.
// PC-only byte flag set while the shop runs (also cleared by MessageControl code at 0x00716F60).
DECOMP_GLOBAL(u8, MesPcFlag, 0x00C06F39);
// PC: one x position for the sell / buy / tune confirm messages (GameCube sell_msg_x..lvup_msg_x).
DECOMP_GLOBAL(int, shop_msg_x, 0x00C1D2F0);
DECOMP_GLOBAL(int, shop_msg_y, 0x00C1D2F4);
#define LAYOUT_SUBSCRN 2
#define LAYOUT_SHOP_LIST 6

typedef unsigned __int64 u64;

// Pad state (PC: 64-bit KEY_BTN masks; re4_tweaks SDK pad.h).
struct KeyWork {
    u64 on;                   // 0x00  held
    u64 trg;                  // 0x08  pressed this frame
    u64 x10;
    u64 rep;                  // 0x18  auto-repeat
};
DECOMP_GLOBAL(KeyWork, Key, 0x00C62F08);
#define KEY_U 0x1000000ULL
#define KEY_R 0x4000000ULL
#define KEY_L 0x8000000ULL
#define KEY_D 0x2000000ULL
#define KEY_CANCEL 0x40000000ULL
#define KEY_OK 0x80000000ULL
#define KEY_MINUS 0x200000000000ULL

// Case puzzle player (pzlPlayer, 0x34 bytes; re4_tweaks SDK puzzle.h).
class pzlPiece {
public:
    u8 pad_00[0xC];
    f32 m_center_x;           // 0x0C
    f32 m_center_y;           // 0x10
    f32 m_pos_x;              // 0x14
    f32 m_pos_y;              // 0x18
    u8 pad_1C[0x24 - 0x1C];
    ItemWork* item;           // 0x24

    DECOMP_METHOD(void, pzlPiece, orientation, (int dir), (dir), 0x00689AA0);
};
class pzlBoard {
public:
    u8 pad_0[4];
    s8 m_size_x;              // 0x4
    s8 m_size_y;              // 0x5

    DECOMP_METHOD(bool, pzlBoard, search, (pzlPiece* p), (p), 0x006894E0);
    DECOMP_METHOD(void, pzlBoard, rmPiece, (pzlPiece* p), (p), 0x0068A760);
};
class pzlPlayer {
public:
    pzlBoard* m_board;        // 0x00
    pzlBoard* m_space;        // 0x04
    void* m_piece;            // 0x08
    u8 m_piece_max;           // 0x0C
    pzlPiece* m_inhand;       // 0x10
    pzlPiece* m_extra;        // 0x14
    u8 pad_18[0x30 - 0x18];
    pzlBoard* m_p_active_board; // 0x30  PC: board the cursor works on


    DECOMP_METHOD0(void, pzlPlayer, save, 0x00689810);
    DECOMP_METHOD0(void, pzlPlayer, rehash, 0x0068C3E0);
    DECOMP_METHOD0(void, pzlPlayer, inHandExtraPiece, 0x006899F0);
    DECOMP_METHOD(void, pzlPlayer, appendExtraPiece, (ItemWork* item), (item), 0x00689910);
    DECOMP_METHOD(bool, pzlPlayer, putPiece, (pzlBoard* board), (board), 0x0068B1A0);
    DECOMP_METHOD(void, pzlPlayer, getPiece, (pzlBoard* board), (board), 0x0068B130);
    DECOMP_METHOD0(u8, pzlPlayer, removeExtraPiece, 0x0068AD80);
};

// Merchant tables (game/merchant.cpp).
struct PriceEntry {
    u16 id;                   // 0x0
    u16 price;                // 0x2  price / 10
    u8 unit;                  // 0x4  pieces per purchase
    u8 pad_5;
};
struct MerchantData;
struct LevelEntry {
    u16 id;                   // 0x0  weapon item id
    u8 lv[4];                 // 0x2  max tune level per type
    u8 isNew;                 // 0x6
    u8 pad_7;
};
class MerchantCharacter {
public:
    void* m_p_info;           // 0x00
    MerchantData* m_p_data;   // 0x04
    PriceEntry* m_p_sell;     // 0x08
    PriceEntry* m_p_exer;     // 0x0C
    void* m_p_lvup;           // 0x10
};
DECOMP_GLOBAL(MerchantCharacter, merchantChar, 0x00C714D0);

// Shop session (PC layout: the lists are u16 arrays; exercise 0x318, selling 0x516, level 0x714).
class Merchant {
public:
    void* m_p_info;           // 0x000
    PriceEntry* m_p_sell;     // 0x004
    PriceEntry* m_p_exer;     // 0x008
    void* m_p_lvup;           // 0x00C
    u8 pad_010[0x314 - 0x10];
    u8 m_exer_tbl_num;        // 0x314
    u8 m_sell_tbl_num;        // 0x315
    u8 m_lvup_tbl_num;        // 0x316

    DECOMP_METHOD(void, Merchant, save, (MerchantData* p_data), (p_data), 0x00712D50);
    DECOMP_METHOD(s16, Merchant, stockNum, (u16 id), (id), 0x007130B0);
    DECOMP_METHOD(s8, Merchant, levelMax, (u16 id, int type), (id, type), 0x007136B0);
    DECOMP_METHOD(bool, Merchant, specialTunable, (ItemWork* p_item), (p_item), 0x007137D0);
    DECOMP_METHOD(bool, Merchant, stockNew, (u16 id), (id), 0x00713420);
    DECOMP_METHOD0(bool, Merchant, stockNew, 0x00713480);
    DECOMP_METHOD(bool, Merchant, levelNew, (u16 id), (id), 0x007135D0);
    DECOMP_METHOD0(bool, Merchant, levelNew, 0x00713600);
    DECOMP_METHOD0(u8, Merchant, sellingItemNum, 0x00713E30);
    DECOMP_METHOD(PriceEntry*, Merchant, sellingItemNo, (int no), (no), 0x00713E40);
    DECOMP_METHOD0(int, Merchant, exerciseItemNum, 0x00714240);
    DECOMP_METHOD(ItemWork*, Merchant, exerciseItemPtr, (int no), (no), 0x00714250);
    DECOMP_METHOD(int, Merchant, sellUnit, (u16 id), (id), 0x007146B0);
    DECOMP_METHOD(int, Merchant, sellPrice, (u16 id, int num), (id, num), 0x007143D0);
    DECOMP_METHOD(int, Merchant, sell, (u16 id, int num, int* pocket), (id, num, pocket), 0x00714700);
    DECOMP_METHOD0(int, Merchant, levelupItemNum, 0x00714A80);
    DECOMP_METHOD(ItemWork*, Merchant, levelupItemPtr, (int no), (no), 0x00714A90);
    DECOMP_METHOD0(void, Merchant, makeList, 0x007157F0);
    DECOMP_METHOD(LevelEntry*, Merchant, levelupItemNo, (int no), (no), 0x00715CE0);
    DECOMP_METHOD(bool, Merchant, tunable, (ItemWork* p_item), (p_item), 0x00713980);
    DECOMP_METHOD(int, Merchant, buyup, (ItemWork* p_item, int num, int* pocket), (p_item, num, pocket), 0x00715AB0);
    DECOMP_METHOD(int, Merchant, levelupPrice, (ItemWork* item, int type, int lv), (item, type, lv), 0x00714C60);
    DECOMP_METHOD(int, Merchant, buyupPrice, (ItemWork* item, int num), (item, num), 0x007158B0);
    DECOMP_METHOD(int, Merchant, buyupPrice, (u16 id, int num), (id, num), 0x007142E0);
    DECOMP_METHOD(PriceEntry*, Merchant, exerciseItemNo, (int no), (no), 0x00715830);
};

// Item table entry (ItemInfo, 4 bytes).
struct ItemInfo {
    u16 id;                   // 0x0
    u8 type;                  // 0x2  1 weapon
    u8 defNum;                // 0x3
    u16 extra;                // 0x4  written by itemInfo (dword store at +2)
};
// Weapon tune tables (game/item.cpp).
DECOMP_FUNC(f32, getPowerRatio, (u16 id, int lv), 0x007066C0);
DECOMP_FUNC(f32, getSpeedRatio, (u16 id, int lv), 0x007067C0);
DECOMP_FUNC(f32, getReloadRatio, (u16 id, int lv), 0x00706850);
DECOMP_FUNC(f32, getBulletRatio, (u16 id, int lv), 0x007068E0);
DECOMP_FUNC(u8, WeaponId2MaxLevel, (u16 id, int type), 0x00706620);
// PC-only: Separate Ways tune levels shown for the shop's weapons {id, level per type}.
struct AdaLevel {
    u16 id;
    s8 lv[4];
};
DECOMP_GLOBAL_ARRAY(AdaLevel, ada_level_tbl, 7, 0x00C16194);
DECOMP_FUNC(u16, WeaponId2ChargeNum, (u16 id, int level), 0x00706590);
DECOMP_FUNC(void, itemInfo, (u16 id, ItemInfo* info), 0x007057D0);

// Inventory manager (cItemMgr).
class cItemMgr {
public:
    u8 pad_00[0xC];
    ItemWork* pArm;           // 0x0C  equipped weapon slot
    u16 m_wep_id;             // 0x10  equipped weapon
    u8 pad_12[0x18 - 0x12];
    ItemWork* pLast;          // 0x18  slot of the last get()
    DECOMP_METHOD(ItemWork*, cItemMgr, minimumSearch, (u16 id), (id), 0x00705EE0);
    DECOMP_METHOD(void, cItemMgr, arm, (ItemWork* p), (p), 0x007080A0);
    DECOMP_METHOD(void, cItemMgr, dump, (ItemWork* p), (p), 0x007088B0);
    DECOMP_METHOD(void, cItemMgr, dumpAll, (ItemWork* p), (p), 0x007088F0);
    DECOMP_METHOD(int, cItemMgr, get, (u16 id, u16 num), (id, num), 0x00707A80);
    DECOMP_METHOD(u16, cItemMgr, num, (u16 id), (id), 0x007061D0);
    DECOMP_METHOD(void, cItemMgr, construct, (ItemWork* p, u16 id), (p, id), 0x00707780);
};
// pG->peseta (GameCube GlobalWork::peseta) is at 0x4FA8 on PC.
#define PG_PESETA(g) ((int*) ((u8*) (g) + 0x4FA8))
DECOMP_GLOBAL(cItemMgr, ItemMgr, 0x00C70FD4);

// The player's weapon object (pPL + 0x7D8, then + 0x34); virtual slot 18 (+0x48) reloads the
// magazine display after a capacity tune-up.
class cPlWepObj {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual void bulletUpdate(int a);
};
struct cPlWep {
    u8 pad_00[0x34];
    cPlWepObj* obj;           // 0x34
};
#define PL_WEP(pl) (*(cPlWep**) ((u8*) (pl) + 0x7D8))

// The sub screen work (GameCube SubScreenWk).
DECOMP_GLOBAL(SUB_SCREEN, SubScreenWk, 0x00C7ACD8);

// GLOBAL_WK fields of the PC layout not in global.h yet.
#define PG_SCF(g) ((u32*) ((u8*) (g) + 0x52CC))         // scenario flags (ScenarioFlagOn / Chk)
#define PG_GAME_CNT(g) (*(u16*) ((u8*) (g) + 0x4F9E))  // clears of the main game (GameCube game_cnt)
DECOMP_FUNC(void, ScenarioFlagOn, (u32 no), 0x00489200);
DECOMP_FUNC(u32, ScenarioFlagChk, (u32 no), 0x0067EFF0);
DECOMP_FUNC(u32, ItemFlagChk, (u32 no), 0x006B3240);

// Sub screen helpers (Sscrn ss_pzzl.cpp / sscrn.cpp).
DECOMP_FUNC(void, tempSpaceDisp, (int sw), 0x00789BB0);
DECOMP_FUNC(void, dispScrollBar, (int top, int n, int num, IdUnit* bar, IdUnit* up, IdUnit* down), 0x00772630);
DECOMP_FUNC(void, pieceModelSet, (pzlPiece* p), 0x00789AE0);
// PC-only: sets up the shop's held piece (ss_pzzl region, 0x00788CD0).
DECOMP_FUNC(void, pieceModelPcSet, (SUB_SCREEN* wk, pzlPiece* p), 0x00788CD0);
struct PieceInfo;
DECOMP_FUNC(u8*, searchItemPieceData, (int item_id, PieceInfo* p_info), 0x00688BC0);
// The cockpit's life meter (GameCube Cckpt.m_LifeMeter).
class cLifeMeter {
public:
    DECOMP_METHOD0(void, cLifeMeter, frameIn, 0x005A2F40);
};
DECOMP_GLOBAL(cLifeMeter, Cckpt_LifeMeter, 0x00C50FF8);
DECOMP_FUNC(void, caseModelMove, (int a), 0x0078BA70);
DECOMP_FUNC(void, pieceModelDisp, (SUB_SCREEN* wk), 0x007896D0);
DECOMP_FUNC(void, pzzlCursorDisp, (SUB_SCREEN* wk, int sw), 0x0078B580);
DECOMP_FUNC(void, sscrnModelFree, (SUB_SCREEN* wk), 0x00776100);
DECOMP_FUNC(void, sscrnLightClear, (SUB_SCREEN* wk), 0x007761E0);
DECOMP_FUNC(void, sscrnDataFilename, (SUB_SCREEN* wk, const char* name), 0x00767A40);

// Ordering table (trans_ot) and the Z clear.
DECOMP_FUNC(void, AddOtDirect, (int ot, void* work, void (*func)(), int a3, int a4, f32 f, int a6), 0x006EE110);
DECOMP_FUNC(void, clearZbuffer, (), 0x0040CEB9);

// Piece / model data.
struct PieceInfo;
DECOMP_GLOBAL_ARRAY(PieceInfo, piece_info, 1, 0x00C0B1B8);
DECOMP_FUNC(u8*, searchItemModelData, (int item_id, PieceInfo* p_info), 0x00688C10);
// CAMERA of the Steam build (pG->Camera, 0xF8 bytes copied by dispItem).
struct CameraWork {
    u8 pad_00[0xA4];
    Vec pos;                  // 0x0A4  (GameCube param.pos)
    u8 pad_B0[0xC0 - 0xB0];
    f32 fovy;                 // 0x0C0  degrees
    u8 pad_C4[0xF8 - 0xC4];
};
typedef char decomp_check_CameraWork_size[sizeof(CameraWork) == 0xF8 ? 1 : -1];
DECOMP_GLOBAL(CameraWork, itemCamera, 0x00C58DA8);
class cLightInfo {
public:
    DECOMP_METHOD(void, cLightInfo, init2, (int a0, int a1, const Vec* p0, const Vec* p1, int a4),
                  (a0, a1, p0, p1, a4), 0x0070E660);
};

// ss_shop.cpp data.
struct ShopMsg {
    u8 msg;
    u8 str;
};
struct ShopItemPlace {
    u32 id;                   // 0xFFFFFFFF ends the table on PC
    Vec rot;                  // degrees
    Vec scale;
};
// dispPrice display flags.
DECOMP_GLOBAL(u32, price_disp_price, 0x00C1D2E4);
DECOMP_GLOBAL(u32, price_disp_num, 0x00C1D2E8);
DECOMP_GLOBAL(u32, price_disp_sold, 0x00C1D2EC);
DECOMP_GLOBAL_ARRAY(ShopItemPlace, shop_item_place, 52, 0x00C1D370);
// Merchant line table {message, voice stream} (indexes are the greeting / answer numbers).
DECOMP_GLOBAL_ARRAY(ShopMsg, shop_msg, 26, 0x00C1D2F8);
DECOMP_GLOBAL(Vec, sell_num_pos, 0x00C1DCC0);
DECOMP_GLOBAL_ARRAY(void*, shop_msg_buf, 5, 0x00C7BB24);
DECOMP_GLOBAL(Vec, shop_pos_save, 0x00C7BB38);
DECOMP_GLOBAL(void*, shop_clear_z, 0x00C7BB54);
DECOMP_GLOBAL(int, shop_read_req, 0x00C7BB58);
// PC: the language-dependent texture pack read by SsShopInit::move.
DECOMP_GLOBAL(u32, shop_tex_pack, 0x00C7BB44);

// Id textures and the PC texture packs.
#define TEX_OWNER_ID_SSCRN 9
DECOMP_FUNC(void, IdTexRelease, (int owner), 0x0064A310);
// PC-only: request / poll a texture pack (SsShopInit::move: language-dependent shop textures).
DECOMP_FUNC(void, TexPackRequest, (u32 id, int a1), 0x00958AB0);
DECOMP_FUNC(int, TexPackReady, (u32 id), 0x0093FBB0);
// PC-only render command queued by the shop's exit routine (0x00958E50; argument 0x3C).
DECOMP_FUNC(void, PcRenderCmd, (int cmd), 0x00958E50);
// PC-only flag word tested by the shop's exit routine (bit 0: not yet).
DECOMP_GLOBAL(u8, PcRenderBusy, 0x00C58F88);

// Fade (PC: the colour is an argument).
struct GXColor {
    u8 r, g, b, a;
};
DECOMP_FUNC(void, FadeSet, (GXColor from, GXColor to, int a2, int time, int a4, u32 flags), 0x0062C4D0);

// The game's operator delete (heap free; `delete` of a class without destructor).
DECOMP_FUNC(void, Mem_delete, (void* p), 0x00656010);

// Strings of the original unit (file name for the read, source name for DvdReadN).
DECOMP_GLOBAL_ARRAY(const char, str_ss_shop_dat, 12, 0x00B2960C);
DECOMP_GLOBAL_ARRAY(const char, str_ss_shop_cpp, 16, 0x00B295F8);

// Screen state machine node (GameCube widget.h): vptr 0, then num, link, cur.
template <class T>
class Widget {
public:
    int num;                  // 0x04  link table size
    Widget** link;            // 0x08
    Widget* cur;              // 0x0C  current widget of the chain

    Widget(int n = 1) {
        int i;
        num = n;
        cur = this;
        link = (Widget**) Mem_alloc(sizeof(Widget*) * num, 0, 0, 1, 13);
        for (i = 0; i < num; i++) {
            link[i] = 0;
        }
    }
    virtual ~Widget() { Mem_free(link); }
    virtual void init(T* wk) {}
    virtual void quit(T* wk) {}
    virtual void move(T* wk) {}

    void connect(int no, Widget* w) {
        if (no < num) {
            link[no] = w;
        }
    }
    void transit(int no, T* wk) {
        if (link[no]) {
            quit(wk);
            cur = link[no];
            cur->init(wk);
            cur->cur = cur;
        }
    }
};

class SsShopInit : public Widget<SUB_SCREEN> {
public:
    int state;  // 0x10

    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class PzzlThinking;
class PieceSelect;
class CaseChange;
class ShopTopMenu;
class SellMenuSelect;
class SellItemNum;
class SellConfirm;
class BuyMenuSelect;
class BuyItemNum;
class BuyConfirm;
class BuyPuzzleEnd;
class LvUpMenuSelect;
class LvUpItemSelect;
class LvUpConfirm;

class SsShopMain : public Widget<SUB_SCREEN> {
public:
    PzzlThinking* thinking;    // 0x10
    int x14;
    int x18;
    PieceSelect* select;       // 0x1C
    CaseChange* caseChange;    // 0x20
    ShopTopMenu* topMenu;      // 0x24
    SellMenuSelect* sellSel;   // 0x28
    SellItemNum* sellNum;      // 0x2C
    SellConfirm* sellConf;     // 0x30
    BuyMenuSelect* buySel;     // 0x34
    BuyItemNum* buyNum;        // 0x38
    BuyConfirm* buyConf;       // 0x3C
    BuyPuzzleEnd* buyEnd;      // 0x40
    LvUpMenuSelect* lvSel;     // 0x44
    LvUpItemSelect* lvItem;    // 0x48
    LvUpConfirm* lvConf;       // 0x4C
    Widget<SUB_SCREEN>* cur;   // 0x50
    Widget<SUB_SCREEN>* next;  // 0x54

    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class ShopTopMenu : public Widget<SUB_SCREEN> {
public:
    s8 state;       // 0x10  0 greeting, 1 wait for the coat, 2 menu
    s8 cursor;      // 0x11  0 sell, 1 buy, 2 tune up
    s8 exit;        // 0x12  the exit entry is selected
    u8 pad_13;
    int greetNum;   // 0x14
    int greet[3];   // 0x18
    int greetIdx;   // 0x24
    int greetStep;  // 0x28
    int result;     // 0x2C  1 leave the shop

    ShopTopMenu() : Widget<SUB_SCREEN>(3) { cursor = 1; }
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class SellMenuSelect : public Widget<SUB_SCREEN> {
public:
    int state;  // 0x10

    SellMenuSelect() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class SellItemNum : public Widget<SUB_SCREEN> {
public:
    u16 repeat;  // 0x10  frames the stick was held
    u16 fast;    // 0x12  step by 8

    SellItemNum() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class SellConfirm : public Widget<SUB_SCREEN> {
public:
    int result;  // 0x10

    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class BuyMenuSelect : public Widget<SUB_SCREEN> {
public:
    int state;  // 0x10

    BuyMenuSelect() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class BuyItemNum : public Widget<SUB_SCREEN> {
public:
    int state;   // 0x10  0 check, 1 confirm
    s16 stock;   // 0x14
    s8 unit;     // 0x16
    u8 pad_17;
    int result;  // 0x18
    int msg;     // 0x1C

    BuyItemNum() : Widget<SUB_SCREEN>(3) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class BuyConfirm : public Widget<SUB_SCREEN> {
public:
    int result;  // 0x10
    int msg;     // 0x14

    BuyConfirm() : Widget<SUB_SCREEN>(3) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class BuyPuzzleEnd : public Widget<SUB_SCREEN> {
public:
    int x10;

    BuyPuzzleEnd() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class LvUpMenuSelect : public Widget<SUB_SCREEN> {
public:
    int state;  // 0x10

    LvUpMenuSelect() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class LvUpItemSelect : public Widget<SUB_SCREEN> {
public:
    LvUpItemSelect() : Widget<SUB_SCREEN>(2) {}
    virtual void init(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

class LvUpConfirm : public Widget<SUB_SCREEN> {
public:
    s8 cur[4];   // 0x10  current tune levels + 1
    s8 max[4];   // 0x14  merchant's max tune levels
    int result;  // 0x18
    int msg;     // 0x1C

    virtual void init(SUB_SCREEN* wk);
    virtual void quit(SUB_SCREEN* wk);
    virtual void move(SUB_SCREEN* wk);
};

#endif
