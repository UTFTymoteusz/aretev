#pragma once

#include "art/data.hpp"
#include "art/dev/ioqueue.hpp"
#include "art/dev/types.hpp"
#include "art/proc.hpp"
#include "art/util.hpp"

namespace art::dev {
    enum dflags_t {
        DV_SYNC     = 0b000000001, // The device completes packets synchronously.
        DV_MQUEUE   = 0b000000010, // The device has queues for read and write packets.
        DV_BUFFERED = 0b000000100, // The device allows for buffered IO.
        DV_CHAR     = 0b000100000, // The device is a character device.
        DV_BLOCK    = 0b001000000, // The device is a block device.
        DV_NET      = 0b010100000, // The device is a network device.
        DV_TTY      = 0b100100000, // The device is a terminal.
    };

    /**
     * @brief A class that represents a logical device.
     */
    class device {
        public:
        char name[16];
        int  flags;

        device(const char* name, int flags = 0);
        virtual ~device();

        virtual error_t init();
        virtual error_t submit(iopkt* packet);
        virtual error_t ioctl(unsigned long request, void* argp);
        virtual void    irq(int irq);

        protected:
        proc::lock _lock;
        ioqueue*   _queue;  // Generic IO queue.
        ioqueue*   _rqueue; // Read request IO queue.
        ioqueue*   _wqueue; // Write request IO queue.

        /**
         * @brief Begins servicing an IO packet.
         * @param packet IO packet to service.
         */
        virtual void begin(iopkt* packet);

        /**
         * @brief Marks an IO packet as complete.
         * @param result Amount of bytes processed.
         */
        void complete(iopkt* packet, usz result);

        /**
         * @brief Marks an IO packet as complete with an error.
         * @param error Error code.
         */
        void complete(iopkt* packet, error_t error);

        /**
         * @brief Checks if there are any new IO packets to service.
         */
        virtual void resched();
    };
}