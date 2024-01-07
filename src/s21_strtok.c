#include "s21_string.h"

char* s21_strtok(char* str, const char* delim) {
  static char* mem = S21_NULL;

  if (str) mem = str;

  if (mem) {
    str = mem + s21_strspn(mem, delim);
    mem = str + s21_strcspn(str, delim);
    if (*mem != '\0') {
      *mem = '\0';
      ++mem;
    }
  }
  if (str)
    return *str != '\0' ? str : S21_NULL;
  else
    return S21_NULL;
}
