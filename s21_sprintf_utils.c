#include "s21_sprintf.h"
#include "s21_string.h"

void s21_write_int(char* buf, unsigned long long num, int base, int uppercase,
                   int width, int precision, int minus, int plus, int space,
                   int zero, int hash, int is_p) {
  char digits[64];
  int len = 0;
  char* alphabet = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

  char pref[3] = {0};
  int pref_len = 0;

  if (base == 10) {
    long long s_num = (long long)num;
    if (s_num < 0) {
      pref[pref_len++] = '-';
      num = -s_num;
    } else if (plus) {
      pref[pref_len++] = '+';
    } else if (space) {
      pref[pref_len++] = ' ';
    }
  } else if (is_p) {
    pref[pref_len++] = '0';
    pref[pref_len++] = 'x';
  } else if (hash && num != 0) {
    if (base == 8) {
      pref[pref_len++] = '0';
    } else if (base == 16) {
      pref[pref_len++] = '0';
      pref[pref_len++] = uppercase ? 'X' : 'x';
    }
  }

  unsigned long long n = num;
  if (n == 0 && precision != 0) digits[len++] = '0';
  while (n > 0) {
    digits[len++] = alphabet[n % base];
    n /= base;
  }

  while (len < precision) digits[len++] = '0';

  int total_len = len + pref_len;
  int pad = width - total_len;
  int idx = 0;

  if (zero && precision == -1 && !minus) {
    for (int i = 0; i < pref_len; i++) buf[idx++] = pref[i];
    while (pad-- > 0) buf[idx++] = '0';
  } else {
    if (!minus)
      while (pad-- > 0) buf[idx++] = ' ';
    for (int i = 0; i < pref_len; i++) buf[idx++] = pref[i];
  }

  for (int i = len - 1; i >= 0; i--) buf[idx++] = digits[i];

  if (minus)
    while (pad-- > 0) buf[idx++] = ' ';

  buf[idx] = '\0';
}