#pragma once

#include "art/dev/blk/types.hpp"

namespace art::dev {
    class blkqueue {
        public:
        blkssz_t sector_size   = 512;
        blkssz_t sector_mask   = 511;
        usz      sector_maxseg = 16;
        int      max_ongoing   = 16;

        bool    push(blkpkt* packet);
        blkpkt* next();
        void    completed(blkpkt* packet);
        int     space();

        private:
        llist<blkpkt*> _queue;
        usz            _ongoing;
    };
}