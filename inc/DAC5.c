/* DAC5.c
 * Jerin Andrews and William Wang
 * Modified: January 1, 2024
 * 5-bit binary-weighted DAC connected to PB4-PB0
 */
#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "../inc/LaunchPad.h"

#define PB0INDEX  11 // UART0_TX  SPI1_CS2  TIMA1_C0  TIMA0_C2
#define PB1INDEX  12 // UART0_RX  SPI1_CS3  TIMA1_C1  TIMA0_C2N
#define PB2INDEX  14 // UART3_TX  UART2_CTS I2C1_SCL  TIMA0_C3  UART1_CTS TIMG6_C0  TIMA1_C0
#define PB3INDEX  15 // UART3_RX  UART2_RTS I2C1_SDA  TIMA0_C3N UART1_RTS TIMG6_C1  TIMA1_C1
#define PB4INDEX  16 // UART1_TX  UART3_CTS TIMA1_C0  TIMA0_C2  TIMA1_C0N

const uint32_t out_init = 0x00000081;

// **************DAC5_Init*********************
// Initialize 5-bit DAC, called once
// Input: none
// Output: none
void DAC5_Init(void){
// Assumes LaunchPad_Init has been called
// I.e., PortB has already been reset and activated (do not reset PortB here again)
    IOMUX->SECCFG.PINCM[PB0INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB1INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB2INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB3INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB4INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB6INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB7INDEX] = out_init;
    IOMUX->SECCFG.PINCM[PB8INDEX] = out_init;
    // Enabling Output to Port B Pins 0-4
    GPIOB->DOE31_0 |= (0x1F | (0x7 << 6));
}

// **************DAC5_Out*********************
// output to DAC5
// Input: data is 5-bit integer, 0 to 31
// Output: none
// Note: this solution must be friendly

uint32_t mask = 0;

void DAC5_Out(uint32_t data){
    mask = (((0x7 << 5) & data) << 1); // shifts values at/to the left of 5th (mask-val)
    data &= ~(0x7 << 5);
    data |= mask;

    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & (~0x1DF)) | data;
}
