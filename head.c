/*
 * head.c
 *
 * Copyright (c) 2022, Alan Potteiger
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static const char *usage = "head [-n number] [file...]";
static int status;

static void
head(char *filename, long line, bool header, int count)
{
        FILE *f;
        long i;
        char ch;

        if (filename != NULL)
                f = fopen(filename, "r+");
        else
                f = stdin;

        if (f == NULL) {
                fprintf(stderr, "head: %s: %s\n", filename, strerror(errno));
                status = EXIT_FAILURE;
                return;
        }

        if (header && count > 0)
                printf("\n==> %s <==\n", filename);

        if (header && count == 0)
                printf("==> %s <==\n", filename);

        for (i = 1; i <= line;) {
                ch = fgetc(f);
                putchar(ch);
                if (ch == '\n')
                        i++;

                if (ch == EOF)
                        break;
        }

        if (filename != NULL)
                fclose(f);
}

int
main(int argc, char *argv[])
{
        char ch;
        char *n;
        long l;
        int i;
        bool header;

        n = NULL;
        header = false;
        status = EXIT_SUCCESS;

        while ((ch = getopt(argc, argv, "n:")) != -1) {
                switch (ch) {
                case 'n':
                        n = optarg;
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

        if (argc > 1)
                header = true;

        if (n != NULL) {
                l = strtol(n, NULL, 10);
        } else
                l = 10;

        if (l == 0) {
                if (errno == EINVAL || errno == ERANGE)
                        fprintf(stderr, "head: %s\n", strerror(errno));
                else
                        fprintf(stderr, "head: -n expects positive decimal "
                                "integer.\n");
                
                return 1;
        }

        if (argc == 0)
                head(NULL, l, false, 0);

        for (i = 0; i < argc; i++) {
                head(argv[i], l, header, i);
        }
        
        return status;
}

