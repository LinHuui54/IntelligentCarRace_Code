/*******************************************************************************
                         #十二届”恩智浦“杯摄像头代码#
@创始队伍：            OK_GO
@创建日期：            2016-11-19
@功能：                提取赛道中线。

@创始队伍留言：       我们一直想写出规范，结构清晰，便于维护和优化的程序。
                      希望这程序能够被师弟们喜欢，并且传承下去。
                      而且希望师弟可以对程序进行优化，加强功能，而不是一味直接使用。
                      在修改的时候请遵循格式修改。
                      如果有那里修改了，请确定修改部分有效后，在下框留下信息！
*******************************************************************************/
/*******************************************************************************
如果你对程序进行了优化，请在这里留下信息，格式如上！
*******************************************************************************/

#ifndef GET_BORDER_H
#define GET_BORDER_H

#include "include.h"
#include "common.h"

#define   center    (CAMERA_W/2)    //图像中心
#define   L_limit   (CAMERA_W-78)     //图像左边界
#define   R_limit   (CAMERA_W-3)   //图像右边界
#define   T_limit   (9)     //图像顶边界
#define   B_limit   (59)

#define   range     (5)     //扫描的范围



#define   N(row)    (CAMERA_H-1-row) //图像行数与存储数据下标的关系（我也不知道怎么说，自己想吧，或者问我）

//扫描方式
enum scanning_mode
{
  
  LEFT_POSITIVE,     //横向_向左_正向
  LEFT_ANASTROPHIC,  //横向_向左_反向
  
  RIGHT_POSITIVE,    //横向_向右_正向
  RIGHT_ANASTROPHIC, //横向_向右_反向
  
};

extern  uint8 img[CAMERA_H][CAMERA_W];      
extern int left_border[CAMERA_H-10];                
extern int right_border[CAMERA_H-10];
extern int img_center[CAMERA_H-10];
extern int circle_flag;
extern uint16 circle_count;
extern uint8  show_data1;
extern int R_barrier_point,L_barrier_point;//可能是障碍的点
extern uint8 cross_limit_flag;
extern uint8 circle_adding_flag;
extern uint8 stop_b_flag;
extern uint8 start_line_barrir_flag;
extern uint8 zhizou_flag;



//提取中线主函数，成功返回1，失败则返回0
uint8 Get_Border();                       

//提出中线变量初始化
void get_border_init();
//寻找跟踪扫起点，成功返回起始点行数，失败返回0 
int8 get_starting_point(uint8 start,uint8 end);
//横向扫描，成功返回1，失败返回最后扫描行数
uint8 following_scan(int start_row,uint8 L_start,uint8 R_start);
//判断为十字后的扫线
uint8 cross_scanning(uint8 start,uint8 end);
//十字补线
uint8 adding_border();

//单行扫描，成功返回1，失败返回0
uint8 one_line_scanning(int scanning_mode,int star,int end,int R_or_C );

//横向_向左_正向
uint8 left_positive(int start,int end,int R_or_C );
//横向_向左_反向
uint8 left_anastrophic(int start,int end,int R_or_C );
//横向_向右_正向
uint8 right_positive(int start,int end,int R_or_C );
//横向_向右_反向
uint8 right_anastrophic(int start,int end,int R_or_C );
//纵向_向上_正向
uint8 up_positive(uint8 C);
//圆环补线
void adding_circle_border();
//斑马线识别
uint8 Zebra_crossing(int R_or_C);
//障碍检测
void scan_barrier(int R_or_C);


#endif