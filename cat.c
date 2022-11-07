/*
 * Copyright (c) 2022, Alan Potteiger
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 * 	this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *	this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *	contributors may be used to endorse or promote products derived from
 *	this software without specific prior written permission.
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
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static const char *usage = "cat [-u] [file...]";
static int status;

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

	status = EXIT_SUCCESS;

	while ((ch = getopt(argc, argv, "u")) != -1) {
		switch (ch) {
		case 'u':
			setvbuf(stdout, NULL, _IONBF, 0);
			break;
		case '?':
		default:
			fprintf(stderr, "Usage:\n\t%s\n", usage);
			return 1;
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
