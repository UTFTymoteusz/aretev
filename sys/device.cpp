#include "art/sys/device.hpp"

#include "art/mm.hpp"

namespace art::sys {
    device::device(const char* name, int flags) {
        strlcpy(this->name, name, sizeof(this->name));
        this->flags = flags;
    }

    device::~device() {
        for (int i = 0; i < this->_attrcnt; i++) {
            if (this->attributes[i])
                delete this->attributes[i];
        }
    }

    optional<attribute> device::attr(const char* name) {
        for (int i = 0; i < this->_attrcnt; i++) {
            if (!this->attributes[i])
                continue;

            if (strcmp(this->attributes[i]->name, name) == 0)
                return *this->attributes[i];
        }

        return ENOENT;
    }

    attribute* device::push(attribute* attrib) {
        this->attributes = (attribute**) mm::realloc(
            this->attributes, (this->_attrcnt + 1) * sizeof(attribute*));
        this->attributes[this->_attrcnt] = attrib;
        this->_attrcnt++;

        return this->attributes[this->_attrcnt - 1];
    }
}