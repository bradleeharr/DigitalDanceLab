/*
===============================================================================
 Name        : Lab5.c
 Author      : Bradlee Harrison & Adam Tran
 Version     :
 Copyright   : $(copyright)
 Description :
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "bradam_utils.h"
#include "clock_config.h"

#define PCLKSEL (*(volatile unsigned int *) 0x400FC1A8)

int sinetable[]={0,6,13,19,25,31,38,44,50,56,63,69,75,81,88,94,100,106,112,118,124,130,137,143,149,155,161,167,172,178,184,190,196,202,207,213,219,225,230,236,241,247,252,258,263,269,274,279,284,290,295,300,305,310,315,320,325,330,334,339,344,348,353,358,362,366,371,375,379,384,388,392,396,400,404,407,411,415,419,422,426,429,433,436,439,442,445,449,452,454,457,460,463,465,468,471,473,475,478,480,482,484,486,488,490,492,493,495,497,498,500,501,502,503,504,505,506,507,508,509,510,510,511,511,511,512,512,512,512,512,512,512,511,511,511,510,510,509,508,507,506,505,504,503,502,501,500,498,497,495,493,492,490,488,486,484,482,480,478,475,473,471,468,465,463,460,457,454,452,449,445,442,439,436,433,429,426,422,419,415,411,407,404,400,396,392,388,384,379,375,371,366,362,358,353,348,344,339,334,330,325,320,315,310,305,300,295,290,284,279,274,269,263,258,252,247,241,236,230,225,219,213,207,202,196,190,184,178,172,167,161,155,149,143,137,130,124,118,112,106,100,94,88,81,75,69,63,56,50,44,38,31,25,19,13,6,0,-6,-13,-19,-25,-31,-38,-44,-50,-56,-63,-69,-75,-81,-88,-94,-100,-106,-112,-118,-124,-130,-137,-143,-149,-155,-161,-167,-172,-178,-184,-190,-196,-202,-207,-213,-219,-225,-230,-236,-241,-247,-252,-258,-263,-269,-274,-279,-284,-290,-295,-300,-305,-310,-315,-320,-325,-330,-334,-339,-344,-348,-353,-358,-362,-366,-371,-375,-379,-384,-388,-392,-396,-400,-404,-407,-411,-415,-419,-422,-426,-429,-433,-436,-439,-442,-445,-449,-452,-454,-457,-460,-463,-465,-468,-471,-473,-475,-478,-480,-482,-484,-486,-488,-490,-492,-493,-495,-497,-498,-500,-501,-502,-503,-504,-505,-506,-507,-508,-509,-510,-510,-511,-511,-511,-512,-512,-512,-512,-512,-512,-512,-511,-511,-511,-510,-510,-509,-508,-507,-506,-505,-504,-503,-502,-501,-500,-498,-497,-495,-493,-492,-490,-488,-486,-484,-482,-480,-478,-475,-473,-471,-468,-465,-463,-460,-457,-454,-452,-449,-445,-442,-439,-436,-433,-429,-426,-422,-419,-415,-411,-407,-404,-400,-396,-392,-388,-384,-379,-375,-371,-366,-362,-358,-353,-348,-344,-339,-334,-330,-325,-320,-315,-310,-305,-300,-295,-290,-284,-279,-274,-269,-263,-258,-252,-247,-241,-236,-230,-225,-219,-213,-207,-202,-196,-190,-184,-178,-172,-167,-161,-155,-149,-143,-137,-130,-124,-118,-112,-106,-100,-94,-88,-81,-75,-69,-63,-56,-50,-44,-38,-31,-25,-19,-13,-6};

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

void setup() {
	// Pin 18 - P0.26 - Bits 21:20 - Function When 10 = AOUT
	PINSEL1 |=  (1<<21);
	PINSEL1 &= ~(1<<20);
	// Pin 20 - P1.31 - Bits 31:30
	PINMODE3 |=  (1<<30) | (1<<31);
	FIO1DIR &= ~(1<<31); 					// Set as Input
	// Pin 21 - P0.2 - Bits 5:4
	PINMODE0 |= (1<<5) | (1<<4);
	FIO0DIR &= ~(1<<2);						// Set as Input
	// Pin 22 - P0.3 - Bits 7:6
	PINMODE0 |= (1<<7) | (1<<6);
	FIO0DIR &= ~(1<<3);						// Set as Input
	T0TCR &= ~(1<<0); 						// Initial timer = Off;


	// VGA Output
	// Synchronization Range:
		// Horizontal: 31.0 - 60.0 KHz (USE 31 KHz)
		// Vertical: 56.2 - 75.0 Hz (Loop toggle at 56.2 Hz)
		// 52: Red , 50: Green, 48: Blue, 46: Horiz Sync, 43: Vert Sync
	// Suggested Resolution: 640 x 480@60.0 Hz - 75.0 Hz
	FIO2DIR |= (1<<11);		// 52: Red
	FIO2DIR |= (1<<8);		// 50: Green
	FIO2DIR |= (1<<6);		// 48: Blue
	FIO2DIR |= (1<<4);	    // 46: Horiz Sync
	FIO2DIR |= (1<<1);		// 43: Vert Sync

}

void vertical_sync()
{
	CLR_V;
	wait_us(1);
	SET_V;
}

void horizontal_sync()
{
	CLR_H;
	SET_H;
}

void vga()
{
	vertical_sync();
	for (int j = 0; j < 480*3; j++)
	{
		for (int i = 0; i < 640/5; i++)
		{
				SET_R;
				SET_G;
		}
		horizontal_sync();
	}
}

int get_start_sw() { return ((FIO1PIN >> 31) & 1); }
int get_sw1() { return ((FIO0PIN >> 2) & 1); }
int get_sw2() { return ((FIO0PIN >> 3) & 1); }

//int sixteenth_notes_square1[] = {0, 1, -1, 2, -2, 3, 4, -4, 5, -5, 6, -6, 7, 8, -8, 9, -9, 10, 11, -11, 12, -12, 13, -13, 14, 15, -15, 16, -16, 17, 18, -18, 19, -19, 20, -20, 21, 22};
//int sixteenth_notes_square[] =
//{D4, G3, D4, G3, D4, G3, D4, G3, A3, A3,
//E4, A4, E4,  A3, E4, A3, G3,  G3, D4, G3,
//D4,  G3, D4, G3, A3,  A3, E4, A4, E4,  A3,
//E4, A3, G3,G3, D4, G3, D4, G3, D4, G3,
//A3, A3, E4, A4, E4,  A3, E4, A3, G3,  G3,
//D4, G3, D4,  G3, D4, G3, A3,  A3, E4, A4, E4,
//A3, E4, A3, G3};
//int numbeats = 65;

int half_notes_square[] = {G3,G3,G3,G3,G3,G3,G3,G3};

//int numbeats = 29;
//int sixteenth_notes_square[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};

int numbeats = 400;
int channel1_note[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
								100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
								100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
								100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
								100, 100, 100, 100, 100, 100,  G5,  G5,  F5,  F5,  G5,  G5, Bb5, Bb5,  G5,  G5,
								G5,   G5,  G5,  G5,  G5,  G5,  F5,  F5,  G5,  G5,  G5,  G5,  G5,  G5, Bb5, Bb5,
								Bb5, Bb5, Bb5, Bb5, Bb5, Bb5, G5, G5, F5, F5, G5,  G5, Bb5, Bb5,  G5,  G5,  G5,
								G5,  G5,   G5,  G5,  G5,  F5, F5, G5, G5, G5, G5, G5, G5, Bb5, Bb5, Bb5, Bb5,
								Bb5, Bb5, Bb5, Bb5, G5, G5, F5, F5, G5, G5, F5, F5, G5,
								G5, G4, G4, C5, C5, Eb5, Eb5, C5, C5, Eb5, Eb5, Eb5, Eb5, C5, C5, C5, C5, C5, C5, C5, C5, C5, C5, G5, G5, F5, F5, G5,
								G5,F5,F5,G5,G5,G5,G5,G5,G5,C5,C5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5, 100,100,100,100,100,100,100,100, G5,G5,F5,F5,G5,G5,
								Bb5,Bb5,G5,G5,G5,G5,G5,G5,G5,G5,F5,F5,G5,G5,G5,G5,G5,G5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,G5,G5,F5,F5,G5,G5,Bb5,Bb5,G5,
								G5,G5,G5,G5,G5,G5,G5,F5,F5,G5,G5,G5,G5,G5,G5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,Bb5,G5,G5,F5,F5,G5,G5,F5,F5,G5,G5,F5,F5,G5,
								G5,G4,G4,C5,C5,Eb5,Eb5,C5,C5,F5,F5,F5,F5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,G5,G5,F5,F5,G5,G5,F5,F5,G5,G5,G5,
								Bb5,Bb5,Bb5,F5,F5,Eb5,Eb5,F5,F5,G5,G5,C5,C5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,100,100,Bb4,Bb4,D5,D5,F5,F5,
								Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,D5,D5,Eb5,Eb5,Eb5,Eb5,Eb5,Eb5,Bb4,Bb4};
//int channel1_notealt[] = {100,100,100,100,  100,100,100,100, 100,100,100,100,   100,100,100,100,
//						  100,100,100,100,  100,100,100,100, 100,100,100,100,   100,100,100,100,
//						  100,100,100, G5,  F5,  G5,Bb5,G5,   G5, G5,100, F5,    G5, G5, G5,Bb5,
//						  Bb5,Bb5,100, G5,  F5,  G5,Bb5,G5,   G5, G5,100, F5,    G5, G5, G5,Bb5,
//						  Bb5,Bb5,100, G5,  F5,  G5, F5 };

int channel2_note[] = {   Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  C2, G2,  C3, G2,   C2, G2, C3, G2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  C2, G2,  C3, G2,   C2, G2, C3, G2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2, G2, D2, G3, D2,    G2, D2, G3, D2,
						  Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2, G2, D2, G3, D2,    G2, D2, G3, D2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2, Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2, Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2,
						  G2, D2, G3, D2,    G2, D2, G3, D2,  G2, D2, G3, D2,    G2, D2, G3, D2,
						  C2, G2, C3, G2,    C2, G2, C3, G2,  C2, G2, C3, G2,    C2, G2, C3, G2,
						  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,  A2,Eb2,  A3,Eb2,   A2,Eb2, A3,Eb2,
						  Eb2,Bb2,Eb3,Bb2,   Eb2,Bb2,Eb3,Bb2};

// After, start ascending arpeggios
// TODO: Finish ADSR envelope
// TODO: Fix sine wave frequency problems
// TODO: Add Square/Sawtooth waves
// TODO: Fix interrupts to stop high frequency from breaking
// TODO: Setup VGA output
// TODO: Setup Serial Dance Pad Input
int main(void)
{
	int max_vout_1 = 0x02F;
	int max_vout_2 = 0x05F;
	int last_start_sw = 0;
	int last_sw1 = 0;
	int last_sw2 = 0;

	int tempo = 140;
	int sixteenth_wait = 1000000*CLKRATIO/((tempo*4)/60);
	int note = 1;
	int half_note = 1;
	int channel_two_note = 0;
	int sixteenth_beat = 0;
	int channel2_beat = 0;


	int sixteenth_note_vout = 0;
	int channel_one_vout = 0;
	int channel_two_vout = 0;
	int bass_note_vout = 0;

	int half_note_vout = 0;
	float half_note_adiv = 1.0;

	int start_sixteenth = 0;
	int start_eighth = 0;
	int start_quarter = 0;
	int start_half = 0;
	int start_whole = 0;

	int start_sixteenth_write = 0;
	int start_half_freq = 0;
	int start_ch2_write = 0;
	int start_channel2 = 0;

	int start_sixteenth_decay = 0;
	int high_pulse_ch1 = 0;
	int high_pulse_ch2 = 0;
	int sixteenth_timeout = 0;
	int ch2_timeout = 0;
    int sinechange = 0;

    int sineindex = 0;
    int lfsr = 0xACE1;

    int volume = 25; // percent, 100% is normal volume
    int noise_level = 0;
    int noise_out = 0;
    int noise_decay = 0;
	setup();

	init_clock();

	T0TCR |= (1<<0); 						// Initial timer = On;
	int start_hs = T0TC;
	int start_vs = T0TC;
	//int hs_time = (96000000/30000/5);
	//int vs_time = (96000000/60/5);
	int hii = 1;


	//horizontal_sync_pulse_time = 3.77 us = 377 clock cycles
	int hs_time = 377;
	int back_porch_time = 189;
	int color_signal_time = 2517;
	int front_porch_time = 94;

	int start_write = 0;
	int horizontal_lines = 0;
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
	while (1)
	{
		// 377 Cycles H low Data low
		if (T0TC - start_write < hs_time)
		{
			CLR_H;
		}
		// 189 Cycles H high, data low
		else if (T0TC - start_write < back_porch_time) 
		{
			SET_H;
		}
		// 2517 Cycles, H high, data high
		else if (T0TC - start_write < color_signal_time) 
		{	
			SET_R;
			CLR_R;
			SET_G;
			CLR_G;
			SET_B;
			CLR_B;
		}
		// 94 Cycles, H high, data low
		else if (T0TC - start_write < front_porch_time)
		{
			CLR_R;
			CLR_G;
			CLR_B;
		}
		// Reset
		else
		{
			start_write = T0TC;
			horizontal_lines++;
		}

		if (horizontal_lines - start_vs < 11);
		{
			CLR_V;
		}
		else if (horizontal_lines - start_vs < )
	}
	while (1)
	{
		// Assign note values to channels
		sixteenth_timeout = get_time_count(note);
		ch2_timeout = get_time_count(channel_two_note);
        sinechange = get_sine_change(half_note);

		// Check If Start Button Pushed
		if (last_start_sw == 0 && get_start_sw() == 1) // rising edge
		{
			T0TCR ^= (1<<0);   // start timer ;  XOR flips bits
			sixteenth_beat = 0; // Resets the song
		}
		last_start_sw = get_start_sw();

		// Check if sw 1 pushed
		if (last_sw1 == 0 && get_sw1() == 1) // rising edge
		{
			if (volume > 0) volume -= 2;
			noise_level = 0;
			//tempo > 5? tempo -= 5 : 0;
			//sixteenth_wait = 1000000*CLKRATIO/((tempo*4)/60);
		}
		last_sw1 = get_sw1();

		// Check if sw2 pushed
		if (last_sw2 == 0 && get_sw2() == 1) // rising edge
		{
			if (volume < 100) volume += 2;
			//tempo < 200? tempo += 5 : 0;
			//sixteenth_wait = 1000000*CLKRATIO/((tempo*4)/60);
		}
		last_sw2 = get_sw2();

		// Loop controls switching notes
		if (sixteenth_beat < numbeats)
		{
			// Every 1/16 beats, the sixteenth note switches
			if (T0TC-start_sixteenth > sixteenth_wait)
			{
			start_sixteenth = T0TC;
			note=(channel1_note[sixteenth_beat]);
			sixteenth_note_vout = max_vout_1;
			sixteenth_beat++;
			}
			// Every 2 sixteenths, the eighth note bass switches
			if (T0TC - start_eighth> sixteenth_wait*2)
			{
				start_eighth = T0TC;
				channel_two_note = channel2_note[sixteenth_beat/2];
				channel_two_vout = max_vout_2;
			}
			// Every 4 sixteenths, the quarter note percussion switches
			if (T0TC - start_quarter > sixteenth_wait*4 && sixteenth_beat > 80)
			{
			start_quarter = T0TC;
			noise_level = 100;
			}
			// Every 16 sixteenths, the half note switches value
			if (T0TC - start_half > sixteenth_wait*16)
			{
			start_half = T0TC;
			half_note = (half_notes_square[sixteenth_beat]);
			half_note_adiv = 3;
			}
			// Every 32 sixteenths, the whole note switches value
			if (T0TC - start_whole > sixteenth_wait*32)
			{

			}

		}
		else sixteenth_beat = 0;


        // Every tick of sine wave, need to go to next value in sine table for half note
      /*  if (T0TC - start_half_freq > sinechange) {
        sineindex++;
        start_half_freq = 0;
        half_note_vout = (sinetable[(sineindex) % 512]+512)/half_note_adiv;
		half_note_vout > 1023 ? half_note_vout = 1023 : 0 ;
        }
       	*/
		// change state of sixteenth square wave to keep sixteenth note freq
		if (T0TC - start_sixteenth_write > sixteenth_timeout)
		{
			start_sixteenth_write = T0TC;
			high_pulse_ch1 = !high_pulse_ch1;
		}
		if (T0TC - start_ch2_write > ch2_timeout)
		{
			start_ch2_write = T0TC;
			high_pulse_ch2 = !high_pulse_ch2;
		}

		if (high_pulse_ch1) { channel_one_vout = sixteenth_note_vout; }
		else { channel_one_vout = 0; }

		if (high_pulse_ch2) { bass_note_vout = channel_two_vout; }
		else { bass_note_vout = 0; }

		if (noise_level != 0)
		{
			lfsr ^= lfsr >> 7;
			lfsr ^= lfsr << 9;
			lfsr ^= lfsr >> 13;
			noise_out = (lfsr & 7)*noise_level ;
		}


		// Output Music Logic
		int total_out = channel_one_vout + bass_note_vout + noise_out; // Melody, Bass, Percussion
		total_out = total_out * volume/100;
		total_out > 1023? total_out = 1023 : 0;
		DACR = ((total_out) << 6) | (1<<16);


		// Attack
		/*if (sixteenth_note_vout > 0 && T0TC - start_sixteenth_attack > sixteenth_wait/100 &&
		{
			start_sixteenth_attack = T0TC;
		}*/
		// Decay
		if (sixteenth_note_vout > 0 && T0TC - start_sixteenth_decay > sixteenth_wait/35)
		{
			start_sixteenth_decay = T0TC;
			sixteenth_note_vout--;
			if (channel_two_vout > 0) channel_two_vout--;
			half_note_adiv *= 1.001;
			noise_level > 0 ? noise_level -= 5 : 0;
		}

		// Sustain
		/* if (sixteenth_note_vout > 0 && T0TC start_sixteenth_sustain > sixteenth_wait/100)
		{
			start_sixteenth_sustain = T0TC;
		} */
		// Release
		/* if (sixteenth_note_vout > 0 && T0TC start_sixteenth_release > sixteenth_wait/100)
		{
			start_sixteenth_release = T0TC;
		} */


	}
}
