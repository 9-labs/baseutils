/*
 * basename.c
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
#include <errno.h>
#include <string.h>

static const char *usage = "basename string [suffix]";

/*
 * basename
 * returns the non-directory portion of a filename.
 *
 * This utility is simple enough that it's entire functionality is inside
 * this main function here. yummy
 */
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

