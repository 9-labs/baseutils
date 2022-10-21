#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

static void
usage()
{
        fprintf(stderr,
                "Usage:\n"
                "\tcat [-u] [file...]\n"
        );
}

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
        int e;

        e = 0;

        setlocale(LC_ALL, "");

        while ((ch = getopt(argc, argv, "u")) != -1) {
                switch (ch) {
                case 'u':
                        setvbuf(stdout, NULL, _IONBF, 0);
                        break;
                case '?':
                default:
                        usage();
                        return 1;
                }
        }

        argv += optind;
        argc -= optind;

        if (argc < 1) {
                output(stdin);
                return 0;
        }

        for (i = 0; i < argc; i++) {
                if (strcmp(argv[i], "-") == 0)
                        f = stdin;
                else
                        f = openfile(argv[i]);

                if (f != NULL)
                        output(f);
                else
                        e = 1;
        }

        return e;
}
