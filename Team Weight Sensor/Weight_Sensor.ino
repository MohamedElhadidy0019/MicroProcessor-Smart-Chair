#include "HX711.h"

// this code assumes readings are in grams

// being falling from chair alert
HX711 hx;

#define calibration_factor        600       // This value is obtained using the SparkFun_HX711_Calibration sketch
#define HX711_DOUT_PIN            4         // dout pin
#define HX711_SCK_PIN             3         // sck pin
#define NUM_OF_READINGS           20        // used to average on readings
#define NUM_STORED_READINGS       5         // size of the readings array
#define MAX_ACCEPTABLE_WEIGHT     6000      // if weight is exceeds this -> OVERLOAD!
#define button                    2         // button pin
#define buzzer                    11        // buzzer pin
#define BUZZ_FREQ                 500       // signal sent to buzzer via function tone()
// maximum acceptable [alert-less] rate of DECREASE change of weight with respect to time ->
//( diff. in Weight / diff. in Time ) == gram per second
#define Weight_Threshold          500

// volatile for interrupt 
volatile byte buzzer_ringing = 0;

struct HXPAIR 
{
    // this struct is used to store weight readings / time
    volatile unsigned long t_reading = 0;
    volatile double w_reading = 0;
};

struct HXPAIR readings[NUM_STORED_READINGS];

void setup()
{
    //for DEBUG 
    Serial.begin(9600);
     
    pinMode(button,  INPUT);
    pinMode(buzzer,  OUTPUT);
    buzzer_ringing = LOW;

    // attach button pin to interrupt on rising event
    attachInterrupt(digitalPinToInterrupt(button),resett,RISING);
    
    // begin falling from chair alert
    hx.begin(HX711_DOUT_PIN, HX711_SCK_PIN);

    hx.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
    hx.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
    
    for (int i = 0; i < NUM_STORED_READINGS; i++)
        readings[i].t_reading = 0;// time zero --> time for this i was not set yet
}

void read_and_store()
{
    // wake up the HX711
    hx.power_up();

    //get current time in millisec
    unsigned long curr_time = millis();

    //get the current weight reading
    double curr_value = hx.get_units(NUM_OF_READINGS);

    // other varients
    // hx.read();// reads the current value
    // hx.read_average(NUM_OF_READINGS);// reads but with average
    // hx.get_value(NUM_OF_READINGS);// read_average()-TARE
    // hx.get_units(NUM_OF_READINGS);// get_value()/SCALE

    // get more accurate
    curr_time = curr_time / 2.0 + millis() / 2.0;

    // put the HX711 to sleep mode, redundant??
    hx.power_down();

    // shift the readings array
    for (int i = NUM_STORED_READINGS - 1; i > 0; i--)
    {
        readings[i].t_reading = readings[i-1].t_reading;
        readings[i].w_reading = readings[i-1].w_reading;
    }

    // store the most recent reading
    // store time in MILLI seconds
    readings[0].t_reading = curr_time;
    readings[0].w_reading = curr_value;
}

void resett()
{
   //shut-down if button pressed
   if(buzzer_ringing == LOW)
     {
        Serial.println(F("button is pressed, buzzer already off, no action"));
        return;
     }

   Serial.println(F("button is pressed, turning off buzzer  "));
   noTone(buzzer);
   buzzer_ringing = LOW;
   
   //clearing the array, as we are selecting multiple readings -> we dont want ->
   //same reading to induce alarm more than once
   for (int i = 0; i < NUM_STORED_READINGS; i++)
        readings[i].t_reading = 0;// time zero = time for this i   was not recorded yet

}

void loop() 
{ 
  // TEST BLOCK //////////////////////////////////
    delay(70);
    Serial.print(F("Button is: "));
    Serial.println(digitalRead(button));

    Serial.print(F("Buzzer is: "));
    Serial.println(buzzer_ringing);

  ///////////////////////////////////////////////
  
  //if already ringing, return as we don't care what happens
  if(buzzer_ringing)
   {
       Serial.println(F("ringing...."));      
       return;
   }
   
  //get current reading
  read_and_store();

  // OVERLOAD check
  if( readings[0].w_reading > MAX_ACCEPTABLE_WEIGHT )
  {
    Serial.print(F("OVERLOAD! "));
    buzzer_ringing=HIGH;
    tone (buzzer,BUZZ_FREQ);  
    return;
  }

  // display readings array
  Serial.print( F("Current readings: ") );
  for (int i = 0; i < NUM_STORED_READINGS; i++)
      {
         Serial.print(readings[i].w_reading);
         Serial.print("   ");
      }
  Serial.println();
  
  Serial.print( F("Current TIME readings: ") );
  for (int i = 0; i < NUM_STORED_READINGS; i++)
      {
         Serial.print(readings[i].t_reading);
         Serial.print(" ");
      }
  Serial.println();
   
  //exit if there is still weight ?
  /*if(readings[0].w_reading)
      {
        Serial.println("There is still weight --> no fall or kidnap ");
        return;
      }*/
      
  //if no readings yet retutn, theoritcially impossible but meh
  if( readings[0].t_reading == 0 )
      {
        Serial.println( F("no readings yet") );
        return;
      }
      
  //Get the maximum "speed" over previous readings
  double speed = 0;
  for(int i = 0; i < NUM_STORED_READINGS-1 ; i++)
  {  
    //break if there is no more VALID readings
    if( readings[i+1].t_reading == 0 )
      break;
      
    // Weight I lost = previous reading - this reading
    double dw = readings[i+1].w_reading - readings[i].w_reading;
    //Serial.println(dw);
    
    //Time elapsed = this - previous
    unsigned long dt = readings[i].t_reading - readings[i+1].t_reading;
    //Serial.println(dt);

    // rate of change of weight
    // multiplying by 1e3 to convert from millis to seconds
    double curr = 1000 * dw / dt;
    //Serial.print(curr);
    
    //note that if curr is negative --> weight increased which doesn't trigger the alarm
    speed = max(speed, curr );
  }

  Serial.print( F("max rate of DECREASE in grams per second! : ") );
  Serial.println(speed);

  // comparing max. rate to alarm threshold
  if(speed < Weight_Threshold)
      {
        Serial.println( F("sub-threshold change ") );
        return;
      }

  // trigerring buzzer
  Serial.println( F("DANGER !! RINGING ") );
  buzzer_ringing = HIGH;
  tone (buzzer,BUZZ_FREQ);  

}
