/* ECE319K_Lab5main.c
 * Digital Piano using a Digital to Analog Converter
 * December 28, 2023
 * 5-bit binary-weighted DAC connected to PB4-PB0
 * 4-bit keyboard connected to PB19-PB16
 * JERIN ANDREWS & WILLIAM WANG
*/

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include "../inc/DAC5.h"  // student's Lab 5
#include "../inc/Key.h"   // student's Lab 5
#include "wave.h"


#include <stdio.h>
#include <string.h>
#include <stdint.h>

// put both EIDs in the next two lines
const char EID1[] = "jta2448"; // replace abc123 with your EID
const char EID2[] = "wyw82"; // replace abc123 with your EID

// prototypes to your low-level Lab 5 code
void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(uint32_t period);
void Sound_Stop(void);

#define key0   261.6 // actual value 261.6 Hz
#define key1  329.6 // actual value 329.6 Hz
#define key2   392 // actual value 392.0 Hz
#define key3   440 // actual value 440.0 Hz

// Consts (Instruments)
const uint8_t sin_wave[32] = {16, 19, 22, 24, 27, 28, 30, 31 ,31 ,31, 30,
                28, 27, 24, 22, 19, 16, 13, 10, 8, 5, 4,
                2, 1, 1, 1, 2, 4, 5, 8, 10, 13};

const uint8_t Flute[64] = {
    14,16,19,20,21,23,24,26,27,28,29,
    30,30,30,29,28,27,26,25,23,21,20,
    18,17,15,15,14,13,12,12,11,11,
    10,10,10,10,10,9,9,9,8,8,
    7,6,5,5,5,4,4,3,3,3,
    3,4,4,5,5,6,7,8,9,10,11,12
};

const uint8_t Guitar[64] = {
    13,13,13,12,10,8,6,4,4,5,7,
    11,16,19,22,24,24,23,21,17,13,9,
    7,7,9,14,20,26,29,31,29,26,
    22,18,16,14,13,13,13,13,14,15,
    16,17,18,17,16,14,12,9,8,7,
    8,9,9,9,9,9,9,10,12,13,13,13
};

const uint8_t Bassoon[64] = {
    16,18,18,18,17,17,16,16,15,14,14,
    14,16,21,28,31,25,15,6,2,2,5,
    8,11,15,20,22,23,22,18,15,11,
    8,6,5,7,11,16,19,21,19,15,
    13,11,11,12,12,12,13,14,15,15,
    15,16,18,19,18,17,16,16,17,16,15,15
};

const uint8_t Horn[64] = {
    16,16,16,17,17,19,20,25,29,30,31,
    26,20,15,9,8,7,7,7,8,8,8,
    9,9,9,10,11,13,15,16,17,18,
    19,20,22,23,25,25,26,25,24,25,
    27,25,22,19,16,12,8,6,4,3,
    3,3,3,4,5,6,8,9,10,12,14,15
};

const uint8_t Trumpet[64] = {
    20,21,21,22,23,23,23,23,21,18,15,
    10,6,3,2,6,15,23,28,29,29,26,
    23,22,21,21,20,21,21,21,21,20,
    20,20,20,20,20,21,21,20,20,20,
    20,20,21,21,22,22,21,21,21,21,
    21,21,22,22,21,21,21,20,20,20,20,20
};


// Global Vars
uint32_t ind;

// use main1 to determine Lab5 assignment
void Lab5Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
    //uint32_t last=0,now;
    Clock_Init80MHz(0);
    LaunchPad_Init();
    //Key_Init();
    Lab5Grader(0); // print assignment, no grading
    while(1){
    }
}
const uint32_t Inputs[12]={0, 1, 7, 8, 15, 16, 17, 23, 24, 25, 30, 31};
uint32_t Testdata;

// use main2a to perform static testing of DAC, if you have a voltmeter
int main2a(void){ // main2a
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      // put a breakpoint on the next line and use meter to measure DACout
      // place data in Table 5.3
      Debug_Dump(Testdata);
    }
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main2b to perform static testing of DAC, if you do not have a voltmeter
// attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To use the scope, there can be no breakpoints in your code
int main2b(void){ // main2b
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    while(1){}; // access to GPIOB->DOE31_0 should be friendly
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      Debug_Dump(Testdata);
        // use TExaSdisplay scope to measure DACout
        // place data in Table 5.3
        // touch and release S2 to continue
      while(LaunchPad_InS2()==0){}; // wait for S2 to be touched
      while(LaunchPad_InS2()!=0){}; // wait for S2 to be released
      if((GPIOB->DOUT31_0&0x20) == 0){
         while(1){}; // DOUT not friendly
      }
    }
  }
}
// use main3 to perform dynamic testing of DAC,
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a monotonic ramp with period 32ms,
int main3(void){ // main3
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<32; i++){ //0-31
      //DAC5_Out(sin_wave[i]);
      DAC5_Out(i);
      Debug_Dump(i);
        // scope to observe waveform
        // place data in Table 5.3
      Clock_Delay1ms(1);
    }
  }
}

// use main4 to debug the four input switches
int main4(void){ // main4
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Key_Init(); // your Lab 5 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 5, Spring 2024, Step 4. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Key_In(); // Your Lab5 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}

// use main5 to debug your system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Sound_Start(6472); // start one continuous wave (defaulted with 9956 -> 4.0 ms period)
  while(1){
  }
}


// use main6 to debug/grade your final system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging

  uint32_t prev = 0;
  uint32_t curr_key;
  while(1){
      curr_key = Key_In();
      // if key goes from not pressed to pressed
      //   -call Sound_Start with the appropriate period
      //   - call Debug_Dump with period
      if (curr_key && (prev != curr_key)) {
          // Hard Coding Keys
          if (curr_key == 1) {
              Sound_Start(2500000/key0);
              Debug_Dump(2500000/key0);
          } else if (curr_key == 2) {
              Sound_Start(2500000/key1);
              Debug_Dump(2500000/key1);
          } else if (curr_key == 4) {
              Sound_Start(2500000/key2);
              Debug_Dump(2500000/key2);
          } else if (curr_key == 8) {
              Sound_Start(2500000/key3);
              Debug_Dump(2500000/key3);

          // Extra Credit Combinations (all at 300 kHz)
          } else if (curr_key == 3 || curr_key == 7 || curr_key == 15 || curr_key == 6) {
              Sound_Start(2500000/500);
              Debug_Dump(2500000/500);
          }
      }
      // if key goes from pressed to not pressed
      //   -call Sound_Stop
      else if (!curr_key && (prev != curr_key)) {
          Sound_Stop();
      }
      // I.e., if key has not changed DO NOT CALL start or stop

      prev = curr_key;
      Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// To grade you must connect PB20 to your DACout
// Run main6 with Lab5Grader(3);   // Grader
// Observe Terminal window



// Wav File (EC) Test
//int main(void){// mainEC
//  Clock_Init80MHz(0);
//  LaunchPad_Init();
//  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
//  DAC5_Init();     // DAC initialization
//  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
//  Key_Init();      // Keyboard initialization
//  Sound_Start((227)); // start one continuous wave at 11 kHz
//  while(1){
//  }
//}



// Lab 5 low-level function implementations

// ARM SysTick period interrupts
// Input: interrupts every 12.5ns*period
//        priority is 0 (highest) to 3 (lowest)
void Sound_Init(uint32_t period, uint32_t priority){
    SysTick->CTRL = 0x00;
    SysTick->LOAD = period-1;
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000)) | priority << 30;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;

    ind = 0;
}

void Sound_Stop(void){
  // either set LOAD to 0 or clear bit 1 in CTRL
  SysTick->LOAD = 0; // alternative: SysTick->CTRL &= (~0x2);
}


void Sound_Start(uint32_t period){
  // write this
  // set reload value
  // write any value to VAL, cause reload
    SysTick->LOAD = period-1;
    SysTick->VAL = 5;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
    // write this
    // output one value to DAC
    uint32_t inp = Key_In();

    // 64-size Instruments
    //DAC5_Out(Trumpet[ind]);
    //ind = (ind+1) & 0x3F;

    if (inp == 1 || inp == 2 || inp == 4 || inp == 8) {
        // 32-size Sin
        ind = (ind+1) & 0x1F; // 0-31
        DAC5_Out(sin_wave[ind]);
    }

    // Custom Sound Wav File (EC)
    if (inp == 3) {
        // 0-3975
        if (ind < 3975) {
            ind = (ind+1);
        } else {
            ind = 0;
        }
        DAC5_Out(Coin[ind]);
    }

    else if (inp == 6) {
        // 0-35630
        if (ind < 35630) {
            ind = (ind+1);
        } else {
            ind = 0;
        }
        DAC5_Out(FastPiano[ind]);
    } else if (inp == 7) {
        // 0-16485
        if (ind < 16485) {
            ind = (ind+1);
        } else {
            ind = 0;
        }
        DAC5_Out(rage[ind]);
    } else if (inp == 15) {
        // 0-13218
        if (ind < 13218) {
            ind = (ind+1);
        } else {
            ind = 0;
        }
        DAC5_Out(phonk[ind]);
    }

}




