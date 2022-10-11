/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022, All rights reserved.
*/
#include <std/decl.h>
#include <boot/loader.h>
#include <Core/Kernel.hpp>
#include <Core/env.hpp>

System::core::memory::heap_t *System::env::current_heap;
System::core::memory::heap_t System::env::common_heap;
System::core::memory::heap_t System::env::lostfound_heap;
extern "C"
{
    extern uint32_t kernel_end;
}
#define VBE_MODE
using namespace System::Kernel;
multiboot_header_loader multiboot section__(.multiboot) = {
    .magic = LOADER_MAGIC,
    .flags = {
        .align = true,
        .mmap = true,
        #ifdef VBE_MODE
        .video = true,
        #else
        .video = false,
        #endif
    },
    #ifdef VBE_MODE
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))),
    #else
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1))),
    #endif
    .reserved = {0},
    .vbe_info = {
        .video_mode = 0,
        .width = 800,
        .height = 600,
        .depth = 32
    }
};
memory_regions regions;

void *mboot;
__cdecl void __init()
{
    System::Kernel::heap.init((uintptr_t)regions.kernel_heap, (uintptr_t)regions.common_heap - (uintptr_t)regions.kernel_heap, true);
    System::env::common_heap.init((uintptr_t)regions.common_heap, (uintptr_t)regions.lf_heap-(uintptr_t)regions.common_heap, true);
    System::env::lostfound_heap.init((uintptr_t)regions.lf_heap, regions.end-(uintptr_t)regions.lf_heap, true);

    System::env::current_heap = &System::Kernel::heap;
    PrepareKernel((multiboot_info_t*)mboot);
    Loaded();
    for(;;) Run();
}

__cdecl void _start()
{
    asm volatile("cli");
    uintptr_t stack = (uintptr_t)regions.kernel_stack + 20*1024*1024;

    asm("movl %0, %%esp" : : "m"(stack));
    asm("movl %0, %%ebp" : : "m"(stack));
    
    regions.kernel_stack = (uint8_t*)kernel_end;
    regions.kernel_heap = regions.kernel_stack+2*1024*1024;
    regions.common_heap = regions.kernel_heap+40*1024*1024;
    regions.lf_heap = regions.common_heap+40*1024*1024;
    regions.end = (uintptr_t)regions.lf_heap+2*1024*1024;
    asm("mov %%ebx, %0" : "=m"(mboot));
    __init();
}