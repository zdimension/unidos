#ifndef UNIDOS_FCB_H
#define UNIDOS_FCB_H

#pragma pack(push, 1)

#include <stdint.h>

struct FCB
{
    uint8_t drive;

    char filename[8];
    char extension[3];

    uint16_t current_block;

    uint16_t record_size;
    uint32_t file_size;

    uint16_t updated_day : 5;
    uint16_t updated_month : 4;
    uint16_t updated_year : 7;

    uint16_t time_sec : 5;
    uint16_t time_min : 6;
    uint16_t time_hour : 5;

    uint8_t specific[8];

    uint8_t current_record;

    uint32_t relative_record_number;
};
#pragma pack(pop)

uint8_t fcb_close(struct FCB* fcb);
uint8_t fcb_open(struct FCB* fcb);
void fcb_filename(struct FCB* fcb, char* buf);
int fcb_get_fd(struct FCB* fcb);

#endif