#ifndef UNIDOS_MOUNT_H
#define UNIDOS_MOUNT_H

#include "dospath.h"

enum DRIVES
{
    DRIVE_A,
    DRIVE_B,
    DRIVE_C,
    DRIVE_D,
    DRIVE_E,
    DRIVE_F,
    DRIVE_G,
    DRIVE_H,
    DRIVE_I,
    DRIVE_J,
    DRIVE_K,
    DRIVE_L,
    DRIVE_M,
    DRIVE_N,
    DRIVE_O,
    DRIVE_P,
    DRIVE_Q,
    DRIVE_R,
    DRIVE_S,
    DRIVE_T,
    DRIVE_U,
    DRIVE_V,
    DRIVE_W,
    DRIVE_X,
    DRIVE_Y,
    DRIVE_Z
};

uint8_t mount_remove(uint8_t drive);

uint8_t mount_add(uint8_t drive, char *path);

uint8_t mount_str_to_real(char *inp, char *buf);

uint8_t mount_to_real(struct dospath *path, char *buf);

#endif