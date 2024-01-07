#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = s21_strlen(dest), j = 0;
  while (src[j] && j < n) {
    dest[i + j] = src[j];
    ++j;
  }
  return dest;
}