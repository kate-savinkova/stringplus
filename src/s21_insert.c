#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = S21_NULL;
  int stop = 0;
  if (str != S21_NULL && src != S21_NULL &&
      ((start_index < s21_strlen(src)) ||
       (start_index == 0 && s21_strlen(src) == 0)))
    result =
        (char *)calloc(s21_strlen(str) + s21_strlen(src) + 1, sizeof(char));
  else
    stop = 1;

  if (!stop) {
    s21_size_t count = 0;
    for (s21_size_t i = 0; i < s21_strlen(src) + s21_strlen(str); i++) {
      if (i < start_index) {
        result[i] = src[i];
      } else if (count < s21_strlen(str)) {
        result[i] = str[count];
        count++;
      } else {
        result[i] = src[i - count];
      }
    }
  }
  return (void *)result;
}
