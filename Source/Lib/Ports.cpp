#include <Lib/Ports.hpp>
namespace System
{
    namespace Ports
    {
        Port::Port(uint16_t port)
        {
            this->port = port;
        }
        Port::~Port()
        {
        }
        Port8Bit::Port8Bit(uint16_t port) : Port(port)
        {
        }
        Port8Bit::~Port8Bit()
        {
        }
        void Port8Bit::Write(uint8_t data)
        {
            __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
        }
        uint8_t Port8Bit::Read()
        {
            uint8_t result;
            __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
            return result;
        }
        Port8BitSlow::Port8BitSlow(uint16_t port) : Port8Bit(port)
        {
            
        }
        Port8BitSlow::~Port8BitSlow()
        {
        }
        void Port8BitSlow::Write(uint8_t data)
        {
            __asm__ volatile("outb %0, %1 \n jmp 1f \n 1: jmp 1f \n 1:" : : "a" (data), "Nd" (port));
        }
        Port16Bit::Port16Bit(uint16_t port) : Port(port)
        {
            
        }
        Port16Bit::~Port16Bit()
        {
        }
        void Port16Bit::Write(uint16_t data)
        {
            __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
        }
        uint16_t Port16Bit::Read()
        {
            uint16_t result;
            __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
            return result;
        }
        Port32Bit::Port32Bit(uint16_t port) : Port(port)
        {
            
        }
        Port32Bit::~Port32Bit()
        {
        }
        void Port32Bit::Write(uint32_t data)
        {
            __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port));
        }
        uint32_t Port32Bit::Read()
        {
            uint32_t result;
            __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
            return result;
        }

    }
}
__cdecl {
    void outb(uint16_t port, uint8_t data)
    {
        __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
    }
    void outw(uint16_t port, uint16_t data)
    {
        __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
    }
    void outl(uint16_t port, uint32_t data)
    {
        __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port));
    }
    uint8_t inb(uint16_t port)
    {
        uint8_t result;
        __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
        return result;
    }
    uint16_t inw(uint16_t port)
    {
        uint16_t result;
        __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
        return result;
    }
    uint32_t inl(uint16_t port)
    {
        uint32_t result;
        __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
        return result;
    }
}