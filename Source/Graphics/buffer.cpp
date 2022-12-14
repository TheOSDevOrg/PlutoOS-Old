#include <Graphics/buffer.hpp>
#include <Lib/String.hpp>
#include <Core/Kernel.hpp>
namespace System
{
    namespace Graphics
    {
        buffer_t Init(uint32_t w, uint32_t h)
        {
            buffer_t buffer;
            buffer.width = w;
            buffer.height = h;
            buffer.size = w * h * 4;
            buffer.buffer = (uint32_t*)kalloc(w*h*4);
            return buffer;
        }
    }
}