#ifndef UNIDOS_INTVEC_H
#define UNIDOS_INTVEC_H

#include <stdint.h>

struct interrupt
{
    uint8_t interrupt;
    uint16_t seg;
    uint16_t off;
    struct interrupt* prev;
    struct interrupt* next;
};

uint8_t intvec_remove(uint8_t interrupt);

uint8_t intvec_set(uint8_t interrupt, uint16_t seg, uint16_t off);

struct interrupt* intvec_find(uint8_t interrupt);

#endif