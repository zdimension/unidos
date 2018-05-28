#include <global.h>
#include "uc.h"

// set C flag in EFLAGS register
void set_flag_C(int flag)
{
    uint32_t r_eflags;

    uc_reg_read(uc, UC_X86_REG_EFLAGS, &r_eflags);

    if (flag)
        r_eflags &= 0xfffffffe; // eflags_C = 0
    else
        r_eflags |= 1; // eflags_C = 1

    uc_reg_write(uc, UC_X86_REG_EFLAGS, &r_eflags);
}