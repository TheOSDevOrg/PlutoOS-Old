#include <Core/Kernel.hpp>
#include <Common/mult.hpp>
#include <HAL/Drivers/Storage/UStar.hpp>
#include <Lib/String.hpp>
extern "C" {
    extern uint32_t kernel_end;
    extern uint32_t kernel_start;
}
namespace System
{
    namespace Kernel
    {
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
            
            Video.Init();
            Video.LoadFont();
            Terminal.Init();
            
        }
        void Loaded()
        {
            printf("Welcome To PlutoOS\n");
            printf("Kernel Start: 0x%x\n",&kernel_start);
            printf("Kernel End: 0x%x\n",&kernel_end);
            printf("Kernel Size: %d bytes\n",&kernel_end-&kernel_start);
            printf("Total Memory: %d MB",mem_get_total_mb());
            printf("Contents of Ramdisk:\n");
            tar_list_all();
        }
        void Run()
        {
        }
    }
}