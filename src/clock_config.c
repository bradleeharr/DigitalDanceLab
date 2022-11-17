#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "clock_config.h"
void init_clock() {

	PINSEL3 &= ~(1<<23);
	PINSEL3 |= (1<<22);                 // Select CLKOUT on P1.27
	CLKOUTCFG |= (0010<<0);
	CLKOUTCFG |= (1<<8);				// Enable CLKOUT

/**
 *  PLL0 Setup Sequence
 */
	PLL0CON &= ~(1<<1); 				// Step 1: Disconnect PLL0 with one feed sequence
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	PLL0CON &= ~(1<<0); 				// Step 2: Disable PLL0 with one feed sequence
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	CCLKCFG = 0; 						// Step 3: Change CPU clock divider setting

	CLKSRCSEL = 0; 						// Step 4: Write to Clock Source Selection Control register to change clock source

	PLL0CFG = 119; 		                // Step 5: Write to PLL0CFG and make it effective with one feed sequence ; PLL0 must be disabled
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	PLL0CON |= (1<<0);					// Step 6: Enable PLL0 with one feed sequence
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	while ((PLL0STAT & (26<<1)) == 0) 	// Step 8:  Wait for PLL0 to achieve lock
		{

	}

	CCLKCFG = 9;						// Step 7: Change the CPU Clock Divider setting for the operation with PLL0

	PLL0CON |= (1<<1);					// Step 9: Connect PLL0 with one feed sequence
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
}

