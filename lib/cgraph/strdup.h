// platform shims for strdup

#pragma once

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
  // Windows provides strdup under a different name
  static inline char *strdup(const char *s) {
    return _strdup(s);
  }

  // Windows does not provide strndup
  static inline char *strndup(const char *s, size_t n) {

    // is this string terminated prior to the given length?
    const char *nul = memchr(s, '\0', n);

    size_t sz = nul == NULL ? n : (nul - s);

    // will adding 1 to this length overflow?
    if (SIZE_MAX - 1 < sz)
      return NULL;

    // allocate space for the string copy
    char *p = malloc(sz + 1);
    if (p == NULL)
      return NULL;

    // construct the copy
    memcpy(p, s, sz);
    p[sz] = '\0';

    return p;
  }
#endif
