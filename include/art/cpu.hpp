#pragma once

#include "art/int.hpp"
#include "art/mm/types.hpp"

namespace art {
    /**
     * @brief A class that abstracts away a CPU (core) in the system.
     * It also contains static methods used to interface with the local CPU.
     */
    class cpu {
        public:
        /**
         * @brief Flushes the entire paging cache.
         */
        static void mm_flush();

        /**
         * @brief Flushes a page translation.
         * @param addr Page address.
         */
        static void mm_flush(void* addr);

        /**
         * @brief Flushes multiple page translations.
         * @param addr  Page address.
         * @param count Count of contiguous page translations to flush.
         */
        static void mm_flush(void* addr, usz count);

        /**
         * @brief Flushes a page translation and broadcasts the change to every other
         * processor.
         * @param addr Page address.
         */
        // static void mm_flushbr(void* addr);

        /**
         * @brief Waits until all page translation flushes are completed on every other
         * processor.
         */
        // static void mm_flushsync();

        /**
         * @brief Halts the local CPU.
         */
        static void halt();

        /**
         * @brief Raises an exception.
         * The actual exception raised depends on the architecture.
         */
        static void dg_fault();

        /**
         * @brief Disables interrupts on the local CPU.
         */
        static void no_interrupts();

        /**
         * @brief Enables interrupts on the local CPU.
         */
        static void interrupts();

        /**
         * @brief Checks if interrupts are enabled on the local CPU.
         */
        static bool check_interrupts();

        /**
         * @brief Changes the current page table on the local CPU.
         * @param pagetbl Pointer to the desired pagetable.
         * @param kernel  Whether the pagetable is user or kernel.
         */
        static void use(mm::pagetbl* pagetbl, bool kernel = false);

        static u8   inb(u16 m_port);
        static u16  inw(u16 m_port);
        static u32  ind(u16 m_port);
        static void outb(u16 m_port, u8 m_data);
        static void outw(u16 m_port, u16 m_data);
        static void outd(u16 m_port, u32 m_data);

        private:
        mm::pagetbl* current_pagetbl;
    };
}