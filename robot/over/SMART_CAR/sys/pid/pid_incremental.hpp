/*
 * @Description: 
 * @Version: 
 * @Autor: ZhangRiven
 * @Date: 2021-06-26 14:21:02
 * @LastEditors: ZhangRiven
 * @LastEditTime: 2021-06-26 20:22:26
 */
#ifndef __PID_INCREMENTAL_H
#define __PID_INCREMENTAL_H
#include "pid.hpp"
#include "main.h"

class PID_INCREMENTAL:public pid//PID����ʽ������
{
private:
  /* data */
//PID_TYPE TYPE;//��ʽ

float Kp_yaw;	  //����ϵ��//yaw方向上的比例系数（P系数）
float Kd_yaw; 	//΢��ϵ��//yaw方向上的微分系数（D系数）

float p_out_yaw;	//�������//yaw方向上的比例输出
float d_out_yaw;	//΢�����//yaw方向上的微分输出
float total_out_yaw;//�����//yaw方向上的总输出
   
float lastError_yaw; //�������ֵ//yaw方向上的上一次误差
float prevError_yaw; //�ϴε����//PID增量算法中的最大输出值。
    
float MaxOutput_yaw;	    //PID����������
public:
  PID_INCREMENTAL();
  ~PID_INCREMENTAL();

  /* ����������ʳ�õķ�����ע�͵��ĺ����ѱ��̳� */
void Set_MaxOutput_yaw(float Set_MaxOutput);
void Set_Kp_yaw(float kp_yaw);//����Kp
void Set_Kd_yaw(float kd_yaw);//����Kd
void Set_Kp_Kd_yaw(float Kp_yaw, float Kd_yaw);//����Kp��Kd
float Get_Kp_yaw(void);  //获取kp
float Get_Kd_yaw(void); //获取kd
float Get_total_out_yaw(void);
/*计算光敏二极管误差*/
float Calculate(float newError);
/*计算陀螺仪误差*/
float Calculate_yaw(float newError);


};
#endif
