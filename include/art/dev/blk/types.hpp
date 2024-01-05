#pragma once

#include "art/dev/types.hpp"

namespace art::dev {
    constexpr int BLK_MAX_RETRIES = 7;

    enum blkcmd {
        BL_READ,
        BL_WRITE,
    };

    struct blkpkt {
        iovec*        iov;
        int           iovcnt;
        blkcnt_t      cnt;
        blkcmd        cmd;
        error_t       error;
        blkpkt*       next;
        int           retries;
        llist<iopkt*> iopkts;
    };
}