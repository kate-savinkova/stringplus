#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0) {
    return 0;
  }
  int flag = 1;
  while (n-- != 0 && *str1 != '\0' && *str2 != '\0') {
    flag = 0;
    if (*str1++ != *str2++) {
      return (*--str1 - *--str2);
    }
  }
  if (flag) {
    return *str1 - *str2;
  }
  return 0;
}
