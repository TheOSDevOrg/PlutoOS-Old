#pragma once
#include <stdint.h>
#include <std/decl.h>
namespace System
{
    namespace Ports
    {
        class Port
        {
            protected:
                uint16_t port;
                Port(uint16_t port);
                ~Port();
        };
        class Port8Bit : public Port
        {
            public:
                Port8Bit(uint16_t port);
                ~Port8Bit();
                virtual uint8_t Read();
                virtual void Write(uint8_t data);
        };
        class Port8BitSlow : public Port8Bit
        {
            public:
                Port8BitSlow(uint16_t port);
                ~Port8BitSlow();
                virtual void Write(uint8_t data);
        };
        class Port16Bit : public Port
        {
            public:
                Port16Bit(uint16_t port);
                ~Port16Bit();
                virtual uint16_t Read();
                virtual void Write(uint16_t data);
        };
        class Port32Bit : public Port
        {
            public:
                Port32Bit(uint16_t port);
                ~Port32Bit();
                virtual uint32_t Read();
                virtual void Write(uint32_t data);
        };
    }
}
__cdecl
{
    void outb(uint16_t port, uint8_t data);
    void outw(uint16_t port, uint16_t data);
    void outl(uint16_t port, uint32_t data);
    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);
    uint32_t inl(uint16_t port);

}