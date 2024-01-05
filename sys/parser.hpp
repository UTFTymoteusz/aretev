#pragma once

#include "art/sys.hpp"

namespace art::sys {
    /**
     * @brief A struct used to return parsed ansi info.
     */
    struct ansires {
        int       cursorx, cursory;
        ansicolor fg, bg;
        int       scroll;
        bool      clear;
    };

    class parser {
        public:
        ansires result;

        parser();
        ~parser();

        private:
    };
}
