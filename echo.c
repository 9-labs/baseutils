/*
 * echo.c
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

static int
echo(char *s)
{
        char *p;
        char i;
        char *err;
        char oct;

        p = s;

        for (; *p != '\0'; p++) {
                if (*p != '\\') {
                        putchar(*p);
                        continue;
                }

                switch (*(p + 1)) {
                case 'a':
                        putchar('\a');
                        p++;
                        continue;
                case 'b':
                        putchar('\b');
                        p++;
                        continue;
                case 'c':
                        return 1;
                case 'f':
                        putchar('\f');
                        p++;
                        continue;
                case 'n':
                        putchar('\n');
                        p++;
                        continue;
                case 'r':
                        putchar('\r');
                        p++;
                        continue;
                case 't':
                        putchar('\t');
                        p++;
                        continue;
                case 'v':
                        putchar('\v');
                        p++;
                        continue;
                case '\\':
                        putchar('\\');
                        p++;
                        continue;
                case '0': /* 3 digit octal number */
                        p += 2;
                        oct = strtol(p, &err, 8);
                        p = err-1;
                        putchar(oct);
                        continue;
                default:
                        putchar('\\');
                        continue;
                }
        }

        return 0;
}

int
main(int argc, char *argv[])
{
        int i;

        if (argc == 1) {
                printf("\n");
                return EXIT_SUCCESS;
        }

        argc--;
        argv++;
        
        for (i = 0; i < argc; i++) {
                if (echo(argv[i]) > 0)
                        return EXIT_SUCCESS;
                if (i < argc)
                        putchar(' ');
        }

        putchar('\n');

        return EXIT_SUCCESS;
}

