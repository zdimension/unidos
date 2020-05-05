/* UniDOS emulator */
/* By Nguyen Anh Quynh, 2015 */

#include <unicorn/unicorn.h>
#include <string.h>

#include "ints/ints.h"
#include "global.h"
#include "psp.h"
#include "intvec.h"

#define DOS_ADDR 0x100


static void usage(char* prog)
{
    printf("UniDOS for DOS emulation. Based on Unicorn engine (www.unicorn-engine.org)\n");
    printf("Syntax: %s <COM>\n", prog);
}

// callback for handling interrupt
void hook_intr(uc_engine* uc, uint32_t intno, void* user_data)
{
    uint32_t r_ip;
    uint8_t r_ah;

    uc_reg_read(uc, UC_X86_REG_IP, &r_ip);
    uc_reg_read(uc, UC_X86_REG_AH, &r_ah);

    // only handle DOS interrupt

    struct interrupt* custom_handler = intvec_find(intno);

    if (custom_handler)
    {
        printf(">>> 0x%x: using custom handler at %04x:%04x for interrupt %02x\n", r_ip, custom_handler->seg,
               custom_handler->off, intno);
        r_ip = MK_FP(custom_handler->seg, custom_handler->off);
        uc_reg_write(uc, UC_ARM_REG_IP, &r_ip);
        return;
    }

    switch (intno)
    {
        default:
            printf(">>> 0x%x: interrupt: %x, function %x\n", r_ip, intno, r_ah);
            break;
        case 0x05:
            break;
        case 0x10:
            int10();
            break;
        case 0x15:
            int15();
            break;
        case 0x21:
            int21();
            break;
        case 0x20:
            int20();
            break;
    }
}

uc_engine* uc;

int main(int argc, char** argv)
{
    uc_hook trace;
    uc_err err;
    char* fname;
    FILE* f;
    uint8_t fcontent[64 * 1024];    // 64KB for .COM file
    long fsize;

    if (argc == 1)
    {
        usage(argv[0]);
        return -1;
    }

    fname = argv[1];
    f = fopen(fname, "r");
    if (f == NULL)
    {
        printf("ERROR: failed to open file '%s'\n", fname);
        return -2;
    }

    // find the file size
    fseek(f, 0, SEEK_END); // seek to end of file
    fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file

    // copy data in from 0x100
    memset(fcontent, 0, sizeof(fcontent));
    fread(fcontent + DOS_ADDR, fsize, 1, f);

    err = uc_open(UC_ARCH_X86, UC_MODE_16, &uc);
    if (err)
    {
        fprintf(stderr, "Cannot initialize unicorn\n");
        return 1;
    }

    // map 64KB in
    if (uc_mem_map(uc, 0, 64 * 1024, UC_PROT_ALL) || uc_mem_map(uc, 0xC0000, 0x10000, UC_PROT_READ))
    {
        printf("Failed to write emulation code to memory, quit!\n");
        uc_close(uc);
        return 0;
    }

    // initialize internal settings
    global_init();

    int10_init();
    int15_init();
    int21_init();

    // setup PSP
    psp_setup(0, fcontent, argc, argv);

    // write machine code to be emulated in, including the prefix PSP
    uc_mem_write(uc, 0, fcontent, DOS_ADDR + fsize);

    // handle interrupt ourself
    uc_hook_add(uc, &trace, UC_HOOK_INTR, hook_intr, NULL, 1, 0);

    err = uc_emu_start(uc, DOS_ADDR, DOS_ADDR + 0x10000, 0, 0);
    if (err)
    {
        printf("Failed on uc_emu_start() with error returned %u: %s\n",
               err, uc_strerror(err));
    }

    uc_close(uc);

    return exit_code;
}
