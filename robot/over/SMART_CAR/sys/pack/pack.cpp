#include "pack.hpp"

PatrolLine       pid_value;   							               //定义所有产生pid误差的对象
Encoder          Robort_Encoder;                                       //定义编码器对象
Motor            Robort_motor;  						               //定义控制电机速度的对象
Distance_patrol  DistancePatrol; 						               //定义产生位置式pid误差的对象
PID_POSITIONAL   turn_direction; 						               //定义控制pid角度环速度的对象
LED_Class        LED1(LED1_GPIO_Port,LED1_Pin);                        //控制LED1
LED_Class        LED2(LED2_GPIO_Port,LED2_Pin);                        //控制LED2
Key              Key_All;								               //控制按钮

char str[63]={0};	                                                   //结合sprintf将数据输出到指定的字符数组中
Sflag flag = {0};													   //放置所有标志位状态位的结构体
Sflag_Device flag_Device = {0};                                        //放置所有设备状态位的结构体
Slocation loc = {0};												   //放置小车所在位置的结构体



void Pack_Init(void)                                                   
{	
	delay_init(216);                                                   //Delay初始化
	LCD_Init();                                                        //LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);                                   
	Robort_motor.Init();                                               //电机初始化
	Robort_motor.Enable();                                             //电机使能
	Robort_Encoder.Encoder_Init();
	pid_value.Init();                                                  //DMA传AD值初始化
	pid_value.SetcompareAD(3,4);  	                                   //设置ad电压比较的口
	Gyroscope_Init();                                                  //陀螺仪初始化
//	HAL_UART_Receive_DMA(&huart2,Camara_RxBuffer,1);                   //打开摄像头串口数据接收中断---------------改
}


void Robot_Start(void)
{
	osDelay(1000);
	while(roll<=0.001f&&roll>=-0.001f&&pitch<=0.001f&&pitch>=-0.001f&&yaw<=180.001f&&yaw>=179.999f)//若无值时陀螺仪初始化失败
	{
		sprintf(str,"Gyroscope Init Error!");
		LCD_ShowString(8,8,(u8 *)str,BLACK,WHITE,12,0);
		osDelay(100);
	}
	/*WT901C姿态传感器初始化成功后，屏幕显示提示信息*/
	sprintf(str,"Gyroscope init success");
	LCD_ShowString(8,8,(u8 *)str,GREEN,WHITE,12,0);
	sprintf(str,"USE START KEY START");
	LCD_ShowString(8,20,(u8 *)str,RED,WHITE,12,0);
	/*等待开始按钮打开*/
	while(HAL_GPIO_ReadPin(KEY_START_GPIO_Port, KEY_START_Pin)!=0)
	{	
		osDelay(1);
	}
	LED1.Open();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	/*等待挡板移开，小车启动*/
	while(!flag.start)
	{
		if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
		{
			HAL_Delay(200);
			if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
			{
				flag.start=1;
			}
		}
	}
	flag.Lcd=1;
}

/*实时检测各个硬件设备*/
void Check_Device(void)
{
	// Check if the infrared up sensor is online
	if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
	{
		flag_Device.Infrared_up=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Infrared_up=Offline;
		osDelay(1);
	}
	// Check if the infrared down left sensor is online
	if(HAL_GPIO_ReadPin(Infrared_down_left_GPIO_Port,Infrared_down_left_Pin)==Have)
	{
		flag_Device.Infrared_down_left=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Infrared_down_left=Offline;
		osDelay(1);
	}
	// Check if the infrared down right sensor is online
	if(HAL_GPIO_ReadPin(Infrared_down_right_GPIO_Port,Infrared_down_right_Pin)==Have)
	{
		flag_Device.Infrared_down_right=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Infrared_down_right=Offline;
		osDelay(1);
	}
	// Check if the color code left sensor is online
	if(HAL_GPIO_ReadPin(colorcode_left_GPIO_Port,colorcode_left_Pin)==Have)
	{
		flag_Device.Color_code_left=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Color_code_left=Offline;
		osDelay(1);
	}
	// Check if the color code right sensor is online
	if(HAL_GPIO_ReadPin(colorcode_right_GPIO_Port,colorcode_right_Pin)==Have)
	{
		flag_Device.Color_code_right=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Color_code_right=Offline;
		osDelay(1);
    }
	// Check if the color code front left sensor is online
	if(HAL_GPIO_ReadPin(colorcode_front_left_GPIO_Port,colorcode_front_left_Pin)==Have)
	{
		flag_Device.Color_code_front_left=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Color_code_front_left=Offline;
		osDelay(1);
	}
	// Check if the color code front right sensor is online
	if(HAL_GPIO_ReadPin(colorcode_front_right_GPIO_Port,colorcode_front_right_Pin)==Have)
	{
		flag_Device.Color_code_front_right=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Color_code_front_right=Offline;
		osDelay(1);
    }
	// Wait for 1ms
	osDelay(1);
}


/*过桥*/
void Up_Down_Bridge(void)
{
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10 ,50);
	/*上桥*/
	while(IF_UPHILL)
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),40);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),-40);//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
		osDelay(1);
	}
	/*桥面*/
	while(IF_FLAT)
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),40);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),-40);//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
		osDelay(1);
	}
	/*下桥*/
	while(IF_DOWNHILL)
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),40);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),-40);//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
		osDelay(1);
	}
//	pid_value.set_yaw_compensation();
//	pid_value.SetPID_Kp_Kd_yaw(10,0);
//	DistancePatrol.Move_Distance(4800,450,2500);
//	DistancePatrol.Set_Kp_Ki_Kd(0.02,0.13,0.08);
//	while(flag_Device.Infrared_down_left==Offline&&flag_Device.Infrared_down_right==Online)
//	{
//		if(flag_Device.Color_code_left==Online||flag_Device.Infrared_down_left==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(140,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),40);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else if(flag_Device.Color_code_right==Online||flag_Device.Infrared_down_right==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(140,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),-40);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else
//		{
//			Robort_motor.Patrol_Distance_Protect(140,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),0);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		HAL_Delay(1);
//	}

//	
//	while(flag_Device.Infrared_down_left==Offline&&flag_Device.Infrared_down_right==Offline)
//	{
//		if(flag_Device.Color_code_left==Online||flag_Device.Infrared_down_left==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),50);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else if(flag_Device.Color_code_right==Online||flag_Device.Infrared_down_right==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),-50);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		HAL_Delay(1);
//	}
//	while(flag_Device.Infrared_down_left==Offline&&flag_Device.Infrared_down_right==Online)
//	{
//		if(flag_Device.Color_code_left==Online||flag_Device.Infrared_down_left==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),50);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else if(flag_Device.Color_code_right==Online||flag_Device.Infrared_down_right==Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),-50);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		else
//		{
//			Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
//			HAL_Delay(1);
//		}
//		HAL_Delay(1);
//	}
//	while(flag_Device.Infrared_down_left==Online&&flag_Device.Infrared_down_right==Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),pid_value.ad_compensation());
//		HAL_Delay(1);
//	}
}



/*机器人从1号平台到2号平台*/
void Robot_loc_1to2(void)
{
	u16 i_temp=0;
	/****************************************1号平台起步****************************************/
	Robort_motor.Enable();
	while(IF_FLAT)
	{	
		Robort_motor.Speed(110);
		osDelay(1);
	}
	/****************************************1号平台下坡****************************************/
	pid_value.SetPID_Kp_Kd(0.01,0.01);
	for(u32 i=0;i<=500;i++)
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect2(120,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(2);
	}
	/****************************************1号平台底-桥底****************************************/
	pid_value.SetPID_Kp_Kd(0.008,0.005);
	pid_value.SetPID_Kp_Kd_yaw(3,50);
	DistancePatrol.Move_Distance(6500,500,3200);
	DistancePatrol.Set_Kp_Ki_Kd(0.007,0.14,0.1);
	pid_value.set_yaw_compensation();
	while(IF_FLAT)
	{
		Robort_motor.Patrol_Distance_Protect(180,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************过桥****************************************/
	Up_Down_Bridge();
	/****************************************桥底-A岔口****************************************/
	pid_value.SetPID_Kp_Kd(0.006,0.001);
	for(u32 i=0;i<=350;i++)
	{	
		Robort_motor.Patrol_Distance_Protect2(160,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************A岔口-2号平台底****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.008,0.003);
	pid_value.SetPID_Kp_Kd_yaw(3,50);
	DistancePatrol.Move_Distance(6000,500,3000);
	DistancePatrol.Set_Kp_Ki_Kd(0.008,0.13,0.08);
	while(IF_FLAT)
	{
		Robort_motor.Patrol_Distance_Protect(180,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************2号平台上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.02,0.01);
	while(IF_UPHILL)
	{
		Robort_motor.Patrol_Distance_Protect2(170,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
}






void __All_Data_Handler(void)
{
	Gyroscope_Start();
	Gyroscope_DataHandler();
}

void LCD_Show(void)
{
	LCD_ShowIntNum(0,0,ad_now[0],4,BLACK,WHITE,24);
	LCD_ShowIntNum(0,24,ad_now[1],4,BLACK,WHITE,24);
	LCD_ShowIntNum(0,48,ad_now[2],4,BLACK,WHITE,24);
	LCD_ShowFloatNum(0,72,yaw,5,BLACK,WHITE,24);

}


