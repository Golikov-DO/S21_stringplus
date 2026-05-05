#include "s21_string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__linux__)
static const int LINUX_MAX_ERR = 133;
static const char* LINUX_ERR_PREFIX = "Unknown error ";
static const char* LINUX_ERRORS[] = {
    "Success",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "No such device or address",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource temporarily unavailable",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Device or resource busy",
    "File exists",
    "Invalid cross-device link",
    "No such device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Numerical result out of range",
    "Resource deadlock avoided",
    "File name too long",
    "No locks available",
    "Function not implemented",
    "Directory not empty",
    "Too many levels of symbolic links",
    "Unknown error 41",
    "No message of desired type",
    "Identifier removed",
    "Channel number out of range",
    "Level 2 not synchronized",
    "Level 3 halted",
    "Level 3 reset",
    "Link number out of range",
    "Protocol driver not attached",
    "No CSI structure available",
    "Level 2 halted",
    "Invalid exchange",
    "Invalid request descriptor",
    "Exchange full",
    "No anode",
    "Invalid request code",
    "Invalid slot",
    "Unknown error 58",
    "Bad font file format",
    "Device not a stream",
    "No data available",
    "Timer expired",
    "Out of streams resources",
    "Machine is not on the network",
    "Package not installed",
    "Object is remote",
    "Link has been severed",
    "Advertise error",
    "Srmount error",
    "Communication error on send",
    "Protocol error",
    "Multihop attempted",
    "RFS specific error",
    "Bad message",
    "Value too large for defined data type",
    "Name not unique on network",
    "File descriptor in bad state",
    "Remote address changed",
    "Can not access a needed shared library",
    "Accessing a corrupted shared library",
    ".lib section in a.out corrupted",
    "Attempting to link in too many shared libraries",
    "Cannot exec a shared library directly",
    "Invalid or incomplete multibyte or wide character",
    "Interrupted system call should be restarted",
    "Streams pipe error",
    "Too many users",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol",
    "Address already in use",
    "Cannot assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Transport endpoint is already connected",
    "Transport endpoint is not connected",
    "Cannot send after transport endpoint shutdown",
    "Too many references: cannot splice",
    "Connection timed out",
    "Connection refused",
    "Host is down",
    "No route to host",
    "Operation already in progress",
    "Operation now in progress",
    "Stale file handle",
    "Structure needs cleaning",
    "Not a XENIX named type file",
    "No XENIX semaphores available",
    "Is a named type file",
    "Remote I/O error",
    "Disk quota exceeded",
    "No medium found",
    "Wrong medium type",
    "Operation canceled",
    "Required key not available",
    "Key has expired",
    "Key has been revoked",
    "Key was rejected by service",
    "Owner died",
    "State not recoverable",
    "Operation not possible due to RF-kill",
    "Memory page has hardware error"};
#endif

#if defined(__APPLE__) || defined(__MACH__)
static const int MAC_MAX_ERR = 107;
static const char* MAC_ERR_PREFIX = "Unknown error: ";
static const char* MAC_ERRORS[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: can't splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC prog. not avail",
    "Program version wrong",
    "Bad procedure for program",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error",
    "Value too large to be stored in data type",
    "Bad executable (or shared library)",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "EMULTIHOP (Reserved)",
    "No message available on STREAM",
    "ENOLINK (Reserved)",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "Policy not found",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full",
    "Capabilities insufficient"};
#endif

int s21_memcmp(const void* str1, const void* str2, s21_size n) {
  const unsigned char* ptr_1 = str1;
  const unsigned char* ptr_2 = str2;
  s21_size itr_count = 0;
  while (itr_count < n) {
    if (*ptr_1 != *ptr_2) {
      return (int)(*ptr_1 - *ptr_2);
    }
    ptr_1++;
    ptr_2++;
    itr_count++;
  }

  return 0;
}

void* s21_memchr(const void* str, int c, s21_size n) {
  const unsigned char* ptr = (const unsigned char*)str;

  for (s21_size i = 0; i < n; i++) {
    if (ptr[i] == (unsigned char)c) {
      return (void*)&ptr[i];
    }
  }

  return NULL;
}

s21_size s21_strlen(const char* str) {
  s21_size len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

char* s21_strncat(char* dest, const char* src, s21_size n) {
  s21_size dest_len = s21_strlen(dest);
  s21_size i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[dest_len + i] = src[i];
  }
  dest[dest_len + i] = '\0';

  return dest;
}

char* s21_strncpy(char* dest, const char* src, s21_size n) {
  char* res = dest;
  s21_size i = 0;

  while (i < n && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }

  while (i < n) {
    dest[i] = '\0';
    i++;
  }

  return res;
}

int s21_strncmp(const char* str1, const char* str2, s21_size n) {
  for (s21_size i = 0; i < n; i++) {
    if (str1[i] == '\0' || str2[i] == '\0' || str1[i] != str2[i]) {
      return (unsigned char)str1[i] - (unsigned char)str2[i];
    }
  }

  return 0;
}

void* s21_memcpy(void* dest, const void* src, s21_size n) {
  unsigned char* d = (unsigned char*)dest;
  const unsigned char* s = (const unsigned char*)src;

  for (s21_size i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return dest;
}

void* s21_memset(void* destination, int value, s21_size n) {
  unsigned char* p = destination;
  for (s21_size i = 0; i < n; i++) {
    p[i] = (unsigned char)value;
  }

  return destination;
}

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size count = 0;
  int found = 0;
  while (str1[count] != '\0' && !found) {
    for (s21_size i = 0; str2[i] != '\0' && !found; i++) {
      if (str1[count] == str2[i]) {
        found = 1;
      }
    }
    if (!found) {
      count++;
    }
  }
  return count;
}

char* s21_strstr(const char* haystack, const char* needle) {
  char* result = S21_NULL;

  if (*needle == '\0') {
    result = (char*)haystack;
  } else {
    for (s21_size i = 0; haystack[i] != '\0' && result == S21_NULL; i++) {
      s21_size j = 0;

      while (needle[j] != '\0' && haystack[i + j] != '\0' &&
             haystack[i + j] == needle[j]) {
        j++;
      }

      if (needle[j] == '\0') {
        result = (char*)&haystack[i];
      }
    }
  }

  return result;
}

char* s21_strpbrk(const char* str1, const char* str2) {
  s21_size i, j;
  char* res = S21_NULL;

  if (str1 != S21_NULL && str2 != S21_NULL) {
    for (i = 0; str1[i] != '\0' && res == S21_NULL; i++) {
      for (j = 0; str2[j] != '\0' && res == S21_NULL; j++) {
        if (str1[i] == str2[j]) {
          res = (char*)&str1[i];
        }
      }
    }
  }

  return res;
}

void s21_itoa(int num, char* buf) {
  char tmp[20];
  int i = 0;
  int neg = 0;

  long n = num;

  if (n < 0) {
    neg = 1;
    n = -n;
  }

  do {
    tmp[i++] = (n % 10) + '0';
    n /= 10;
  } while (n > 0);

  if (neg) tmp[i++] = '-';

  int j = 0;
  while (i--) {
    buf[j++] = tmp[i];
  }
  buf[j] = '\0';
}

char* s21_strerror(int errnum) {
  static char unknown_res[100];
  char num_buf[20];

#if defined(__APPLE__) || defined(__MACH__)
  if (errnum >= 0 && errnum <= MAC_MAX_ERR) {
    return (char*)MAC_ERRORS[errnum];
  }
  s21_itoa(errnum, num_buf);

  int i = 0;
  while (MAC_ERR_PREFIX[i]) {
    unknown_res[i] = MAC_ERR_PREFIX[i];
    i++;
  }

#elif defined(__linux__)
  if (errnum >= 0 && errnum <= LINUX_MAX_ERR) {
    return (char*)LINUX_ERRORS[errnum];
  }
  s21_itoa(errnum, num_buf);

  int i = 0;
  while (LINUX_ERR_PREFIX[i]) {
    unknown_res[i] = LINUX_ERR_PREFIX[i];
    i++;
  }
#endif

  int j = 0;
  while (num_buf[j]) {
    unknown_res[i++] = num_buf[j++];
  }

  unknown_res[i] = '\0';

  return unknown_res;
}

char* s21_strrchr(const char* str, int c) {
  const char* last = NULL;

  while (*str != '\0') {
    if ((unsigned char)*str == (unsigned char)c) {
      last = str;
    }
    str++;
  }

  if (*str == (unsigned char)c) {
    last = str;
  }

  return (char*)last;
}

char* s21_strchr(const char* str, int c) {
  while (*str) {
    if ((unsigned char)*str == (unsigned char)c) {
      return (char*)str;
    }
    str++;
  }

  if ((unsigned char)c == '\0') {
    return (char*)str;
  }

  return NULL;
}

char* s21_strtok(char* str, const char* delim) {
  static char* saved;

  if (str != NULL) {
    saved = str;
  }

  if (saved == NULL) {
    return NULL;
  }

  while (*saved && s21_strchr(delim, *saved)) {
    saved++;
  }

  if (*saved == '\0') {
    saved = NULL;
    return NULL;
  }

  char* token_start = saved;

  while (*saved && !s21_strchr(delim, *saved)) {
    saved++;
  }

  if (*saved) {
    *saved = '\0';
    saved++;
  } else {
    saved = NULL;
  }

  return token_start;
}

static int s21_is_trim_char(char c, const char* trim_chars) {
  int res = 0;

  for (s21_size i = 0; trim_chars[i] != '\0' && !res; i++) {
    if (c == trim_chars[i]) {
      res = 1;
    }
  }

  return res;
}

void* s21_trim(const char* src, const char* trim_chars) {
  char* result = S21_NULL;

  if (src != S21_NULL) {
    if (trim_chars == S21_NULL || *trim_chars == '\0') {
      s21_size len = s21_strlen(src);
      result = (char*)malloc(len + 1);

      if (result != S21_NULL) {
        for (s21_size i = 0; i <= len; i++) {
          result[i] = src[i];
        }
      }
    } else {
      s21_size start = 0;
      s21_size end = s21_strlen(src);

      while (src[start] != '\0' && s21_is_trim_char(src[start], trim_chars)) {
        start++;
      }

      if (start == end) {
        result = (char*)malloc(1);
        if (result != S21_NULL) {
          result[0] = '\0';
        }
      } else {
        end--;

        while (end > start && s21_is_trim_char(src[end], trim_chars)) {
          end--;
        }

        s21_size new_len = end - start + 1;

        result = (char*)malloc(new_len + 1);

        if (result != S21_NULL) {
          for (s21_size i = 0; i < new_len; i++) {
            result[i] = src[start + i];
          }
          result[new_len] = '\0';
        }
      }
    }
  }

  return result;
}

void* s21_to_lower(const char* str) {
  char* ptr_str_low = NULL;
  if (str != NULL) {
    s21_size length = s21_strlen(str);
    ptr_str_low = (char*)malloc(sizeof(char) * (length + 1));
    if (ptr_str_low) {
      for (s21_size i = 0; i <= length; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          ptr_str_low[i] = str[i] + 32;
        } else {
          ptr_str_low[i] = str[i];
        }
      }
    }
  }
  return (void*)ptr_str_low;
}

void* s21_to_upper(const char* str) {
  if (!str) return NULL;
  const unsigned char* pt_str = (const unsigned char*)str;

  s21_size length = s21_strlen(str);

  char* str_upper = malloc(length + 1);
  if (!str_upper) return NULL;
  for (s21_size i = 0; i < length; ++i) {
    str_upper[i] = toupper(pt_str[i]);
  }
  str_upper[length] = '\0';

  return str_upper;
}

void* s21_insert(const char* src, const char* str, s21_size start_index) {
  if (!src || !str) return NULL;
  const unsigned char* ptr_src = (const unsigned char*)src;
  const unsigned char* ptr_str = (const unsigned char*)str;

  s21_size src_length = s21_strlen(src);
  s21_size str_length = s21_strlen(str);
  if (start_index > src_length) return NULL;

  s21_size full_length = src_length + str_length;
  char* ptr_insert = malloc(full_length + 1);
  if (!ptr_insert) return NULL;

  s21_size i = 0;
  for (; i < start_index; ++i) {
    ptr_insert[i] = ptr_src[i];
  }
  for (s21_size j = 0; j < str_length; ++i, ++j) {
    ptr_insert[i] = ptr_str[j];
  }
  for (s21_size k = start_index; k < src_length; ++i, ++k) {
    ptr_insert[i] = ptr_src[k];
  }
  ptr_insert[full_length] = '\0';

  return (void*)ptr_insert;
}