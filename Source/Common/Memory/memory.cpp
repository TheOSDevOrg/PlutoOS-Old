#include <Common/Memory/memory.hpp>
#include <Core/Kernel.hpp>
#include <std/ptr.h>
#include <Core/env.hpp>
using namespace System::core::memory;
__cdecl void * kalloc(size_t sz)
{
    return System::Kernel::heap.unsafe_alloc(sz);
}
__cdecl bool kfree(void *data)
{
    return System::Kernel::heap.free(data);
}
__cdecl void * krealloc(void *data, size_t sz)
{
    if (!System::Kernel::heap.contains((uintptr_t)data))
    {
        //dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = System::Kernel::heap.unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy(d, data, off);
    System::Kernel::heap.free(data);

    return d;
}

__cdecl void * malloc(size_t sz)
{
    return System::env::common_heap.unsafe_alloc(sz);
}
__cdecl bool mfree(void *data)
{
    return System::env::common_heap.free(data);
}
__cdecl void * mrealloc(void *data, size_t sz)
{
   if (!System::env::common_heap.contains((uintptr_t)data))
    {
       // dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = System::env::common_heap.unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy(d, data, off);
    System::env::common_heap.free(data);

    return d;
}

__cdecl void * alloc(size_t sz)
{
    System::env::current_heap->defragment();
    return System::env::current_heap->unsafe_alloc(sz);
}
__cdecl bool free(void *data)
{
    bool r = System::env::current_heap->free(data);
    System::env::current_heap->defragment();
    return r;
}
__cdecl bool freelst(void **lst)
{
    return false;
    if (!System::env::current_heap->contains((uintptr_t)lst)) return false;
    size_t sz = ((data_t*)((uintptr_t)lst-sizeof(data_t)))->size/sizeof(void*);
    for (int i = 0; i < sz; i++) if (System::env::current_heap->contains((uintptr_t)lst[i])) System::env::current_heap->free(lst[i]);
    return System::env::current_heap->free(lst);
}
__cdecl void * realloc(void *data, size_t sz)
{
    if (!System::env::current_heap->contains((uintptr_t)data))
    {
      //  dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = System::env::current_heap->unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy((void *)d, (void *)data, off);
    System::env::current_heap->free(data);

    return d;
}