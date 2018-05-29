#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "fcb.h"
#include "defs.h"
#include "global.h"
#include "mount.h"

struct FCB_map
{
    uint8_t drive;

    char filename[8];
    char extension[3];

    int host_fd;

    struct FCB_map* prev;
    struct FCB_map* next;
} *FCB_items = NULL;

bool fcb_matches(struct FCB* fcb, struct FCB_map* map)
{
    return fcb->drive == map->drive
            && !strncmp(fcb->filename, map->filename, 8)
            && !strncmp(fcb->extension, map->extension, 3);
}

uint8_t fcb_close(struct FCB* fcb)
{
    for(struct FCB_map* cur = FCB_items; cur; cur = cur->next)
    {
        if (fcb_matches(fcb, cur))
        {
            if (cur->prev)
            {
                cur->prev->next = cur->next;
            }

            if (cur->next)
            {
                cur->next->prev = cur->prev;
            }

            close(cur->host_fd);
            free(cur);

            return ERR_SUCCESS;
        }
    }

    return ERR_FCB_UNAVAILABLE;
}

uint8_t fcb_open(struct FCB* fcb)
{
    printf("Opening FCB / %d (%c:) / %.8s.%.3s\n", fcb->drive, fix_drive(fcb->drive) + 'A', fcb->filename, fcb->extension);

    for(struct FCB_map* cur = FCB_items; cur; cur = cur->next)
    {
        if (fcb_matches(fcb, cur))
        {
            printf("File already open\n");
            return ERR_UNABLE_TO_COMPLETE_FILE_OPERATION;
        }
    }

    char fname[15];
    fcb_filename(fcb, fname);

    char fixed[512];
    mount_str_to_real(fname, fixed);

    int host_fd = open(fixed, O_CREAT | O_TRUNC | O_RDWR);
    if (host_fd < 0) {   // failed to open
        printf("Unable to open file\n");
        return ERR_UNABLE_TO_COMPLETE_FILE_OPERATION;
    }

    struct FCB_map* map = malloc(sizeof(struct FCB_map));
    memset(map, 0, sizeof(struct FCB_map));
    
    map->drive = fcb->drive;
    memcpy(map->filename, fcb->filename, 8);
    memcpy(map->extension, fcb->extension, 3);
    map->host_fd = host_fd;

    fcb->current_block = 0;
    fcb->record_size = 0x80;
    
    if (!FCB_items)
    {
        FCB_items = map;
        return ERR_SUCCESS;
    }

    struct FCB_map* cur = FCB_items;
    while (cur->next) cur = cur->next;

    cur->next = map;
    map->prev = cur;

    return ERR_SUCCESS;
}

void fcb_filename(struct FCB* fcb, char* fname)
{
    memset(fname, 0, 15);
    fname[0] = fix_drive(fcb->drive) + 'A';
    fname[1] = ':';
    fname[2] = '\\';
    strcat(fname, fcb->filename);
    fname[11] = '.';
    strcat(fname, fcb->extension);
}

int fcb_get_fd(struct FCB* fcb)
{
    for(struct FCB_map* cur = FCB_items; cur; cur = cur->next)
    {
        if (fcb_matches(fcb, cur))
        {
            return cur->host_fd;
        }
    }

    return -1;
}