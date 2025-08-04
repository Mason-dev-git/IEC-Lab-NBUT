#include "PatrolLine.hpp"

uint16_t ad_now[10]={0};//adc采样当前值
uint16_t ad_max[10]={0};//adc采样最大值
uint16_t ad_min[10]={4095, 4095, 4095, 4095, 4095, 4095,4095, 4095};//adc采样最小值
uint16_t ad_avr[10]={2048, 2048, 2048, 2048, 2048, 2048,2048, 2048};//adc采样平均值
int32_t *error_avr=new int32_t[10]; //误差平均
float yaw_compensation;
PatrolLine::PatrolLine(/* args */)
{
  compare_ad_left=4;
  compare_ad_right=5;
}

PatrolLine::~PatrolLine()
{
}

void PatrolLine::Init(void)
{
 HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ad_now, 10);//参数：句柄；地址；长度
}

/*巡线值最大最小处理*/
void PatrolLine::Value_Handling(void)//巡线值最大最小处理
{
  for(uint8_t i=0; i<10; i++)
  {
    if(ad_now[i] < ad_min[i])
    {
      if(ad_now[i]==0)
      {

      }
      else 
      {
        ad_min[i] = ad_now[i];
      }
    }
      
    if(ad_now[i] > ad_max[i])
      ad_max[i] = ad_now[i];
    ad_avr[i] = (ad_max[i]+ad_min[i])/2;
//		ad_now[i]=ad_now[i]-4096;
//		ad_now[i]=abs(ad_now[i]);
  }
}

void PatrolLine::SetPID_Kp_Kd(float kp, float kd)//设置PID参数
{
  PatrolLinePID.Set_Kp_Kd(kp, kd);
}
void PatrolLine::SetPID_Kp_Kd_yaw(float kp, float kd)// 设置陀螺仪PID参数
{
  PatrolLinePID.Set_Kp_Kd_yaw(kp, kd);// 调用PID控制器的方法来设置陀螺仪的比例和微分系数
}
 //PID设置最大输出
void PatrolLine::SetPID_MaxOutput(float Set_MaxOutput)
{
  PatrolLinePID.Set_MaxOutput(Set_MaxOutput);
}
/*设置比较AD通道*/
void PatrolLine::SetcompareAD(uint8_t left,uint8_t right)
{
  compare_ad_left=left;
  compare_ad_right=right;
}
int32_t PatrolLine::Get_error()//ADC动态误差
{
  /*不设初始值会溢出*/
  int32_t res_avr=0,res_total=0;                 
  int32_t res = (int32_t)(ad_now[compare_ad_left]-ad_now[compare_ad_right]);
  error_avr[10]=res;
      for(uint8_t i=0;i<10;i++)
    {
      error_avr[i]=error_avr[i+1];
      res_total+=error_avr[i];
    }
      if(error_avr[0]!=0)
    {
      res_avr=res_total/10;
    }
  return res_avr;
}
float PatrolLine::Get_yaw_compensation()
{
   return yaw_compensation;
}
void PatrolLine::set_yaw_compensation()
{
	yaw_compensation=yaw;
}

float PatrolLine::Get_error_yaw()
{ 
	float yaw_error;  //两角度差值
	yaw_error=yaw-yaw_compensation;
	if(yaw_error>=270&&yaw_error<=360)
	{
		yaw_error=-1.0*(yaw_compensation-yaw+360); 
		return yaw_error;
	}
	else if(yaw_error<=-270&&yaw_error>=-360)
	{
		yaw_error=yaw-yaw_compensation+360;
		return yaw_error;
	}
	else
	{
		return yaw_error;
	}
}
/*获取AD最小值差值用以补偿两光电误差*/
int32_t PatrolLine::Get_ADmin_error()//ADC静态误差
{
  int32_t ADmin_error;
  ADmin_error=ad_min[compare_ad_left]-ad_min[compare_ad_right];
  if (ad_now[compare_ad_left]>=4090&&ad_now[compare_ad_right]>=4090)
  {
    ADmin_error=0;
  }
  
  return ADmin_error;
}
/*获取补偿后的误差*/
int32_t PatrolLine::Get_Compensation_error()
{
  int32_t compensation_error;
  compensation_error= Get_error()-Get_ADmin_error();
  return compensation_error;
}
/*补偿*/
int32_t PatrolLine::Calcu_Compensation()//补偿
{
  compensation = PatrolLinePID.Calculate(Get_Compensation_error());
  return compensation;
}
int32_t PatrolLine::Yaw_Compensation()//偏航角补偿
{
  compensation = PatrolLinePID.Calculate_yaw(Get_error_yaw());
  return compensation;
}
int32_t PatrolLine::Read_Compensation()
{
  return compensation;
}
int32_t PatrolLine::LCDshow_Compensation()
{
  int32_t LCDshowcompensation;
    if(compensation<0)
  {
    LCDshowcompensation=abs(compensation);
  }
    if(compensation>=0)
  {
    LCDshowcompensation=compensation;
  }
  return LCDshowcompensation;
}
uint16_t PatrolLine::Get_ad_now(uint8_t num)
{
  uint16_t ad_value=ad_now[num];
  return ad_value;
}
float PatrolLine::GetPID_Kp(void)
{
  return PatrolLinePID.Get_Kp();
}
float PatrolLine::GetPID_Kd(void)
{
  return PatrolLinePID.Get_Kd();
}
uint8_t PatrolLine::compare_ad()
{
  if(ad_now[compare_ad_left] >= ad_now[compare_ad_right])
	{
		return 0;
	}
  if(ad_now[compare_ad_left] < ad_now[compare_ad_right])
  {
		return 1;
	}
	return 0;
}

/*判断灰度口是否上白线*/
uint8_t PatrolLine::ad_if_white(uint8_t ad_value)
{
  if(ad_now[ad_value]>2000)  
  {
    HAL_Delay(2);
    if(ad_now[ad_value]>2000)
    {
      return 1;
    }
  }
  return 0;
}

/*ad补偿值*/
int32_t PatrolLine::ad_compensation()
{
  if(ad_now[0]>2000)
  {
    return 150;
  }
  else if(ad_now[9]>2000)
  {
    return -150;
  }
  else if(ad_now[1]>2000)
  {
    return 120;
  }
  else if(ad_now[8]>2000)
  {
    return -120;
  }
  else if(ad_now[2]>2000)
  {
    return 90;
  }
  else if(ad_now[7]>2000)
  {
    return -90;
  }
  else if(ad_now[3]>2000)
  {
    return 50;
  }
  else if(ad_now[6]>2000)
  {
    return -50;
  }
  else
  {
    return 0;
  }

}



/*曲线偏心ad补偿值*/
int32_t PatrolLine::ad_circle_compensation()
{
  if(ad_now[0]>2000)
  {
    return 100;
  }
  else if(ad_now[1]>2000)
  {
    return 50;
  }
  else if(ad_now[4]>2000)
  {
    return -50;
  }
  else if(ad_now[5]>2000)
  {
    return -100;
  }
  else if(ad_now[6]>2000)
  {
    return -100;
  }
  else if(ad_now[7]>2000)
  {
    return -100;
  }
  else if(ad_now[8]>2000)
  {
    return -100;
  }
  else if(ad_now[9]>2000)
  {
    return -100;
  }
  else
  {
    return -100;
  }
}
