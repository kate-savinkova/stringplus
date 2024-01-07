#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *char_dest = (char *)dest;
  char *char_src = (char *)src;
  for (s21_size_t i = 0; i < n; ++i) char_dest[i] = char_src[i];
  return dest;
}
