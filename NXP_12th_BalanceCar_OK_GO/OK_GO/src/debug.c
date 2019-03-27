#include "include.h"

float P_ANGLE=140;//140        
float D_ANGLE=3.2;//3
float P_Speed=1;//0.9
float I_Speed=2;//1.5//起步是2，起步后是1.5
float Turnning_P=30;     //25//27.5//30
float Turnning_D=1.22;      //1//1.1//1.2
float Set_angle=6;       //5
float Target_speed1=300;//255//300
float stop_car=0;
float little_s=1;
float accelerate=650;
float slow_down=50;

void DDEBUG()
{
  //P_ANGLE=(float)buff[0];
  //D_ANGLE=((float)(buff[1]))/10;
  //P_Speed=((float)(buff[2]))/10;
  //I_Speed=((float)(buff[3]))/10;
  //Turnning_P=(((float)(buff[4]))/10)+10;
  //Turnning_D=((float)(buff[5]))/10;
  //accelerate=((float)(buff[6]))*10;
  //slow_down=(float)buff[7];
  //Set_angle=((float)(buff[6]))/10;
  //Target_speed1=(float)buff[7];
  stop_car=(float)buff[8];
}