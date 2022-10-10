#include <Debug/Serial.hpp>
#include <Lib/Ports.hpp>
#include <Lib/String.hpp>
namespace System
{
    namespace Debug
    {
        void Serial::Init()
        {
            outb(SERIAL_COMPORT + 1, 0x00);    // Disable all interrupts
            outb(SERIAL_COMPORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
            outb(SERIAL_COMPORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
            outb(SERIAL_COMPORT + 1, 0x00);    //                  (hi byte)
            outb(SERIAL_COMPORT + 3, 0x03);    // 8 bits, no parity, one stop bit
            outb(SERIAL_COMPORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
            outb(SERIAL_COMPORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
        }
        void Serial::Char(char c)
        {
            while (IsTransmitEmpty() == 0);
            outb(SERIAL_COMPORT, c);
        }
        void Serial::String(char* str)
        {
            for (int i = 0; str[i] != '\0'; i++)
                Char(str[i]);
        }
        void Serial::Dprintf(const char* format, ...)
        {
            va_list args;
            va_start(args, format);
            char buffer[1024];
            vsprintf(buffer, format, args);
            String(buffer);
            va_end(args);
        }
        bool Serial::IsTransmitEmpty()
        {
            return inb(SERIAL_COMPORT + 5) & 0x20;
        }
    }
}