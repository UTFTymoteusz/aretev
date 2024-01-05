#pragma once

#include "art/int.hpp"
#include "art/sys/types.hpp"

namespace art::sys {
    typedef u32 ttycolor_t;

    /**
     * @brief A class that provides low-level access to a display.
     */
    class ttydisp {
        public:
        int height;
        int width;

        ttydisp();
        virtual ~ttydisp();

        /**
         * @brief Writes a string.
         * The string will be placed at the cursor and will not wrap around if it manages
         * to exceed the display's width.
         * @param x   X coordinate.
         * @param y   Y coordinate.
         * @param str String to write.
         * @param bg  Background color of the text.
         * @param fg  Foreground color of the text.
         * @param len Length of the string.
         * @return A reference to this ttydisp object.
         */
        virtual ttydisp& write(int x, int y, const char* str, ttycolor_t bg,
                               ttycolor_t fg, usz len);

        /**
         * @brief Clears the display.
         * @param c  Optional character parameter that specifies the character to fill the
         * screen with.
         * @param bg Background color of the text.
         * @param fg Foreground color of the text.
         * @return A reference to this ttydisp object.
         */
        virtual ttydisp& clear(char c = '\0', art::sys::ttycolor_t bg = ANSI_BLACK,
                               art::sys::ttycolor_t fg = ANSI_BWHITE);

        /**
         * @brief Toggles the cursor.
         * @param enabled True if the cursor is to be displayed.
         * @return A reference to this ttydisp object.
         */
        virtual ttydisp& cursor(bool enabled);

        /**
         * @brief Sets the position of the cursor.
         * @param x X coordinate.
         * @param y Y coordinate.
         * @return A reference to this ttydisp object.
         */
        virtual ttydisp& cursor(int x, int y);

        /**
         * @brief Sets the current foreground color.
         * This will only apply to newly-written text.
         * @param color ansicolor to set.
         * @return A display-specific ttycolor_t.
         */
        virtual ttycolor_t forecolor(ansicolor color);

        /**
         * @brief Sets the current background color.
         * This will only apply to newly-written text.
         * @param color ansicolor to set.
         * @return A display-specific ttycolor_t.
         */
        virtual ttycolor_t backcolor(ansicolor color);
    };
}