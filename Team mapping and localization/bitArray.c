#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>

uint8_t arr[2];
uint8_t read_arr(uint8_t index)
{
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    return((arr[bigIndex]>>shiftIndex)&0x01);
}
void write_arr(uint8_t index,uint8_t state)
{
    uint8_t bigIndex=index/8;
    uint8_t shiftIndex=index%8;
    if(state)    //set bit
    {
        arr[bigIndex]|=1<<shiftIndex;
    }
    else    // clear bit 
    {
        arr[bigIndex]&=~1<<shiftIndex;
    }

}
int main(void)
{
    for(int i=0;i<4;i++)
    {
        write_arr(i,1);
    }
    for(int i=8;i<16;i++)
    {
        write_arr(i,1);

    }
    //write_arr(7,1);
    for(int i=0;i<16;i++)
    {
        printf("inedx[%d]=%d\n",i,read_arr(i));
    }
}



