#pragma once
#include <stdint.h>
namespace System
{
    namespace HAL
    {
        namespace Drivers
        {
            namespace Input
            {
                class Keyboard
                {
                    public:
                        void Initialize();
                        void SetLayout(uint8_t layout_lower[],uint8_t layout_upper[]);
                        void HandleKey(int32_t keycode);   
                    private:
                        uint8_t* curr_layout_upper;
                        uint8_t* curr_layout_lower; 
                        uint8_t shift = 0;
                        uint8_t caps = 0;
                };
            }
        }
    }
}
extern "C"
{
    void keyboard_interrupt_handler();
}