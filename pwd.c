/* baseutils: `pwd.c` - *p*rints name of *w*orking *d*irectory.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static const char *usage = "pwd [-L|-P]";

int
main(int argc, char *argv[])
{
        char ch;
        bool L;
        char *path;

        L = true;

        while ((ch = getopt(argc, argv, ":LP")) != -1) {
                switch (ch) {
                case 'L':
                        L = true;
                        break;
                case 'P':
                        L = false;
                        break;
                case '?':
                default:
                        fprintf(stderr, "Usage:\n\t%s\n", usage);
                        return 1;
                }
        }

        argc -= optind;

        if (argc > 0) {
                fprintf(stderr, "Not expecting any arguments.\n");
                return 1;
        }

        if (L) {
                path = getenv("PWD");
                if (path == NULL)
                        L = false;
        }

        if (!L)
                path = getcwd(NULL, 0);

        if (path == NULL) {
                fprintf(stderr, "pwd: %s\n", strerror(errno));
                return 1;
        }

        puts(path);

        return EXIT_SUCCESS;
}

