#pragma once

#include "art/data.hpp"
#include "art/int.hpp"

namespace art::sys::acpi {
    constexpr auto MT_PROCESSOR_ENABLED = 1 << 0;
    constexpr auto MT_PROCESSOR_ONLINE  = 1 << 1;

    struct packed sdthdr {
        char signature[4];

        u32 length;
        u8  revision;
        u8  checksum;

        char oem_id[6];
        char oembl_id[8];

        u32 oem_revision;
        u32 creator_id;
        u32 creator_revision;
    };

    struct packed rsdp {
        char signature[8];
        u8   checksum;
        char oem_id[6];
        u8   revision;
        u32  rsdt_address;
    };

    struct packed rsdt {
        sdthdr header;
        u32    table_pointers[];
    };

    struct packed xsdp {
        rsdp basersdp;
        u32  length;
        u64  xsdt_address;
        u8   extended_checksum;
        u8   reserved[3];
    };

    struct packed xsdt {
        sdthdr header;
        u64    table_pointers[];
    };

    struct packed fadt {
        sdthdr header;
        u32    firmware_ctrl;
        u32    dsdt;

        u8 reserved0;

        u8  preffer_power_management_profile;
        u16 sci_interrupt;
        u32 smi_command_port;

        u8 acpi_enable;
        u8 acpi_disable;

        u8 s4bios_req;
        u8 pstate_control;

        u32 pm1a_event_block;
        u32 pm1b_event_block;
        u32 pm1a_controlblock;
        u32 pm1b_controlblock;
        u32 pm2_controlblock;
        u32 pmimer_block;

        u32 gpe0_block;
        u32 gpe1_block;

        u8 pm1_event_length;
        u8 pm1_control_length;
        u8 pm2_control_length;
        u8 pmimer_length;

        u8 gpe0_length;
        u8 gpe1_length;
        u8 gpe1_base;

        u8 c_state_control;

        u16 worst_c2_latency;
        u16 worst_c3_latency;

        u16 flush_size;
        u16 flush_stride;

        u8 duty_offset;
        u8 duty_width;

        u8 day_alarm;
        u8 month_alarm;
        u8 century;

        u16 boot_arch_flags;

        u8  reserved1;
        u32 fixed_flags;
    };

    struct packed madt {
        enum entrytype {
            MT_LAPIC      = 0,
            MT_IOAPIC     = 1,
            MT_IRQ_SOURCE = 2,
            MT_NMI        = 4,
            MT_LAPIC_ADDR = 5,
        };

        struct packed entry {
            u8 type;
            u8 len;
        };

        struct packed lapic {
            entry base;
            u8    id;
            u8    apic_id;
            u32   flags;

            bool can_start() {
                return flags & MT_PROCESSOR_ENABLED || flags & MT_PROCESSOR_ONLINE;
            }
        };

        struct packed ioapic {
            entry base;
            u8    id;
            u8    reserved;
            u32   addr;
            u32   global_interrupt_base;
        };

        struct packed int_override {
            entry base;
            u8    bus_source;
            u8    irq_source;
            u32   global_interrupt;
            u16   flags;
        };

        struct packed nmi {
            entry base;
            u8    id;
            u16   flags;
            u8    num;
        };

        struct packed addr_override {
            entry base;
            u16   reserved;
            u64   addr;
        };

        sdthdr header;
        u32    apic_addr;
        u32    flags;
        u8     data[];

        template <typename T>
        T find_entry(int type, int index) {
            return (T) find_entry(type, index);
        }

        void* find_entry(int type, int index);
    };
}