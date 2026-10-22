#ifndef DECOMP_SS_ITEM_H
#define DECOMP_SS_ITEM_H
#include "types.h"
#include "bind.h"

// Local Steam view, kept distinct from the map's partial SUB_SCREEN.
struct SsItemWork { s8 x0, col, idx[2], sel[2], comb[2], x8; };
struct ItemSubScreen {
    char filename[32]; u8 pad_20[0x2C-0x20];
    s32 open_flag, attr_flag; u32 close_flag;
    u8 pad_38[0x41-0x38]; u8 trans_off; u8 pad_42[6];
    u8 (*scrn_out_func)(ItemSubScreen*);
    u8 pad_4C[0x1D8-0x4C];
    u8* pCmmn; u8* pSwitchDat; u8 pad_1E0[4]; u8* pItemDat;
    u8 pad_1E8[0x244-0x1E8];
    void* p_exam_item; void* p_exam_model;
    u8 pad_24C[0x260-0x24C];
    s8 menu_no,menu_next,menu_old; u8 cursor_mode;
    u8 pad_264; u8 alpha_flag; u16 alpha_cnt; s8 cmd_menu_no;
    u8 pad_269[0x300-0x269];
    SsItemWork* item;
};
class ItemWidget {
public:
    int num; ItemWidget** link; ItemWidget* cur;
    virtual ~ItemWidget();
    virtual void init(ItemSubScreen* wk);
    virtual void quit(ItemSubScreen* wk);
    virtual void move(ItemSubScreen* wk);
    DECOMP_METHOD(void, ItemWidget, transitOriginal, (int no, ItemSubScreen* wk), (no,wk), 0x004023BF);
    DECOMP_METHOD0(ItemWidget*, ItemWidget, constructSelect, 0x0040E70F);
    DECOMP_METHOD0(ItemWidget*, ItemWidget, constructCommand, 0x00409C37);
    DECOMP_METHOD0(ItemWidget*, ItemWidget, constructCombine, 0x0040BDBB);
    DECOMP_METHOD0(ItemWidget*, ItemWidget, constructExamine, 0x00402342);
    void connect(int no, ItemWidget* p) { if (no<num) link[no]=p; }
    void transit(int no, ItemSubScreen* wk) {
        if (link[no]) { quit(wk); cur=link[no]; cur->init(wk); cur->cur=cur; }
    }
};
class SsItemInit : public ItemWidget { public: int state; virtual void init(ItemSubScreen*); virtual void move(ItemSubScreen*); };
class ItemSelect : public ItemWidget { public: int state; virtual void init(ItemSubScreen*); virtual void move(ItemSubScreen*); };
struct ItemIdUnit;
class ItemCommand : public ItemWidget {
public:
    ItemIdUnit* id[16]; ItemIdUnit* sub[11]; u8 pad_7C[0x90-0x7C];
    int mode; s8 num; u8 cursorOld; s8 subSel; u8 pad_97; int state;
    virtual void init(ItemSubScreen*); virtual void move(ItemSubScreen*);
};
class ItemCombine : public ItemWidget { public: virtual void init(ItemSubScreen*); virtual void quit(ItemSubScreen*); virtual void move(ItemSubScreen*); };
class SsItemMain : public ItemWidget {
public:
    int state; ItemSelect* sel; ItemCommand* cmd; ItemCombine* comb;
    ItemWidget* exam; ItemWidget* cur; ItemWidget* next;
    virtual void init(ItemSubScreen*); virtual void move(ItemSubScreen*); virtual void quit(ItemSubScreen*);
};
typedef char check_item_widget[sizeof(ItemWidget)==0x10 ? 1 : -1];
typedef char check_item_work[sizeof(SsItemWork)==9 ? 1 : -1];
typedef char check_item_command[sizeof(ItemCommand)==0x9C ? 1 : -1];
#endif
