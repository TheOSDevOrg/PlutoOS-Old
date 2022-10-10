#include <Common/mult.hpp>
namespace System
{
    namespace Common
    {
        multiboot_info_t* mboot_ptr;
        multiboot_info_t* Get()
        {
            return mboot_ptr;
        }
        void Store(multiboot_info_t* mboot)
        {
            mboot_ptr = mboot;
        }
        void *GetModule(size_t indx)
        {
            if (indx > mboot_ptr->mods_count)
            {
                return nullptr;
            }
            uintptr_t *mods = (uintptr_t*)mboot_ptr->mods_addr;
            return (void*)mods[indx];
        }
    }
}