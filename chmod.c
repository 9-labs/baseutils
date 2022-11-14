/* baseutils: `chmod.c` - change file modes (permissions).
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
#include <ftw.h>

#include "util.h"

static const char *usage = "chmod [-R] mode file...";
static int status;

static mode_t mode;

static void recurse(const char *path);

static int
ftwcb(const char *name, const struct stat *object, int flag)
{
        if (chmod(name, mode) != 0) {
                fprintf(stderr, "chmod: %s\n", strerror(errno));
                status = EXIT_FAILURE;
        }

        return 0;
}

/* -R flag
   If named file is a directory, set the mode of all contained file and the
   directory itself. */
static void
recurse(const char *path)
{
        int r;
        
        r = ftw(path, ftwcb, 1);

        if (r != 0) {
                fprintf(stderr, "chmod: %s\n", strerror(errno));
                status = EXIT_FAILURE;
        }
}

/* chmod - Change file modes (permissions)
   Change file `mode` of `file...`
        -R If named file is directory, set mode of the directory and all the
                files it contains. */
int
main(int argc, char *argv[])
{
        bool R;
        char *m;
        int i;
        struct stat *s;
        
        status = EXIT_SUCCESS;
        R = false;
        s = malloc(sizeof(struct stat));

        /* We only support one flag (-R), there is no reason to use getopt
           for argument parsing here, just interferes with mode parsing. */

        if (strcmp(argv[1], "-R") == 0) {
                R = true;
                argc -= 1;
                argv += 1;
        }

        argc -= 1;
        argv += 1;

        if (argc < 2) {
                fprintf(stderr, "chmod: requires a mode and file argument.\n");
                fprintf(stderr, "Usage:\n\t%s\n", usage);
                return EXIT_FAILURE;
        }

        m = argv[0];

        argc--;
        argv++;

        for (i = 0; i < argc; i++) {
                stat(argv[i], s);
                mode = s->st_mode;

                if ((modeset(m, &mode)) != 0) {
                        fprintf(stderr, "chmod: invalid file mode: %s\n", m);
                        return EXIT_FAILURE;
                }

                if (R) { // -R 
                        recurse(argv[i]);
                        return status;
                }

                if (chmod(argv[i], mode) != 0) {
                        fprintf(stderr, "chmod: %s\n", strerror(errno));
                        status = EXIT_FAILURE;
                }
        }

        free(s);

        return status;
}

