#include <i2cmaster.h>

/////////////////////////////////
/*
请先使用USB转TTL模块设置好IIC模式
GYTOF10M-----mini
VCC----VCC
SCL----A5
SDA----A4
GND--GND
*/
/////////////////////////////////

//////////////////////////////////
#define uint16_t unsigned int
#define iic_add  0xa4
typedef struct
{
    float distance;
    float amp;
    float temp;
  
} gyir;
unsigned char Re_buf;
unsigned char sign=0;
gyir my_ir;
uint16_t delay_t=0;


void setup() {
      byte td=0;
       Serial.begin(115200);
       i2c_init();
       iic_read(0x02,&td,1);
       switch(td)
       {
         case 0:delay_t=1010;break;
         case 1:delay_t=110;break;
         case 2:delay_t=32;break;
         case 3:delay_t=12;break;
         case 4:delay_t=7;break;
       }
        delay(delay_t); 
}
void loop() {
  unsigned char data[16]={0};

 
   iic_read(0x08,data,5);
   my_ir.distance=(data[0]<<8)|data[1];
   my_ir.amp=(data[2]<<8)|data[3];
   my_ir.temp=data[4];

    Serial.print("distance: ");
   Serial.print(my_ir.distance/10);
   Serial.print(",amp: ");
   Serial.print( my_ir.amp);
    Serial.print(",temp:");
    Serial.println(my_ir.temp);
   
 
 
   delay(delay_t); 
}
void iic_read(unsigned char add,unsigned char *data,unsigned char len)
{
  i2c_start_wait(iic_add);
   i2c_write(add);
   i2c_start_wait(iic_add+1);
   while(len-1)
   {
    *data++=i2c_readAck();
    len--;
    }
    *data=i2c_readNak();
    i2c_stop();
}
void iic_write( char add,unsigned char data)
{
  i2c_start_wait(iic_add);
   i2c_write(add);
   i2c_write(data);
    i2c_stop();
}

