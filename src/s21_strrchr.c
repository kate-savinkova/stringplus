#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *psymbol = S21_NULL;
  do {
    if (*str == c) psymbol = (char *)str;
  } while (*str++);
  return psymbol;
}