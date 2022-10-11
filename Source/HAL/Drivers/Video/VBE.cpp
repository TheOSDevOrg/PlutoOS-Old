#include <HAL/Drivers/Video/VBE.hpp>
#include <Lib/String.hpp>
#include <Common/realmode.hpp>
#include <Common/mult.hpp>
#include <boot/mboot.h>
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
                    buf = System::Graphics::Init(Width, Height);
                }
                    void VBE::SetMode(int w, int h)
    {
        FillHeaders();
        registers16_t regs;
        memset(&regs, 0, sizeof(registers16_t));
        
        vbe_mode_info_t* minfo = (vbe_mode_info_t*)(VBE_CTRL_PTR + sizeof(vbe_ctrl_info_t) + 512);
        uint16_t* modes = (uint16_t*)REAL_PTR(CtrlInfo.videomode);
        uint16_t mode;

        for (int i = 0; modes[i] != 0xFFFF; i++)
        {
            regs.ax = 0x4F01;
            regs.cx = modes[i];
            regs.es = SEG(minfo);
            regs.di = OFF(minfo);
            int32(0x10, &regs);

            if (minfo->width == w && minfo->height == h && minfo->depth == 32)
            {
                mode = modes[i];
                regs.ax = 0x4F02;
                regs.bx = mode | 0x4000;
                int32(0x10, &regs);
                ModeInfo = *minfo;
                Size     = ModeInfo.width * ModeInfo.height * 4;
                 System::Kernel::Serial.Dprintf("VBE: %d x %d x %d", ModeInfo.width, ModeInfo.height, ModeInfo.depth);
                    Buffer = (uint32_t*)ModeInfo.physical_base;
                    Width = ModeInfo.width;
                    Height = ModeInfo.height;
                    Pitch = ModeInfo.pitch;
                    Size = Width * Height * 4;
                return;
            }
        }
    }
    void VBE::FillHeaders()
    {
        memset(&ModeInfo, 0, sizeof(vbe_mode_info_t));
        memset(&CtrlInfo, 0, sizeof(vbe_ctrl_info_t));
        vbe_ctrl_info_t* info = (vbe_ctrl_info_t*)VBE_CTRL_PTR;
        registers16_t regs;

        memset(&regs, 0, sizeof(registers16_t));
        regs.ax = 0x4F00;
        regs.es = 0x8000;
        regs.di = 0x0000;
        int32(0x10, &regs);

        if (regs.ax != 0x4F) {  return; }
        CtrlInfo = *info;
        ModeInfo = *(vbe_mode_info_t*)System::Common::Get()->vbe_mode_info;
        Size     = ModeInfo.width * ModeInfo.height * 4;
       
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
                void VBE::SetPixel(uint32_t x, uint32_t y, uint32_t color)
                {
                    if(x >= Width || y >= Height)
                        return;
                   //lets set the pixel with pitch
                   buf.buffer[y * (Pitch / 4) + x] = color;
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
                    ssfn_dst.x = 0;
                    ssfn_dst.y = 0;
                    for(int i = 0; i < Size; i++)
                    {
                        Buffer[i] = 0x00000000;
                    }
                }
                void VBE::Clear(uint32_t color)
                {
                    ssfn_dst.x = 0;
                    ssfn_dst.y = 0;
                    for(int i = 0; i < buf.size; i++)
                    {
                        buf.buffer[i] = color;
                    }
                }
                void VBE::Render()
                {
                   for(int i = 0; i < Size; i++)
                   {
                       Buffer[i] = buf.buffer[i];
                   }
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
                void VBE::DrawBitmap(int32_t x, int32_t y, GFX::Bitmap* bitmap)
                {
                    if (bitmap == nullptr) { return; }

                    int32_t w = bitmap->Width;
                    int32_t h = bitmap->Height;

                    uint32_t* data = (uint32_t*)bitmap->ImageData;
                    for (int32_t yy = 0; yy < h; yy++)
                    {
                        for (int32_t xx = 0; xx < w; xx++)
                        {
                            uint32_t color = data[(xx + (yy * w))];
                            SetPixel(x + xx, y + yy, color);
                        }
                    }
                }
            }
        }
    }
}