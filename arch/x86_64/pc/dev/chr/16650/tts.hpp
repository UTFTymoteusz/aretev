#pragma once

#include "art/dev.hpp"
#include "art/int.hpp"

using namespace art;

class tts16650 : public dev::tty {
    public:
    tts16650(const char* name, u16 base);
    ~tts16650();

    private:
    u16 _base;

    error_t tty_init();
    usz     tty_read(char* ptr, usz len);
    usz     tty_write(const char* ptr, usz len);
    error_t tty_uninit();
    void    tty_irq();
};