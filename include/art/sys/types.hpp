#pragma once

#include "art/mm/shptr.hpp"
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
        AT_INVALID,   // Invalid attribute type
        AT_INTEGER,   // Integer attirbute type
        AT_BOOL,      // Bool attribute type
        AT_STRING,    // String attribute type
        AT_BYTES,     // Generic data attribute type
        AT_REFERENCE, // Devdesc reference attribute type
    };

    enum rsrctype {
        RS_IO,
        RS_MEMORY,
        RS_OTHER,
    };

    class devdesc;
    class driver;

    struct attribute {
        char     name[32];
        attrtype type;
        union {
            bool        boolval;
            const char* stringval;
            int         intval;
        };
        shptr<devdesc> reference;
        bool           strclean = false;

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

        attribute(const char* name, shptr<devdesc> devd) : reference(devd) {
            strlcpy(this->name, name, sizeof(this->name));

            this->type = AT_REFERENCE;
        }

        ~attribute() {
            if (this->strclean)
                delete stringval;
        }
    };

    struct resource {
        rsrctype type;
        u64      start, end;

        resource() {}

        resource(rsrctype type, u64 start, u64 end) {
            this->type  = type;
            this->start = start;
            this->end   = end;
        }
    };
}