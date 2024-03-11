/* ST7735Rtestmain
 * Jonathan Valvano
 * August 8, 2023
 * Derived
 * Derived from spi_controller_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 *              spi_controller_command_data_control_LP_MSPM0G3507_nortos_ticlang
 *              spi_peripheral_register_format_LP_MSPM0G3507_nortos_ticlang
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/SPI.h"
#include "../inc/ST7735.h"
#include "Logo.h"
#include <stdlib.h>
#include <stdio.h>

// ST7735

int main1(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  SPI_Init();
  GPIOB->DOUT31_0 = BLUE ;
  while(1){
    SPI_Reset();
    GPIOB->DOUTTGL31_0 = BLUE;
  }
}
int main2(void){
uint32_t command=0;
  Clock_Init40MHz();
  LaunchPad_Init();
  SPI_Init();
  GPIOB->DOUTSET31_0 = BLUE ;
  while(1){
    SPI_OutCommand(command);
    for(int i=0; i<8; i++){
      SPI_OutData(i);
    }
    command = (command+1)&0x0F;
    GPIOB->DOUTTGL31_0 = BLUE;
  }
}

int main3(void){ uint32_t data=0;
  Clock_Init40MHz();
  LaunchPad_Init();
  GPIOB->DOUTSET31_0 = BLUE ;

  ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetCursor(0, 0);
  ST7735_OutString("LCD test main3\n");
  while(1){
    ST7735_SetCursor(0, 1);
    ST7735_OutUDec4(data);
    data = (data+1)&0xFF;
    GPIOB->DOUTTGL31_0 = BLUE;
    Clock_Delay1ms(100); // 100ms
  }
}

const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";

int main4(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_OutString("LCD test main4\n");
  ST7735_DrawString(0, 1, (char *)Hello_English, ST7735_RED);
  ST7735_DrawString(0, 2, (char *)Hello_Spanish, ST7735_WHITE);
  ST7735_DrawString(0, 3, (char *)Hello_Portuguese, ST7735_YELLOW);
  ST7735_DrawString(0, 4, (char *)Hello_French, ST7735_CYAN);
  GPIOB->DOUTSET31_0 = BLUE ;
  while(1){
    GPIOB->DOUTTGL31_0 = BLUE;
    Clock_Delay1ms(100); // 100ms
  }
}

int main5(void){uint32_t sound; uint32_t time=0;
  Clock_Init40MHz();
  LaunchPad_Init();
  srand(1);
  ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_Drawaxes(ST7735_YELLOW, ST7735_BLACK, "time","rand", ST7735_WHITE, "", ST7735_BLACK, 3075, 1024);
  while(1){
    time++;
    GPIOB->DOUTTGL31_0 = BLUE;
    sound = 2048+400*rand()/RAND_MAX;
    ST7735_PlotPoint2(sound,ST7735_WHITE);
    Clock_Delay1ms(1); // 1ms
    if(time>10){
      time = 0;
      ST7735PlotIncrement();
    }
  }
}

int main6(void){uint32_t j; // main 6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_OutString("Graphics test\n");
  ST7735_OutString("cubic function\n");
  ST7735_PlotClear(0,4095);  // range from 0 to 4095
  for(j=0;j<128;j++){
    ST7735_PlotPoints(j*j/2+900-(j*j/256)*j,32*j); // cubic,linear
    ST7735_PlotNext();
  }   // called 128 times
  while(1){
  }
}
int main(void){ // main 7
int x, y, dx, dy;

  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_WHITE);

  x = 8;
  y = 64;
  dx = 1;
  dy = 1;
  ST7735_DrawBitmap(x, y, TEXAS, 104, 24);
  while((LaunchPad_InS2()==0)&&(LaunchPad_InS3()==0)){};

  while(1){
    ST7735_DrawBitmap(x, y, TEXAS, 104, 24);
    x = x + dx;
    y = y + dy;
    if((x >= 40) || (x <= -8)){
      dx = -1*dx;
    }
    if((y >= 170) || (y <= 10)){
      dy = -1*dy;
    }
  }
}

void BookExamples(void){ // examples from the book
  int8_t cc = 0x56; // (‘V’)
  int32_t xx = 100;
  int16_t yy = -100;
  float zz = 3.14159265;
  printf("Hello world\n");      //Hello world
  printf("cc = %c %d\n",cc,cc);  //cc = V 86
  printf("cc = %#x\n",cc);      //cc = 0x56
  printf("xx = %c %d\n",xx,xx);  //xx = d 100
  printf("xx = %#x\n",xx);      //xx = 0x64
  printf("yy = %d\n",yy);        //yy = -100
  printf("%#x   \n",yy);        //yy = 0xffffff9c
  printf("%e \n",zz);            //zz = 3.14159e+00
  printf("%E \n",zz);            //zz = 3.14159E+00
  printf("%f     \n",zz);        //zz = 3.14159
  printf("%g     \n",zz);        //zz = 3.14159 (shorter of two, either f or e)
  printf("%3.2f     \n",zz);    //zz =  3.14
}
int main8(void){ int32_t i,n; // main 8
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitPrintf();
  BookExamples();
  while((LaunchPad_InS2()==0)&&(LaunchPad_InS3()==0)){};
  n = 0;
  while(1){
    printf("\ni=");
    for(i=0; i<1; i++){
      printf("%d ",i+n);
    }
    n = n+10000000; // notice what happens when this goes above 2,147,483,647
  }
}
#define B ST7735_BLACK
#define Y ST7735_YELLOW
const uint16_t Enemy2[140] = { // 14 wide 10 high
  B,B,B,B,B,B,B,B,B,B,B,B,B,B,
  B,B,Y,B,B,B,B,B,B,B,B,Y,B,B,
  B,B,B,Y,B,B,B,B,B,B,Y,B,B,B,
  B,B,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,B,B,
  B,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,B,
  B,Y,Y,Y,B,Y,Y,Y,Y,B,Y,Y,Y,B,
  B,Y,B,Y,Y,Y,Y,Y,Y,Y,Y,B,Y,B,
  B,Y,B,B,Y,B,B,B,B,Y,B,B,Y,B,
  B,B,B,Y,B,B,B,B,B,B,Y,B,B,B,
  B,B,B,B,B,B,B,B,B,B,B,B,B,B};
void DrawEnemy2(int32_t x, int32_t y){
  ST7735_DrawBitmap(x, y, Enemy2, 14, 10);
}
#define T33ms 2666666
int main9(void){ int32_t i,x=64,y=80;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_BLACK);
  while(1){
    for(i=0;  i<20; i++){DrawEnemy2(x+i,y);   Clock_Delay(T33ms);}
    for(i=19; i>=0; i--){DrawEnemy2(x+i,y);   Clock_Delay(T33ms);}
    for(i=0;  i<20; i++){DrawEnemy2(x,  y+i); Clock_Delay(T33ms);}
    for(i=19; i>=0; i--){DrawEnemy2(x,  y+i); Clock_Delay(T33ms);}
    for(i=0;  i<20; i++){DrawEnemy2(x+i,y+i); Clock_Delay(T33ms);}
    for(i=19; i>=0; i--){DrawEnemy2(x+i,y+i); Clock_Delay(T33ms);}
  }
}

const uint16_t Enemy[96] = { // 12 wide, 8 high
  B,Y,B,B,B,B,B,B,B,B,Y,B,
  B,B,Y,B,B,B,B,B,B,Y,B,B,
  B,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,B,
  Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,
  Y,Y,Y,B,Y,Y,Y,Y,B,Y,Y,Y,
  Y,B,Y,Y,Y,Y,Y,Y,Y,Y,B,Y,
  Y,B,B,Y,B,B,B,B,Y,B,B,Y,
  B,B,Y,B,B,B,B,B,B,Y,B,B};
const uint16_t EnemyBlank[96] = { // 12 by 8 black rectangle
  B,B,B,B,B,B,B,B,B,B,B,B,  B,B,B,B,B,B,B,B,B,B,B,B,
  B,B,B,B,B,B,B,B,B,B,B,B,  B,B,B,B,B,B,B,B,B,B,B,B,
  B,B,B,B,B,B,B,B,B,B,B,B,  B,B,B,B,B,B,B,B,B,B,B,B,
  B,B,B,B,B,B,B,B,B,B,B,B,  B,B,B,B,B,B,B,B,B,B,B,B};
int32_t Oldx, Oldy;
void DrawEnemy(int32_t x, int32_t y){
  if((x == Oldx)&&(y == Oldy)) return;
  ST7735_DrawBitmap(Oldx, Oldy, EnemyBlank, 12, 8);
  ST7735_DrawBitmap(x, y, Enemy, 12, 8);
  Oldx = x; Oldy = y;
}
int main10(void){ int32_t i,x=64,y=80;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(x, y, Enemy, 12, 8); // initial object
  Oldx = x; Oldy = y;                    // initial position
  while(1){
    for(i=-50; i<+50; i+=5){DrawEnemy(x+i,y);   Clock_Delay(T33ms);}
    for(i=+50; i>-50; i-=5){DrawEnemy(x+i,y);   Clock_Delay(T33ms);}
    for(i=-50; i<+50; i+=5){DrawEnemy(x,  y+i); Clock_Delay(T33ms);}
    for(i=+50; i>-50; i-=5){DrawEnemy(x,  y+i); Clock_Delay(T33ms);}
    for(i=-50; i<+50; i+=5){DrawEnemy(x+i,y+i); Clock_Delay(T33ms);}
    for(i=+50; i>-50; i-=5){DrawEnemy(x+i,y+i); Clock_Delay(T33ms);}
  }
}


