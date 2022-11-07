#ifndef _MODE_H_
#define _MODE_H_
#include <sys/stat.h>

mode_t	modeset(const void *set, mode_t mode);
void * 	modecomp(const char *str);

#endif
