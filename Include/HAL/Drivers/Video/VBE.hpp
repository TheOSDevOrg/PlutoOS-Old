#pragma once
#include <stdint.h>
#include <boot/mboot.h>
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
                        void Disable();
                        void DrawChar(char c);
                        void DrawString(int x, int y, char* str);
                        uint32_t GetWidth();
                        uint32_t GetHeight();
                        uint32_t GetSize();
                        uint32_t* GetBuffer();
                        void LoadFont();
                    private:
                        void FillHeaders();
                        uint32_t Size;
                        uint32_t * Buffer;
                        uint32_t Width;
                        uint32_t Height;
                        uint32_t Pitch;
                };
            }
        }
    }
}