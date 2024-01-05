#pragma once

#include "art/dev/blk/blkqueue.hpp"
#include "art/dev/device.hpp"

namespace art::dev {
    class blkdev : public device {
        public:
        blkssz_t sector_size = 512;

        blkdev(const char* name, int flags);
        virtual ~blkdev();

        error_t submit(iopkt* packet);

        protected:
        virtual error_t submit(blkpkt* packet);
        virtual void    begin(blkpkt* packet) = 0;
        void            complete(blkpkt* packet);
        void            resched();
        bool            convert(iopkt* packet);

        private:
        blkqueue _blkqueue; // Block queue.
    };
}
