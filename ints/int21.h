#ifndef UNIDOS_INT21_H
#define UNIDOS_INT21_H

#include <unicorn/unicorn.h>
#include "defs.h"

#pragma pack(push, 1)
struct i65_extended_country_info
{
    uint8_t id;
    uint16_t size;
    uint16_t country_id;
    uint16_t code_page;
    struct dos_country_data_new country_data;
};

struct i65_char_table
{
    uint8_t id;
    uint32_t uppercase_ptr;
};

struct i65_file_char_table
{
    uint8_t id;
    uint32_t file_char_ptr;
};

struct i65_file_terminator_table
{
    uint8_t id;
    uint32_t file_terminator_ptr;
};

struct i65_file_terminator
{
    uint16_t size;
    uint8_t unknown[7];
    uint8_t data_len;
    uint8_t data[0];
};

struct i65_collate_table
{
    uint8_t id;
    uint32_t collate_ptr;
};

struct i65_dbcs_table
{
    uint8_t id;
    uint32_t dbcs_ptr;
};
#pragma pack(pop)

void int21_init();

void int21();

#endif
