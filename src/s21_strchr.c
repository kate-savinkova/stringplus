#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *psymbol = S21_NULL;
  while (*str != c && *str) str++;
  return *str == c ? (char *)str : psymbol;
}