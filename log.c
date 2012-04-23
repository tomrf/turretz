#include <stdio.h>
#include <stdarg.h>
#include "common.h"


void LOG(char *fmt, ...)
{
    va_list args;
#ifdef ALLEGRO_MINGW32
    FILE *output;
    if ((output = fopen("log.txt", "a")) == NULL)
        exit(1);
#endif
    fprintf(stderr, "[LOG] ");


    va_start(args, fmt);
#ifdef ALLEGRO_MINGW32
    vfprintf(output, fmt, args);
#else
    vfprintf(stderr, fmt, args);
#endif

    va_end(args);

#ifdef ALLEGRO_MINGW32
    fprintf(output, "\n");
    fclose(output);
#endif

    fprintf(stderr, "\n");
}



void DIE(char *fmt, ...)
{
    va_list args;

    fprintf(stderr, "[DIE] ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    clean_shutdown();
    exit(1);
}

