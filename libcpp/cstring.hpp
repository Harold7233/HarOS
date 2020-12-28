//
// Created by Harold on 2020/4/10.
//

#ifndef OS_CPP_CSTRING_HPP
#define OS_CPP_CSTRING_HPP

#include <sys/types.hpp>

uint32_t str_to_u32int(const char *cp, char **endp, uint32_t base);

int32_t str_to_s32int(const char *cp, char **endp, uint32_t base);

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

int snprintf(char *buf, size_t size, const char *fmt, ...);

int vsprintf(char *buf, const char *fmt, va_list args);

int sprintf(char *buf, const char *fmt, ...);

char *strcpy(char *dest, const char *src);

int strcmp(const char *cs, const char *ct);

void *memcpy(void *dest, const void *src, size_t count);

void *memset(void *s, char c, size_t count);

size_t strlen(const char *);

uint32_t strnlen(const char *s, size_t count);


#endif //OS_CPP_CSTRING_HPP
