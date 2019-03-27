/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-12-19
 */
#include "common.h"
#include "include.h"

//**************ȫ�ֱ�������*********
uint8 imgbuff[CAMERA_SIZE]; //�洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W]; //��ѹ�����ͼ������飬����ͼ����                         
int left_border[CAMERA_H-10];//�洢��߽�
int right_border[CAMERA_H-10]; //�洢�ұ߽�
int img_center[CAMERA_H-10];//�洢����
int Derror=0;
int left_speed=0,right_speed=0;                //�����ֵ��ٶ�ֵ
float str_error,last_str_error,str_sum_error,str_P_spped;
float last_PIspeed,now_PIspeed;
float speed_out=0;                               //���ֱ���ٶ�
float Set_speed;
float error_sum;
int Turnning_speed;
//***ң�ر���
uint8 buff[DATA_PACKET];                        //������ջ�����
uint8 diglt_flag=0;
uint8 nrf_flag=0;
//***********
//**************�����ⲿ��������*************
extern encoder_e encoder;       //�������ṹ��
extern struct PID_e PID;
extern int16 All_speed;
extern float GYRO_Y;
extern float OutData[5];                              //SCIʾ��������
extern int8  left_tend;
extern int8  right_tend;
extern int8  left_max;
extern int8  right_max;
extern uint16  stop_count;
//**************��������**********************
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PORTE_IRQHandler(void);
void PIT0_IRQHandler(void);
void beep();                                          //������
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
    DisableInterrupts;//��ֹ���ж�
    //�����ж����ȼ�
    NVIC_SetPriorityGrouping(0);                            //�����ж����ȼ�����
    NVIC_SetPriority(PORTE_VECTORn,0);                     //���նˣ�������������ȼ�����������ν��
    NVIC_SetPriority(PORTA_VECTORn,1);                     //���նˣ�������������ȼ�����������ν��
    NVIC_SetPriority(PIT0_VECTORn,2);
    NVIC_SetPriority(DMA0_VECTORn,3);
    camera_init(imgbuff);                                   //����ͷ��ʼ��
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
    //�����Ǻͼ��ٶȼƵĳ�ʼ��
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
    //�������ɼ���ʼ��
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    //��ʼ�� PWM ���
    ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);//����
    ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);//��
    ftm_pwm_init(FTM0, FTM_CH2,10*1000,0);//�ҷ�
    ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);//����
    pit_init_ms(PIT0, 1);                                   //��ʼ��PIT0����ʱʱ��Ϊ�� 1ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϸ�λ����Ϊ PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                                 //ʹ��PIT0�ж�
    //nrf�ж�
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTE_IRQn);       
    EnableInterrupts;//�ж�����
}

void main()
{ 
    uint8 key_state=0;
    uint8 key_number=0;
    uint8 speed_state=0;
    uint8 speed_state_falg=0;
    
    key_init_init();
    LED_Init();
    //״̬�л�
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
    //״̬��Ӧ����
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
    //����ѡ��ͷ���
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
      //while(!nrf_init());                                   //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
    while(1)
    {
      //******************��������
      camera_get_img(); 				    	//����ͷ��ȡͼ��
      img_extract((uint8 *)img,(uint8 *)imgbuff,CAMERA_SIZE);   //ͼ���ѹ
      Get_Border();                                             //��ȡ
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
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/**********************ƽ���жϳ���*******************/
void PIT0_IRQHandler(void)
{ 
    int16 L_real_speed=0,R_real_speed=0;             //��������ʵ�ٶ�
    static uint16 count;
    int balance_speed=0;                           //ƽ���ٶ�
    static uint8 time_count20=0; 
    
    //ƽ�ⲿ��
    acc_gyro_Calculate();//�ɼ��Ƕ�
    balance_speed=balance_Speed_Calculate(angleX,GYRO_Y);
    //ֱ���ٶȲ��֣�20ms����һ�β���
    time_count20++;
    if(time_count20==20)
    {
      time_count20=0;
      L_real_speed=-ftm_quad_get(FTM1);
      R_real_speed=ftm_quad_get(FTM2);
      ftm_quad_clean(FTM1);
      ftm_quad_clean(FTM2);
      encoder.Speed=(L_real_speed+R_real_speed)/2;
      error_sum+=encoder.Speed-All_speed;     //�ٶ�������
      if(error_sum>125)       error_sum=125;     //���� slow_down//75
      if(error_sum<-550)      error_sum=-550;  //���� accelerate
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
    PIT_Flag_Clear(PIT0);                      //���жϱ�־λ
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
   //Բ������0.8
}

/*!
 *  @brief      PORTD�˿��жϷ�����
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
    relen = nrf_rx(buff,DATA_PACKET);               //�ȴ�����һ�����ݰ������ݴ洢��buff��
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