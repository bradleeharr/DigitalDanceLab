# DDL
Final Project for ECE 4723 - Digital Design Lab

We created an Arcade-style Dance Rhythm Game using a [PlayStation 2 Dance Pad](https://www.amazon.com/Dance-Revolution-Regular-Playstation2-Playstation-2/dp/B000GF7KAW) for player input, a [LPC1769 MBed](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-arm-cortex-m3/512-kb-flash-64-kb-sram-ethernet-usb-lqfp100-package:LPC1769FBD100) for game logic, and using a [DE-10Lite FPGA](https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=1021) to output VGA visuals.  There are two songs playable. Each song is stored as an array of notes in memory. Using input from the dance pad, we allow a player to adjust the tempo and volume of the output music as well as detect the different steps input for scoring for the game. 

# Audio
Two songs are possible.
There are several "instruments." we used square waves with varying sustain, delay, attack, and range to allow for different sound envelopes. We also made a percussive sound using a simple linear feedback shift register. 

There are two channels of audio, a bass line and a melody line.
Each channel is represented as an array of notes, as such:
```c
int channel2_note[] = {
  Eb2,Bb2,Eb3,Bb2,  Eb2,Bb2,Eb3,Bb2,  G2, D2, G3, D2,    G2, D2, G3, D2,
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
};
```

Notes, like `Eb2` and `Bb`, are defined using C #define statements as the specific frequency that each note corresponds to, like in <a href=https://upload.wikimedia.org/wikipedia/commons/a/ad/Piano_key_frequencies.png>this</a> table. 

# VGA
A foundational part of this project was learning how to use the VGA standard. While initially seemingly complex, it is a fundamentally simple protocol. There are two lines of data that are used for syncrhonization: a "Vertical Sync" and a "Horizontal" Sync. The horizontal sync triggers once **for every (horizontal) line**. Then, after the horizontal sync has triggered (the height of the screen, in pixels) times, the vertical sync should trigger once **for every frame**. This means for each frame, the vertical sync will trigger once and exactly once.

<p align="center"> (Left) VGA Horizontal Sync -- (Middle) VGA Color Data Line -- (Right) VGA Vertical Sync </p> 

<p align="center">
  
  <img src="https://github.com/bradleeharr/DigitalDanceLab/assets/56418392/1f0d91a1-6eb1-4725-b649-fd7549d10e63" style="max-width:220px; width: 220px; max-height: 250px; height: 250px;">
  <img src="https://github.com/bradleeharr/DigitalDanceLab/assets/56418392/b15303d7-8b6a-40c3-94e5-78663d6ddfee" style="max-width:220px; width: 220px; max-height: 250px; height: 250px;">
  <img src="https://github.com/bradleeharr/DigitalDanceLab/assets/56418392/f3e2fb80-be7e-4808-aeba-ae65b039f824" style="max-width:220px; width: 220px; max-height: 250px; height: 250px;">

</p>

The VGA pinout should be as follows:

<p align="center">
  <img src="https://github.com/user-attachments/assets/93d0059a-2a8b-4721-b3f8-ac733c64ccbe" style="max-width:500px; width:500px; max-height: 250px; height: 250px;">
</p>

# FPGA
The De10-Lite FPGA is used functionally as a block of memory. The microprocessor makes actions to write to this block of memory, and the FPGA buffers this output into a suitable VGA format. 


# Display
Below is an example of the display being used. It is fairly rough, but for the limited time we worked on this project, we were happy enough to be able to get the VGA display to work successfully.
<p align="center"> 
  (Left) Test Mode Displaying Random Rectangles -- (Right) Game Mode Displaying Scrolling Rectangles, Indicating when to Step on Dance Pad
</p>
<p align="center"> 
<img src="https://github.com/bradleeharr/DigitalDanceLab/assets/56418392/c6a0c211-e574-452a-8595-58e780e7f5a2" style="max-wdith:400px; width:400px; max-height:300px; height:300px;">
  <img src="https://github.com/bradleeharr/DigitalDanceLab/assets/56418392/656dd980-e8d2-4a89-b372-f794f37b6263" style="max-wdith:400px; width:400px; max-height:300px; height:300px;">

</p>
<p align="center"> 
</p>
<p align="center"> 
</p>

