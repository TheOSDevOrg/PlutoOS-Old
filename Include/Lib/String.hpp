#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <stdarg.h>
#include <Common/Memory/memory.hpp>
__cdecl void memcpyd(uint32_t *dst, uint32_t *src, size_t sz);
__cdecl void memsetd(uint32_t *dst, uint16_t data, size_t sz);

__cdecl void memcpyw(uint16_t *dst, uint16_t *src, size_t sz);
__cdecl void memsetw(uint16_t *dst, uint16_t data, size_t sz);

__cdecl void memcpyb(uint8_t  *dst, uint8_t  *src, size_t sz);
__cdecl void memsetb(uint8_t  *dst, uint8_t  data, size_t sz);

__cdecl void memcpy(void *dst, void *src, size_t sz);
__cdecl void memset(void *dst, int  data, size_t sz);
__cdecl int strcmp(const char* str1, const char* str2);
static inline int strnum(const char* str, int base)
{
	int ret = 0;
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			ret = ret * base + str[i] - '0';
		}
		else if (str[i] >= 'a' && str[i] <= 'z')
		{
			ret = ret * base + str[i] - 'a' + 10;
		}
		else if (str[i] >= 'A' && str[i] <= 'Z')
		{
			ret = ret * base + str[i] - 'A' + 10;
		}
		i++;
	}
	return ret;
}
__cdecl size_t strlen(const char* str);
__cdecl char* strcpy(char* dst, const char* src);
__cdecl char* strcat(char* dst, const char* src);
__cdecl int vsprintf(char* str, const char* format, va_list ap);
__cdecl uint32_t mem_get_total_mb();
static inline int stroct(const char* str)
{
	return strnum(str, 8);
}
// allocate overloads
extern void *operator new(size_t size);
extern void *operator new[](size_t size);

// delete overloads
extern void operator delete(void *p);
extern void operator delete(void *p, size_t size);
extern void operator delete[](void *p);
extern void operator delete[](void *p, size_t size);