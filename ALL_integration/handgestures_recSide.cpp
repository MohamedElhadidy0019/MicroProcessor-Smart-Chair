


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

#include "handgestures_recSide.h"



const uint64_t pipe = 0x1;  // This is the transmit pipe
int sendData[1];  // One element array holding our random number
RF24 radio(CE_PIN, CSN_PIN);


void RadioSetup()
{
    Serial.begin(9600);
    delay(1000);
    radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
}

int RecieveData()
{
    if (radio.available())
    {
        bool done = false;
        while (!done)
        {
            done = radio.read(sendData, sizeof(sendData));
        }
        return sendData[0];
    }
    return -10000;
}

void HandGesturesMove()
{
    int r = RecieveData();
    if (r == 0) {
        //Serial.println("recieved: move right");
        //MoveRight();          //uncomment this later when we know the name of the functions
    }
    else if (r == 1) {
        //Serial.println("recieved: move left");
        //MoveLeft();          //uncomment this later when we know the name of the functions
    }
    else if (r == 2) {
        //Serial.println("recieved: move forward");
        //MoveForward();          //uncomment this later when we know the name of the functions
    }
    else if (r == 3) {
        //Serial.println("recieved: move backward");
        //MoveBackward();          //uncomment this later when we know the name of the functions
    }

}

void setup_hangestures()
{
    RadioSetup();
}

void loop_handgestures()
{
    //if(Ser)
    HandGesturesMove();

}
