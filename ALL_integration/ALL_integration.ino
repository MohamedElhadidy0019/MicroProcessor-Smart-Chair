#include "handgestures_recSide.h"

char Code_State;
// a   ->hand)gestur
// b
// c
//d
//e
//f
//g

void setup()
{
  Serial.begin(9600);
  //non blocking delay 100 ms
  while (!Serial.available());
  Code_State = Serial.read();


}


void loop()
{
  switch (Code_State)
  {
    case 'a':
      loop_handgestures(&Code_State);
      break;
  }


}
