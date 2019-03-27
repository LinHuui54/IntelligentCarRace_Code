/*******************************************************************************
                         #ʮ���족�����֡�������ͷ����#
@��ʼ���飺            OK_GO
@�������ڣ�            2016-11-19
@���ܣ�                ��ȡ�������ߡ�

@��ʼ�������ԣ�       ����һֱ��д���淶���ṹ����������ά�����Ż��ĳ���
                      ϣ��������ܹ���ʦ����ϲ�������Ҵ�����ȥ��
                      ����ϣ��ʦ�ܿ��ԶԳ�������Ż�����ǿ���ܣ�������һζֱ��ʹ�á�
                      ���޸ĵ�ʱ������ѭ��ʽ�޸ġ�
                      ����������޸��ˣ���ȷ���޸Ĳ�����Ч�����¿�������Ϣ��
*******************************************************************************/
/*******************************************************************************
�����Գ���������Ż�����������������Ϣ����ʽ���ϣ�
*******************************************************************************/

#ifndef GET_BORDER_H
#define GET_BORDER_H

#include "include.h"
#include "common.h"

#define   center    (CAMERA_W/2)    //ͼ������
#define   L_limit   (CAMERA_W-78)     //ͼ����߽�
#define   R_limit   (CAMERA_W-3)   //ͼ���ұ߽�
#define   T_limit   (9)     //ͼ�񶥱߽�
#define   B_limit   (59)

#define   range     (5)     //ɨ��ķ�Χ



#define   N(row)    (CAMERA_H-1-row) //ͼ��������洢�����±�Ĺ�ϵ����Ҳ��֪����ô˵���Լ���ɣ��������ң�

//ɨ�跽ʽ
enum scanning_mode
{
  
  LEFT_POSITIVE,     //����_����_����
  LEFT_ANASTROPHIC,  //����_����_����
  
  RIGHT_POSITIVE,    //����_����_����
  RIGHT_ANASTROPHIC, //����_����_����
  
};

extern  uint8 img[CAMERA_H][CAMERA_W];      
extern int left_border[CAMERA_H-10];                
extern int right_border[CAMERA_H-10];
extern int img_center[CAMERA_H-10];
extern int circle_flag;
extern uint16 circle_count;
extern uint8  show_data1;
extern int R_barrier_point,L_barrier_point;//�������ϰ��ĵ�
extern uint8 cross_limit_flag;
extern uint8 circle_adding_flag;
extern uint8 stop_b_flag;
extern uint8 start_line_barrir_flag;
extern uint8 zhizou_flag;



//��ȡ�������������ɹ�����1��ʧ���򷵻�0
uint8 Get_Border();                       

//������߱�����ʼ��
void get_border_init();
//Ѱ�Ҹ���ɨ��㣬�ɹ�������ʼ��������ʧ�ܷ���0 
int8 get_starting_point(uint8 start,uint8 end);
//����ɨ�裬�ɹ�����1��ʧ�ܷ������ɨ������
uint8 following_scan(int start_row,uint8 L_start,uint8 R_start);
//�ж�Ϊʮ�ֺ��ɨ��
uint8 cross_scanning(uint8 start,uint8 end);
//ʮ�ֲ���
uint8 adding_border();

//����ɨ�裬�ɹ�����1��ʧ�ܷ���0
uint8 one_line_scanning(int scanning_mode,int star,int end,int R_or_C );

//����_����_����
uint8 left_positive(int start,int end,int R_or_C );
//����_����_����
uint8 left_anastrophic(int start,int end,int R_or_C );
//����_����_����
uint8 right_positive(int start,int end,int R_or_C );
//����_����_����
uint8 right_anastrophic(int start,int end,int R_or_C );
//����_����_����
uint8 up_positive(uint8 C);
//Բ������
void adding_circle_border();
//������ʶ��
uint8 Zebra_crossing(int R_or_C);
//�ϰ����
void scan_barrier(int R_or_C);


#endif