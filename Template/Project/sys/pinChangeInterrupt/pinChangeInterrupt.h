//
// Created by joey on 2024-06-18.
//

#include "../sys/headerfile.h"


#define P25					ARDUINO_13_PIN              
#define P24					ARDUINO_12_PIN              
#define P23					ARDUINO_11_PIN            
#define P22					ARDUINO_10_PIN              
#define P20					ARDUINO_9_PIN          
#define P19					ARDUINO_8_PIN             
#define P18					ARDUINO_7_PIN             
#define P17					ARDUINO_6_PIN             
#define P16					ARDUINO_5_PIN             
#define P15					ARDUINO_4_PIN            
#define P14					ARDUINO_3_PIN            
#define P13					ARDUINO_2_PIN               
#define P12					ARDUINO_1_PIN              
#define P11					ARDUINO_0_PIN               

void initPinChangeInterrupt();
void enableBMA400ReadyPin50Hz();
void disableBMA400ReadyPin50Hz();
bool isBMA400ReadyPin50Hz();

void enableBMA400Tap();
void disableBMA400Tap();
bool isBMA400Tap();