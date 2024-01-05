#include "sys/acpi/acpi.hpp"

#include "art/arch/sys/acpi.hpp"
#include "art/core.hpp"
#include "art/mm.hpp"
#include "art/sys.hpp"
#include "art/util.hpp"

namespace art::sys::acpi {
    vector<table*> tables;

    bool root_init();
    bool xsdp_init(xsdp* ptr);
    bool rsdp_init(rsdp* ptr);
    void facp_init();

    bool push_table(table* table) {
        auto header    = (sdthdr*) table;
        char buffer[8] = {};

        memcpy((void*) buffer, (void*) header->signature, 4);

        if (!validate_table(table, header->length)) {
            printk("sys::acpi: Found table '%s', but the checksum was not right\n",
                   buffer);
            return false;
        }

        tables.push(table);
        printk("sys::acpi: Found table '%s', len %i\n", buffer, header->length);

        return true;
    }

    bool validate_table(const void* tbl, usz len) {
        u8* m_tbl = (u8*) tbl;
        u8  sum   = 0;

        for (usz i = 0; i < len; i++)
            sum += m_tbl[i];

        return sum == 0;
    }

    void init() {
        if (!root_init())
            return;

        facp_init();
    }

    bool root_init() {
        auto xsdp = art::arch::sys::acpi::find_xsdp();
        if (xsdp != nullptr && xsdp_init(xsdp))
            return true;

        auto rsdp = art::arch::sys::acpi::find_rsdp();
        if (rsdp != nullptr && rsdp_init(rsdp))
            return true;

        return false;
    }

    bool xsdp_init(xsdp* ptr) {
        auto xsdt = (art::sys::acpi::xsdt*) mm::kernel_pg.map(
            ptr->xsdt_address, ptr->length, nullptr, mm::PG_RW);

        if (!push_table((table*) xsdt))
            return false;

        acpirev = xsdt->header.revision;

        for (usz i = sizeof(sys::acpi::xsdt); i < xsdt->header.length; i += 8) {
            u64  addr = *((u64*) ((u8*) xsdt + i));
            auto table_hdr =
                (sdthdr*) mm::kernel_pg.map(addr, sizeof(sdthdr), nullptr, mm::PG_RW);
            auto table = (art::sys::acpi::table*) mm::kernel_pg.map(
                addr, table_hdr->length, nullptr, mm::PG_RW);

            mm::kernel_pg.unmap(table_hdr, sizeof(sdthdr));

            push_table(table);
        }

        return true;
    }

    bool rsdp_init(rsdp* ptr) {
        auto rsdt = (sys::acpi::rsdt*) mm::kernel_pg.map(ptr->rsdt_address, 4096, nullptr,
                                                         mm::PG_RW);

        if (!push_table((table*) rsdt))
            return false;

        acpirev = rsdt->header.revision;

        for (usz i = sizeof(sys::acpi::rsdt); i < rsdt->header.length; i += 4) {
            u32  addr = *((u32*) ((u8*) rsdt + i));
            auto table_hdr =
                (sdthdr*) mm::kernel_pg.map(addr, sizeof(sdthdr), nullptr, mm::PG_RW);
            auto table = (art::sys::acpi::table*) mm::kernel_pg.map(
                addr, table_hdr->length, nullptr, mm::PG_RW);

            mm::kernel_pg.unmap(table_hdr, sizeof(sdthdr));

            push_table(table);
        }

        return true;
    }

    void facp_init() {
        auto fadt = (art::sys::acpi::fadt*) get_table("FACP", 0);
        // assert(fadt);

        auto table_hdr =
            (sdthdr*) mm::kernel_pg.map(fadt->dsdt, sizeof(sdthdr), nullptr, mm::PG_RW);
        auto table = (art::sys::acpi::table*) mm::kernel_pg.map(
            fadt->dsdt, table_hdr->length, nullptr, mm::PG_RW);

        mm::kernel_pg.unmap(table_hdr, sizeof(sdthdr));

        push_table(table);

        // assert(fadt->pm_timer_block);
    }
}

namespace art::sys {
    acpi::table* get_table(const char signature[4], int index) {
        for (auto& table : acpi::tables) {
            if (memcmp((void*) signature, (void*) ((acpi::sdthdr*) table), 4) != 0) {
                continue;
            }

            if (index > 0) {
                index--;
                continue;
            }

            return (art::sys::acpi::table*) table;
        }

        return nullptr;
    }
}