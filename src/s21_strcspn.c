#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t answer = 0;
  s21_size_t temp1 = 0;
  s21_size_t temp2 = 0;
  for (; *str1; str1++) {
    temp1 = 0;
    for (s21_size_t i = 0; i < s21_strlen(str2); i++) {
      if (*str1 == *(str2 + i)) {
        temp1 = 1;
        break;
      }
    }

    if (temp1 == 0) {
      temp2 += 1;
    }

    if (temp1 == 1) break;
  }
  answer = temp2;
  return answer;
}
