#ifndef _UTIL_H_
#define _UTIL_H_
#include <sys/stat.h>

mode_t	modeset(const void *set, mode_t mode);
void * 	modecomp(const char *str);

#endif /* _UTIL_H_ */
