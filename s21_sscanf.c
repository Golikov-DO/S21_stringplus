#include <ctype.h>
#include <math.h>
#include <stdarg.h>

#include "s21_string.h"

/* ================= HELPERS ================= */

static int s21_atos(const char **p, char *res, int width) {
  const char *s = *p;
  int i = 0;

  while ((width != 0) && s[i] && !isspace(s[i])) {
    if (res) res[i] = s[i];
    i++;
    if (width > 0) width--;
  }

  if (res) res[i] = '\0';
  *p += i;

  return i > 0;
}

static int s21_atoi_scale(const char **p, long long *res, int width, int base) {
  const char *s = *p;
  int sign = 1;
  unsigned long long val = 0;
  int digits = 0;

  if (width != 0 && (*s == '-' || *s == '+')) {
    if (*s == '-') sign = -1;
    s++;
    if (width > 0) width--;
  }

  if (base == 16 && (width == -1 || width >= 2) && s[0] == '0' &&
      (s[1] == 'x' || s[1] == 'X')) {
    s += 2;
    if (width > 0) width -= 2;
  }

  while ((width != 0) && *s) {
    int d = -1;

    if (isdigit(*s))
      d = *s - '0';
    else if (*s >= 'a' && *s <= 'f')
      d = *s - 'a' + 10;
    else if (*s >= 'A' && *s <= 'F')
      d = *s - 'A' + 10;

    if (d < 0 || d >= base) break;

    val = val * base + d;
    s++;
    digits++;
    if (width > 0) width--;
  }

  if (!digits) return 0;

  if (res) *res = (long long)(val * sign);
  *p = s;

  return 1;
}

static int s21_atof(const char **p, long double *res, int width) {
  const char *s = *p;
  int sign = 1;
  long double val = 0.0;

  if (width != 0 && (*s == '-' || *s == '+')) {
    if (*s == '-') sign = -1;
    s++;
    if (width > 0) width--;
  }

  while ((width != 0) && isdigit(*s)) {
    val = val * 10 + (*s - '0');
    s++;
    if (width > 0) width--;
  }

  if (width != 0 && *s == '.') {
    s++;
    if (width > 0) width--;
    long double k = 0.1;

    while ((width != 0) && isdigit(*s)) {
      val += (*s - '0') * k;
      k /= 10;
      s++;
      if (width > 0) width--;
    }
  }

  if (width != 0 && (*s == 'e' || *s == 'E')) {
    s++;
    if (width > 0) width--;

    int esign = 1;
    if (*s == '-' || *s == '+') {
      if (*s == '-') esign = -1;
      s++;
      if (width > 0) width--;
    }

    int exp = 0;
    while ((width != 0) && isdigit(*s)) {
      exp = exp * 10 + (*s - '0');
      s++;
      if (width > 0) width--;
    }

    long double p10 = powl(10.0L, exp);
    val = esign == 1 ? val * p10 : val / p10;
  }

  if (res) *res = sign * val;
  *p = s;

  return 1;
}

/* ================= MAIN ================= */

int s21_sscanf(const char *str, const char *format, ...) {
  const char *s = str;
  const char *start = str;
  const char *f = format;

  va_list ap;
  va_start(ap, format);

  int count = 0;

  while (*f) {
    if (*f == '%') {
      f++;

      int suppress = 0;
      int width = -1;
      char length = 0;

      if (*f == '*') {
        suppress = 1;
        f++;
      }

      if (isdigit(*f)) {
        width = 0;
        while (isdigit(*f)) {
          width = width * 10 + (*f - '0');
          f++;
        }
      }

      if (*f == 'h' || *f == 'l' || *f == 'L') {
        length = *f;
        f++;
      }

      if (*f != 'c')
        while (isspace(*s)) s++;

      if (*f == 's') {
        char *dst = suppress ? NULL : va_arg(ap, char *);
        if (s21_atos(&s, dst, width) && !suppress) count++;

      } else if (*f == 'd' || *f == 'i') {
        long long tmp;
        int base = (*f == 'd') ? 10 : 10;

        if (*f == 'i') {
          const char *chk = s;
          if (*chk == '-' || *chk == '+') chk++;
          if (*chk == '0') {
            if (chk[1] == 'x' || chk[1] == 'X')
              base = 16;
            else
              base = 8;
          }
        }

        if (s21_atoi_scale(&s, suppress ? NULL : &tmp, width, base)) {
          if (!suppress) {
            if (length == 'l') {
              long *p = va_arg(ap, long *);
              *p = (long)tmp;
            } else if (length == 'h') {
              short *p = va_arg(ap, short *);
              *p = (short)tmp;
            } else {
              int *p = va_arg(ap, int *);
              *p = (int)tmp;
            }
            count++;
          }
        }

      } else if (*f == 'u' || *f == 'o' || *f == 'x' || *f == 'X') {
        int base = (*f == 'o') ? 8 : (*f == 'u' ? 10 : 16);
        long long tmp;

        if (s21_atoi_scale(&s, suppress ? NULL : &tmp, width, base)) {
          if (!suppress) {
            if (length == 'l') {
              unsigned long *p = va_arg(ap, unsigned long *);
              *p = (unsigned long)tmp;
            } else if (length == 'h') {
              unsigned short *p = va_arg(ap, unsigned short *);
              *p = (unsigned short)tmp;
            } else {
              unsigned int *p = va_arg(ap, unsigned int *);
              *p = (unsigned int)tmp;
            }
            count++;
          }
        }

      } else if (*f == 'f' || *f == 'e' || *f == 'E' || *f == 'g' ||
                 *f == 'G') {
        long double tmp;
        if (s21_atof(&s, suppress ? NULL : &tmp, width)) {
          if (!suppress) {
            if (length == 'L') {
              long double *p = va_arg(ap, long double *);
              *p = tmp;
            } else if (length == 'l') {
              double *p = va_arg(ap, double *);
              *p = (double)tmp;
            } else {
              float *p = va_arg(ap, float *);
              *p = (float)tmp;
            }
            count++;
          }
        }

      } else if (*f == 'c') {
        int w = width == -1 ? 1 : width;
        char *dst = suppress ? NULL : va_arg(ap, char *);

        for (int i = 0; i < w && *s; i++) {
          if (!suppress) dst[i] = *s;
          s++;
        }

        if (!suppress) count++;

      } else if (*f == 'p') {
        long long tmp;
        if (s21_atoi_scale(&s, suppress ? NULL : &tmp, width, 16)) {
          if (!suppress) {
            void **p = va_arg(ap, void **);
            *p = (void *)(unsigned long long)tmp;
            count++;
          }
        }

      } else if (*f == 'n') {
        int *p = va_arg(ap, int *);
        *p = s - start;

      } else if (*f == '%') {
        if (*s == '%')
          s++;
        else
          break;
      }

      f++;

    } else if (isspace(*f)) {
      while (isspace(*s)) s++;
      f++;

    } else {
      if (*s == *f) {
        s++;
        f++;
      } else {
        break;
      }
    }
  }

  va_end(ap);
  return count;
}