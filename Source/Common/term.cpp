#include <Common/term.hpp>
#include <Core/Kernel.hpp>
#include <Lib/String.hpp>
#include <std/ssfn.hpp>
namespace System
{
    namespace Common
    {
        void Terminal::Init()
        {
            CursorX = 0;
            CursorY = 0;
            Buffer = System::Kernel::Video.GetBuffer();
        }
        void Terminal::String(char* str)
        {
            int i = 0;
            while (str[i] != 0)
            {
                System::Kernel::Video.DrawChar(str[i]);
                //update CursorX and CursorY
                CursorX += 8;
                if (CursorX >= System::Kernel::Video.GetWidth())
                {
                    CursorX = 0;
                    CursorY += 16;
                }
                i++;
            }
        }
        void Terminal::TPrintf(const char* format, ...)
        {
            char buffer[1024];
            va_list args;
            va_start(args, format);
            vsprintf(buffer, format, args);
            va_end(args);
            String(buffer);
        }
    }
}
extern "C" {
    void printf(const char* format, ...)
    {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        System::Kernel::Terminal.String(buffer);
    }
}