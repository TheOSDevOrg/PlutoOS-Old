#pragma once

#include <stdint.h>
#include <Common/Memory/heap.hpp>
#include <std/array.h>
#include <std/ptr.h>

namespace System::env
{
    extern System::core::memory::heap_t *current_heap;
    
    extern System::core::memory::heap_t common_heap;
    extern System::core::memory::heap_t lostfound_heap;
}