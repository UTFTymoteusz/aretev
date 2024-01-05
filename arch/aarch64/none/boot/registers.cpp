#include "registers.hpp"

#include "art/data.hpp"
#include "art/int.hpp"

namespace art {
    section(".boot.text") tcr_el1& tcr_el1::ips(u8 size_or_attribute) {
        u8 attribute;

        if (size_or_attribute < 32) {
            attribute = size_or_attribute;
        }
        else {
            switch (size_or_attribute) {
            case 32:
                attribute = 0b000;
                break;
            case 36:
                attribute = 0b001;
                break;
            case 40:
                attribute = 0b010;
                break;
            case 42:
                attribute = 0b011;
                break;
            case 44:
                attribute = 0b100;
                break;
            case 48:
                attribute = 0b101;
                break;
            case 52:
                attribute = 0b110;
                break;
            default:
                return *this;
            }
        }

        this->value = (this->value & ~(0b11UL << 32)) | ((attribute & 0b111UL) << 32);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::tbi1(bool ignore) {
        this->value = (this->value & ~(0b1UL << 38)) | ((u64) ignore << 38);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::tbi0(bool ignore) {
        this->value = (this->value & ~(0b1UL << 37)) | ((u64) ignore << 37);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::tg1(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 30)) | ((attribute & 0b11UL) << 30);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::sh1(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 28)) | ((attribute & 0b11UL) << 28);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::orgn1(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 26)) | ((attribute & 0b11UL) << 26);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::irgn1(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 24)) | ((attribute & 0b11UL) << 24);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::epd1(bool enabled) {
        this->value = (this->value & ~(0b1UL << 23)) | (!enabled << 23);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::t1sz(int sz) {
        this->value = (this->value & ~(0b111111UL << 16)) | ((sz & 0b111111UL) << 16);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::tg0(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 14)) | ((attribute & 0b11UL) << 14);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::sh0(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 12)) | ((attribute & 0b11UL) << 12);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::orgn0(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 10)) | ((attribute & 0b11UL) << 10);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::irgn0(u8 attribute) {
        this->value = (this->value & ~(0b11UL << 8)) | ((attribute & 0b11UL) << 8);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::epd0(bool enabled) {
        this->value = (this->value & ~(0b1UL << 7)) | (!enabled << 7);
        return *this;
    }

    section(".boot.text") tcr_el1& tcr_el1::t0sz(int sz) {
        this->value = (this->value & ~(0b111111UL)) | (sz & 0b111111UL);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::ee(bool big_endian) {
        value = (value & ~(0b1UL << 25)) | (big_endian << 25);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::e0e(bool big_endian) {
        value = (value & ~(0b1UL << 24)) | (big_endian << 24);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::wxn(bool enabled) {
        value = (value & ~(0b1UL << 19)) | (enabled << 19);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::i(bool enabled) {
        value = (value & ~(0b1UL << 12)) | (enabled << 12);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::uma(bool enabled) {
        value = (value & ~(0b1UL << 9)) | (enabled << 9);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::sa0(bool enabled) {
        value = (value & ~(0b1UL << 5)) | (enabled << 5);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::sa(bool enabled) {
        value = (value & ~(0b1UL << 3)) | (enabled << 3);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::c(bool enabled) {
        value = (value & ~(0b1UL << 2)) | (enabled << 2);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::a(bool enabled) {
        value = (value & ~(0b1UL << 1)) | (enabled << 1);
        return *this;
    }

    section(".boot.text") sctlr_el1& sctlr_el1::m(bool enabled) {
        value = (value & ~(0b1UL)) | enabled;
        return *this;
    }
}