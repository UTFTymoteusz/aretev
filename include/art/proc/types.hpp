#pragma once

namespace art::arch::proc {
    struct fpu_context;
    struct context;
}

namespace art::proc {
    typedef int pid_t;
    typedef int tid_t;

    enum state {
        ST_FRESH,
        ST_RUNNING,
        ST_BLOCK,
        ST_BLOCKWAIT,
        ST_SLEEP,
        ST_DEAD,
    };

    class process;
    class thread;
}