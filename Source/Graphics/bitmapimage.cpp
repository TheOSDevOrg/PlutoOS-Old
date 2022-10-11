#include <Graphics/bitmapimage.hpp>
#include <Core/Kernel.hpp>
#include <HAL/Drivers/Storage/UStar.hpp>
#include <Lib/String.hpp>
    uint32_t RGBToPackedValue(uint8_t r, uint8_t g, uint8_t b)
    {
        if (r == 0 && g == 0 && b == 0) { return 0xFF000000; }
        return (uint32_t)((0xFF << 24) | (r << 16) | (g << 8) | b);
    }
namespace GFX
{
    // constructor - blank
    Bitmap::Bitmap() { }

    // constructor - new bitmap
    Bitmap::Bitmap(int32_t w, int32_t h, COLOR_DEPTH depth)
    {
        if ((uint8_t)depth < 24) { printf("Only 24 and 32-bit bitmaps are supported"); return; }

        Width = w;
        Height = h;
        Depth = depth;
        ImageData = (uint8_t*)malloc(Width * Height * 4);
    }

    // constructor - load from disk
    Bitmap::Bitmap(char* fullname)
    {
        void* file = tar_fopen(fullname);    

        bmp_fileheader_t* h = (bmp_fileheader_t*)file;
        uint32_t offset = h->bfOffBits;

        bmp_infoheader_t* info = (bmp_infoheader_t*)(file + sizeof(bmp_fileheader_t));
        
        Width = info->biWidth;
        Height = info->biHeight;
        ImageData = (uint8_t*)((uint32_t)file + offset);
        Buffer = (uint8_t*)file;
        TotalSize = tar_ftell(fullname);
        Depth = (COLOR_DEPTH)info->biBitCount;
        if (Width == 0 || Height == 0 || Depth == 0 || TotalSize < 0) { printf("Unable to locate file"); return; }

        uint32_t* new_data = (uint32_t*)alloc(Width * Height * 4);
        for (int32_t yy = Height - 1; yy >= 0; yy--)
        {
            for (int32_t xx = 0; xx < Width; xx++)
            {
                if (Depth == COLOR_DEPTH_24)
                {
                    uint32_t offset = (3 * (xx + (yy * Width)));
                    uint32_t color = RGBToPackedValue(ImageData[offset + 2], ImageData[offset + 1], ImageData[offset]);
                    uint32_t output_offset = xx + ((Height - yy - 1) * Width);
                    new_data[output_offset] = color;
                }
                else if (Depth == COLOR_DEPTH_32)
                {
                    uint32_t offset = (4 * (xx + (yy * Width)));
                    uint32_t color = RGBToPackedValue(ImageData[offset + 2], ImageData[offset + 1], ImageData[offset]);
                    uint32_t output_offset = xx + ((Height - yy - 1) * Width);
                    new_data[output_offset] = color;
                }
            }
        }  
        ImageData = (uint8_t*)new_data;
    }

    Bitmap::~Bitmap()
    {
        free(Buffer);
    }
}