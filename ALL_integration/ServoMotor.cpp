;
#include"ServoMotor.h"
/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/



Servo myservo1; // create servo object to control a servo
Servo myservo2;
int Count = 0;
int pastCount = 0;
// int potpin = A8; // analog pin used to connect the potentiometer
// int potpin2 = A9;
#define potpin A11
#define potpin2 A12
int val; // variable to read the value from the analog pin

int val2;

int pastValServ1 = 0;
int pastValServ2 = 0;

void setup_Servo() {

  myservo1.attach(11);
  myservo2.attach(12);
}

void loop_Servo(char * Code_state_ptr) {

  setup_Servo();
  while (1)
  {
    if(Serial1.available())
    {
      Code_state_ptr=Serial1.read();
      if(Code_state_ptr!='f')
      {
          break;
      }
    }
    ServoFin();
  }
}

                


void ServoFin() {
  val = analogRead(potpin);
  val2 = analogRead(potpin2);
  val = map(val, 0, 1023, 0, 180);
  val2 = map(val2 , 0 , 1023 , 0 , 180);


  delay(100);
  if (val != pastValServ1 )
  {
    myservo1.write(val) ;
    pastValServ1 = val ;
    delay(10);
  }

  if  (abs (val2 - pastValServ2) > 10  ) {
    myservo2.write(val2);
    pastValServ2 = val2 ;
    delay(100);
  }




}
