// platform shims for strdup

#pragma once

#include <string.h>

#ifdef _MSC_VER
  // Windows provides strdup under a different name
  static inline char *strdup(const char *s) {
    return _strdup(s);
  }
#endif
