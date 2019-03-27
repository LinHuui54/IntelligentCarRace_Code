#ifndef _CONTROL_H
#define _CONTROL_H

#define XOUT    ADC0_SE8
#define YOUT    ADC0_SE16
#define ZOUT    ADC0_SE18

#define Gyro1   ADC1_SE16
#define Gyro2   ADC1_DP0
#define Ang     ADC0_SE18

/***********************直立控制参数********************/
#define MMA7361_vertical             1620    // 1620// 1760  //1850// 2600//1680
#define GYRO_VAL                     0       //0   //陀螺仪中值        //加大向后，减少向前
#define Gyro_ratio                   0.55    //0.55
#define GRAVITY_ADJUST_TIME_CONSTANT 2
#define DT                           0.001
#define MMA7361_ratio                0.0895//1150    
/*#define P_ANGLE               100  //10         //7
#define D_ANGLE               2 */  //0.17       //0.12

 //反复调整以上5个参数，以及机械结构
 
#define MOTOR_DEAD_VAL_L  15    //  死区电压
#define MOTOR_DEAD_VAL_R  15





/***********************函数声明********************/
 extern   void Rd_Ad_Value(void);                              //AD采集
 extern   void AD_Calculate(void);                              //AD采集和计算
 extern   int balance_Speed_Calculate(float angle,float angle_dot);
 static   unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
 extern   void OutPut_Data(void);                              //SCI采参数
 void PID_Speed();
 extern int Derror;
 
 typedef struct 
{
      float error_sum;
      float L;
      float R;
      float error;
      float last;
      float Speed;
}encoder_e;

struct PID_e
{
    float Balance;
    float Speed;
    float Direction;
}; 

#endif