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
#ifndef IMGDATA_TREATING_H
#define IMGDATA_TREATING_H

#include "include.h"
#include "common.h"

/*#define   Turnning_P       9      //8
#define   Turnning_D       0.6    //0.45*/
 
#define  dynamic_ratio     0.1
  
//�����ٶ�pid
void Speed_control(int left_speed,int right_speed);
int Get_derror();
int Turnning_PID(int derror);



extern int  circle_derror;
extern int  show_data0;
extern int barrier_flag;//�ϰ���־
extern int ramp_flag1;//�µ���־
extern int ramp_stop_flag;
//extern int barrier_stop_flag;//������ֹͣ���
extern uint8 b_start_flag;

#endif