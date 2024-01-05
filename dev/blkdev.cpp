#include "art/dev/blkdev.hpp"

namespace art::dev {
    blkdev::blkdev(const char* name, int flags) : device(name, flags | DV_BLOCK) {
        this->_queue->max_ongoing(-1);
    }

    blkdev::~blkdev() {}

    error_t blkdev::submit(iopkt* packet) {
        locking(this->_lock) {
            if (!this->_blkqueue.space())
                this->_queue->push(packet);
            else
                this->convert(packet);
        }

        return ENONE;
    }

    void blkdev::resched() {
        blkpkt* next;

        while (this->_blkqueue.space() && (next = this->_blkqueue.next()))
            this->begin(next);
    }

    error_t blkdev::submit(blkpkt* packet) {
        return EIO;
    }

    bool blkdev::convert(iopkt* packet) {
        return false;
    }
}