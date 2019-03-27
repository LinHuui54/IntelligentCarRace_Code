#ifndef _CONTROL_H
#define _CONTROL_H

#define XOUT    ADC0_SE8
#define YOUT    ADC0_SE16
#define ZOUT    ADC0_SE18

#define Gyro1   ADC1_SE16
#define Gyro2   ADC1_DP0
#define Ang     ADC0_SE18

/***********************ֱ�����Ʋ���********************/
#define MMA7361_vertical             1620    // 1620// 1760  //1850// 2600//1680
#define GYRO_VAL                     0       //0   //��������ֵ        //�Ӵ���󣬼�����ǰ
#define Gyro_ratio                   0.55    //0.55
#define GRAVITY_ADJUST_TIME_CONSTANT 2
#define DT                           0.001
#define MMA7361_ratio                0.0895//1150    
/*#define P_ANGLE               100  //10         //7
#define D_ANGLE               2 */  //0.17       //0.12

 //������������5���������Լ���е�ṹ
 
#define MOTOR_DEAD_VAL_L  15    //  ������ѹ
#define MOTOR_DEAD_VAL_R  15





/***********************��������********************/
 extern   void Rd_Ad_Value(void);                              //AD�ɼ�
 extern   void AD_Calculate(void);                              //AD�ɼ��ͼ���
 extern   int balance_Speed_Calculate(float angle,float angle_dot);
 static   unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
 extern   void OutPut_Data(void);                              //SCI�ɲ���
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