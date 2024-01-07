#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "s21_string.h"

int str_to_int(char *str);

long long int int_to_str(long double value, char *outStr, bool isFractPart);

long long int float_to_str(long double arg, char *outStr, int afterpoint,
                           bool isExp, int *exp);

long long int float_to_exp_str(long double num, char *outStr, char e,
                               int afterpoint);

long long int u_int_to_hex_upper_str(unsigned long long int arg, char *outStr);

long long int u_int_to_hex_lower_str(unsigned long long int arg, char *outStr);

long long int u_int_to_octal_str(unsigned long long int arg, char *outStr);

int check_exp(long double num);

bool is_left_alignment = false;
bool is_print_positive_sign = false;
bool is_print_space_non_sign = false;
bool is_print_dot_decimal = false;
bool is_print_zeros = false;

bool get_flag(const char *format);

bool get_length_type(const char *format);

bool get_number(const char *format, int *outNumber, int *outNumberLength);

bool get_specifier(const char *format);

void reset_states();

void print_format_str(char *str, va_list param);

void print_c(char *str, char arg);

void print_s(char *str, char *arg);

void print_f(char *str, long double arg);

void print_d(char *str, long long int arg);

void print_u(char *str, unsigned long long int arg);

void print_e(char *str, long double arg, char e);

void print_g(char *str, long double arg, char g);

void print_x(char *str, unsigned long long int arg, char x);

void print_o(char *str, unsigned long long int arg);

void print_p(char *str, unsigned long long int arg);

char specifiers[15] = {'c', 'd', 'i', 'e', 'E', 'f', 'g', 'G',
                       'o', 's', 'u', 'x', 'X', 'p', 'n'};

char current_specifier = ' ';

int stage_number = 0;

int width_number = -1;

int precision_number = -1;

long long index_print = 0;

int printed_symbols_counts = 0;

bool ish = false;

int isl = 0;

bool isL = false;

int s21_sprintf(char *str, const char *format, ...) {
  va_list param;
  printed_symbols_counts = 0;
  va_start(param, format);
  printed_symbols_counts = 0;
  index_print = 0;
  bool isHandle = false;
  int widthNumberLength = 0;
  int precisionNumberLength = 0;

  for (; *format; format++) {
    if (*format == '%') {
      isHandle = !isHandle;
      if (isHandle) {
        stage_number = 2;
        continue;
      }
    }
    if (isHandle) {
      if (get_flag(format)) continue;
      if (stage_number == 2) {
        if (*format == '*') {
          width_number = va_arg(param, int);
          width_number = abs(width_number);
          format++;
        } else if (get_number(format, &width_number, &widthNumberLength)) {
          format += widthNumberLength;
        }
        stage_number++;
      }
      if (stage_number == 3) {
        if (*format == '.') {
          format++;
          precision_number = 0;
          if (*format == '*') {
            precision_number = va_arg(param, int);
            if (precision_number < 0) {
              precision_number = -1;
            }
            format++;
          } else if (get_number(format, &precision_number,
                                &precisionNumberLength)) {
            format += precisionNumberLength;
          }
        }
        stage_number++;
      }
      if (stage_number == 4) {
        if (get_flag(format) || get_length_type(format)) {
          continue;
        } else {
          stage_number++;
        }
      }
      if (stage_number == 5) {
        get_specifier(format);
        print_format_str(str, param);
        reset_states();
        isHandle = false;
      }
    } else {
      str[index_print] = *format;
      index_print++;
      printed_symbols_counts++;
    }
  }
  str[index_print] = '\0';
  va_end(param);
  reset_states();
  index_print = 0;
  return printed_symbols_counts;
}

void reset_states() {
  current_specifier = ' ';
  precision_number = -1;
  width_number = -1;
  stage_number = 0;
  is_left_alignment = false;
  is_print_positive_sign = false;
  is_print_space_non_sign = false;
  is_print_dot_decimal = false;
  is_print_zeros = false;
  ish = false;
  isl = 0;
  isL = false;
}

bool get_flag(const char *format) {
  bool result = false;

  switch (*format) {
    case '-':
      is_left_alignment = true;
      result = true;
      break;
    case '+':
      is_print_positive_sign = true;
      result = true;
      break;
    case ' ':
      is_print_space_non_sign = true;
      result = true;
      break;
    case '#':
      is_print_dot_decimal = true;
      result = true;
      break;
    case '0':
      is_print_zeros = true;
      result = true;
      break;
    default:
      result = false;
      break;
  }

  return result;
}

bool get_length_type(const char *format) {
  bool result = false;

  switch (*format) {
    case 'h':
      ish = true;
      isl = 0;
      isL = false;
      result = true;
      break;
    case 'l':
      ish = false;
      isl++;
      isL = false;
      result = true;
      break;
    case 'L':
      ish = false;
      isl = 0;
      isL = true;
      result = true;
      break;
    default:
      result = false;
      break;
  }

  return result;
}

bool get_number(const char *format, int *outNumber, int *outNumberLength) {
  bool result = false;
  if (*format > 47 && *format < 58) {
    result = true;
    char buff[10] = {0};
    int buffIdx = 1;
    buff[0] = *format;
    format++;

    for (; format != NULL; format++) {
      if (*format > 47 && *format < 58) {
        buff[buffIdx] = *format;
        buffIdx++;
      } else {
        break;
      }
    }

    *outNumberLength = buffIdx;
    *outNumber = str_to_int(buff);
  }

  return result;
}

int str_to_int(char *str) {
  int num = 0;
  while (*str) {
    num = num * 10 + (*str++ - '0');
  }
  return num;
}

bool get_specifier(const char *format) {
  bool result = false;

  for (int i = 0; i < 15; i++) {
    if (*format == specifiers[i]) {
      result = true;
      current_specifier = specifiers[i];
    }
  }

  return result;
}

void print_format_str(char *str, va_list param) {
  int *arg = NULL;

  switch (current_specifier) {
    case 'c':
      print_c(str, va_arg(param, int));
      break;
    case 'd':
    case 'i':
      if (isl == 1) {
        print_d(str, va_arg(param, long int));
      } else if (isl == 2) {
        print_d(str, va_arg(param, long long int));
      } else if (ish) {
        print_d(str, va_arg(param, int));
      } else if (isl == 0 && !ish) {
        print_d(str, va_arg(param, int));
      }
      break;
    case 'f':
      if (isL)
        print_f(str, va_arg(param, long double));
      else
        print_f(str, va_arg(param, double));
      break;
    case 's':
      if (isl) {
        char smChars[1024];
        wcstombs(smChars, va_arg(param, wchar_t *), 1024);
        print_s(str, smChars);
      } else
        print_s(str, va_arg(param, char *));
      break;
    case 'n':
      arg = va_arg(param, int *);
      *arg = printed_symbols_counts;
      break;
    case 'u':
      if (isl == 1) {
        print_u(str, va_arg(param, unsigned long int));
      } else if (isl == 2) {
        print_u(str, va_arg(param, unsigned long long int));
      } else if (ish) {
        print_u(str, va_arg(param, unsigned int));
      } else if (isl == 0 && !ish) {
        print_u(str, va_arg(param, unsigned int));
      }
      break;
    case 'e':
    case 'E':
      if (isL)
        print_e(str, va_arg(param, long double), current_specifier);
      else
        print_e(str, va_arg(param, double), current_specifier);
      break;
    case 'g':
    case 'G':
      if (isL)
        print_g(str, va_arg(param, long double), current_specifier);
      else
        print_g(str, va_arg(param, double), current_specifier);
      break;
    case 'x':
    case 'X':
      if (isl == 1) {
        print_x(str, va_arg(param, unsigned long int), current_specifier);
      } else if (isl == 2) {
        print_x(str, va_arg(param, unsigned long long int), current_specifier);
      } else if (ish) {
        print_x(str, va_arg(param, unsigned int), current_specifier);
      } else if (isl == 0 && !ish) {
        print_x(str, va_arg(param, unsigned int), current_specifier);
      }
      break;
    case 'o':
      if (isl == 1) {
        print_o(str, va_arg(param, unsigned long int));
      } else if (isl == 2) {
        print_o(str, va_arg(param, unsigned long long int));
      } else if (ish) {
        print_o(str, va_arg(param, unsigned int));
      } else if (isl == 0 && !ish) {
        print_o(str, va_arg(param, unsigned int));
      }
      break;
    case 'p':
      print_p(str, va_arg(param, unsigned long int));
      break;
  }
}

void print_c(char *str, char arg) {
  s21_size_t size = width_number;

  if (1 > size) {
    size = 1;
  }

  if (is_left_alignment) {
    long long currentLength = index_print + size;

    str[index_print] = arg;
    index_print++;
    printed_symbols_counts++;
    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long spaceLength = index_print + size - 1;

    for (; index_print < spaceLength; index_print++) {
      printed_symbols_counts++;

      if (is_print_zeros)
        str[index_print] = '0';
      else
        str[index_print] = ' ';
    }
    str[index_print] = arg;
    index_print++;
    printed_symbols_counts++;
  }
}

void print_s(char *str, char *arg) {
  int size = precision_number;

  if (arg == NULL) {
    arg = "(null)";
  }

  if (size == -1) {
    size = s21_strlen(arg);
  }
  if (width_number > size) {
    size = width_number;
  }

  if (is_left_alignment) {
    long long printedLength = index_print + precision_number;
    long long currentLength = index_print + size;

    if (precision_number == -1) printedLength = currentLength;

    for (; *arg && index_print < printedLength; index_print++) {
      if (precision_number == 0)
        str[index_print] = ' ';
      else
        str[index_print] = *arg;

      printed_symbols_counts++;
      arg++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long spaceLength = 0;

    if (precision_number != -1 && width_number != -1)
      if (width_number >= precision_number)
        spaceLength = index_print + width_number - precision_number;
      else if (width_number < precision_number &&
               (width_number < (int)s21_strlen(arg)))
        spaceLength = index_print + precision_number - size;
      else if (width_number < precision_number &&
               (width_number == (int)s21_strlen(arg)))
        spaceLength = 0;
      else
        spaceLength = index_print + precision_number - width_number - 1;
    else
      spaceLength = index_print + width_number - s21_strlen(arg);

    long long currentLength = index_print + size;

    for (; index_print < spaceLength; index_print++) {
      printed_symbols_counts++;

      if (is_print_zeros)
        str[index_print] = '0';
      else
        str[index_print] = ' ';
    }
    for (; *arg && index_print < currentLength; index_print++) {
      if (precision_number == 0)
        str[index_print] = ' ';
      else
        str[index_print] = *arg;
      printed_symbols_counts++;
      arg++;
    }
  }
}

void print_f(char *str, long double arg) {
  if (arg == INFINITY) {
    print_s(str, "inf");
    return;
  }

  if (arg != arg) {
    print_s(str, "nan");
    return;
  }

  if (precision_number == -1) precision_number = 6;

  char printStr[1024] = {0};

  long long count = float_to_str(arg, printStr, precision_number, false, NULL);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long spaceLength = 0;

    if (width_number != -1) spaceLength = index_print + size - count;

    if (!is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = ' ';
      }

    long long idxPrint = 0;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      idxPrint++;
      if (spaceLength > 0) spaceLength++;
      printed_symbols_counts++;
    }

    if (is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = '0';
      }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_d(char *str, long long int arg) {
  char printStr[1024] = {0};

  long long count = int_to_str(arg, printStr, false);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (precision_number > size) {
    size = precision_number;
  }

  if (precision_number == 0 && arg == 0) {
    if (width_number == -1)
      count--;
    else
      printStr[count - 1] = ' ';
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long int spaceLength = 0;

    if (precision_number > 0)
      spaceLength = index_print + size - precision_number - 1;
    else {
      long long int strSize = s21_strlen(printStr);

      if (is_print_zeros) {
        if (arg < 0) {
          strSize--;
        } else if (is_print_space_non_sign) {
          strSize--;
        } else if (is_print_space_non_sign) {
          strSize--;
        }
      }

      spaceLength = index_print + size - strSize;
    }

    if (!is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = ' ';
      }

    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    if ((printStr[0] == ' ' && is_print_space_non_sign) || printStr[0] == '-' ||
        (printStr[0] == '+' && is_print_positive_sign)) {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    if (is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = '0';
      }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_u(char *str, unsigned long long int arg) {
  char printStr[1024] = {0};

  is_print_positive_sign = false;
  is_print_space_non_sign = false;

  long long count = int_to_str(arg, printStr, false);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (precision_number > size) {
    size = precision_number;
  }

  if (precision_number == 0 && arg == 0) {
    if (width_number == -1)
      count--;
    else
      printStr[count - 1] = ' ';
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long int spaceLength = 0;

    if (precision_number > 0)
      spaceLength = index_print + size - precision_number;
    else
      spaceLength = index_print + size - s21_strlen(printStr);

    for (; index_print < spaceLength; index_print++) {
      printed_symbols_counts++;

      if (is_print_zeros)
        str[index_print] = '0';
      else
        str[index_print] = ' ';
    }

    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    if ((printStr[0] == ' ' && is_print_space_non_sign) || printStr[0] == '-' ||
        (printStr[0] == '+' && is_print_positive_sign)) {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_e(char *str, long double arg, char e) {
  if (arg == INFINITY) {
    print_s(str, "inf");
    return;
  }

  if (arg != arg) {
    print_s(str, "nan");
    return;
  }

  if (precision_number == -1) precision_number = 6;

  char printStr[1024] = {0};

  long long count = float_to_exp_str(arg, printStr, e, precision_number);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      printZeros++;
      idxPrint++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long spaceLength = 0;

    if (width_number != -1) spaceLength = index_print + size - count;

    if (!is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = ' ';
      }

    long long idxPrint = 0;

    if (printStr[0] == ' ' || printStr[0] == '-' || printStr[0] == '+') {
      str[index_print] = printStr[0];
      index_print++;
      idxPrint++;
      if (spaceLength > 0) spaceLength++;
      printed_symbols_counts++;
    }

    if (is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = '0';
      }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_g(char *str, long double arg, char g) {
  if (arg == INFINITY) {
    print_s(str, "inf");
    return;
  }

  if (arg != arg) {
    print_s(str, "nan");
    return;
  }

  if (precision_number == -1) precision_number = 6;

  int exp = check_exp(arg);

  if ((exp < -4 || exp >= precision_number) && arg != 0) {
    precision_number--;

    long double temp;

    if (exp < 0)
      temp = arg * pow(10, abs(exp));
    else
      temp = arg / pow(10, abs(exp));

    temp = fabsl(temp);

    long double integerPart = 0;
    modfl(temp, &integerPart);  //(long long int) arg;

    long double doubleFractPart = temp - integerPart;

    doubleFractPart *= pow(10, precision_number);

    doubleFractPart = roundl(doubleFractPart);

    if (!is_print_dot_decimal && arg != 1 && (int)doubleFractPart != 1 &&
        (int)doubleFractPart != 0)
      while ((int)(fmodl(doubleFractPart, 10)) == 0) {
        precision_number--;
        doubleFractPart /= 10;
      }

    if (precision_number < 0) precision_number = 0;

    if (g == 'G')
      print_e(str, arg, 'E');
    else if (g == 'g')
      print_e(str, arg, 'e');

  } else if (arg == 0 || arg == 1) {
    precision_number = 0;

    print_f(str, arg);
  } else {
    long double integerPart = 0;
    modfl(arg, &integerPart);

    for (long double temp = fabsl(integerPart); temp > 0;) {
      precision_number--;

      temp /= 10;
      modfl(temp, &temp);
    }

    long double doubleFractPart = arg - integerPart;

    doubleFractPart *= pow(10, precision_number);

    doubleFractPart = roundl(doubleFractPart);

    if (!is_print_dot_decimal && arg != 1 && (int)doubleFractPart != 1 &&
        (int)doubleFractPart != 0)
      while ((int)(fmodl(doubleFractPart, 10)) == 0) {
        precision_number--;
        doubleFractPart /= 10;
      }

    if (precision_number < 0) precision_number = 0;

    print_f(str, arg);
  }
}

bool isPointer = false;

void print_x(char *str, unsigned long long int arg, char x) {
  char printStr[1024] = {0};

  long long count = 0;

  if (arg == 0) {
    count = 1;
    printStr[0] = '0';
  } else if (x == 'X')
    count += u_int_to_hex_upper_str(arg, printStr + count);
  else if (x == 'x')
    count += u_int_to_hex_lower_str(arg, printStr + count);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (precision_number > size) {
    size = precision_number;
  }

  if (precision_number == 0 && arg == 0) {
    if (width_number == -1)
      count--;
    else
      printStr[count - 1] = ' ';

    if (isPointer) {
      count--;
      size--;
    }
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (is_print_dot_decimal && (arg != 0 || isPointer)) {
      str[index_print] = '0';
      str[index_print + 1] = x;
      index_print += 2;
      printed_symbols_counts += 2;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long int spaceLength = 0;

    if (size == 0)
      spaceLength = index_print + width_number - precision_number;
    else if (precision_number > 0 &&
             (count <= width_number || count <= precision_number))
      spaceLength = index_print + size - precision_number;
    else {
      long long int strSize = s21_strlen(printStr);

      if (is_print_zeros && arg != 0) {
        if (is_print_space_non_sign) {
          strSize--;
        } else if (is_print_space_non_sign) {
          strSize--;
        }
      }

      spaceLength = index_print + size - strSize;
    }

    if (!is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = ' ';
      }

    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    if (is_print_dot_decimal && (arg != 0 || isPointer)) {
      str[index_print] = '0';
      str[index_print + 1] = x;
      index_print += 2;
      printed_symbols_counts += 2;
    }

    if (is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = '0';
      }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_o(char *str, unsigned long long int arg) {
  char printStr[1024] = {0};

  long long count = 0;

  if (arg == 0) {
    count = 1;
    printStr[0] = '0';
  } else
    count += u_int_to_octal_str(arg, printStr + count);

  int size = width_number;

  if (count > size) {
    size = count;
  }

  if (precision_number > size) {
    size = precision_number;
  }

  if (precision_number == 0 && arg == 0) {
    if (width_number == -1)
      count--;
    else
      printStr[count - 1] = ' ';
  }

  if (is_left_alignment) {
    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    long long currentLength = index_print + size;

    if (is_print_dot_decimal) {
      str[index_print] = '0';
      index_print++;
      printed_symbols_counts++;
    }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }

    for (; index_print < currentLength; index_print++) {
      printed_symbols_counts++;
      str[index_print] = ' ';
    }
  } else {
    long long int spaceLength = 0;

    if (precision_number > 0)
      spaceLength = index_print + size - precision_number;
    else {
      long long int strSize = s21_strlen(printStr);

      if (is_print_zeros && arg != 0) {
        if (is_print_space_non_sign) {
          strSize--;
        } else if (is_print_space_non_sign) {
          strSize--;
        }
      }

      spaceLength = index_print + size - strSize;
    }

    if (!is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = ' ';
      }

    long long printZeros = precision_number;
    printZeros -= count;
    long long idxPrint = 0;

    if (is_print_dot_decimal && arg != 0) {
      str[index_print] = '0';
      index_print++;
      printed_symbols_counts++;
      printZeros--;
    }

    if (is_print_zeros)
      for (; index_print < spaceLength; index_print++) {
        printed_symbols_counts++;

        str[index_print] = '0';
      }

    long long endPrintZeros = index_print + printZeros;

    for (; index_print < endPrintZeros; index_print++) {
      str[index_print] = '0';
      printed_symbols_counts++;
    }

    for (; idxPrint < count; index_print++, idxPrint++) {
      str[index_print] = printStr[idxPrint];
      printed_symbols_counts++;
    }
  }
}

void print_p(char *str, unsigned long long int arg) {
  is_print_dot_decimal = true;
  isPointer = true;

  if (!is_left_alignment) width_number -= 2;

  print_x(str, arg, 'x');

  isPointer = false;
}

int check_exp(long double num) {
  int exp = 0;

  if (num == 0) {
    exp = 0;
  } else if (fabsl(num) / 10 >= 1) {
    while (fabsl(num) / 10 >= 1) {
      num /= 10;
      exp++;
    }
  } else {
    while (fabsl(num) * 10 <= 9.99999) {
      num *= 10;
      exp++;
    }

    exp *= -1;
  }

  return exp;
}

long long int float_to_exp_str(long double num, char *outStr, char e,
                               int afterpoint) {
  int exp = 0;
  long long int count = 0;

  if (num == 0) {
    count = float_to_str(num, outStr, afterpoint, true, &exp);

    outStr[count++] = e;
    outStr[count++] = '+';

    count += 2;

    outStr[count - 1] = (char)(exp % 10 + '0');
    exp /= 10;
    outStr[count - 2] = (char)(exp % 10 + '0');
  } else if (fabsl(num) / 10 >= 1) {
    while (fabsl(num) / 10 >= 1) {
      num /= 10;
      exp++;
    }

    count = float_to_str(num, outStr, afterpoint, true, &exp);

    outStr[count++] = e;
    outStr[count++] = '+';

    if (exp < 10) {
      outStr[count++] = '0';
    }

    count += int_to_str(exp, outStr + count, true);
  } else {
    while (fabsl(num) * 10 <= 9.99999) {
      num *= 10;
      exp++;
    }

    count = float_to_str(num, outStr, afterpoint, true, &exp);

    outStr[count++] = e;
    outStr[count++] = '-';

    if (exp < 10) {
      outStr[count++] = '0';
    }

    count += int_to_str(exp, outStr + count, true);
  }

  return count;
}

long long int int_to_str(long double value, char *outStr, bool isFractPart) {
  long long int count = 0;

  if (outStr == NULL) return 0;

  if (!isFractPart) {
    if (value < 0) {
      value *= (-1);
      *outStr++ = '-';
      count++;
    } else if (is_print_positive_sign) {
      *outStr++ = '+';
      count++;
    } else if (is_print_space_non_sign) {
      *outStr++ = ' ';
      count++;
    }
  }

  if (value == 0) {
    *outStr = '0';
    return count + 1;
  }

  long double temp = value;

  for (; temp > 0; outStr++) {
    *outStr = '\0';

    temp /= 10;
    modfl(temp, &temp);
  }

  for (temp = value; temp > 0;) {
    *--outStr = (char)(fmodl(temp, 10) + '0');
    count++;

    temp /= 10;
    modfl(temp, &temp);
  }

  return count;
}

long long int float_to_str(long double arg, char *outStr, int afterpoint,
                           bool isExp, int *exp) {
  long double integerPart = 0;
  modfl(arg, &integerPart);

  long double doubleFractPart = arg - integerPart;
  doubleFractPart *= pow(10, afterpoint);

  long long int fractPart = (long long int)roundl(doubleFractPart);

  if (fractPart != 0 && fractPart % (int)pow(10, afterpoint) == 0) {
    if (fractPart > 0)
      integerPart++;
    else
      integerPart--;

    if (isExp)
      while (fabsl(integerPart) / 10 >= 1) {
        integerPart /= 10;
        (*exp)++;
      }

    fractPart = 0;
  }

  if (afterpoint == 0) {
    integerPart += (long long int)fractPart;

    long long int count = int_to_str(integerPart, outStr, false);

    if (is_print_dot_decimal) {
      outStr[count] = '.';
      count++;
    }

    return count;
  }

  long long int count = 0;

  count = int_to_str(integerPart, outStr, false);

  outStr[count] = '.';
  count++;

  long long int fractPartCount =
      int_to_str(llabs(fractPart), outStr + count, true);

  while (fractPartCount < afterpoint) {
    outStr[count + fractPartCount] = '0';
    fractPartCount++;
  }

  outStr[count + fractPartCount] = '\0';

  return count + fractPartCount;
}

long long int u_int_to_hex_lower_str(unsigned long long int arg, char *outStr) {
  char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

  char hex[1024] = {};
  long long int count = 0;
  do {
    hex[count++] = digits[arg % 16];
    arg /= 16;
  } while (arg != 0);

  for (long long int i = count - 1, j = 0; i > -1; i--, j++) outStr[j] = hex[i];

  return count;
}

long long int u_int_to_hex_upper_str(unsigned long long int arg, char *outStr) {
  char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                     '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  char hex[1024] = {};
  long long int count = 0;
  do {
    hex[count++] = digits[arg % 16];
    arg /= 16;
  } while (arg != 0);

  for (long long int i = count - 1, j = 0; i > -1; i--, j++) outStr[j] = hex[i];

  return count;
}

long long int u_int_to_octal_str(unsigned long long int arg, char *outStr) {
  char digits[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

  char hex[1024] = {};
  long long int count = 0;
  do {
    hex[count++] = digits[arg % 8];
    arg /= 8;
  } while (arg != 0);

  for (long long int i = count - 1, j = 0; i > -1; i--, j++) outStr[j] = hex[i];

  return count;
}
