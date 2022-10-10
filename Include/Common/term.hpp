#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
namespace System
{
    namespace Common
    {
        class Terminal
        {
            public:
                void Init();
                void Char(char c);
                void String(char* str);
                void TPrintf(const char* format, ...);
            private:
                uint32_t CursorX;
                uint32_t CursorY;
                uint32_t Color;
                uint32_t* Buffer;
                uint32_t Width;
                uint32_t Height;

        };
    }
}
extern "C" {
    void printf(const char* format, ...);
}