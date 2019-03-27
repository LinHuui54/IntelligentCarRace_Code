
#ifndef _L3G4200D_I2C_H_
#define _L3G4200D_I2C_H_

#include "common.h"


extern uint8 iic_ack;

void L3g4200d_i2c_init();
void L3g4200d_start_i2c();
void L3g4200d_stop_i2c();
void L3g4200d_SendAck_i2c(uint8 a);
uint8 L3g4200d_RecvAck_i2c();
void L3g4200d_SendByte_i2c(uint8 c);
uint8 L3g4200d_RcvByte_i2c();

#endif