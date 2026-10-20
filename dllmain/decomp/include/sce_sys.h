#ifndef SCE_SYS_H
#define SCE_SYS_H

#include "bind.h"
#include "types.h"

class cSceSys {
public:
    u8 pad_00[0x6F];
    u8 m_init_loop_flag;   // 0x6F  set while readEmData waits inside a scenario task

    DECOMP_METHOD0(u8, cSceSys, checkCTaskRange, 0x004066A4);
};

DECOMP_GLOBAL(cSceSys, SceSys, 0x00C67BA0);

DECOMP_FUNC(void, SceSleep, (int frames), 0x004040E3);
DECOMP_FUNC(void, TaskSleep, (int frames), 0x0040838C);

#endif
