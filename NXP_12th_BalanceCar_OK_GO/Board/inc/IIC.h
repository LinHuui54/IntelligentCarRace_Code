#include "common.h"

#define L3G_SDA_IN()  {gpio_ddr (PTE5, GPI);}     //��������
#define L3G_SDA_OUT() {gpio_ddr (PTE5, GPO);}     //���

#define L3G_SCL_H      gpio_set (PTE6, 1); //SCL//D8
#define L3G_SCL_L      gpio_set (PTE6, 0); //SCL
#define L3G_SDA_H      gpio_set (PTE5, 1); //SDA���//D10	 
#define L3G_SDA_L      gpio_set (PTE5, 0); //SDA���	
#define L3G_R_SDA      gpio_get (PTE5);  //SDA����



extern void IIC_Init(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void Receiveack(void);
extern void IIC_Ack(void);
extern void IIC_NAck(void);
extern void IIC_Send_Byte(uint8 txd);
extern uint8 IIC_Read_Byte(void);
extern void IIC_Write(uint8 SlaveAddress,uint8 addr,uint8 dat);
extern uint8 IIC_Read(uint8 SlaveAddress,uint8 addr) ;
extern void IIC_Read_n_byte(uint8 device,uint8 addr,uint8 n,uint8 *buf) ;    //��n������

