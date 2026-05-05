#ifndef S21_STRING_H
#define S21_STRING_H

#include <stddef.h>  // для size_t

#define S21_NULL ((void*)0)

typedef size_t s21_size;

// memory
void* s21_memchr(const void* str, int c, s21_size n);
int s21_memcmp(const void* str1, const void* str2, s21_size n);
void* s21_memcpy(void* dest, const void* src, s21_size n);
void* s21_memset(void* destination, int value, s21_size n);

// string
char* s21_strncat(char* dest, const char* src, s21_size n);
char* s21_strchr(const char* str, int c);
int s21_strncmp(const char* str1, const char* str2, s21_size n);
char* s21_strncpy(char* dest, const char* src, s21_size n);
s21_size s21_strcspn(const char* str1, const char* str2);
s21_size s21_strlen(const char* str);
char* s21_strpbrk(const char* str1, const char* str2);
char* s21_strrchr(const char* str, int c);
char* s21_strstr(const char* haystack, const char* needle);
char* s21_strtok(char* str, const char* delim);
void* s21_trim(const char* src, const char* trim_chars);
void* s21_to_lower(const char* str);
void* s21_to_upper(const char* str);
void* s21_insert(const char* src, const char* str, s21_size start_index);

// доп
char* s21_strerror(int errnum);

// sprintf
int s21_sprintf(char* str, const char* format, ...);

// sscanf
int s21_sscanf(const char* str, const char* format, ...);

#endif