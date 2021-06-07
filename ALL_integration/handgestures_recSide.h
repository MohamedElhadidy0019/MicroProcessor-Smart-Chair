

/*
  MEGA
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 48
   4 - CSN to Arduino pin 49
   5 - SCK to Arduino pin 52
   6 - MOSI to Arduino pin 51
   7 - MISO to Arduino pin 50
   8 - UNUSED


*/
#ifndef handGestures_h
#define handGestures_h

#include<Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN   48
#define CSN_PIN 49


void RadioSetup();
int RecieveData();

void HandGesturesMove();

void setup_hangestures();

void loop_handgestures();

#endif
