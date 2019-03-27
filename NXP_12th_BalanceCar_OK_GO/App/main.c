/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火K60 平台主程序
 * @author     野火科技
 * @version    v5.0
 * @date       2013-12-19
 */
#include "common.h"
#include "include.h"

//**************全局变量声明*********
uint8 imgbuff[CAMERA_SIZE]; //存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W]; //解压后接收图像的数组，用于图像处理                         
int left_border[CAMERA_H-10];//存储左边界
int right_border[CAMERA_H-10]; //存储右边界
int img_center[CAMERA_H-10];//存储中线
int Derror=0;
int left_speed=0,right_speed=0;                //左右轮的速度值
float str_error,last_str_error,str_sum_error,str_P_spped;
float last_PIspeed,now_PIspeed;
float speed_out=0;                               //输出直走速度
float Set_speed;
float error_sum;
int Turnning_speed;
//***遥控变量
uint8 buff[DATA_PACKET];                        //定义接收缓冲区
uint8 diglt_flag=0;
uint8 nrf_flag=0;
//***********
//**************变量外部调用声明*************
extern encoder_e encoder;       //编码器结构体
extern struct PID_e PID;
extern int16 All_speed;
extern float GYRO_Y;
extern float OutData[5];                              //SCI示波器参数
extern int8  left_tend;
extern int8  right_tend;
extern int8  left_max;
extern int8  right_max;
extern uint16  stop_count;
//**************函数声明**********************
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PORTE_IRQHandler(void);
void PIT0_IRQHandler(void);
void beep();                                          //蜂鸣器
void key_manage(uint8 i);
void show_data();
void display_waveform();
void nrf_control();
void key_init_init();
void data1();
void data2();
void data3();

void all_init()
{
    DisableInterrupts;//禁止总中断
    //设置中断优先级
    NVIC_SetPriorityGrouping(0);                            //配置中断优先级分组
    NVIC_SetPriority(PORTE_VECTORn,0);                     //场终端，必须是最高优先级，其它无所谓啦
    NVIC_SetPriority(PORTA_VECTORn,1);                     //场终端，必须是最高优先级，其它无所谓啦
    NVIC_SetPriority(PIT0_VECTORn,2);
    NVIC_SetPriority(DMA0_VECTORn,3);
    camera_init(imgbuff);                                   //摄像头初始化
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
    //陀螺仪和加速度计的初始化
    port_init_NoALT(PTA8,PULLUP); 
    IIC_Init();
    DELAY_MS(100); 
    Init_L3G4200D();
    Init_L3G4200D();    
    Init_L3G4200D();
    Init_L3G4200D();
    DELAY_MS(100);
    MMA845x_init();
    MMA845x_init();
    MMA845x_init();
    MMA845x_init();
    DELAY_MS(100);
    //*************************
    //编码器采集初始化
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    //初始化 PWM 输出
    ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);//左正
    ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);//左反
    ftm_pwm_init(FTM0, FTM_CH2,10*1000,0);//右反
    ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);//右正
    pit_init_ms(PIT0, 1);                                   //初始化PIT0，定时时间为： 1ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断复位函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                                 //使能PIT0中断
    //nrf中断
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    //设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTE_IRQn);       
    EnableInterrupts;//中断允许
}

void main()
{ 
    uint8 key_state=0;
    uint8 key_number=0;
    uint8 speed_state=0;
    uint8 speed_state_falg=0;
    
    key_init_init();
    LED_Init();
    //状态切换
    while(!speed_state)
    {
       if(key_check(KEY_A) ==  KEY_DOWN)
      {
       speed_state=1;
       speed_state_falg=1;
      }
      if(key_check(KEY_B) ==  KEY_DOWN)
      {
       while(speed_state_falg)
       { beep();}
       speed_state=2;
       speed_state_falg=1;
      }
      if(key_check(KEY_START) ==  KEY_DOWN)
      {
        while(speed_state_falg)
        {beep();}
        speed_state=3;
        speed_state_falg=1;
      }
      LED_PrintValueC(80,1,speed_state);
    }
    //状态对应数据
    switch(speed_state)
    {
      case 1:
        data1();
        beep();
        break;
      case 2:
        data2();
        beep();
        break;
      case 3:
        data3();
        beep();
        break;
      default:
        while(1)
         beep();   
    }  
    //功能选择和发车
    while(!key_state)
    {
      if(key_check(KEY_D) ==  KEY_DOWN)
      {
        key_number=1;
        beep();
      }
      if(key_check(KEY_R) ==  KEY_DOWN)
      {
        key_number=2;
        beep();
      }
      if(key_check(KEY_L) ==  KEY_DOWN)
      {
        nrf_flag=1;
        key_number=3;
        beep();
      }
      if(key_check(KEY_U) ==  KEY_DOWN)
      {
        key_state=1;
        beep();
      }
      LED_PrintValueC(80,1,speed_state);
    } 
    all_init();
    //if(nrf_flag)
      //while(!nrf_init());                                   //初始化NRF24L01+ ,等待初始化成功为止
    while(1)
    {
      //******************正常程序
      camera_get_img(); 				    	//摄像头获取图像
      img_extract((uint8 *)img,(uint8 *)imgbuff,CAMERA_SIZE);   //图像解压
      Get_Border();                                             //提取
      Derror=Get_derror();
      Turnning_speed=Turnning_PID(Derror);
      LED_PrintValueI(80,2,Derror);
      LED_PrintValueI(80,3,Turnning_speed);
      //LED_PrintValueF(80,3,Turnning_P,2);
      //LED_PrintValueF(80,4,Turnning_D,2);
      //LED_PrintValueF(80,5,I_Speed,2);
      key_manage(key_number);
    }
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/**********************平衡中断程序*******************/
void PIT0_IRQHandler(void)
{ 
    int16 L_real_speed=0,R_real_speed=0;             //左右轮真实速度
    static uint16 count;
    int balance_speed=0;                           //平衡速度
    static uint8 time_count20=0; 
    
    //平衡部分
    acc_gyro_Calculate();//采集角度
    balance_speed=balance_Speed_Calculate(angleX,GYRO_Y);
    //直走速度部分，20ms进入一次测速
    time_count20++;
    if(time_count20==20)
    {
      time_count20=0;
      L_real_speed=-ftm_quad_get(FTM1);
      R_real_speed=ftm_quad_get(FTM2);
      ftm_quad_clean(FTM1);
      ftm_quad_clean(FTM2);
      encoder.Speed=(L_real_speed+R_real_speed)/2;
      error_sum+=encoder.Speed-All_speed;     //速度误差积分
      if(error_sum>125)       error_sum=125;     //减速 slow_down//75
      if(error_sum<-550)      error_sum=-550;  //加速 accelerate
    }
    //******************************
    PID_Speed();
    left_speed=(int)-(balance_speed+PID.Speed-Turnning_speed);//
    right_speed=(int)-(balance_speed+PID.Speed+Turnning_speed);//
    if(count<1000)
      count++;
    else
    {
      if(!stop_car)
      {
        Speed_control(left_speed,right_speed); 
      }
      else
        Speed_control(0,0);
    }
    if(stop_count<10010)
    {
      stop_count++;
    }
    PIT_Flag_Clear(PIT0);                      //清中断标志位
}

void data1()
{
   P_Speed=1;
   I_Speed=1.5;
   Turnning_P=30; 
   Turnning_D=1.22;
   Target_speed1=300;
   Set_angle=5;
}

void data2()
{
   P_Speed=0.8;
   I_Speed=1.4;
   Turnning_P=28.5; 
   Turnning_D=1.22;//1.22
   Target_speed1=280;
   Set_angle=6;
}

void data3()
{
   P_Speed=0.8;
   I_Speed=1.4;
   Turnning_P=25; 
   Turnning_D=1;
   Target_speed1=280;//255
   //圆环补线0.8
}

/*!
 *  @brief      PORTD端口中断服务函数
 *  @since      v5.0
 */
void PORTE_IRQHandler(void)
{
  //nrf_irq
  PORT_FUNC(E,27,nrf_handler);
}
/*
void  show_data()
{
  uint8 ch[5];
  
  strcpy(ch,"B_P =");
  LED_P6x8Str(5,0,ch);
  LED_PrintValueF(30,0,P_ANGLE,1);
  
  strcpy(ch,"B_D =");
  LED_P6x8Str(5,1,ch);
  LED_PrintValueF(30,1,D_ANGLE,1);
  
  strcpy(ch,"S_P =");
  LED_P6x8Str(5,2,ch);
  LED_PrintValueF(30,2,P_Speed,1);
  
  strcpy(ch,"S_I =");
  LED_P6x8Str(5,3,ch);
  LED_PrintValueF(30,3,I_Speed,1);
  
  strcpy(ch,"D_P =");
  LED_P6x8Str(5,4,ch);
  LED_PrintValueF(30,4,Turnning_P,1);
  
  strcpy(ch,"D_D =");
  LED_P6x8Str(5,5,ch);
  LED_PrintValueF(30,5,Turnning_D,1);
  
  strcpy(ch,"Ang =");
  LED_P6x8Str(5,6,ch);
  LED_PrintValueF(30,6,Set_angle,1);
  
  strcpy(ch,"Spe =");
  LED_P6x8Str(5,7,ch);
  LED_PrintValueF(30,7,Target_speed1,1);
}
*/

void key_manage(uint8 i)
{
  switch (i)
  {
    case 1: img_show();
      break;
    case 2: display_waveform();
      break;
    case 3: nrf_control();
      break;
    case 4: 
      break;
  }
}

void display_waveform()
{
    OutData[0] = GYRO_Y;
    OutData[1] = GYRO_X;
    OutData[2] = ACC_ANGLE;
    OutData[3] = angleX;
    OutData[4] = 0;
    vcan_sendware((uint8_t *)OutData, sizeof(OutData));
}

void nrf_control()
{
    uint8 relen;
    relen = nrf_rx(buff,DATA_PACKET);               //等待接收一个数据包，数据存储在buff里
    if(relen != 0)
    {  
       relen=0;
       DDEBUG();
       beep();
    }
}

void key_init_init()
{
   key_init(KEY_U);
   key_init(KEY_D);
   key_init(KEY_L);
   key_init(KEY_R);
   key_init(KEY_A);
   key_init(KEY_B);
   key_init(KEY_START);
}

void beep()
{
  gpio_init(PTD9,GPO,1);
  DELAY_MS(5);
  gpio_init(PTD9,GPO,0);
}