#ifndef UNIDOS_FDTABLE_H
#define UNIDOS_FDTABLE_H

#define FD_TABLE_SIZE 256

void fdtable_init(void);

// clear FD
void fdtable_clear(int fd);

// map host FD to dos FD
int fdtable_set(int hostfd);

// map DOS FD to host FD
int fdtable_get(int dosfd);

#endif