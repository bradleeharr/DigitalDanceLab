#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define PINMODE3  (*(volatile unsigned int *) 0x4002C04C)
#define PINSEL3   (*(volatile unsigned int *) 0x4002C00C)
#define PLL0CON   (*(volatile unsigned int *) 0x400FC080)
#define PLL0CFG   (*(volatile unsigned int *) 0x400FC084)
#define PLL0STAT  (*(volatile unsigned int *) 0x400FC088)
#define PLL0FEED  (*(volatile unsigned int *) 0x400FC08C)
#define CLKOUTCFG (*(volatile unsigned int *) 0x400FC1C8) // Clock Enable Synchronizer
#define CLKSRCSEL (*(volatile unsigned int *) 0x400FC10C) // Clock Source Selection
#define CCLKCFG   (*(volatile unsigned int *) 0x400FC104) // Clock Divider

void init_clock();
