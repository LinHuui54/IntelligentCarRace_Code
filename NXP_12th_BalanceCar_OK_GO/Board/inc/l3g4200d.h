

#ifndef __L3G4200D_H__
#define  __L3G4200D_H__

//#define L3G_IIC_ADDR    0x69  	// 请注意此处 SDO已接上拉电阻 SDO默认高电平 IIC地址为0x69  若接地则IIC地址为0x68  
#define L3G_IIC_ADDR    0xd2								//具体参看 L3G4200D 手册P22

#define READ_L3G        0x1
#define WRITE_L3G       0x0

// 寄存器地址定义
#define WHO_AM_I        0x0f
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define REFRENCE        0x25
#define STATUS_REG      0x27
#define GYRO_OUT_X_L         0x28
#define GYRO_OUT_X_H         0x29
#define GYRO_OUT_Y_L         0x2A
#define GYRO_OUT_Y_H         0x2B
#define GYRO_OUT_Z_L         0x2C
#define GYRO_OUT_Z_H         0x2D
#define INT_CFG         0x30
#define INT_SRC         0x31
#define INT_TSH_XH      0x32
#define INT_TSH_XL      0x33
#define INT_TSH_YH      0x34
#define INT_TSH_YL      0x35
#define INT_TSH_ZH      0x36
#define INT_TSH_ZL      0x37
#define INT_DURATION    0x38

//unsigned char ReadGyroID();


void Write_l3g4200d(uint8,uint8);
uint8 Read_l3g4200d(uint8);

void extern Init_l3g4200d();
int  extern GetData_l3g4200d(uint8 REG_Address);



#endif
