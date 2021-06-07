#ifndef MOTORS_PID_H
#define MOTORS_PID_H


#include "Arduino.h"

/*______________________PINS____________________*/
#define SpeedSensorPin 2  // interrupt pin
#define ENA 10
#define ENB 9
#define IN1 7
#define IN2 6
#define IN3 5
#define IN4 4
/*_____________________________________________*/
#define UNITSTEP 100
#define RIGHT 1
#define LEFT -1
#define ANGLE_90 37

//need to call this function in the void setup() in the .ino file
void motors_setup();

void setTarget_CM(double cm);

void setTarget_Angle(int angle);
void MoveTargetSteps();
void DoRotate(int RDir);
void RotateRight();

void RotateLeft();

void FreeRotate(int FRDir);

void Move(int kam, int mode = 0);
void IncCounter();
void GetRPM();




#endif

