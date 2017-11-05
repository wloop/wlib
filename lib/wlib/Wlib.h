#ifndef EMBEDDEDTESTS_WLIB_H
#define EMBEDDEDTESTS_WLIB_H

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(x, y) (((x) < (y)) ? (x) : (y))

#endif //EMBEDDEDTESTS_WLIB_H
