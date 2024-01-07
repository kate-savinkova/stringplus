#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *str_copy = (char *)str;
  char *psymbol = S21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (str_copy[i] == c) {
      psymbol = str_copy + i;
      break;
    }
  }
  return psymbol;
}