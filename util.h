/* baseutils: `util.h` - utility definitions.
   Copyright (c) 2022, Alan Potteiger
   See `LICENSE` for copyright and license details */

#ifndef _UTIL_H_
#define _UTIL_H_
#include <sys/stat.h>

int     modeset(const char *modestr, mode_t *modeset);

#endif /* _UTIL_H_ */
