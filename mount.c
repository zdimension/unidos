#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mount.h"
#include "defs.h"

struct mount_point
{
    uint8_t drive;
    char path[514];
    struct mount_point* prev;
    struct mount_point* next;
} * mount_points = NULL;

uint8_t mount_remove(uint8_t drive)
{
    for (struct mount_point* cur = mount_points; cur; cur = cur->next)
    {
        if (cur->drive == drive)
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

uint8_t mount_add(uint8_t drive, char* path)
{
    //printf("Adding mount point %c = %s\n", drive + 'A', path);

    for (struct mount_point* cur = mount_points; cur; cur = cur->next)
    {
        if (cur->drive == drive)
        {
            printf("Drive %c already mounted (to %s)\n", cur->drive + 'A', cur->path);
            return ERR_INVALID_DRIVE_SPECIFIED;
        }
    }

    struct mount_point* pnt = calloc(1, sizeof(struct mount_point));
    pnt->drive = drive;
    strcpy(pnt->path, path);

    if (!mount_points)
    {
        mount_points = pnt;
        return ERR_SUCCESS;
    }

    struct mount_point* cur = mount_points;
    while (cur->next)
        cur = cur->next;

    cur->next = pnt;
    pnt->prev = cur;

    return ERR_SUCCESS;
}

uint8_t mount_str_to_real(char* inp, char* buf)
{
    struct dospath path;
    path_parse(inp, &path);
    return mount_to_real(&path, buf);
}

uint8_t mount_to_real(struct dospath* path, char* buf)
{
    struct dospath real;
    path_absolute(*path, &real);

    struct mount_point* good = NULL;

    for (struct mount_point* cur = mount_points; cur; cur = cur->next)
    {
        if (cur->drive == real.drive)
        {
            good = cur;
            break;
        }
    }

    if (!good)
    {
        printf("Drive %c is not mounted\n", real.drive + 'A');
        return ERR_INVALID_DRIVE_SPECIFIED;
    }

    buf[0] = 0;

    strcat(buf, good->path);

    for (int i = 0; i < real.depth; i++)
    {
        strcat(buf, "/");
        strcat(buf, real.path[i]);
    }

    return ERR_SUCCESS;
}