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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

/*
 * This is a portable parser for symbolic file modes (ex. in `chmod`) to
 * be used throughout baseutils. Inspired by `getmode()` and `setmode()` as
 * seen in <unistd.h> throughout various BSD libc implementations.
 * 	-- Alan
 */

/*
 * Following is an excerpt from the POSIX definition of `chmod` containing a 
 * grammar for symbolic modes.
 *
 * symbolic_mode	: clause
 * 			| symbolic_mode ',' clause
 * 			;
 *
 * clause		: actionlist
 * 			| wholist actionlist
 *			;
 * 
 * wholist		: who
 * 			| wholist who
 * 			;
 * 
 * who			: 'u' | 'g' | 'o' | 'a'
 * 			;
 * 
 * actionlist		: action
 * 			| actionlist action
 *			;
 * 
 * action		: op
 * 			| op permlist
 *			| op permcopy
 *			;
 * 
 * permcopy		: 'u' | 'g' | 'o'
 * 			;
 * 
 * op			: '+' | '-' | '='
 * 			;
 * 
 * permlist		: perm
 * 			| perm permlist
 *			; 
 * 
 * perm			: 'r' | 'w' | 'x' | 'X' | 's' | 't'  
 *			;
 */

struct action {
	char op;
	char permcopy;
	mode_t perms;
	struct action *next;
};

enum state {
	CLAUSE,
	WHOLIST,
	ACTION,
	OP,
	PERMCOPY,
	PERM,
	APPLY
};

mode_t
modeset(const void *set, mode_t mode)
{
	struct action *action;
	mode_t mid;
	bool u, g, o;

	action = (struct action *) set;

	if (action->permcopy != '\0') {

		action->perms = mode;

		if (action->permcopy != 'u') {
			mid = 0;
			mid |= S_IRWXU;
			mid = ~mid;
			action->perms &= mid;
		}

		if (action->permcopy != 'g') {
			mid = 0;
			mid |= S_IRWXG;
			mid = ~mid;
			action->perms &= mid;
		}

		if (action->permcopy != 'o') {
			mid = 0;
			mid |= S_IRWXO;
			mid = ~mid;
			action->perms &= mid;
		}

	}

	switch (action->op) {
	case '+':
		printf("Fuck: %o\n", action->perms);
		mode |= action->perms;
		break;
	case '-':
		action->perms = ~action->perms;
		mode &= action->perms;
		break;
	case '\0':
	default:
		break;
	}

	if (action->next != NULL)
		mode = modeset((void *) action->next, mode);

	return mode;
}

void * /* compiling state to mode_t */
modecomp(const char *str)
{
	enum state state;
	const char *ptr;
	struct action *root;
	struct action *curr;
	bool u, g, o;
	bool r, w, x, X, s, t;

	state = CLAUSE;
	ptr = str;

	root = NULL;
	curr = NULL;

	r = w = x = X = s = t = false;

	while (*ptr != '\0' || state == APPLY) {

		if (*ptr == ',') {
			ptr++;
			state = CLAUSE;
			continue;
		}

		switch (*ptr) {
		case 'u': case 'g': case 'o': case 'a':
		case '+': case '-': case '=':
		case 'r': case 'w': case 'x': case 'X': case 's': case 't':
			break;
		default:
			if (state != APPLY) {
				errno = EINVAL;
				return NULL;
			}
		}

		switch (state) {
			case CLAUSE:
				u = g = o = false;
				state = WHOLIST;
				continue;
			case WHOLIST:
				switch (*ptr) {
					case 'u':
						u = true;
						ptr++;
						state = WHOLIST;
						continue;
					case 'g':
						g = true;
						ptr++;
						state = WHOLIST;
						continue;
					case 'o':
						o = true;
						ptr++;
						state = WHOLIST;
						continue;
					case 'a':
						u = g = o = true;
						ptr++;
						state = WHOLIST;
						continue;
					default:
						state = ACTION;
						continue;
				}
			case ACTION:
				if (root == NULL) {
					root = calloc(1,
						sizeof(struct action));
					curr = root;
				} else {
					curr->next = calloc(1,
						sizeof(struct action));
					curr = curr->next;
				}
				curr->op = '\0';
				state = OP;
			case OP:
				switch (*ptr) {
				case '+':
					curr->op = '+';
					ptr++;
					state = PERMCOPY;
					continue;
				case '-':
					curr->op = '-';
					ptr++;
					state = PERMCOPY;
					continue;
				case '=':
					curr->op = '=';
					ptr++;
					state = PERMCOPY;
					continue;
				default:
					// an operator is required
					errno = EINVAL;
					return NULL;
				}
			case PERMCOPY:
				switch (*ptr) {
				case 'u':
					curr->permcopy = 'u';
					ptr++;
					state = APPLY;
					continue;
				case 'g':
					curr->permcopy = 'g';
					ptr++;
					state = APPLY;
					continue;
				case 'o':
					curr->permcopy = 'o';
					ptr++;
					state = APPLY;
					continue;
				default:
					state = PERM;
					continue;
				}
			case PERM:
				switch (*ptr) {
				case 'r':
					r = true;
					ptr++;
					state = PERM;
					break;
				case 'w':
					w = true;
					ptr++;
					state = PERM;
					break;
				case 'x':
					x = true;
					ptr++;
					state = PERM;
					break;
				default:
					if (*ptr == ',') {
						ptr++;
						state = CLAUSE;
						continue;
					}
					state = CLAUSE;
					break;
				}
			case APPLY:
				/*
				 * If we have an = operator we're going to 
				 * change it to a - with the current wholist
				 * and all permissions. Then create a new
				 * action of op + with the following perms.
				 */
				if (curr->op == '=') {
					curr->op = '-';
					if (u) {
						curr->perms |= S_IRUSR;
						curr->perms |= S_IWUSR;
						curr->perms |= S_IXUSR;
					}
					if (g) {
						curr->perms |= S_IRGRP;
						curr->perms |= S_IWGRP;
						curr->perms |= S_IXGRP;
					}
					if (o) {
						curr->perms |= S_IROTH;
						curr->perms |= S_IWOTH;
						curr->perms |= S_IXOTH;
					}

					curr->next = calloc(1,
							sizeof(struct action));

					if (curr->permcopy != '\0')
						curr->next->permcopy =
							curr->permcopy;

					curr = curr->next;
					curr->op = '+';
				}

				if (r) {
					if (u)
						curr->perms |= S_IRUSR;
					if (g)
						curr->perms |= S_IRGRP;
					if (o)
						curr->perms |= S_IROTH;
					r = false;
				}

				if (w) {
					if (u)
						curr->perms |= S_IWUSR;
					if (g)
						curr->perms |= S_IWGRP;
					if (o)
						curr->perms |= S_IWOTH;

					w = false;
				}

				if (x) {
					if (u)
						curr->perms |= S_IXUSR;
					if (g)
						curr->perms |= S_IXGRP;
					if (o)
						curr->perms |= S_IXOTH;

					x = false;
				}

				state = PERM;

				continue;
		}
	}

	return root;
}

