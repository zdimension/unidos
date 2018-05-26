#ifndef DOSCORN_INT10_H
#define DOSCORN_INT10_H

#include <unicorn/unicorn.h>

void int10_init(void);
void int10(uc_engine *uc);

#endif
