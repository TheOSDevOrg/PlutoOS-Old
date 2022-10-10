#pragma once
#include <stdint.h>
#include <std/decl.h>
    typedef struct
    {
        uint16_t di, si, bp, sp, bx, dx, cx, ax;
        uint16_t gs, fs, es, ds, eflags;
    } packed__ registers16_t;
    __cdecl { extern void int32(uint8_t interrupt, registers16_t* regs); }