#pragma once
#include <boot/mboot.h>
#include <HAL/Drivers/Video/VBE.hpp>
#include <Debug/Serial.hpp>
#include <Common/term.hpp>
#include <Common/Memory/heap.hpp>
#include <HAL/Drivers/Input/Keyboard.hpp>
namespace System
{
    namespace Kernel
    {
        extern System::HAL::Drivers::Video::VBE Video;
        extern System::Debug::Serial Serial;
        extern System::Common::Terminal Terminal;
        extern System::core::memory::heap_t heap;
        extern System::HAL::Drivers::Input::Keyboard Keyboard;
        void PrepareKernel(multiboot_info_t* mboot);
        void Loaded();
        void Run();
    }
}