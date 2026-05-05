#include "s21_sprintf.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

static const char* s21_parse_format(const char* f, va_list* args, int* minus,
                                    int* plus, int* space, int* zero, int* hash,
                                    int* width, int* precision, int* len_h,
                                    int* len_l, int* len_L, char* spec) {
  while (*f == '-' || *f == '+' || *f == ' ' || *f == '0' || *f == '#') {
    if (*f == '-')
      *minus = 1;
    else if (*f == '+') {
      *plus = 1;
      *space = 0;
    } else if (*f == '0')
      *zero = 1;
    else if (*f == '#')
      *hash = 1;
    else if (!*plus)
      *space = 1;
    f++;
  }

  if (*f == '*') {
    *width = va_arg(*args, int);
    f++;
    if (*width < 0) {
      *minus = 1;
      *width = -(*width);
    }
  } else {
    while (*f >= '0' && *f <= '9') *width = *width * 10 + (*f++ - '0');
  }

  if (*f == '.') {
    f++;
    if (*f == '*') {
      *precision = va_arg(*args, int);
      f++;
      if (*precision < 0) *precision = -1;
    } else {
      *precision = 0;
      while (*f >= '0' && *f <= '9')
        *precision = *precision * 10 + (*f++ - '0');
    }
  }

  if (*f == 'h') {
    *len_h = 1;
    f++;
  } else if (*f == 'l') {
    *len_l = 1;
    f++;
  } else if (*f == 'L') {
    *len_L = 1;
    f++;
  }

  *spec = *f++;
  return f;
}

static void s21_handle_char(char** str, va_list* args, int width, int minus) {
  char c = (char)va_arg(*args, int);
  int pad = width - 1;
  if (!minus)
    while (pad-- > 0) *(*str)++ = ' ';
  *(*str)++ = c;
  if (minus)
    while (pad-- > 0) *(*str)++ = ' ';
}

static void s21_handle_string(char** str, va_list* args, int width,
                              int precision, int minus) {
  char* s = va_arg(*args, char*);
  if (!s) s = "(null)";
  int len = (int)s21_strlen(s);
  if (precision != -1 && precision < len) len = precision;
  int pad = width - len;
  if (!minus)
    while (pad-- > 0) *(*str)++ = ' ';
  for (int i = 0; i < len; i++) *(*str)++ = s[i];
  if (minus)
    while (pad-- > 0) *(*str)++ = ' ';
}

int s21_sprintf(char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);
  char* start = str;

  while (*format) {
    if (*format == '%') {
      format++;
      int minus = 0, plus = 0, space = 0, zero = 0, hash = 0, width = 0,
          precision = -1, len_h = 0, len_l = 0, len_L = 0;
      char spec = 0;

      format =
          s21_parse_format(format, &args, &minus, &plus, &space, &zero, &hash,
                           &width, &precision, &len_h, &len_l, &len_L, &spec);

      char buf[1024] = {0};

      switch (spec) {
        case 'c':
          s21_handle_char(&str, &args, width, minus);
          break;
        case 's':
          s21_handle_string(&str, &args, width, precision, minus);
          break;
        case 'd':
        case 'i': {
          long long val =
              len_l ? va_arg(args, long)
                    : (len_h ? (short)va_arg(args, int) : va_arg(args, int));
          s21_write_int(buf, (unsigned long long)val, 10, 0, width, precision,
                        minus, plus, space, zero, 0, 0);
          char* p = buf;
          while (*p) *str++ = *p++;
          break;
        }
        case 'u':
        case 'o':
        case 'x':
        case 'X': {
          unsigned long long val =
              len_l ? va_arg(args, unsigned long)
                    : (len_h ? (unsigned short)va_arg(args, unsigned int)
                             : va_arg(args, unsigned int));
          int base = (spec == 'o') ? 8 : ((spec == 'u') ? 10 : 16);
          s21_write_int(buf, val, base, (spec == 'X'), width, precision, minus,
                        0, 0, zero, hash, 0);
          char* p = buf;
          while (*p) *str++ = *p++;
          break;
        }
        case 'p': {
          void* ptr = va_arg(args, void*);
          if (ptr == NULL) {
            char* nil = "(nil)";
            while (*nil) *str++ = *nil++;
            break;
          }
          s21_write_int(buf, (unsigned long long)ptr, 16, 0, width, precision,
                        minus, 0, 0, zero, 0, 1);
          char* p = buf;
          while (*p) *str++ = *p++;
          break;
        }
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G': {
          long double val =
              len_L ? va_arg(args, long double) : va_arg(args, double);
          s21_write_float(buf, val, width, precision, minus, plus, space, zero,
                          spec);
          char* p = buf;
          while (*p) *str++ = *p++;
          break;
        }
        case 'n': {
          long long count = str - start;

          if (len_l) {
            long* p = va_arg(args, long*);
            *p = (long)count;
          } else if (len_h) {
            short* p = va_arg(args, short*);
            *p = (short)count;
          } else {
            int* p = va_arg(args, int*);
            *p = (int)count;
          }

          break;
        }
        case '%':
          *str++ = '%';
          break;
        default:
          *str++ = spec;
          break;
      }
    } else {
      *str++ = *format++;
    }
  }

  *str = '\0';
  va_end(args);
  return (int)(str - start);
}