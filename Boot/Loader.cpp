/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022, All rights reserved.
*/
#include <std/decl.h>
#include <boot/loader.h>
#include <Core/Kernel.hpp>
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
        PrepareKernel((multiboot_info_t*)mboot);
    Loaded();
    for(;;) Run();
}

__cdecl void _start()
{
    asm("mov %%ebx, %0" : "=m"(mboot));
    __init();
}