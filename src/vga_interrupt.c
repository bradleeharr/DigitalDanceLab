#include "bradam_utils.h"
#include "vga_interrupt.h"

//back_porch_time = 1.89us = 189 clock cycles
//color_signal_time = 25.17us = 2517 clock cycles
//front_porch_time = 0.94us = 94 clock cycles

 // VGA CODE:
 // VGA – 640 x 480
 // if clock is 25 MHz - H Sync = 96 Clocks
 // if clock is 25 MHz - H back porch = 48 Clocks
 // if clock is 25 MHz - H front porch = 16 Clocks
 // Vertical Sync = 2 Horizontal Cycles
 // Vertical Back Porch = 33 Horizontal Cycles
 // Vertical Front Porch = 10 Horizontal Cycles


