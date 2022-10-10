#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#define SERIAL_COMPORT 0x3F8
namespace System
{
    namespace Debug
    {
        class Serial
        {
            public:
                void Init();
                void Char(char c);
                void String(char* str);
                void Dprintf(const char* format, ...);
            private:
                bool IsTransmitEmpty();
        };
    }
}