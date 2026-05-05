#ifndef S21_SPRINTF_INTERNAL_H
#define S21_SPRINTF_INTERNAL_H

void s21_write_int(char* buf, unsigned long long num, int base, int uppercase,
                   int width, int precision, int minus, int plus, int space,
                   int zero, int hash, int is_p);

void s21_write_float(char* buf, double num, int width, int precision, int minus,
                     int plus, int space, int zero, char specifier);

#endif