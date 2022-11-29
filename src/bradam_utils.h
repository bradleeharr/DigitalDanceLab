/*
 * bradam_utils.h
 *
 *  Created on: Nov 11, 2022
 *      Author: harr0263
 */

#ifndef BRADAM_UTILS_H_
#define BRADAM_UTILS_H_

#define DAC_BIAS (1<<16)

//#define beat 	0
#define C2 		  1
#define D2 		  2
#define Eb2      -2
#define E2		  3
#define F2		  4
#define G2		  5
#define Ab2      -5
#define A2		  6
#define Bb2      -6
#define B2		  7

#define C3 		  8
#define CS3		 -8
#define Db3 	 -8
#define	D3 		  9
#define	DS3 	 -9
#define Eb3		 -9
#define	E3 		  10
#define	F3 		  11
#define	FS3 	 -11
#define Gb3		 -11
#define G3 		  12
#define	GS3 	 -12
#define Ab3		 -12
#define A3 		  13
#define	AS3 	 -13
#define Bb3		 -13
#define	B3 		  14

#define	C4 		  15
#define	CS4 	 -15
#define Db4 	 -15
#define	D4 		  16
#define	DS4 	 -16
#define Eb4		 -16
#define	E4 		 17
#define	F4 		 18
#define	FS4 	-18
#define Gb4		-18
#define	G4 		 19
#define	GS4 	-19
#define Ab4		-19
#define	A4 		 20
#define	AS4 	-20
#define Bb4		-20
#define	B4 		 21

#define	C5 		 22
#define	CS5 	-22
#define Db5 	-22
#define	D5 		 23
#define	DS5 	-23
#define Eb5		-23
#define	E5 		 24
#define	F5 		 25
#define	FS5 	-25
#define Gb5		-25
#define	G5 		 26
#define	GS5 	-26
#define Ab5		-26
#define	A5 		 27
#define AS5 	-27
#define Bb5		-27
#define B5 		 28

#define C6 		 29

// Logic of waiting
#define CLKRATIO 100

#define PINSEL1   	(*(volatile unsigned int *) 0x4002C004)
#define PINMODE3   	(*(volatile unsigned int *) 0x4002C04C)
#define PINMODE0    (*(volatile unsigned int *) 0x4002C040)
#define FIO1DIR  	(*(volatile unsigned int *) 0x2009c020)
#define FIO1PIN  	(*(volatile unsigned int *) 0x2009c034)
#define FIO0DIR     (*(volatile unsigned int *) 0x2009C000)
#define FIO0PIN     (*(volatile unsigned int *) 0x2009C014)


#define DACR  	    (*(volatile unsigned int *) 0x4008C000)
#define DACCTRL  	(*(volatile unsigned int *) 0x4008C004)
#define DACCNTVAL	(*(volatile unsigned int *) 0x4008C008)

#define T0TC  		(*(volatile unsigned int *) 0x40004008)
#define T0TCR 		(*(volatile unsigned int *) 0x40004004)
#define PCLKSEL0 	(*(volatile unsigned int *) 0x400FC1A8)

#define T1TC	    (*(volatile unsigned int *) 0x40008008)
#define T1TCR 	    (*(volatile unsigned int *) 0x40008004)

// 52: Red , 50: Green, 48: Blue, 46: Horiz Sync, 44: Vert Sync
#define FIO2DIR 	(*(volatile unsigned int *) 0x2009C040)
#define FIO2SET 	(*(volatile unsigned int *) 0x2009C058)
#define FIO2CLR 	(*(volatile unsigned int *) 0x2009C05C)
#define PINMODE4	(*(volatile unsigned int *) 0x4002C050)

#define DACTIMEOUT ((DACCTRL & 1) == 1)

void wait(float sec);

void wait_us(int us);

int get_time_count(int note);

int get_sine_change(int note);

#endif /* BRADAM_UTILS_H_ */
