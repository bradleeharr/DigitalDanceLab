/*
 * vga_interrupt.h
 *
 *  Created on: Nov 22, 2022
 *      Author: harr0263
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define PCONP (*(volatile unsigned int *) 0x400FC0C4)
#define T0MR0 (*(volatile unsigned int *) 0x40004018)
#define T0MCR (*(volatile unsigned int *) 0x40004014)
#define T0TCR (*(volatile unsigned int *) 0x40004004)
#define T0IR  (*(volatile unsigned int *) 0x40004000)
#define ISER0 (*(volatile unsigned int *) 0xE000E100)

#define SET_R 	(FIO2SET = (1<<11))
#define CLR_R 	(FIO2CLR = (1<<11))

#define SET_G 	(FIO2SET = (1<<8))
#define CLR_G 	(FIO2CLR = (1<<8))

#define SET_B 	(FIO2SET = (1<<6))
#define CLR_B 	(FIO2CLR = (1<<6))

#define SET_H 	(FIO2SET = (1<<4))
#define CLR_H 	(FIO2CLR = (1<<4))

#define SET_V 	(FIO2SET = (1<<1))
#define CLR_V 	(FIO2CLR = (1<<1))
void TIMER0_IRQHandler(void);

void TimerInterruptInitialize(void);

void TIMER0_IRQHandler(void);

#endif /* INTERRUPT_H_ */
