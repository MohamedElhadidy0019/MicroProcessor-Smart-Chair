// ---------------- define pins -----------------
#define enA 10
#define in1 7
#define in2 6
#define enB 9
#define in3 5
#define in4 4
// ---------------- algorithm includes -----------------
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#define right 0
#define up 1
#define left 2
#define down 3
#define ROW 20
#define COL 20
#define oo 250
// ---------------- array of directions  -----------------
char s [200] = "" ;
int orientation = 1 ;
int i = 0 ;

typedef struct node {
  uint8_t weight;
  uint16_t xy;
  //  4 free  2parent   10 x & y
  struct node* next;
} Node;

 Node src, dest;
 
// ------------------- Deal with bits of node -----------------
int getx(struct node n) {
  uint16_t x = n.xy & 0b0000001111111111;
  x = x / COL;
  return x;
}
int gety(struct node n) {
  uint16_t y = n.xy & 0b0000001111111111;
  y = y % COL;
  return y;
}
int get_x_parent(struct node n){
  int old_x = getx(n);

  int x = n.xy & 0b0000110000000000;
  x = x >> 10;
  if (x == 0){
    return old_x;
  }
  else if (x == 1){
    return old_x - 1;
  }
  else if (x == 2){
    return old_x;
  }
  else{
    return old_x + 1;
  }
}
int get_y_parent(struct node n) {
  int old_y = gety(n);

  int x = n.xy & 0b0000110000000000;

  x = x >> 10;

  if (x == 0) {
    return old_y + 1;
  }
  else if (x == 1){
    return old_y;
  }
  else if (x == 2){
    return old_y - 1;
  }
  else{
    return old_y;
  }
}
void set_xy(struct node* n, int x, int y) {
  n->xy = n->xy & 0b1111110000000000;
  n->xy = (x * COL + y) | n->xy;
}
void set_xyp(struct node* n, int diff) {

  diff = diff << 10;
  n->xy = n->xy & 0b1111001111111111;
  n->xy = n->xy | diff;
}


// ------------------- Deal with Bit array  -----------------
uint8_t read_Map(uint8_t* map, uint8_t index_y, uint8_t index_x) //function takes (x,y) and return the value of the bit wether it is 1 or 0
{
  // index= x+ width*y   source: https://softwareengineering.stackexchange.com/questions/212808/treating-a-1d-data-structure-as-2d-grid/212813
  //index_x = MAP_MAX_X - index_x;
  //index_y = MAP_MAX_Y - index_y;
  uint8_t index = index_x + COL * index_y;
  uint8_t bigIndex = index / 8;
  uint8_t shiftIndex = index % 8;
  return((map[bigIndex] >> shiftIndex) & 0x01);
}
void write_Map(uint8_t* map, uint8_t index_y, uint8_t index_x, uint8_t state)  // (x_position, y_postion, state to set the bit or clear it)
{
  //index_x = MAP_MAX_X - index_x;
  //index_y = MAP_MAX_Y - index_y;
  uint8_t index = index_x + COL * index_y;
  uint8_t bigIndex = index / 8;
  uint8_t shiftIndex = index % 8;
  if (state)    //set bit
  {
    map[bigIndex] |= 1 << shiftIndex;
  }
  else    // clear bit
  {
    map[bigIndex] &= ~(1 << shiftIndex);
  }

}

void aStarSearch(uint8_t* map, struct node src, struct node dest); // implemenation is below
void setup()
{
    Serial.begin(9600);
// ---------------motor pins--------------
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
// --------------------------------------
}

void forward_5cm()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 60);
    analogWrite(enB, 60);

    delay(180);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

void ninety_degrees_left()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 80);
    analogWrite(enB, 80);

    delay(400);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}
void backward_5cm()
{
}

void ninety_degrees_right()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(enA, 80);
    analogWrite(enB, 80);

    delay(350);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

bool x =1 ;
void loop()
{    
    if(x ==1 ) {
      //Serial.println("st loop ");

      //-------------------Write the map ----------------------------------
      uint8_t Map[(int)(ROW * COL / 8) + 5];
      // 1 --> free & 0 --> block 
      for (int i = 0; i < ROW; i++)
      {
        for (int j = 0; j < COL; j++)
        {
          write_Map(Map, i, j, 1);
        }
      }
      //Serial.println("end writing to map");
      write_Map(Map, 4, 3, 0);
      write_Map(Map, 1, 1, 0);
      write_Map(Map, 3, 3, 0);
      write_Map(Map, 2, 3, 0);
      write_Map(Map, 2, 4, 0);
      write_Map(Map, 2, 5, 0);
      write_Map(Map, 2, 6, 0);
      write_Map(Map, 3, 6, 0);
      write_Map(Map, 4, 6, 0);
      write_Map(Map, 9, 0, 0);
    // ---------------------- Run  the algorithm  -----------------------
      //Serial.println("define nodes ");
     
      set_xy(&src, 0, 0); 
      set_xy(&dest, 10 , 6);
      Serial.print("Src --> \t"); Serial.print(getx(src)); 
      Serial.print("   ,   "); Serial.print(gety(src)); 
      Serial.print("\n");
      Serial.print("Dest--> \t"); Serial.print(getx(dest)); 
      Serial.print("   ,   "); Serial.print(gety(dest)); 
      Serial.print("\n");

      
      aStarSearch(Map, src, dest);
      // ---------------------- Print the path -----------------------
      Serial.print("The path is -->  ");
      for(int i=0 ; i < strlen(s) ; i++ ) {
        Serial.print(s[i]);  
      }
      x=0;
      Serial.println("\nDone \n" );
    }
    else {
     x=0;
    }
    
    for(i =0 ; i<strlen(s) ; i++)
    {
     if(s[i] == 'U')
     {
                  if(orientation == 0)
                  {
                    delay(2000);
                    ninety_degrees_left();
                    delay(2000);
                    forward_5cm(); 
                  }      
                  else if (orientation == 1)
                  {
                    delay(2000);
                    forward_5cm();         
                  }
                  else if (orientation == 2)
                  {
                    delay(2000);
                    ninety_degrees_right();
                    delay(2000);
                    forward_5cm();        
                  }
                  else 
                  {
                    delay(2000);
                    ninety_degrees_right();
                    delay(2000);
                    ninety_degrees_right();
                    delay(2000);
                    forward_5cm();        
                  }
                 orientation = 1 ;
                 set_xy(&src, getx(src)-1, gety(src));                    


     }
     else if (s[i] == 'R')
     {
                
                if(orientation == 0)
                {        
                  delay(2000);
                  forward_5cm(); 
                }      
                else if (orientation == 1)
                {
                  delay(2000);
                  ninety_degrees_right();
                  delay(2000);
                  forward_5cm();         
                }
                else if (orientation == 2)
                {
                  delay(2000);
                  ninety_degrees_right();
                  delay(2000);
                  ninety_degrees_right();
                  delay(2000);
                  forward_5cm();        
                }
                else 
                {        
                  ninety_degrees_left();
                  delay(2000);
                  forward_5cm();        
                }
               orientation = 0 ;
               set_xy(&src, getx(src), gety(src)+1);               
    
     }
     else if (s[i] == 'D')
     {
      

              if(orientation == 0)
              {
                delay(2000);
                ninety_degrees_right();        
                delay(2000);
                forward_5cm(); 
              }      
              else if (orientation == 1)
              {
                delay(2000);
                ninety_degrees_right();
                delay(2000);
                ninety_degrees_right();
                delay(2000);
                forward_5cm();         
              }
              else if (orientation == 2)
              {
                delay(2000);
                ninety_degrees_left();
                delay(2000);
                forward_5cm();        
              }
              else 
              {               
                delay(2000);
                forward_5cm();        
              }
             orientation = 3 ;                   
             set_xy(&src, getx(src)+1, gety(src));                    

                    
     }
     else if (s[i] == 'L')
     {

              if(orientation == 0)
              {
                delay(2000);
                ninety_degrees_right();
                delay(2000);
                ninety_degrees_right();
                delay(2000);
                forward_5cm(); 
              }      
              else if (orientation == 1)
              {
                delay(2000);
                ninety_degrees_left();       
                delay(2000);
                forward_5cm();  
              }
              else if (orientation == 2)
              {                    
                delay(2000);
                forward_5cm();        
              }
              else 
              {               
                delay(2000);
                ninety_degrees_right();
                delay(2000);
                forward_5cm();        
              }
              orientation = 2 ;
              set_xy(&src, getx(src), gety(src)-1);                    

     
     }
     
    }
  
}


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
bool peek_s(struct node* num)
{
  if (!isEmpty_s())
  {
    *num = stack[top];
    return true;
  }
  else
    return false;

}
bool pop_s(struct node* num) {

  if (!isEmpty_s()) {
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
Node* newNode(uint16_t data, uint8_t priority) {

  Node* temp = (Node*)calloc(1, sizeof(struct node));
  temp->xy = data;
  temp->weight = priority;
  temp->next = NULL;

  return temp;
}
void pop(Node** head) {
  Node* temp = *head;
  (*head) = (*head)->next;
  free(temp);
}
void push(Node** head, uint16_t data, uint8_t priority) {
  Node* start = (*head);

  // Create new Node
  Node* temp = newNode(data, priority);

  if (!start) {
    *head = temp;
    return;
  }

  if ((*head)->weight > priority) {
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
Node* peek(Node** head) {
  return (*head);
}
int isEmpty(Node** head)
{
  return (*head) == NULL;
}


// ------------------- Helping functions to A star algorithm  -----------------
int isValid(int row, int col) {
  return (row >= 0) && (row < ROW) && (col >= 0)
    && (col < COL);
}
int compare(struct node  node1, struct node node2)
{
  if (node1.weight > node2.weight)
    return 1;
  return 0;
}
int isUnBlocked(uint8_t* map, int row, int col) { //  1 unblocked & 0 blocked
  return read_Map(map, row, col);
}
int isDestination(int row, int col, struct node dest) {
  if (row == getx(dest) && col == gety(dest))
    return (1);
  else
    return (0);
}
int calculateHValue(int row, int col, struct node dest) {
  //return round(sqrt(pow(row - getx(dest), 2) + pow(col - gety(dest), 2)));
  return abs(row - getx(dest)) + abs(col - gety(dest));
}
// ------------------- The main Algorithm   -----------------
void tracePath(struct node cellDetails[][COL], Node dest, Node src) {

  //Serial.println("\nThe Path is ");
  int row = getx(dest);
  int col = gety(dest);

  int src_x = getx(src);
  int src_y = gety(src);


  while (row != src_x || col != src_y) {
    push_s(cellDetails[row][col]);
    int temp_row = get_x_parent(cellDetails[row][col]);
    int temp_col = get_y_parent(cellDetails[row][col]);

    row = temp_row;
    col = temp_col;
  }

  push_s(cellDetails[row][col]);
  int j = 0;
  Node  curr;
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
  while (!isEmpty_s()) {

    pop_s(&curr);
    /*Serial.print("( ") ; Serial.print(getx(curr)) ; 
    Serial.print(" ,  "); Serial.print( gety(curr));
    Serial.print(" -->  ");
*/
    //path[idx] = curr;
    //idx++;
    int x_diff = getx(curr) - get_x_parent(curr);
    int y_diff = gety(curr) - get_y_parent(curr);

    if (x_diff == 1 && y_diff == 0) {
      s[j] = 'D';
    }
    else if (x_diff == -1 && y_diff == 0) {
      s[j] = 'U';
    }
    else if (x_diff == 0 && y_diff == 1){
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
void aStarSearch(uint8_t* map, struct node src, struct node dest) {
  // If the source is out of range
  if (isValid(getx(src), gety(src)) == 0 ||
    isValid(getx(dest), gety(dest)) == 0) {
    Serial.println("Source or dest is invalid\n");
    return;
  }

  // Either the source or the destination is blocked
  if (isUnBlocked(map, getx(src), gety(src)) == 0
    || isUnBlocked(map, getx(dest), gety(dest))
    == 0) {
    Serial.println("Source or the destination is blocked\n");
    return;
  }

  // If the destination cell is the same as source cell
  if (isDestination(getx(src), gety(src), dest)
    == 1) {
    Serial.println("We are already at the destination\n");
    return;
  }

  uint8_t closedList[(int)(ROW * COL * 1.0 / 8) + 2];
  memset(closedList, 0, sizeof(closedList));

  Node cellDetails[ROW][COL];
  int i, j;
  for (i = 0; i < ROW; i++) {
    for (j = 0; j < COL; j++) {
      cellDetails[i][j].weight = oo;
      cellDetails[i][j].xy = 0;
      cellDetails[i][j].next = NULL;
    }
  }

  // Initialising the parameters of the starting node
  i = getx(src), j = gety(src);
  cellDetails[i][j].weight = calculateHValue(i, j, dest);
  set_xy(&cellDetails[i][j], i, j);
  Node* openList = newNode(src.xy, src.weight);

  uint8_t foundDest = 0;

  while (!isEmpty(&openList)) {

    Node p = *peek(&openList);
    pop(&openList);

    // Add this vertex to the closed list
    i = getx(p);
    j = gety(p);
    write_Map(closedList, i, j, 1);

    int  fNew;
    //----------- 1st Successor (North) ------------

    if (isValid(i - 1, j) == 1) {

      if (isDestination(i - 1, j, dest) == 1) {
        set_xy(&cellDetails[i - 1][j], i - 1, j);
        set_xyp(&cellDetails[i - 1][j], down);
        Serial.println("The destination cell is found\n");
        tracePath(cellDetails, dest, src);
        foundDest = 1;
        return;
      }

      else if (read_Map(closedList, i - 1, j) == 0
        && isUnBlocked(map, i - 1, j)
        == 1) {
        fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
          calculateHValue(i - 1, j, dest);

        if (cellDetails[i - 1][j].weight == oo
          || cellDetails[i - 1][j].weight > fNew) {
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
    if (isValid(i + 1, j) == 1) {
      // If the destination cell is the same as the
      // current successor
      if (isDestination(i + 1, j, dest) == 1) {

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
      else if (read_Map(closedList, i + 1, j) == 0
        && isUnBlocked(map, i + 1, j)
        == 1) {
        fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
          calculateHValue(i + 1, j, dest);

        if (cellDetails[i + 1][j].weight == oo
          || cellDetails[i + 1][j].weight > fNew) {


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
    if (isValid(i, j + 1) == 1) {
      // If the destination cell is the same as the
      // current successor
      if (isDestination(i, j + 1, dest) == 1) {

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
      else if (read_Map(closedList, i, j + 1) == 0
        && isUnBlocked(map, i, j + 1)
        == 1) {
        fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
          calculateHValue(i, j + 1, dest);


        if (cellDetails[i][j + 1].weight == oo
          || cellDetails[i][j + 1].weight > fNew) {

          cellDetails[i][j + 1].weight = fNew;
          set_xy(&cellDetails[i][j + 1], i, j + 1);
          set_xyp(&cellDetails[i][j + 1], left);
          push(&openList, cellDetails[i][j + 1].xy, fNew);
        }
      }
    }

    //----------- 4th Successor (West) ------------

    // Only process this cell if this is a valid one
    if (isValid(i, j - 1) == 1) {
      // If the destination cell is the same as the
      // current successor
      if (isDestination(i, j - 1, dest) == 1) {
        set_xy(&cellDetails[i][j - 1], i, j - 1);
        set_xyp(&cellDetails[i][j - 1], right);
        Serial.println("The destination cell is found\n");
        tracePath(cellDetails, dest, src);
        foundDest = 1;
        return;
      }

      else if (read_Map(closedList, i, j - 1) == 0
        && isUnBlocked(map, i, j - 1)
        == 1) {
        fNew = cellDetails[i][j].weight - calculateHValue(i, j, dest) + 1 +
          calculateHValue(i, j - 1, dest);

        if (cellDetails[i][j - 1].weight == oo
          || cellDetails[i][j - 1].weight > fNew) {

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
