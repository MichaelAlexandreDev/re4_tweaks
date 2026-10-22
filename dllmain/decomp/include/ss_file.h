#ifndef DECOMP_SS_FILE_H
#define DECOMP_SS_FILE_H
#include "types.h"
#include "bind.h"

struct SsFileWork {
    s8 mode, category, scroll, cursor;
    u8 page, messageBase, pageCount, color;
    u32 messageAttr;
    u8 layout, fileNo, extraE, extraF;
    u8 flag10, extra11;
    u8 pad_12[2];
    u32 extra14;
    DECOMP_METHOD(u32, SsFileWork, makeSpecialToken,
                  (int a, int b, u32 flags, int c, int d, f32 e),
                  (a, b, flags, c, d, e), 0x0040B5A5);
};
typedef char check_ss_file_work[sizeof(SsFileWork) == 0x18 ? 1 : -1];

struct FileSubScreen {
    char filename[32];
    u8 pad_20[0x2C-0x20];
    s32 open_flag, attr_flag;
    u32 close_flag, model_flag;
    u8 pad_3C[0x40-0x3C];
    u8 unknown_40;
    u8 trans_off;
    u8 pad_42[6];
    u8 (*scrn_out_func)(FileSubScreen*);
    u8 pad_4C[0x1AC-0x4C];
    s8 stage_no;
    u8 pad_1AD[0x1D8-0x1AD];
    u8* pCmmn;
    u8* pSwitchDat;
    u8 pad_1E0[0x1F8-0x1E0];
    u8* pFileDat;
    u8 pad_1FC[0xC];
    void* pTplDat;
    u8 pad_20C[0x54];
    s8 menu_no, menu_next, menu_old;
    u8 cursor_mode;
    u8 pad_264;
    u8 alpha_flag;
    u16 alpha_cnt;
    u8 pad_268[0x2F6-0x268];
    u16 open_file_id;
    u8 pad_2F8[0x308-0x2F8];
    SsFileWork* file;
    u8 pad_30C[0x344-0x30C];
    u8 layout_flag;
};
typedef char check_file_screen[sizeof(FileSubScreen) == 0x348 ? 1 : -1];

class FileWidget {
public:
    int num;
    FileWidget** link;
    FileWidget* cur;
    virtual ~FileWidget();
    virtual void init(FileSubScreen* wk);
    virtual void quit(FileSubScreen* wk);
    virtual void move(FileSubScreen* wk);
    DECOMP_METHOD(void, FileWidget, transitOriginal, (int no, FileSubScreen* wk), (no, wk), 0x004023BF);
    void connect(int no, FileWidget* target) { if (no < num) link[no] = target; }
    void transit(int no, FileSubScreen* wk) {
        if (link[no]) { quit(wk); cur=link[no]; cur->init(wk); cur->cur=cur; }
    }
};
typedef char check_file_widget[sizeof(FileWidget) == 0x10 ? 1 : -1];

class SsFileInit : public FileWidget {
public:
    int state;
    virtual void init(FileSubScreen* wk);
    virtual void move(FileSubScreen* wk);
};
class FileSelect : public FileWidget {
public:
    DECOMP_METHOD0(FileSelect*, FileSelect, construct, 0x00404453);
    int state;
    virtual void init(FileSubScreen* wk);
    virtual void quit(FileSubScreen* wk);
    virtual void move(FileSubScreen* wk);
};
class MessageDisplay : public FileWidget {
public:
    DECOMP_METHOD0(MessageDisplay*, MessageDisplay, construct, 0x004024C3);
    u8 state;
    u8 tpl_state;
    u8 tpl_first;
    u8 pad_13;
    s16 x, y;
    u32 extra18;
    virtual void init(FileSubScreen* wk);
    virtual void quit(FileSubScreen* wk);
    virtual void move(FileSubScreen* wk);
};
class SsFileMain : public FileWidget {
public:
    int state;
    int sound_wait;
    int sound_count;
    FileSelect* select;
    MessageDisplay* display;
    FileWidget* cur;
    FileWidget* next;
    virtual void init(FileSubScreen* wk);
    virtual void move(FileSubScreen* wk);
    virtual void quit(FileSubScreen* wk);
};

#endif
