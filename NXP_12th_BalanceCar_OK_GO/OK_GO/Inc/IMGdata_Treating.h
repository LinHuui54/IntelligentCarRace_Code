/*******************************************************************************
                         #十二届”恩智浦“杯摄像头代码#
@创始队伍：            OK_GO
@创建日期：            2016-11-30
@功能：                算出曲率。

@创始队伍留言：       我们一直想写出规范，结构清晰，便于维护和优化的程序。
                      希望这程序能够被师弟们喜欢，并且传承下去。
                      而且希望师弟可以对程序进行优化，加强功能，而不是一味直接使用。
                      在修改的时候请遵循格式修改。
                      如果有那里修改了，请确定修改部分有效后，在下框留下信息！
*******************************************************************************/
/*******************************************************************************
如果你对程序进行了优化，请在这里留下信息，格式如上！
*******************************************************************************/
#ifndef IMGDATA_TREATING_H
#define IMGDATA_TREATING_H

#include "include.h"
#include "common.h"

/*#define   Turnning_P       9      //8
#define   Turnning_D       0.6    //0.45*/
 
#define  dynamic_ratio     0.1
  
//控制速度pid
void Speed_control(int left_speed,int right_speed);
int Get_derror();
int Turnning_PID(int derror);



extern int  circle_derror;
extern int  show_data0;
extern int barrier_flag;//障碍标志
extern int ramp_flag1;//坡道标志
extern int ramp_stop_flag;
//extern int barrier_stop_flag;//检测过就停止检测
extern uint8 b_start_flag;

#endif