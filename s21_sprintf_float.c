#include "s21_sprintf.h"
#include "s21_string.h"

void s21_write_float(char* buf, double num, int width, int precision, int minus,
                     int plus, int space, int zero, char specifier) {
  if (precision < 0) precision = 6;

  char tmp[256];
  int idx = 0;
  char sign_char = 0;

  // 1. Знак (вынесен отдельно для корректной работы флага 0)
  if (num < 0) {
    sign_char = '-';
    num = -num;
  } else if (plus) {
    sign_char = '+';
  } else if (space) {
    sign_char = ' ';
  }

  // 2. Выбор формата для g/G
  char use = specifier;
  if (specifier == 'g' || specifier == 'G') {
    double t = num;
    int exp = 0;
    if (t > 0) {
      while (t >= 10.0) {
        t /= 10.0;
        exp++;
      }
      while (t < 1.0 && t > 0) {
        t *= 10.0;
        exp--;
      }
    }

    // Для g точность = общее количество ЗНАЧАЩИХ цифр
    int sig_digits = (precision == 0) ? 1 : precision;

    // Решаем, какой формат использовать
    if (exp < -4 || exp >= sig_digits) {
      use = (specifier == 'g') ? 'e' : 'E';
      // Для e формата precision = количество цифр ПОСЛЕ точки
      precision = sig_digits - 1;
    } else {
      use = 'f';
      // Для f формата precision = количество цифр ПОСЛЕ точки
      precision = sig_digits - 1 - exp;
      if (precision < 0) precision = 0;
    }
  }

  // 3. Генерация цифр
  if (use == 'e' || use == 'E') {
    double t = num;
    int exp = 0;
    if (t > 0) {
      while (t >= 10.0) {
        t /= 10.0;
        exp++;
      }
      while (t < 1.0 && t > 0) {
        t *= 10.0;
        exp--;
      }
    }

    // Округление: добавляем 0.5 * 10^(-precision) к t
    double rounding = 0.5;
    for (int r = 0; r < precision; r++) rounding /= 10.0;
    t += rounding;

    // Если после округления t стало 10.0, нормализуем
    if (t >= 10.0) {
      t /= 10.0;
      exp++;
    }

    long long int_p = (long long)t;
    tmp[idx++] = int_p + '0';

    if (precision > 0) {
      tmp[idx++] = '.';
      // Дробная часть от округленного t
      double work_frac = t - (double)int_p;
      for (int i = 0; i < precision; i++) {
        work_frac *= 10;
        int d = (int)(work_frac + 1e-12);
        tmp[idx++] = '0' + d;
        work_frac -= d;
      }
    }

    tmp[idx++] = (use == 'e') ? 'e' : 'E';
    tmp[idx++] = exp >= 0 ? '+' : '-';
    if (exp < 0) exp = -exp;

    // Экспонента всегда 2 цифры
    tmp[idx++] = (exp / 10) + '0';
    tmp[idx++] = (exp % 10) + '0';

  } else {
    // 'F' Логика
    double rounding = 0.5;
    for (int i = 0; i < precision; i++) rounding /= 10.0;
    num += rounding;
    long long int_part = (long long)num;
    double frac = num - (double)int_part;
    char int_buf[64];
    int int_len = 0;
    unsigned long long n = (unsigned long long)int_part;
    do {
      int_buf[int_len++] = "0123456789"[n % 10];
      n /= 10;
    } while (n > 0);
    for (int i = int_len - 1; i >= 0; i--) tmp[idx++] = int_buf[i];
    if (precision > 0) {
      tmp[idx++] = '.';
      for (int i = 0; i < precision; i++) {
        frac *= 10;
        int d = (int)(frac + 1e-12);
        tmp[idx++] = '0' + d;
        frac -= d;
      }
    }
  }

  // 4. Удаление trailing zeros для g/G
  if (specifier == 'g' || specifier == 'G') {
    // Находим позицию экспоненты (e или E)
    int exp_pos = -1;
    for (int i = 0; i < idx; i++) {
      if (tmp[i] == 'e' || tmp[i] == 'E') {
        exp_pos = i;
        break;
      }
    }

    int end = (exp_pos != -1) ? exp_pos - 1 : idx - 1;

    // Удаляем конечные нули в мантиссе
    while (end > 0 && tmp[end] == '0') end--;
    // Если после удаления нулей осталась точка - удаляем и её
    if (tmp[end] == '.') end--;

    if (exp_pos != -1) {
      // Если есть экспонента, сдвигаем её влево
      int shift = (exp_pos - 1) - end;
      for (int i = exp_pos; i < idx; i++) {
        tmp[i - shift] = tmp[i];
      }
      idx -= shift;
    } else {
      idx = end + 1;
    }
  }
  tmp[idx] = '\0';

  // 5. Вывод с паддингом
  int pad = width - idx - (sign_char ? 1 : 0);
  int out = 0;

  if (sign_char) buf[out++] = sign_char;

  if (!minus) {
    char fill = zero ? '0' : ' ';
    while (pad-- > 0) buf[out++] = fill;
  }

  for (int i = 0; i < idx; i++) buf[out++] = tmp[i];
  if (minus)
    while (pad-- > 0) buf[out++] = ' ';
  buf[out] = '\0';
}