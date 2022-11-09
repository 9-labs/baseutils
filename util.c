/*
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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

/*
 * Following is an excerpt from the POSIX definition of `chmod` containing a 
 * grammar for symbolic modes.
 *
 * symbolic_mode        : clause
 *                      | symbolic_mode ',' clause
 *                      ;
 *
 * clause               : actionlist
 *                      | wholist actionlist
 *                      ;
 * 
 * wholist              : who
 *                      | wholist who
 *                      ;
 * 
 * who                  : 'u' | 'g' | 'o' | 'a'
 *                      ;
 * 
 * actionlist           : action
 *                      | actionlist action
 *                      ;
 * 
 * action               : op
 *                      | op permlist
 *                      | op permcopy
 *                      ;
 * 
 * permcopy             : 'u' | 'g' | 'o'
 *                      ;
 * 
 * op                   : '+' | '-' | '='
 *                      ;
 * 
 * permlist             : perm
 *                      | perm permlist
 *                      ; 
 * 
 * perm                 : 'r' | 'w' | 'x' | 'X' | 's' | 't'  
 *                      ;
 */

/*
 * Accepts symbolic mode string `modestr` and applies its changes to `modeset`
 * Returns > 0 for syntax error and sets errno=EINVAL
 */
int
modeset(const char *modestr, mode_t *modeset)
{
        const char *ptr;
        char op, perm;
        mode_t copy, work, who, mode;

        ptr = modestr;
        mode = *modeset;

clause:
        op = perm = 0;
        copy = work = who = 0;

        /* Find who's permissions we're editing */
        for (; *ptr != '\0'; ptr++) {
                switch (*ptr) {
                case 'u':
                        who |= S_IRWXU | S_ISUID;
                        continue;
                case 'g':
                        who |= S_IRWXG | S_ISGID;
                        continue;
                case 'o':
                        who |= S_IRWXO;
                        continue;
                case 'a':
                        who |= S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID;
                        continue;
                default:
                        break;
                }

                break;
        }

        /* find operator */
        switch (*ptr) {
        case '+':
        case '-':
        case '=':
                op = *ptr;
                break;
        default:
                errno = EINVAL;
                return EXIT_FAILURE;
        }

        ptr++;

        /* find permissions, setup mask with who */
        for (; *ptr != '\0'; ptr++) {
                switch (*ptr) {
                case 'r':
                        work = who & (S_IRUSR | S_IRGRP | S_IROTH);
                        continue;
                case 'w':
                        work = who & (S_IWUSR | S_IWGRP | S_IWOTH);
                        continue;
                case 'x':
                        work = who & (S_IXUSR | S_IXGRP | S_IXOTH);
                        continue;
                /* permcopy */
                case 'u':
                        copy = *modeset;
                        copy |= copy >> 3;
                        copy |= copy >> 3;
                        work = who & copy;
                        break;
                case 'g':
                        copy = *modeset;
                        copy |= copy << 3;
                        copy |= copy >> 3;
                        work = who & copy;
                        break;
                case 'o':
                        copy = *modeset;
                        copy |= copy << 3;
                        copy |= copy << 3;
                        work = who & copy;
                        break;
                default:
                        break;
                }

                if (work == 0) {
                        /* invalid or missing permission character */
                        errno = EINVAL;
                        return EXIT_FAILURE;
                }
                break;
        }

        /* do our work on the given mode depending on our operator */
        switch (op) {
        case '+':
                /* + operator: turn selected bits on */
                mode |= work;
                break;
        case '-':
                /* - operator: turn selected bits off */
                work = ~work;
                mode &= work;
                break;
        case '=':
                /* = operator: clear user permissions mentioned, then set */
                who = ~who;
                mode &= who;

                mode |= work;
                break;
        }

        if (*ptr == ',') {
                ptr++;
                goto clause;
        }

        *modeset = mode;

        return EXIT_SUCCESS;
}

