/*
===============================================================================
 Name        : main.c
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
#include "vga_interrupt.h"

#define PCLKSEL0 (*(volatile unsigned int *) 0x400FC1A8)
#define ICER0 	 (*(volatile unsigned int *) 0xE000E180)

int sinetable[]={0,6,13,19,25,31,38,44,50,56,63,69,75,81,88,94,100,106,112,118,124,130,137,143,149,155,161,167,172,178,184,190,196,202,207,213,219,225,230,236,241,247,252,258,263,269,274,279,284,290,295,300,305,310,315,320,325,330,334,339,344,348,353,358,362,366,371,375,379,384,388,392,396,400,404,407,411,415,419,422,426,429,433,436,439,442,445,449,452,454,457,460,463,465,468,471,473,475,478,480,482,484,486,488,490,492,493,495,497,498,500,501,502,503,504,505,506,507,508,509,510,510,511,511,511,512,512,512,512,512,512,512,511,511,511,510,510,509,508,507,506,505,504,503,502,501,500,498,497,495,493,492,490,488,486,484,482,480,478,475,473,471,468,465,463,460,457,454,452,449,445,442,439,436,433,429,426,422,419,415,411,407,404,400,396,392,388,384,379,375,371,366,362,358,353,348,344,339,334,330,325,320,315,310,305,300,295,290,284,279,274,269,263,258,252,247,241,236,230,225,219,213,207,202,196,190,184,178,172,167,161,155,149,143,137,130,124,118,112,106,100,94,88,81,75,69,63,56,50,44,38,31,25,19,13,6,0,-6,-13,-19,-25,-31,-38,-44,-50,-56,-63,-69,-75,-81,-88,-94,-100,-106,-112,-118,-124,-130,-137,-143,-149,-155,-161,-167,-172,-178,-184,-190,-196,-202,-207,-213,-219,-225,-230,-236,-241,-247,-252,-258,-263,-269,-274,-279,-284,-290,-295,-300,-305,-310,-315,-320,-325,-330,-334,-339,-344,-348,-353,-358,-362,-366,-371,-375,-379,-384,-388,-392,-396,-400,-404,-407,-411,-415,-419,-422,-426,-429,-433,-436,-439,-442,-445,-449,-452,-454,-457,-460,-463,-465,-468,-471,-473,-475,-478,-480,-482,-484,-486,-488,-490,-492,-493,-495,-497,-498,-500,-501,-502,-503,-504,-505,-506,-507,-508,-509,-510,-510,-511,-511,-511,-512,-512,-512,-512,-512,-512,-512,-511,-511,-511,-510,-510,-509,-508,-507,-506,-505,-504,-503,-502,-501,-500,-498,-497,-495,-493,-492,-490,-488,-486,-484,-482,-480,-478,-475,-473,-471,-468,-465,-463,-460,-457,-454,-452,-449,-445,-442,-439,-436,-433,-429,-426,-422,-419,-415,-411,-407,-404,-400,-396,-392,-388,-384,-379,-375,-371,-366,-362,-358,-353,-348,-344,-339,-334,-330,-325,-320,-315,-310,-305,-300,-295,-290,-284,-279,-274,-269,-263,-258,-252,-247,-241,-236,-230,-225,-219,-213,-207,-202,-196,-190,-184,-178,-172,-167,-161,-155,-149,-143,-137,-130,-124,-118,-112,-106,-100,-94,-88,-81,-75,-69,-63,-56,-50,-44,-38,-31,-25,-19,-13,-6};



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

	ICER0 = (1<<1); // Disable Interrupts -> To be enabled later when initalizing interrupts

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

int horizontal_sync_cycles = 380 - 24;
int back_porch_cycles = 180 - 20;
int color_signal_cycles = 2517 - 8;
int front_porch_cycles = 94 - 16;
int write_cycle = 119 - 12; // 21 pixels per row

volatile int sync_state;
volatile int horizontal_lines;
volatile int random;
int red = 0;
int green = 0;
int blue = 0;

void HSyncInterruptInitialize(void)
{
	CLR_H;									// Clear everything
	T0MR0 = T0TC + horizontal_sync_cycles;  // interrupt after horizontal sync time
	T0IR = (1<<0); 							// clear old MR0 match events
	T0MCR |= (1<<0); 						// interrupt on MR0 match
	T0TCR = 1; 								// make sure timer enabled
	ISER0 = (1<<1); 						// enable Timer0 interrupts
}

//void PixelWriteInterruptInitialize(void)
//{
//	T0MR1 = T0TC + 32;			// Interrupt after 32 cycles (320ns)
//	T0IR = (1<<1);				// Clear old MR1 match events
//	T0MCR |= (1<<1);			// Interrupt on MR1 match
//}

void PixelWriteInterruptStop(void)
{
	CLR_R;
	CLR_G;
	CLR_B;
	T0MCR &= ~(1<<1); 			// disable MR1 event
	T0IR = (1<<1);   			// clear MR1 event
}

volatile int x = 0;
volatile int y = 0;
volatile int pixel;

volatile int vpos = 0;

void TIMER0_IRQHandler(void)
{

	// Case: End of Horizontal Sync
	if (sync_state == 0)
	{  // check for MR0 event
		T0MR0 = T0TC + back_porch_cycles; //back_porch_cycles; // next interrupt after back porch sync
		T0IR = (1<<0); // clear MR0 event
		SET_H; // Do whatever you need for this interrupt
	}
	// Case: End of Back Porch -> Where we write pixels
	else if (sync_state == 1)
	{
		T0MR0 = T0TC + write_cycle; // color_signal_cycles; //color_signal_cycles;
		T0IR = (1<<0); // clear MR0 event
		// DO whatever you want for this interrupt
		//(x%2 == 1) ? (SET_B) : (CLR_B);
		//x++;
	}
	// Case: Write a Pixel (After Back Porch, Before Front Porch)
	else if (sync_state < 13)
	{
		T0MR0 = T0TC + write_cycle;
		T0IR = (1<<0);
		vpos = (vpos + 1) % 480;
		if ((sync_state == 2) || (sync_state == 10))
		{
			if ((horizontal_lines > 100 + vpos) && (horizontal_lines < 300 + vpos))
			{
				SET_R;
				CLR_G;
				CLR_B;
			}
			else
			{
				CLR_R;
				CLR_G;
				CLR_B;
			}
		}
		else if (sync_state == 3 || sync_state == 11)
		{
			if ((horizontal_lines > 200) && (horizontal_lines < 275))
			{
				SET_R;
				SET_G;
				CLR_B;
			}
			else
			{
				CLR_R;
				CLR_G;
				CLR_B;
			}
		}
		else if (sync_state == 4 || sync_state == 12)
		{
			CLR_R;
			SET_G;
			CLR_B;

		}
		else if (sync_state == 5)
		{
			CLR_R;
			SET_G;
			SET_B;
		}
		else if (sync_state == 6)
		{
			CLR_R;
			SET_B;
			CLR_G;
		}
		else if (sync_state == 7)
		{
			if ((horizontal_lines > 300) && (horizontal_lines < 350))
			{
				SET_R;
				CLR_G;
				SET_B;
			}
			else
			{
				CLR_R;
				CLR_G;
				CLR_B;
			}
		}
		else if (sync_state == 8)
		{
			SET_R;
			SET_G;
			SET_B;
		}
		else
		{
			CLR_R;
			CLR_G;
			CLR_B;
		}
	}
	// Case: End of Writing Pixels
	else if (sync_state == 13)
	{
		T0MR0 = T0TC + front_porch_cycles; //front_porch_cycles; // next interrupt after front porch cycle
		T0IR = (1<<0); // clear MR0 event
		// DO whatever you want for this interrupt
		CLR_R;
		CLR_G;
		CLR_B;
	}
	// Case: End of Front Porch
	else if (sync_state == 14)
	{
		T0MR0 = T0TC + horizontal_sync_cycles;  //horizontal_sync_cycles;
		T0IR = (1<<0);
		CLR_H;
		horizontal_lines = (horizontal_lines + 1) % 525;
		if (horizontal_lines == 0)
		{
			CLR_V;
		}
		else if (horizontal_lines == 2)
		{
			SET_V;
		}
		else if (horizontal_lines == 33) //2+31)
		{
			// Video gets put out
		}
		else if (horizontal_lines == 513) // 33 + 480)
		{
			// Video stops getting put out
			CLR_R;
			CLR_G;
			CLR_B;
		}
	}
	sync_state = (sync_state + 1) % 15;
}


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

	int tempo = 90;
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
    int volume = 10; // percent, 100% is normal volume
    int noise_level = 0;
    int noise_out = 0;
    int noise_decay = 0;

	setup();
	init_clock();

	int start_line_write = 0;

	T0TCR |= (1<<0);
	PCLKSEL0 |= (1<<2);

	int line_time = 100000000/30000;

	int x_val = 0;
	int y_val = 0;
	//HSyncInterruptInitialize();

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


			if (red) {
				red = 0; green = 1; blue = 0;
			}
			else if (green) {
				red = 0; green = 0; blue = 1;
			}
			else if (blue) {
				red = 1; green = 0; blue = 0;
			}
			T0TCR ^= (1<<0);
		}
		last_start_sw = get_start_sw();

		// Check if sw 1 pushed
		if (last_sw1 == 0 && get_sw1() == 1) // rising edge
		{
			if (volume > 0) volume -= 2;
			if (volume < 0) volume = 0;
			noise_level = 0;
			//tempo > 5? tempo -= 5 : 0;
			//sixteenth_wait = 1000000*CLKRATIO/((tempo*4)/60);
		}
		last_sw1 = get_sw1();

		// Check if sw2 pushed
		if (last_sw2 == 0 && get_sw2() == 1) // rising edge
		{
			if (volume < 30) volume += 2;
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
			noise_level = 50;
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

		// Decay
		if (sixteenth_note_vout > 0 && T0TC - start_sixteenth_decay > sixteenth_wait/35)
		{
			start_sixteenth_decay = T0TC;
			sixteenth_note_vout--;
			if (channel_two_vout > 0) channel_two_vout--;
			half_note_adiv *= 1.001;
			noise_level > 0 ? noise_level -= 5 : 0;
		}

	}
}
