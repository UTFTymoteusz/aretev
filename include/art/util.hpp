#pragma once

#include "art/cpu.hpp"
#include "art/util/buffer.hpp"
#include "art/util/cbuf.hpp"
#include "art/util/llist.hpp"
#include "art/util/set.hpp"
#include "art/util/vector.hpp"

struct _scopebong {
    int  i;
    bool state;
};

/**
 * @brief This mess allows for interrupts to be enabled or disabled in a block.
 * Please don't stare at it for too long, it's embarassing.
 */
#define intguard(int)                                   \
    for (_scopebong intbong = ({                        \
             bool state = art::cpu::check_interrupts(); \
             if (int)                                   \
                 art::cpu::interrupts();                \
             else                                       \
                 art::cpu::no_interrupts();             \
                                                        \
             _scopebong{                                \
                 .i     = 0,                            \
                 .state = state,                        \
             };                                         \
         });                                            \
         intbong.i < 1; ({                              \
             if (!intbong.state)                        \
                 art::cpu::no_interrupts();             \
             else                                       \
                 art::cpu::interrupts();                \
                                                        \
             intbong.i++;                               \
         }))

#define interruptible intguard(true)
#define uninterruptible intguard(false)
