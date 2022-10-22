#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static int
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
		return 1;
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

	return 0;
}

int
main(int argc, char *argv[])
{
	char ch;
	char *n;
	long l;
	int i;
	bool header;
	int status;

	n = NULL;
	header = false;
	status = 0;

	while ((ch = getopt(argc, argv, "n:")) != -1) {
		switch (ch) {
		case 'n':
			n = optarg;
			break;
		case '?':
		default:
			fprintf(stderr,
				"Usage:\n\thead [-n number] [file...]\n");
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
		status += head(NULL, l, false, 0);

	for (i = 0; i < argc; i++) {
		status += head(argv[i], l, header, i);
	}
	
	return status;
}
