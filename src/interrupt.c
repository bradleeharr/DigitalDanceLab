#include "bradam_utils.h"
#define PCONP (*(volatile unsigned int *) 0x400FC0C4)
#define T0MR0 (*(volatile unsigned int *) 0x40004018)
#define T0MCR (*(volatile unsigned int *) 0x40004014) 
#define T0TCR (*(volatile unsigned int *) 0x40004004)
#define T0IR  (*(volatile unsigned int *) 0x40004000)
#define ISER0 (*(volatile unsigned int *) 0xE000E100)

void TIMER0_IRQHandler(void);

// Interrupt 1 ms from now (like mbed Ticker or Timeout)
// Assuming default 1 MHz timer PCLK (1 ms = 1000 PCLKs)
void TimerInterruptInitialize(void)
{
 T0MR0 = T0TC + 1000; // 1st interrupt 1000 clocks from now
 T0IR = (1<<0); // clear old MR0 match events
 T0MCR |= (1<<0); // interrupt on MR0 match
 T0TCR = 1; // make sure timer enabled
 ISER0 = (1<<1); // enable Timer0 interrupts
}
// Periodic 1 ms interrupt (like mbed Ticker)
void TIMER0_IRQHandler(void)
{
 // Only need to check timer’s IR if using multiple
 // interrupt conditions with the same timer
 if ((T0IR>>0) & 1) 
 {  // check for MR0 event
    T0MR0 = T0MR0 + 1000; // next intr. 1000 clocks from last
    T0IR = (1<<0); // clear MR0 event
    // Do whatever you need for this interrupt
 }
}

int main(void) 
{
  // (1) Timer 0 configuration (see page 490 of user manual)
  PCONP |= 1 << 1; // Power up Timer 0 (see page 63 of user manual)
  PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock (page 56 user manual)

  // MR0 is "Match Register 0". MR0 can be enabled through the MCR to reset
  // the Timer/Counter (TC), stop both the TC and PC, and/or generate an interrupt
  // every time MR0 matches the TC. (see page 492 and 496 of user manual)
  T0MR0 = 500; //Toggle Time in mS
  //LPC_TIM0->MR0 = 1 << 23; // Give a value suitable for the LED blinking

  // frequency based on the clock frequency
  // MCR is "Match Control Register". The MCR is used to control if an
  // interrupt is generated and if the TC is reset when a Match occurs.
  // (see page 492 and 496 of user manual)
  T0MCR |= 1 << 0; // Interrupt on Match 0 compare
  T0MCR |= 1 << 1; // Reset timer on Match 0
  // TCR is "Timer Control Register". The TCR is used to control the Timer
  // Counter functions. The Timer Counter can be disabled or reset
  // through the TCR. (see page 492 and 494 of user manual)
  T0TCR |= 1 << 1; // Manually Reset Timer 0 (forced);
  T0TCR &= ~(1 << 1); // Stop resetting the timer
  // (2) Enable timer interrupt;
  // TIMER0_IRQn is 1, see lpc17xx.h and page 73 of user manual
  NVIC_EnableIRQ(TIMER0_IRQn); // see core_cm3.h header file
  // (3) Some more one-time set-up's;
  T0TCR |= 1 << 0; // Start timer (see page 492 and 494 of user manual)
  PCONP |= ( 1 << 15 ); // Power up GPIO (see lab1)
  FIO1DIR |= 1 << 29; // Put P1.29 into output mode. LED is connected to P1.29
  // (4) infinite loop;
  while (1) // Why do we need this?
  {
    FIO1PIN ^= 1 << 29; // Toggle the LED (see lab1)
  }
  return 0;
}


// Here, we describe what should be done when the interrupt on Timer 0 is handled;
// We do that by writing this function, whose address is “recorded” in the vector table
// from file startup_LPC17xx.s under the name TIMER0_IRQHandler;
void TIMER0_IRQHandler(void)
{
  // IR is "Interrupt Register". The IR can be written to clear interrupts. The IR
  // can be read to identify which of eight possible interrupt sources are
  // pending. (see page 492 and 493 of user manual)
  if ( (T0IR & 0x01) == 0x01 ) // if MR0 interrupt (this is a sanity check);
  {
    T0IR |= 1 << 0; // Clear MR0 interrupt flag (see page 492 and 493 of user manual)
    FIO1PIN ^= 1 << 29; // Toggle the LED (see lab1)
  }
}