#ifndef DOSCORN_INT15_H
#define DOSCORN_INT15_H

#include <unicorn/unicorn.h>

void int15_init(uc_engine *uc);
void int15(uc_engine *uc);

#endif
