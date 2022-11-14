/* baseutils: `ls.c` - list directory contents (in.. many different ways...).
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

/* This is the POSIX usage of `ls`... What happened man?
        ls [-ikqrs] [-glno] [-A|-a] [-C|-m|-x|-1] \
        [-F|-p] [-H|-L] [-R|-d] [-S|-f|-t] [-c|-u] [file...] 
   Got some work to do I guess */

static const char *usage = {
        "ls"
};

int
main(int argc, char *argv[])
{
        puts("ls: lists all the options for ls.");
        puts("Oops, no I meant:\nls: lists directory contents");
        return EXIT_SUCCESS;
}

