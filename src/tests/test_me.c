#include "test_me.h"

#define GREEN "\x1B[32m"
#define RED "\x1B[31m"
#define WHITE "\x1B[0m"

int main(void) {
  int failed = 0;
  Suite *s21_string_test[] = {
      // Part 1
      test_memchr(), test_memcmp(), test_memcpy(), test_memmove(),
      test_memset(), test_strcat(), test_strncat(), test_strchr(),
      test_strcmp(), test_strncmp(), test_strcpy(), test_strncpy(),
      test_strcspn(), test_strerror(), test_strlen(), test_strpbrk(),
      test_strrchr(), test_strspn(), test_strstr(), test_strtok(),

      // Part 2
      test_sprintf_c(), test_sprintf_signed_i(), test_sprintf_f(),
      test_sprintf_unsigned(), test_sprintf_string(),

      // Part 3
      test_sprintf_e(), test_sprintf_g(), test_sprintf_octal(),
      test_sprintf_percent(), test_sprintf_n(), test_sprintf_HEX(),
      test_sprintf_hex(), test_sprintf_pointer(), test_sprintf_signed(),

      // Part 5
      test_to_upper(), test_to_lower(), test_insert(), test_trim(),

      NULL};

  for (int i = 0; s21_string_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(s21_string_test[i]);
    srunner_run_all(sr, CK_NORMAL);
    failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    if (failed == 0)
      printf(GREEN "Success\n" WHITE);
    else
      printf(RED "Failed\n" WHITE);

    failed = 0;
  }

  return 0;
}
