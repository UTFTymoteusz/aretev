#include "art/dev/ioqueue.hpp"

#include "art/mm.hpp"
#include "art/util.hpp"

namespace art::dev {
    ioqueue::ioqueue() {
        uninterruptible this->max_ongoing(1);
    }

    ioqueue::~ioqueue() {}

    bool ioqueue::push(iopkt* packet) {
        guard(this->_lock, proc::lock);

        if (this->space())
            return true;

        if (packet->noblock) {
            packet->complete(EAGAIN);
            return false;
        }

        this->_queue.push_back(packet);
        return false;
    }

    iopkt* ioqueue::next() {
        guard(this->_lock, proc::lock);

        iopkt* result = this->_queue.pop_front();
        if (!result)
            return nullptr;

        this->_ongoing_cur++;

        return result;
    }

    void ioqueue::completed(iopkt*) {
        guard(this->_lock, proc::lock);

        this->_ongoing_cur--;
    }

    void ioqueue::max_ongoing(int max) {
        if (max == -1) {
            this->_ongoing_max = 65536;
            return;
        }

        this->_ongoing_max = max;
    }

    int ioqueue::space() {
        guard(this->_lock, proc::lock);
        return this->_ongoing_max - this->_ongoing_cur;
    }
}