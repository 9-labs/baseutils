/* baseutils: `util.c` - general util procedures for baseutils.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700       /* Single UNIX Specification, Version 4
                                   required for -t, S_ISVTX bits */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

/* Following is an excerpt from the POSIX definition of `chmod` containing a 
   grammar for symbolic modes.
 
   symbolic_mode        : clause
                        | symbolic_mode ',' clause
                        ;
 
   clause               : actionlist
                        | wholist actionlist
                        ;
   
   wholist              : who
                        | wholist who
                        ;
   
   who                  : 'u' | 'g' | 'o' | 'a'
                        ;
   
   actionlist           : action
                        | actionlist action
                        ;
   
   action               : op
                        | op permlist
                        | op permcopy
                        ;
   
   permcopy             : 'u' | 'g' | 'o'
                        ;
   
   op                   : '+' | '-' | '='
                        ;
   
   permlist             : perm
                        | perm permlist
                        ; 

   perm                 : 'r' | 'w' | 'x' | 'X' | 's' | 't'  
   ; */

/* Accepts symbolic mode string `modestr` and applies its changes to `modeset`
   Returns > 0 for any syntax errors */
int
modeset(const char *modestr, mode_t *modeset)
{
        const char *ptr;
        char *end;
        char op, perm;
        mode_t copy, work, who, mode;

        ptr = modestr;
        mode = *modeset;

        /* if octal number we set the mode absolutely and return */
        mode = (mode_t) strtol(ptr, &end, 8);
        if (*end == '\0') {
               *modeset = mode;
               return EXIT_SUCCESS;
        }

clause:
        op = perm = 0;
        copy = work = who = 0;

        /* Find who's permissions we're editing */
        for (; *ptr != '\0'; ptr++) {
                switch (*ptr) {
                case 'u':
                        who |= S_IRWXU;
                        continue;
                case 'g':
                        who |= S_IRWXG;
                        continue;
                case 'o':
                        who |= S_IRWXO;
                        continue;
                case 'a':
                        who |= S_IRWXU | S_IRWXG | S_IRWXO;
                        continue;
                default:
                        who |= S_IRWXU | S_IRWXG | S_IRWXO;
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
                return EXIT_FAILURE;
        }

        ptr++;

        /* find permissions/permcopy and setup mask to be used */
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
                /* TODO
                case 'X':
                        continue;
                case 's':
                        continue;
                */
                case 't':
                        work |= S_ISVTX;
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

