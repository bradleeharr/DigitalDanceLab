/*
 * bradam_utils.c
 *
 *  Created on: Nov 11, 2022
 *      Author: harr0263
 */
#include "bradam_utils.h"

void wait_us(int us)
{
  int start = T0TC;  // note starting time
  T0TCR |= (1<<0);   // start timer
  while ((T0TC-start)<us) {} // wait for time to pass
 }

void wait(float sec)
{
  wait_us(sec*1000000); // convert seconds to microseconds
}

int get_time_count(int note)
{	int timeout = 0;
	switch(note)
	{
        case 100: timeout = 1000000000; break;

        case   1: timeout = 3822.19164*4; break; // C1
        case   2: timeout = 3405.29864*4; break; // D2
        case  -2: timeout = 3213.36760925*4; break; // DS2 ; Eb2
        case   3: timeout = 3033.7965*4; break; // E2
        case   4: timeout = 2863.52443*4; break; // F2
        case   5: timeout = 2551.02041*4; break; // G2
        case  -5: timeout = 2407.31824747*4; break; // GS2 ; Ab2
        case   6: timeout = 2272.72727*4; break; // A2
        case  -6: timeout = 2145.002145*4; break; // AS2 ; Bb2
        case   7: timeout = 2024.78335*2; break; // B2

		case   8: timeout = 3822.19164*2; break; // C2
		case  -8: timeout = 3607.76391*2; break; // CS2 ; Db2
		case   9: timeout = 3405.29864*2; break; // D2
		case  -9: timeout = 3213.36760925*2; break; // DS2 ; Eb2
		case   10: timeout = 3033.7965*2; break; // E2
		case   11: timeout = 2863.52443*2; break; // F2
		case  -11: timeout = 2702.7027027*2; break; // FS2 ; Gb2
		case   12: timeout = 2551.02041*2; break; // G2
		case  -12: timeout = 2407.31824747*2; break; // GS2 ; Ab2
		case   13: timeout = 2272.72727*2; break; // A2
		case  -13: timeout = 2145.002145*2; break; // AS2 ; Bb2
		case   14: timeout = 2024.78335*2; break; // B2


		case   15: timeout = 3822.19164; break; // C4
		case  -15: timeout = 3607.5036075; break; // CS4 ; Db4
		case   16: timeout = 3405.29864; break; // D4
		case  -16: timeout = 3213.36760925; break; // DS4 ; Eb4
		case  17: timeout = 3033.70446; break; // E4
		case  18: timeout = 2863.44243; break; // F3
		case -18: timeout = 2702.7027027; break; // FS3 ; Gb3
		case  19: timeout = 2551.02041; break; // G3
		case -19: timeout = 2407.31824747; break; // GS3 ; Ab3
		case  20: timeout = 2272.72727; break; // A3
		case -20: timeout = 2145.002145; break; // AS3 ; Bb3
		case  21: timeout = 2024.78335; break; // B3

		case  22: timeout = 3822.19164/2; break; // C4
		case -22: timeout = 3607.5036075/2; break; // CS4 ; Db4
		case  23: timeout = 3405.29864/2; break; // D4
		case -23: timeout = 3213.36760925/2; break; // DS4 ; Eb4
		case  24: timeout = 3033.70446/2; break; // E4
		case  25: timeout = 2863.44243/2; break; // F4
		case -25: timeout = 2702.7027027/2; break; // FS4 ; Gb4
		case  26: timeout = 2551.02041/2; break; // G4
		case -26: timeout = 2407.31824747/2; break; // GS4 ; Ab4
		case  27: timeout = 2272.72727/2; break; // A4
		case -27: timeout = 2145.002145/2; break; // AS4 ; Bb4
		case  28: timeout = 2024.78335/2; break; // B4

		case  29: timeout = 3822.19164/4; break; // C5
		case -29: timeout = 3607.5036075/4; break; // CS5 ; Db5
		case  30: timeout = 3405.29864/4; break; // D5
		case -30: timeout = 3213.36760925/4; break; // DS5 ; Eb5
		case  31: timeout = 3033.70446/4; break; // E5
		case  32: timeout = 2863.44243/4; break; // F5
		case -32: timeout = 2702.7027027/4; break; // FS5 ; Gb5
		case  33: timeout = 2551.02041/4; break; // G5
		case -33: timeout = 2407.31824747/4; break; // GS5 ; Ab5
		case  34: timeout = 2272.72727/4; break; // A5
		case -34: timeout = 2145.002145/4; break; // AS5 ; Bb5
		case  35: timeout = 2024.78335/4; break; // B5

		case 36: timeout = 3822.19164/8; break; // C6
		}
	return timeout*CLKRATIO;
}


/*
    Returns a timeout value for when the sine wave should go to the next sample of the sine
    based on frequency. There are 512 samples in one period, so for a 1 Hz wave, the timeout
    should be 1/512 seconds. For a 440 Hz wave, the timeout should be 1/(512*440) seconds
    which is 4.43 microseconds... now this is not good
*/
int get_sine_change(int note)
{
    return 4.43892045;

}
