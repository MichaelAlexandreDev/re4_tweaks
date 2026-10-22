// Steam file screen. The GameCube source is a logic reference; Steam offsets and calls come from
// the hash-verified bio4.exe disassembly.
// CFLAGS: /O2 /Oy- /GS /MT /EHsc /GR
#include "ss_file.h"
#include "mem.h"
#include "dvd.h"
#include "snd.h"

struct FileIdUnit {
    u8 be_flag;                  // 0x00
    u8 pad_01[0x7A-1];
    u8 texNo;                    // 0x7A
    u8 pad_7B[0x7E-0x7B];
    u16 timer[4];                // 0x7E
    u8 pad_86[4];
    u8 rev_flag;                 // 0x8A
    u8 tex_flag;                 // 0x8B
    u8 pad_8C[0x91-0x8C];
    u8 anima_state;              // 0x91
    u8 pad_92[2];
    Vec pos;                     // 0x94
};
typedef char check_file_id_unit[sizeof(FileIdUnit) == 0xA0 ? 1 : -1];
class FileIdSystem {
public:
    DECOMP_METHOD(u8, FileIdSystem, setCk, (u8 group), (group), 0x0040A61E);
    DECOMP_METHOD(FileIdUnit*, FileIdSystem, unitPtr, (u8 no, u8 group), (no, group), 0x004095F7);
    DECOMP_METHOD(void, FileIdSystem, set, (void* data, u8 a, u8 group, u8 b, u8 c, u8 d),
                  (data, a, group, b, c, d), 0x00404449);
    DECOMP_METHOD(void, FileIdSystem, dispSw, (int group, int on), (group, on), 0x004088F5);
};
DECOMP_GLOBAL(FileIdSystem, FileIds, 0x00C7AB28);

class FileHudSystem {
public:
    DECOMP_METHOD(void, FileHudSystem, dispSw, (int group, int on), (group, on), 0x004088F5);
};
DECOMP_GLOBAL(FileHudSystem, FileHud, 0x00C59788);

class FileLifeMeter {
public:
    DECOMP_METHOD(void, FileLifeMeter, fix, (int on), (on), 0x00406C08);
    DECOMP_METHOD0(void, FileLifeMeter, frameIn, 0x00402F27);
    DECOMP_METHOD0(void, FileLifeMeter, frameOut, 0x0040CEE1);
};
DECOMP_GLOBAL(FileLifeMeter, FileLife, 0x00C50FF8);

class FileMessageControl {
public:
    DECOMP_METHOD(void, FileMessageControl, Delete, (int no), (no), 0x0040CDCE);
    DECOMP_METHOD(void, FileMessageControl, waitEnd, (int no), (no), 0x004034BD);
    DECOMP_METHOD(void, FileMessageControl, setLayout, (int slot, int layout), (slot, layout), 0x0040C2DE);
    DECOMP_METHOD(void, FileMessageControl, setupFont, (int x, int y, void* palette, int flags),
                  (x, y, palette, flags), 0x00401DBB);
    DECOMP_METHOD(void, FileMessageControl, setText,
                  (int no, int x, int y, u32 attr, int trans, int color, int flags),
                  (no, x, y, attr, trans, color, flags), 0x0040E7A5);
};
DECOMP_GLOBAL(FileMessageControl, FileMessages, 0x00C17830);
DECOMP_GLOBAL(const char*, FileLanguage, 0x00C06F40);
DECOMP_FUNC(int, ftol2_sse, (), 0x00A6ECF0);

struct FileGlobal { u8 pad_00[0x54]; u32 flags54; };
DECOMP_GLOBAL(FileGlobal*, FileGlobalPtr, 0x00C06F3C);
DECOMP_GLOBAL(u8, FileSomeFlag, 0x00C7B2D0);
DECOMP_GLOBAL(u8, FileLayoutFlag, 0x00C17934);
DECOMP_FUNC(void, FileExitResourceCommand, (int command), 0x004092E6);
DECOMP_GLOBAL(u32, FileResource, 0x00C7B2C8);
DECOMP_GLOBAL(int, FileReadReq, 0x00C7B330);
DECOMP_GLOBAL(int, FileWait, 0x00C7B334);
DECOMP_GLOBAL(u16, FileTplSelectedId, 0x00C7B33C);
DECOMP_GLOBAL(int, FileTplReadReq, 0x00C7B340);
DECOMP_GLOBAL(u32, FileTplResource, 0x00C7B2CC);
DECOMP_GLOBAL(u32, FileTplDrawArg0, 0x00C7B338);
DECOMP_GLOBAL(u32, FileTplDrawArg1, 0x00C1C4C0);
DECOMP_GLOBAL(f32, FileTplDrawHeight, 0x00C62EA4);
DECOMP_GLOBAL_ARRAY(char, FileTplName, 16, 0x00C1C4C4);
DECOMP_GLOBAL(u32, FileMessageDoneFlags, 0x00C17848);
DECOMP_GLOBAL(u32, FileSystemVariant, 0x00C06F44);
DECOMP_FUNC(u16, FileTemplateIdRaw, (), 0x0076B480);
DECOMP_FUNC(u8, FileLastPageRaw, (), 0x0076C2D0);
DECOMP_FUNC(void, FileSetLangExt3, (char* path), 0x00407F5E);
DECOMP_FUNC(int, FileSprintf, (char* out, const char* format, int value), 0x00A6E37B);
DECOMP_FUNC(void, FileDrawTpl, (void* tpl, int id, u32 x, u32 y, int height, int width,
                                int ot, int priority), 0x004039A9);
DECOMP_FUNC(void, FileIdSubErase, (), 0x004070EA);
DECOMP_FUNC(void, FileIdNumErase, (), 0x00408A99);
DECOMP_FUNC(void, FileIdFreeBuffer, (), 0x00408CAB);
DECOMP_FUNC(void, FileDataFilename, (FileSubScreen* wk, const char* name), 0x00404101);
DECOMP_FUNC(void, FileCameraInit, (FileSubScreen* wk, void* camera), 0x0040EBF1);
DECOMP_FUNC(void, FileLightCreate, (FileSubScreen* wk, void* light), 0x0040DBFC);
DECOMP_FUNC(void, FileIdTexDataLoad, (void* data, int owner), 0x00401F46);
DECOMP_FUNC(void*, FileItemNew, (u32 size), 0x0040E386);
DECOMP_FUNC(void, FileRebuildListsRaw, (), 0x0076B150);
DECOMP_FUNC(void, FileModelFree, (FileSubScreen* wk), 0x00407CA2);
DECOMP_FUNC(void, FileGeneralModelAlloc, (FileSubScreen* wk), 0x004020E0);
DECOMP_FUNC(void, FilePlayerModelInit, (), 0x004026E9);
DECOMP_FUNC(void, FileLightClear, (FileSubScreen* wk), 0x00402AE5);
DECOMP_FUNC(void, FileModelClear, (FileSubScreen* wk), 0x00402D79);
DECOMP_FUNC(void, FileResourceRequest, (u32 resource, int arg), 0x00401915);
DECOMP_FUNC(int, FileResourceReady, (u32 resource), 0x00402CF2);
DECOMP_FUNC(void, FileSetFade, (u32 a, int frames, int c), 0x0040D0A8);
DECOMP_FUNC(void, FileMainMenuInit, (FileSubScreen* wk, int no), 0x00409B56);
DECOMP_FUNC(int, FileMainMenu, (FileSubScreen* wk), 0x004074B9);
DECOMP_FUNC(void, FileDispListRaw, (int rows), 0x0076B8C0);
DECOMP_GLOBAL(unsigned __int64, FileKeyTrg, 0x00C62F10);
DECOMP_GLOBAL(unsigned __int64, FileKeyRep, 0x00C62F20);
DECOMP_FUNC(bool, FileKey2Game, (FileSubScreen* wk), 0x00405466);
DECOMP_FUNC(bool, FileKeyTrgCheck, (u32 low, u32 high), 0x0040EC0F);
DECOMP_FUNC(bool, FileKeyRepCheck, (u32 low, u32 high), 0x0040EDF4);
DECOMP_FUNC(int, FileOpeGetMdtNo, (), 0x0040CBE4);
DECOMP_FUNC(void, FileSpecialMove, (u32 token, int a, int b, int c), 0x0040DF49);
DECOMP_FUNC(int, FileCountARaw, (), 0x0076B050);
DECOMP_FUNC(int, FileCountBRaw, (), 0x0076B0B0);
DECOMP_FUNC(int, FilePageCountRaw, (), 0x0076B3B0);
DECOMP_FUNC(u32, FileMessageAttrRaw, (), 0x0076B450);
DECOMP_FUNC(int, FileIdToNoRaw, (), 0x0076B760);
DECOMP_FUNC(void, FileSetLogMesAddrRaw, (), 0x0076B520);

class FileInventorySystem {
public:
    DECOMP_METHOD(void, FileInventorySystem, get, (u16 item_id, int arg), (item_id, arg), 0x00403F8F);
};
DECOMP_GLOBAL(FileInventorySystem, FileInventory, 0x00C70FD4);

struct FileInfoRow { u8 message_base, color, attr_type, layout; };
struct FileNumberEntry { u16 id; u16 pad; };
DECOMP_GLOBAL_ARRAY(FileInfoRow, FileInfo, 256, 0x00C1BFBC);
DECOMP_GLOBAL_ARRAY(FileNumberEntry, FileNumbersA, 64, 0x00C7B144);
DECOMP_GLOBAL_ARRAY(FileNumberEntry, FileNumbersB, 64, 0x00C7B148);
DECOMP_GLOBAL_ARRAY(u8*, FileLogMessages, 24, 0x00C7B248);
DECOMP_GLOBAL(void*, FileCommonMessageData, 0x00C7158C);
DECOMP_GLOBAL(int, FileCommonMessageReady, 0x00C715A0);
DECOMP_GLOBAL(void*, FileMessagePageData, 0x00C71594);
DECOMP_GLOBAL(int, FileMessagePageReady, 0x00C715A8);

class FileMessageDataControl {
public:
    DECOMP_METHOD(u8, FileMessageDataControl, getMesNum, (int no), (no), 0x00409944);
};
DECOMP_GLOBAL(FileMessageDataControl, FileMessageData, 0x00C71588);

class FileDvdControl {
public:
    DECOMP_METHOD(void, FileDvdControl, readCancel, (int req, int flags), (req, flags), 0x00404C69);
};
DECOMP_GLOBAL(FileDvdControl, FileDvd, 0x00C53118);

static int FileCountA(int category)
{
    int result;
    __asm { mov eax, category
            call FileCountARaw
            mov result, eax }
    return result;
}

static int FileCountB(int category)
{
    int result;
    __asm { mov edx, category
            call FileCountBRaw
            mov result, eax }
    return result;
}

static int FilePageCount(int no)
{
    int result;
    __asm { mov eax, no
            call FilePageCountRaw
            mov result, eax }
    return result;
}

// Steam helper ABI: page in EAX, zero-based file selector in EDX, u16 result in AX.
static u16 FileTemplateId(int page, int selector)
{
    u16 result;
#ifdef DECOMP_HARNESS
    __asm { mov eax, page
            mov edx, selector
            call dword ptr [FileTemplateIdRaw]
            mov result, ax }
#else
    __asm { mov eax, page
            mov edx, selector
            call FileTemplateIdRaw
            mov result, ax }
#endif
    return result;
}

// Steam helper ABI: FileSubScreen* in EAX, boolean result in AL.
static u8 FileLastPage(FileSubScreen* wk)
{
    u8 result;
#ifdef DECOMP_HARNESS
    __asm { mov eax, wk
            call dword ptr [FileLastPageRaw]
            mov result, al }
#else
    __asm { mov eax, wk
            call FileLastPageRaw
            mov result, al }
#endif
    return result;
}

static u32 FileMessageAttr(int kind)
{
    u32 result;
    __asm { mov ecx, kind
            call FileMessageAttrRaw
            mov result, eax }
    return result;
}

static int FileIdToNo(u16 id)
{
    int result;
    __asm { movzx eax, id
            call FileIdToNoRaw
            mov result, eax }
    return result;
}

static void FileSetLogMesAddr(FileSubScreen* wk)
{
    __asm { mov eax, wk
            call FileSetLogMesAddrRaw }
}

static void FileDispList(FileSubScreen* wk, int rows)
{
#ifdef DECOMP_HARNESS
    __asm {
        mov eax, wk
        push rows
        call dword ptr [FileDispListRaw]
        add esp, 4
    }
#else
    __asm {
        mov eax, wk
        push rows
        call FileDispListRaw
        add esp, 4
    }
#endif
}

struct FileModel { u8 pad_00[0x158]; f32 alpha; };
DECOMP_GLOBAL(FileModel*, FileModel0, 0x00C7BA6C);
DECOMP_GLOBAL(FileModel*, FileModel1, 0x00C7BA64);
DECOMP_GLOBAL(FileModel*, FileModel2, 0x00C7BA68);

// FUNCTION: BIO4 0x0076B740
// STATUS: matching
void SsFileInit::init(FileSubScreen* wk)
{
    state = wk->open_flag == 0x40 ? 2 : 0;
}

// FUNCTION: BIO4 0x0076C170
// STATUS: matching
void FileSelect::init(FileSubScreen*)
{
}

// FUNCTION: BIO4 0x0076C180
// STATUS: matching
void FileSelect::quit(FileSubScreen*)
{
}

// FUNCTION: BIO4 0x0076D430
// STATUS: stub
void FileSelect::move(FileSubScreen* wk)
{
    SsFileWork* fw = wk->file;
    u8 draw_list = 1;
    state = 0;

    if (FileKey2Game(wk) || (fw->extraF && FileKeyTrgCheck(0x20000000, 0))) {
        state = 1;
        if (fw->extra11) FileSpecialMove(fw->extra14, 4, 60, 0);
        goto finish;
    }

    if (fw->mode == 1) {
        const u32 back_low = (u32)FileKeyTrg & 0x40000000;
        const u32 back_high = (u32)(FileKeyTrg >> 32) & 0x2000;
        const u32 back_bits = back_low | back_high;
        if (back_bits || FileKeyTrgCheck(0x01000000, back_bits)) {
            if (fw->cursor == 0 || back_bits) {
                fw->mode = 0;
                SndCall(0, 0x20, 0, 0, 0, 0);
                draw_list = 0;
                goto finish;
            }
        }

        if (FileKeyTrgCheck(0x80000000, 0)) {
            if (fw->cursor == 0 && FileCountA(fw->category) != FileCountB(fw->category)) {
                const int mdt = FileOpeGetMdtNo();
                if (mdt == 0x18) goto select_error;
                for (int i=0; i<16; ++i) FileMessages.Delete(i);
                FileSetLogMesAddr(wk);
                FileMessagePageData = FileLogMessages[mdt];
                FileMessagePageReady = 1;
                fw->messageBase = 0;
                fw->color = 0;
                fw->messageAttr = FileMessageAttr(3);
                fw->layout = 2;
                fw->pageCount = FileMessageData.getMesNum(2);
                fw->page = 0;
                fw->fileNo = 0;
                fw->extraE = 1;
                transitOriginal(0, wk);
                SndCall(0, 4, 0, 0, 0, 0);
                draw_list = 0;
                goto finish;
            }

            {
                const int table_index = (s8)fw->category * 16 + (s8)fw->cursor;
                const u16 item_id = FileCountA(fw->category) != FileCountB(fw->category)
                    ? FileNumbersA[table_index].id : FileNumbersB[table_index].id;
                if (item_id == 0xFFFF) goto select_error;
                for (int i=0; i<16; ++i) FileMessages.Delete(i);
                u8* data = wk->pFileDat;
                FileMessagePageData = data + *(u32*)(data + 0x14);
                FileMessagePageReady = 1;
                const int file_no = FileIdToNo(item_id);
                const FileInfoRow* info = &FileInfo[file_no];
                fw->messageBase = info->message_base;
                fw->color = info->color;
                fw->messageAttr = FileMessageAttr(info->attr_type);
                fw->layout = info->layout;
                fw->pageCount = (u8)FilePageCount(file_no - 1);
                fw->page = 0;
                fw->fileNo = (u8)file_no;
                fw->extraE = 0;
                if ((u8)FileLanguage[8] == 1 && fw->color == 0x3F)
                    fw->color = 0x9D;
                fw->flag10 = (item_id >= 0x106 && item_id <= 0x10A) ? 1 : 0;
                transitOriginal(0, wk);
                SndCall(0, 4, 0, 0, 0, 0);
                draw_list = 0;
                goto finish;
            }
        }

        {
            const s8 old_cursor = fw->cursor;
            if (FileKeyRepCheck(0x01000000, 0)) --fw->cursor;
            if (FileKeyRepCheck(0x02000000, 0)) ++fw->cursor;
            if (fw->cursor < 0) fw->cursor = 0;
            else {
                const int last = FileCountB(fw->category) - 1;
                if (fw->cursor > last) fw->cursor = last;
            }
            if (old_cursor != fw->cursor) SndCall(0, 0x20, 0, 0, 0, 0);
            if (fw->cursor > fw->scroll + 4) fw->scroll = fw->cursor - 4;
            if (fw->cursor < fw->scroll) fw->scroll = fw->cursor;
        }
    } else if (fw->mode == 0) {
        const u32 back_low = (u32)FileKeyTrg & 0x40000000;
        const u32 back_high = (u32)(FileKeyTrg >> 32) & 0x2000;
        const u32 back_bits = back_low | back_high;
        if (back_bits || FileKeyTrgCheck(0x01000000, back_bits)) {
            if (wk->open_flag != 0x40) {
                fw->mode = 2;
                state = 2;
            }
            goto finish;
        }
        if (FileKeyTrgCheck(0x80000000, 0) || FileKeyTrgCheck(0x02000000, 0)) {
            fw->mode = 1;
            SndCall(0, 0x20, 0, 0, 0, 0);
            goto finish;
        }
        if (FileGlobalPtr->flags54 & 0x1000) goto finish;
        {
            const s8 old_category = fw->category;
            if (FileKeyRepCheck(0x08000000, 0)) --fw->category;
            if (FileKeyRepCheck(0x04000000, 0)) ++fw->category;
            if (*(u16*)((u8*)FileGlobalPtr + 0x4F9E) != 0) {
                if (fw->category < 0) fw->category = 0;
                else if (fw->category > 2) fw->category = 2;
            } else if ((s8)wk->stage_no > 0) {
                if (fw->category < 0) fw->category = 0;
                else if (fw->category >= (s8)wk->stage_no)
                    fw->category = (s8)wk->stage_no - 1;
            } else {
                fw->category = 0;
            }
            if (old_category != fw->category) {
                fw->scroll = 0;
                fw->cursor = 0;
                SndCall(0, 0x21, 0, 0, 0, 0);
            }
        }
    }
    goto finish;

select_error:
    SndCall(0, 0x24, 0, 0, 0, 0);

finish:
    if (FileSomeFlag) {
        FileGlobalPtr->flags54 &= ~0x400;
        FileSomeFlag = 0;
        return;
    }
    if (draw_list) FileDispList(wk, 5);
}

// FUNCTION: BIO4 0x0076B870
// STATUS: stub
static u8 sscrn_file_out(FileSubScreen*)
{
    return (u8)(FileIds.unitPtr(0, 0x19)->anima_state & 1);
}

static void* FileArchivePtr(u8* archive, int index)
{
    return archive + *(u32*)(archive + index * 4);
}

// FUNCTION: BIO4 0x0076C310
// STATUS: stub
void MessageDisplay::init(FileSubScreen* wk)
{
    FileIdUnit* anchor = FileIds.unitPtr(0xFE, 0x1E);
    x = (s16)((anchor->pos.x + 320.0) * 0.8f);
    y = (s16)((240.0 - anchor->pos.y) * 0.8f);
    const u8 language = (u8)FileLanguage[8];
    if (language == 0 || language == 6 || language == 7)
        FileMessages.setupFont(0x1C, 0x1C, FileArchivePtr(wk->pFileDat, 4), 2);

    switch (wk->file->layout) {
    case 0:
        FileMessages.setLayout(0, 7);
        break;
    case 1:
        FileMessages.setLayout(0, 8);
        break;
    case 2:
        FileMessages.setLayout(0, 4);
        FileMessages.setLayout(1, 4);
        wk->layout_flag |= 0xFF;
        FileLayoutFlag |= 0xFF;
        break;
    }
    if (wk->file->flag10 == 0) {
        FileMessages.setText(wk->file->messageBase + wk->file->page, x, y,
                             wk->file->messageAttr, 0, wk->file->color, 2);
    }
    FileIds.unitPtr(0, 0x1D)->be_flag |= 8;
    FileIds.unitPtr(0, 0x1E)->be_flag |= 8;
    FileIds.unitPtr(0, 0x1E)->rev_flag &= 0xF0;
    FileIdUnit* destination = FileIds.unitPtr(0xFC, 0x1E);
    FileIdUnit* source;
    if (wk->file->layout == 1)
        source = FileIds.unitPtr(0xFD, 0x1E);
    else
        source = FileIds.unitPtr(0xFB, 0x1E);
    destination->pos = source->pos;
    state = 0;
    tpl_state = 0;
    tpl_first = 1;
}

// FUNCTION: BIO4 0x0076DDB0
// STATUS: stub
void MessageDisplay::move(FileSubScreen* wk)
{
    SsFileWork* fw = wk->file;
    const u8 page_before_input = fw->page;
    extra18 = 0;

    if (state == 0) {
        if (FileKeyTrg & 0x40000000ULL) {
            if (fw->extraF) extra18 = 1;
            if (tpl_state == 2) FileDvd.readCancel(FileTplReadReq, 0x40);
            else {
                tpl_state = 3;
                tpl_first = 0;
            }
            state = 1;
        } else {
            if (FileKeyTrg & 0x80000000ULL) {
                if (fw->extraE == 1) FileMessages.waitEnd(0);
                if (FileLastPage(wk)) state = 1;
                else ++fw->page;
            }
            if (state == 0) {
                if (FileKeyTrg & 0x08000000ULL) {
                    if (fw->page != 0) --fw->page;
                } else if ((FileKeyTrg & 0x04000000ULL) && !FileLastPage(wk)) {
                    ++fw->page;
                }
            }

            if (page_before_input != fw->page) {
                if (fw->fileNo != 1 && fw->fileNo != 3 && fw->fileNo != 9) {
                    FileMessages.setText(fw->messageBase + fw->page, x, y,
                                         fw->messageAttr, 0, fw->color, 2);
                }
                // Steam localizes each manual page, splits it at LF, and feeds each line and its
                // file metadata through a parser. That path is not implemented here, so this
                // manual-page behavior remains incomplete under STATUS: stub.
                SndCall(0, 0x22, 0, 0, 0, 0);
            }
        }
    } else if (state == 1) {
        FileIds.unitPtr(0, 0x1E)->rev_flag |= 0x0F;
        SndCall(0, 0x23, 0, 0, 0, 0);
        state = 2;
    } else if (state == 2) {
        if ((s16)FileIds.unitPtr(0, 0x1E)->timer[2] == 0) transit(0, wk);
    }

    const int selector = (int)fw->fileNo - 1 + (fw->flag10 == 1 ? 0x15 : 0);
    const u16 selected_tpl = FileTemplateId(fw->page, selector);
    const u16 previous_tpl = FileTplSelectedId;
    FileTplSelectedId = selected_tpl;

    switch (tpl_state) {
    case 0:
        if (previous_tpl != selected_tpl || tpl_first == 1) tpl_state = 1;
        if (tpl_first == 0 && selected_tpl != 0xFFFF && FileResourceReady(FileTplResource)) {
            FileDrawTpl(wk->pTplDat, 0, FileTplDrawArg0, FileTplDrawArg1,
                        (int)FileTplDrawHeight, 0x150, 0x13, 3);
        }
        break;
    case 1:
        if (tpl_first == 1) tpl_first = 0;
        if (selected_tpl != 0xFFFF) {
            FileSprintf(FileTplName, "SS/___/f%02dA.tpl", fw->fileNo);
            FileSetLangExt3(FileTplName + 3);
            FileTplName[10] = (char)(selected_tpl + 0x60);
            FileTplResource = 0;
            int suffix = -1;
            switch ((s8)FileTplName[4]) {
            case 'e': suffix = 0; break;
            case 'h': suffix = 1; break;
            case 'm': suffix = 2; break;
            case 'o': suffix = 3; break;
            case 'q': suffix = 4; break;
            case 'r': suffix = 5; break;
            case 's':
                if ((s8)FileTplName[5] == 't') suffix = 6;
                else if ((s8)FileTplName[5] == 's') suffix = 9;
                break;
            }
            if (suffix >= 0) {
                int id = (((s8)FileTplName[8] - '0') << 4) |
                         ((s8)FileTplName[9] - '0');
                id = (id << 4) | ((s8)FileTplName[10] - 'W');
                FileTplResource = ((u32)id << 4) | 0x3F000000 | (u32)suffix;
            }
            FileTplReadReq = DvdReadN(FileTplName, wk->pTplDat, 0, 0, 0, 0x10,
                                      "src\\ss_file.cpp", 0x91F);
            FileResourceRequest(FileTplResource, 0);
            tpl_state = 2;
        } else {
            tpl_state = 3;
        }
        break;
    case 2:
        if (Dvd.ReadCheckOne(FileTplReadReq, 0, 0, 0) == 1 &&
            FileResourceReady(FileTplResource)) {
            tpl_first = 0;
            tpl_state = 0;
            FileTplResource = 0;
        } else if (previous_tpl != selected_tpl) {
            FileDvd.readCancel(FileTplReadReq, 0x40);
            tpl_state = 1;
        }
        break;
    }

    int page_digits = (int)fw->page + 1;
    int count_digits = fw->pageCount;
    const int page_ones = page_digits % 10;
    const int page_tens = (page_digits / 10) % 10;
    const int count_ones = count_digits % 10;
    const int count_tens = (count_digits / 10) % 10;
    FileIdUnit* tens_page = FileIds.unitPtr(0x10, 0x1E);
    FileIdUnit* ones_page = FileIds.unitPtr(0x11, 0x1E);
    FileIdUnit* tens_count = FileIds.unitPtr(0x12, 0x1E);
    FileIdUnit* ones_count = FileIds.unitPtr(0x13, 0x1E);
    if (fw->pageCount <= 9) {
        tens_page->be_flag &= ~8;
        tens_count->be_flag &= ~8;
        ones_page->texNo = (u8)page_ones;
        ones_count->texNo = (u8)count_ones;
        ones_page->tex_flag |= 2;
        ones_count->tex_flag |= 2;
    } else {
        tens_page->be_flag |= 8;
        tens_count->be_flag |= 8;
        tens_page->texNo = (u8)page_tens;
        ones_page->texNo = (u8)page_ones;
        tens_count->texNo = (u8)count_tens;
        ones_count->texNo = (u8)count_ones;
        tens_page->tex_flag |= 2;
        ones_page->tex_flag |= 2;
        tens_count->tex_flag |= 2;
        ones_count->tex_flag |= 2;
    }

    if (fw->page == 0) FileIds.unitPtr(1, 0x1E)->be_flag &= ~8;
    else FileIds.unitPtr(1, 0x1E)->be_flag |= 8;

    const bool last_page = FileLastPage(wk) != 0;
    FileIdUnit* next_arrow = FileIds.unitPtr(2, 0x1E);
    FileIdUnit* end_mark = FileIds.unitPtr(3, 0x1E);
    if (last_page) {
        next_arrow->be_flag &= ~8;
        end_mark->be_flag |= 8;
    } else {
        next_arrow->be_flag |= 8;
        end_mark->be_flag &= ~8;
    }
    if (fw->extraE != 0 || fw->flag10 != 0)
        FileIds.unitPtr(0xFC, 0x1E)->be_flag &= ~8;

    // C06F44 also selects manual strings and a platform-specific frame marker; the marker's
    // target unit/position helper at 0x40551F is unresolved, so that write stays out of this stub.
}

// FUNCTION: BIO4 0x0076CE70
// STATUS: stub
void SsFileInit::move(FileSubScreen* wk)
{
    FileSubScreen* screen = wk;
    switch (state) {
    case 0:
        if (screen->scrn_out_func(screen) != 1) break;
        if (screen->menu_old == 2) screen->trans_off = 1;
        FileIdSubErase();
        FileIdNumErase();
        FileIdFreeBuffer();
        FileIds.set(screen->pCmmn + 0x30, 0xFF, 0x14, 0x0C, 6, 0);
        FileResource = 0;
        FileWait = 0;
        ++state;
        break;
    case 1:
        if (--FileWait >= 0) break;
        state = 2;
        break;
    case 2: {
        FileHud.dispSw(0x21, 1);
        FileHud.dispSw(0x32, 1);
        FileIds.dispSw(2, 1);
        FileDataFilename(screen, "ss_file.dat");
        FileReadReq = DvdReadN(screen->filename, screen->pSwitchDat, 0, 0, 0, 0x10,
                               "src\\ss_file.cpp", 0x303);
        if (FileReadReq <= 0) break;
        if (screen->menu_old == 2 && screen->open_flag != 0x40) {
            FileModelFree(screen);
            FileGeneralModelAlloc(screen);
            FilePlayerModelInit();
            FileLightClear(screen);
            FileLife.fix(1);
            FileLife.frameIn();
        } else {
            FileModelClear(screen);
        }
        screen->trans_off = 0;
        {
            const char language = screen->filename[4];
            const char variant = screen->filename[5];
            if (language == 'e') FileResource = 0x38000003;
            else if (language == 'h') {
                if (variant == 't') FileResource = 0x38000006;
                else if (variant == 's') FileResource = 0x38000009;
            } else if (language == 'm') FileResource = 0x38000001;
            else if (language == 'o') FileResource = 0x38000000;
            else if (language == 'q') FileResource = 0x38000002;
            else if (language == 'r') FileResource = 0x38000005;
            else if (language == 's') FileResource = 0x38000004;
        }
        FileResourceRequest(FileResource, 0);
        ++state;
        // The source read and resource request are polled in the same frame.
    }
    case 3: {
        u32 status;
        if (Dvd.ReadCheckOne(FileReadReq, &status, (u32*)&wk, 0) != 1) break;
        if (!FileResourceReady(FileResource)) break;
        screen->pFileDat = screen->pSwitchDat;
        ++state;
        FileResource = 0;
        if (screen->open_flag == 0x40) FileSetFade(0, 5, 0);
        transit(0, screen);
        break;
    }
    case 4:
        if (screen->open_flag == 0x40) FileSetFade(0, 5, 0);
        transit(0, screen);
        break;
    }
    if (screen->menu_old == 2 && screen->open_flag != 0x40) {
        if (FileModel0) FileModel0->alpha = 0.0f;
        if (FileModel1) FileModel1->alpha = 0.0f;
        if (FileModel2) FileModel2->alpha = 0.0f;
    }
}

// FUNCTION: BIO4 0x0076C530
// STATUS: matching
void MessageDisplay::quit(FileSubScreen*)
{
    for (int i=0; i<16; ++i) FileMessages.Delete(i);
    FileExitResourceCommand(0x3F);
    FileGlobalPtr->flags54 |= 0x400;
    FileSomeFlag = 1;
    FileIds.unitPtr(0, 0x1D)->be_flag &= ~8;
}

// FUNCTION: BIO4 0x0076CC40
// STATUS: matching
void SsFileMain::quit(FileSubScreen* wk)
{
    if (select) delete select;
    if (display) delete display;
    cur = 0;
    for (int i=0; i<16; ++i) FileMessages.Delete(i);
    Mem_free(wk->pTplDat);
    Mem_free(wk->file);
    FileExitResourceCommand(0x3F);
    FileIds.unitPtr(0, 0x19)->rev_flag |= 1;
    if (wk->menu_next == 2) {
        FileLife.frameOut();
        wk->alpha_flag = 1;
    }
    wk->scrn_out_func = sscrn_file_out;
}

// FUNCTION: BIO4 0x0076D230
// STATUS: stub
void SsFileMain::move(FileSubScreen* wk)
{
    FileWidget* child = cur;
    if (!child) return;
    switch (state) {
    case 0:
        child->move(wk);
        next = child->cur;
        if (child == select) {
            if (select->state == 2) {
                state = 1;
                wk->close_flag |= 0x10;
                FileMainMenuInit(wk, 1);
                SndCall(0, 0xA, 0, 0, 0, 0);
            } else if (select->state == 1) {
                wk->close_flag |= 0x10;
                transitOriginal(0, wk);
            }
        } else if (child == display && display->extra18 == 1) {
            wk->close_flag |= 0x10;
            transitOriginal(0, wk);
        }
        cur = next;
        break;
    case 1:
        if (FileMainMenu(wk)) {
            switch (wk->menu_no) {
            case 0: transitOriginal(2, wk); break;
            case 1: transitOriginal(1, wk); break;
            case 2: transitOriginal(4, wk); break;
            case 3:
                wk->file->mode = 0;
                FileMainMenuInit(wk, 0);
                state = 0;
                break;
            case 4: transitOriginal(0, wk); break;
            }
        }
        if (FileKeyTrg & 0x02000000ULL) {
            wk->file->mode = 0;
            FileMainMenuInit(wk, 0);
            state = 0;
            SndCall(0, 0x20, 0, 0, 0, 0);
        }
        FileDispList(wk, 5);
        break;
    }
    if (sound_wait && ++sound_count > 10) {
        sound_wait = 0;
        SndCall(0, 0x1F, 0, 0, 0, 0);
    }
}

// FUNCTION: BIO4 0x0076C690
// STATUS: stub
void SsFileMain::init(FileSubScreen* wk)
{
    FileSelect* new_select = (FileSelect*)FileItemNew(0x14);
    select = new_select ? new_select->construct() : 0;
    MessageDisplay* new_display = (MessageDisplay*)FileItemNew(0x1C);
    display = new_display ? new_display->construct() : 0;
    if (select && display) {
        select->connect(0, display);
        display->connect(0, select);
    }

    FileCameraInit(wk, (u8*)FileGlobalPtr + 0x74);
    FileIdTexDataLoad(FileArchivePtr(wk->pFileDat, 0x18), 9);
    if (FileIds.setCk(0x14) == 0)
        FileIds.set(FileArchivePtr(wk->pCmmn, 0x0C), 0xFF, 0x14, 0x0C, 6, 0);
    FileIds.set(FileArchivePtr(wk->pFileDat, 7), 0xFF, 0x19, 9, 2, 0);
    FileIds.set(FileArchivePtr(wk->pFileDat, 9), 0xFF, 0x1D, 0x13, 4, 0);
    FileIds.set(FileArchivePtr(wk->pFileDat, 8), 0xFF, 0x1E, 0x13, 2, 0);
    FileIds.unitPtr(0xFC, 0x19)->be_flag &= ~8;
    FileIds.unitPtr(0, 0x1D)->be_flag &= ~8;
    FileIds.unitPtr(0, 0x1E)->be_flag &= ~8;
    FileIds.unitPtr(0, 0x1E)->rev_flag |= 0x0F;
    FileIds.unitPtr(4, 0x1E)->be_flag &= ~8;
    FileLightCreate(wk, wk->pCmmn + 0x48);
    if (wk->menu_old == 2 && wk->open_flag != 0x40) {
        wk->alpha_flag = 0;
        wk->alpha_cnt = 10;
    }

    FileCommonMessageData = FileArchivePtr(wk->pCmmn, 0x14);
    FileCommonMessageReady = 1;
    FileMessagePageData = FileArchivePtr(wk->pFileDat, 0x14);
    FileMessagePageReady = 1;
    FileMainMenuInit(wk, 0);
    state = 0;

    wk->file = (SsFileWork*)Mem_alloc(0x18, 0, 0, 1, 0x0D);
    wk->file->mode = 0;
    wk->file->category = 0;
    wk->file->scroll = 0;
    wk->file->cursor = 0;
    if (FileGlobalPtr->flags54 & 0x1000) {
        wk->file->category = 3;
    } else {
        const s8 stage = wk->stage_no;
        if (stage >= 1 && stage <= 3) wk->file->category = stage - 1;
        else if (stage == 5) wk->file->category = 3;
        else wk->file->category = 0;
    }
    wk->pTplDat = Mem_alloc(0x20800, 0, 0, 1, 0x0D);
    FileMessages.setLayout(0, 2);

    if (wk->open_flag == 0x40) {
        SsFileWork* fw = wk->file;
        const u16 item_id = wk->open_file_id;
        const int file_no = FileIdToNo(item_id);
        FileInventory.get(item_id, 0);
        wk->unknown_40 = 1;
        const u8 language = (u8)FileLanguage[8];
        if (language == 0 || language == 6 || language == 7)
            FileMessages.setupFont(0x1C, 0x1C, FileArchivePtr(wk->pFileDat, 4), 2);

        const FileInfoRow* info = &FileInfo[file_no];
        fw->messageBase = info->message_base;
        fw->color = info->color;
        fw->messageAttr = FileMessageAttr(info->attr_type);
        fw->layout = info->layout;
        fw->pageCount = (u8)FilePageCount(file_no - 1);
        fw->page = 0;
        fw->mode = 0;
        fw->scroll = 0;
        fw->cursor = 0;
        fw->fileNo = (u8)file_no;
        fw->extraE = 0;
        fw->extraF = (item_id == 0xAC || item_id == 0xAE || item_id == 0xB4) ? 1 : 0;
        if (item_id >= 0x106 && item_id <= 0x10A) {
            fw->flag10 = 1;
            fw->extra11 = 1;
            fw->extra14 = fw->makeSpecialToken(0, 0x44, 0x80000003, 0, 0, 0.0f);
        } else {
            fw->flag10 = 0;
            fw->extra11 = 0;
        }
        cur = display;
        if (display) display->init(wk);
        sound_wait = 0;
    } else {
        cur = select;
        SndCall(0, 0x1E, 0, 0, 0, 0);
        sound_count = 0;
        sound_wait = 1;
        wk->file->extra11 = 0;
    }
    FileRebuildListsRaw();
}
