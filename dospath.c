#include <stdlib.h>
#include <string.h>

#include "dospath.h"
#include "global.h"

void path_to_string(struct dospath path, char* buf)
{
    if (path.drive != -1)
    {
        buf[0] = path.drive + 'A';
        strcat(buf, ":");
    }

    if (path.depth == 0)
    {
        strcat(buf, "\\");
    }
    else
    {
        for (int i = 0; i < path.depth; i++)
        {
            strcat(buf, "\\");
            strcat(buf, path.path[i]);
        }
    }
}

void path_parse(char* str, struct dospath* path)
{
    if (str[1] == ':')
    {
        path->drive = str[0] - 'A';
    }
    else
    {
        path->drive = -1;
    }

    char* p = strtok(str + 2, "\\");
    path->depth = 0;
    path->path = NULL;

    while (p)
    {
        path->path = realloc(path->path, sizeof(char*) * ++path->depth);

        path->path[path->depth - 1] = p;

        p = strtok(NULL, "\\");
    }
}

void path_combine(struct dospath first, struct dospath second, struct dospath* output)
{
    if (second.drive != -1)
    {
        path_copy(second, output);
        return;
    }

    output->drive = first.drive;
    output->depth = first.depth + second.depth;
    output->path = (char**)malloc(output->depth * sizeof(char*));

    for(int i = 0; i < first.depth; i++)
    {
        size_t len = strlen(first.path[i]) + 1;
        output->path[i] = (char*)malloc(len);
        memcpy(output->path[i], first.path[i], len);
    }

    for(int i = 0; i < second.depth; i++)
    {
        int off = i + first.depth;
        size_t len = strlen(second.path[i]) + 1;
        output->path[off] = (char*)malloc(len);
        memcpy(output->path[off], second.path[i], len);
    }
}

void path_absolute(struct dospath path, struct dospath* output)
{
    path_combine(cur_path[cur_drive], path, output);
}

void path_copy(struct dospath src, struct dospath* dest)
{
    dest->drive = src.drive;
    dest->depth = src.depth;
    dest->path = (char**)malloc(dest->depth * sizeof(char*));

    for(int i = 0; i < dest->depth; i++)
    {
        size_t len = strlen(src.path[i]) + 1;
        dest->path[i] = (char*)malloc(len);
        memcpy(dest->path[i], src.path[i], len);
    }
}