#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"
#include "../inc/DAC5.h"
#include "../inc/Key.h"
#include "wave.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

const char EID1[] = "";
const char EID2[] = "";

// prototypes
void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(uint32_t period);
void Sound_Stop(void);

#define key0   261.6 // actual value 261.6 Hz
#define key1  329.6 // actual value 329.6 Hz
#define key2   392 // actual value 392.0 Hz
#define key3   440 // actual value 440.0 Hz

// Consts (Instruments)
const uint16_t sin_wave[64] = {
32, 35, 38, 41, 44, 46, 49, 51, 54, 56,
58, 59, 61, 62, 62, 63, 63, 63, 62, 62,
61, 59, 58, 56, 54, 51, 49, 46, 44, 41,
38, 35, 32, 28, 25, 22, 19, 17, 14, 12,
9, 7, 5, 4, 2, 1, 1, 0, 0, 0,
1, 1, 2, 4, 5, 7, 9, 12, 14, 17,
19, 22, 25, 28};

// Global Vars
uint32_t ind;

// use main1 to determine Lab5 assignment
void Lab5Grader(int mode);
void Grader_Init(void);

const uint32_t Inputs[12]={0, 1, 7, 8, 15, 16, 17, 23, 24, 25, 30, 31};
uint32_t Testdata;

// use main2 to perform static testing of DAC with voltmeter
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();
  DAC5_Init();
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      // put a breakpoint on the next line and use meter to measure DACout
      Debug_Dump(Testdata);
    }
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}

// use main3 to perform dynamic testing of DAC,
// attach PB20 (scope uses PB20 as ADC input) to DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in code
// DACout will be a monotonic ramp with period 32ms,
int main3(void){ // main3
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();
  Lab5Grader(2);   // Scope
  DAC5_Init();
  Debug_Init();
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
  Key_Init();
  Debug_Init();
  UART_Init();
  __enable_irq(); // UART uses interrupts
  while(1){
    now = Key_In();
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}

// attach PB20 (scope uses PB20 as ADC input) to DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in code
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

int main(void){
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Debug Initialization

  uint32_t prev = 0;
  uint32_t curr_key;
  while(1){
      curr_key = Key_In();
      // if key goes from not pressed to pressed
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

          // Combinations for special sounds (all at 300 kHz)
          } else if (curr_key == 3 || curr_key == 7 || curr_key == 6) {
              Sound_Start(2500000/500);
              Debug_Dump(2500000/500);
          } else if (curr_key == 15) {
              Sound_Start(2500000/350);
              Debug_Dump(2500000/350);
          }
      }
      // if key goes from pressed to not pressed
      else if (!curr_key && (prev != curr_key)) {
          Sound_Stop();
      }

      prev = curr_key;
      Clock_Delay(800000); // 10ms, to debounce switch
  }
}

// low-level function implementations

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
  // set reload value
  // write any value to VAL, cause reload
    SysTick->LOAD = period-1;
    SysTick->VAL = 5;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
    // output one value to DAC
    uint32_t inp = Key_In();

//    // 64-size Sin Wave
//    DAC5_Out(sin_wave[ind]);
//    ind = (ind+1) & 0x3F;

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
    } else if (inp == 6) {
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
        if (ind < 15112) {
            ind = (ind+1);
        } else {
            ind = 0;
        }
        DAC5_Out(swvader[ind]);
    }

}




