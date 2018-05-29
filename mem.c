#include <stdint.h>
#include <stddef.h>
#include <unicorn/unicorn.h>
#include "mem.h"
#include "global.h"
#include "defs.h"

struct mem_block
{
    uint16_t segment;
    uint16_t size;
    struct mem_block* prev;
    struct mem_block* next;
} *mem_blocks = NULL;

uint8_t mem_free(uint16_t seg)
{
    for(struct mem_block* cur = mem_blocks; cur; cur = cur->next)
    {
        if (cur->segment == seg)
        {
            if (cur->prev)
            {
                cur->prev->next = cur->next;
            }

            if (cur->next)
            {
                cur->next->prev = cur->prev;
            }

            uc_err err = uc_mem_unmap(uc, cur->segment << 4, cur->size);
            if (err)
            {
                printf("Failed to free memory block!\n");
                print_uc_err(err);
                return ERR_INSUFFICIENT_MEMORY;
            }

            free(cur);

            return ERR_SUCCESS;
        }
    }

    return ERR_INVALID_MEMORY_BLOCK_ADDRESS;
}

uint8_t mem_alloc(uint16_t size, uint16_t *seg)
{
    uint32_t addr = 0x10000;

    do
    {
        uint32_t lim = addr + size;

        if (addr >= 0xA0000 || lim >= 0xA0000)
        {
            printf("OUT OF MEMORY!\n");
            return ERR_INSUFFICIENT_MEMORY;
        }

        for(struct mem_block* cur = mem_blocks; cur; cur = cur->next)
        {
            uint32_t begin = cur->segment << 4;
            uint32_t end = begin + cur->size;
            if ((addr < begin && begin < lim) || (addr < end && end < lim))
            {
                addr = end;
                if (addr & 0xF)
                    addr = ALIGN(addr, 16);
                continue;
            }
        }

        break;
    } while(true);

    struct mem_block* blk = malloc(sizeof(struct mem_block));

    blk->segment = addr >> 4;
    blk->size = size;

    uc_err err = uc_mem_map(uc, blk->segment << 4, blk->size, UC_PROT_ALL);
    if (err)
    {
        printf("Failed to allocate memory block!\n");
        print_uc_err(err);
        return ERR_INSUFFICIENT_MEMORY;
    }

    *seg = blk->segment;

    if (!mem_blocks)
    {
        mem_blocks = blk;
        return ERR_SUCCESS;
    }

    struct mem_block* cur = mem_blocks;
    while (cur->next) cur = cur->next;

    cur->next = blk;
    blk->prev = cur;

    return ERR_SUCCESS;
}

uint8_t mem_realloc(uint16_t size, uint16_t seg)
{
    struct mem_block* cur = NULL;

    for(cur = mem_blocks; cur; cur = cur->next)
    {
        if (cur->segment == seg)
            break;
    }

    if (!cur)
        return ERR_INVALID_MEMORY_BLOCK_ADDRESS;

    uint16_t old_size = cur->size;
    void* buf = malloc(old_size);
    uc_mem_read(uc, cur->segment << 4, buf, old_size);

    if (mem_free(seg))
    {
        return ERR_INSUFFICIENT_MEMORY;
    }

    uint16_t new_seg;

    if (mem_alloc(size, &seg))
    {
        return ERR_INSUFFICIENT_MEMORY;
    }

    uc_mem_write(uc, seg << 4, buf, MIN(size, old_size));

    return ERR_SUCCESS;
}