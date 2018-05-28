#include <curses.h>
#include <global.h>

#include "int10.h"

void int10_init(void)
{

}


// callback for handling interrupt
void int10() {
    uint16_t r_ip;
    uint8_t r_ah;

    uc_reg_read(uc, UC_X86_REG_AH, &r_ah);
    uc_reg_read(uc, UC_X86_REG_IP, &r_ip);

    //printf(">>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x10, r_ah);

    switch(r_ah) {
        default:
            printf("\n>>> 0x%x: interrupt: %x, AH = %02x\n", r_ip, 0x10, r_ah);
            break;
    }
}