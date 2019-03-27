#include"IIC.h"
#include "common.h"
#include "include.h"


void I2C_delay(void)
{
   
}

void IIC_Init(void)        //I2C��ʼ��   
{           
            port_init_NoALT(PTE5,ODO| PULLUP );
            port_init_NoALT(PTE6,ODO| PULLUP);
            gpio_init(PTE5,GPO,1); 
            gpio_init(PTE6,GPO,1); 
           
	//    SDA_H;
	//    SCL_H;
}

void IIC_Start(void)      //������ʼ�ź�
{
	   L3G_SDA_OUT();     //SDAת��Ϊ���
	   L3G_SDA_H;		
	   L3G_SCL_H;
	   I2C_delay();
           L3G_SDA_L;//START:when CLK is high,DATA change form high to low 
	   I2C_delay();
}	  

void IIC_Stop(void)       //����ֹͣ�ź�
{
	   L3G_SDA_OUT();      //SDAת��Ϊ����
           L3G_SCL_L;
           I2C_delay();
           L3G_SDA_L;
 	   I2C_delay();
           L3G_SCL_H;
           I2C_delay();
	   L3G_SDA_H;          //����IIC���߽����ź�
	   I2C_delay();							   	
}

void Receiveack(void)     //����Ӧ��
{
           uint8 i=0,j;
           L3G_SDA_IN();      //SDAת��Ϊ����
	   L3G_SCL_H;
	   I2C_delay();	 
           j=L3G_R_SDA;
	while(j&&(i<10))
	  {
	     I2C_delay();
             j=L3G_R_SDA;
	     i++;
	  }
   	  L3G_SCL_L;           //ʱ�����0	   
	  I2C_delay();	  
} 


void IIC_Ack(void)        //����ACKӦ��
{
	L3G_SDA_OUT();
        L3G_SCL_L; 
        I2C_delay();
	L3G_SDA_L;
	I2C_delay();
	L3G_SCL_H;
	I2C_delay();
	L3G_SCL_L;
	I2C_delay();
}
   
void IIC_NAck(void)                //������ACKӦ��	 
{
	L3G_SDA_OUT();
        L3G_SCL_L; 
        I2C_delay();
	L3G_SDA_H;
	I2C_delay();
	L3G_SCL_H;
	I2C_delay();
	L3G_SCL_L;
	I2C_delay();
}					 				     
		  

void IIC_Send_Byte(uint8 txd)         //��������
{                        
    uint8 t;   
	  L3G_SDA_OUT(); 	    
    for(t=0;t<8;t++)
         {   
		L3G_SCL_L; 	
		I2C_delay(); 
                if((txd&0x80)>>7) 
                { L3G_SDA_H;}
                else  
                { L3G_SDA_L;}     
                txd<<=1; 	  
		I2C_delay();   //������ʱ
	     	L3G_SCL_H;
		I2C_delay(); 
         }	 
		L3G_SCL_L;
//		I2C_delay();
//		SDA_H;
//		DELAY_US(6);	
} 	    


uint8 IIC_Read_Byte(void)             //��������
{
	      uint8 i,j,receive=0;
	      L3G_SDA_IN();             ////SDAת��Ϊ����
        for(i=0;i<8;i++ )
	  {
            L3G_SCL_L;
            I2C_delay();
            L3G_SCL_H;
	    I2C_delay();
            receive<<=1;
            j=L3G_R_SDA;
            if(j)receive++;   
//	    I2C_delay();
            
          }	
            L3G_SCL_L;
//	    I2C_delay();
        return receive ;
}

void IIC_Write(uint8 SlaveAddress,uint8 addr,uint8 dat)     //д����
{
              IIC_Start();
	      IIC_Send_Byte(SlaveAddress) ;
	      Receiveack() ;
	      IIC_Send_Byte(addr) ;
	      Receiveack() ;
	      IIC_Send_Byte(dat) ;
	      Receiveack() ;
	      IIC_Stop();      
}


uint8 IIC_Read(uint8 SlaveAddress,uint8 addr)               //������
{
               uint8 read_data;
	       IIC_Start();
	       IIC_Send_Byte(SlaveAddress) ;
	       Receiveack() ;
	       IIC_Send_Byte(addr) ;
	       Receiveack() ;
	       IIC_Start();
	       IIC_Send_Byte((SlaveAddress+1)) ;
	       Receiveack() ;
	       read_data=IIC_Read_Byte();
               IIC_NAck();
               IIC_Stop();  
               return  read_data ;	
}


void IIC_Read_n_byte(uint8 device,uint8 addr,uint8 n,uint8 *buf)     //��n������
{
	       uint8 i;
               IIC_Start(); 
	       IIC_Send_Byte(device) ;
	       Receiveack() ;
	       IIC_Send_Byte(addr) ;
	       Receiveack() ;
	       IIC_Start();
	       IIC_Send_Byte((device+1)) ;
	       Receiveack() ;
	       for(i=0;i<n;i++)
	         {
					     buf[i]= IIC_Read_Byte();  
						   if(i==n-1)
							 {
							     IIC_NAck();
							 }
							 else
							 {
							     IIC_Ack();
							 } 
		}
					 IIC_Stop();
     
}
