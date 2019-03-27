/*******************************************************************************
                         #ʮ���족�����֡�������ͷ����#
@��ʼ���飺            OK_GO
@�������ڣ�            2016-11-30
@���ܣ�                ������ʡ�

@��ʼ�������ԣ�       ����һֱ��д���淶���ṹ����������ά�����Ż��ĳ���
                      ϣ��������ܹ���ʦ����ϲ�������Ҵ�����ȥ��
                      ����ϣ��ʦ�ܿ��ԶԳ�������Ż�����ǿ���ܣ�������һζֱ��ʹ�á�
                      ���޸ĵ�ʱ������ѭ��ʽ�޸ġ�
                      ����������޸��ˣ���ȷ���޸Ĳ�����Ч�����¿�������Ϣ��
*******************************************************************************/
/*******************************************************************************
�����Գ���������Ż�����������������Ϣ����ʽ���ϣ�
*******************************************************************************/
#include "include.h"
#include "common.h"

//Сs�ж�
int8  left_tend;
int8  right_tend;
int8  left_max=40;
int8  right_max=40;
int  circle_derror=0;
uint8 b_flag=0;
/********�µ��жϲ���***/
int ramp_flag0=0;
int ramp_flag1=0;
int ramp_count=0;
int ramp_speed=0;
extern encoder_e encoder;       //�������ṹ��

int ramp_stop_flag=0;
int barrier_stop_flag=0;
int ramp_start_flag=0;//�µ���ʼ����־
int barrier_flag=0;//�ϰ���־
uint16 b_count=0;
uint16 b_time_count=0;
uint8 b_stop_flag=0;
int show_data0;//���ڹ۲�ֲ�����
uint8 b_start_flag=0;//��ʼ����ϰ���־


/*
 *  @brief         ���ת���ٶ�
 * @param          derror:ת��ƫ��
 *  Sample usage   ����ֵ��ת���ٶ�ռ�ձ�
 */
int Turnning_PID(int derror)
{
   int Turning_speed=0;
     
   //Сs���жϣ��ò�ͬ��pd
   //LED_PrintValueI(80,5,abs(left_tend-right_tend));
   //LED_PrintValueI(80,6,abs(left_max-right_max));
  if((abs(left_tend-right_tend)<15)&&(abs(left_max-right_max)<20)&&!b_flag)//10//20
  {
      Turning_speed=16*derror-(0.9)*GYRO_X;//16//0.9
  }
   else
   {
     if(derror>0)//����
        Turning_speed=(Turnning_P)*derror-(Turnning_D)*GYRO_X;//+0.15
     else
        Turning_speed=Turnning_P*derror-(Turnning_D)*GYRO_X;
   }

   
   left_tend=0;
   right_tend=0;
   left_max=40;
   right_max=40;
   //if(Turning_speed>175) Turning_speed=175;
   //if(Turning_speed<-200) Turning_speed=-200;
   return  (int)Turning_speed;                          
}

/*
 *  @brief         ���ת��ƫ��
 * @param          NULL
 *  Sample usage   ����ֵ��ת��ƫ��
 */
int Get_derror()
{
   uint8 i=0;
   uint8 n=0;
   uint8 last_width[CAMERA_H-10]={
                                  33,33,33,32,32,32,
                                  32,31,31,31,30,30,
                                  30,30,29,29,28,28,
                                  28,27,27,26,26,26,
                                  26,25,25,24,24,24,
                                  23,22,22,22,21,21,
                                  21,20,20,19,19,19,
                                  18,18,17,17,16,16,
                                  15,15
                                  /*33,33,32,32,31,31,
                                  31,31,30,30,30,29,
                                  29,29,28,28,27,27,
                                  27,26,26,25,25,25,
                                  24,24,23,23,22,22,
                                  22,21,21,20,20,20,
                                  19,19,18,18,18,17,
                                  17,16,16,15,15,14,
                                  14,14*/
                                  };
   int derror;    
   int average=0;
   int img_center_sum=0;
   int count2=0;
   int double_count=0;
   //circle_adding_flag=0;
    //****************************�õ�����*********************
    //��ȡ����
        for(i=0;i<50;i++)
        {
          if(left_border[i]!=0&&right_border[i]!=0)
          {
            img_center[i]=((left_border[i]+right_border[i])/2);
            double_count++;
          }
          if(left_border[i]==0&&right_border[i]!=0)   
          {
            img_center[i]=right_border[i]-last_width[i]-1;
          }
          if(left_border[i]!=0&&right_border[i]==0)
          {
            img_center[i]=left_border[i]+last_width[i]+1;
          }
          
          if(left_border[i]==0&&right_border[i]==0)
          {
            img_center[i]=0;
          }
        }
        
        show_data0=barrier_flag;
        
	for(n=0;n<50;n++)
		{ 
                  if(img_center[n]!=0)
                  {
		    img_center_sum+=img_center[n];
                    count2++;
                    //Сs���ж�
                    if(n>2)
                    {
                      if(img_center[n]>img_center[n-1])
                        right_tend++;
                      if(img_center[n]<img_center[n-1])
                        left_tend++;
                      if(img_center[n]>right_max)
                          right_max=img_center[n];
                      if(img_center[n]<left_max)
                          left_max=img_center[n];
                    }
                  }
		}
        /*LED_PrintValueI(80,1,ramp_more_than_40);
        LED_PrintValueI(80,2,ramp_less_than_40);
        LED_PrintValueI(80,3,ramp_bottom);
        LED_PrintValueI(80,4,ramp_double_flag);
       LED_PrintValueI(80,5,right_border[ramp_bottom]-right_border[ramp_top]); */
       /*if(encoder.Speed>150)
       {
          //ramp_start_flag=1;
          b_start_flag=1;
       }*/
        
/******************************�ϰ�����**************************************************/
      if(double_count>25&&!b_stop_flag&&b_start_flag&&!cross_limit_flag)//b_start_flag//&&!b_stop_flag
      {
        if((abs(left_tend-right_tend)<15)&&(abs(left_max-right_max)<20))
        {
             scan_barrier(15);//25
             //�ж����ϰ�ɨ�������������Ŀ���Ƿ�С���������
             if(R_barrier_point&&L_barrier_point)
             {
                if((abs(R_barrier_point-L_barrier_point))<(right_border[N(15)]-left_border[N(15)]))//(2*b_width[N(30)]-10)
                {
                   //b_flag=1;
                  b_count++;
                }
                else
                {
                  scan_barrier(17);//25
                  if(R_barrier_point&&L_barrier_point)
                  {
                    if((abs(R_barrier_point-L_barrier_point))<right_border[N(17)]-left_border[N(17)])
                      //b_flag=1;
                      b_count++;
                    else
                    {
                      scan_barrier(19);//25
                      if(R_barrier_point&&L_barrier_point)
                      {
                        if((abs(R_barrier_point-L_barrier_point))<(right_border[N(19)]-left_border[N(19)]))
                            //b_flag=1;
                            b_count++;
                        else
                        {
                          scan_barrier(21);//25
                          if(R_barrier_point&&L_barrier_point)
                          {
                            if((abs(R_barrier_point-L_barrier_point))<(right_border[N(21)]-left_border[N(21)]))
                              //b_flag=1;
                              b_count++;
                            else
                            {
                              scan_barrier(23);//25
                              if(R_barrier_point&&L_barrier_point)
                              {
                                if((abs(R_barrier_point-L_barrier_point))<(right_border[N(23)]-left_border[N(23)]))
                                  //b_flag=1;
                                  b_count++;
                              else
                              {
                                 scan_barrier(25);//25
                                 if(R_barrier_point&&L_barrier_point)
                                 {
                                   if((abs(R_barrier_point-L_barrier_point))<(right_border[N(25)]-left_border[N(25)]))
                                      //b_flag=1;
                                     b_count++;
                                 }
                              }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
             }
        }
      }
      if(b_count&&!cross_limit_flag&&!b_stop_flag)//&&!b_stop_flag
      {
        if(b_count>=5)
        {
          b_flag=1;
          b_stop_flag=1;
          b_time_count=0;
          //beep();
        }
        else
         b_time_count++;
        if(b_time_count>20)
        {
          b_count=0;
        }
      }
      LED_PrintValueI(80,5,b_count);
/********************************************************************************/    
/********************************************************************************/
        average=img_center_sum/count2;
        //derror=average-40;                  
        if(b_flag&&!circle_adding_flag&&!cross_limit_flag)
        {
          b_time_count++;
          if(b_time_count>20)
          {
            b_flag=0;
            b_stop_flag=0;
            b_count=0;
          }
          //beep();
          //derror=(R_barrier_point+L_barrier_point)/2-40;
          if((R_barrier_point+L_barrier_point)/2>40)
            derror=average-30;
          else
            derror=average-50;
          /*if(derror>=0)  
          {
            if(derror>15)   derror=15;
            if(derror<10)   derror=10;
          }
          if(derror<0)
          {
            if(derror<-15)  derror=-15;
            if(derror>-10)  derror=-10;
          }*/
        }
        else
        {
          /*if(!start_line_barrir_flag)
            derror=average-32;
          else
          {*/
            derror=average-40;
          //}
        }
        if(zhizou_flag)
          derror=0;
      
        if(abs(derror)<2)
          derror=0;
        if(derror>16)derror=16;//18
        if(derror<-17)derror=-17;//18
        //��ֹ���߲�̫С
        if(circle_adding_flag&&!cross_limit_flag)
        {
          if(abs(derror)<14)//14
          {
            if(derror>0)
              derror=14;
            if(derror<0)
              derror=-14;
          }
          /*if(derror<14&&derror>0)//13
            derror=14;
          else
          if(derror>-14&&derror<=0)//-13
            derror=-14;*/

        }
        //�����ж�Բ�����߷���
        if(!circle_flag)
        {
          circle_derror+=derror;
          if(circle_derror>300)
            circle_derror=300;
          if(circle_derror<-300)
            circle_derror=-300;
        }
        return derror;
}
/***********************************************************************/
/*
*  ����˵��������ٶȿ���
*  ����˵����   
*
*  �������أ��޷��Ž��ֵ
*  �޸�ʱ�䣺2013-2-10
* ��ע���ο��廪Դ��
*/
/******************************************************************************/
void Speed_control(int left_speed,int right_speed)
{
    if(left_speed > 700)  left_speed = 700;
    if(left_speed < -700) left_speed = -700;
    if(right_speed > 700)  right_speed = 700;
    if(right_speed < -700) right_speed = -700;
    if(left_speed>=0)
      {
        ftm_pwm_duty(FTM0,FTM_CH0,(uint32)left_speed);//����
        ftm_pwm_duty(FTM0,FTM_CH1,0);//��
      }
      else
      {
        left_speed=-left_speed;
        ftm_pwm_duty(FTM0,FTM_CH1,(uint32)left_speed);//��
        ftm_pwm_duty(FTM0,FTM_CH0,0);//����
      }
      
      if(right_speed>=0)
      {
        ftm_pwm_duty(FTM0,FTM_CH3,(uint32)right_speed);//����
        ftm_pwm_duty(FTM0,FTM_CH2,0);//�ҷ�
      }
      else
      {
        right_speed=-right_speed;
        ftm_pwm_duty(FTM0,FTM_CH2,(uint32)right_speed);//�ҷ�
        ftm_pwm_duty(FTM0,FTM_CH3,0);//����
      }
}
