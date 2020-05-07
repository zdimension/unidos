#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "intvec.h"
#include "defs.h"

struct interrupt* interrupts = NULL;

uint8_t intvec_remove(uint8_t interrupt)
{
    for (struct interrupt* cur = interrupts; cur; cur = cur->next)
    {
        if (cur->interrupt == interrupt)
        {
            if (cur->prev)
            {
                cur->prev->next = cur->next;
            }

            if (cur->next)
            {
                cur->next->prev = cur->prev;
            }

            free(cur);

            return ERR_SUCCESS;
        }
    }

    return ERR_INVALID_DRIVE_SPECIFIED;
}

uint8_t intvec_set(uint8_t interrupt, uint16_t seg, uint16_t off)
{
    dbgprintf("Setting interrupt vector %02x = %04x:%04x\n", interrupt, seg, off);

    struct interrupt* intp = NULL;
    bool existing = false;

    for (struct interrupt* cur = interrupts; cur; cur = cur->next)
    {
        if (cur->interrupt == interrupt)
        {
            intp = cur;
            existing = true;
            break;
        }
    }

    if (!intp)
    {
        intp = calloc(1, sizeof(struct interrupt));
        intp->interrupt = interrupt;
    }

    intp->seg = seg;
    intp->off = off;

    if (!interrupts)
    {
        interrupts = intp;
        return ERR_SUCCESS;
    }

    if (!existing)
    {
        struct interrupt* cur = interrupts;
        while (cur->next)
            cur = cur->next;

        cur->next = intp;
        intp->prev = cur;
    }

    return ERR_SUCCESS;
}

struct interrupt* intvec_find(uint8_t interrupt)
{
    for (struct interrupt* cur = interrupts; cur; cur = cur->next)
    {
        if (cur->interrupt == interrupt)
        {
            return cur;
        }
    }

    return 0;
}