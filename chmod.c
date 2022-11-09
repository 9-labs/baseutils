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
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>

#include "util.h"

static const char *usage = "chmod [-R] mode file...";
static int status;

static mode_t mode;

static void recurse(const char *path);

static int
ftwcallback(const char *name, const struct stat *object, int flag)
{
	if (chmod(name, mode) != 0) {
		fprintf(stderr, "chmod: %s\n", strerror(errno));
		status = EXIT_FAILURE;
	}

	return 0;
}

/*
 * -R flag
 *  If named file is a directory, set the mode of all contained file and the
 *  directory itself.
 */
static void
recurse(const char *path)
{
	int r;
	
	r = ftw(path, ftwcallback, 1);

	if (r != 0) {
		fprintf(stderr, "chmod: %s\n", strerror(errno));
		status = EXIT_FAILURE;
	}
}

/*
 * chmod - Change file modes (permissions)
 * Change file `mode` of `file...`
 * 	-R If named file is directory, set mode of the directory and all the
 * 		files it contains.
 */
int
main(int argc, char *argv[])
{
	bool R;
	char *m;
	void *set;
	int i;
	struct stat *s;
	
	status = EXIT_SUCCESS;
	R = false;
	s = malloc(sizeof(struct stat));

	/*
	 * We only support one flag (-R), there is no reason to use getopt
	 * for argument parsing here, just interferes with mode parsing.
	 */

	if (strcmp(argv[1], "-R") == 0) {
		R = true;
		argc -= 1;
		argv += 1;
	}

	argc -= 1;
	argv += 1;

	if (argc < 2) {
		fprintf(stderr, "chmod: requires a mode and file argument.\n");
		fprintf(stderr, "Usage:\n\t%s\n", usage);
		return EXIT_FAILURE;
	}

	m = argv[0];

	if ((set = modecomp(m)) == NULL) {
		fprintf(stderr, "chmod: invalid file mode: %s\n", m);
		return EXIT_FAILURE;
	}

	argc--;
	argv++;

	for (i = 0; i < argc; i++) {
		stat(argv[i], s);
		mode = s->st_mode;

		mode = modeset(set, mode);

		if (R) { // -R 
			recurse(argv[i]);
			return status;
		}

		if (chmod(argv[i], mode) != 0) {
			fprintf(stderr, "chmod: %s\n", strerror(errno));
			status = EXIT_FAILURE;
		}
	}

	free(s);

	return status;
}