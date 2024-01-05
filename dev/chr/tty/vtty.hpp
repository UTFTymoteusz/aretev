#pragma once

#include "art/dev/chr/tty.hpp"
#include "art/sys.hpp"

namespace art::dev {
    struct vchar {
        char            chr;
        sys::ttycolor_t fg;
        sys::ttycolor_t bg;
    };

    class vtty : public tty {
        public:
        vtty(const char* name, sys::ttydisp* display);
        virtual ~vtty();

        private:
        int             _x, _y;
        sys::ttydisp*   _display;
        vchar*          _backbuffer;
        sys::ttycolor_t _fg;
        sys::ttycolor_t _bg;

        /**
         * @brief Scrolls the backbuffer.
         * Positive values scroll down, negative scroll up.
         * @param amount Amount to scroll by.
         */
        void scroll(int amount);

        /**
         * @brief Writes out the specified range of the backbuffer.
         * This method limits the length to the amount of chars left in the line.
         * @param x   X coordinate.
         * @param y   Y coordinate.
         * @param len Length of the string to write.
         */
        void commit(int x, int y, int len);

        /**
         * @brief Clears a line.
         * @param y Y coordinate.
         */
        void clear(int y);

        /**
         * @brief Writes a character.
         * @param x X coordinate.
         * @param y Y coordinate.
         * @param c The character to write.
         */
        void putchar(int x, int y, char c);

        usz tty_read(char* ptr, usz len);
        usz tty_write(const char* ptr, usz len);
    };
}