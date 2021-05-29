//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>


#define MAP_MAX_X 100
#define MAP_MAX_Y 150
uint8_t Map[1875];  // 100bit*150bit == 100cm*150cm

// x is width MAP_MAX_X   is its size 
//y is height MAP_MAY_Y   is its size
// we access the array by coordinated(x,y)

uint8_t read_Map(uint8_t index_x,uint8_t index_y) //function takes (x,y) and return the value of the bit wether it is 1 or 0
{
    // index= x+ width*y   source: https://softwareengineering.stackexchange.com/questions/212808/treating-a-1d-data-structure-as-2d-grid/212813
    int index= index_x + MAP_MAX_X * index_y;
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    return((Map[bigIndex]>>shiftIndex)&0x01);
}
void write_Map(uint8_t index_x,uint8_t index_y,uint8_t state)  // (x_position, y_postion, state to set the bit or clear it)
{
    int index = index_x + MAP_MAX_X * index_y;
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    if(state)    //set bit
    {
        Map[bigIndex]|=1<<shiftIndex;
    }
    else    // clear bit 
    {
        Map[bigIndex]&=~1<<shiftIndex;
    }

}
int main(void)
{
    write_Map(10, 11, 1);
    printf("%d\n", read_Map(10, 11));
    printf("%d\n", read_Map(10, 10));
    /*read_Map(10, 11);
    read_Map(10, 10);*/
}



