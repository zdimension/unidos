#include "global.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include "dospath.h"
#include "fdtable.h"
#include "mount.h"

uint8_t cur_drive = 2;
struct dospath cur_path[26];
time_t time_offset = 0;
bool verify_switch = false;
bool ctrl_break_check = false;
bool ext_ctrl_break_check = false;

uint16_t current_proc_seg = 0;

const time_t time_fix()
{
    return time(NULL) + time_offset;
}

void ptime_fix(struct timeval* val)
{
    gettimeofday(val, NULL);
    val->tv_sec += time_offset;
}

void global_init()
{
    fdtable_init();

    mount_add(DRIVE_A, "dos/v11object/");
    mount_add(DRIVE_C, ".");
}

void print_uc_err(uc_err err)
{
    printf("Unicorn error returned %u: %s\n",
           err, uc_strerror(err));
}