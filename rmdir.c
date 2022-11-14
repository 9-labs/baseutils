/* baseutils: `rmdir.c` - remove directories.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static const char *usage = "rmdir [-p] dir...";
static int status;

/* Remove all directories in the given path from right to left
   rmdir() the current `path`, place an early null terminator to trim the last
   path component and keep calling this function recursively until all 
   mentioned directories are removed. */
static void
pflag(char *path)
{
        int i;

        if (rmdir(path) == -1) {
                fprintf(stderr, "rmdir: %s: %s\n", path, strerror(errno));
                
                status = EXIT_FAILURE;
                return;
        }

        i = strlen(path);

        for (; i > 0; i--) {
                if (path[i] == '/') {
                        path[i] = '\0';
                        pflag(path);

                        break;
                }
        }
}

/* rmdir - Remove directories
   Remove directories specified by `dir` operand in order given.
        -p removes all directories in a path name for each directory operand.*/
int
main(int argc, char *argv[])
{
        char ch;
        bool p;
        int i;

        p = false;
        status = EXIT_SUCCESS;

        while ((ch = getopt(argc, argv, "p")) != -1) {
                switch (ch) {
                case 'p':
                        p = true;
                        break;
                case '?':
                default:
                        fprintf(stderr,
                                "Usage:\n\t%s\n", usage);
                        return 1;
                }
        }

        argc -= optind;
        argv += optind;

        if (argc < 1) {
                fprintf(stderr, "rmdir: requires directory name(s).\n");
                fprintf(stderr, "Usage:\n\t%s\n", usage);

                return EXIT_FAILURE;
        }       

        for (i = 0; i < argc; i++) {
                if (p) {
                        // -p: Remove all directories in the path name (L<-R)
                        pflag(argv[i]);
                        return status;
                }

                if (rmdir(argv[i]) == -1) {
                        fprintf(stderr, "rmdir: %s: %s\n", argv[i],
                                        strerror(errno));

                        status = EXIT_FAILURE;
                }
        }

        return status;
}

