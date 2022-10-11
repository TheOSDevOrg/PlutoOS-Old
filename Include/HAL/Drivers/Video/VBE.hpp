#pragma once
#include <stdint.h>
#include <boot/mboot.h>
#include <Graphics/bitmapimage.hpp>
namespace System
{
    namespace HAL
    {
        namespace Drivers
        {
            namespace Video
            {
                class VBE
                {
                    public:
                        void Init();
                        void SetMode(int w, int h);
                        void FillHeaders();
                        void Clear();
                        void Clear(uint32_t color);
                        void Render();
                        void Disable();
                        void DrawChar(char c);
                        void DrawString(int x, int y, char* str);
                        void SetPixel(uint32_t x, uint32_t y, uint32_t color);
                        uint32_t GetWidth();
                        uint32_t GetHeight();
                        uint32_t GetSize();
                        uint32_t* GetBuffer();
                        void LoadFont();
                        void DrawBitmap(int32_t x, int32_t y, GFX::Bitmap* bitmap);
                    private:
                        
                        uint32_t Size;
                        uint32_t * Buffer;
                        uint32_t Width;
                        uint32_t Height;
                        uint32_t Pitch;
                        vbe_ctrl_info_t CtrlInfo;
                        vbe_mode_info_t ModeInfo;
                };
            }
        }
    }
}