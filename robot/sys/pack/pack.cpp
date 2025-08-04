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
                                                     

/********************函数寻址********************/
/*
	功能函数：
	机器人初始化                           （0x00）
	实时检测各个硬件设备                   （0x01）
	过桥                                   （0x02）
	更改方向函数(右转)                     （0x03）
	更改方向函数(左转)                     （0x04）
	平台编码值停止                         （0x05）
	过跷跷板							   （0x06）									   
	撞景点								   （0x07）

	运行函数：                             
    机器人启动							   （0x10）
	机器人从1号平台到2号平台               （0x11）
	机器人从2号平台到3号平台			   （0x12）
	机器人从3号平台到1号门				   （0x13）
	机器人从1号门到2号门				   （0x14）
	机器人从1号门到7号平台				   （0x15）
	机器人从2号门到7号平台				   （0x16）
	机器人从2号门过4号门到7号平台		   （0x17）
	机器人从7号平台到8号平台			   （0x18）
	机器人从8号平台到回家点1			   （0x19）
	机器人从回家点1到1号门				   （0x20）
	机器人从1号门到回家点2				   （0x21）
	机器人从1号门到3号门				   （0x22）
	机器人从3号门到回家点2				   （0x23）
	机器人从3号门过2号门到回家点2		   （0x24）
	机器人从回家点2到家					   （0x25）
	机器人启动（第二轮）				   （0x26）
	机器人从3号平台直达7号平台（第二轮）   （0x27）
	机器人从回家点1直达回家点2（第二轮）   （0x28）
	机器人去六号平台                       （0x29） 
*/




/*机器人初始化（0x00）*/
void Pack_Init(void)                                                   
{	 
	delay_init(216);                                                   //Delay初始化
	LCD_Init();                                                        //LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);                                   
	Robort_motor.Init();                                               //电机初始化
	Robort_motor.Enable();                                             //电机使能
	Robort_Encoder.Encoder_Init();
	pid_value.Init();                                                  //DMA传AD值初始化
	pid_value.SetcompareAD(4,5);  	                                   //设置ad电压比较的口
	Gyroscope_Init();                                                  //陀螺仪初始化
//	HAL_UART_Receive_IT(&huart1,GyrRxHeadBuffer,1);                    //开启陀螺仪串口接收中断
	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);                     //开启摄像头串口接收中断
}


/*实时检测各个硬件设备（0x01）*/
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
	// Check if the infrared Hit sensor is online
	if(HAL_GPIO_ReadPin(Infrared_Hit_GPIO_Port,Infrared_Hit_Pin)==Have)
	{
		flag_Device.Infrared_Hit=Online;
		osDelay(1);
	}
	else
	{
		flag_Device.Infrared_Hit=Offline;
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


/*过桥（0x02）*/
void Up_Down_Bridge(void)
{
	pid_value.SetPID_Kp_Kd_yaw(8,50);
	/*上桥*/
	while(IF_UPHILL)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	while(IF_FLAT)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	while(IF_DOWNHILL)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	/*桥面*/
	while(IF_FLAT)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	/*下桥*/
	while(IF_DOWNHILL)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	while(IF_FLAT)
	{
		if(flag_Device.Color_code_front_left==Online||flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_right==Online||flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60);
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		osDelay(1);
	}
	while(IF_UPHILL)
	{
		Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
}



/*更改方向函数(右转) 参数：需要更改的角度（绝对值）,位置（平台为1，地皮为0）（0x03）*/
void Change_dir_Pid_Right(float angle,uint8_t location)
{
	float  yaw_compensation;  							 //上平台时取姿态的值用于比较
	float  yaw_value;         							 //处理后的值
	float  yaw_abs;           							 //两角度差值
	uint16_t pid_times;                                  //pid巡线调整次数
	yaw_compensation=yaw;                                                                        
	if(angle>90&&location==1)                           //大角度平台转向参数
	{                                                    
		pid_times = 4000;  
		turn_direction.Init(angle,200,angle-25); 	
		turn_direction.Set_Kp_Ki_Kd(1,1,0.65);      
	}else if(angle<=90&&location==1)
	{                                                    //小角度平台转向参数
        pid_times = 4000;    
		turn_direction.Init(angle,200,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(1.5,1,0.65);
	}else if(angle>90&&location==0)
	{                                                    //大角度地皮转向参数
        pid_times = 5000;    	
		turn_direction.Init(angle,300,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(2,1,0.65);
	}else if(angle<=90&&location==0)
	{                                                    //小角度地皮转向参数
        pid_times = 5000;
		turn_direction.Init(angle,300,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(2,1,0.65);
	}
	osDelay(1);
	for(int i=0;i<pid_times;i++)
	{	
		yaw_value=yaw;
		if(yaw_value>yaw_compensation+10)                //对数据进行处理
		{
			osDelay(1);  yaw_value=yaw_value-360;
		}
		yaw_abs=yaw_compensation-yaw_value;
		Robort_motor.Speed(1.0f*(turn_direction.Calculate(yaw_abs)+100),-1.0f*(turn_direction.Calculate(yaw_abs)+115));  //所有平台掉头调速的入口
		if(yaw_abs>=angle-10)
		{
			Robort_motor.Speed(0);
			break;
		}
		osDelay(1);
	}
	osDelay(50);
}


/*更改方向函数(左转) 参数：需要更改的角度（绝对值）,位置（平台为1，地皮为0）（0x04）*/
void Change_dir_Pid_Left(float angle,uint8_t location)
{
	float  yaw_compensation;  							 //上平台时取姿态的值用于比较
	float  yaw_value;         							 //处理后的值
	float  yaw_abs;           							 //两角度差值
	uint16_t pid_times;                                  //pid巡线调整次数
	yaw_compensation=yaw;                                                                                
	if(angle>90&&location==1)                           //大角度平台转向参数
	{                                                    
		pid_times = 4000;  
		turn_direction.Init(angle,200,angle-25); 	
		turn_direction.Set_Kp_Ki_Kd(1,1,0.65);       
	}else if(angle<=90&&location==1)
	{                                                    //小角度平台转向参数
        pid_times = 4000;         	
		turn_direction.Init(angle,200,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(1.5,1,0.65);
	}else if(angle>90&&location==0)
	{                                                    //大角度地皮转向参数
        pid_times = 4000;         		
		turn_direction.Init(angle,200,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(2,1.5,0.65);
	}else if(angle<=90&&location==0)
	{                                                    //小角度地皮转向参数
        pid_times = 4000;         		
		turn_direction.Init(angle,300,angle-20); 
		turn_direction.Set_Kp_Ki_Kd(2,1.5,0.65);
	}
	
	for(int i=0;i<pid_times;i++)
	{	
		yaw_value=yaw;
		if(yaw_value<yaw_compensation-10)                //对数据进行处理
		{
			yaw_value=yaw_value+360;
		}
		yaw_abs=yaw_value-yaw_compensation;
		Robort_motor.Speed(-1.0f*(turn_direction.Calculate(yaw_abs)+100),1.0f*(turn_direction.Calculate(yaw_abs)+100));  //所有平台掉头调速的入口
		if(yaw_abs>=angle-10)
		{
			Robort_motor.Speed(0);
			break;
		}
		osDelay(1);
	}
	osDelay(50);
}



/*平台编码值停止（0x05）*/
void Color_stop_terrace(u32 speed)
{
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(1800))
	{
		Robort_motor.Patrol_Distance_Protect(speed,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Robort_motor.Enable();
	osDelay(50);
}



/*过跷跷板（0x06）*/
void Upper_seesaw(void)
{
	
	while(!(IF_FLAT)){
	if(flag_Device.Color_code_front_right==Online)
    {
		  Robort_motor.Patrol_Distance_Protect(220,0,0,80 );//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(220,0,0,-40);//进行位置pid巡线
			osDelay(1);
		}
		 if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(220,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(220,0,0,60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(220,0,0,0);//进行位置pid巡线
			osDelay(1);
		}
	
	}
	
	
	
	
	
	
	 DistancePatrol.Init();
	 
   while(!(DistancePatrol.Run_Right_Distance(3000)))
	 {
		 if(flag_Device.Color_code_front_right==Online)
    {
		  Robort_motor.Patrol_Distance_Protect(180,0,0,120 );//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-40);//进行位置pid巡线
			osDelay(1);
		}
		 if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,0);//进行位置pid巡线
			osDelay(1);
		}
		 
	 }
		 Robort_motor.Enable();
	      
}



/*撞景点（0x07）*/
void Robot_Hit_Spot(void)
{
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(flag_Device.Infrared_Hit!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
//	Robort_motor.Speed(0);
//	osDelay(1000);
	for(u32 i=0;i<=300;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(30);
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	for(u32 i=0;i<=200;i++)
	{	
		Robort_motor.Back_WhiteStraight(250,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
}




/*机器人启动（0x10）*/
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
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);	
	LED2.Open();
	flag.start=1;
	/*等待挡板移开，小车启动*/
	while(flag.start)
	{
		if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
		{
			osDelay(200);
			if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
			{
				flag.start=0;
			}
		}
	}
	while(!(flag.start))
	{
		if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==No)
		{
			osDelay(200);
			if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==No)
			{
				flag.start=1;
			}
		}
	}
	
	LED2.Close();
	flag.Lcd=1;
	flag.Direction = 0;//设定出发方向
}




/*机器人启动（第二轮）（0x26）*/
void Robot_Start_2(void)
{
	LED2.Open();
	/*等待挡板放下，小车复位*/
	while(flag.start)
	{
		if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
		{
			osDelay(200);
			if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==Have)
			{
				flag.start=0;
			}
		}
	}
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	sprintf(str,"Reset successfully");
	LCD_ShowString(8,8,(u8 *)str,GREEN,WHITE,12,0);
	flag.Lcd=1;
	flag.Direction = 0;//设定出发方向
	/*等待挡板移开，小车启动*/
	while(!flag.start)
	{
		if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==No)
		{
			osDelay(200);
			if(HAL_GPIO_ReadPin(Infrared_up_GPIO_Port, Infrared_up_Pin)==No)
			{
				flag.start=1;
			}
		}
	}
	LED2.Close();
}





/*机器人从1号平台到2号平台（0x11）*/
 void Robot_loc_1_to_loc_2(void)
{
	/****************************************1号平台起步****************************************/
	Robort_motor.Enable();
	while(IF_FLAT)
	{	
		Robort_motor.Speed(200);
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.07,0.01);
	for(u32 i=0;i<=700;i++)
	{
		pid_value.Value_Handling(); 
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.015,0.01);
	while(!(IF_UPHILL))
	{
		pid_value.Value_Handling(); 
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();		 
	pid_value.SetPID_Kp_Kd_yaw(8 ,46);
	while(!(IF_DOWNHILL))
	{
	if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
while(IF_DOWNHILL)
	{
	if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
while(!(IF_DOWNHILL))
	{
	if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(IF_DOWNHILL)
	{
	if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(!(IF_UPHILL))
	{
	if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(80,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(80,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(80,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	flag.ad_state=0;
    flag.track_state=0;
	while(flag_Device.Color_code_front_left!=Online)
	{	
		Robort_motor.Patrol_Distance_Protect2(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.1);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
    /****************************************巡线上坡****************************************/
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(2);
	}
	
	/****************************************巡线上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);	
	/****************************************闪灯展示掉头****************************************/
	osDelay(300);
	LED1.toggle();
	osDelay(300);
	LED1.toggle();
	osDelay(300);
	LED1.toggle();
	osDelay(300);
	LED1.toggle();
	Change_dir_Pid_Right(180,1);
}



/*机器人从2号平台到3号平台（0x12）*/
void Robot_loc_2_to_loc_3(void)
{
	/****************************************2号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Speed(200);
		osDelay(1);
	}
	/****************************************2号平台下坡****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.015);
	for(u32 i=0;i<400;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************岔口A转弯检测****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(3,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(40,0);
	/****************************************上山检测****************************************/
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************过山****************************************/
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	for(u32 i=0;i<2500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************岔口C转弯检测****************************************/
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.007,0.003);
	pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_front_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(125,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=1000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置式巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(16000,400,14000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.055,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.012);
    pid_value.SetPID_Kp_Kd_yaw(3,50);
	for(u32 i=0;i<=2000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************上坡止停****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}




/*机器人从2号平台到4号平台*/
void Robot_loc_2_to_loc_4(void)
{
	/****************************************2号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Speed(200);
		osDelay(1);
	}
	/****************************************2号平台下坡****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.015);
	for(u32 i=0;i<400;i++)
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************岔口A转弯检测****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(3,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Right(40,0);
	/****************************************上山检测****************************************/
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************过山****************************************/
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	for(u32 i=0;i<2000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************岔口C转弯检测****************************************/
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_front_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Left(40,0);	
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(20000,400,18000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.008,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(!(IF_UPHILL))//for(u32 i=0;i<=2000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************上坡止停****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}



/*机器人从4号平台到3号平台*/
void Robot_loc_4_to_loc_3(void)
{
	/****************************************4号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(200);
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.015);
	for(u32 i=0;i<=1000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(40000,400,38000);
	DistancePatrol.Set_Kp_Ki_Kd(0.002,0.05,0.08);
	DistancePatrol.Set_MaxOutput(400);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************上坡止停****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}



/*机器人从3号平台到1号门（0x13）*/
void Robot_loc_3to_door1(void)
{
	/****************************************3号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(200);
		osDelay(1);
	}
	/****************************************3号平台下坡方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置循迹****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(12000,400,10000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.055,0.08);
	DistancePatrol.Set_MaxOutput(400);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Right(90,0);//小角度右转
	/****************************************识别1号门****************************************/
	loc.door = 1;//所在位置1号门
	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	while(!(IF_UPHILL))
	{
		if(flag.Go_doorClose[1] == 1)//1号门禁行
		{
			Robort_motor.Speed(0);
			osDelay(100);
			break;
		}else{
			Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		}
		osDelay(1);
	}
	loc.door = 0;//门位挂空防止摄像头非法更改门禁信息
}



/*机器人从1号门到5号平台*/
void Robot_door1_to_loc_5(void)
{
	/****************************************巡线过1号门****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3500))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	while(flag_Device.Color_code_front_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(400,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Left(100,0);
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(25000,400,23000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.05,0.08);
	DistancePatrol.Set_MaxOutput(400);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}



/*机器人从1号门到2号门（0x14）*/
void Robot_door1_to_door2(void)
{
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_right!=Online)
	{	
		Robort_motor.Back_WhiteStraight(250,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Left(50,0);
	/****************************************识别2号门****************************************/
	loc.door = 2;
	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	while(!(IF_UPHILL))
	{
		if(flag.Go_doorClose[2] == 1)//2号门禁行
		{
			Robort_motor.Speed(0);
			osDelay(50);
			break;
		}else{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		}
		osDelay(1);
	}
	loc.door = 0;
}



/*机器人从2号门到5号平台*/
void Robot_door2_to_loc_5(void)
{
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3500))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(10000,400,8000);
	DistancePatrol.Set_Kp_Ki_Kd(0.007,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right==Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online&&flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Left(40,0);
	pid_value.SetPID_Kp_Kd(0.007,0.01);
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(10000,400,9000);
	DistancePatrol.Set_Kp_Ki_Kd(0.007,0.05,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}



/*机器人从2号门过4号门到5号平台*/
void Robot_door2_to_door4_to_loc_5(void)
{
	/****************************************倒车转向****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_right!=Online)
	{	
		Robort_motor.Back_WhiteStraight(250,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	osDelay(50);
	Robort_motor.Speed(0);
	Change_dir_Pid_Left(40,0);
	/****************************************巡线矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(9000,400,8500);
	DistancePatrol.Set_Kp_Ki_Kd(0.009,0.07,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	for(u32 i=0;i<=1500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************岔口D转弯检测****************************************/
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right==Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
		osDelay(50);
	Change_dir_Pid_Right(90,0);
	/****************************************巡线过门****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3500))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();	
	/****************************************岔口J转弯检测****************************************/
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Change_dir_Pid_Left(90,0);
	/****************************************巡线矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(9000,400,8500);
	DistancePatrol.Set_Kp_Ki_Kd(0.008,0.07,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************上坡止停****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
}


/*机器人从悬崖到7号平台（内部函数15、16、17）*/
void Robot_Precipice_to_loc_7(void)
{
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.08,0.015);
    pid_value.SetPID_Kp_Kd_yaw(6,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(90,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=600;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(15000,400,14000);
	DistancePatrol.Set_Kp_Ki_Kd(0.006,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************巡线上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);	
	Change_dir_Pid_Right(180,1);
}




/*机器人从1号门到7号平台（0x15）*/
void Robot_door1_to_loc_7(void)
{
	/****************************************巡线过1号门****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	/****************************************岔口H转弯检测****************************************/
	while(flag_Device.Color_code_front_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(100,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=600;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************岔口I转弯检测****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(6000,400,4500);
	DistancePatrol.Set_Kp_Ki_Kd(0.01,0.08,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(90,0);
	/****************************************悬崖-7号平台****************************************/
	Robot_Precipice_to_loc_7();
}





/*机器人从5号平台到7号平台*/
void Robot_loc_5to_loc7(void)
{
	/****************************************5号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(100);
		osDelay(1);
	}
	/****************************************5号平台下坡方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=1000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(18000,400,17000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.05,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	for(u32 i=0;i<=2300;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Left(90,0);
	/****************************************悬崖-7号平台****************************************/
	Robot_Precipice_to_loc_7();
}












/*机器人从2号门到7号平台（0x16）*/
void Robot_door2_to_loc_7(void)
{
	/****************************************巡线过2号门****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(10000,400,9000);
	DistancePatrol.Set_Kp_Ki_Kd(0.007,0.05,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right==Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************岔口J转弯检测****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online&&flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	osDelay(100);
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(130,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=400;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(6000,400,4500);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(90,0);
	/****************************************悬崖-7号平台****************************************/
	Robot_Precipice_to_loc_7();
}




/*机器人从2号门过4号门到7号平台（0x17）*/
void Robot_door2_to_door4_to_loc_7(void)
{
	/****************************************倒车转向****************************************/
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_right!=Online)
	{	
		Robort_motor.Back_WhiteStraight(250,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	osDelay(50);
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(40,0);
	/****************************************巡线矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=600;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(9000,400,8000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.07,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	for(u32 i=0;i<=1500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************岔口D转弯检测****************************************/
	while(flag_Device.Color_code_front_right!=Online)
	{		
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right==Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Change_dir_Pid_Right(90,0);
	osDelay(100);
	/****************************************过4号门****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();

	/****************************************岔口J转弯检测****************************************/
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(100,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=400;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线岔口I转弯检测****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(6000,400,4500);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(90,0);
	/****************************************悬崖-7号平台****************************************/
	Robot_Precipice_to_loc_7();
}




/*机器人从7号平台到回家点1（未启用）*/
void Robot_loc_7_to_HomePoint1(void)
{
	/****************************************7号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(100);
		osDelay(1);
	}
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************7号平台下坡方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.007,0.01);
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(12000,400,11000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	for(u32 i=0;i<=1800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.007,0.01);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(90,0);
	pid_value.SetPID_Kp_Kd(0.007,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	for(u32 i=0;i<=2000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(flag_Device.Color_code_left!=Online&&flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(90,0);
}



/*机器人从7号平台到8号平台（0x18）*/
void Robot_loc_7_to_loc_8(void)
{
	/****************************************7号平台起步****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(100);
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	/****************************************7号平台下坡****************************************/
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************7号平台下坡方向矫正****************************************/
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(1500))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	/****************************************位置巡线****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(28000,400,26000);
	DistancePatrol.Set_Kp_Ki_Kd(0.004,0.05,0.08);
	DistancePatrol.Set_MaxOutput(400);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(6,50);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************巡线上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(2200))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Robort_motor.Enable();
	osDelay(100);	
	Change_dir_Pid_Left(180,1);
}



/*机器人从8号平台到回家点1（0x19）*/
void Robot_loc_8_to_HomePoint1(void)
{
	/****************************************8号平台起步下坡****************************************/
	while(!(IF_DOWNHILL))
	{		
		Robort_motor.Speed(100);
		osDelay(1);
	}
	/****************************************8号平台下坡****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************8号平台下坡方向矫正****************************************/
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线岔口L转弯检测****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(11000,400,9000);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(400);
	pid_value.SetPID_Kp_Kd(0.01,0.015);
    pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	osDelay(50);
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(130,0);
	/****************************************过跷跷板****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Speed(-40);
		osDelay(2);
	}
	pid_value.set_yaw_compensation();
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);
		osDelay(1);
	}
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(!pid_value.ad_if_white(1)&&!pid_value.ad_if_white(8))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(500))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Robort_motor.Enable();
	Change_dir_Pid_Left(45,0);
	Robot_Hit_Spot();//撞景点
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_right!=Online)
	{	
		Robort_motor.Back_WhiteStraight(250,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
	Change_dir_Pid_Right(90,0);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	flag.Direction = 1;//设定回家方向
}




/*机器人从回家点1到1号门（0x20）*/
void Robot_HomePoint1_to_door1(void)
{
	Change_dir_Pid_Left(90,0);
	Robort_motor.Speed(0);
	osDelay(100);
	/****************************************识别1号门****************************************/
	loc.door = 1;//所在位置1号门
	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	while(!(IF_UPHILL))
	{
		if(flag.Back_doorClose[1] == 1)//1号门禁行
		{
			Robort_motor.Speed(0);
			osDelay(100);
			break;
		}else{
			Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		}
		osDelay(1);
	}
	loc.door = 0;//门位挂空防止摄像头非法更改门禁信息
}




/*机器人从1号门到回家点2（0x21）*/
void Robot_door1_to_HomePoint2(void)
{
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(90,0);
		/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=400;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线岔口I转弯检测****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(6000,400,4500);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	osDelay(50);
	Robort_motor.Speed(0);
    osDelay(100);
	Change_dir_Pid_Left(120,0);//大角度右转
}




/*机器人从1号门到3号门（0x22）*/
void Robot_door1_to_door3(void)
{
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_left!=Online)
	{	
		Robort_motor.Back_WhiteStraight(200,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(50,0);
	pid_value.SetPID_Kp_Kd(0.007,0.01);
	for(u32 i=0;i<=1000;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(7000,400,6000);
	DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	/****************************************识别3号门****************************************/
	loc.door = 3;
	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
	pid_value.SetPID_Kp_Kd(0.007,0.003);
	while(!(IF_UPHILL))
	{
		if(flag.Back_doorClose[3] == 1)//1号门禁行
		{
			Robort_motor.Speed(0);
			osDelay(100);
			break;
		}
		else{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		}
		osDelay(1);
	}
	loc.door = 0;
}



/*机器人从3号门到回家点2（0x23）*/
void Robot_door3_to_HomePoint2(void)
{
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_front_right==Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(130,0);
	pid_value.SetPID_Kp_Kd(0.005,0.015);
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************再次位置循迹****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(7000,400,6000);
	DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.007,0.015);
    pid_value.SetPID_Kp_Kd_yaw(6,50);
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(360,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
    osDelay(100);
	Change_dir_Pid_Right(40,0);//大角度右转
}



/*机器人从3号门过2号门到回家点2（0x24）*/
void Robot_door3_to_door2_to_HomePoint2(void)
{
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_left!=Online)
	{	
		Robort_motor.Back_WhiteStraight(200,-pid_value.Yaw_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Change_dir_Pid_Left(90,0);
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.003);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4000))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(350,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(10,20);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),0);
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Right(140,0);
	/****************************************巡线方向矫正****************************************/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/****************************************位置巡线岔口I转弯检测****************************************/
	pid_value.set_yaw_compensation();
	DistancePatrol.Move_Distance(6000,400,4500);
	DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
	DistancePatrol.Set_MaxOutput(350);
	pid_value.SetPID_Kp_Kd(0.02,0.015);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	osDelay(50);
	Robort_motor.Speed(0);
    osDelay(100);
	Change_dir_Pid_Left(120,0);//大角度右转
}




/*机器人从回家点2到家（0x25）*/
void Robot_HomePoint2_to_Home(void)
{
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.01,0.015);
	for(u32 i=0;i<2500;i++)
	{
		Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.009,0.003);
	pid_value.SetPID_Kp_Kd_yaw(7,50);
	while(flag_Device.Color_code_front_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}
	Robort_motor.Speed(0);
    osDelay(100);
	Change_dir_Pid_Right(140,0);//大角度右转
	
	pid_value.SetPID_Kp_Kd(0.07,0.01);
	for(u32 i=0;i<=700;i++)
	{	
		Robort_motor.Patrol_Distance_Protect2(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.015,0.01);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect2(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	pid_value.set_yaw_compensation();		 
	pid_value.SetPID_Kp_Kd_yaw(8 ,46);
	while(!(IF_DOWNHILL))
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(IF_DOWNHILL)
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(!(IF_DOWNHILL))
	{
		
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(IF_DOWNHILL)
	{
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(!(IF_UPHILL))
	{   	
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(150,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
			osDelay(1);
		}
	}
	while(IF_UPHILL)
	{
		Robort_motor.Patrol_Distance_Protect2(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);   
	}
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect2(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);
	}
	while(!(IF_UPHILL))
	{
        Robort_motor.Patrol_Distance_Protect2(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
		osDelay(1);	 
	}			 
	/****************************************巡线上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);	
	Change_dir_Pid_Right(180,1);
	Robort_motor.Speed(0);
	osDelay(100);
}



/*机器人去六号平台（0x29）*/
void Robot_loc_6(void)
{
	Change_dir_Pid_Left(100,0);
	Robort_motor.Speed(0);
	osDelay(100);
  /*********从西湖景点旁边的十字路口开始（转弯后）**********/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=500;i++)
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect(280,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.012);
    pid_value.SetPID_Kp_Kd_yaw(6,46);   
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.003,0.006);
	pid_value.SetPID_Kp_Kd_yaw(2,12);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
    Change_dir_Pid_Right(130,0);
	Robort_motor.Speed(0);
	pid_value.SetcompareAD(4,5);  	 
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_circle_compensation());
		osDelay(1);
    }
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd_yaw(13,51);
	DistancePatrol.Init(); 
	while(!(DistancePatrol.Run_Right_Distance(400)))
    {
	    Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),0);
	    osDelay(1);
	}
	Robort_motor.Enable();
	while(flag_Device.Color_code_front_left!=Online)
		{
	    	Robort_motor.turn_left(200,0);
			osDelay(1);
		}
	Upper_seesaw();//过跷跷板
	for(u32 i=0;i<=500;i++)
	{
		Robort_motor.Speed(-40);
		osDelay(2);
	}
//	osDelay(200);
	pid_value.SetcompareAD(4,5);  
	pid_value.SetPID_Kp_Kd(0.007,0.015);
	while(!(pid_value.ad_if_white(3)||pid_value.ad_if_white(4)))
	{
		Robort_motor.turn_left(300,0.2);
	    osDelay(1);
	}
	
//	if(ad_now[1]>2000||ad_now[2]>2000||ad_now[3]>2000||ad_now[4]>2000||ad_now[5]>2000||ad_now[6]>2000||ad_now[7]>2000||ad_now[8]>2000)
//   {
//         
//		pid_value.Value_Handling(); 		
//		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//   }
//   else 
//   {
//      Change_dir_Pid_Left(30,0);//小角度左转
//   
//   }
//	while(ad_now[1]<2000&&ad_now[2]<2000&&ad_now[3]<2000&&ad_now[3]<2000&&ad_now[7]<2000&&ad_now[8]<2000)
//    {
//		Robort_motor.turn_left(200,-1);
//	    osDelay(1);
//	}
    /*************跷跷板下来到六号平台**********/   
    pid_value.SetPID_Kp_Kd(0.007,0.015);
	while(!(IF_FLAT))
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_front_left!=Online)
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_front_left==Online)
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(250,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(100);
	Change_dir_Pid_Left(90,0);//小角度左转
	/****************************************巡线上坡****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);	
	Change_dir_Pid_Right(180,1);
	/************下面这里是平台转弯后要去下一个跷跷板了**************/
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Speed(100);
		osDelay(1);
	}
	pid_value.SetcompareAD(4,5);
	pid_value.SetPID_Kp_Kd(0.005,0.015);	
	for(u32 i=0;i<=1000;i++)
	{		
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();	
	pid_value.SetPID_Kp_Kd(0.005,0.015);
	pid_value.SetPID_Kp_Kd_yaw(13,51);
	while(flag_Device.Color_code_left!=Online&&flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
		osDelay(1);
	}	 
	Robort_motor.Speed(0);
	Change_dir_Pid_Left(90,0);//小角度左转
	pid_value.SetcompareAD(4,5);
	while(!(IF_UPHILL))
	{
		Robort_motor.Patrol_Distance_Protect(220,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	/*********同样的用上山状态来判断是否到达跷跷板底部（六号平台记得转弯后再用过跷跷板函数）************/
	DistancePatrol.Init(); 
	pid_value.set_yaw_compensation();	
	while(!(DistancePatrol.Run_Right_Distance(400)))
    {
	    Robort_motor.Patrol_Distance_Protect(100,0,pid_value.Yaw_Compensation(),0);
	    osDelay(1);
	}
	 Robort_motor.Enable();
	while(flag_Device.Color_code_front_left!=Online)
		{
	    	Robort_motor.turn_left(200,0);
			osDelay(1);
		}

	while(!(IF_FLAT))
	{
		if(flag_Device.Color_code_front_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,100 );//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,0);//进行位置pid巡线
			osDelay(1);
		}
	}
	DistancePatrol.Init(); 
	while(!(DistancePatrol.Run_Right_Distance(3000)))
	{
		if(flag_Device.Color_code_front_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,60 );//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_front_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		 if(flag_Device.Color_code_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,-60);//进行位置pid巡线
			osDelay(1);
		}
		else if(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,60 );//进行位置pid巡线
			osDelay(1);
		}
		else
		{
			Robort_motor.Patrol_Distance_Protect(180,0,0,0);//进行位置pid巡线
			osDelay(1);
		}
	 }
		Robort_motor.Enable();
	    Robort_motor.Speed(-40);
		osDelay(1000);
	        DistancePatrol.Init();
	    pid_value.set_yaw_compensation();		 
	    pid_value.SetPID_Kp_Kd_yaw(13,50);
	      
        while(!(DistancePatrol.Run_Right_Distance(300)))
        {
		   Robort_motor.Patrol_Distance_Protect(140,0,pid_value.Yaw_Compensation(),0);
		}
		 Robort_motor.Enable();//过跷跷板
     while(!(pid_value.ad_if_white(1)||pid_value.ad_if_white(2)||pid_value.ad_if_white(3)||pid_value.ad_if_white(4)||pid_value.ad_if_white(5)||pid_value.ad_if_white(6)||pid_value.ad_if_white(7)))
	  {
			Robort_motor.turn_left(300,0.2);
			osDelay(1);
			
	   }
		 
//		while(ad_now[1]<2000&&ad_now[2]<2000&&ad_now[3]<2000&&ad_now[3]<2000&&ad_now[7]<2000&&ad_now[8]<2000)
//    {
//		Robort_motor.turn_left(200,-1);
//	    osDelay(1);
//	}
	 

		while(flag_Device.Color_code_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
			osDelay(1);
		}
		Change_dir_Pid_Left(30,0);
		pid_value.SetPID_Kp_Kd_yaw(10,50);
		pid_value.set_yaw_compensation();
		while(!(pid_value.ad_if_white(0)||pid_value.ad_if_white(1)))
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
	
		while(flag_Device.Color_code_front_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
		while(flag_Device.Color_code_front_left==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);
			osDelay(1);
		}
//		while(flag_Device.Color_code_right!=Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
//			osDelay(1);
//		}


//		pid_value.SetPID_Kp_Kd_yaw(13,51);
//		pid_value.set_yaw_compensation();
//		while(flag_Device.Color_code_left!=Online)
//		{
//			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),0);//进行位置pid巡线
//			osDelay(1);
//		}
//		while(1)
//		{
//		Robort_motor.Speed(0);
//			osDelay(1);
//		}
//		Robort_motor.Speed(0);
//		osDelay(100);
//		Change_dir_Pid_Left(30,0);
//		while(!(pid_value.ad_if_white(2)||pid_value.ad_if_white(3)||pid_value.ad_if_white(4)))
//		{
//			Robort_motor.turn_left(200,0);
//			osDelay(1);
//		}
//		for(u32 i=0;i<=700;i++)
//		{
//			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
//			osDelay(2);
//		}
//		while(ad_now[2]<2000&&ad_now[3]<2000&&ad_now[4]<2000)
//        {
//				Robort_motor.turn_left(200,-1);
//		}
		for(u32 i=0;i<=700;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());//进行位置pid巡线
			osDelay(2);
		}
		pid_value.set_yaw_compensation();
		pid_value.SetPID_Kp_Kd(0.005,0.015);
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(flag_Device.Color_code_right!=Online&&flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
}



void Robot_HomePoint1_to_loc_6(void)
{
	Change_dir_Pid_Left(100,0);
	Robort_motor.Speed(0);
	osDelay(100);
  /*********从西湖景点旁边的十字路口开始（转弯后）**********/
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=500;i++)
	{
		pid_value.Value_Handling(); 		
		Robort_motor.Patrol_Distance_Protect(280,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.012);
    pid_value.SetPID_Kp_Kd_yaw(6,46);   
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.003,0.006);
	pid_value.SetPID_Kp_Kd_yaw(2,12);
	while(flag_Device.Color_code_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	osDelay(50);
    Change_dir_Pid_Right(130,0);
	Robort_motor.Speed(0);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3800))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Robort_motor.Enable();
	Change_dir_Pid_Left(30,0);
	Robort_motor.Speed(0);
	osDelay(50);
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3800))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	Robort_motor.Speed(0);
	osDelay(1000);
	while(!(pid_value.ad_if_white(3)||pid_value.ad_if_white(4)))
	{
		Robort_motor.turn_left(350,0.2);
		osDelay(1);
	}
	pid_value.SetPID_Kp_Kd(0.005,0.01);
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Change_dir_Pid_Left(90,0);
	Robort_motor.Speed(0);
	osDelay(50);
		/****************************************上坡止停****************************************/
	pid_value.SetPID_Kp_Kd(0.05,0.01);
	while(!(IF_DOWNHILL))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(2);
	}
	Color_stop_terrace(200);
	Change_dir_Pid_Right(180,1);
	
	while(!(IF_DOWNHILL))
	{	
		Robort_motor.Speed(100);
		osDelay(1);
	}
	for(u32 i=0;i<=800;i++)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);

	}
	while(flag_Device.Color_code_left!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Change_dir_Pid_Left(100,0);
	Robort_motor.Speed(0);
	osDelay(50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(4900))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Robort_motor.Enable();
	Change_dir_Pid_Left(35,0);
	Robort_motor.Speed(0);
	osDelay(50);
	pid_value.set_yaw_compensation();
	pid_value.SetPID_Kp_Kd(0.005,0.01);
    pid_value.SetPID_Kp_Kd_yaw(5,50);
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3700))
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
		osDelay(1);
	}
	Robort_motor.Enable();
	Robort_motor.Speed(0);
	osDelay(1000);
	while(!(pid_value.ad_if_white(3)||pid_value.ad_if_white(4)))
	{
		Robort_motor.turn_left(350,0.2);
		osDelay(1);
	}
	while(flag_Device.Color_code_front_right!=Online)
	{
		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
		osDelay(1);
	}
	DistancePatrol.Init();
	while(!DistancePatrol.Run_Right_Distance(3500))
	{
		Robort_motor.turn_left(200,0.8);
		osDelay(1);
	}
	Robort_motor.Speed(0);
	Robort_motor.Enable();
	
}






/*机器人从3号平台直达7号平台（第二轮）（0x27）*/
void Robot_loc_3_to_loc_7(void)
{

	if(flag.Go_doorClose[1] == 1&&flag.Go_doorClose[2] == 1)//1、2号门都禁行时走4号门
	{
		/****************************************3号平台起步****************************************/
		while(!(IF_DOWNHILL))
		{		
			Robort_motor.Speed(200);
			osDelay(1);
		}
		 pid_value.SetPID_Kp_Kd(0.005,0.01);
		while(!(IF_UPHILL))
		{		
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		for(u32 i=0;i<=800;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(25000,400,23000);
		DistancePatrol.Set_Kp_Ki_Kd(0.005,0.04,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.01,0.015);
		pid_value.SetPID_Kp_Kd_yaw(6,50);
		for(u32 i=0;i<=2800;i++)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		} 
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		while(flag_Device.Color_code_front_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		pid_value.SetPID_Kp_Kd_yaw(6,50);
		while(flag_Device.Color_code_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
		while(flag_Device.Color_code_right==Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
				while(flag_Device.Color_code_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Right(90,0);
		
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		while(!(IF_UPHILL))
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(2);
		}
		/****************************************巡线过4号门****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.003);
		DistancePatrol.Init();
		while(!DistancePatrol.Run_Right_Distance(4000))
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		Robort_motor.Enable();
		while(flag_Device.Color_code_front_right!=Online)
		{	
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(2);
		}
		pid_value.set_yaw_compensation();
		pid_value.SetPID_Kp_Kd_yaw(6,50);
		while(flag_Device.Color_code_left!=Online)
		{	
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Right(90,0);
		/****************************************巡线方向矫正****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=400;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		/****************************************位置巡线岔口I转弯检测****************************************/
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(6000,400,4500);
		DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.02,0.015);
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Left(90,0);
	}else{
		/****************************************3号平台起步****************************************/
		while(!(IF_DOWNHILL))
		{		
			Robort_motor.Speed(200);
			osDelay(1);
		}
		/****************************************3号平台下坡方向矫正****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=800;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		/****************************************位置循迹****************************************/
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(10000,400,9000);
		DistancePatrol.Set_Kp_Ki_Kd(0.005,0.055,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.02,0.015);
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(flag_Device.Color_code_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
		if(flag.Go_doorClose[1] == 0)//1号门通行
		{
			Change_dir_Pid_Right(90,0);//小角度右转
			pid_value.SetPID_Kp_Kd(0.005,0.01);
			while(!(IF_UPHILL))
			{		
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(1);
			}
			/****************************************巡线过1号门****************************************/
			pid_value.SetPID_Kp_Kd(0.005,0.003);
			DistancePatrol.Init();
			while(!DistancePatrol.Run_Right_Distance(4000))
			{
				Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(1);
			}
			Robort_motor.Enable();
			while(flag_Device.Color_code_front_left!=Online)
			{	
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(2);
			}
			pid_value.set_yaw_compensation();
			pid_value.SetPID_Kp_Kd_yaw(6,50);
			while(flag_Device.Color_code_right!=Online)
			{	
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
				osDelay(1);
			}
			Robort_motor.Speed(0);
			osDelay(100);
			Change_dir_Pid_Left(90,0);
			/****************************************巡线方向矫正****************************************/
			pid_value.SetPID_Kp_Kd(0.005,0.01);
			for(u32 i=0;i<=400;i++)
			{
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(1);
			}
			/****************************************位置巡线岔口I转弯检测****************************************/
			pid_value.set_yaw_compensation();
			DistancePatrol.Move_Distance(6000,400,4500);
			DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
			DistancePatrol.Set_MaxOutput(350);
			pid_value.SetPID_Kp_Kd(0.02,0.015);
			pid_value.SetPID_Kp_Kd_yaw(5,50);
			while(flag_Device.Color_code_right!=Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			Robort_motor.Speed(0);
			osDelay(100);
			Change_dir_Pid_Right(90,0);
		}else{//1号门禁行，走2号门
			Change_dir_Pid_Right(40,0);//小角度右转
			pid_value.SetPID_Kp_Kd(0.005,0.01);
			while(!(IF_UPHILL))
			{		
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(2);
			}
			/****************************************巡线过2号门****************************************/
			pid_value.SetPID_Kp_Kd(0.005,0.003);
			DistancePatrol.Init();
			while(!DistancePatrol.Run_Right_Distance(4500))
			{
				Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(1);
			}
			Robort_motor.Enable();
			/****************************************位置循迹****************************************/
			pid_value.set_yaw_compensation();
			DistancePatrol.Move_Distance(10000,400,8000);
			DistancePatrol.Set_Kp_Ki_Kd(0.005,0.055,0.08);
			DistancePatrol.Set_MaxOutput(350);
			pid_value.SetPID_Kp_Kd(0.01,0.015);
			pid_value.SetPID_Kp_Kd_yaw(5,50);
			for(u32 i=0;i<=1400;i++)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			while(flag_Device.Color_code_front_left!=Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			while(flag_Device.Color_code_front_left==Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			while(flag_Device.Color_code_front_left!=Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			while(flag_Device.Color_code_front_left==Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			Robort_motor.Speed(0);
			osDelay(100);
			Change_dir_Pid_Right(140,0);
			/****************************************巡线方向矫正****************************************/
			pid_value.SetPID_Kp_Kd(0.005,0.01);
			for(u32 i=0;i<=400;i++)
			{
				Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
				osDelay(1);
			}
			/****************************************位置巡线岔口I转弯检测****************************************/
			pid_value.set_yaw_compensation();
			DistancePatrol.Move_Distance(6000,400,4500);
			DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
			DistancePatrol.Set_MaxOutput(350);
			pid_value.SetPID_Kp_Kd(0.02,0.015);
			pid_value.SetPID_Kp_Kd_yaw(5,50);
			while(flag_Device.Color_code_left!=Online)
			{
				Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
				osDelay(1);
			}
			Robort_motor.Speed(0);
			osDelay(100);
			Change_dir_Pid_Left(90,0);
		}
	}
	/****************************************悬崖-7号平台****************************************/
	Robot_Precipice_to_loc_7();
}





/*机器人从回家点1直达回家点2（第二轮）（0x28）*/
void Robot_HomePoint1_to_HomePoint2(void)
{
	if(flag.Go_doorClose[1] == 0)//1号门通行
	{
		pid_value.SetPID_Kp_Kd(0.005,0.1);
		while(!(IF_UPHILL))
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(2);
		}
		/****************************************巡线过1号门****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.003);
		DistancePatrol.Init();
		while(!DistancePatrol.Run_Right_Distance(4000))
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		Robort_motor.Enable();
		while(flag_Device.Color_code_front_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(50);
		Change_dir_Pid_Right(100,0);
		/****************************************巡线方向矫正****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=600;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		/****************************************位置巡线岔口I转弯检测****************************************/
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(6000,400,4500);
		DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.01,0.015);
		pid_value.SetPID_Kp_Kd_yaw(7,50);
		while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		osDelay(50);
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Left(130,0);//大角度右转
	}else if(flag.Go_doorClose[2] == 0){
		Change_dir_Pid_Right(60,0);
	    Robort_motor.Speed(0);
	    osDelay(50);
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=500;i++)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(7000,400,6000);
		DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.01,0.015);
		pid_value.SetPID_Kp_Kd_yaw(7,50);
		while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(50);
		Change_dir_Pid_Left(100,0);
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		while(!(IF_UPHILL))
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		DistancePatrol.Init();
		while(!DistancePatrol.Run_Right_Distance(4000))
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		Robort_motor.Enable();
		while(flag_Device.Color_code_front_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(400,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		pid_value.SetPID_Kp_Kd(0.01,0.015);
		pid_value.SetPID_Kp_Kd_yaw(7,50);
		while(flag_Device.Color_code_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(50);
		Change_dir_Pid_Right(130,0);
		/****************************************巡线方向矫正****************************************/
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=600;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		/****************************************位置巡线岔口I转弯检测****************************************/
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(6000,400,4500);
		DistancePatrol.Set_Kp_Ki_Kd(0.005,0.06,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.01,0.015);
		pid_value.SetPID_Kp_Kd_yaw(7,50);
		while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		osDelay(50);
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Left(130,0);//大角度右转
	}else if(flag.Go_doorClose[3] == 0){//1号门禁行，3号门通行
		Change_dir_Pid_Right(50,0);
	    Robort_motor.Speed(0);
	    osDelay(100);
		pid_value.SetPID_Kp_Kd(0.007,0.01);
		for(u32 i=0;i<=500;i++)
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(7000,400,6000);
		DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.02,0.015);
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(!(IF_UPHILL))
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		pid_value.SetPID_Kp_Kd(0.005,0.003);
		DistancePatrol.Init();
		while(!DistancePatrol.Run_Right_Distance(4000))
		{
			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		Robort_motor.Enable();
        while(flag_Device.Color_code_left!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		osDelay(100);
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Left(140,0);//大角度左转
		pid_value.SetPID_Kp_Kd(0.005,0.01);
		for(u32 i=0;i<=500;i++)
		{
			Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
			osDelay(1);
		}
		/****************************************再次位置循迹****************************************/
		pid_value.set_yaw_compensation();
		DistancePatrol.Move_Distance(7000,400,6000);
		DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
		DistancePatrol.Set_MaxOutput(350);
		pid_value.SetPID_Kp_Kd(0.005,0.015);
		pid_value.SetPID_Kp_Kd_yaw(5,50);
		while(flag_Device.Color_code_front_right!=Online)
		{
			Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
			osDelay(1);
		}
		Robort_motor.Speed(0);
		osDelay(100);
		Change_dir_Pid_Right(40,0);//小角度右转
	}
}



//void Robot_HomePoint1_to_door4(void)
//{
//	flag.Direction = 1;//将行驶方向改为回家方向
//	pid_value.SetPID_Kp_Kd(0.007,0.01);
//	while(flag_Device.Color_code_left!=Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	while(flag_Device.Color_code_left==Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	while(flag_Device.Color_code_left!=Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	Robort_motor.Speed(0);
//	osDelay(100);
//	Change_dir_Pid_Left(90,0);
//	/****************************************识别4号门****************************************/
//	loc.door = 4;
//	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
//	pid_value.SetPID_Kp_Kd(0.007,0.003);
//	while(!(IF_UPHILL))
//	{
//		if(flag.Back_doorClose[4] == 1)//1号门禁行
//		{
//			Robort_motor.Speed(0);
//			osDelay(100);
//			break;
//		}else{
//			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		}
//		osDelay(1);
//	}
//	loc.door = 0;
//}


//void Robot_door4_to_door2(void)
//{
//	pid_value.set_yaw_compensation();
//	pid_value.SetPID_Kp_Kd_yaw(10,20);
//	while(flag_Device.Color_code_right!=Online)
//	{	
//		Robort_motor.Back_WhiteStraight(150,-pid_value.Yaw_Compensation());
//		osDelay(1);
//	}
//	Robort_motor.Speed(0);
//	Change_dir_Pid_Left(60,0);
//	pid_value.SetPID_Kp_Kd(0.007,0.01);
//	for(u32 i=0;i<=1000;i++)
//	{
//		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	pid_value.set_yaw_compensation();
//	DistancePatrol.Move_Distance(7000,400,6000);
//	DistancePatrol.Set_Kp_Ki_Kd(0.006,0.08,0.08);
//	DistancePatrol.Set_MaxOutput(350);
//	pid_value.SetPID_Kp_Kd(0.02,0.015);
//    pid_value.SetPID_Kp_Kd_yaw(5,50);
//	while(flag_Device.Color_code_right!=Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(200,DistancePatrol.Get_CalculateSpeed(),pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
//		osDelay(1);
//	}
//	/****************************************识别2号门****************************************/
//	loc.door = 2;
//	HAL_UART_Receive_IT(&huart2,CameraRxBuffer,1);//开启摄像头串口接收中断
//	pid_value.SetPID_Kp_Kd(0.007,0.003);
//	while(!(IF_UPHILL))
//	{
//		if(flag.Back_doorClose[2] == 1)//1号门禁行
//		{
//			Robort_motor.Speed(0);
//			osDelay(100);
//			break;
//		}
//		else{
//			Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		}
//		osDelay(1);
//	}
//	loc.door = 0;
//}


//void Robot_door2_to_door3_to_HomePoint2(void)
//{
//	pid_value.set_yaw_compensation();
//	pid_value.SetPID_Kp_Kd_yaw(10,20);
//	while(flag_Device.Color_code_right!=Online)
//	{	
//		Robort_motor.Back_WhiteStraight(150,-pid_value.Yaw_Compensation());
//		osDelay(1);
//	}
//	Robort_motor.Speed(0);
//	Change_dir_Pid_Right(100,0);
//	pid_value.SetPID_Kp_Kd(0.007,0.01);
//	while(!(IF_UPHILL))
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	pid_value.SetPID_Kp_Kd(0.007,0.01);
//	for(u32 i=0;i<=600;i++)
//	{
//		Robort_motor.Patrol_Distance_Protect(200,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(2);
//	}
//	while(flag_Device.Color_code_right!=Online)
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	Robort_motor.Speed(0);
//	osDelay(100);
//	Change_dir_Pid_Left(130,0);
//	pid_value.SetPID_Kp_Kd(0.008,0.015);
//	for(u32 i=0;i<=500;i++)
//	{
//		Robort_motor.Patrol_Distance_Protect(300,0,pid_value.Calcu_Compensation(),pid_value.ad_compensation());
//		osDelay(1);
//	}
//	/****************************************再次位置循迹****************************************/
//	pid_value.set_yaw_compensation();
//	DistancePatrol.Set_MaxOutput(350);
//	pid_value.SetPID_Kp_Kd(0.007,0.015);
//    pid_value.SetPID_Kp_Kd_yaw(6,50);
//	while(flag_Device.Color_code_front_right!=Online)
//	{
//			Robort_motor.Patrol_Distance_Protect(360,0,pid_value.Yaw_Compensation(),pid_value.Calcu_Compensation());//进行位置pid巡线
//		osDelay(1);
//	}
//	Robort_motor.Speed(0);
//    osDelay(100);
//	Change_dir_Pid_Right(40,0);//小角度右转
//}



void __Hardware_Handler(void)
{
	Gyroscope_Start();
	Check_Device();
}

void LCD_Show(void)
{
	LCD_ShowIntNum(0,0,DistancePatrol.Get_CaptureRightCount(),5,BLACK,WHITE,12);
	LCD_ShowIntNum(0,12,ad_now[5],5,BLACK,WHITE,12);
}




/*测试函数（仅调试）
 *@灰度循迹
 *@要用哪个();解注释
 */
void PatrolLineTest(void)
{
	Robot_Start();
	Robot_loc_1_to_loc_2();
	Robot_loc_2_to_loc_4();
	Robot_loc_4_to_loc_3();	
	Robot_loc_3to_door1();
	if(flag.Go_doorClose[1] == 1)
	{
		Robot_door1_to_door2();
		if(flag.Go_doorClose[2] == 1)
		{
			Robot_door2_to_door4_to_loc_5();
		}else{
			Robot_door2_to_loc_5();
		}
	}else{
		Robot_door1_to_loc_5();
	}
	Robot_loc_5to_loc7();
	Robot_loc_7_to_loc_8();
	Robot_loc_8_to_HomePoint1();
    Robot_HomePoint1_to_HomePoint2();
	Robot_HomePoint2_to_Home();
	//第二轮
	Robot_Start_2();
	Robot_loc_1_to_loc_2();
	Robot_loc_2_to_loc_4();
	Robot_loc_4_to_loc_3();	
	Robot_loc_3to_door1();
	if(flag.Go_doorClose[1] == 1)
	{
		Robot_door1_to_door2();
		if(flag.Go_doorClose[2] == 1)
		{
			Robot_door2_to_door4_to_loc_5();
		}else{
			Robot_door2_to_loc_5();
		}
	}else{
		Robot_door1_to_loc_5();
	}
	Robot_loc_5to_loc7();
	Robot_loc_7_to_loc_8();
	Robot_loc_8_to_HomePoint1();
    Robot_HomePoint1_to_HomePoint2();
	Robot_HomePoint2_to_Home();
	while(1)
	{
		Robort_motor.Speed(0);
		osDelay(100);
	}
}

