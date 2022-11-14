/* baseutils: `cat.c` - con`cat`enate and print files.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static const char *usage = "cat [-u] [file...]";

static FILE *
openfile(char *filename)
{
        FILE *f;

        f = fopen(filename, "r");
        
        if (f == NULL)
                fprintf(stderr, "cat: %s: %s\n", filename, strerror(errno));

        return f;
}

static void
output(FILE *f)
{
        char ch;

        while ((ch = fgetc(f)) != EOF) {
                putchar(ch);
        }

        if (f != stdin)
                fclose(f);
}

int
main(int argc, char *argv[])
{
        char ch;
        int i;
        FILE *f;
        int status;

        status = EXIT_SUCCESS;

        while ((ch = getopt(argc, argv, "u")) != -1) {
                switch (ch) {
                case 'u':
                        setvbuf(stdout, NULL, _IONBF, 0);
                        break;
                case '?':
                default:
                        fprintf(stderr, "Usage:\n\t%s\n", usage);
                        return EXIT_FAILURE;
                }
        }

        argv += optind;
        argc -= optind;

        if (argc < 1) {
                output(stdin);
                return EXIT_SUCCESS;
        }

        for (i = 0; i < argc; i++) {
                if (strcmp(argv[i], "-") == 0)
                        f = stdin;
                else
                        f = openfile(argv[i]);

                if (f != NULL)
                        output(f);
                else
                        status = EXIT_FAILURE;
        }

        return status;
}

