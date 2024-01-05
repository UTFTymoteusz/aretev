#pragma once

#define NCCS 32

typedef unsigned int tcflag_t;
typedef char         cc_t;

enum iflag {};

enum oflag {
    OPOST  = 1 << 0, // Post-process output.
    ONLCR  = 1 << 1, // Map NL to CR-NL on output.
    OCRNL  = 1 << 2, // Map NL to CR on output.
    ONOCR  = 1 << 3, // No CR output at column 0.
    ONLRET = 1 << 4, // NL performs CR function.
};

enum lflag {
    ISIG   = 1 << 0,
    ICANON = 1 << 1,
    ECHO   = 1 << 2,
    ECHONL = 1 << 3,
};

enum speed_t {
    B0,      // Hang up
    B50,     // 50 baud
    B75,     // 75 baud
    B110,    // 110 baud
    B134,    // 134.5 baud
    B150,    // 150 baud
    B200,    // 200 baud
    B300,    // 300 baud
    B600,    // 600 baud
    B1200,   // 1200 baud
    B1800,   // 1800 baud
    B2400,   // 2400 baud
    B4800,   // 4800 baud
    B9600,   // 9600 baud
    B19200,  // 19200 baud
    B38400,  // 38400 baud
    B57600,  // 57600 baud
    B115200, // 115200 baud
};

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t     c_cc[NCCS];
};