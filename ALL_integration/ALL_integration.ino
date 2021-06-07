#include "handgestures_recSide.h"

char Code_State;
// h   ->hand)gestur
// b
// c
//d
//e
//f
//g

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  //non blocking delay 100 ms
  while (!Serial1.available());
  Code_State = Serial1.read();

  //Code_State='a';
}


void loop()
{
  if (Serial1.available())
  {
    Code_State = Serial1.read();
  }
  switch (Code_State)
  {
    case 'h':
      loop_handgestures(&Code_State);
      break;
    default :

      break;
  }


}
