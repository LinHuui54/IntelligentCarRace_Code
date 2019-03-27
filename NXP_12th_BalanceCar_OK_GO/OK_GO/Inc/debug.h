#ifndef DEBUG_H_
#define DEBUG_H_

extern float P_Speed;
extern float I_Speed;
extern float Set_angle;
extern float Turnning_P;
extern float Turnning_D;
extern float Target_speed1;
extern float P_ANGLE;        
extern float D_ANGLE;
extern float stop_car;
extern float little_s;
extern uint8 buff[DATA_PACKET];                        //定义接收缓冲区
extern float accelerate;
extern float slow_down;

//扫描方式
enum debug_enum
{
  P_Speed_debug,
  I_Speed_debug,
  Set_angle_debug,
  Turnning_P_debug,
  Turnning_D_debug,
  Target_speed1_debug,
  P_ANGLE_debug,        
  D_ANGLE_debug,
};

void DDEBUGH();

#endif