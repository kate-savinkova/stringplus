#include "s21_string.h"

int s21_strcmp(const char *str1, const char *str2) {
  s21_size_t i = 0;
  s21_size_t strlen_1 = s21_strlen(str1);
  s21_size_t strlen_2 = s21_strlen(str2);
  for (; (int)*str1 - (int)*str2 == 0 && i < strlen_1 && i < strlen_2;
       str1++, str2++, i++)
    ;
  return *str1 - *str2;
}
