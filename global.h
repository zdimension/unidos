#ifndef UNIDOS_GLOBAL_H
#define UNIDOS_GLOBAL_H

#include <bits/types/time_t.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unicorn/unicorn.h>
#include "dospath.h"

extern uc_engine* uc;

extern unsigned char cur_drive;
extern struct dospath cur_path[26];
extern time_t time_offset;
extern bool verify_switch;
extern bool ctrl_break_check;
extern bool ext_ctrl_break_check;

extern uint16_t current_proc_seg;

const time_t time_fix();

void ptime_fix(struct timeval* val);

void global_init();

void print_uc_err(uc_err err);

uint8_t fix_drive(uint8_t drive);

#endif