#include "common.h"
#include "l3g4200d.h"
#include "l3g4200d_i2c.h"




void Write_l3g4200d(uint8 addr,uint8 data)
{
    L3g4200d_start_i2c();
    L3g4200d_SendByte_i2c(L3G_IIC_ADDR);     //发送设备地址+写信号
    L3g4200d_SendByte_i2c(addr);
    L3g4200d_SendByte_i2c(data);
    L3g4200d_stop_i2c();
}

uint8 Read_l3g4200d(uint8 addr)
{
    uint8 data;
    
    L3g4200d_start_i2c();
    L3g4200d_SendByte_i2c(L3G_IIC_ADDR);     //发送设备地址+写信号
    L3g4200d_SendByte_i2c(addr);
    
    L3g4200d_start_i2c();
    L3g4200d_SendByte_i2c(L3G_IIC_ADDR+1); 
    
    data=L3g4200d_RcvByte_i2c();
    
    L3g4200d_SendAck_i2c(1);
    L3g4200d_stop_i2c();   
    
    return data;
}


void Init_l3g4200d()
{
    L3g4200d_i2c_init();          
//0xEF; CTRL_REG1   Output Data Rate 800Hz /Bandwidth cut-off 50/Power down normal mode
//0x00; CTRL_REG2   High pass filter mode configuration/High pass filter cut off frecuency configuration
//0x08; CTRL_REG3   Disable;
//0x00; CTRL_REG4   continous update/Little Endian/FS = 250 dps/Self Test Disabled/4-wire interface
//0x00; CTRL_REG5   Reboot memory content/FIFO disable/HPF disabled/INT1 selection configuration/Out selection configuration
//0x00; REFERENCE
   
    Write_l3g4200d(CTRL_REG1,0xE9);
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    Write_l3g4200d(CTRL_REG2,0x00);
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    Write_l3g4200d(CTRL_REG3,0x08);
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    Write_l3g4200d(CTRL_REG4,0x00);    //2000dbp
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    Write_l3g4200d(CTRL_REG5,0x00);  
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    Write_l3g4200d(REFRENCE,0x00);
    asm("nop");asm("nop");
    asm("nop");asm("nop");
    asm("nop");asm("nop");
}


//**************************************
//合成数据
//**************************************
int GetData_l3g4200d(uint8 REG_Address)
{
	int H,L;
        int temp;
	L=Read_l3g4200d(REG_Address);
	H=Read_l3g4200d(REG_Address+1);
        temp = (int16)((H<<8)+L);
	return temp;    //合成数据
}