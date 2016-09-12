#ifndef DEFS_H
#define DEFS_H

#define SIP_USER "ilianz"
#define SIP_DOMAIN "example.com"
#define SIP_PASS "secret"

#include <pjmedia.h>

#define IS_DIGIT(D) (((D >= '0') && (D <= '9')))

// if somebody has this werid name ...
#ifdef MATH_PI
#   undef MATH_PI
#   define MATH_PI  (3.14159265)
#else
#   define MATH_PI  (3.14159265)
#endif

typedef int (*thCb)(int, void*);
typedef pj_status_t (*pjThrEpoint)(void*);

union port_data {
    pj_int16_t *buff;
};

#endif // DEFS_H
