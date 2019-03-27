/*****************************************
�� �� ����LED.h
�� �� �ߣ�������ӿƼ�
�������ڣ�2012/11/08
��    ����LED128x64��ʾ���ײ�����
------------------------------------------
����ͷ�ļ���


 void LED_Init(void);
 void LED_CLS(void); 
 void LED_Set_Pos(uint8 x, uint8 y);//�������꺯��
 void LED_WrDat(uchar data);   //д���ݺ���
 void LED_Fill(uint8 dat);
 void LED_P6x8Char(uint8 x,uint8 y,uint8 ch);
 void LED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
 void LED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LED_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LED_PXx16MixStr(uint8 x, uint8 y, uint8 ch[]);
 
void LED_PrintBMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
void LED_PrintValueC(uchar x, uchar y,char data);
void LED_PrintValueI(uchar x, uchar y, int data);
void LED_PrintValueF(uchar x, uchar y, float data, uchar num);
void LED_PrintPIC(uchar table[ROW][COLUMN]);
void LED_PrintPIC2(uchar table[ROW][COLUMN]);
void LED_PrintEdge(void);
void LED_Cursor(uchar cursor_column, uchar cursor_row);

*****************************************/

#ifndef __OLED_H__
#define __OLED_H__

//#include "short_w.h"
#include "common.h"
#include "MK60_gpio.h"
#include "include.h"

/*****************************������������*****************************/


typedef char *pchar;
typedef unsigned char *puchar;
typedef int  *pint;
typedef unsigned int *puint;
/*#define LED_SCL       PORTA_PA0  
#define LED_SDA       PORTA_PA1
#define LED_RST       PORTA_PA2 
#define LED_DC         PORTA_PA3 
#define LED_PORT    DDRA*/

/*****************************************
���� �� ��:��ʼ�����Ŷ���
��˵ �� ��:ֱ���������IO�Ϳ�����*/

#define SCL_PIN PTA5
#define SDA_PIN PTE26
#define RST_PIN PTE25
#define DC_PIN  PTE24//A0 A3Ϊ������

#define CS_PIN PTE1

#define INIT 0
#define LED_SCL_Init  gpio_init(SCL_PIN,GPO,INIT) // ʱ�ӳ�ʼ������
#define LED_SDA_Init  gpio_init(SDA_PIN,GPO,INIT)//���ݿ�D1
#define LED_RST_Init  gpio_init(RST_PIN,GPO,INIT)//��λ���ܵ�ƽ
#define LED_DC_Init   gpio_init(DC_PIN,GPO,INIT)//ƫ�ó���
#define LED_CS_Init   gpio_init(CS_PIN,GPO,INIT)//ƫ�ó���


#define LED_SCLH  GPIO_PDOR_REG(GPIOX_BASE(SCL_PIN))  |= (1 << PTn(SCL_PIN));// ʱ�Ӷ��� 
#define LED_SCLL  GPIO_PDOR_REG(GPIOX_BASE(SCL_PIN)) &= ~(1 << PTn(SCL_PIN));

#define LED_SDAH  GPIO_PDOR_REG(GPIOX_BASE(SDA_PIN))  |= (1 << PTn(SDA_PIN));//���ݿ�D0
#define LED_SDAL  GPIO_PDOR_REG(GPIOX_BASE(SDA_PIN)) &= ~(1 << PTn(SDA_PIN));

#define LED_RSTH  GPIO_PDOR_REG(GPIOX_BASE(RST_PIN))  |= (1 << PTn(RST_PIN));//��λ���ܵ�ƽ
#define LED_RSTL  GPIO_PDOR_REG(GPIOX_BASE(RST_PIN)) &= ~(1 << PTn(RST_PIN));

#define LED_DCH   GPIO_PDOR_REG(GPIOX_BASE(DC_PIN))  |= (1 << PTn(DC_PIN));
#define LED_DCL   GPIO_PDOR_REG(GPIOX_BASE(DC_PIN)) &= ~(1 << PTn(DC_PIN));//ƫ�ó���

extern int img_center[50];				        //��ȡ�е���λ��
extern int left_border[CAMERA_H-10]; 
extern int right_border[CAMERA_H-10]; 
/************************************************/

void  LEDPIN_Init(void);   //LED�������ų�ʼ��
void LED_Init(void);
void LED_CLS(void);
void LED_Set_Pos(uint8 x, uint8 y);//�������꺯��
void LED_WrDat(uint8 data);   //д���ݺ���
void LED_P6x8Char(uint8 x,uint8 y,uint8 ch);
void LED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void LED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void LED_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
void LED_PXx16MixStr(uint8 x, uint8 y, uint8 ch[]);
void LED_PrintBMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
void LED_Fill(uint8 dat);
void LED_PrintValueC(uint8 x, uint8 y,int8 data);
void LED_PrintValueI(uint8 x, uint8 y, int16 data);
void LED_PrintValueF(uint8 x, uint8 y, float data, uint8 num);
void LED_PrintValueFP(uint8 x, uint8 y, uint16 data, uint8 num);
void LED_PrintEdge(void);
void LED_Cursor(uint8 cursor_column, uint8 cursor_row);
void LED_PrintLine(void);
 
 
 
 void img_show(void);
 void team_name(void);
 void look_look(void);

#endif