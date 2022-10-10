#pragma once
#include <boot/mboot.h>
#include <HAL/Drivers/Video/VBE.hpp>
#include <Debug/Serial.hpp>
#include <Common/term.hpp>
namespace System
{
    namespace Kernel
    {
        extern System::HAL::Drivers::Video::VBE Video;
        extern System::Debug::Serial Serial;
        extern System::Common::Terminal Terminal;
        void PrepareKernel(multiboot_info_t* mboot);
        void Loaded();
        void Run();
    }
}