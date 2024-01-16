#include "art/dev/ioqueue.hpp"

#include "art/mm.hpp"
#include "art/proc.hpp"
#include "art/util.hpp"

namespace art::dev {
    ioqueue::ioqueue() {
        proc::int_guard ig(false);
        this->max_ongoing(1);
    }

    ioqueue::~ioqueue() {}

    bool ioqueue::push(iopkt* packet) {
        proc::lock_guard lg(this->_lock);

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
        proc::lock_guard lg(this->_lock);

        iopkt* result = this->_queue.pop_front();
        if (!result)
            return nullptr;

        this->_ongoing_cur++;

        return result;
    }

    void ioqueue::completed(iopkt*) {
        proc::lock_guard lg(this->_lock);

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
        proc::lock_guard lg(this->_lock);
        return this->_ongoing_max - this->_ongoing_cur;
    }
}