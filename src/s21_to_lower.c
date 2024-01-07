#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *result = S21_NULL;
  int stop = 0;
  if (str != S21_NULL)
    result = (char *)calloc(s21_strlen(str) + 1, sizeof(char));
  else
    stop = 1;
  if (!stop) {
    for (s21_size_t i = 0; i < s21_strlen(str); i++) {
      if ((int)str[i] >= 65 && (int)str[i] <= 90) {
        result[i] = (char)((int)str[i] + 32);
      } else {
        result[i] = str[i];
      }
    }
  }
  if (!stop) result[s21_strlen(str)] = '\0';
  return (void *)result;
}
