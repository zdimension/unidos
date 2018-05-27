#include <curses.h>
#include <unicorn/unicorn.h>

#include "int15.h"
#include "../util/uc.h"

enum SDT_MODEL
{
    MODEL_386 = 0xF8,
    MODEL_PC_80C88 = 0xF9,
    MODEL_8086 = 0xFA,
    MODEL_PC_XT = 0xFB,
    MODEL_286 = 0xFC,
    MODEL_PC_JR = 0xFD,
    MODEL_PC_XT_2 = 0xFE,
    MODEL_PC_8088 = 0xFF
};

struct SDT
{
    uint16_t num_bytes;
    uint8_t model;
    uint8_t submodel;
    uint8_t bios_rev;

    union
    {
        struct
        {
            // feature 1
            uint8_t dual_bus : 1;
            uint8_t micro_channel : 1;
            uint8_t extended_bios_area : 1;
            uint8_t wait_external_event : 1;
            uint8_t int_15_4f_called : 1;
            uint8_t rtc_installed : 1;
            uint8_t second_8259 : 1;
            uint8_t dma_3_used : 1;

            // feature 2
            uint8_t reserved_2_0 : 1;
            uint8_t data_streaming_supported : 1;
            uint8_t non_8042_kbd : 1;
            uint8_t int_15_c8_supported : 1;
            uint8_t int_15_c7_supported : 1;
            uint8_t int_15_c6_supported : 1;
            uint8_t int_15_09_supported : 1;
            uint8_t dma_32_supported : 1;

            // feature 3
            uint8_t iml_scsi_supported : 1;
            uint8_t iml_system : 1;
            uint8_t info_panel_installed : 1;
            uint8_t scsi_subsystem_supported : 1;
            uint8_t post_rom_to_ram_supported : 1;
            uint8_t reserved_3_5 : 1;
            uint8_t reserved_3_6 : 1;
            uint8_t reserved_3_7 : 1;

            // feature 4
            uint8_t postext_supported : 1;
            uint8_t memory_split_supported : 1;
            uint8_t reserved_4_2 : 1;
            uint8_t abios_conf : 3;
            uint8_t has_eeprom : 1;
            uint8_t unknown_4_7 : 1;

            // feature 5
            uint8_t flash_bios : 1;
            uint8_t has_enhanced_mouse : 1;
            uint8_t reserved_5_2 : 1;
            uint8_t reserved_5_3 : 1;
            uint8_t reserved_5_4 : 1;
            uint8_t reserved_5_5 : 1;
            uint8_t reserved_5_6 : 1;
            uint8_t reserved_5_7 : 1;
        };

        uint8_t bytes[5];
    };
};

void int15_init(uc_engine *uc)
{
    struct SDT sdt = {0};
    sdt.num_bytes = sizeof(sdt) - sizeof(uint16_t);
    sdt.model = MODEL_386;
    sdt.bios_rev = 42;

    sdt.rtc_installed = TRUE;

    uc_mem_write(uc, 0xC0000, &sdt, sizeof(sdt));
}


// callback for handling interrupt
void int15(uc_engine *uc) {
    uint16_t r_ip;
    uint8_t r_ah;

    uc_reg_read(uc, UC_X86_REG_AH, &r_ah);
    uc_reg_read(uc, UC_X86_REG_IP, &r_ip);

    //printf(">>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x15, r_ah);

    switch(r_ah) {
        default:
            printf("\n>>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x15, r_ah);
            break;

        case 0xc0: // system parameters
        {
            uint8_t r_ah = 0x86;

            uc_reg_write(uc, UC_X86_REG_AH, &r_ah);

            uint16_t r_es = 0xC000;
            uint16_t r_bx = 0x0000;

            uc_reg_write(uc, UC_X86_REG_ES, &r_es);
            uc_reg_write(uc, UC_X86_REG_BX, &r_bx);

            set_flag_C(uc, 0);
        }
    }
}