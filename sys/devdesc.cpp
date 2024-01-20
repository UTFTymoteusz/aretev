#include "art/sys/devdesc.hpp"

#include "art/mm.hpp"

namespace art::sys {
    devdesc::devdesc(const char* name, int flags) {
        strlcpy(this->name, name, sizeof(this->name));
        this->flags = flags;
    }

    devdesc::~devdesc() {
        for (auto attrib : this->attributes)
            delete attrib;
    }

    optional<attribute> devdesc::attr(const char* name) {
        for (auto attrib : this->attributes) {
            if (strcmp(attrib->name, name) == 0)
                return *attrib;
        }

        return ENOENT;
    }

    optional<resource> devdesc::res(int id) {
        if (id >= this->resources.size())
            return ENOENT;

        return this->resources[id];
    }

    attribute* devdesc::push(attribute* attrib) {
        this->attributes.push(attrib);
        return *this->attributes.end();
    }

    void devdesc::push(resource&& attrib) {
        this->resources.push(attrib);
    }
}