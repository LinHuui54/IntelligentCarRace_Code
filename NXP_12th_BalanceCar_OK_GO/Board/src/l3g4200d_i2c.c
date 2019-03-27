

#include "include.h"
#include "common.h"
//#include  "MKL_gpio.h"

#define SCL_PORT    PTE6
#define SDA_PORT    PTE5


#define IIC_SCL_PDO       gpio_init(SCL_PORT,GPO,1)
#define IIC_SDA_PDO       gpio_init(SDA_PORT,GPO,1)
#define IIC_SDA_PDI       gpio_init(SDA_PORT,GPI,1)

#define IIC_SCL_OUT_1     gpio_set (SCL_PORT, 1)
#define IIC_SCL_OUT_0     gpio_set (SCL_PORT, 0)
#define IIC_SDA_OUT_1     gpio_set (SDA_PORT, 1)
#define IIC_SDA_OUT_0     gpio_set (SDA_PORT, 0)
#define IIC_SDA_IN        gpio_get (SDA_PORT)


uint8 iic_ack;

void static somenop()                  // 根据情况自己设定
{
    asm("nop");asm("nop");
}

void L3g4200d_i2c_init()
{
    IIC_SCL_PDO;
    IIC_SDA_PDO;
}

void L3g4200d_start_i2c()
{
    IIC_SDA_OUT_1;
    
    IIC_SCL_OUT_1;
    somenop();
    somenop();
    
    IIC_SDA_OUT_0;
    somenop();
    somenop();
    
    IIC_SCL_OUT_0;
}

void L3g4200d_stop_i2c()
{
    IIC_SDA_OUT_0;
    
    IIC_SCL_OUT_1;
    somenop();
    somenop();
    
    IIC_SDA_OUT_1;
    somenop();
    somenop();    
}

void L3g4200d_SendAck_i2c(uint8 a)
{
    if(a==0)    IIC_SDA_OUT_0;
    else        IIC_SDA_OUT_1;
    
    somenop();
    
    IIC_SCL_OUT_1;
    somenop();
    somenop();
    
    IIC_SCL_OUT_0;    
    somenop();  
    somenop();
}

uint8 L3g4200d_RecvAck_i2c()
{
    uint8 cy=0;
    
    IIC_SCL_OUT_1;
    somenop();
    somenop();

    IIC_SDA_PDI;
    if(IIC_SDA_IN)  cy=1;
    
    IIC_SCL_OUT_0; 
    somenop();
    somenop();

    IIC_SDA_PDO;
    return cy;    
}

void L3g4200d_SendByte_i2c(uint8 c)
{
    uint8 i;
    for(i=0;i<8;i++)
    {          
        if((c<<i) & 0x80)   IIC_SDA_OUT_1;
        else                IIC_SDA_OUT_0;
        IIC_SCL_OUT_1;
        somenop(); 
        somenop();
        
        IIC_SCL_OUT_0;
        somenop(); 
        somenop(); 
    }   
    L3g4200d_RecvAck_i2c();
    
    IIC_SDA_PDO;
}


uint8 L3g4200d_RcvByte_i2c()
{
    uint8 retc;
    uint8 i;
    
    retc=0;
    
    IIC_SDA_OUT_1;
    
    IIC_SDA_PDI;
    for(i=0;i<8;i++)
    {      
        IIC_SCL_OUT_1;
        somenop(); 
        somenop();
        
        retc=retc<<1;         
        if(IIC_SDA_IN==1)   retc=retc+1;   
        IIC_SCL_OUT_0;
        somenop(); 
        somenop(); 
    }
    IIC_SDA_PDO;
    
    return retc;
}

