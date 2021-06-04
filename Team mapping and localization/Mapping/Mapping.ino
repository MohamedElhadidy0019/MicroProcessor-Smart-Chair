#define TrigerPin1 11
#define FirstUltra_Echo 8

#define Switch_front 5
#define Switch_back 6
#define Switch_right 3
#define Switch_left 2
#define Width 100
#define height 100
#define ARR_SIZE 50
#define DISTANCE_PER_MOVE 5

// declare map of size 200 * 200 that will hold the information about the existance of the obstacles.

// int Map[Width][height];
int Map[ARR_SIZE][ARR_SIZE];
double front_angle = 0;
int current_location_front = 0;
int current_location_width = 0;
int x_position = 0;
int y_position = 0;
// fill it with zeroes initially ( if (x,y)= Zero --> free to configure a path passing througth this point)
//memset(Map,0,sizeof Map);

//Definations related to transformation:
const float Ultra1_to_base_link = 10; // distance from the first US to the base link of the robot (motor) in cm
const float Ultra2_to_base_link = 10;
const float Ultra3_to_base_link = 10;
float distance;
float duration;
// angles
digita
void Transform(int distance, int Ultra_num)
{

  if (distance <= 400 || distance >= 2)
  {
    float x, y;
    switch (Ultra_num)
    {
    case 1:
      // x =
      // y =
      // Map[x][y] = 1;
      break;

    case 2:
      // x =
      // y =
      // Map[x][y] = 1;
      break;

    case 3:
      // x =
      // y =
      // Map[x][y] = 1;
      break;
    }
  }
  return;
}
void calculate_Distance(int duration, int num)
{
  distance = (duration * .0343) / 2.0; // 0.343 is the speed of the sound , dividing by 2 because duration is for goes and return.
  if (distance > 100)
  {
    distance = -1; //readiung out of range
  }
  Transform(distance, num);
}

void Read_ultrasonic()
{
  digitalWrite(TrigerPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigerPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigerPin1, LOW);
  duration = pulseIn(FirstUltra_Echo, HIGH);
  calculate_Distance(duration, 1);
  Serial.print("distance=");
  Serial.println(distance);
}

void Read_swithces()
{

  if (digitalRead(Switch_front))
  {
    while (digitalRead(Switch_front))
      ;
    delay(10);
    // curretn (x,y) location of the chair
    x_position = x_position + DISTANCE_PER_MOVE * sin(front_angle);
    y_position = y_position + DISTANCE_PER_MOVE * cos(front_angle);
  }
  if (digitalRead(Switch_back))
  {
    while (digitalRead(Switch_back))
      ;
    delay(10);
    x_position = x_position - DISTANCE_PER_MOVE * sin(front_angle);
    y_position = y_position - DISTANCE_PER_MOVE * cos(front_angle);
  }
  if (digitalRead(Switch_left)) //rotate anti-clock wise
  {
    while (digitalRead(Switch_left))
      ;
    delay(10);
    front_angle -= (M_PI / 2.0);
    // current_location_width += 5;
  }
  if (digitalRead(Switch_right)) //rotate clockwise
  {
    while (digitalRead(Switch_right))
      ;
    delay(10);
    // current_location_width -= 5;
    front_angle += (M_PI / 2.0);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(TrigerPin1, OUTPUT);
  pinMode(Switch_front, INPUT);
  pinMode(Switch_back, INPUT);
  pinMode(Switch_left, INPUT);
  pinMode(Switch_right, INPUT);
  pinMode(FirstUltra_Echo, INPUT);
}
void loop()
{
  Read_ultrasonic();
 // Read_swithces();
  // if (distance != -1)
  // {
  //   int x_map=x_position+distance*sin(front_angle);
  //   int y_map=y_position+distance*cos(front_angle);
  //   Map[x_map][y_map]= 1;
  //   Serial.print("(");
  //   Serial.print(x_map);
  //   Serial.print(",");
  //   Serial.print(y_map);
  //   Serial.println(")");
  //   // Map[current_location_width][current_location_front + int(distance)] = 1;
  // }
}

// Map[current_location + int(distance)] = 1;
// for (int i = current_location; i < current_location + int(distance); i++)
// {
//   Map[i] = 0;
// }
// for (int i = 0; i < 50; i++)
// {
//   Serial.print("Map[");
//   Serial.print(i);
//   Serial.print("]=");
//   Serial.print(Map[i]);
//   Serial.print(", ");
// }
// Serial.println("");
// Serial.println("");
// Serial.println("");
//delay(500);

//digitalWrite(TrigerPin2, LOW);
//delayMicroseconds(2);
//digitalWrite(TrigerPin2, HIGH);
//delayMicroseconds(10);
//digitalWrite(TrigerPin2, LOW);
//duration = pulseIn(SecondUltra_Echo, HIGH);
//calculate_Distance(duration,2);
//
//digitalWrite(TrigerPin3, LOW);
//delayMicroseconds(2);
//digitalWrite(TrigerPin3, HIGH);
//delayMicroseconds(10);
//digitalWrite(TrigerPin3, LOW);
//duration = pulseIn(ThirdUltra_Echo, HIGH);
//calculate_Distance(duration,3);