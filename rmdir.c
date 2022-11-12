/*
 * rmdir.c
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

static const char *usage = "rmdir [-p] dir...";
static int status;

/*
 * Remove all directories in the given path from right to left
 * rmdir() the current `path`, place an early null terminator to trim the last
 * path component and keep calling this function recursively until all 
 * mentioned directories are removed.
 */
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

/*
 * rmdir - Remove directories
 * Remove directories specified by `dir` operand in order given.
 *      -p removes all directories in a path name for each directory operand.
 */
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

