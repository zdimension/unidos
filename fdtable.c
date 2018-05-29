#include "fdtable.h"

int fdtable[FD_TABLE_SIZE];

// initialize FD table
void fdtable_init(void)
{
    int i;

    // enable special fd
    fdtable[0] = 0;
    fdtable[1] = 1;
    fdtable[2] = 2;

    // disable other fd
    for (i = 3; i < FD_TABLE_SIZE; i++)
        fdtable[i] = -1;
}

// clear FD
void fdtable_clear(int fd)
{
    if (fd < 3 || fd >= FD_TABLE_SIZE)
        return;

    fdtable[fd] = -1;
}

// map host FD to dos FD
int fdtable_set(int hostfd)
{
    int i;

    for (i = 0; i < FD_TABLE_SIZE; i++)
    {
        if (fdtable[i] == -1)
        {
            // found a free slot, which is also a dos FD
            fdtable[i] = hostfd;
            return i;
        }
    }

    // not found
    return -1;
}

// map DOS FD to host FD
int fdtable_get(int dosfd)
{
    if (dosfd > 0 && dosfd < FD_TABLE_SIZE)
        return fdtable[dosfd];

    return -1;
}