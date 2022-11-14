/* baseutils: `echo.c` - write arguments to stdout.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

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

