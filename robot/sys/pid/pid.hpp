#ifndef _PID_H
#define _PID_H
enum PID_TYPE   //用于选择PID类型
{
  INCREMENTAL = 0,    //增量式PID
  POSITIONAL,     //位置式PID
};
class pid//PID基本类型
{
protected:
  PID_TYPE TYPE;//PID类型

  float Kp;	//比例系数
  float Kd; //微分系数

  float p_out;	//比例输出
  float d_out;	//微分输出
  float total_out;//总输出

  float lastError; //最后的误差值
  float prevError; //上次的误差值

  float MaxOutput;  //PID最大输出限制

  void limit(float *a1, float a2);//限幅(比大小函数)

public:
  pid(PID_TYPE type);
  ~pid();

  /* 以下是你能使用的方法 */
 //设置最大输出
 void Set_MaxOutput(float Set_MaxOutput);
 float Get_Kp(void);  //获取kp
 float Get_Kd(void); //获取kd
 void Set_Kp(float kp);//设置Kp
 void Set_Kd(float kd);//设置Kd
 void Set_Kp_Kd(float kp, float kd);//设置Kp、Kd
 PID_TYPE Get_PID_Type(void);//获取PID的类型
 float Get_total_out(void);//获得上一次的总输出

};

#endif
