// Steam window-model initializers. Guide: references/re4-gamecube/src/game/et00.cpp.
// Integer/pointer code only; no imported GameCube addresses or object layouts.
#include "et00.h"

class Et00Window : public cEmWindow {
public:
    DECOMP_METHOD(void, Et00Window, setEff, (u8 effId), (effId), 0x00404A8E);
    DECOMP_METHOD0(u16, Et00Window, ChkStatus, 0x0040ABC8);
    DECOMP_METHOD0(u8, Et00Window, SetBreakModel, 0x0040391D);
};
DECOMP_FUNC(void*, Et00GetEtcAddr, (void* arc, const char* name), 0x004032CE);
DECOMP_FUNC(Et00Window*, Et00SetWindow, (void* bin, void* tpl, Vec* pos, Vec* ang, int flag, u8 etcNo, void* arc), 0x004088AF);
// FUNCTION: BIO4 0x00617030
// STATUS: matching
int EtXX_init(void* arc, EtcSetData* d, cModel** out, int flag, const char* bin, const char* tpl, const char* eff, int effId)
{
    void* pBin = Et00GetEtcAddr(arc,bin);
    void* pTpl = Et00GetEtcAddr(arc,tpl);
    Et00Window* em = Et00SetWindow(pBin,pTpl,&d->pos,&d->ang,flag,d->type,arc);
    if (em==0) {
        pLog->err(0,0,"Etxx_init : set failed");
        return 0;
    }
    void* pEff = Et00GetEtcAddr(arc,eff);
    EspDataLoad((u32)pEff,effId,0);
    em->setEff((u8)effId);
    em->setNoSuspend(1);
    if (em->ChkStatus() & 1) em->SetBreakModel();
    *out=em;
    return 1;
}

// FUNCTION: BIO4 0x00617100
// STATUS: matching
int Et00_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et0000.bin","et0000.tpl","et00.eff",0x58);
}

// FUNCTION: BIO4 0x00617140
// STATUS: matching
int Et07_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et0700.bin","et0700.tpl","et07.eff",0x5F);
}

// FUNCTION: BIO4 0x00617180
// STATUS: matching
int Et1d_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et1d00.bin","et1d00.tpl","et1d.eff",0x75);
}

// FUNCTION: BIO4 0x006171C0
// STATUS: matching
int Et25_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et2500.bin","et2500.tpl","et25.eff",0x7D);
}

// FUNCTION: BIO4 0x00617200
// STATUS: matching
int Et29_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et2900.bin","et2900.tpl","et29.eff",0x81);
}

// FUNCTION: BIO4 0x00617240
// STATUS: matching
int Et2c_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et2c00.bin","et2c00.tpl","et2c.eff",0x84);
}

// FUNCTION: BIO4 0x00617280
// STATUS: matching
int Et35_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et3500.bin","et3500.tpl","et35.eff",0x8D);
}

// FUNCTION: BIO4 0x006172C0
// STATUS: matching
int Et36_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et3600.bin","et3600.tpl","et36.eff",0x8E);
}

// FUNCTION: BIO4 0x00617300
// STATUS: matching
int Et44_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et4400.bin","et4400.tpl","et44.eff",0x9C);
}

// FUNCTION: BIO4 0x00617340
// STATUS: matching
int Et48_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et4800.bin","et4800.tpl","et48.eff",0xA0);
}

// FUNCTION: BIO4 0x00617380
// STATUS: matching
int Et4a_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et4a00.bin","et4a00.tpl","et4a.eff",0xA2);
}

// FUNCTION: BIO4 0x006173C0
// STATUS: matching
int Et50_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5000.bin","et5000.tpl","et50.eff",0xA8);
}

// FUNCTION: BIO4 0x00617400
// STATUS: matching
int Et51_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5100.bin","et5100.tpl","et51.eff",0xA9);
}

// FUNCTION: BIO4 0x00617440
// STATUS: matching
int Et52_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5200.bin","et5200.tpl","et52.eff",0xAA);
}

// FUNCTION: BIO4 0x00617480
// STATUS: matching
int Et53_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5300.bin","et5300.tpl","et53.eff",0xAB);
}

// FUNCTION: BIO4 0x006174C0
// STATUS: matching
int Et54_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5400.bin","et5400.tpl","et54.eff",0xAC);
}

// FUNCTION: BIO4 0x00617500
// STATUS: matching
int Et55_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5500.bin","et5500.tpl","et55.eff",0xAD);
}

// FUNCTION: BIO4 0x00617540
// STATUS: matching
int Et56_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5600.bin","et5600.tpl","et56.eff",0xAE);
}

// FUNCTION: BIO4 0x00617580
// STATUS: matching
int Et57_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5700.bin","et5700.tpl","et57.eff",0xAF);
}

// FUNCTION: BIO4 0x006175C0
// STATUS: matching
int Et58_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5800.bin","et5800.tpl","et58.eff",0xB0);
}

// FUNCTION: BIO4 0x00617600
// STATUS: matching
int Et5a_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5a00.bin","et5a00.tpl","et5a.eff",0xB2);
}

// FUNCTION: BIO4 0x00617640
// STATUS: matching
int Et5b_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5b00.bin","et5b00.tpl","et5b.eff",0xB3);
}

// FUNCTION: BIO4 0x00617680
// STATUS: matching
int Et5c_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5c00.bin","et5c00.tpl","et5c.eff",0xB4);
}

// FUNCTION: BIO4 0x006176C0
// STATUS: matching
int Et5d_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5d00.bin","et5d00.tpl","et5d.eff",0xB5);
}

// FUNCTION: BIO4 0x00617700
// STATUS: matching
int Et5e_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5e00.bin","et5e00.tpl","et5e.eff",0xB6);
}

// FUNCTION: BIO4 0x00617740
// STATUS: matching
int Et5f_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et5f00.bin","et5f00.tpl","et5f.eff",0xB7);
}

// FUNCTION: BIO4 0x00617780
// STATUS: matching
int Et60_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et6000.bin","et6000.tpl","et60.eff",0xB8);
}

// FUNCTION: BIO4 0x006177C0
// STATUS: matching
int Et64_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et6400.bin","et6400.tpl","et64.eff",0xBC);
}

// FUNCTION: BIO4 0x00617800
// STATUS: matching
int Et65_init_decomp(void* arc, EtcSetData* d, cModel** out, int flag)
{
    return EtXX_init(arc,d,out,flag,"et6500.bin","et6500.tpl","et65.eff",0xBD);
}
