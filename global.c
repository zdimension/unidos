#include "global.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include "util/dospath.h"

uint8_t cur_drive = 2;
struct dospath cur_path[26];
time_t time_offset = 0;
bool verify_switch = false;
bool ctrl_break_check = false;
bool ext_ctrl_break_check = false;

const time_t time_fix()
{
    return time(NULL) + time_offset;
}

void ptime_fix(struct timeval* val)
{
    gettimeofday(val, NULL);
    val->tv_sec += time_offset;
}