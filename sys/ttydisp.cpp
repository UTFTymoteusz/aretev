#include "art/sys/ttydisp.hpp"

namespace art::sys {
    ttydisp::ttydisp() {}

    ttydisp::~ttydisp() {}

    ttydisp& ttydisp::write(int, int, const char*, ttycolor_t, ttycolor_t, usz) {
        return *this;
    }

    ttydisp& ttydisp::clear(char, ttycolor_t, ttycolor_t) {
        return *this;
    }

    ttydisp& ttydisp::cursor(bool) {
        return *this;
    }

    ttydisp& ttydisp::cursor(int, int) {
        return *this;
    }

    ttycolor_t ttydisp::forecolor(ansicolor) {
        return 0;
    }

    ttycolor_t ttydisp::backcolor(ansicolor) {
        return 0;
    }
}