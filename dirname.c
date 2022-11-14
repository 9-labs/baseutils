/* baseutils: `dirname.c` - returns the directory portion of a filename.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static const char *usage = "dirname string";

/* This utility is simple enough that it's entire functionality is inside
   this main function here. yummy */
int
main(int argc, char *argv[])
{
        char *string, *ptr;

        argc--;
        argv++;

        if (argc < 1) {
                fprintf(stderr, "dirname: expecting a string operand.\n"
                                "Usage:\n\t%s\n", usage);
                return EXIT_FAILURE;
        }

        string = argv[0];
        ptr = string;

        if (*string == '\0') {
                printf(".\n");
                return EXIT_SUCCESS;
        }

        /* if entirely / characters result is 1 / */
        for (; (*ptr != '\0') && (*ptr != '/'); ptr++);
        if (*ptr == '\0') {
                printf("/\n");
                return EXIT_SUCCESS;
        }

        /* remove trailing / characters */
        ptr = string + (strlen(string)-1);
        for (; (ptr > string) && (*ptr == '/'); ptr--);
        *(ptr + 1) = '\0';

        /* check if no / are remaining */
        ptr = string;
        for (; (*ptr != '\0') && (*ptr != '/'); ptr++);
        if (*ptr == '\0') {
                printf(".\n");
                return EXIT_SUCCESS;
        }

        /* remove trailing non-slash characters if they exist */
        ptr = string + (strlen(string)-1);
        for (; (ptr >= string) && (*ptr != '/'); ptr--);
        *(ptr + 1) = '\0';

        /* remove trailing slash characters if they exist */
        ptr = string + (strlen(string)-1);
        for (; (ptr >= string) && (*ptr == '/'); ptr--);
        *(ptr + 1) = '\0';

        /* if empty, yield a single / */
        if (*string == '\0') {
                *string = '/';
                *(string+1) = '\0';
        }

        printf("%s\n", string);
        return EXIT_SUCCESS;
}

