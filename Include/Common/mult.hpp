#pragma once
#include <boot/mboot.h>
#include <stdint.h>
#include <stddef.h>
namespace System
{
    namespace Common
    {
        multiboot_info_t* Get();
        void Store(multiboot_info_t* mboot);
        void *GetModule(size_t indx);
    }
}