// Enemy module table of the Steam build (src\read.cpp on PC). Logic follows
// references/re4-gamecube/src/game/read.cpp; Steam differences are noted.

#include "dvd.h"
#include "global.h"
#include "mem.h"
#include "read.h"
#include "sce_sys.h"

// Module link helpers reached through the incremental-link table; names are
// provisional until their bodies are identified.
DECOMP_FUNC(void, DLL_Unlink, (void* module), 0x0040E24B);
DECOMP_FUNC(void, DLL_Link, (void* module, void* bss, int a2, int dll), 0x0040E5CF);

// Steam-only file cache calls made after a read in game mode (0x100), and
// two more of the same subsystem called when the enemy modules are released.
DECOMP_FUNC(void, FileCacheFunc958EC0, (int a0, int a1), 0x004074C8);
DECOMP_FUNC(void, FileCacheFunc958F30, (int a0, int a1, int a2), 0x0040EE2B);
DECOMP_FUNC(void, FileCacheRequest, (int file, int a1), 0x0040B9B5);
DECOMP_FUNC(int, FileCacheReady, (int file), 0x00406C99);
DECOMP_FUNC(void, FileCacheRelease, (int file, int a1), 0x00409110);

// Ashley's module. Steam adds the Armor costume (module 0x0C) and forces the
// normal module in Separate Ways; the GameCube only knew 3 and 5.
static u8 checkAshleyId(u8 id)
{
    if (id == 3 || id == 5 || id == 0x0C) {
        if (pG->subCostume == 0) {
            id = 3;
        } else {
            id = (pG->subCostume == 2) ? 0x0C : 5;
        }
        if (pG->flags_SYSTEM[0] & SYS_PS2_ADA_GAME) {
            id = 3;
        }
    }
    return id;
}

// FUNCTION: BIO4 0x006AD4D0
// STATUS: matching
ReadModule* SearchEmModule(u8 id)
{
    id = checkAshleyId(id);
    for (u32 i = 0; i < (u32) EM_READ_MODULE_COUNT; i++) {
        if (EmReadModule[i].id == id) {
            return &EmReadModule[i];
        }
    }
    return NULL;
}

// FUNCTION: BIO4 0x006AD570
// STATUS: matching
ReadModule* pullEmModule()
{
    for (int i = 0; i < EM_READ_MODULE_COUNT; i++) {
        if (EmReadModule[i].pArc == NULL) {
            return &EmReadModule[i];
        }
    }
    return NULL;
}

// FUNCTION: BIO4 0x006ADAA0
// STATUS: matching
void InitModule(ReadModule* m)
{
    DECOMP_HOOK(EmModuleRelease, (m));
    if (m->pModule != NULL && (m->flag & 2)) {
        DLL_Unlink(m->pModule);
    }
    if (m->pArc != NULL && (m->flag & 4)) {
        if (m->flag & 8) {
            Debug_free(m->pArc);
        } else {
            Mem_free(m->pArc);
        }
    }
    if (m->pModule != NULL && (m->flag & 1)) {
        Mem_free(m->pModule);
    }
    memclr_asm(m, sizeof(ReadModule));
}

// Releases every enemy module (room and chapter changes). Steam first resets
// part of its file cache; the GameCube only ran the loop.
// FUNCTION: BIO4 0x006AE130
// STATUS: matching
void EmReadInit()
{
    FileCacheFunc958EC0(0, 2);
    FileCacheFunc958F30(1, 2, 1);
    for (int i = 0; i < EM_READ_MODULE_COUNT; i++) {
        InitModule(&EmReadModule[i]);
    }
}

static void readWait()
{
    if (SceSys.checkCTaskRange() == 1) {
        SceSys.m_init_loop_flag = 1;
        SceSleep(1);
        SceSys.m_init_loop_flag = 0;
    } else {
        TaskSleep(1);
    }
}

// Reads the data file and, separately, the module file of enemy `id` for the
// current character. Steam reads the module with its own request instead of
// splitting it out of the data file, waits on the file cache in game mode
// and no longer logs failures.
// FUNCTION: BIO4 0x006ADB40
// STATUS: functional
static bool readEmData(ReadModule* m, int id, void* addr, u32 size)
{
    DvdReadInfo info;
    u32 len;
    ReadFile* e;
    const char* name;
    int req;
    int ret;
    int mode;
    void* pArc;
    void* pModule = NULL;
    u32 bssSize = 0;
    u32 newSize;

    switch (pG->pl_type) {
    case 0:
    case 1:
    default:
        e = &EmFileTbl[id];
        break;
    case 3:
    case 5:
        e = &EmFileTbl_Wesker[id];
        break;
    case 2:
        e = &EmFileTbl_Ada[id];
        break;
    case 4:
        e = &EmFileTbl_Klauser[id];
        break;
    }
    if (e->file == 0) {
        return false;
    }
    name = FileTbl[e->file].name;
    if (Dvd.FileExistCheck(name, &len) < 0) {
        return false;
    }
    mode = (pG->Rno0 == 2) ? 0x100 : 1;
    if (addr == NULL) {
#line 1129 "src\\read.cpp"
        req = DvdReadN(name, NULL, 0, 0, 0, mode | 0x8004, __FILE__, __LINE__);
    } else {
#line 1132 "src\\read.cpp"
        req = DvdReadN(name, addr, 0, 0, 0, mode | 0x8000, __FILE__, __LINE__);
    }
    while ((ret = Dvd.ReadCheck(req, &info)) != 1) {
        if (ret < 0) {
            return false;
        }
        readWait();
    }
    if (mode == 0x100) {
        FileCacheRequest(e->file, 1);
        while (FileCacheReady(e->file) == 0) {
            readWait();
        }
    } else {
        FileCacheRelease(e->file, 1);
    }
    len = info.size[0][0];
    if (addr == NULL) {
        m->flag |= 4;
        pArc = (void*) info.addr[0][0];
        if (len < size) {
            void* old = pArc;
            newSize = size;
            Mem_free(old);
            pArc = Mem_alloc(size, NULL, 0, 1, 0xD);
            if (pArc != old) {
                return false;
            }
        } else {
            newSize = len;
        }
    } else {
        m->flag &= ~4;
        pArc = addr;
        newSize = len;
    }
    if (e->dll != 0) {
        name = FileTbl[e->dll].name;
        if (addr == NULL) {
#line 1274 "src\\read.cpp"
            req = DvdReadN(name, NULL, 0, 0, 0, mode | 4, __FILE__, __LINE__);
        } else {
            pModule = (u8*) pArc + newSize;
#line 1277 "src\\read.cpp"
            req = DvdReadN(name, pModule, 0, 0, 0, mode | 0x10, __FILE__, __LINE__);
        }
        while ((ret = Dvd.ReadCheckOne(req, &bssSize, NULL, &pModule)) != 1) {
            if (ret < 0) {
                return false;
            }
            readWait();
        }
        if (addr == NULL) {
            m->flag |= 1;
        } else {
            m->flag &= ~1;
        }
    }
    m->id = id;
    m->pArc = pArc;
    m->size = newSize;
    m->pModule = (DLLHeader*) pModule;
    m->bssSize = bssSize;
    return true;
}

// Links the module read for enemy `id` and runs its prolog, which sets
// EmInitFunc and the family's Em10SetFunc. Steam dropped the BSS size check.
// FUNCTION: BIO4 0x006AE040
// STATUS: functional
static void setEmModule(ReadModule* m, int id)
{
    ReadFile* e;

    switch (pG->pl_type) {
    case 0:
    case 1:
    default:
        e = &EmFileTbl[id];
        break;
    case 3:
    case 5:
        e = &EmFileTbl_Wesker[id];
        break;
    case 2:
        e = &EmFileTbl_Ada[id];
        break;
    case 4:
        e = &EmFileTbl_Klauser[id];
        break;
    }
    if (m->pModule != NULL) {
        if (!(m->flag & 2)) {
            DLL_Link(m->pModule, (m->pModule->bssSize != 0) ? m : NULL, 0, e->dll);
            m->flag |= 2;
        }
        DECOMP_HOOK(EmModulePrologBefore, (m));
        m->pModule->prolog();
        DECOMP_HOOK(EmModulePrologAfter, (m));
        m->pInitFunc = EmInitFunc;
    } else {
        m->pModule = NULL;
        m->flag &= ~2;
        m->pInitFunc = NULL;
    }
}

static void* readEm(int id, void* data_addr, u32 malloc_size);

// The archive of enemy module `id`: the loaded slot (its init function
// becomes EmInitFunc), or loads it (readEm).
// FUNCTION: BIO4 0x006AE8D0
// STATUS: functional
void* EmReadSearch(u8 id, void* data_addr, u32 malloc_size)
{
    ReadModule* m;

    id = checkAshleyId(id);
    m = SearchEmModule(id);
    if (m != NULL) {
        EmInitFunc = m->pInitFunc;
        return m->pArc;
    }
    return readEm(id, data_addr, malloc_size);
}

// Loads enemy module `id` into a free slot with all display flags forced on
// during the load (Steam also clears 0x800 and 0x2000). Returns the archive.
// FUNCTION: BIO4 0x006AE800
// STATUS: functional
static void* readEm(int id, void* data_addr, u32 malloc_size)
{
    u32 flags = pG->Disp_flg;
    ReadModule* m;

    pG->Disp_flg = 0xFFFFFFFF;
    pG->Disp_flg &= ~0x800;
    pG->Disp_flg &= ~0x2000;
    m = pullEmModule();
    if (m == NULL) {
        return NULL;
    }
    if (!readEmData(m, id, data_addr, malloc_size)) {
        pG->Disp_flg = flags;
        return NULL;
    }
    setEmModule(m, id);
    pG->Disp_flg = flags;
    return m->pArc;
}
