#include <Arduino.h>
#line 1 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"

#include <stdlib.h>
#include <stdint.h>
//#include <NewPing.h> //library for ultrasonic sensor
#include "PID_motors.h"

#define PI (float)3.141
/* Sensors data : */
//const byte TrigerPins [3] = {10,11,12};              /* index --> 0 : front, 1 :right, 2 : left */
//const byte echos [3] = {7,8,9};                      /* index --> 0 : front, 1 :right, 2 : left */
// const byte TrigerPins = 11;
// const byte echos = 8;
#define TrigerPins A0
#define echos A1
//byte current_sensor = 0;
float distance;
float duration;

/* Map :*/
/* #define MAP_MAX_X 100
#define MAP_MAX_Y 150
uint8_t Map[1875];   */
/* 100bit*150bit == 100cm*150cm
                                                        x is width : MAP_MAX_X   
                                                        y is height: MAP_MAY_Y   
                                                        we access the array by coordinated(x,y)*/

/* Robot position : */
#define DISTANCE_PER_MOVE 5
int x_position = 0;
int y_position = 0;
double current_angle = 0;

/* Motor */
// ---------------- define pins -----------------
// commented these pins as they are now in PID_motors.h file
// #define enA 10
// #define in1 7
// #define in2 6
// #define enB 9
// #define in3 5
// #define in4 4
// ---------------- algorithm includes -----------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define right 0
#define up 1
#define left 2
#define down 3
#define ROW 20
#define COL 20
#define oo 250
uint8_t Map[(int)((ROW * COL) / 8) + 5];
#define FREE_MAP 1
#define OBSTACLE_MAP 0
// ---------------- array of directions  -----------------
char s[200] = "";
int orientation = 3;
int i = 0;

typedef struct node
{
    uint8_t weight;
    uint16_t xy;
    //  4 free  2parent   10 x & y
    struct node *next;
} Node;
Node src, dest;

#line 74 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Delay_nonBlocking(int wnated_in_millis);
#line 82 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Transform(int dist, int sensorNumber);
#line 141 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void calculate_Distance(int duration, int sensor_num);
#line 154 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Read_ultrasonic();
#line 167 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int getx(struct node n);
#line 174 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int gety(struct node n);
#line 181 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int get_x_parent(struct node n);
#line 205 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int get_y_parent(struct node n);
#line 231 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void set_xy(struct node *n, int x, int y);
#line 237 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void set_xyp(struct node *n, int diff);
#line 246 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
uint8_t read_Map(uint8_t *map, uint8_t index_y, uint8_t index_x);
#line 257 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void write_Map(uint8_t *map, uint8_t index_y, uint8_t index_x, uint8_t state);
#line 274 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void forward_5cm();
#line 296 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void ninety_degrees_left();
#line 318 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void ninety_degrees_right();
#line 347 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
bool isEmpty_s();
#line 355 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
bool isFull();
#line 363 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
bool peek_s(struct node *num);
#line 374 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
bool pop_s(struct node *num);
#line 389 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
bool push_s(struct node num);
#line 402 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
Node* newNode(uint16_t data, uint8_t priority);
#line 413 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void pop(Node **head);
#line 420 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void push(Node **head, uint16_t data, uint8_t priority);
#line 456 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
Node* peek(Node **head);
#line 461 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int isEmpty(Node **head);
#line 467 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int isValid(int row, int col);
#line 472 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int compare(struct node node1, struct node node2);
#line 479 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int isUnBlocked(uint8_t *map, int row, int col);
#line 484 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int isDestination(int row, int col, struct node dest);
#line 492 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
int calculateHValue(int row, int col, struct node dest);
#line 498 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void tracePath(struct node cellDetails[][20], Node dest, Node src);
#line 573 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void aStarSearch(uint8_t *map, struct node src, struct node dest);
#line 778 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void setup();
#line 814 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void loop();
#line 1019 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void clear_path_s();
#line 74 "e:\\zzArduino\\MicroProcessors\\MicroProcessor-Smart-Chair\\Team mapping and localization\\Integration\\Map\\Map.ino"
void Delay_nonBlocking(int wnated_in_millis)
{
    int theTime = millis();
    while ((millis() - theTime) < wnated_in_millis)
        ;
}

/* to transform the distance into x , y (for the map indexing) position based on the sensor number */
void Transform(int dist, int sensorNumber)
{
    //90 degree= PI/2
    //180 degree = PI
    switch (orientation)
    {
    case 0:
        current_angle = -(PI / (float)2.0);
        break;
    case 1:
        current_angle = PI;
        break;
    case 2:
        current_angle = PI / (float)2.0;
        break;
    case 3:
        current_angle = 0;
        break;
    default:
        break;
    }
    x_position = getx(src);
    y_position = gety(src);
    int x, y;
    if (dist != -1 || dist == 0)
    {
        switch (sensorNumber)
        {
        case 0: /* front */
            x = x_position + dist * cos(current_angle);
            y = y_position + dist * sin(current_angle * (float)-1.0);
            break;

        case 1: /* right */
            x = x_position + dist * sin(current_angle * (float)-1.0);
            y = y_position - dist * cos(current_angle);
            break;

        case 2: /* left */

            x = x_position + dist * sin(current_angle);
            y = y_position + dist * cos(current_angle);
            break;
        }
        /* if x or y are < 0 , then point is out of the rang of the map */

        if (!(x < 0 || y < 0))
        {
            Serial.print("Writing now at (x,y)=");
            Serial.print(x);
            Serial.print(",");
            Serial.println(y);
            write_Map(Map, x, y, OBSTACLE_MAP);
        }
    }
    return;
}

/* convert the duration from the sensor -- > distance */
void calculate_Distance(int duration, int sensor_num)
{
    /*0.343 is the speed of the sound , dividing by 2 because this duration is for goes and return.*/
    distance = (duration * 0.0343) / 2.0;
    if (distance > 50)
    {
        distance = -1; //reading out of range
        return;
    }
    Transform((int)(distance / 5), sensor_num);
}

/* handle the readings from the 3 sensors simultaneously */
void Read_ultrasonic()
{
    digitalWrite(TrigerPins, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigerPins, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigerPins, LOW);
    duration = pulseIn(echos, HIGH);
    calculate_Distance(duration, 0);
    Serial.print("distance=");
    Serial.println(distance);
}
/* ------------------- Deal with bits of node -----------------*/
int getx(struct node n)
{
    uint16_t x = n.xy & 0b0000001111111111;
    x = x / COL;
    return x;
}

int gety(struct node n)
{
    uint16_t y = n.xy & 0b0000001111111111;
    y = y % COL;
    return y;
}

int get_x_parent(struct node n)
{
    int old_x = getx(n);

    int x = n.xy & 0b0000110000000000;
    x = x >> 10;
    if (x == 0)
    {
        return old_x;
    }
    else if (x == 1)
    {
        return old_x - 1;
    }
    else if (x == 2)
    {
        return old_x;
    }
    else
    {
        return old_x + 1;
    }
}

int get_y_parent(struct node n)
{
    int old_y = gety(n);

    int x = n.xy & 0b0000110000000000;

    x = x >> 10;

    if (x == 0)
    {
        return old_y + 1;
    }
    else if (x == 1)
    {
        return old_y;
    }
    else if (x == 2)
    {
        return old_y - 1;
    }
    else
    {
        return old_y;
    }
}

void set_xy(struct node *n, int x, int y)
{
    n->xy = n->xy & 0b1111110000000000;
    n->xy = (x * COL + y) | n->xy;
}

void set_xyp(struct node *n, int diff)
{

    diff = diff << 10;
    n->xy = n->xy & 0b1111001111111111;
    n->xy = n->xy | diff;
}

// ------------------- Deal with Bit array  -----------------
uint8_t read_Map(uint8_t *map, uint8_t index_y, uint8_t index_x) //function takes (x,y) and return the value of the bit wether it is 1 or 0
{
    // index= x+ width*y   source: https://softwareengineering.stackexchange.com/questions/212808/treating-a-1d-data-structure-as-2d-grid/212813
    //index_x = MAP_MAX_X - index_x;
    //index_y = MAP_MAX_Y - index_y;
    uint8_t index = index_x + COL * index_y;
    uint8_t bigIndex = index / 8;
    uint8_t shiftIndex = index % 8;
    return ((map[bigIndex] >> shiftIndex) & 0x01);
}

void write_Map(uint8_t *map, uint8_t index_y, uint8_t index_x, uint8_t state) // (x_position, y_postion, state to set the bit or clear it)
{
    //index_x = MAP_MAX_X - index_x;
    //index_y = MAP_MAX_Y - index_y;
    uint8_t index = index_x + COL * index_y;
    uint8_t bigIndex = index / 8;
    uint8_t shiftIndex = index % 8;
    if (state) //set bit
    {
        map[bigIndex] |= 1 << shiftIndex;
    }
    else // clear bit
    {
        map[bigIndex] &= ~(1 << shiftIndex);
    }
}

void forward_5cm()
{
    Serial.println("MOVING FORWARD NOW");
    Move(5, 0);
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, LOW);

    // digitalWrite(in3, HIGH);
    // digitalWrite(in4, LOW);

    // analogWrite(enA, 60);
    // analogWrite(enB, 60);

    // Delay_nonBlocking(180);
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, HIGH);
    // digitalWrite(in3, HIGH);
    // digitalWrite(in4, HIGH);
    // analogWrite(enA, 0);
    // analogWrite(enB, 0);
}

void ninety_degrees_left()
{
    Serial.println("MOVING LEFT NOW");
    RotateLeft();
    // digitalWrite(in1, LOW);
    // digitalWrite(in2, HIGH);

    // digitalWrite(in3, HIGH);
    // digitalWrite(in4, LOW);

    // analogWrite(enA, 80);
    // analogWrite(enB, 80);

    // Delay_nonBlocking(400);
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, HIGH);
    // digitalWrite(in3, HIGH);
    // digitalWrite(in4, HIGH);
    // analogWrite(enA, 0);
    // analogWrite(enB, 0);
}

void ninety_degrees_right()
{
    Serial.println("MOVING RIGHT NOW");
    RotateRight();
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, LOW);

    // digitalWrite(in3, LOW);
    // digitalWrite(in4, HIGH);

    // analogWrite(enA, 80);
    // analogWrite(enB, 80);

    // Delay_nonBlocking(350);
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, HIGH);
    // digitalWrite(in3, HIGH);
    // digitalWrite(in4, HIGH);
    // analogWrite(enA, 0);
    // analogWrite(enB, 0);
}

//bool x = 1;

// ------------------- Stack Data structure -----------------
const int MAX_SIZE = 90;
struct node stack[90];
int top = -1;

bool isEmpty_s()
{
    if (top == -1)
        return true;
    else
        return false;
}

bool isFull()
{
    if (top == MAX_SIZE)
        return true;
    else
        return false;
}

bool peek_s(struct node *num)
{
    if (!isEmpty_s())
    {
        *num = stack[top];
        return true;
    }
    else
        return false;
}

bool pop_s(struct node *num)
{

    if (!isEmpty_s())
    {
        num->xy = stack[top].xy;
        num->weight = stack[top].weight;

        top--;
        return true;
    }
    else
        return false;
}

bool push_s(struct node num)
{
    if (!isFull())
    {
        top++;
        stack[top] = num;
        return true;
    }
    else
        return false;
}

// ------------------- Set Data structure -----------------
Node *newNode(uint16_t data, uint8_t priority)
{

    Node *temp = (Node *)calloc(1, sizeof(struct node));
    temp->xy = data;
    temp->weight = priority;
    temp->next = NULL;

    return temp;
}

void pop(Node **head)
{
    Node *temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

void push(Node **head, uint16_t data, uint8_t priority)
{
    Node *start = (*head);

    // Create new Node
    Node *temp = newNode(data, priority);

    if (!start)
    {
        *head = temp;
        return;
    }

    if ((*head)->weight > priority)
    {
        temp->next = *head;
        (*head) = temp;
    }
    else
    {

        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
               start->next->weight <= priority)
        {
            start = start->next;
        }

        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}

Node *peek(Node **head)
{
    return (*head);
}

int isEmpty(Node **head)
{
    return (*head) == NULL;
}

// ------------------- Helping functions to A star algorithm  -----------------
int isValid(int row, int col)
{
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

int compare(struct node node1, struct node node2)
{
    if (node1.weight > node2.weight)
        return 1;
    return 0;
}

int isUnBlocked(uint8_t *map, int row, int col)
{ //  1 unblocked & 0 blocked
    return read_Map(map, row, col);
}

int isDestination(int row, int col, struct node dest)
{
    if (row == getx(dest) && col == gety(dest))
        return (1);
    else
        return (0);
}

int calculateHValue(int row, int col, struct node dest)
{
    //return round(sqrt(pow(row - getx(dest), 2) + pow(col - gety(dest), 2)));
    return abs(row - getx(dest)) + abs(col - gety(dest));
}
// ------------------- The main Algorithm   -----------------
void tracePath(struct node cellDetails[][COL], Node dest, Node src)
{

    //Serial.println("\nThe Path is ");
    int row = getx(dest);
    int col = gety(dest);

    int src_x = getx(src);
    int src_y = gety(src);

    while (row != src_x || col != src_y)
    {
        push_s(cellDetails[row][col]);
        int temp_row = get_x_parent(cellDetails[row][col]);
        int temp_col = get_y_parent(cellDetails[row][col]);

        row = temp_row;
        col = temp_col;
    }

    push_s(cellDetails[row][col]);
    int j = 0;
    Node curr;
    pop_s(&curr);

    //path[idx] = curr;
    //idx++; path[idx] = curr;
    //idx++;
    /*
  Serial.print("\n");
  Serial.print("( ") ; Serial.print(getx(curr)) ; 
  Serial.print(" ,  "); Serial.print( gety(curr));
  Serial.print(" -->  ");
  */
    while (!isEmpty_s())
    {

        pop_s(&curr);
        /*Serial.print("( ") ; Serial.print(getx(curr)) ; 
    Serial.print(" ,  "); Serial.print( gety(curr));
    Serial.print(" -->  ");
*/
        //path[idx] = curr;
        //idx++;
        int x_diff = getx(curr) - get_x_parent(curr);
        int y_diff = gety(curr) - get_y_parent(curr);

        if (x_diff == 1 && y_diff == 0)
        {
            s[j] = 'D';
        }
        else if (x_diff == -1 && y_diff == 0)
        {
            s[j] = 'U';
        }
        else if (x_diff == 0 && y_diff == 1)
        {
            s[j] = 'R';
        }
        else
        //if (x_diff == 0 && y_diff == -1)
        {
            s[j] = 'L';
        }

        j++;
    }

    s[j] = '\0';
    //Serial.print("\nDirections are -->\t ");
    //Serial.print(s);
    //Serial.print("\n");
    return;
}

void aStarSearch(uint8_t *map, struct node src, struct node dest)
{
    // If the source is out of range
    if (isValid(getx(src), gety(src)) == 0 ||
        isValid(getx(dest), gety(dest)) == 0)
    {
        Serial.println("Source or dest is invalid\n");
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(map, getx(src), gety(src)) == 0 || isUnBlocked(map, getx(dest), gety(dest)) == 0)
    {
        Serial.println("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(getx(src), gety(src), dest) == 1)
    {
        Serial.println("We are already at the destination\n");
        return;
    }

    uint8_t closedList[(int)(ROW * COL * 1.0 / 8) + 2];
    memset(closedList, 0, sizeof(closedList));

    Node cellDetails[ROW][COL];
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            cellDetails[i][j].weight = oo;
            cellDetails[i][j].xy = 0;
            cellDetails[i][j].next = NULL;
        }
    }

    // Initialising the parameters of the starting node
    i = getx(src), j = gety(src);
    cellDetails[i][j].weight = calculateHValue(i, j, dest);
    set_xy(&cellDetails[i][j], i, j);
    Node *openList = newNode(src.xy, src.weight);

    uint8_t foundDest = 0;

    while (!isEmpty(&openList))
    {

        Node p = *peek(&openList);
        pop(&openList);

        // Add this vertex to the closed list
        i = getx(p);
        j = gety(p);
        write_Map(closedList, i, j, 1);

        int fNew;
        //----------- 1st Successor (North) ------------

        if (isValid(i - 1, j) == 1)
        {

            if (isDestination(i - 1, j, dest) == 1)
            {
                set_xy(&cellDetails[i - 1][j], i - 1, j);
                set_xyp(&cellDetails[i - 1][j], down);
                Serial.println("The destination cell is found\n");
                tracePath(cellDetails, dest, src);
                foundDest = 1;
                return;
            }

            else if (read_Map(closedList, i - 1, j) == 0 && isUnBlocked(map, i - 1, j) == 1)
            {
                fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
                       calculateHValue(i - 1, j, dest);

                if (cellDetails[i - 1][j].weight == oo || cellDetails[i - 1][j].weight > fNew)
                {
                    // Update the details of this cell
                    cellDetails[i - 1][j].weight = fNew;
                    set_xyp(&cellDetails[i - 1][j], down);
                    set_xy(&cellDetails[i - 1][j], i - 1, j);

                    push(&openList, cellDetails[i - 1][j].xy, fNew);
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j) == 1)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j, dest) == 1)
            {

                set_xy(&cellDetails[i + 1][j], i + 1, j);
                set_xyp(&cellDetails[i + 1][j], up);
                Serial.println("The destination cell is found\n");
                tracePath(cellDetails, dest, src);
                foundDest = 1;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (read_Map(closedList, i + 1, j) == 0 && isUnBlocked(map, i + 1, j) == 1)
            {
                fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
                       calculateHValue(i + 1, j, dest);

                if (cellDetails[i + 1][j].weight == oo || cellDetails[i + 1][j].weight > fNew)
                {

                    // Update the details of this cell
                    cellDetails[i + 1][j].weight = fNew;
                    set_xyp(&cellDetails[i + 1][j], up);
                    set_xy(&cellDetails[i + 1][j], i + 1, j);

                    push(&openList, cellDetails[i + 1][j].xy, fNew);
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j + 1) == 1)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j + 1, dest) == 1)
            {

                set_xy(&cellDetails[i][j + 1], i, j + 1);
                set_xyp(&cellDetails[i][j + 1], left);
                Serial.println("The destination cell is found\n");
                tracePath(cellDetails, dest, src);
                foundDest = 1;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (read_Map(closedList, i, j + 1) == 0 && isUnBlocked(map, i, j + 1) == 1)
            {
                fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
                       calculateHValue(i, j + 1, dest);

                if (cellDetails[i][j + 1].weight == oo || cellDetails[i][j + 1].weight > fNew)
                {

                    cellDetails[i][j + 1].weight = fNew;
                    set_xy(&cellDetails[i][j + 1], i, j + 1);
                    set_xyp(&cellDetails[i][j + 1], left);
                    push(&openList, cellDetails[i][j + 1].xy, fNew);
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j - 1) == 1)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == 1)
            {
                set_xy(&cellDetails[i][j - 1], i, j - 1);
                set_xyp(&cellDetails[i][j - 1], right);
                Serial.println("The destination cell is found\n");
                tracePath(cellDetails, dest, src);
                foundDest = 1;
                return;
            }

            else if (read_Map(closedList, i, j - 1) == 0 && isUnBlocked(map, i, j - 1) == 1)
            {
                fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
                       calculateHValue(i, j - 1, dest);

                if (cellDetails[i][j - 1].weight == oo || cellDetails[i][j - 1].weight > fNew)
                {

                    cellDetails[i][j - 1].weight = fNew;
                    set_xyp(&cellDetails[i][j - 1], right);
                    set_xy(&cellDetails[i][j - 1], i, j - 1);
                    push(&openList, cellDetails[i][j - 1].xy, fNew);
                }
            }
        }
    }
    if (foundDest == 0)
        Serial.println("Failed to find the Destination Cell\n");

    return;
}

void setup()
{
    Serial.begin(9600);
    /*   for(byte i = 0 ; i<3 ; i++) {pinMode(TrigerPins[i], INPUT);}
  for(byte i = 0 ; i<3 ; i++) {pinMode(echos[i], INPUT);} */
    pinMode(TrigerPins, OUTPUT);
    digitalWrite(TrigerPins, LOW);
    pinMode(echos, INPUT);
    // ---------------motor pins--------------
    //in OID_motors.h file now
    // pinMode(enA, OUTPUT);
    // pinMode(enB, OUTPUT);
    // pinMode(in1, OUTPUT);
    // pinMode(in2, OUTPUT);
    // pinMode(in3, OUTPUT);
    // pinMode(in4, OUTPUT);
    // digitalWrite(in1, LOW);
    // digitalWrite(in2, LOW);
    // digitalWrite(in3, LOW);
    // digitalWrite(in4, LOW);
    // analogWrite(enA, 0);
    // analogWrite(enB, 0);
    motors_setup(); //function to setup motors
    {
        for (uint8_t i = 0; i < ROW; i++)
        {
            for (uint8_t j = 0; j < COL; j++)
            {
                write_Map(Map, i, j, FREE_MAP);
            }
        }
        //Serial.println("finished the MAAAAAP");
    }
    set_xy(&src, 0, 0);
}
//NewPing us(3, 4, 100);
void loop()
{
    clear_path_s();
    // read_Map(Map,0,3);
    Serial.print("Value of(0,3) (y,x)=");
    Serial.println(read_Map(Map, 0, 3));

    // int reading = us.ping_cm(); //take reading for ultrasonic
    // Serial.print("value of newping=");
    // Serial.println(reading);
    // //   Delay_nonBlocking(500);
    //   Serial.println("Entered the loop");
    Read_ultrasonic();
    //Delay_nonBlocking(500);
    //Serial.println("st loop ");

    //-------------------Write the map ----------------------------------
    // 1 --> free & 0 --> block

    //Serial.println("end writing to map");
    /*       write_Map(Map, 4, 3, 0);
      write_Map(Map, 1, 1, 0);
      write_Map(Map, 3, 3, 0);
      write_Map(Map, 2, 3, 0);
      write_Map(Map, 2, 4, 0);
      write_Map(Map, 2, 5, 0);
      write_Map(Map, 2, 6, 0);
      write_Map(Map, 3, 6, 0);
      write_Map(Map, 4, 6, 0);
      write_Map(Map, 9, 0, 0); */
    // int temp_now;
    // Serial.print("distnace=");
    // Serial.println(temp_now);
    // Serial.println("READ DIST \n\n");
    // ---------------------- Run  the algorithm  -----------------------
    //Serial.println("define nodes ");
    //Node dest;

    set_xy(&dest, 5, 5); //
    Serial.print("Src --> \t");
    Serial.print(getx(src));
    Serial.print("   ,   ");
    Serial.print(gety(src));
    Serial.print("\n");
    Serial.print("Dest--> \t");
    Serial.print(getx(dest));
    Serial.print("   ,   ");
    Serial.print(gety(dest));
    Serial.print("\n");

    aStarSearch(Map, src, dest);
    // ---------------------- Print the path -----------------------
    Serial.print("The path is -->  ");
    for (int i = 0; i < strlen(s); i++)
    {
        Serial.print(s[i]);
    }
    Serial.println("\nDone \n");

    //0 -> right
    // 1 ->up
    // 2 -> left
    // 3-> down

    for (int i = 0; i < 1; i++)
    {
        Serial.println("LET's MOOOOVE ");
        Delay_nonBlocking(500);
        if (s[i] == 'U')
        {
            if (orientation == 0)
            {
                Delay_nonBlocking(100);
                ninety_degrees_left();
                Delay_nonBlocking(100);
                forward_5cm();
            }
            else if (orientation == 1)
            {
                Delay_nonBlocking(100);
                forward_5cm();
            }
            else if (orientation == 2)
            {
                Delay_nonBlocking(100);
                ninety_degrees_right();
                Delay_nonBlocking(100);
                forward_5cm();
            }
            else
            {
                Delay_nonBlocking(100);
                ninety_degrees_right();
                Delay_nonBlocking(100);
                ninety_degrees_right();
                Delay_nonBlocking(100);

                forward_5cm();
            }
            orientation = 1;
            set_xy(&src, getx(src) - 1, gety(src));
        }
        else if (s[i] == 'R')
        {

            if (orientation == 0)
            {
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 1)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 2)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else
            {
                ninety_degrees_left();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            orientation = 0;
            set_xy(&src, getx(src), gety(src) + 1);
        }
        else if (s[i] == 'D')
        {

            if (orientation == 0)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 1)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 2)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_left();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else
            {
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            orientation = 3;
            set_xy(&src, getx(src) + 1, gety(src));
        }
        else if (s[i] == 'L')
        {

            if (orientation == 0)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                ninety_degrees_right();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 1)
            {
                Delay_nonBlocking(2000);
                ninety_degrees_left();
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else if (orientation == 2)
            {
                Delay_nonBlocking(2000);
                forward_5cm();
            }
            else
            {
                Delay_nonBlocking(100);
                ninety_degrees_right();
                Delay_nonBlocking(100);
                forward_5cm();
            }
            orientation = 2;
            set_xy(&src, getx(src), gety(src) - 1);
        }
    }
}
void clear_path_s()
{
    for (int i = 0; i < 200; i++)
    {
        s[i] = '\0';
    }
}

