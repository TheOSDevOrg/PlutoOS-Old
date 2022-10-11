#include <HAL/Drivers/Video/VBE.hpp>
#include <Lib/String.hpp>
#include <Common/realmode.hpp>
#include <Common/mult.hpp>
#include <Core/Kernel.hpp>
#include <stdint.h>
#include <Graphics/buffer.hpp>
#include <HAL/Drivers/Storage/UStar.hpp>
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <std/ssfn.hpp>
#define VBE_CTRL_PTR 0x80000
#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)
namespace System
{
    namespace HAL
    {
        namespace Drivers
        {
            namespace Video
            {
                using namespace System::Graphics;
                buffer_t buf;
                void VBE::Init()
                {
                    Buffer = (uint32_t*)System::Common::Get()->framebuffer_addr;
                    Width = System::Common::Get()->framebuffer_width;
                    Height = System::Common::Get()->framebuffer_height;
                    Pitch = System::Common::Get()->framebuffer_pitch;
                    Size = Width * Height * 4;
                    buf = System::Graphics::Init(Width, Height);
                }
                void VBE::LoadFont()
                {
                    ssfn_dst.ptr = (uint8_t*)buf.buffer;                  /* address of the linear frame buffer */
                    ssfn_dst.w = Width;                          /* width */
                    ssfn_dst.h = Height;                           /* height */
                    ssfn_dst.p = Pitch;                          /* bytes per line */
                    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
                    ssfn_src = (ssfn_font_t*)tar_fopen("./unifont.sfn");
                    ssfn_dst.x = 0;
                    ssfn_dst.y = 0;

                }
                void VBE::DrawChar(char c)
                {
                    if(c == '\n') {
                        ssfn_dst.x = 0;
                        ssfn_dst.y += 16;
                        return;
                    }
                    ssfn_putc(c);
                }
                    void VBE::DrawString(int x, int y, char* str)
                {
                    if (str == NULL) { return; }

                    int xx = x, yy = y, i = 0;
                    while (str[i] != 0)
                    {
                        if(str[i] == '\n')
                        {
                            ssfn_dst.x = xx;
                            ssfn_dst.y += 16;
                            i++;
                            continue;
                        }
                        ssfn_putc(str[i]);
                        i++;
                    }
                }
                void VBE::Clear()
                {
                    for(int i = 0; i < Size; i++)
                    {
                        Buffer[i] = 0x00000000;
                    }
                }
                void VBE::Clear(uint32_t color)
                {
                    for(int i = 0; i < buf.size; i++)
                    {
                        buf.buffer[i] = color;
                    }
                }
                void VBE::Render()
                {
                    memcpyd((uint32_t*)Buffer, buf.buffer, Size);
                }
                void VBE::Disable()
                {
                    registers16_t regs;
                    memset(&regs, 0, sizeof(registers16_t));
                    regs.ax = 0x4F02;
                    regs.bx = 0x03;
                    int32(0x10, &regs);
                }
                uint32_t VBE::GetWidth()
                {
                    return Width;
                }
                uint32_t VBE::GetHeight()
                {
                    return Height;
                }
                uint32_t VBE::GetSize()
                {
                    return Size;
                }
                uint32_t* VBE::GetBuffer()
                {
                    return Buffer;
                }
            }
        }
    }
}