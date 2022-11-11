/*
 * dirname.c
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

static const char *usage = "dirname string";

/*
 * dirname
 * returns the directory portion of a filename.
 *
 * This utility is simple enough that it's entire functionality is inside
 * this main function here. yummy
 */
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

