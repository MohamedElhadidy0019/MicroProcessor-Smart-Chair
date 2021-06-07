#include"PID_motors.h"


const byte NUMSTEPS = 40;
const int MOTOTSPEED = 150;//57
volatile long SpeedSensorCounter = 0;
long PreviousSSCntTime = 0;
int RPM;
int WheelRadius = 3;
int target;

volatile int NumStepsMoved = 0;



//need to call this function in the void setup() in the .ino file
void motors_setup()
{
    pinMode(SpeedSensorPin, INPUT_PULLUP);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    //pinMode(13, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, FALLING);
    analogWrite(ENA, 0); //map(target-OrderCounter,0,target,0,255));
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENB, 0); //map(target-OrderCounter,0,target,0,255));
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    target = 40;
}

void setTarget_CM(double cm)
{ //convert cm to numsteps
    target = cm / (2 * PI * WheelRadius) * NUMSTEPS;
}
void setTarget_Angle(int angle)
{ //convert angle to numsteps
    int singlestep = 360 / NUMSTEPS;
    //if (angle % singlestep >= singlestep / 2) angle += singlestep;
    target = angle / singlestep;
    //Serial.print("Angle: ");
    //Serial.print(target);
    //Serial.println(" Step");
}

void MoveTargetSteps()
{
    long StartTime = millis();

    int z = 0;
    detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
    NumStepsMoved = 0;
    attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);

    while (z < target)
    {
        //Serial.print("NumStepsMoved: ");
        //Serial.println(z);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

        analogWrite(ENA, MOTOTSPEED);
        analogWrite(ENB, MOTOTSPEED);

        detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
        z = NumStepsMoved;
        attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);
    }
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);

    detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
    NumStepsMoved = 0;
    attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);
}

//////////////////////////////////////
void DoRotate(int RDir)
{

    long StartTime = millis();

    int z = 0;
    detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
    NumStepsMoved = 0;
    attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);

    while (z < target)
    {
        //Serial.print("NumStepsMoved: ");
        //Serial.println(z);
        if (RDir == RIGHT)
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);

            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }
        else
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);

            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }

        analogWrite(ENA, MOTOTSPEED);
        analogWrite(ENB, MOTOTSPEED);

        detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
        z = NumStepsMoved;
        attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);
    }
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);

    detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
    NumStepsMoved = 0;
    attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);
}
///////////////
void RotateRight()
{
    setTarget_CM(ANGLE_90);
    DoRotate(RIGHT);
}
void RotateLeft()
{
    setTarget_CM(ANGLE_90);
    DoRotate(LEFT);
}
///////////////////////////////////////////

void FreeRotate(int FRDir)
{
    setTarget_CM(5);
    DoRotate(FRDir);
}
void Move(int kam, int mode = 0)
{ //move the wheen: mode = 0, cm   ; mode = 1 , angle
    if (mode == 0)
        setTarget_CM(kam);
    else if (mode == 1)
        setTarget_Angle(kam);
    MoveTargetSteps();
    //dir=!dir;
}

void IncCounter()
{
    SpeedSensorCounter++;
    NumStepsMoved++;
}
void GetRPM()
{
    if (millis() - PreviousSSCntTime >= 1000)
    {
        int z;
        detachInterrupt(digitalPinToInterrupt(SpeedSensorPin));
        z = SpeedSensorCounter;
        SpeedSensorCounter = 0;
        attachInterrupt(digitalPinToInterrupt(SpeedSensorPin), IncCounter, CHANGE);

        RPM = 60.0 * ((z / NUMSTEPS) / (1.0 * (millis() - PreviousSSCntTime) / 1000));
        SpeedSensorCounter = 0;

        PreviousSSCntTime = millis();
    }
}
