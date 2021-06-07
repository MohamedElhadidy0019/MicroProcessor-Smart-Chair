#ifndef MAP_H
#define MAP_H
#include <Arduino.h>
#include "PID_motors.h"  
/*_____________________MACROS_____________________*/
#define PI (float)3.141
#define DISTANCE_PER_MOVE 5

#define right 0
#define up 1
#define left 2
#define down 3
#define ROW 24
#define COL 24
#define oo 1000
#define FREE_MAP 1
#define OBSTACLE_MAP 0

/*____________Structs____________________*/
typedef struct node
{
    uint8_t weight;
    uint16_t xy;
    //  4 free  2parent   10 x & y
    struct node *next;
} Node;

/*____need to call this function in the void setup() in the .ino file___*/

void Delay_nonBlocking(int wnated_in_millis);
void Transform(int dist, int sensorNumber);
void calculate_Distance(int duration, int sensor_num);
void Read_ultrasonic();

/* ------------------- Deal with bits of node -----------------*/
int getx(struct node n);
int gety(struct node n);
int get_x_parent(struct node n);
int get_y_parent(struct node n);
void set_xy(struct node *n, int x, int y);
void set_xyp(struct node *n, int diff);
// ------------------- Deal with Bit array  -----------------

uint8_t read_Map(uint8_t *Map_local, uint8_t index_x, uint8_t index_y); 
void write_Map(uint8_t *Map_local, uint8_t index_x, uint8_t index_y, uint8_t state);
void forward_5cm();
void ninety_degrees_left();
void ninety_degrees_right();
// ------------------- Stack Data structure -----------------

bool isEmpty_s();
bool isFull();
bool peek_s(struct node *num);
bool pop_s(struct node *num);
bool push_s(struct node num);

// ------------------- Set Data structure -----------------
Node *newNode(uint16_t data, uint8_t priority);
void pop(Node **head);
void push(Node **head, uint16_t data, uint8_t priority);
Node *peek(Node **head);
int isEmpty(Node **head);
// ------------------- Helping functions to A star algorithm  -----------------
int isValid(int row, int col);
int compare(struct node node1, struct node node2);
int isUnBlocked(uint8_t *map, int row, int col);
int isDestination(int row, int col, struct node dest);
int calculateHValue(int row, int col, struct node dest);
// ------------------- The main Algorithm   -----------------
void tracePath(struct node cellDetails[][COL], Node dest, Node src);
void aStarSearch(uint8_t *map, struct node src, struct node dest);

void Mapping_setup();
void Mapping_unsetup();
void Mapping_loop();

#endif