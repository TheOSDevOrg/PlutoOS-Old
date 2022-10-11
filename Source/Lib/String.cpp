#include <Lib/String.hpp>
#include <Lib/Ports.hpp>
__cdecl void memcpyd(uint32_t *dst, uint32_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetd(uint32_t *dst, uint16_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpyw(uint16_t *dst, uint16_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetw(uint16_t *dst, uint16_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpyb(uint8_t *dst, uint8_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetb(uint8_t *dst, uint8_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpy(void *dst, void *src, size_t sz)
{
    int i;
    int s,k;
    
    s = sz/4;
    sz -= (k  = s*4);
    memcpyd((uint32_t*)dst, (uint32_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    s = sz/2;
    sz -= (k = s*2);
    memcpyw((uint16_t*)dst, (uint16_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    memcpyb((uint8_t*)dst, (uint8_t*)src, sz);
}
__cdecl void memset(void *dst, int data, size_t sz)
{
    if (data < INT8_MAX) memsetb((uint8_t*)dst, (uint8_t)data, sz);
    else if (data < INT16_MAX) memsetw((uint16_t*)dst, (uint16_t)data, sz);
    else memsetd((uint32_t*)dst, (uint32_t)data, sz);
}
__cdecl int strcmp(const char* str1, const char* str2)
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i]) return str1[i] - str2[i];
		i++;
	}
	return str1[i] - str2[i];
}
__cdecl size_t strlen(const char* str)
{
    size_t i = 0;
    while (str[i] != '\0') i++;
    return i;
}
__cdecl char* strcpy(char* dst, const char* src)
{
    size_t i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return dst;
}
__cdecl char* strcat(char* dst, const char* src)
{
    size_t i = 0;
    while (dst[i] != '\0') i++;
    strcpy(dst + i, src);
    return dst;
}
__cdecl char* strchr(const char* str, int c)
{
    size_t i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c) return (char*)(str + i);
        i++;
    }
    return NULL;
}
__cdecl char* itoa(int value, char* str, int base)
{
    char* rc;
    char* ptr;
    char* low;
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if (value < 0 && base == 10) *ptr++ = '-';
    low = ptr;
    do
    {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    *ptr-- = '\0';
    while (low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}
__cdecl int vsprintf(char* str, const char* format, va_list args)
{
 //lets use itoa for vsprintf
    char* s;
    int c;
    int i;
    int len = 0;
    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
                case 'c':
                    c = va_arg(args, int);
                    *str++ = c;
                    len++;
                    break;
                case 's':
                    s = va_arg(args, char*);
                    while (*s != '\0')
                    {
                        *str++ = *s++;
                        len++;
                    }
                    break;
                case 'd':
                    i = va_arg(args, int);
                    itoa(i, str, 10);
                    len += strlen(str);
                    str += strlen(str);
                    break;
                case 'x':
                    i = va_arg(args, int);
                    itoa(i, str, 16);
                    len += strlen(str);
                    str += strlen(str);
                    break;
                case '%':
                    *str++ = '%';
                    len++;
                    break;
            }
        }
        else
        {
            *str++ = *format;
            len++;
        }
        format++;
    }
    *str = '\0';
    return len;
}
__cdecl uint32_t mem_get_total_mb()
{
    // borrowed kindly from nik300
    register uint64_t* mem;
    uint64_t mem_count, a;
    uint32_t memkb;
    uint8_t	irq1, irq2;

    /* save IRQ's */
    irq1 = inb(0x21);
    irq2 = inb(0xA1);

    /* kill all irq's */
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    mem_count = 0;
    memkb = 0;

    __asm__ __volatile__("wbinvd");

    do 
    {
        memkb++;
        mem_count += 1024 * 1024;
        mem = (uint64_t*)mem_count;

        a = *mem;
        *mem = 0x55AA55AA;

        asm("":::"memory");
        if (*mem != 0x55AA55AA) { mem_count = 0; }
        else 
        {
            *mem = 0xAA55AA55;
            asm("":::"memory");
            if (*mem != 0xAA55AA55)
                mem_count = 0;
        }

        asm("":::"memory");
        *mem = a;

    } while (memkb < 4096 && mem_count != 0);

    mem = (uint64_t*)0x413;

    outb(0x21, irq1);
    outb(0xA1, irq2);

    return memkb;
}
void *operator new(size_t size) { return malloc(size); }
void *operator new[](size_t size) { return malloc(size); }
void operator delete(void *p) { free(p); }
void operator delete(void *p, size_t size) { free(p); }
void operator delete[](void *p) { free(p); }
void operator delete[](void *p, size_t size) { free(p); }