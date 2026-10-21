#ifndef SND_H
#define SND_H

#include "bind.h"
#include "types.h"

// Sound calls (GameCube snd.h prototypes; PC addresses from symbols/).
class cModel;
DECOMP_FUNC(u32, SndCall, (u16 blk, u16 no, Vec* pos, int id, int vol, cModel* obj), 0x0040ADC1);
DECOMP_FUNC(int, SndStop, (u32 id, int time), 0x0096FA80);

#endif
