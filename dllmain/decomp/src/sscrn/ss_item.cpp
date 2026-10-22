// Steam key-item / treasure screen. GC source is a logic guide only.
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "ss_item.h"
#include "mem.h"
#include "dvd.h"
#include "snd.h"

struct ItemIdUnit {
    u8 be_flag; u8 pad_01[0x68-1]; ItemIdUnit* pParent; u8 pad_6C[0x7A-0x6C]; u8 texNo; u8 pad_7B[0x8A-0x7B];
    u8 rev_flag,tex_flag; u8 pad_8C[0x91-0x8C]; u8 anima_state;
    u8 pad_92[2]; Vec pos0;
    u8 pad_A0[0x12C-0xA0]; void* path0; void* path1; void* curve;
};
class ItemIdSystem {
public:
    DECOMP_METHOD(u8, ItemIdSystem, setCk, (u8 group), (group), 0x0040A61E);
    DECOMP_METHOD(void, ItemIdSystem, unitParent, (ItemIdUnit* parent,ItemIdUnit* child), (parent,child), 0x0040D580);
    DECOMP_METHOD(ItemIdUnit*, ItemIdSystem, unitPtr, (u8 no,u8 group), (no,group), 0x004095F7);
    DECOMP_METHOD(void, ItemIdSystem, setTime, (ItemIdUnit* u,s16 t), (u,t), 0x004022A7);
    DECOMP_METHOD(void, ItemIdSystem, movePos, (ItemIdUnit* u), (u), 0x00407F9A);
    DECOMP_METHOD(void, ItemIdSystem, set, (void* data,u8 a,u8 group,u8 b,u8 c,u8 d), (data,a,group,b,c,d), 0x00404449);
    DECOMP_METHOD(void, ItemIdSystem, dispSw, (int group,int on), (group,on), 0x004088F5);
};
DECOMP_GLOBAL(ItemIdSystem, ItemIds, 0x00C7AB28);
DECOMP_GLOBAL(ItemIdSystem, ItemNumbers, 0x00C7AC78);
DECOMP_GLOBAL(ItemIdSystem, ItemHud, 0x00C59788);
struct UiItemWork { u16 id,num; u8 flags; };
class ScreenItemMgr {
public:
    DECOMP_METHOD(u8, ScreenItemMgr, use, (UiItemWork* item), (item), 0x0040519B);
    DECOMP_METHOD(void, ScreenItemMgr, dumpAll, (UiItemWork* item), (item), 0x00406433);
    DECOMP_METHOD(UiItemWork*, ScreenItemMgr, at, (int no), (no), 0x004092A5);
    DECOMP_METHOD(u8, ScreenItemMgr, makeItemList, (u8* list,int flag,s8* a,s8* b), (list,flag,a,b), 0x00405DCB);
    DECOMP_METHOD(u8, ScreenItemMgr, combine, (UiItemWork* a,UiItemWork* b,int flag), (a,b,flag), 0x004010EB);
};
DECOMP_GLOBAL(ScreenItemMgr, ScreenItems, 0x00C70FD4);
DECOMP_GLOBAL(UiItemWork, ItemDummy, 0x00C7B594);
DECOMP_GLOBAL_ARRAY(s8, ItemCounts, 2, 0x00C7B354);
DECOMP_GLOBAL_ARRAY(u8, ItemList, 0x180, 0x00C7B358);
DECOMP_GLOBAL(UiItemWork*, ItemSelected, 0x00C7B500);
DECOMP_GLOBAL(unsigned __int64, ItemKeyTrg, 0x00C62F10);
DECOMP_GLOBAL_ARRAY(int, ItemFrameState, 2, 0x00C7B4F0);
DECOMP_GLOBAL(u8, ItemInitFlag, 0x00C7B524);
DECOMP_FUNC(void, ItemFrameSetRaw, (), 0x0076F960);
DECOMP_FUNC(void, ItemTexNoRaw, (), 0x0076F180);
DECOMP_FUNC(int, ItemCursorSelect, (ItemSubScreen*,int), 0x00770270);
DECOMP_FUNC(bool, ItemKey2Game, (ItemSubScreen*), 0x00405466);
static void ItemSetFrame(ItemSubScreen* wk,int col) { __asm { mov ecx,col
    push wk
    call ItemFrameSetRaw
    add esp,4 } }
static u8 ItemTexture(u16 id) { u8 tex; __asm { movzx ecx,id
    call ItemTexNoRaw
    mov tex,al } return tex; }

// FUNCTION: BIO4 0x0076F0B0
// STATUS: stub
UiItemWork* __fastcall ITEM_PTR(int idx,int col)
{
    u8 no;
    ItemDummy.flags=0;
    if (idx<0 || idx>ItemCounts[col]-1) goto DUMMY;
    no=ItemList[idx+col*ItemCounts[0]];
    if (no==0xFF) return &ItemDummy;
    return ScreenItems.at(no);
DUMMY:
    return &ItemDummy;
}
// FUNCTION: BIO4 0x0076F110
// STATUS: stub
static int ITEM_AT(UiItemWork* p,int col)
{
    for (int i=0;i<ItemCounts[col];i++)
        if (p==ScreenItems.at(ItemList[i+col*ItemCounts[0]])) return i;
    return -1;
}

// FUNCTION: BIO4 0x0076EE30
// STATUS: matching
void SsItemInit::init(ItemSubScreen* wk)
{
    state=wk->open_flag==0x80 ? 2 : 0;
    ItemInitFlag=0;
}

DECOMP_GLOBAL_ARRAY(void*, ItemPaths0, 2, 0x00C7B51C);
DECOMP_GLOBAL_ARRAY(void*, ItemPaths1, 2, 0x00C7B4F8);
DECOMP_GLOBAL_ARRAY(void*, ItemCurves, 2, 0x00C7B34C);
DECOMP_GLOBAL_ARRAY(Vec, ItemFrameScreen, 2, 0x00C7B504);
DECOMP_GLOBAL(u8, ItemFrameOn, 0x00C7B348);
DECOMP_FUNC(void, ItemPathParametrize, (void*,void*), 0x00406604);
class ItemLifeMeter {
public:
    DECOMP_METHOD(void, ItemLifeMeter, fix, (int on), (on), 0x00406C08);
    DECOMP_METHOD0(void, ItemLifeMeter, frameIn, 0x00402F27);
    DECOMP_METHOD0(void, ItemLifeMeter, frameOut, 0x0040CEE1);
};
DECOMP_GLOBAL(ItemLifeMeter, ItemLife, 0x00C50FF8);
DECOMP_FUNC(void, ItemExitResourceCommand, (int), 0x004092E6);

// Steam's internal exit initializer takes wk in EDI. Only its local caller uses it.
// FUNCTION: BIO4 0x0076EE60
// STATUS: matching
static void sscrn_item_out_init(ItemSubScreen* wk)
{
    ItemIdUnit* u=ItemIds.unitPtr(0,0x16); u->rev_flag|=1;
    ItemIds.unitPtr(5,0x16)->rev_flag|=4;
    u=ItemNumbers.unitPtr(0x40,0x15);
    u->path0=ItemPaths0[0]; u->curve=ItemCurves[0]; u->path1=ItemPaths1[0];
    ItemPathParametrize(u->path0,u->path1);
    if (ItemFrameOn) {
        u->rev_flag&=~1; u->pos0=ItemFrameScreen[0];
        ItemIds.setTime(u,0); ItemIds.movePos(u); ItemIds.setTime(u,15); ItemIds.movePos(u);
    }
    u->rev_flag|=1;
    u=ItemNumbers.unitPtr(0x50,0x15);
    u->path0=ItemPaths0[1]; u->curve=ItemCurves[1]; u->path1=ItemPaths1[1];
    ItemPathParametrize(u->path0,u->path1);
    if (ItemFrameOn) {
        u->rev_flag&=~1; u->pos0=ItemFrameScreen[1];
        ItemIds.setTime(u,0); ItemIds.movePos(u); ItemIds.setTime(u,15); ItemIds.movePos(u);
    }
    u->rev_flag|=1;
    ItemIds.unitPtr(1,0x1E)->rev_flag|=1;
    if (wk->menu_next==2) { ItemLife.frameOut(); wk->alpha_flag=1; }
}
// FUNCTION: BIO4 0x0076F080
// STATUS: matching
static u8 sscrn_item_out(ItemSubScreen* wk)
{
    if (ItemNumbers.unitPtr(0x40,0x15)->anima_state&1) {
        ItemExitResourceCommand(0x39);
        return 1;
    }
    return 0;
}

// FUNCTION: BIO4 0x0076F8D0
// STATUS: matching
void SsItemMain::quit(ItemSubScreen* wk)
{
    delete sel; delete cmd; delete comb; delete exam;
    cur=0;
    Mem_free(wk->item);
    sscrn_item_out_init(wk);
    wk->scrn_out_func=sscrn_item_out;
}

// FUNCTION: BIO4 0x007704F0
// STATUS: stub
void ItemSelect::init(ItemSubScreen* wk)
{
    wk->item->comb[0]=-1; wk->item->comb[1]=-1;
    for (int i=0;i<2;i++) ItemSetFrame(wk,i);
}

// FUNCTION: BIO4 0x00771800
// STATUS: matching
void ItemSelect::move(ItemSubScreen* wk)
{
    SsItemWork* iw=wk->item; s8 col=iw->col;
    state=0; wk->cursor_mode=1;
    if (ItemKey2Game(wk)) { state=1; return; }
    if (ItemKeyTrg & 0x0000200040000000ULL) {
        state=2; SndCall(0,0xA,0,0,0,0);
        for (int i=0;i<2;i++) ItemIds.unitPtr((u8)(i+1),0x16)->be_flag|=8;
        return;
    }
    if (ItemKeyTrg & 0x80000000ULL) {
        ItemSelected=ITEM_PTR(iw->sel[col],col);
        if (!(ItemSelected->flags&1)) return;
        transitOriginal(0,wk);
        for (int i=0;i<2;i++) {
            ItemIdUnit* u=ItemIds.unitPtr((u8)(i+1),0x16);
            if (i==iw->col) u->be_flag|=8; else u->be_flag&=~8;
        }
        return;
    }
    if (ItemCursorSelect(wk,0)) {
        state=2; SndCall(0,0xA,0,0,0,0);
        for (int i=0;i<2;i++) ItemIds.unitPtr((u8)(i+1),0x16)->be_flag|=8;
    }
}

// FUNCTION: BIO4 0x00770940
// STATUS: stub
void ItemCombine::init(ItemSubScreen* wk)
{
    SsItemWork* iw=wk->item; s8 col=iw->col; u8 base=0;
    switch (col) { case 0:base=0x60;break; case 1:base=0x70;break; }
    ItemIds.unitPtr(base,0x16)->rev_flag&=0xF0;
    ItemIds.unitPtr((u8)(base+1),0x16)->be_flag|=8;
    ItemIds.unitPtr((u8)(base+2),0x16)->be_flag|=8;
    UiItemWork* item=ITEM_PTR(iw->sel[col],col);
    ItemIdUnit* u=ItemIds.unitPtr((u8)(base+1),0x16);
    u->tex_flag|=2; u->texNo=ItemTexture(item->id);
    iw->comb[col]=iw->sel[col];
    ItemSetFrame(wk,col);
}

// FUNCTION: BIO4 0x0076F3B0
// STATUS: matching
void ItemCombine::quit(ItemSubScreen* wk)
{
    SsItemWork* iw=wk->item; s8 col=iw->col; u8 base=0;
    switch (col) { case 0:base=0x60;break; case 1:base=0x70;break; }
    ItemIds.unitPtr(base,0x16)->rev_flag|=0xF;
    ItemIds.unitPtr((u8)(base+1),0x16)->be_flag&=~8;
    ItemIds.unitPtr((u8)(base+2),0x16)->be_flag&=~8;
    iw->comb[col]=-1;
}

// FUNCTION: BIO4 0x007721D0
// STATUS: stub
void ItemCombine::move(ItemSubScreen* wk)
{
    SsItemWork* iw=wk->item; s8 col=iw->col;
    wk->cursor_mode=2;
    if (ItemKeyTrg & 0x0000200040000000ULL) {
        s8 old=iw->idx[col]; iw->idx[col]=iw->sel[col];
        int d=old-iw->idx[col]; iw->comb[col]=-1;
        if (d>0) ItemFrameState[col]=1;
        else if (d<0) ItemFrameState[col]=2;
        else ItemSetFrame(wk,col);
        transit(1,wk);
    } else if (ItemKeyTrg & 0x80000000ULL) {
        UiItemWork* a=ITEM_PTR(iw->sel[col],col);
        UiItemWork* b=ITEM_PTR(iw->comb[col],col);
        if (b!=a && ScreenItems.combine(b,a,0)) {
            ScreenItems.makeItemList(ItemList,0,&ItemCounts[0],&ItemCounts[1]);
            iw->idx[col]=(s8)ITEM_AT(b,col);
            transitOriginal(0,wk); SndCall(0,0x27,0,0,0,0);
        } else SndCall(0,7,0,0,0,0);
    } else ItemCursorSelect(wk,1);
}

DECOMP_FUNC(void, ItemIdErase, (), 0x004070EA);
DECOMP_FUNC(void, ItemNumErase, (), 0x00408A99);
DECOMP_FUNC(void, ItemFreeBuffer, (), 0x00408CAB);
DECOMP_FUNC(void, ItemDataFilename, (ItemSubScreen*,const char*), 0x00404101);
DECOMP_FUNC(void, ItemModelFree, (ItemSubScreen*), 0x00407CA2);
DECOMP_FUNC(void, ItemGeneralModelAlloc, (ItemSubScreen*), 0x004020E0);
DECOMP_FUNC(void, ItemPlayerModelInit, (), 0x004026E9);
DECOMP_FUNC(void, ItemLightClear, (ItemSubScreen*), 0x00402AE5);
DECOMP_FUNC(void, ItemModelClear, (ItemSubScreen*), 0x00402D79);
DECOMP_FUNC(void, ItemResourceRequest, (u32,u16), 0x00401915);
DECOMP_FUNC(int, ItemResourceReady, (u32), 0x00402CF2);
DECOMP_FUNC(void, ItemFadeSetW, (u32,int,u32), 0x0040D0A8);
DECOMP_GLOBAL(int, ItemLoadWait, 0x00C7B5B0);
DECOMP_GLOBAL(int, ItemReadReq, 0x00C7B5AC);
DECOMP_GLOBAL(u32, ItemResource, 0x00C7B528);
struct ItemFadedModel { u8 pad_00[0x158]; f32 alpha; };
DECOMP_GLOBAL(ItemFadedModel*, ItemPlayerModel, 0x00C7BA6C);
DECOMP_GLOBAL(ItemFadedModel*, ItemWeaponModel, 0x00C7BA64);
DECOMP_GLOBAL(ItemFadedModel*, ItemWeaponModel2, 0x00C7BA68);
static u8* ItemArc(u8* data,int idx) { return data+((u32*)data)[idx]; }

// Unlike GC, Steam falls through state 0 -> 1 -> 2 in the same frame.
// FUNCTION: BIO4 0x00770B50
// STATUS: stub
void SsItemInit::move(ItemSubScreen* wk)
{
    switch (state) {
    case 0:
        if (wk->scrn_out_func(wk)!=1) break;
        if (wk->menu_old==2) wk->trans_off=1;
        ItemIdErase(); ItemNumErase(); ItemFreeBuffer();
        ItemIds.set(ItemArc(wk->pCmmn,0xC),0xFF,0x14,0xC,2,0);
        ItemResource=0; ItemLoadWait=0; state++;
        // fallthrough
    case 1:
        if (--ItemLoadWait>=0) break;
        state++;
        // fallthrough
    case 2:
        ItemHud.dispSw(0x21,1); ItemHud.dispSw(0x32,1);
        ItemIds.dispSw(2,1); ItemHud.dispSw(0x34,1);
        ItemDataFilename(wk,"ss_item.dat");
        ItemReadReq=DvdReadN(wk->filename,wk->pSwitchDat,0,0,0,0x10,"src\\ss_item.cpp",0xF2);
        if (ItemReadReq<=0) break;
        if (wk->menu_old==2 && wk->open_flag!=0x80) {
            ItemModelFree(wk); ItemGeneralModelAlloc(wk); ItemPlayerModelInit(); ItemLightClear(wk);
            ItemLife.fix(1); ItemLife.frameIn();
        } else ItemModelClear(wk);
        switch ((s8)wk->filename[4]) {
        case 'p':ItemResource=0x39000000;break;
        case 'n':ItemResource=0x39000001;break;
        case 'r':ItemResource=0x39000002;break;
        case 'e':ItemResource=0x39000003;break;
        case 't':ItemResource=0x39000004;break;
        case 's':ItemResource=0x39000005;break;
        case 'h':
            if (wk->filename[5]=='t') ItemResource=0x39000006;
            else if (wk->filename[5]=='s') ItemResource=0x39000009;
            break;
        }
        ItemResourceRequest(ItemResource,0); wk->trans_off=0; state++;
        // fallthrough
    case 3: {
        u32 stat,size;
        if (Dvd.ReadCheckOne(ItemReadReq,&stat,&size,0)!=1) break;
        if (!ItemResourceReady(ItemResource)) break;
        wk->pItemDat=wk->pSwitchDat; state++; ItemResource=0;
        // fallthrough
    }
    case 4:
        if (wk->open_flag==0x80) ItemFadeSetW(0,5,0);
        transit(0,wk); break;
    }
    if (wk->menu_old==2 && wk->open_flag!=0x80) {
        if (ItemPlayerModel) ItemPlayerModel->alpha=0;
        if (ItemWeaponModel) ItemWeaponModel->alpha=0;
        if (ItemWeaponModel2) ItemWeaponModel2->alpha=0;
    }
}

// FUNCTION: BIO4 0x0076F450
// STATUS: matching
static void setCommandId(u8 mode, ItemIdUnit** tbl, s8* num)
{
    switch (mode) {
    case 0:
        tbl[0] = ItemIds.unitPtr(0x20, 0x16);
        tbl[1] = ItemIds.unitPtr(0x21, 0x16);
        tbl[2] = ItemIds.unitPtr(0x22, 0x16);
        tbl[3] = ItemIds.unitPtr(0x29, 0x16);
        *num = 3;
        tbl[4] = ItemIds.unitPtr(0x23, 0x16);
        tbl[5] = ItemIds.unitPtr(0x26, 0x16);
        tbl[6] = ItemIds.unitPtr(0x24, 0x16);
        tbl[7] = ItemIds.unitPtr(0x27, 0x16);
        tbl[8] = ItemIds.unitPtr(0x25, 0x16);
        tbl[9] = ItemIds.unitPtr(0x28, 0x16);
        break;
    case 1:
        tbl[0] = ItemIds.unitPtr(0x30, 0x16);
        tbl[1] = ItemIds.unitPtr(0x31, 0x16);
        tbl[2] = ItemIds.unitPtr(0x32, 0x16);
        tbl[3] = ItemIds.unitPtr(0x37, 0x16);
        *num = 2;
        tbl[4] = ItemIds.unitPtr(0x33, 0x16);
        tbl[5] = ItemIds.unitPtr(0x35, 0x16);
        tbl[6] = ItemIds.unitPtr(0x34, 0x16);
        tbl[7] = ItemIds.unitPtr(0x36, 0x16);
        break;
    }
}


// FUNCTION: BIO4 0x00770850
// STATUS: stub
void ItemCommand::init(ItemSubScreen* wk)
{
    u8 column=wk->item->col!=0;
    setCommandId(column,id,&num);
    for (int i=0;i<num*2+4;i++) { id[i]->rev_flag&=0xF0; id[i]->be_flag|=8; }
    mode=0; cursorOld=1; state=0; wk->cmd_menu_no=0;
    for (int i=0;i<num;i++) {
        if (i==wk->cmd_menu_no) id[i*2+4]->be_flag|=8;
        else id[i*2+4]->be_flag&=~8;
    }
    SndCall(0,4,0,0,0,0);
}

DECOMP_FUNC(void*, ItemNew, (u32 size), 0x0040E386);
DECOMP_FUNC(void, ItemCameraInit, (ItemSubScreen*,void* camera), 0x0040EBF1);
DECOMP_GLOBAL(u8*, ItemGameGlobals, 0x00C06F3C);
DECOMP_FUNC(void, ItemIdTexDataLoad, (void*,int), 0x00401F46);
DECOMP_FUNC(void, ItemNumberDisp, (int,int,Vec*,int), 0x004059CF);
DECOMP_FUNC(void, ItemNumberBuffer, (int count), 0x00403C92);
DECOMP_FUNC(void, ItemLightCreate, (ItemSubScreen*,void*), 0x0040DBFC);
DECOMP_FUNC(void, ItemFrameInit, (ItemSubScreen*), 0x0076FC80);
DECOMP_FUNC(void, ItemMainMenuInit, (ItemSubScreen*,int), 0x00409B56);
DECOMP_FUNC(bool, ItemMainMenu, (ItemSubScreen*), 0x004074B9);
DECOMP_GLOBAL(u8*, ItemMessageData, 0x00C71594);
DECOMP_GLOBAL(int, ItemMessageFlag, 0x00C715A8);
DECOMP_GLOBAL(s8, ItemSavedCol, 0x00C7B527);
DECOMP_GLOBAL_ARRAY(s8, ItemSavedIdx, 2, 0x00C7B525);

// FUNCTION: BIO4 0x00770F20
// STATUS: stub
void SsItemMain::init(ItemSubScreen* wk)
{
    ItemWidget* p;
    p=(ItemWidget*)ItemNew(0x14); sel=(ItemSelect*)(p ? p->constructSelect() : 0);
    p=(ItemWidget*)ItemNew(0x9C); cmd=(ItemCommand*)(p ? p->constructCommand() : 0);
    p=(ItemWidget*)ItemNew(0x10); comb=(ItemCombine*)(p ? p->constructCombine() : 0);
    p=(ItemWidget*)ItemNew(0x70); exam=p ? p->constructExamine() : 0;
    sel->connect(0,cmd); cmd->connect(0,sel); cmd->connect(1,comb); cmd->connect(2,exam);
    comb->connect(0,sel); comb->connect(1,cmd); exam->connect(0,sel); cur=sel;
    ItemCameraInit(wk,ItemGameGlobals+0x74);
    ItemIdTexDataLoad(ItemArc(wk->pItemDat,5),9);
    if (!ItemIds.setCk(0x14)) ItemIds.set(ItemArc(wk->pCmmn,0xC),0xFF,0x14,0xC,2,0);
    ItemNumbers.set(ItemArc(wk->pItemDat,7),0xFF,0x15,0xC,6,0);
    for (int i=0;i<32;i++) ItemNumbers.set(ItemArc(wk->pCmmn,8),0xFF,(u8)(i+0x40),0xC,5,0);
    ItemNumberBuffer(32);
    for (int i=0;i<32;i++) ItemNumberDisp(i+0x40,0,0,0);
    for (int col=0;col<2;col++) {
        int no=col*8+0x40;
        for (int n=0;n<8;n++,no++) {
            ItemIdUnit* parent=ItemNumbers.unitPtr((u8)(0x18+col*0x10-n),0x15);
            ItemNumbers.unitParent(parent,ItemNumbers.unitPtr(0,(u8)no));
        }
    }
    ItemIds.set(ItemArc(wk->pItemDat,6),0xFF,0x16,0xC,4,0);
    for (int i=0;i<2;i++) {
        ItemIdUnit* tbl[16]; s8 count;
        setCommandId((u8)i,tbl,&count);
        for (int j=0;j<count*2+4;j++) tbl[j]->rev_flag|=0xF;
    }
    ItemIds.set(ItemArc(wk->pCmmn,0x10),0xFF,0x1E,0x13,1,0);
    ItemIds.unitPtr(0,0x1E)->be_flag&=~8;
    ItemIds.unitPtr(0x60,0x16)->rev_flag|=0xF;
    ItemIds.unitPtr(0x61,0x16)->be_flag&=~8;
    ItemIds.unitPtr(0x62,0x16)->be_flag&=~8;
    ItemIds.unitPtr(0x70,0x16)->rev_flag|=0xF;
    ItemIds.unitPtr(0x71,0x16)->be_flag&=~8;
    ItemIds.unitPtr(0x72,0x16)->be_flag&=~8;
    ItemLightCreate(wk,ItemArc(wk->pCmmn,0x12));
    if (wk->menu_old==2 && wk->open_flag!=0x80) { wk->alpha_flag=0; wk->alpha_cnt=10; }
    ItemMessageData=ItemArc(wk->pItemDat,4); ItemMessageFlag=1;
    wk->item=(SsItemWork*)Mem_alloc(9,0,0,1,0xD); memclr_asm(wk->item,9);
    ItemMainMenuInit(wk,0); state=0;
    wk->item->comb[0]=-1; wk->item->comb[1]=-1;
    ItemFrameInit(wk);
    if (ItemInitFlag) {
        wk->item->col=ItemSavedCol; wk->item->idx[0]=ItemSavedIdx[0]; wk->item->idx[1]=ItemSavedIdx[1];
        ItemSetFrame(wk,0); ItemSetFrame(wk,1);
    }
    SndCall(0,0x1E,0,0,0,0);
}

// Both 64-bit mask arguments are retained exactly as pushed by Steam.
DECOMP_FUNC(int, ItemKeyTrgCheck, (unsigned __int64,unsigned __int64), 0x004065C3);
DECOMP_FUNC(void, ItemNameDispRaw, (), 0x0076F770);
static void ItemNameDisp(ItemSubScreen* wk) { __asm { mov eax,wk
    call ItemNameDispRaw } }

// Steam strips the GC debug item-make menu and changes the tab link numbers.
// FUNCTION: BIO4 0x00771570
// STATUS: stub
void SsItemMain::move(ItemSubScreen* wk)
{
    if (!cur) return;
    if (cur!=exam && state==0) ItemNameDisp(wk);
    wk->cursor_mode=0;
    switch (state) {
    case 0: {
        ItemWidget* w=cur; w->move(wk); next=w->cur;
        if (cur==sel) {
            switch (sel->state) {
            case 2: state=1; wk->close_flag|=2; ItemMainMenuInit(wk,1); break;
            case 1: wk->close_flag|=2; transitOriginal(0,wk); break;
            default:
                if (wk->open_flag!=0x80 && next==cur && ItemKeyTrgCheck(0x800000ULL,0x400000ULL)) {
                    wk->close_flag=0; wk->menu_old=0; transitOriginal(1,wk);
                }
                break;
            }
        }
        if (cur==cmd) {
            switch (cmd->state) {
            case 1: wk->close_flag|=2; transitOriginal(0,wk); break;
            case 2:
                wk->close_flag|=2;
                ItemSavedCol=wk->item->col; ItemSavedIdx[0]=wk->item->idx[0]; ItemSavedIdx[1]=wk->item->idx[1];
                transitOriginal(5,wk); SndCall(0,0x1A,0,0,0,0); break;
            }
        }
        cur=next; break;
    }
    case 1:
        if (ItemMainMenu(wk)) {
            switch (wk->menu_no) {
            case 0: transitOriginal(1,wk); break;
            case 1:
                wk->item->col=0; ItemMainMenuInit(wk,0); state=0; SndCall(0,6,0,0,0,0); break;
            case 2: transitOriginal(3,wk); break;
            case 3: transitOriginal(4,wk); break;
            case 4: transitOriginal(0,wk); break;
            }
        }
        if (ItemKeyTrgCheck(0x2000000ULL,0x200000ULL)) {
            wk->item->col=0; ItemMainMenuInit(wk,0); state=0; SndCall(0,6,0,0,0,0);
        }
        break;
    }
}

DECOMP_GLOBAL(int, ItemCommandMode, 0x00C7B5B4);
DECOMP_FUNC(void, ItemVectorAdd, (const Vec*,const Vec*,Vec*), 0x00407F63);
DECOMP_FUNC(int, ItemKeyRepCheck, (unsigned __int64,unsigned __int64), 0x0040E9FD);
DECOMP_FUNC(void, ItemFrameMoveRaw, (), 0x007700B0);
static void ItemFrameMove(ItemSubScreen* wk,int col) { __asm { mov edi,col
    push wk
    call ItemFrameMoveRaw
    add esp,4 } }
struct ItemModelManager { u32 vptr; u8* array; u32 count,size; };
DECOMP_GLOBAL(ItemModelManager, ItemModels, 0x00C16320);
static void* ItemGetModel(u32 no) { return no<ItemModels.count ? ItemModels.array+ItemModels.size*no : 0; }

// FUNCTION: BIO4 0x007719C0
// STATUS: stub
void ItemCommand::move(ItemSubScreen* wk)
{
    SsItemWork* iw = wk->item;

    wk->cursor_mode = 1;
    switch (mode) {
    case 0:
        if (ItemKeyTrg & 0x0000200040000000ULL) {
            {
                int j;
                for (j = 0; j < num * 2 + 4; j++) {
                    id[j]->rev_flag |= 0xF;
                }
            }
            transit(0, wk);
            SndCall(0, 5, 0, 0, 0, 0);
            return;
        }
        if (ItemKeyTrg & 0x80000000ULL) {
            u8 used = 0;
            switch (iw->col) {
            case 0:
                switch (wk->cmd_menu_no) {
                case 0:
                    ItemCommandMode = used;
                    break;
                case 1:
                    ItemCommandMode = 2;
                    break;
                case 2:
                    ItemCommandMode = 1;
                    break;
                }
                break;
            case 1:
                switch (wk->cmd_menu_no) {
                case 0:
                    ItemCommandMode = iw->col;
                    break;
                case 1:
                    ItemCommandMode = 2;
                    break;
                }
                break;
            }
            if (!(ItemCommandMode > 2)) {
                int min = 1;  // a literal 1 folds to `<= 0`; the target keeps `cmpwi 1; blt`
                if (!(ItemCommandMode < min)) {
                {
                    int j;
                    for (j = 0; j < num * 2 + 4; j++) {
                        id[j]->rev_flag |= 0xF;
                    }
                }
                }
            }
            switch (ItemCommandMode) {
            case 2:
                transit(1, wk);
                SndCall(0, 9, 0, 0, 0, 0);
                return;
            case 0:
                used = ScreenItems.use(ItemSelected);
                break;
            case 1:
                // Both stores through PSet: the x24C store may then alias `ItemSelected`, so its `lis`
                // and load stay below it (the store is on the critical path).
                wk->p_exam_model = ItemGetModel(2);
                wk->p_exam_item = ItemSelected;
                if (ItemSelected->id == 0xA2) {
                    state = 2;
                    return;
                }
                transit(2, wk);
                SndCall(0, 0x1A, 0, 0, 0, 0);
                return;
            case 3:
                mode = 1;
                subSel = 1;
                SndCall(0, 0xB, 0, 0, 0, 0);
                return;
            }
            if (used == 1) {
                {
                    int j;
                    for (j = 0; j < num * 2 + 4; j++) {
                        id[j]->rev_flag |= 0xF;
                    }
                }
                state = 1;
                SndCall(0, 8, 0, 0, 0, 0);
                return;
            }
            SndCall(0, 7, 0, 0, 0, 0);
        }
        if (ItemKeyRepCheck(0x01000000ULL,0x100000ULL)) {
            wk->cmd_menu_no--;
        } else if (ItemKeyRepCheck(0x02000000ULL,0x200000ULL)) {
            wk->cmd_menu_no++;
        }
        wk->cmd_menu_no = wk->cmd_menu_no < 0 ? num - 1 : (wk->cmd_menu_no > num - 1 ? 0 : wk->cmd_menu_no);
        if (ItemKeyRepCheck(0x03000000ULL,0x300000ULL)) {
            SndCall(0, 0xA, 0, 0, 0, 0);
        }
        if (cursorOld != wk->cmd_menu_no) {
            int j;
            for (j = 0; j < num; j++) {
                if (j == wk->cmd_menu_no) {
                    id[j * 2 + 4]->be_flag |= 8;
                } else {
                    id[j * 2 + 4]->be_flag &= ~8;
                }
            }
            cursorOld = wk->cmd_menu_no;
        }
        break;
    case 1: {
        int base = 0;
        switch (ItemCommandMode) {
        case 0:
            base = 0x70;
            break;
        case 3:
            base = 0x80;
            break;
        }
        int j;
        for (j = 0; j < 11; j++) {
            sub[j] = ItemIds.unitPtr(base + j, 0x1C);
            sub[j]->be_flag |= 8;
            sub[j]->rev_flag &= 0xF0;
        }
        ItemVectorAdd(&id[wk->cmd_menu_no * 2 + 4]->pos0, &id[wk->cmd_menu_no * 2 + 4]->pParent->pos0, &sub[0]->pos0);
        mode = 2;
    }
        // fall through: the sub menu is processed in the frame that opens it
    case 2:
        if (ItemKeyTrg & 0x0000200040000000ULL) {
            {
                int j;
                for (j = 0; j < 11; j++) {
                    sub[j]->rev_flag |= 0xF;
                }
            }
            mode = 0;
            SndCall(0, 5, 0, 0, 0, 0);
            return;
        }
        if (ItemKeyTrg & 0x80000000ULL) {
            if (ItemCommandMode == 3) {
                if (subSel == 0) {
                    ScreenItems.dumpAll(ItemSelected);
                    SndCall(0, 0xD, 0, 0, 0, 0);
                } else {
                    SndCall(0, 5, 0, 0, 0, 0);
                }
            }
            {
                int j;
                for (j = 0; j < num * 2 + 4; j++) {
                    id[j]->rev_flag |= 0xF;
                }
            }
            {
                int j;
                for (j = 0; j < 11; j++) {
                    sub[j]->rev_flag |= 0xF;
                }
            }
            transit(0, wk);
            return;
        }
        {
            s8 old = subSel;
            if (ItemKeyTrgCheck(0x01000000ULL,0x100000ULL)) {
                subSel--;
            } else if (ItemKeyTrgCheck(0x02000000ULL,0x200000ULL)) {
                subSel++;
            }
            subSel = subSel < 0 ? 0 : (subSel > 1 ? 1 : subSel);
            if (old != subSel) {
                SndCall(0, 0xA, 0, 0, 0, 0);
            }
        }
        sub[5]->be_flag &= ~8;
        sub[7]->be_flag &= ~8;
        if (subSel == 0) {
            sub[5]->be_flag |= 8;
        } else {
            sub[7]->be_flag |= 8;
        }
        break;
    }
    {
        int j;
        for (j = 0; j < 2; j++) {
            ItemFrameMove(wk, j);
        }
    }
}

