#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

#define CHECK_STRNCMP(s1, s2, n)                                            \
  do {                                                                      \
    int original = strncmp(s1, s2, n);                                      \
    int copy = s21_strncmp(s1, s2, n);                                      \
    ck_assert((original == 0 && copy == 0) || (original > 0 && copy > 0) || \
              (original < 0 && copy < 0));                                  \
  } while (0)

#define CHECK_MEMCMP(s1, s2, n)                                             \
  do {                                                                      \
    int original = memcmp(s1, s2, n);                                       \
    int copy = s21_memcmp(s1, s2, n);                                       \
    ck_assert((original == 0 && copy == 0) || (original > 0 && copy > 0) || \
              (original < 0 && copy < 0));                                  \
  } while (0)

// strlen test: basic functionality and edge cases
START_TEST(test_strlen) {
  ck_assert_int_eq(s21_strlen("hello"), strlen("hello"));
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("abc123"), strlen("abc123"));
  ck_assert_int_eq(s21_strlen("hel\nlo"), strlen("hel\nlo"));
}
END_TEST

// strncat test: concatenation behavior
START_TEST(test_strncat) {
  char buf1[20] = "Hello";
  char buf2[20] = "Hello";
  char *src = " world!";
  ck_assert_str_eq(s21_strncat(buf1, src, 6), strncat(buf2, src, 6));

  char b3[20] = "Hello";
  char b4[20] = "Hello";
  ck_assert_str_eq(s21_strncat(b3, src, 3), strncat(b4, src, 3));

  char b5[20] = "Hello";
  char b6[20] = "Hello";
  ck_assert_str_eq(s21_strncat(b5, src, 0), strncat(b6, src, 0));

  char b7[20] = "Hello";
  char b8[20] = "Hello";
  ck_assert_str_eq(s21_strncat(b7, "", 5), strncat(b8, "", 5));
}
END_TEST

// strncmp test: comparison logic
START_TEST(test_strncmp) {
  char a1[] = "abc", b1[] = "abc";
  CHECK_STRNCMP(a1, b1, 2);
  CHECK_STRNCMP(a1, b1, 0);

  char a2[] = "acd", b2[] = "abd", b3[] = "bcd";
  CHECK_STRNCMP(a2, b2, 2);
  CHECK_STRNCMP(a2, b2, 3);
  CHECK_STRNCMP(a2, b3, 3);

  char e1[] = "", e2[] = "", e3[] = "   ";
  CHECK_STRNCMP(e1, e2, 1);
  CHECK_STRNCMP(e1, e3, 2);
  CHECK_STRNCMP(a1, e3, 2);

  char h1[] = {(char)200, '\0'};
  char h2[] = {(char)100, '\0'};
  CHECK_STRNCMP(h1, h2, 1);
}
END_TEST

// memcpy test: memory copy correctness
START_TEST(test_memcpy) {
  char src[] = "Hello";
  char dest[10];
  ck_assert_ptr_eq(s21_memcpy(dest, src, 6), dest);
  ck_assert_str_eq(dest, "Hello");
}
END_TEST

// memset test: memory fill behavior
START_TEST(test_memset) {
  char buf1[10], buf2[10];

  ck_assert_ptr_eq(s21_memset(buf1, 'A', 10), buf1);
  memset(buf2, 'A', 10);
  ck_assert_mem_eq(buf1, buf2, 10);

  char b1 = 'x', b2 = 'x';
  s21_size n = 0;
  s21_memset(&b1, 'A', n);
  memset(&b2, 'A', n);
  ck_assert_int_eq(b1, b2);

  s21_memset(buf1, 0, 10);
  memset(buf2, 0, 10);
  ck_assert_mem_eq(buf1, buf2, 10);
}
END_TEST

// memcmp test: comparison of memory blocks
START_TEST(test_memcmp) {
  char a1[] = "abc";
  char b1[] = "abc";
  ck_assert_int_eq(s21_memcmp(a1, b1, 3), memcmp(a1, b1, 3));

  char a2[] = "abc\0d";
  char b2[] = "abc\0d";
  ck_assert_int_eq(s21_memcmp(a2, b2, 4), memcmp(a2, b2, 4));

  char a3[] = "abc\0f";
  char b3[] = "ab\0ce";
  CHECK_MEMCMP(a3, b3, 4);
  CHECK_MEMCMP(a3, b3, 3);
  CHECK_MEMCMP(a3, b3, 0);

  char empty[] = "";
  CHECK_MEMCMP(a3, empty, 1);
}
END_TEST

// memchr test: search character in memory
START_TEST(test_memchr) {
  char str[] = "Hello world";

  ck_assert_ptr_eq(s21_memchr(str, 'o', 11), memchr(str, 'o', 11));
  ck_assert_ptr_eq(s21_memchr(str, 'H', 11), memchr(str, 'H', 11));
  ck_assert_ptr_eq(s21_memchr(str, 'd', 11), memchr(str, 'd', 11));
  ck_assert_ptr_eq(s21_memchr(str, 'x', 11), memchr(str, 'x', 11));
  ck_assert_ptr_eq(s21_memchr(str, 'o', 4), memchr(str, 'o', 4));
  ck_assert_ptr_eq(s21_memchr(str, 'H', 0), memchr(str, 'H', 0));

  unsigned char data[] = {1, 2, 3, 4, 5};
  ck_assert_ptr_eq(s21_memchr(data, 3, 5), memchr(data, 3, 5));

  char with_null[] = {'a', 'b', '\0', 'c'};
  ck_assert_ptr_eq(s21_memchr(with_null, '\0', 4), memchr(with_null, '\0', 4));
}
END_TEST

// strcspn test: first occurrence of any character from set
START_TEST(test_strcspn) {
  ck_assert_uint_eq(s21_strcspn("abcdef", "xyzd"), strcspn("abcdef", "xyzd"));
  ck_assert_uint_eq(s21_strcspn("hello", "h"), strcspn("hello", "h"));
  ck_assert_uint_eq(s21_strcspn("abc", "xyz"), strcspn("abc", "xyz"));

  ck_assert_uint_eq(s21_strcspn("hello", ""), strcspn("hello", ""));
  ck_assert_uint_eq(s21_strcspn("", "abc"), strcspn("", "abc"));
}
END_TEST

// strncpy test: copying strings with size limit
START_TEST(test_strncpy) {
  {
    char src[] = "Spurgerl";
    char my[20] = "XXXXXXXXXXXXXXXXXXXX";
    char std[20] = "XXXXXXXXXXXXXXXXXXXX";
    ck_assert_mem_eq(s21_strncpy(my, src, 6), strncpy(std, src, 6), 6);
  }

  {
    char src[] = "Hello";
    char my[10] = "XXXXXXXXXX";
    char std[10] = "XXXXXXXXXX";
    s21_strncpy(my, src, 8);
    strncpy(std, src, 8);
    ck_assert_mem_eq(my, std, 8);
  }

  {
    char my[10] = "Original";
    char std[10] = "Original";
    s21_size n = 0;

    ck_assert_ptr_eq(s21_strncpy(my, "Ignore", n), my);
    ck_assert_ptr_eq(strncpy(std, "Ignore", n), std);
    ck_assert_mem_eq(my, std, sizeof(my));
  }

  {
    char my[5] = "AAAA";
    char std[5] = "AAAA";
    s21_strncpy(my, "", 3);
    strncpy(std, "", 3);
    ck_assert_mem_eq(my, std, 3);
  }

  {
    char src[] = "abc";
    char my[5] = "XXXXX";
    char std[5] = "XXXXX";
    s21_strncpy(my, src, 3);
    strncpy(std, src, 3);
    ck_assert_mem_eq(my, std, 3);
  }
}
END_TEST

// strstr test: substring search
START_TEST(test_strstr) {
  ck_assert_ptr_eq(s21_strstr("hello world", "world"),
                   strstr("hello world", "world"));

  ck_assert_ptr_eq(s21_strstr("abcdef", "cd"), strstr("abcdef", "cd"));

  ck_assert_ptr_eq(s21_strstr("abcdef", "gh"), strstr("abcdef", "gh"));

  ck_assert_ptr_eq(s21_strstr("abc", ""), strstr("abc", ""));

  ck_assert_ptr_eq(s21_strstr("", ""), strstr("", ""));

  ck_assert_ptr_eq(s21_strstr("aaa", "aa"), strstr("aaa", "aa"));

  ck_assert_ptr_eq(s21_strstr("abc", "abcdef"), strstr("abc", "abcdef"));
}
END_TEST

// strpbrk test: search any of a set of characters
START_TEST(test_strpbrk) {
  // matches
  ck_assert_ptr_eq(s21_strpbrk("Hello world", "H"),
                   strpbrk("Hello world", "H"));
  ck_assert_ptr_eq(s21_strpbrk("world", "d"), strpbrk("world", "d"));

  // no match
  ck_assert_ptr_eq(s21_strpbrk("abc", "def"), strpbrk("abc", "def"));

  // whitespace
  ck_assert_ptr_eq(s21_strpbrk("Hello world", " "),
                   strpbrk("Hello world", " "));

  // multiple candidates
  ck_assert_ptr_eq(s21_strpbrk("apple", "el"), strpbrk("apple", "el"));

  // empty pattern
  ck_assert_ptr_eq(s21_strpbrk("Hello world", ""), strpbrk("Hello world", ""));
}
END_TEST

// strerror test
START_TEST(test_strerror) {
  // valid range
  for (int i = 0; i < 150; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }

  // invalid values
  int cases[] = {-1, -1000, 500, 10000};
  for (int i = 0; i < 4; i++) {
    ck_assert_str_eq(s21_strerror(cases[i]), strerror(cases[i]));
  }
}
END_TEST

// strrchr test
START_TEST(test_strrchr) {
  // basic matches
  ck_assert_ptr_eq(s21_strrchr("Hello", 'l'), strrchr("Hello", 'l'));
  ck_assert_ptr_eq(s21_strrchr("abc", 'a'), strrchr("abc", 'a'));
  ck_assert_ptr_eq(s21_strrchr("abc", 'c'), strrchr("abc", 'c'));

  // multiple occurrences
  ck_assert_ptr_eq(s21_strrchr("ababa", 'a'), strrchr("ababa", 'a'));

  // not found
  ck_assert_ptr_eq(s21_strrchr("abc", 'x'), strrchr("abc", 'x'));

  // null terminator
  ck_assert_ptr_eq(s21_strrchr("abc", '\0'), strrchr("abc", '\0'));
}
END_TEST

// strchr test
START_TEST(test_strchr) {
  // basic matches
  ck_assert_ptr_eq(s21_strchr("Hello", 'l'), strchr("Hello", 'l'));
  ck_assert_ptr_eq(s21_strchr("abc", 'a'), strchr("abc", 'a'));
  ck_assert_ptr_eq(s21_strchr("abc", 'c'), strchr("abc", 'c'));

  // multiple occurrences
  ck_assert_ptr_eq(s21_strchr("ababa", 'a'), strchr("ababa", 'a'));

  // not found
  ck_assert_ptr_eq(s21_strchr("abc", 'x'), strchr("abc", 'x'));

  // null terminator
  ck_assert_ptr_eq(s21_strchr("abc", '\0'), strchr("abc", '\0'));
}
END_TEST

// strtok test
START_TEST(test_strtok) {
  // basic tokenization
  {
    char str1[] = "a,b,c";
    char str2[] = "a,b,c";

    char *t1 = s21_strtok(str1, ",");
    char *t2 = strtok(str2, ",");

    while (t1 && t2) {
      ck_assert_str_eq(t1, t2);
      t1 = s21_strtok(NULL, ",");
      t2 = strtok(NULL, ",");
    }
    ck_assert_ptr_eq(t1, t2);
  }

  // multiple delimiters in a row
  {
    char str1[] = "a,,b";
    char str2[] = "a,,b";

    char *t1 = s21_strtok(str1, ",");
    char *t2 = strtok(str2, ",");

    while (t1 && t2) {
      ck_assert_str_eq(t1, t2);
      t1 = s21_strtok(NULL, ",");
      t2 = strtok(NULL, ",");
    }
    ck_assert_ptr_eq(t1, t2);
  }

  // delimiters at the beginning
  {
    char str1[] = ",,abc";
    char str2[] = ",,abc";

    char *t1 = s21_strtok(str1, ",");
    char *t2 = strtok(str2, ",");

    ck_assert_str_eq(t1, t2);
  }

  // only delimiters
  {
    char str1[] = ",,,";
    char str2[] = ",,,";

    ck_assert_ptr_eq(s21_strtok(str1, ","), strtok(str2, ","));
  }

  // empty string
  {
    char str1[] = "";
    char str2[] = "";

    ck_assert_ptr_eq(s21_strtok(str1, ","), strtok(str2, ","));
  }

  // multiple delimiter characters
  {
    char str1[] = "a,b;c";
    char str2[] = "a,b;c";

    char *t1 = s21_strtok(str1, ",;");
    char *t2 = strtok(str2, ",;");

    while (t1 && t2) {
      ck_assert_str_eq(t1, t2);
      t1 = s21_strtok(NULL, ",;");
      t2 = strtok(NULL, ",;");
    }
    ck_assert_ptr_eq(t1, t2);
  }
}
END_TEST

// sprintf basic test
START_TEST(test_sprintf_basic) {
  char my[200];
  char std[200];

  s21_sprintf(my, "Hello %s", "world");
  sprintf(std, "Hello %s", "world");
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%d", 123);
  sprintf(std, "%d", 123);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%u", 123u);
  sprintf(std, "%u", 123u);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%c", 'A');
  sprintf(std, "%c", 'A');
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%%");
  sprintf(std, "%%");
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%*d", 5, 42);
  sprintf(std, "%*d", 5, 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%*d", -5, 42);
  sprintf(std, "%*d", -5, 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.*d", 3, 7);
  sprintf(std, "%.*d", 3, 7);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.*d", -3, 7);
  sprintf(std, "%.*d", -3, 7);
  ck_assert_str_eq(my, std);

  char fmt[] = "%\x01";
  s21_sprintf(my, fmt);

  int my_n = -1;
  int std_n = -1;

  s21_sprintf(my, "Hello%n", &my_n);
  sprintf(std, "Hello%n", &std_n);

  ck_assert_int_eq(my_n, std_n);
}
END_TEST

// sprintf flags test
START_TEST(test_sprintf_flags) {
  char my[200];
  char std[200];

  s21_sprintf(my, "%+d", 42);
  sprintf(std, "%+d", 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "% d", 42);
  sprintf(std, "% d", 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%-5d", 42);
  sprintf(std, "%-5d", 42);
  ck_assert_str_eq(my, std);

  // flag zero
  s21_sprintf(my, "%05d", 42);
  sprintf(std, "%05d", 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%+07d", -15);
  sprintf(std, "%+07d", -15);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%03d", 7);
  sprintf(std, "%03d", 7);
  ck_assert_str_eq(my, std);

  int my_n = -1;
  int std_n = -1;

  s21_sprintf(my, "%+5d%n", 42, &my_n);
  sprintf(std, "%+5d%n", 42, &std_n);

  ck_assert_int_eq(my_n, std_n);
}
END_TEST

// sprintf float test
START_TEST(test_sprintf_float) {
  char my[200];
  char std[200];
  long double ld = 3.14159L;

  // basic float
  s21_sprintf(my, "%f", 3.14);
  sprintf(std, "%f", 3.14);
  ck_assert_str_eq(my, std);

  // negative float
  s21_sprintf(my, "%f", -2.5);
  sprintf(std, "%f", -2.5);
  ck_assert_str_eq(my, std);

  // precision
  s21_sprintf(my, "%.2f", 3.14159);
  sprintf(std, "%.2f", 3.14159);
  ck_assert_str_eq(my, std);

  // width
  s21_sprintf(my, "%10.2f", 3.14);
  sprintf(std, "%10.2f", 3.14);
  ck_assert_str_eq(my, std);

  // left align
  s21_sprintf(my, "%-10.2f", 3.14);
  sprintf(std, "%-10.2f", 3.14);
  ck_assert_str_eq(my, std);

  // plus flag
  s21_sprintf(my, "%+f", 3.14);
  sprintf(std, "%+f", 3.14);
  ck_assert_str_eq(my, std);

  // space flag
  s21_sprintf(my, "% f", 3.14);
  sprintf(std, "% f", 3.14);
  ck_assert_str_eq(my, std);

  // zero value
  s21_sprintf(my, "%.0f", 0.0);
  sprintf(std, "%.0f", 0.0);
  ck_assert_str_eq(my, std);

  // flag 0 for float
  s21_sprintf(my, "%08.2f", 3.14);
  sprintf(std, "%08.2f", 3.14);
  ck_assert_str_eq(my, std);

  // e specifier (scientific notation)
  s21_sprintf(my, "%e", 314.15);
  sprintf(std, "%e", 314.15);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%012.3e", -0.00123);
  sprintf(std, "%012.3e", -0.00123);
  ck_assert_str_eq(my, std);

  // E specifier (scientific notation, uppercase)
  s21_sprintf(my, "%E", 314.15);
  sprintf(std, "%E", 314.15);
  ck_assert_str_eq(my, std);

  // g specifier
  s21_sprintf(my, "%g", 0.00001);
  sprintf(std, "%g", 0.00001);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%g", 12345.0);
  sprintf(std, "%g", 12345.0);
  ck_assert_str_eq(my, std);

  // G specifier
  s21_sprintf(my, "%G", 0.00001);
  sprintf(std, "%G", 0.00001);
  ck_assert_str_eq(my, std);

  // Тест для нуля в e формате
  s21_sprintf(my, "%e", 0.0);
  sprintf(std, "%e", 0.0);
  ck_assert_str_eq(my, std);

  // Тест для нуля с точностью 0 в e формате
  s21_sprintf(my, "%.0e", 0.0);
  sprintf(std, "%.0e", 0.0);
  ck_assert_str_eq(my, std);

  // Тест для g с нулем
  s21_sprintf(my, "%g", 0.0);
  sprintf(std, "%g", 0.0);
  ck_assert_str_eq(my, std);

  // Тест для g с precision 0
  s21_sprintf(my, "%.0g", 123.45);
  sprintf(std, "%.0g", 123.45);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.3e", 9.9999);
  sprintf(std, "%.3e", 9.9999);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.2e", 9.999);
  sprintf(std, "%.2e", 9.999);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%Lf", ld);
  sprintf(std, "%Lf", ld);

  ck_assert_str_eq(my, std);
}
END_TEST

// sprintf width and precision test
START_TEST(test_sprintf_width_precision) {
  char my[200];
  char std[200];

  s21_sprintf(my, "%5d", 42);
  sprintf(std, "%5d", 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.5d", 42);
  sprintf(std, "%.5d", 42);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%8.3d", 123);
  sprintf(std, "%8.3d", 123);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%.3s", "hello");
  sprintf(std, "%.3s", "hello");
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%10c", 'B');
  sprintf(std, "%10c", 'B');
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%10d", 1);
  sprintf(std, "%10d", 1);
  ck_assert_str_eq(my, std);

  int my_n = -1;
  int std_n = -1;

  s21_sprintf(my, "%8.3d%n", 123, &my_n);
  sprintf(std, "%8.3d%n", 123, &std_n);

  ck_assert_int_eq(my_n, std_n);
}
END_TEST

// sprintf char left align test
START_TEST(test_sprintf_char_minus) {
  char my[100];
  char std[100];

  s21_sprintf(my, "%-5c", 'A');
  sprintf(std, "%-5c", 'A');

  ck_assert_str_eq(my, std);
}
END_TEST

// sprintf string left padding test
START_TEST(test_sprintf_string_minus_padding) {
  char my[100];
  char std[100];

  s21_sprintf(my, "%-10s", "abc");
  sprintf(std, "%-10s", "abc");

  ck_assert_str_eq(my, std);
}
END_TEST

// sprintf unsigned length test
START_TEST(test_sprintf_unsigned_length) {
  char my[100];
  char std[100];

  unsigned long ul = 123456;
  unsigned short us = 42;

  s21_sprintf(my, "%lu", ul);
  sprintf(std, "%lu", ul);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%hu", us);
  sprintf(std, "%hu", us);
  ck_assert_str_eq(my, std);
}
END_TEST

// sprintf edge test
START_TEST(test_sprintf_edge) {
  char my[200];
  char std[200];

  // zero precision int
  s21_sprintf(my, "%.0d", 0);
  sprintf(std, "%.0d", 0);
  ck_assert_str_eq(my, std);

  // NULL string
  char *null_str = NULL;

  s21_sprintf(my, "%s", null_str);
  sprintf(std, "%s", (char *)null_str);
  ck_assert_str_eq(my, std);

  // multiple args
  s21_sprintf(my, "%d %s %c", 10, "hi", 'X');
  sprintf(std, "%d %s %c", 10, "hi", 'X');
  ck_assert_str_eq(my, std);

  int my_n1 = -1, my_n2 = -1;
  int std_n1 = -1, std_n2 = -1;

  s21_sprintf(my, "A%nB%nC", &my_n1, &my_n2);
  sprintf(std, "A%nB%nC", &std_n1, &std_n2);

  ck_assert_int_eq(my_n1, std_n1);
  ck_assert_int_eq(my_n2, std_n2);
}
END_TEST

// sprintf length test
START_TEST(test_sprintf_length) {
  char my[200];
  char std[200];

  short sh = -12;
  long lg = 123456;

  s21_sprintf(my, "%hd", sh);
  sprintf(std, "%hd", sh);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%ld", lg);
  sprintf(std, "%ld", lg);
  ck_assert_str_eq(my, std);

  short my_h = -1, std_h = -1;
  long my_l = -1, std_l = -1;

  s21_sprintf(my, "Test%hn", &my_h);
  sprintf(std, "Test%hn", &std_h);
  ck_assert_int_eq(my_h, std_h);

  s21_sprintf(my, "Test%ln", &my_l);
  sprintf(std, "Test%ln", &std_l);
  ck_assert_int_eq(my_l, std_l);
}
END_TEST

// trim test: basic cases
START_TEST(test_trim_basic) {
  char *r1 = s21_trim("  hello  ", " ");
  ck_assert_str_eq(r1, "hello");
  free(r1);

  char *r2 = s21_trim("***abc***", "*");
  ck_assert_str_eq(r2, "abc");
  free(r2);

  char *r3 = s21_trim("abc", "*");
  ck_assert_str_eq(r3, "abc");
  free(r3);

  char *r4 = s21_trim("aaa", "a");
  ck_assert_str_eq(r4, "");
  free(r4);
}
END_TEST

// trim test: edge cases
START_TEST(test_trim_edge) {
  char *r1 = s21_trim("", " ");
  ck_assert_str_eq(r1, "");
  free(r1);

  char *r2 = s21_trim("abc", "");
  ck_assert_str_eq(r2, "abc");
  free(r2);

  char *r3 = s21_trim("abc", NULL);
  ck_assert_str_eq(r3, "abc");
  free(r3);

  char *r4 = s21_trim(NULL, " ");
  ck_assert_ptr_null(r4);

  char *r5 = s21_trim(" \n\tabc\t\n ", " \n\t");
  ck_assert_str_eq(r5, "abc");
  free(r5);
}
END_TEST

// to_lower test: edge cases
START_TEST(test_to_lower) {
  char *res1 = s21_to_lower("HELLO");
  ck_assert_str_eq(res1, "hello");
  free(res1);

  char *res2 = s21_to_lower("hello");
  ck_assert_str_eq(res2, "hello");
  free(res2);

  char *res3 = s21_to_lower("HeLLo");
  ck_assert_str_eq(res3, "hello");
  free(res3);

  char *res4 = s21_to_lower("A1!B@2#");
  ck_assert_str_eq(res4, "a1!b@2#");
  free(res4);

  char *res5 = s21_to_lower("");
  ck_assert_str_eq(res5, "");
  free(res5);

  char *res6 = s21_to_lower(NULL);
  ck_assert_ptr_null(res6);

  char *res7 = s21_to_lower("A B C");
  ck_assert_str_eq(res7, "a b c");
  free(res7);

  char *res8 = s21_to_lower("A");
  ck_assert_str_eq(res8, "a");
  free(res8);

  char *res9 = s21_to_lower("123");
  ck_assert_str_eq(res9, "123");
  free(res9);
}
END_TEST

// to_upper test
START_TEST(test_to_upper) {
  char *res1 = s21_to_upper("hello");
  ck_assert_str_eq(res1, "HELLO");
  free(res1);

  char *res2 = s21_to_upper("HELLO");
  ck_assert_str_eq(res2, "HELLO");
  free(res2);

  char *res3 = s21_to_upper("HeLLo");
  ck_assert_str_eq(res3, "HELLO");
  free(res3);

  char *res4 = s21_to_upper("a1!b@2#");
  ck_assert_str_eq(res4, "A1!B@2#");
  free(res4);

  char *res5 = s21_to_upper("");
  ck_assert_str_eq(res5, "");
  free(res5);

  char *res6 = s21_to_upper(NULL);
  ck_assert_ptr_null(res6);

  char *res7 = s21_to_upper("A B C");
  ck_assert_str_eq(res7, "A B C");
  free(res7);

  char *res8 = s21_to_upper("a");
  ck_assert_str_eq(res8, "A");
  free(res8);

  char *res9 = s21_to_upper("123");
  ck_assert_str_eq(res9, "123");
  free(res9);
}
END_TEST

// insert test
START_TEST(test_insert) {
  char *res1 = s21_insert("Hello", " world", 5);
  ck_assert_str_eq(res1, "Hello world");
  free(res1);

  char *res2 = s21_insert("Hello", " ", 5);
  ck_assert_str_eq(res2, "Hello ");
  free(res2);

  char *res3 = s21_insert("world", "Hello ", 0);
  ck_assert_str_eq(res3, "Hello world");
  free(res3);

  char *res4 = s21_insert("Hello", "", 2);
  ck_assert_str_eq(res4, "Hello");
  free(res4);

  char *res5 = s21_insert("", "Hello", 0);
  ck_assert_str_eq(res5, "Hello");
  free(res5);

  char *res6 = s21_insert("abc", "X", 0);
  ck_assert_str_eq(res6, "Xabc");
  free(res6);

  char *res7 = s21_insert("abc", "X", 3);
  ck_assert_str_eq(res7, "abcX");
  free(res7);

  char *res8 = s21_insert(NULL, "abc", 1);
  ck_assert_ptr_null(res8);

  char *res9 = s21_insert("abc", NULL, 2);
  ck_assert_ptr_null(res9);

  char *res10 = s21_insert("abc", "X", 5);
  ck_assert_ptr_null(res10);

  char *res11 = s21_insert("a c", "!", 1);
  ck_assert_str_eq(res11, "a! c");
  free(res11);

  char *res12 = s21_insert("123", "0", 1);
  ck_assert_str_eq(res12, "1023");
  free(res12);
}
END_TEST

// sprintf bases and hash flag test
START_TEST(test_sprintf_bases) {
  char my[200];
  char std[200];

  // 1. Восьмеричная система (%o)
  s21_sprintf(my, "%o", 1234);
  sprintf(std, "%o", 1234);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%#o", 1234);
  sprintf(std, "%#o", 1234);
  ck_assert_str_eq(my, std);

  // 2. Шестнадцатеричная система (%x, %X)
  s21_sprintf(my, "%x", 255);
  sprintf(std, "%x", 255);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%X", 255);
  sprintf(std, "%X", 255);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%#x", 255);
  sprintf(std, "%#x", 255);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%#X", 255);
  sprintf(std, "%#X", 255);
  ck_assert_str_eq(my, std);

  // 3. Указатель (%p)
  int val = 42;
  void *ptr = &val;
  s21_sprintf(my, "%p", ptr);
  sprintf(std, "%p", ptr);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%p", NULL);
  sprintf(std, "%p", NULL);
  ck_assert_str_eq(my, std);

  // 4. Сложные комбинации
  s21_sprintf(my, "%#010x", 255);
  sprintf(std, "%#010x", 255);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%#-10x", 255);
  sprintf(std, "%#-10x", 255);
  ck_assert_str_eq(my, std);

  // 5. Граничный случай: ноль с решеткой
  s21_sprintf(my, "%#x", 0);
  sprintf(std, "%#x", 0);
  ck_assert_str_eq(my, std);

  s21_sprintf(my, "%#o", 0);
  sprintf(std, "%#o", 0);
  ck_assert_str_eq(my, std);
}
END_TEST

START_TEST(test_sscanf) {
  // %d
  int d1, d2;
  ck_assert_int_eq(s21_sscanf("123", "%d", &d1), sscanf("123", "%d", &d2));
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(s21_sscanf("-456", "%d", &d1), sscanf("-456", "%d", &d2));
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(s21_sscanf("+789", "%d", &d1), sscanf("+789", "%d", &d2));
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(s21_sscanf("  42", "%d", &d1), sscanf("  42", "%d", &d2));
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(s21_sscanf("12345", "%3d", &d1),
                   sscanf("12345", "%3d", &d2));
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(s21_sscanf("123", "%*d"), sscanf("123", "%*d"));

  // %hd, %ld
  short h1, h2;
  long l1, l2;
  s21_sscanf("32767", "%hd", &h1);
  sscanf("32767", "%hd", &h2);
  ck_assert_int_eq(h1, h2);
  s21_sscanf("123456789", "%ld", &l1);
  sscanf("123456789", "%ld", &l2);
  ck_assert_int_eq(l1, l2);

  // %i
  int i1, i2;
  s21_sscanf("123", "%i", &i1);
  sscanf("123", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  s21_sscanf("012", "%i", &i1);
  sscanf("012", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  s21_sscanf("0x1A", "%i", &i1);
  sscanf("0x1A", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  s21_sscanf("0XFF", "%i", &i1);
  sscanf("0XFF", "%i", &i2);
  ck_assert_int_eq(i1, i2);

  // %u
  unsigned u1, u2;
  s21_sscanf("4294967295", "%u", &u1);
  sscanf("4294967295", "%u", &u2);
  ck_assert_uint_eq(u1, u2);
  unsigned short hu1, hu2;
  s21_sscanf("65535", "%hu", &hu1);
  sscanf("65535", "%hu", &hu2);
  ck_assert_uint_eq(hu1, hu2);

  // %o
  unsigned o1, o2;
  s21_sscanf("123", "%o", &o1);
  sscanf("123", "%o", &o2);
  ck_assert_uint_eq(o1, o2);
  s21_sscanf("0777", "%o", &o1);
  sscanf("0777", "%o", &o2);
  ck_assert_uint_eq(o1, o2);

  // %x %X
  unsigned x1, x2;
  s21_sscanf("1A3f", "%x", &x1);
  sscanf("1A3f", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  s21_sscanf("0xFF", "%x", &x1);
  sscanf("0xFF", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  s21_sscanf("1A3F", "%X", &x1);
  sscanf("1A3F", "%X", &x2);
  ck_assert_uint_eq(x1, x2);

  // %p
  void *p1, *p2;
  ck_assert_int_eq(s21_sscanf("0x1A3F", "%p", &p1),
                   sscanf("0x1A3F", "%p", &p2));
  ck_assert_ptr_eq(p1, p2);

  // %s
  char s1[50], s2[50];
  s21_sscanf("hello world", "%s", s1);
  sscanf("hello world", "%s", s2);
  ck_assert_str_eq(s1, s2);
  s21_sscanf("   spaced", "%s", s1);
  sscanf("   spaced", "%s", s2);
  ck_assert_str_eq(s1, s2);
  s21_sscanf("hello", "%3s", s1);
  sscanf("hello", "%3s", s2);
  ck_assert_str_eq(s1, s2);
  ck_assert_int_eq(s21_sscanf("skip", "%*s"), sscanf("skip", "%*s"));

  // %c
  char c1, c2;
  s21_sscanf("A", "%c", &c1);
  sscanf("A", "%c", &c2);
  ck_assert_int_eq(c1, c2);
  char cb1[5], cb2[5];
  s21_sscanf("ABCD", "%4c", cb1);
  sscanf("ABCD", "%4c", cb2);
  ck_assert_int_eq(cb1[0], cb2[0]);

  // %%
  int pc1, pc2;
  ck_assert_int_eq(s21_sscanf("100%", "%d%%", &pc1),
                   sscanf("100%", "%d%%", &pc2));
  ck_assert_int_eq(pc1, pc2);
  ck_assert_int_eq(s21_sscanf("100#", "%d%%", &x1),
                   sscanf("100#", "%d%%", &x2));

  // %n
  int n1, n2;
  s21_sscanf("12345", "%3d%n", &d1, &n1);
  sscanf("12345", "%3d%n", &d2, &n2);
  ck_assert_int_eq(n1, n2);

  // %f %e %E %g %G
  float f1, f2;
  double lf1, lf2;
  long double Lf1, Lf2;
  s21_sscanf("3.14", "%f", &f1);
  sscanf("3.14", "%f", &f2);
  ck_assert_float_eq(f1, f2);
  s21_sscanf("-0.5", "%f", &f1);
  sscanf("-0.5", "%f", &f2);
  ck_assert_float_eq(f1, f2);
  s21_sscanf("1.23e-4", "%f", &f1);
  sscanf("1.23e-4", "%f", &f2);
  ck_assert_float_eq(f1, f2);
  s21_sscanf("2.5e+3", "%e", &f1);
  sscanf("2.5e+3", "%e", &f2);
  ck_assert_float_eq(f1, f2);
  s21_sscanf("123.456", "%lf", &lf1);
  sscanf("123.456", "%lf", &lf2);
  ck_assert_double_eq(lf1, lf2);
  s21_sscanf("3.1415926535", "%Lf", &Lf1);
  sscanf("3.1415926535", "%Lf", &Lf2);
  ck_assert_ldouble_eq_tol(Lf1, Lf2, 1e-10);
  s21_sscanf("1.23E-2", "%E", &f1);
  sscanf("1.23E-2", "%E", &f2);
  ck_assert_float_eq(f1, f2);

  // Комбинации
  char str1[20], str2[20];
  int num1, num2;
  ck_assert_int_eq(s21_sscanf("John 25", "%s %d", str1, &num1),
                   sscanf("John 25", "%s %d", str2, &num2));
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(num1, num2);
  unsigned long ul1, ul2;
  s21_sscanf("12345", "%lu", &ul1);
  sscanf("12345", "%lu", &ul2);
  ck_assert_uint_eq(ul1, ul2);

  // Обычные символы в format
  int val1, val2;
  ck_assert_int_eq(s21_sscanf("age=30", "age=%d", &val1),
                   sscanf("age=30", "age=%d", &val2));
  ck_assert_int_eq(val1, val2);
  ck_assert_int_eq(s21_sscanf("age:30", "age=%d", &val1),
                   sscanf("age:30", "age=%d", &val2));
}
END_TEST

// suite
Suite *string_suite(void) {
  Suite *s = suite_create("s21_string");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_strlen);
  tcase_add_test(tc, test_strncat);
  tcase_add_test(tc, test_strncmp);
  tcase_add_test(tc, test_memcpy);
  tcase_add_test(tc, test_memset);
  tcase_add_test(tc, test_memcmp);
  tcase_add_test(tc, test_memchr);
  tcase_add_test(tc, test_strcspn);
  tcase_add_test(tc, test_strncpy);
  tcase_add_test(tc, test_strstr);
  tcase_add_test(tc, test_strpbrk);
  tcase_add_test(tc, test_strerror);
  tcase_add_test(tc, test_strrchr);
  tcase_add_test(tc, test_strchr);
  tcase_add_test(tc, test_strtok);
  tcase_add_test(tc, test_sprintf_float);
  tcase_add_test(tc, test_sprintf_basic);
  tcase_add_test(tc, test_sprintf_flags);
  tcase_add_test(tc, test_sprintf_width_precision);
  tcase_add_test(tc, test_sprintf_edge);
  tcase_add_test(tc, test_sprintf_length);
  tcase_add_test(tc, test_sprintf_char_minus);
  tcase_add_test(tc, test_sprintf_unsigned_length);
  tcase_add_test(tc, test_sprintf_string_minus_padding);
  tcase_add_test(tc, test_trim_basic);
  tcase_add_test(tc, test_trim_edge);
  tcase_add_test(tc, test_to_lower);
  tcase_add_test(tc, test_to_upper);
  tcase_add_test(tc, test_insert);
  tcase_add_test(tc, test_sprintf_bases);
  tcase_add_test(tc, test_sscanf);

  suite_add_tcase(s, tc);
  return s;
}

// main
int main(void) {
  Suite *s = string_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return failed == 0 ? 0 : 1;
}