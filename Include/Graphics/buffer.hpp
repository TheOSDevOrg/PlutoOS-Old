#pragma once
#include <stdint.h>
namespace System
{
    namespace Graphics
    {
        struct buffer_t
        {
            uint32_t* buffer;
            uint32_t width;
            uint32_t height;
            uint32_t size;
        };
        buffer_t Init(uint32_t w, uint32_t h);
    }
}