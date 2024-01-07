#include "s21_string.h"
s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t i, j;
  i = 0;
  while (*(str1 + i)) {
    j = 0;
    while (*(str2 + j)) {
      if (*(str1 + i) == *(str2 + j)) {
        break;  // нашли соответствие
      }
      j++;
    }
    if (!*(str2 + j)) {
      return i;  // не нашли соответствие
    }
    i++;
  }
  return i;
}
