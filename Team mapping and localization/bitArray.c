//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>


#define MAP_MAX_X 32
#define MAP_MAX_Y 32
uint8_t Map[128];  // 100bit*150bit == 100cm*150cm

// x is width MAP_MAX_X   is its size 
//y is height MAP_MAY_Y   is its size
// we access the array by coordinated(x,y)

uint8_t read_Map(uint8_t index_x, uint8_t index_y) //function takes (x,y) and return the value of the bit wether it is 1 or 0
{
    // index= x+ width*y   source: https://softwareengineering.stackexchange.com/questions/212808/treating-a-1d-data-structure-as-2d-grid/212813
    int index = index_y + MAP_MAX_Y * index_x;
    uint8_t bigIndex = index / 8;
    uint8_t shiftIndex = index % 8;
    return((Map[bigIndex] >> shiftIndex) & 0x01);
}
void write_Map(uint8_t index_x, uint8_t index_y, uint8_t state)  // (x_position, y_postion, state to set the bit or clear it)
{
    int index = index_y + MAP_MAX_Y * index_x;
    uint8_t bigIndex = index / 8;
    uint8_t shiftIndex = index % 8;
    if (state)    //set bit
    {
        Map[bigIndex] |= 1 << shiftIndex;
    }
    else    // clear bit 
    {
        Map[bigIndex] &= ~(1 << shiftIndex);
    }

}
int main(void)
{
    for (int i = 0; i < MAP_MAX_X; i++)
    {
        for (int j = 0; j < MAP_MAX_Y; j++)
        {
            write_Map(i, j, 1);
        }
    }
    Map[0] = 0;
    write_Map(5, 7, 0);
    write_Map(4, 2, 0);
    for (int i = 0; i < MAP_MAX_X; i++)
    {
        for (int j = 0; j < MAP_MAX_Y; j++)
        {
            printf("%d ", read_Map(i, j));
        }
        printf("\n");
    }
    /*read_Map(10, 11);
    read_Map(10, 10);*/
}



