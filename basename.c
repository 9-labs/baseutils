/* baseutils: `basename.c` - returns non-directory portion of a filename.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static const char *usage = "basename string [suffix]";

/* This utility is simple enough that it's entire functionality is inside
   this main function here. yummy */
int
main(int argc, char *argv[])
{
        char *string;
        char *ptr, *suffix;

        suffix = NULL;

        argc--;
        argv++;

        if (argc < 1) {
                fprintf(stderr, "basename: expecting a string operand.\n"
                                "Usage:\n\t%s\n", usage);
                return EXIT_FAILURE;
        }

        string = argv[0];
        ptr = string;

        if (argc > 1)
                suffix = argv[1];

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

        /* actual basename portion now */
        ptr = string + (strlen(string)-1);
        for (; (ptr > string) && (*ptr != '/'); ptr--);
        if (*ptr == '/')
                string = ptr + 1;

        if (suffix == NULL)
                goto done;

       /* suffix handling */

        if (strcmp(string, suffix) == 0)
                goto done;

        if (strlen(suffix) < strlen(string)) {
                ptr = string + (strlen(string) - strlen(suffix));
                if (strcmp(ptr, suffix) == 0)
                        *(ptr) = '\0';
        }

done:
        printf("%s\n", string);
        return EXIT_SUCCESS;
}

