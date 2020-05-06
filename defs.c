#include <stdio.h>

#include "defs.h"


int dbgprintf_placeholder(const char *__restrict __format, ...)
{
    ;
}

int (*dbgprintf)(const char *__restrict __format, ...) = dbgprintf_placeholder;