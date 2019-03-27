#include "include.h"
#include"Acc_Gyro.h"
#include "common.h"
#include"IIC.h"
#include"math.h" 
//#include"PID.h"
//#include "usart.h"
//#include"dianci.h"
uint8 MMA8451[4],L3G4200[4];
float GYRO_X,GYRO_Z,GYRO_Y,ACC_ANGLE;  //GYRO_X��ת����ٶ�
float angleX;
float adeo;
void Init_L3G4200D(void)
{
	IIC_Write(L3G4200_Addr,CTRL_REG1, 0x4d);       //����x��z��,200hz
       
	IIC_Write(L3G4200_Addr,CTRL_REG2, 0x09);
     
	IIC_Write(L3G4200_Addr,CTRL_REG3, 0x08);
     
	IIC_Write(L3G4200_Addr,CTRL_REG4, 0x10);	//+-500dps
  
	IIC_Write(L3G4200_Addr,CTRL_REG5, 0x00);

}	

void MMA845x_init(void)
{	
        
        IIC_Write(MMA8451_Addr,MMA845x_CTRL_REG1, 0x00);   //������д����ģʽ���ٿ�����
        DELAY_MS(2); 
        IIC_Write(MMA8451_Addr,MMA845x_XYZ_DATA_CFG, 0x00);
        DELAY_MS(2); 
        IIC_Write(MMA8451_Addr,MMA845x_CTRL_REG1, 0x05);    //800hz,������ģʽ������ģʽ��
        DELAY_MS(2); 
       
}
int16 getdata(uint8 s_addr,uint8 addr)
{
           uint8  H,L;
	   uint16 data;
        
           if(s_addr==MMA8451_Addr)
            {
	      H=IIC_Read(s_addr,addr) ;     //��8λ
	      L=IIC_Read(s_addr,addr+1);   //��8λ
            }
           else
            {
               L=IIC_Read(s_addr,addr) ;     //��8λ
	       H=IIC_Read(s_addr,addr+1);   //��8λ
            }
	   data=((H<<8)+L);     //�ϳ�16λ      
	   return data;
}

void acc_gyro_Calculate(void)
{
           int16 x_acc,z_acc,gyro;
           float acc_x,acc_z;
           float gyro_z,gyro_x,gyro_y;   //xΪת����ٶ�
         
           /*IIC_Read_n_byte(MMA8451_Addr,MMA845x_OUT_X_MSB,2,MMA8451) ;
           x_acc=(MMA8451[0]<<8)+MMA8451[1];
           acc_x=-x_acc;*/
     
           IIC_Read_n_byte(MMA8451_Addr,MMA845x_OUT_Z_MSB,2,MMA8451) ;
           z_acc=(MMA8451[0]<<8)+MMA8451[1];
           acc_z=z_acc;
           
           //ACC_ANGLE=acc_z;
           ACC_ANGLE =(z_acc-550)*0.0056;
           
           if(ACC_ANGLE>=90)   ACC_ANGLE=90;
           if(ACC_ANGLE<=-90)   ACC_ANGLE=-90;
           //********************L3G�����ǲɼ�****************
           /*IIC_Read_n_byte(L3G4200_Addr,OUT_Z_H,2,L3G4200) ;       //ת����ٶ�
           gyro=(L3G4200[0]<<8)+L3G4200[1];
           gyro_z=gyro;*/
           
           IIC_Read_n_byte(L3G4200_Addr,OUT_X_H,2,L3G4200) ;      //ƽ����ٶ�
           gyro=(L3G4200[0]<<8)+L3G4200[1];
           gyro_x=gyro;
           
           IIC_Read_n_byte(L3G4200_Addr,OUT_Y_H,2,L3G4200);      //ƽ����ٶ�
           gyro=(L3G4200[0]<<8)+L3G4200[1];
           gyro_y=-gyro;
           
           //GYRO_X=0.0175*gyro_x;
           //GYRO_Z=0.0175*gyro_z;
           //GYRO_Y=0.0175*gyro_y;
           GYRO_Y=0.0275*gyro_y;
           GYRO_X=0.0175*gyro_x;//0.0175
          
#if 1       //һ�׻����˲�
           
           angleX=(0.9993)*(angleX+GYRO_Y*0.001)+0.0007*ACC_ANGLE;
   
#else      //���׻����˲�
           angleX=Comlementary2(ACC_ANGLE,GYRO_X);
           
#endif      
        //   Kalman_Filter(ACC_ANGLE,GYRO_Y);*/
          
}
float y1;
float Comlementary2(float newangle,float newgyro)  //���׻����˲�
{
        float k=10;          //�ɵ���
        float dt=0.001;
        float x1,x2;
        x1=(newangle-angleX)*k*k;
        y1=dt*x1+y1;
        x2=y1+(newangle-angleX)*k*2+newgyro;
        angleX=dt*x2+angleX;
        return angleX;
}
