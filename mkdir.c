/* baseutils: `mkdir.c` - makes directories.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static const char *usage = "mkdir [-p] [-m mode] dir...";
static int status;

/* -p: If leading directories in path are missing, create them. */
static void
pswitch(char *path)
{
        int e;
        char *copy;
        char *ptr;
        mode_t m;

        copy = strdup(path);
        ptr = copy;

        /* if based in the root directory, we're leaving the initial '/' */
        if (ptr[0] == '/')
                ptr++;

        /* Looping through and attempting to create each required directory */
        while (*ptr != '\0') {
                if (*ptr != '/') {
                        ptr++;
                        continue;
                }

                *ptr = '\0';

                /* If we've reached the end and this is the final directory,
                   abort the loop so the main function can create the directory
                 */
                if (strcmp(path, copy) == 0)
                        break;

                /* POSIX demands it done like this for some reason... */
                e = mkdir(copy, 0);
                if (e != -1)
                        chmod(copy, (S_IWUSR|S_IXUSR|(~umask(umask(0))))&0777);

                if (e == -1 && errno != EEXIST) {
                        fprintf(stderr, "mkdir: %s: %s\n", copy, 
                                        strerror(errno));
                        status = EXIT_FAILURE;
                }

                *ptr = '/';

                ptr++;
        }
}

int
main(int argc, char *argv[])
{
        char ch;
        bool p;
        char *m;
        int i;
        char *copy;
        mode_t mode;
        void *set;

        p = false;
        m = NULL;
        status = EXIT_SUCCESS;
        copy = NULL;

        while ((ch = getopt(argc, argv, "pm:")) != -1) {
                switch (ch) {
                case 'p':
                        p = true;
                        break;
                case 'm':
                        m = optarg;
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
                fprintf(stderr, "mkdir: requires directory name(s).\n");
                fprintf(stderr, "Usage:\n\t%s\n", usage);

                return EXIT_FAILURE;
        }       

        for (i = 0; i < argc; i++) {
                /*-p: if leading directories in path are missing create them */
                if (p)
                        pswitch(argv[i]);

                mode = S_IRWXU | S_IRWXG | S_IRWXO;
                
                if (m != NULL) {
                        if (modeset(m, &mode) != 0)
                                fprintf(stderr, "mkdir: invalid file mode: %s",
                                                m);
                }

                if (mkdir(argv[i], mode) == -1) {
                        fprintf(stderr, "mkdir: %s: %s\n",
                                        argv[i], strerror(errno));
                        status = EXIT_FAILURE;
                }
        }

        return status;
}

