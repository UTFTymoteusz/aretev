#include "art/dev/netdev.hpp"

namespace art::dev {
    netdev::netdev(const char* name, int flags) : device(name, flags | DV_NET) {}
    netdev::~netdev() {}
}