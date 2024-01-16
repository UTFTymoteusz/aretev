#pragma once

#include "art/string.hpp"

namespace art::sys {
    enum ansicolor {
        ANSI_BLACK = 0,
        ANSI_RED,
        ANSI_GREEN,
        ANSI_YELLOW,
        ANSI_BLUE,
        ANSI_MAGENTA,
        ANSI_CYAN,
        ANSI_WHITE,
        ANSI_GRAY,
        ANSI_BRED,
        ANSI_BGREEN,
        ANSI_BYELLOW,
        ANSI_BBLUE,
        ANSI_BMAGENTA,
        ANSI_BCYAN,
        ANSI_BWHITE,
    };

    enum attrtype {
        AT_INVALID,
        AT_INTEGER,
        AT_BOOL,
        AT_STRING,
        AT_BYTES,
    };

    enum rsrctype {
        RS_IO,
        RS_MEMORY,
        RS_OTHER,
    };

    struct attribute {
        char     name[32];
        attrtype type;
        union {
            bool        boolval;
            const char* stringval;
            int         intval;
        };
        bool strclean = false;

        attribute() {
            this->type = AT_INVALID;
        }

        attribute(const char* name, bool val) {
            strlcpy(this->name, name, sizeof(this->name));

            this->type    = AT_BOOL;
            this->boolval = val;
        }

        attribute(const char* name, const char* val, bool clean = false) {
            strlcpy(this->name, name, sizeof(this->name));

            this->type      = AT_STRING;
            this->stringval = val;
            this->strclean  = clean;
        }

        attribute(const char* name, int val) {
            strlcpy(this->name, name, sizeof(this->name));

            this->type    = AT_INTEGER;
            this->boolval = val;
        }

        ~attribute() {
            if (this->strclean)
                delete stringval;
        }
    };

    struct resource {
        rsrctype type;
        u64      start, end;
    };

    class devdesc;
    class driver;
}