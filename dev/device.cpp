#include "art/dev/device.hpp"

#include "art/errno.hpp"
#include "art/string.hpp"

namespace art::dev {
    device::device(const char* name, int flags) {
        strlcpy(this->name, name, sizeof(this->name));
        this->flags = flags;

        if (!(this->flags & DV_SYNC)) {
            this->_queue = new ioqueue();

            if (this->flags & DV_MQUEUE) {
                this->_rqueue = new ioqueue();
                this->_wqueue = new ioqueue();
            }
        }
    }

    device::~device() {
        if (this->_queue)
            delete this->_queue;

        if (this->_rqueue)
            delete this->_rqueue;

        if (this->_wqueue)
            delete this->_wqueue;
    }

    error_t device::init() {
        return ENONE;
    }

    error_t device::submit(iopkt* packet) {
        uninterruptible locking(this->_lock) {
            if (this->flags & DV_SYNC) {
                this->begin(packet);
            }
            else if (this->flags & DV_MQUEUE) {
                switch (packet->cmd) {
                case IO_READ:
                    if (this->_rqueue->push(packet))
                        this->begin(packet);
                    break;
                case IO_WRITE:
                    if (this->_wqueue->push(packet))
                        this->begin(packet);
                    break;
                default:
                    if (this->_queue->push(packet))
                        this->begin(packet);
                    break;
                }
            }
            else {
                if (this->_queue->push(packet))
                    this->begin(packet);
            }
        }

        return ENONE;
    }

    error_t device::ioctl(unsigned long, void*) {
        return EINVAL;
    }

    void device::irq(int) {
        this->_lock.acquire();
        this->_lock.release();
    }

    void device::begin(iopkt* packet) {
        packet->complete(ENOSYS);
    }

    void device::complete(iopkt* packet, usz result) {
        packet->complete(result);

        this->_queue->completed(packet);
        this->resched();
    }

    void device::complete(iopkt* packet, error_t error) {
        packet->complete(error);

        this->_queue->completed(packet);
        this->resched();
    }

    void device::resched() {
        iopkt* next;

        while (this->_queue->space() && (next = this->_queue->next()))
            this->begin(next);

        if (this->flags & DV_MQUEUE) {
            while (this->_rqueue->space() && (next = this->_rqueue->next()))
                this->begin(next);

            while (this->_wqueue->space() && (next = this->_wqueue->next()))
                this->begin(next);
        }
    }
}
