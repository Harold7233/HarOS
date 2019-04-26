#ifndef STRING_H
#define STRING_H
#include <common/common.h>
#include "stdarg.h"


u32int_t str_to_u32int(const char *cp, char **endp, u32int_t base);
s32int_t str_to_s32int(const char *cp, char **endp, u32int_t base);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char *buf, size_t size, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
char * strcpy(char * dest,const char *src);
int strcmp(const char * cs,const char * ct);
void * memcpy(void * dest,const void *src,size_t count);
void * memset(void * s,char c,size_t count);
u32int_t strnlen(const char *s, size_t count);
#endif