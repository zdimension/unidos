#ifndef UNIDOS_MEM_H
#define UNIDOS_MEM_H

uint8_t mem_alloc(uint16_t size, uint16_t *seg);

uint8_t mem_realloc(uint16_t size, uint16_t seg);

uint8_t mem_free(uint16_t seg);

#endif