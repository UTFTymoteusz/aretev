#include "art/dev/blk/blkqueue.hpp"

#include "art/util.hpp"

namespace art::dev {
    bool blkqueue::push(blkpkt* packet) {
        if (this->space())
            return true;

        this->_queue.push_back(packet);
        return false;
    }

    blkpkt* blkqueue::next() {
        blkpkt* result = this->_queue.pop_front();
        if (!result)
            return nullptr;

        this->_ongoing++;

        return result;
    }

    void blkqueue::completed(blkpkt*) {
        this->_ongoing--;
    }

    int blkqueue::space() {
        return this->max_ongoing - this->_ongoing;
    }
}