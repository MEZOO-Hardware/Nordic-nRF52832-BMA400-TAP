//
// Created by joey on 2024-06-18.
//

#ifndef NORDIC_NRF52832_TEMPLATE_PORT_H
#define NORDIC_NRF52832_TEMPLATE_PORT_H

#include "../sys/headerfile.h"

typedef enum
{
    PORT0_NULL,
    PORT1_NULL,
    PORT2_NULL,
    PORT3_NULL,
    PORT4_Int,
    PORT5_NULL,
    PORT6_Tx,
    PORT7_NULL,
    PORT8_Rx,
    PORT9_NULL,
    PORT10_NULL,
    PORT11_NULL,
    PORT12_NULL,
    PORT13_Button1,
    PORT14_Button2,
    PORT15_Button3,
    PORT16_Button4,
    PORT17_LED1,
    PORT18_LED2,
    PORT19_LED3,
    PORT20_LED4,
    PORT21_NULL,
    PORT22_NULL,
    PORT23_NULL,
    PORT24_NULL,
    PORT25_NULL,
    PORT26_SDA,
    PORT27_SCL,
    PORT28_MAX30001_CS,
    PORT29_MISO,
    PORT30_SCLK,
    PORT31_MOSI
} PORT_NRF52832;

typedef enum
{
    Port_SCLK      = PORT30_SCLK,
    Port_MISO      = PORT29_MISO,
    Port_MOSI      = PORT31_MOSI,
    Port_RX        = PORT8_Rx,
    Port_TX        = PORT6_Tx,
    Port_SCL       = PORT27_SCL,
    Port_SDA       = PORT26_SDA
} Port_Communication;

void initPortOutSet(uint8_t portNumber);
void initPortOutClear(uint8_t portNumber);
void initPortToggle(uint8_t portNumber);
void initNRF52Port();
void initComoPort();
void initPort();
void initPowerHold();

#endif //NORDIC_NRF52832_TEMPLATE_PORT_H
