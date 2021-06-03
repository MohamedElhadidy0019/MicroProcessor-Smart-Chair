#include <Arduino.h>
#line 1 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>

/* Sensors data : */
const byte TrigerPins [3] = {10,11,12};              /* index --> 0 : front, 1 :right, 2 : left */
const byte echos [3] = {7,8,9};                      /* index --> 0 : front, 1 :right, 2 : left */
byte current_sensor = 0;
float distance;
float duration;

/* Map :*/
#define MAP_MAX_X 100
#define MAP_MAX_Y 150
uint8_t Map[1875];                                   /* 100bit*150bit == 100cm*150cm
                                                        x is width : MAP_MAX_X   
                                                        y is height: MAP_MAY_Y   
                                                        we access the array by coordinated(x,y)*/
                                                                                            
/* Robot position : */
#define DISTANCE_PER_MOVE 5
int x_position = 0;
int y_position = 0;
double current_angle = 0;                      

/* for testing : */
#define Switch_front 5
#define Switch_back 6
#define Switch_right 3
#define Switch_left 2


#line 33 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void setup();
#line 45 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void loop();
#line 54 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
uint8_t read_Map(uint8_t index_x,uint8_t index_y);
#line 65 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void write_Map(uint8_t index_x,uint8_t index_y,uint8_t state);
#line 82 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Transform(int dist, int sensorNumber);
#line 112 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void calculate_Distance(int duration, int sensor_num);
#line 124 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Read_ultrasonic();
#line 146 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Read_swithces();
#line 33 "e:\\Desktop\\college\\second term\\Microprocessor\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void setup()
{
  Serial.begin(9600);
  for(byte i = 0 ; i<3 ; i++) {pinMode(TrigerPins[i], INPUT);}
  for(byte i = 0 ; i<3 ; i++) {pinMode(echos[i], INPUT);}
  pinMode(Switch_front, INPUT);
  pinMode(Switch_back, INPUT);
  pinMode(Switch_left, INPUT);
  pinMode(Switch_right, INPUT);
  
}

void loop()
{
  
  Read_ultrasonic();
  Read_swithces();
  
}

/*function takes (x,y) and return the value of the bit wether it is 1 or 0 */
uint8_t read_Map(uint8_t index_x,uint8_t index_y) 
{
    /* index = x + width * y 
    source: https://softwareengineering.stackexchange.com/questions/212808/treating-a-1d-data-structure-as-2d-grid/212813 */  
    int index= index_x + MAP_MAX_X * index_y;
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    return((Map[bigIndex]>>shiftIndex)&0x01);
}

/* (x_position, y_postion, state to set the bit or clear it) */
void write_Map(uint8_t index_x,uint8_t index_y,uint8_t state)  
{
    int index = index_x + MAP_MAX_X * index_y;
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    if(state)    //set bit
    {
        Map[bigIndex]|=1<<shiftIndex;
    }
    else        // clear bit 
    {
        Map[bigIndex]&=~1<<shiftIndex;
    }

}

/* to transform the distance into x , y (for the map indexing) position based on the sensor number */
void Transform(int dist, int sensorNumber)
{
    int x , y ;
    if (dist != -1)
    { switch (sensorNumber)
      {
      case 0:     /* front */
        x = x_position + dist * sin(current_angle);
        y = y_position + dist * cos (current_angle);
        break;

      case 1:     /* right */
        x = x_position + dist * cos(current_angle);
        y = y_position + dist * sin(current_angle * -1 );
        break;

      case 2:     /* left */
      
        x = x_position - dist * cos(current_angle);
        y = y_position + dist * sin(current_angle);
        break;
      }
      /* if x or y are < 0 , then point is out of the rang of the map */
      if( ! (x < 0 || y < 0 ))  write_Map(x,y,1);
     
    }
    return;
  }

/* convert the duration from the sensor -- > distance */
void calculate_Distance(int duration, int sensor_num)
{
  /*0.343 is the speed of the sound , dividing by 2 because this duration is for goes and return.*/
  distance = (duration * .0343) / 2.0; 
  if (distance > 100)
  {
    distance = -1; //readiung out of range
  }
  Transform(distance, sensor_num);
}

/* handle the readings from the 3 sensors simultaneously */
void Read_ultrasonic()
{
  /* setting the trigger of the current sensor : low */
  digitalWrite(TrigerPins[current_sensor], LOW);
   
  delayMicroseconds(2);

  digitalWrite(TrigerPins[current_sensor], HIGH);
  delayMicroseconds(10); 
  
  duration = pulseIn(echos[current_sensor], HIGH);
  calculate_Distance(duration, current_sensor);
  Serial.print("Distance is =");
  Serial.println(distance);
  
  digitalWrite(TrigerPins[current_sensor], LOW);
  current_sensor = (current_sensor == 2)? 0 : current_sensor + 1;
  

}

/* act as the control part */
void Read_swithces()
{

  if (digitalRead(Switch_front))
  {
    while (digitalRead(Switch_front))
      ;
    delay(10);
    // curretn (x,y) location of the chair
    x_position = x_position + DISTANCE_PER_MOVE * sin(current_angle);
    y_position = y_position + DISTANCE_PER_MOVE * cos(current_angle);
  }
  if (digitalRead(Switch_back))
  {
    while (digitalRead(Switch_back))
      ;
    delay(10);
    x_position = x_position - DISTANCE_PER_MOVE * sin(current_angle);
    y_position = y_position - DISTANCE_PER_MOVE * cos(current_angle);
  }
  if (digitalRead(Switch_left)) //rotate anti-clock wise
  {
    while (digitalRead(Switch_left))
      ;
    delay(10);
    current_angle -= (M_PI / 2.0);
    // current_location_width += 5;
  }
  if (digitalRead(Switch_right)) //rotate clockwise
  {
    while (digitalRead(Switch_right))
      ;
    delay(10);
    // current_location_width -= 5;
    current_angle += (M_PI / 2.0);
  }
}

