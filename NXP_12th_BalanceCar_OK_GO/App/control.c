#include "include.h"


#define P_Direction   2.1//2.2    //2.9
#define D_Direction   0.07//0.09   //0.08           

float OutData[5] = { 0 };                                    //SCI示波器参数
float Gyro_Now,angle_offset_vertical;  //陀螺仪转化后的角速度，转化后的加速度角度
float g_fCarAngle,g_fGyroscopeAngleIntegral; //融合后的角度
volatile float R_Balance_Speed,L_Balance_Speed;      //平衡所需要速度
volatile int     MMA7361 ,L3G,real_angle;                             //加速度计AD ,陀螺仪AD，模块输出的角度
//********************************************************
/*#define Target_speed1  170
#define P_Speed        1.5
#define I_Speed        2.7
#define Set_angle      10*/
uint16 Go_time;
uint16 time_speed=0;
int16 All_speed=0;
encoder_e encoder;       //编码器结构体
float speed_temp;
struct PID_e PID;       //定义一个PID机构体
extern float error_sum;
int accelerate_time=0;
//**************************************************************************
/*
*  功能说明：AD采集
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
void Rd_Ad_Value(void)
{

    MMA7361 = adc_once(ZOUT, ADC_12bit);   //Z
    //L3G= - GetData_l3g4200d(0x2a);           //0x2a是Y轴  0x28是x轴 // gyro1
    
    //由于使用软件滤波，因此不再用 硬件融合角度
    // real_angle = adc_once(Ang,ADC_12bit); //ang
}

//**************************************************************************
/*
*  功能说明：SCI示波器CRC校验
内部调用函数
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//************************************************
//
/*
*  功能说明：SCI示波器发送函数

*  参数说明：
OutData[]  需要发送的数值赋予该数组
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//****************************************************
void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {

        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    for(i=0;i<10;i++)
    {
        uart_putchar (UART4,(char)databuf[i]);
    }
}
//**************************************************************************
//   Kalman滤波
//**************************************************************************

float anglee, angle_dot;         //外部需要引用的变量
//-------------------------------------------------------
// 0.00015     //0.0001
const float Q_angle=0.001, Q_gyro=0.003, R_angle=0.5, dt=0.001;
//0.0001         //0.00015        //1.2
//注意：dt的取值为kalman滤波器采样时间;         //0.8
static float P[2][2] = {
    { 1, 0 },
    { 0, 1 }
};

static float Pdot[4] ={0,0,0,0};

static const char C_0 = 1;

static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)          //gyro_m:gyro_measure
{
    anglee+=(gyro_m-q_bias) * dt;
    
    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1]=- P[1][1];
    Pdot[2]=- P[1][1];
    Pdot[3]=Q_gyro;
    
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
    
    
    angle_err = angle_m - anglee;
    
    

    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    
    E = R_angle + C_0 * PCt_0;
    
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    
    
    anglee   += K_0 * angle_err;
    q_bias  += K_1 * angle_err;
    angle_dot = gyro_m-q_bias;
}
//**************************************************************************
//   清华角度滤波方案
//*************************************************************************
/*
*  功能说明：清华角度滤波
*  参数说明：G_angle                       加速度计角度0-90内
*            Gyro                         陀螺仪角速度转花后的数值
*            GRAVITY_ADJUST_TIME_CONSTANT  时间校正系数
DT                             定时器时间 单位s
*  函数返回：无符号结果值
*  修改时间：2013-2-10
* 备注：参考清华源码
*/
//
//*************************************************************************
void QingHua_AngleCalaulate(float G_angle,float Gyro)
{
    float fDeltaValue;

    g_fCarAngle = g_fGyroscopeAngleIntegral;   //最终融合角度
    fDeltaValue = (G_angle - g_fCarAngle) / GRAVITY_ADJUST_TIME_CONSTANT;  //时间系数矫正
    g_fGyroscopeAngleIntegral += (Gyro + fDeltaValue) * DT;                //融合角度
}
//**************************************************************************
/*
*  功能说明：直立角度计算
*  参数说明：

*  函数返回：无符号结果值
*  修改时间：2013-2-10
* 备注：参考清华源码
*/
//**************************************************************************
void AD_Calculate(void)
{
    /*static uint16 i=0;
    Rd_Ad_Value();                          //采集 AD


    //Gyro_Now = -(GYRO_VAL - L3G ) * Gyro_ratio;                            //陀螺仪采集到的角速度归一化
    GYRO_Y=-Gyro_ratio*GYRO_Y;
    //GYRO_Y=-0.0175*GYRO_Y;
    angle_offset_vertical =(MMA7361_vertical - MMA7361) * MMA7361_ratio ;  //将加速度计采集到的角度归一化，乘上0.375是为了归一化到0~90°
    if(i==0)
    {
    //g_fCarAngle=angle_offset_vertical;
    g_fGyroscopeAngleIntegral=angle_offset_vertical;
    i=1;
    }
    

    if(angle_offset_vertical > 90)angle_offset_vertical = 90;               //防止加速度角度溢出
    if(angle_offset_vertical < -90)angle_offset_vertical = -90;

    //计算融合后的角度
    //QingHua_AngleCalaulate(angle_offset_vertical,GYRO_Y);                 //清华角度滤波方案
     Kalman_Filter(angle_offset_vertical,GYRO_Y);*/
}

/***********************************************************************/
/*
*  功能说明：直立速度计算
*  参数说明：angle                融合后最终角度
*            angle_dot            陀螺仪角速度
*
*  函数返回：无符号结果值
*  修改时间：2013-2-10
* 备注：参考清华源码
*/
/******************************************************************************/
int balance_Speed_Calculate(float angle,float angle_dot)
{
    float Start;
    Start = (angle+Set_angle) * P_ANGLE  + angle_dot * D_ANGLE ;  //直立时所要的速度 Set_angle
    
    /***********************将最大速度限制在985个PWM内******************************/  
    return (int)Start;
}

/***********************************************************************/
/*
*  功能说明：速度环
*  参数说明：
*
*  函数返回：无符号结果值
*  修改时间：2013-2-10
* 备注
*/
/******************************************************************************/
void PID_Speed()
{
      uint8 time_flag;
      float temp;
      //static uint8 accelerate_flag=0;
      //static uint8 test=0;
      //static uint8 test1=0;
      
      time_speed++;
      
      if(time_speed>=10)
      {
          time_speed=0;time_flag=1;
      }
      else
          time_flag=0;
    
       Go_time++;
       if(Go_time>1000)
          Go_time=1001;
       if(Go_time>1000)
         {
           /* if(accelerate_flag&&accelerate_time<700)
            {
               accelerate_time++;
               if(accelerate_time<700)
                 I_Speed=2;
               else
               {
                 if(I_Speed>1.4)
                   I_Speed-=0.01;
               }
            } 
            else if(accelerate_time>=700)
            {
              if(!test1)
              {
               beep();
               test1=1;
              }
               I_Speed=1.4;
            }*/
            if(time_flag&&(All_speed<Target_speed1))  
            {
                All_speed = All_speed+4; //4
            } 
           /* if(time_flag&&(All_speed>Target_speed1))  
            {
                All_speed = All_speed-5; 
                if(All_speed<10)
                  All_speed=10;
            } */
         }
       
      /* if(All_speed>(Target_speed1)-20)
       {
         if(!test)
         {
           beep();
           test=1;
         }
         accelerate_flag=1;
       }*/
   
       temp = encoder.Speed-All_speed;  
       
       //speed_temp=speed_temp*0.97+temp*0.05;
       
       PID.Speed =temp*P_Speed + error_sum*I_Speed;             
}



