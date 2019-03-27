/*******************************************************************************
                         #ʮ���족�����֡�������ͷ����#
@��ʼ���飺            OK_GO
@�������ڣ�            2016-11-18
@���ܣ�                ��ȡ�������ߡ�

@��ʼ�������ԣ�       ����һֱ��д���淶���ṹ����������ά�����Ż��ĳ���
                      ϣ��������ܹ���ʦ����ϲ�������Ҵ�����ȥ��
                      ����ϣ��ʦ�ܿ��ԶԳ�������Ż�����ǿ���ܣ�������һζֱ��ʹ�á�
                      ���޸ĵ�ʱ������ѭ��ʽ�޸ġ�
                      ����������޸��ˣ���ȷ���޸Ĳ�����Ч�����¿�������Ϣ��
*******************************************************************************/
/*******************************************************************************
�����Գ���������Ż�����������������Ϣ����ʽ���ϣ�
*******************************************************************************/

#include "include.h"

uint8 L_nostarting_flag=0,R_nostarting_flag=0;    //û����־
int circle_flag=0;                                 //Բ����ʶ
uint16 circle_count=0;                             //Բ����־��ʱ
uint8  circle_count_flag=0;
uint8  show_data1=0;
uint16  stop_count=0;
int R_barrier_point=0,L_barrier_point=0;//�������ϰ��ĵ�
uint8 cross_limit_flag=0;
uint8 circle_adding_flag=0;
uint8 stop_b_flag=0;
uint8 start_line_barrir_flag=0;
uint8 zhizou_flag=0;
//*************************�㼶�Ӻ���*******************************************
/*
 *  @brief         ɨ������������
 *  @param         NULL
 *  Sample usage   ����ֵ��0��Чͼ��
                           1Ϊ��Чͼ��

 */
uint8 Get_Border()
{
    int8 i=0;
    uint8 S_or_F=0;
    uint8 starting_row=0;                      //����ɨ��һ��
    uint16 L_average=0,R_average=0;            //���ұ߽�ƽ��ֵ
    uint8 L_valid_count=0,R_valid_count=0;     //�����Ч��������
    uint8 L_start=0,R_start=0;                 //��������ɨ�����
    uint8 cross_flag=0;                        //ʮ�ֱ�־
    uint8 adding_border_flag=0;                //���߳ɹ���־
  
    cross_limit_flag=0;
    circle_adding_flag=0;
    //���ߣ����ߵ�  ����
    get_border_init();
    //������ʶ��,����10���Ӻ���
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
    if(stop_count>1800)//�ӳ�ʶ���ϰ�����
    {
      if(!b_start_flag)
        b_start_flag=1;
    }
   /* if(stop_count>2400||stop_count<2000)//||stop_count<250
      start_line_barrir_flag=1;
    else
      start_line_barrir_flag=0;*/
    //�����
    starting_row=get_starting_point(B_limit,T_limit);
    if(starting_row<50)//��ǰ��ͼ���̫��Ͳ��ж��ϰ�
      stop_b_flag=0;
    else
      stop_b_flag=1;    
    if(starting_row>59) starting_row=59;
    if(starting_row)//�����
      {
         //*******�������*********
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
         
         L_start=((L_average+range)<R_limit ? (L_average+range): R_limit);//��ֹԽ��
         R_start=((R_average-range)>L_limit ? (R_average-range): L_limit);//��ֹԽ��
         //**************************
         //����ɨ
         cross_flag=following_scan(starting_row,L_start,R_start);
         //ʮ�ִ���
         if(cross_flag)
         {
           uint8 b=0;
           uint8 flag=0;
           //��ʮ��ǰȷ������Բ��
           for(b=(L_limit+18);b<(R_limit-18);b++) //20��-20
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
            if(!adding_border_flag)//����ʧ��
            {
              //uint8 a=0;
              //a=cross_scanning(cross_flag-10,T_limit);
              //if((!a)&&flag)
              if(left_border[0]&&left_border[1]&&flag)
              {
                    //uint8 cc=0,bb=0;
                    uint8 ii;
                    circle_derror=150;
                    //Բ������
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
                    //Բ������
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
    else//û��㣬�п����ǻ���Բ��
    {
        S_or_F=0;
        //Բ������
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
    //Բ����������,��ĳ��ʱ��λ���г��ڲ���
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



//*************************һ���Ӻ���*******************************************
/*
 *  @brief         ɨ�����߳�ʼ�� 
 *  @param         NULL
 *  Sample usage  ��
 */
void get_border_init()
{
  uint8 i;
  //���ұ߽硢����ɨ�����ݻ����������߳�ʼ��
  for(i=0;i<CAMERA_H-10;i++)
  {
    left_border[i]=0;
    right_border[i]=0;
    img_center[i]=0;
    circle_flag=0;
  }
  //����ɨ�����ݻ�������ʼ��
}

/*
 *  @brief        Ѱ�Ҹ���ɨ�����
 *  @param        start       ɨ����ʼ��
 *  @param        end         ɨ����ֹ��   
 *  Sample usage  ���м�������ɨ����������ɨ��5����Ч���ݲ��˳��������������ɨ��
                  ʧ�ܷ���0
                  �ɹ���������
 */
int8 get_starting_point(uint8 start,uint8 end)
{
  uint8 i;                        
  int8 S_or_F=0;                   //ִ�гɹ�orʧ�ܱ�־
  uint8 valid_row=0;                //��Ч��
  uint8 L_valid_count=0,R_valid_count;              //��Ч�м���
  
  for(i=start;i>end;i--) 
  {
     uint8 Left_S_or_F=0,Right_S_or_F=0;//��ʱ����
     
     //��������ɨ
     Left_S_or_F=one_line_scanning(LEFT_POSITIVE,center,L_limit,i);
     Right_S_or_F=one_line_scanning(RIGHT_POSITIVE,center,R_limit,i);
     //������߶��գ�����ɨ
     if((Left_S_or_F||Right_S_or_F)==0)
     {
        Left_S_or_F=one_line_scanning(LEFT_ANASTROPHIC,center,L_limit,i);
        Right_S_or_F=one_line_scanning(RIGHT_ANASTROPHIC,center,R_limit,i);
        //������ɨ��ɨ�����߶���ֵ���ж�ΪԲ��
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
       
     //�������������Ч���˳�   
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
 *  @brief              ����ɨ��ͼ��
 *  @param  start_row   ɨ����ʼ��
 *  @param  L_start     ɨ������ʼ��
 *  @param  R_start     ɨ������ʼ��
 *  Sample  �ֱ��������ʼ�㿪ʼɨ������ɨ 
            ����0Ϊ��ʮ��
                ����Ϊʮ��
                
 */
uint8 following_scan(int start_row,uint8 L_start,uint8 R_start)
{
  int8  i;
  int8  S_or_F=0;                           //ִ�гɹ�orʧ�ܱ�־
  uint8 L_end=0,R_end=0;                    //ɨ�������
  uint8 L_empty_count=0,R_empty_count=0;    //���м���
  uint8 L_stop=0,R_stop=0;                  //������ֹͣɨ��־
  uint8 cross_stop=0;                       //ֹͣ�ж��Ƿ���ʮ��
  
  for(i=start_row;i>T_limit;i--)
  {
     uint8 L_judge=0,R_judge=0;    //��ɨɨ�������
     
     L_end=((L_start-range*2)>L_limit ? (L_start-range*2) : L_limit);
     R_end=((R_start+range*2)<R_limit ? (R_start+range*2) : R_limit);
     
     if(!L_stop)
       L_judge=one_line_scanning(LEFT_POSITIVE,L_start,L_end,i);
     if(!R_stop)
       R_judge=one_line_scanning(RIGHT_POSITIVE,R_start,R_end,i);
     
     //���ݽ��������Ӧ����ex������ɨ����ʼ�㣬ɨ��ֹͣ��ʮ���ж�ֹͣ*********
     if(L_judge&&!L_stop)
     {
       L_start=((left_border[N(i)]+range)>L_limit ? (left_border[N(i)]+range): L_limit);
       L_empty_count=0;
       L_nostarting_flag=0;
       //�ж�������ߺ��䣬�Ͳ�̫������ʮ��
       if(left_border[N(i)]>40)
         cross_stop=1;
       //Խ���ֹͣɨ��
       if(left_border[N(i)]>=(R_limit-2))
       {
         L_stop=1;
         left_border[N(i)]=0;
       }
     }
     else
     {
       L_empty_count++;
       //��������û����ͣ
       if(L_empty_count==10&&!L_nostarting_flag)//3
         L_stop=1;
     } 
     if(R_judge==1&&!R_stop)
     {
       R_start=((right_border[N(i)]-range)<R_limit ?  (right_border[N(i)]-range) : R_limit);
       R_empty_count=0;
       R_nostarting_flag=0;
       //�ж�������ߺ��䣬�Ͳ�̫������ʮ��
       if(right_border[N(i)]<40)
         cross_stop=1;
       //Խ���ֹͣɨ��
       if(right_border[N(i)]<=(L_limit+2))
       {
          R_stop=1;
          right_border[N(i)]=0;
       }
     }
     else
     {
       R_empty_count++;
       //��������û����ͣ
       if(R_empty_count==10&&!R_nostarting_flag)//3
         R_stop=1;
     }
     //*************************************************************************
     //ʮ���ж�,Ȼ���˳�
     if(!cross_stop&&(L_empty_count>=10)&&(R_empty_count>=10))//3��3
     {
       S_or_F=i;
       break;
     }
      //����ɨ�߶�ֹͣ�˾��˳�
     if(L_stop==1&&R_stop==1)
     {
       S_or_F=0;
       break;
     }
  }
  return S_or_F;
}

/*
 *  @brief        �ж�Ϊʮ�ֺ��ɨ���Ӻ���
 *  @param        start       ɨ����ʼ��
 *  @param        end         ɨ����ֹ��   
 *  Sample usage  NULL
 */
uint8 cross_scanning(uint8 start,uint8 end)
{
  uint8 i,count=0; 
  uint8 S_or_F=0;
  for(i=start;i>end;i--) 
  {
     uint8 Left_S_or_F=0,Right_S_or_F=0;//ɨ����ı�־
     
     //��������ɨ
     Left_S_or_F=one_line_scanning(LEFT_POSITIVE,center,L_limit,i);
     Right_S_or_F=one_line_scanning(RIGHT_POSITIVE,center,R_limit,i);
     //������߶��գ�����ɨ
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
 *  @brief   ʮ�ֲ���
 *  @param   NULL   
 *  Sample   ��ʮ�����沿��������
             ʧ�ܷ���0
             �ɹ�����1
 */

//����ٸ�
uint8 adding_border()
{
  int8 i;
  uint8 S_or_F=1;
  uint8 left_end_i=0,right_end_i=0;         //��¼
  uint8 left_start_i=0,right_start_i=0;     //��¼��һ���ڵ������
  int8 left_distance=0,right_distance=0;
  uint8 left_flag=0,right_flag=0;           //�Ѿ��ҵ���һ���ڵ��־
  
  for(i=0;i<CAMERA_H-10;i++)
  {
    //�ҵ�һ����
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
    //�Ҷϵ�
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
    //�ҵ������ϵ��˳�ѭ��
    if(left_end_i!=0&&right_end_i!=0)
      break;
  }
  
  //�ҵ���߶ϵ㣬Ȼ���������
  if(left_end_i)
  {
    if(left_end_i-left_start_i>=5)//8
    {
         S_or_F=1;
        //��һ������ϵ�Ĳ�ֵ
        left_distance=abs(left_border[left_end_i]-left_border[left_start_i]);
        //��������
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
  
  //�ҵ��ұ߶ϵ㣬Ȼ���������
  if(right_end_i)
  {
    if(right_end_i-right_start_i>=5)//8
    {
      S_or_F=1;
      //��һ������ϵ�Ĳ�ֵ
      right_distance=abs(right_border[right_start_i]-right_border[right_end_i]);
      //��������
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

//**************************�����Ӻ���*******************************************
/*
 *  @brief         ɨ��һ�л�һ��
 *  @param scanning_modeɨ�跽ʽ   
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  ����ɨ�跽ʽ����ȡ��ͬɨ�跽��������Ҫע���������ɨ����starΪ
                  ɨ�����ʼ��
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 one_line_scanning(int scanning_mode,int star,int end,int R_or_C )
{
  uint8 S_or_F=0;                   //ִ�гɹ�orʧ�ܱ�־
  
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


//*************************�����Ӻ���*******************************************
/*
 *  @brief      ��������ɨ��
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  ����ʼ������ɨ��ɨ���׺ں��˳�
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 left_positive(int start,int end,int R_or_C )
{
    uint8 spot;
    uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
    //���м������ɨ
    for(spot=start;spot>end;spot--)
    {
        //������㣬�׵��ں�
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
 *  @brief      ���ҷ���ɨ��
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  ���������ɨ��ɨ���ڰװ��˳�
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 left_anastrophic(int start,int end,int R_or_C )
{
   uint8 spot;
   uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
   //���м����ұ�ɨ
   for(spot=start;spot>end;spot--)
      {
       //������㣬�ڵ��װ�
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
 *  @brief      ��������ɨ��
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  ����ʼ������ɨ��ɨ���׺ں��˳�
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 right_positive(int start,int end,int R_or_C )
{
   uint8 spot;
   uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
  //���м����ұ�ɨ
   for(spot=start;spot<end;spot++)
     {
        //������㣬�׵��ں�
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
 *  @brief      ���ҷ���ɨ��
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  �������ɨ��ɨ���ڰװ��˳�
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 right_anastrophic(int start,int end,int R_or_C)
{
   uint8 spot;
   uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
   //���м������ɨ
   for(spot=start;spot<end;spot++)
      {
       //������㣬�ڵ��װ�
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
 *  @brief      ��������ɨ��
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  ����ʼ������ɨ��ɨ���׺ں��˳�
                  �ɹ�����1��ʧ�ܷ���0
 */
uint8 up_positive(uint8 C)
{
    uint8 spot;
    uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
    //���м�����ɨ
    for(spot=59;spot>18;spot--)//59��15
    {
        //������㣬�׵��ں�
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
 *  @brief      Բ������
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
 *  @brief      ��������ɨ�������
 *  @param      start   ɨ����ʼ��    
 *  @param      end     ɨ�������
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  
                  �а����߷���1��û�з���0
 */
uint8 Zebra_crossing(int R_or_C)
{
   uint8 spot;
   uint8 S_or_F=0;                 //ִ�гɹ�orʧ�ܱ�־
   uint8 zebra_count=0;
   //���м������ɨ
   for(spot=L_limit;spot<R_limit;spot++)
      {
       //������㣬�ڵ��װ�
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
 *  @brief      ���м�������ɨ����㣨�ϰ���
 *  @param      R_or_C  ��ǰɨ���л���
 *  Sample usage  null
 */
void scan_barrier(int R_or_C )
{
   uint8 spot;
  //���м����ұ�ɨ
   R_barrier_point=0;
   L_barrier_point=0;
   for(spot=40;spot<R_limit;spot++)
     {
        //������㣬�׵��ں�
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot+1]==0&&img[R_or_C][spot+2]==0)
         {
           R_barrier_point=spot-1;
           break;
         }     
     }
   
    //���м������ɨ
    for(spot=40;spot>L_limit;spot--)
    {
        //������㣬�׵��ں�
        if(img[R_or_C][spot]==0xff&&img[R_or_C][spot-1]==0&&img[R_or_C][spot-2]==0)
        {
          L_barrier_point=spot+1;
          break;
        }
    }
}


//******************************************************************************




