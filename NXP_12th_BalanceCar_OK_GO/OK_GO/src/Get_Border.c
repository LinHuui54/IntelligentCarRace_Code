/*******************************************************************************
                         #十二届”恩智浦“杯摄像头代码#
@创始队伍：            OK_GO
@创建日期：            2016-11-18
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

#include "include.h"

uint8 L_nostarting_flag=0,R_nostarting_flag=0;    //没起点标志
int circle_flag=0;                                 //圆环标识
uint16 circle_count=0;                             //圆环标志延时
uint8  circle_count_flag=0;
uint8  show_data1=0;
uint16  stop_count=0;
int R_barrier_point=0,L_barrier_point=0;//可能是障碍的点
uint8 cross_limit_flag=0;
uint8 circle_adding_flag=0;
uint8 stop_b_flag=0;
uint8 start_line_barrir_flag=0;
uint8 zhizou_flag=0;
//*************************零级子函数*******************************************
/*
 *  @brief         扫描中线主函数
 *  @param         NULL
 *  Sample usage   返回值：0无效图像
                           1为有效图像

 */
uint8 Get_Border()
{
    int8 i=0;
    uint8 S_or_F=0;
    uint8 starting_row=0;                      //跟踪扫第一行
    uint16 L_average=0,R_average=0;            //左右边界平均值
    uint8 L_valid_count=0,R_valid_count=0;     //起点有效行数计数
    uint8 L_start=0,R_start=0;                 //传给跟踪扫的起点
    uint8 cross_flag=0;                        //十字标志
    uint8 adding_border_flag=0;                //补线成功标志
  
    cross_limit_flag=0;
    circle_adding_flag=0;
    //中线，边线等  清零
    get_border_init();
    //斑马线识别,大于10秒钟后检测
    if(stop_count>3000)//3000
    {
     /* else
        start_line_barrir_flag=0;*/
      if(Zebra_crossing(30))
      {
        stop_car=1;
      }
    }
    if(stop_count<3000)
    {
      if(Zebra_crossing(30))
        zhizou_flag=1;
      else
        zhizou_flag=0;
    }
    else
      zhizou_flag=0;
    if(stop_count>1800)//延迟识别障碍！！
    {
      if(!b_start_flag)
        b_start_flag=1;
    }
   /* if(stop_count>2400||stop_count<2000)//||stop_count<250
      start_line_barrir_flag=1;
    else
      start_line_barrir_flag=0;*/
    //找起点
    starting_row=get_starting_point(B_limit,T_limit);
    if(starting_row<50)//若前面图像空太多就不判断障碍
      stop_b_flag=0;
    else
      stop_b_flag=1;    
    if(starting_row>59) starting_row=59;
    if(starting_row)//有起点
      {
         //*******计算起点*********
         for(i=N(starting_row)-1;i>=N(starting_row)-5;i--)
         {
           if(left_border[i]!=0)
             L_valid_count++;
           if(right_border[i]!=0)
             R_valid_count++;
           
           L_average+=left_border[i];
           R_average+=right_border[i];
         } 
         if(L_valid_count==5)
           L_average/=L_valid_count;
         else
         {
           L_average=L_limit;
           L_nostarting_flag=1;
         }
         if(R_valid_count==5)
           R_average/=R_valid_count;
         else
         {
           R_average=R_limit;
           R_nostarting_flag=1;
         }
         
         L_start=((L_average+range)<R_limit ? (L_average+range): R_limit);//防止越界
         R_start=((R_average-range)>L_limit ? (R_average-range): L_limit);//防止越界
         //**************************
         //跟踪扫
         cross_flag=following_scan(starting_row,L_start,R_start);
         //十字处理
         if(cross_flag)
         {
           uint8 b=0;
           uint8 flag=0;
           //补十字前确定不是圆环
           for(b=(L_limit+18);b<(R_limit-18);b++) //20，-20
           {
              if(!up_positive(b))
              {
                adding_border_flag=adding_border();
                if(adding_border_flag)
                {
                  cross_limit_flag=1;
                  flag=0;
                }
                break;
              }
              else if(b>38)
                flag=1;
           }
            if(!adding_border_flag)//部线失败
            {
              //uint8 a=0;
              //a=cross_scanning(cross_flag-10,T_limit);
              //if((!a)&&flag)
              if(left_border[0]&&left_border[1]&&flag)
              {
                    //uint8 cc=0,bb=0;
                    uint8 ii;
                    circle_derror=150;
                    //圆环补线
                    uint8 a=0;
                    circle_count_flag=1;
                    circle_count=0;
                    up_positive(37);
                    a=circle_flag;
                    up_positive(43);
                    if(circle_flag>a)
                      a=circle_flag;
                    circle_flag=a;
                    for(ii=0;ii<N(cross_flag);ii++)
                    {
                      left_border[ii]=0;
                      right_border[ii]=0;
                    }
                     if((up_positive(37)||up_positive(43)))
                     {
                        adding_circle_border();
                        circle_adding_flag=1;
                     }
              }
              else if(right_border[0]&&right_border[1]&&flag)
              {
                    uint8 ii;
                    circle_derror=-150;
                    //圆环补线
                    uint8 a=0;
                    circle_count_flag=1;
                    circle_count=0;
                    up_positive(37);
                    a=circle_flag;
                    up_positive(43);
                    if(circle_flag>a)
                      a=circle_flag;
                    circle_flag=a;
                    for(ii=0;ii<N(cross_flag);ii++)
                    {
                      left_border[ii]=0;
                      right_border[ii]=0;
                    }
                     if(up_positive(37)||up_positive(43))
                     {
                        adding_circle_border();
                        circle_adding_flag=1;
                     }
              }
              else
                circle_adding_flag=0;
            }
         }
         S_or_F=1;
      }
    else//没起点，有可能是基础圆环
    {
        S_or_F=0;
        //圆环补线
        if(circle_flag)
        {
          uint8 a=0;
          //stop_car=1;
          circle_count_flag=1;
          circle_count=0;
          up_positive(37);
          a=circle_flag;
          up_positive(43);
          if(circle_flag>a)
            a=circle_flag;
          circle_flag=a;
           if(up_positive(37)||up_positive(43))
           {
              adding_circle_border();
              circle_adding_flag=1;
           }
        }
        else
          circle_adding_flag=0;
    }
    //圆环进入后计数,在某个时间段会进行出口补线
    /*if(circle_count_flag)//circle_count_flag
      circle_count++;
    if(circle_count>0)
    {
        uint8 i=0;
        uint8 flag=0;
        if(circle_count>10)
        {
          //stop_car=1;
          circle_count_flag=0;
          circle_count=0;
        }
        for(i=0;i<20;i++)
        {
          if(left_border[i]==0&&right_border[i]==0)
            flag=1;
          else
            {
                flag=0;
                break;
            }
        }
        if(flag==1)
        {
          uint8 a=0;
          up_positive(10);
          if(circle_flag>a)
             a=circle_flag;
          up_positive(70);
          if(circle_flag>a)
            a=circle_flag;
          /* up_positive(40);
          if(circle_flag>a)
            a=circle_flag;*/
         /*                                         circle_flag=a;
          adding_circle_border();
          circle_adding_flag=1;
        }
    }*/
    
    return S_or_F;
}

//******************************************************************************



//*************************一级子函数*******************************************
/*
 *  @brief         扫描中线初始化 
 *  @param         NULL
 *  Sample usage  无
 */
void get_border_init()
{
  uint8 i;
  //左右边界、纵向扫描数据缓冲区和中线初始化
  for(i=0;i<CAMERA_H-10;i++)
  {
    left_border[i]=0;
    right_border[i]=0;
    img_center[i]=0;
    circle_flag=0;
  }
  //纵向扫描数据缓冲区初始化
}

/*
 *  @brief        寻找跟踪扫提起点
 *  @param        start       扫描起始行
 *  @param        end         扫描终止行   
 *  Sample usage  从中间向两边扫，单边连续扫到5行有效数据才退出，否则继续往下扫。
                  失败返回0
                  成功返回行数
 */
int8 get_starting_point(uint8 start,uint8 end)
{
  uint8 i;                        
  int8 S_or_F=0;                   //执行成功or失败标志
  uint8 valid_row=0;                //有效行
  uint8 L_valid_count=0,R_valid_count;              //有效行计数
  
  for(i=start;i>end;i--) 
  {
     uint8 Left_S_or_F=0,Right_S_or_F=0;//临时变量
     
     //左右正向扫
     Left_S_or_F=one_line_scanning(LEFT_POSITIVE,center,L_limit,i);
     Right_S_or_F=one_line_scanning(RIGHT_POSITIVE,center,R_limit,i);
     //如果两边都空，则反向扫
     if((Left_S_or_F||Right_S_or_F)==0)
     {
        Left_S_or_F=one_line_scanning(LEFT_ANASTROPHIC,center,L_limit,i);
        Right_S_or_F=one_line_scanning(RIGHT_ANASTROPHIC,center,R_limit,i);
        //反过来扫，扫到两边都有值，判断为圆环
        if(Right_S_or_F&&Left_S_or_F)
        {
          S_or_F=0;
          circle_flag=1;
          break;
        }      
     }
     if(Left_S_or_F)
     {
       L_valid_count++;
       valid_row=i;
       //left_sum+=left_border[N(valid_row)];
     }
     else
     {
       L_valid_count=0;
     }
     if(Right_S_or_F)
     {
       R_valid_count++;
       valid_row=i;
       //right_sum+=right_border[N(valid_row)];
     }
     else
     {
       R_valid_count=0;
     }
       
     //如果连续五行有效，退出   
     if((L_valid_count>=5&&R_valid_count<=2)||(R_valid_count>=5&&L_valid_count<=2)||(L_valid_count>=5&&R_valid_count>=5))
     {
          S_or_F=valid_row-1;
        break;
     }
     else
       S_or_F=0;
  }
  
  return S_or_F;

}

/*
 *  @brief              跟踪扫描图像
 *  @param  start_row   扫描起始行
 *  @param  L_start     扫描左起始点
 *  @param  R_start     扫描右起始点
 *  Sample  分别从左右起始点开始扫，跟踪扫 
            返回0为非十字
                行数为十字
                
 */
uint8 following_scan(int start_row,uint8 L_start,uint8 R_start)
{
  int8  i;
  int8  S_or_F=0;                           //执行成功or失败标志
  uint8 L_end=0,R_end=0;                    //扫描结束点
  uint8 L_empty_count=0,R_empty_count=0;    //空行计数
  uint8 L_stop=0,R_stop=0;                  //左右线停止扫标志
  uint8 cross_stop=0;                       //停止判断是否是十字
  
  for(i=start_row;i>T_limit;i--)
  {
     uint8 L_judge=0,R_judge=0;    //正扫扫到跳变点
     
     L_end=((L_start-range*2)>L_limit ? (L_start-range*2) : L_limit);
     R_end=((R_start+range*2)<R_limit ? (R_start+range*2) : R_limit);
     
     if(!L_stop)
       L_judge=one_line_scanning(LEFT_POSITIVE,L_start,L_end,i);
     if(!R_stop)
       R_judge=one_line_scanning(RIGHT_POSITIVE,R_start,R_end,i);
     
     //根据结果做出相应处理，ex：计算扫描起始点，扫线停止，十字判断停止*********
     if(L_judge&&!L_stop)
     {
       L_start=((left_border[N(i)]+range)>L_limit ? (left_border[N(i)]+range): L_limit);
       L_empty_count=0;
       L_nostarting_flag=0;
       //判断如果左线很弯，就不太可能是十字
       if(left_border[N(i)]>40)
         cross_stop=1;
       //越界就停止扫线
       if(left_border[N(i)]>=(R_limit-2))
       {
         L_stop=1;
         left_border[N(i)]=0;
       }
     }
     else
     {
       L_empty_count++;
       //连续三行没到就停
       if(L_empty_count==10&&!L_nostarting_flag)//3
         L_stop=1;
     } 
     if(R_judge==1&&!R_stop)
     {
       R_start=((right_border[N(i)]-range)<R_limit ?  (right_border[N(i)]-range) : R_limit);
       R_empty_count=0;
       R_nostarting_flag=0;
       //判断如果右线很弯，就不太可能是十字
       if(right_border[N(i)]<40)
         cross_stop=1;
       //越界就停止扫线
       if(right_border[N(i)]<=(L_limit+2))
       {
          R_stop=1;
          right_border[N(i)]=0;
       }
     }
     else
     {
       R_empty_count++;
       //连续三行没到就停
       if(R_empty_count==10&&!R_nostarting_flag)//3
         R_stop=1;
     }
     //*************************************************************************
     //十字判断,然后退出
     if(!cross_stop&&(L_empty_count>=10)&&(R_empty_count>=10))//3，3
     {
       S_or_F=i;
       break;
     }
      //两边扫线都停止了就退出
     if(L_stop==1&&R_stop==1)
     {
       S_or_F=0;
       break;
     }
  }
  return S_or_F;
}

/*
 *  @brief        判断为十字后的扫描子函数
 *  @param        start       扫描起始行
 *  @param        end         扫描终止行   
 *  Sample usage  NULL
 */
uint8 cross_scanning(uint8 start,uint8 end)
{
  uint8 i,count=0; 
  uint8 S_or_F=0;
  for(i=start;i>end;i--) 
  {
     uint8 Left_S_or_F=0,Right_S_or_F=0;//扫到点的标志
     
     //左右正向扫
     Left_S_or_F=one_line_scanning(LEFT_POSITIVE,center,L_limit,i);
     Right_S_or_F=one_line_scanning(RIGHT_POSITIVE,center,R_limit,i);
     //如果两边都空，则反向扫
     if((Left_S_or_F||Right_S_or_F)==0)
     {
        Left_S_or_F=one_line_scanning(LEFT_ANASTROPHIC,center,L_limit,i);
        Right_S_or_F=one_line_scanning(RIGHT_ANASTROPHIC,center,R_limit,i);
     }
     if((Left_S_or_F||Right_S_or_F))
       count++;
  } 
  if(count>0)
    S_or_F=1;
  else
    S_or_F=0;
  
  return S_or_F;
}

/*
 *  @brief   十字补线
 *  @param   NULL   
 *  Sample   将十字下面部分往上移
             失败返回0
             成功返回1
 */

//最后再改
uint8 adding_border()
{
  int8 i;
  uint8 S_or_F=1;
  uint8 left_end_i=0,right_end_i=0;         //记录
  uint8 left_start_i=0,right_start_i=0;     //记录第一个黑点的行数
  int8 left_distance=0,right_distance=0;
  uint8 left_flag=0,right_flag=0;           //已经找到第一个黑点标志
  
  for(i=0;i<CAMERA_H-10;i++)
  {
    //找第一个点
    if(!left_flag)
      if((left_border[i]==0&&left_border[i+1]!=0&&left_border[i+2]!=0)||left_border[i]!=0)
      {
        left_start_i=i+1;
        left_flag=1;
      }
    if(!right_flag)
      if((right_border[i]==0&&right_border[i+1]!=0&&right_border[i+2]!=0)||right_border[i]!=0)
      {
        right_start_i=i+1;
        right_flag=1;
      }
    //找断点
    if(!left_end_i&&left_flag)
      if(left_border[i]!=0&&left_border[i+1]==0&&left_border[i+2]==0)
      {
         int8 j=0;
         if((i-3)>5)
         {
           left_end_i=i-3;
           for(j=i;j>i-3;j--)
              left_border[j]=0;
         }
      }
    if(!right_end_i&&right_flag)
      if(right_border[i]!=0&&right_border[i+1]==0&&right_border[i+2]==0)
      {
        int8 j=0;
        if((i-3)>5)
        {
          right_end_i=i-3;
          for(j=i;j>i-3;j--)
            right_border[j]=0;
        }
      }
    //找到两个断点退出循环
    if(left_end_i!=0&&right_end_i!=0)
      break;
  }
  
  //找到左边断点，然后进行移线
  if(left_end_i)
  {
    if(left_end_i-left_start_i>=5)//8
    {
         S_or_F=1;
        //第一个点与断点的差值
        left_distance=abs(left_border[left_end_i]-left_border[left_start_i]);
        //进行上移
          for(i=left_end_i;i<50;i++)
          {
            if(left_border[i]==0)
              left_border[i]=left_border[left_start_i]+left_distance;
            left_start_i++;
          }
    }
    else
      S_or_F=0;
  }
  else
    S_or_F=0;
  
  //找到右边断点，然后进行移线
  if(right_end_i)
  {
    if(right_end_i-right_start_i>=5)//8
    {
      S_or_F=1;
      //第一个点与断点的差值
      right_distance=abs(right_border[right_start_i]-right_border[right_end_i]);
      //进行上移
      if(right_distance>1)
        for(i=right_end_i;i<50;i++)
          {
            if(right_border[i]==0)
              right_border[i]=right_border[right_start_i]-right_distance;
            right_start_i++;
          }
       else
          S_or_F=0;
    }
  }
  else
    S_or_F=0;
  return S_or_F;
}
//******************************************************************************

//**************************二级子函数*******************************************
/*
 *  @brief         扫描一行或一列
 *  @param scanning_mode扫描方式   
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  根据扫描方式，采取不同扫描方法，其中要注意的是纵向扫描中star为
                  扫描的起始行
                  成功返回1，失败返回0
 */
uint8 one_line_scanning(int scanning_mode,int star,int end,int R_or_C )
{
  uint8 S_or_F=0;                   //执行成功or失败标志
  
  switch (scanning_mode)
  {
    case LEFT_POSITIVE:
      S_or_F=left_positive(star,end,R_or_C);
      break;
    case LEFT_ANASTROPHIC:
      S_or_F=left_anastrophic(star,end,R_or_C);
      break;
    case RIGHT_POSITIVE:
      S_or_F=right_positive(star,end,R_or_C);
      break;
    case RIGHT_ANASTROPHIC:
      S_or_F=right_anastrophic(star,end,R_or_C);
      break;
  }
  return S_or_F;
}
//******************************************************************************


//*************************三级子函数*******************************************
/*
 *  @brief      向左正向扫描
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  从起始点向左扫，扫到白黑黑退出
                  成功返回1，失败返回0
 */
uint8 left_positive(int start,int end,int R_or_C )
{
    uint8 spot;
    uint8 S_or_F=0;                 //执行成功or失败标志
    //从中间向左变扫
    for(spot=start;spot>end;spot--)
    {
        //找跳变点，白到黑黑
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot-1]==0&&img[R_or_C][spot-2]==0)
        {
          left_border[N(R_or_C)]=spot+1;
          S_or_F=1;
          break;
        }
    }
    if(left_border[N(R_or_C)]==0)
      S_or_F=0;
    return S_or_F;
    
}

/*
 *  @brief      向右反向扫描
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  从起点向右扫，扫到黑白白退出
                  成功返回1，失败返回0
 */
uint8 left_anastrophic(int start,int end,int R_or_C )
{
   uint8 spot;
   uint8 S_or_F=0;                 //执行成功or失败标志
   //从中间向右变扫
   for(spot=start;spot>end;spot--)
      {
       //找跳变点，黑到白白
        if(img[R_or_C][spot]==0&&img[R_or_C][spot-1]==0xff&&img[R_or_C][spot-2]==0xff)
           {
             right_border[N(R_or_C)]=spot-1;
             S_or_F=1;
             break;
           }
      }
   if(right_border[N(R_or_C)]==0)
     S_or_F=0;
   return S_or_F;
}

/*
 *  @brief      向右正向扫描
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  从起始点向右扫，扫到白黑黑退出
                  成功返回1，失败返回0
 */
uint8 right_positive(int start,int end,int R_or_C )
{
   uint8 spot;
   uint8 S_or_F=0;                 //执行成功or失败标志
  //从中间往右边扫
   for(spot=start;spot<end;spot++)
     {
        //找跳变点，白到黑黑
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot+1]==0&&img[R_or_C][spot+2]==0)
         {
           right_border[N(R_or_C)]=spot-1;
           S_or_F=1;
           break;
         }     
     }
   if(right_border[N(R_or_C)]==0)
     S_or_F=0;
   return S_or_F;
}

/*
 *  @brief      向右反向扫描
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  起点向左扫，扫到黑白白退出
                  成功返回1，失败返回0
 */
uint8 right_anastrophic(int start,int end,int R_or_C)
{
   uint8 spot;
   uint8 S_or_F=0;                 //执行成功or失败标志
   //从中间向左边扫
   for(spot=start;spot<end;spot++)
      {
       //找跳变点，黑到白白
       if(img[R_or_C][spot]==0&&img[R_or_C][spot+1]==0xff&&img[R_or_C][spot+2]==0xff)
        {
           left_border[N(R_or_C)]=spot+1;
           S_or_F=1;
           break;
         }
      }
   if(left_border[N(R_or_C)]==0)
     S_or_F=0;
   return S_or_F;
}

/*
 *  @brief      向上正向扫描
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  从起始点向上扫，扫到白黑黑退出
                  成功返回1，失败返回0
 */
uint8 up_positive(uint8 C)
{
    uint8 spot;
    uint8 S_or_F=0;                 //执行成功or失败标志
    //从中间向上扫
    for(spot=59;spot>18;spot--)//59，15
    {
        //找跳变点，白到黑黑
        if(img[spot][C]==0xff&&img[spot-1][C]==0&&img[spot-2][C]==0)
        {
          circle_flag=spot;
          S_or_F=1;
          break;
        }
    }
    return S_or_F;  
}


/*
 *  @brief      圆环补线
 */
void adding_circle_border()
{
  float add=0;
  uint8 i=0;
  add=40/N(circle_flag)+0.8;
  for(i=59;i>circle_flag;i--)
  {
    if(circle_derror<0)
      left_border[N(i)]=(int)((add)*N(i));
    else
      right_border[N(i)]=(int)(80-(add)*N(i));   
  }
  for(i=circle_flag;i>T_limit;i--)
  {
    left_border[N(i)]=0;
    right_border[N(i)]=0;
  }
  
}


/*
 *  @brief      从左向右扫描斑马线
 *  @param      start   扫描起始点    
 *  @param      end     扫描结束点
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  
                  有斑马线返回1，没有返回0
 */
uint8 Zebra_crossing(int R_or_C)
{
   uint8 spot;
   uint8 S_or_F=0;                 //执行成功or失败标志
   uint8 zebra_count=0;
   //从中间向左边扫
   for(spot=L_limit;spot<R_limit;spot++)
      {
       //找跳变点，黑到白白
       if(img[R_or_C][spot]==0&&img[R_or_C][spot+1]==0xff&&img[R_or_C][spot+2]==0xff)
        {
           zebra_count++;
        }
      }
   if(zebra_count>6)
     S_or_F=1;
   show_data1=S_or_F;
   return S_or_F;
}

/*
 *  @brief      从中间向两边扫跳变点（障碍）
 *  @param      R_or_C  当前扫的行或列
 *  Sample usage  null
 */
void scan_barrier(int R_or_C )
{
   uint8 spot;
  //从中间往右边扫
   R_barrier_point=0;
   L_barrier_point=0;
   for(spot=40;spot<R_limit;spot++)
     {
        //找跳变点，白到黑黑
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot+1]==0&&img[R_or_C][spot+2]==0)
         {
           R_barrier_point=spot-1;
           break;
         }     
     }
   
    //从中间向左变扫
    for(spot=40;spot>L_limit;spot--)
    {
        //找跳变点，白到黑黑
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot-1]==0&&img[R_or_C][spot-2]==0)
        {
          L_barrier_point=spot+1;
          break;
        }
    }
}


//******************************************************************************




