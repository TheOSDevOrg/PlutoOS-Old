#include <HAL/Drivers/Input/Keyboard.hpp>
#include <Lib/Ports.hpp>
#include <HAL/Interrupts/idt.hpp>
#include <Core/Kernel.hpp>
#define ESCAPE 1
#define SHIFT 42
#define SHIFT_RELEASE -86
#define CAPSLOCK 58
#define BACKSPACE 14
#define PAGEUP 73
#define PAGEDOWN 81
namespace System
{
    namespace HAL
    {
        namespace Drivers
        {
            namespace Input
            {
                void Keyboard::Initialize()
                {
                    System::hal::interrupts::idt::manager_t::register_irq(1,(System::hal::interrupts::idt::callback_t)keyboard_interrupt_handler);
                }
                void Keyboard::SetLayout(uint8_t layout_lower[],uint8_t layout_upper[])
                {
                    curr_layout_lower = layout_lower;
                    curr_layout_upper = layout_upper;
                }
                void Keyboard::HandleKey(int32_t keycode)
                {
                    switch(keycode) {
                        case SHIFT:
                        shift = 1;
                        break;
                        case SHIFT_RELEASE:
                        shift = 0;
                        break;
                        case CAPSLOCK:
                        if(caps > 0){
                            caps = 0;
                        }else {
                            caps = 1;
                        }
                        break;
                        default:
                        if(keycode < 0) return;
                        char ch;
                        if(shift || caps) {
                            ch = curr_layout_upper[keycode];
                        }else {
                            ch = curr_layout_lower[keycode];
                        }
                        //pass to term
                        System::Kernel::Serial.Char(ch);
                    }
                }
            }
        }
    }
}
extern "C"
{
    void keyboard_interrupt_handler()
     {
        uint8_t status;
	    signed char keycode;
	    outb(0x20, 0x20);

	    status = inb(0x64);
	    if(status & 1){
		keycode = inb(0x60);
        System::Kernel::Keyboard.HandleKey(keycode);
        #ifdef KEYBOARD_PRINTCODE
        printf("%s\n", keycode);
        #endif
	}
}
}