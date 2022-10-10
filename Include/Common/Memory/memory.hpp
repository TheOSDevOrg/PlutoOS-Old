#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
__cdecl void * kalloc  (size_t sz);
__cdecl bool   kfree   (void *data);
__cdecl void * krealloc(void *data, size_t sz);

__cdecl void * malloc  (size_t sz);
__cdecl bool   mfree   (void *data);
__cdecl void * mrealloc(void *data, size_t sz);

__cdecl void * alloc  (size_t sz);
__cdecl bool   free   (void *data);
__cdecl bool   freelst(void **lst);
__cdecl void * realloc(void *data, size_t sz);