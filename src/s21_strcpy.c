#include "s21_string.h"
char *s21_strcpy(char *dest, const char *src) {
  char *original_dest = dest;
  while ((*dest++ = *src++) != '\0')
    ;
  return original_dest;
}
