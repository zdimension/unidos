#include <unicorn/unicorn.h>
#include "defs.h"
#include "global.h"
#include "psp.h"

void psp_setup(int16_t seg, uint8_t *fcontent, int argc, char **argv)
{
    uint32_t abs = MK_FP(seg, 0);
    int i, j;
    uint8_t c = 0;
    struct PSP *PSP = (struct PSP *) (fcontent + abs);

    // CPMExit: INT 20h
    PSP->CPMExit[0] = 0xcd;
    PSP->CPMExit[1] = 0x20;

    // DOS Far Call: INT 21h + RETF
    PSP->DOSFarCall[0] = 0xcd;
    PSP->DOSFarCall[1] = 0x21;
    PSP->DOSFarCall[2] = 0xcb;

    // first FSB = empty file name
    PSP->FCB1[0] = 0x01;
    PSP->FCB1[1] = 0x20;

    for (i = 2; i < argc && c < 0x7E; i++)
    {
        j = 0;
        PSP->CommandLine[c++] = ' ';
        while (argv[i][j] && c < 0x7E)
            PSP->CommandLine[c++] = argv[i][j++];
    }

    PSP->CommandLine[c] = 0x0D;
    PSP->CommandLineLength = c;
    /*
    printf("==== offset of c = %x\n", offsetof(struct PSP, CommandLineLength));
    printf("==== offset of c1 = %x\n", offsetof(struct PSP, CPMCall5Compat));
    printf("==== offset of c1 = %u\n", offsetof(struct PSP, OldTSRAddress));
    printf("==== cmd[1] = %c\n", PSP->CommandLine[1]);
    printf("==== cmd[2] = %c\n", PSP->CommandLine[2]);
    */
}

uint8_t psp_create(int16_t seg)
{
    uc_err err = uc_mem_map(uc, MK_FP(seg, 0), 4096, UC_PROT_ALL);
    if (err)
    {
        printf("Failed to allocate memory for new PSP!\n");
        print_uc_err(err);
        return ERR_INSUFFICIENT_MEMORY;
    }

    return ERR_SUCCESS;
}

uint8_t psp_copy(uint16_t source, uint16_t target)
{
    void *buf = malloc(256);

    uc_mem_read(uc, MK_FP(source, 0), buf, 256);

    uc_mem_write(uc, MK_FP(target, 0) << 4, buf, 256);

    return ERR_SUCCESS;
}