#include <Core/Kernel.hpp>
#include <Common/mult.hpp>
#include <HAL/Drivers/Storage/UStar.hpp>
#include <Lib/String.hpp>
#include <Graphics/colors.hpp>
#include <HAL/Interrupts/idt.hpp>
#include <HAL/Interrupts/gdt.hpp>
#include <Graphics/bitmapimage.hpp>
extern "C" {
    extern uint32_t kernel_end;
    extern uint32_t kernel_start;
}
namespace System
{
    namespace Kernel
    {
        using namespace System::core::memory;
        using namespace System::hal::interrupts;
        heap_t heap;
        System::HAL::Drivers::Video::VBE Video;
        System::Debug::Serial Serial;
        System::Common::Terminal Terminal;
        uint8_t* ramdisk;
        size_t rd_size;
        using namespace System::Common;
        void PrepareKernel(multiboot_info_t* mboot)
        {
            Store(mboot);
            Serial.Init();
            ramdisk = (uint8_t*)GetModule(0);
            rd_size = tar_calc_size();
            ramdisk_init(ramdisk,rd_size);
            GFX::Bitmap* bmp = new GFX::Bitmap("./bg.bmp");
            Video.Init();
            Video.Clear();
            Video.LoadFont();
            
            Video.DrawBitmap(20,20,bmp);
            Terminal.Init();
            gdt::manager_t::load_gdt();
            idt::manager_t::init();
            
        }
        void Loaded()
        {
            printf("Welcome To PlutoOS\n");
            printf("Kernel Start: 0x%x\n",&kernel_start);
            printf("Kernel End: 0x%x\n",&kernel_end);
            printf("Kernel Size: %d bytes\n",&kernel_end-&kernel_start);
            printf("Total Memory: %d MB\n",mem_get_total_mb());
            idt::manager_t::enable_interrupts();
            heap.print_table();
           // asm("int $0x0");
        }
        void Run()
        {
            Video.Render();
        }
    }
}