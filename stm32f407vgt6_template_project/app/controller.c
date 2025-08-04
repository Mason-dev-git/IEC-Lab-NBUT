#include "controller.h"

CurrentControlMode current_mode;
pid_incremental_t linecontrol_pid_left, linecontrol_pid_right;
pid_incremental_t speedcontrol_pid_left, speedcontrol_pid_right;
pid_position_t directcontrol_pid_left, directcontrol_pid_right;
pid_position_t steeringcontrol_pid;
pid_incremental_t steeringcontrol_pid_left, steeringcontrol_pid_right;

extern volatile unsigned char Digtal;
static float left_speed, right_speed;
#if USE_13_MOTOR
float weight[8] = {16,12,8,4,4,8,12,16};
#else
int8_t weight[8] = {80, 40, 20, 10, -10, -20, -40, -80};
#endif

// 基础直线速度
float left_base_speed, right_base_speed;
// 转向速度
float left_turn_speed, right_turn_speed;
// 合成速度
float left_speed, right_speed;
u32 left_distance = 0, right_distance = 0;

static float ref_angel;


void Controller_Init(void)
{
    current_mode = STOP;
    LineControlInit();
    DirectControlInit();

    HAL_TIM_Base_Start_IT(&htim5);   // Internal 50ms timer for control
}

void Controller_IRQHandler(void)
{
    // debug_print("Controller_IRQHandler current_mode:%d\r\n", current_mode);
    switch (current_mode) {
        case STOP: motor_stop(); break;
        case LINE_CONTROL: LineControl_IRQHandler(); break;
        case DIRECT_CONTROL: break;
        case DISTANCE_CONTROL: break;
        case SPEED_CONTROL: break;
        case STEERING_CONTROL: break;
        default: break;
    }
}



/**
 * @brief 巡线控制对外控制端口
 *
 * @param speed 巡线速度
 */
void LineControl(float speed)
{
    current_mode = LINE_CONTROL;
    linecontrol_pid_left.target = speed;
    linecontrol_pid_right.target = speed;
}

/**
 * @brief 巡线控制初始化
 *
 */
void LineControlInit(void)
{
#if USE_13_MOTOR
    pid_incremental_init(&linecontrol_pid_left, 0.82, 0.6, 0.12, 0, 100);
    pid_incremental_init(&linecontrol_pid_right, 0, 0, 0, 0, 100);
#else
    pid_incremental_init(&linecontrol_pid_left, 0, 0.13, 0, 700, 1000);
    pid_incremental_init(&linecontrol_pid_right, 0, 0.13, 0, 700, 1000);
#endif
}

/**
 * @brief 巡线控制中断处理函数
 *
 */
void LineControl_IRQHandler(void)
{
    // 1. 转向环
    // 输入灰度值，查出权重
    // 权重总和作为输入，目标值是0，输出是转向所需的速度turn_speed（以50ms下编码器脉冲数为单位）

    // 2. 速度环
    // 编码器的值（50ms下的脉冲数）作为输入，目标是速度（以50ms下编码器的脉冲数为单位）
    // 输出是达到目标所需的pwm的ccr值

    // 寻迹的过程
    // 1. 转向环拿到turn_speed
    // 2. turn_speed叠加上base_speed得到最终目标的速度，作为速度环的输入

    float weight_sum_left = 0;
	  float weight_sum_right = 0;
	    float weight_sum=0;
    unsigned char digtal = IIC_Get_Digtal();
    s32 leften = motor_left_encoder();
    s32 righten = motor_right_encoder();
    motor_left_encoder_reset();
    motor_right_encoder_reset();
    debug_print("digtal:%d\n", (s32)digtal);
    for (uint8_t i = 1; i < 5; i++) {
        if ((digtal & (0X80 >> i-1)) == 0)
            weight_sum_left = weight[i-1];
				   // LCD_ShowFloatNum(0,0+i*16,weight[i-1],4,BLACK,WHITE,16);
    }
		for (uint8_t i = 5; i < 9; i++) {
        if ((digtal & (0X80 >> i-1)) == 0)
            weight_sum_right = weight[i-1];
				   // LCD_ShowFloatNum(0,0+i*16,weight[i-1],4,BLACK,WHITE,16);
    }
    // leften += weight_sum;
    // righten -= weight_sum;
    weight_sum=0;
		//确实该好好休息一下了
		    //float left_out = pid_incremental_calculate(&linecontrol_pid_left, leften);
        //float right_out = pid_incremental_calculate(&linecontrol_pid_right, righten);
		linecontrol_pid_left.target = weight_sum_right;
	
		float left_out = pid_incremental_calculate(&linecontrol_pid_left, weight_sum_left);
    debug_print("%d,%d,%d,%d\n", (int)left_speed, (int)right_speed, (int)left_out);
    left_speed -= left_out;
		if(left_speed<=0){left_speed=16/(abs(left_speed));};
		
    motor_set_speed(800+left_speed,800);
}




void DirectControl(float speed)
{
    current_mode = DIRECT_CONTROL;
    HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 开启陀螺仪接收中断
    ref_angel = get_angle_yaw();
    directcontrol_pid_left.target = speed;
    directcontrol_pid_right.target = speed;
}

void DirectControlInit(void) {}


void DirectControl_IRQHandler(void) {}




void SpeedControl(float left_speed, float right_speed)
{
    current_mode = SPEED_CONTROL;
    linecontrol_pid_left.target = left_speed;
    linecontrol_pid_right.target = right_speed;
}

void SpeedControlInit(void)
{
#if USE_13_MOTOR
    pid_incremental_init(&speedcontrol_pid_left, 0, 8, 0, 0, 100);
    pid_incremental_init(&speedcontrol_pid_right, 0, 8, 0, 0, 100);
#else
    pid_incremental_init(&speedcontrol_pid_left, 0, 0.13, 0, 700, 1000);
    pid_incremental_init(&speedcontrol_pid_right, 0, 0.13, 0, 700, 1000);
#endif
}


void SpeedControl_IRQHandler(void)
{
    s32 leften = motor_left_encoder();
    s32 righten = motor_right_encoder();
    motor_left_encoder_reset();
    motor_right_encoder_reset();
    left_speed += pid_incremental_calculate(&speedcontrol_pid_left, leften);
    right_speed += pid_incremental_calculate(&speedcontrol_pid_right, righten);
    motor_set_speed(left_speed, right_speed);
}




void SteeringControl(float angel)
{
    current_mode = STEERING_CONTROL;
    HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 开启陀螺仪接收中断
    float target = get_angle_yaw() + angel;
    if (target > 360)
        target -= 360;
    else if (target < 0)
        target += 360;
    steeringcontrol_pid.target = target;
}

void SteeringControlInit(void) {}


void SteeringControl_IRQHandler(void)
{
    float current_yaw = get_angle_yaw();
    float error = steeringcontrol_pid.target - current_yaw;
    if (error > 180)
        error -= 360;
    else if (error < -180)
        error += 360;
    float speed = pid_position_calculate_yaw(&steeringcontrol_pid, error);

    steeringcontrol_pid_left.target = speed;
    steeringcontrol_pid_right.target = -speed;
    s32 leften = motor_left_encoder();
    s32 righten = motor_right_encoder();
    motor_left_encoder_reset();
    motor_right_encoder_reset();
    left_speed += pid_incremental_calculate(&steeringcontrol_pid_left, leften);
    right_speed += pid_incremental_calculate(&steeringcontrol_pid_right, righten);
    motor_set_speed(left_speed, right_speed);
}

void set_base_speed(float lspeed, float rspeed)
{
    left_base_speed = lspeed;
    right_base_speed = rspeed;
}

/**
 * @brief 速度环控制，输入目标的速度，自动设置电机的占空比
 *
 * @param ltarget 左轮的目标速度，以编码器脉冲数为单位
 * @param rtarget 右轮的目标速度，以编码器脉冲数为单位
 */
void speed_loop_control(float ltarget, float rtarget)
{
    // 速度环
    // 编码器的值（50ms下的脉冲数）作为输入，目标是速度（以50ms下编码器的脉冲数为单位）
    // 输出是达到目标所需的pwm的ccr值

    // 拿编码器值
    s32 leften = motor_left_encoder();
    motor_left_encoder_reset();
    s32 righten = motor_right_encoder();
    motor_right_encoder_reset();
    // 记录路程
    left_distance += leften;
    right_distance += righten;
    // 计算所需的pwm值
    float left_out = pid_incremental_calculate(&linecontrol_pid_left, leften);
    float right_out = pid_incremental_calculate(&linecontrol_pid_right, righten);
    left_speed += left_out*6;
    right_speed += right_out*6;
    motor_set_speed(left_speed, right_speed);
}

/**
 * @brief 巡线控制
 * 
 */
void track_control(void)
{
    // 转向环
    // 输入灰度值，查出权重
    // 权重总和作为输入，目标值是0，输出是转向所需的速度turn_speed（以50ms下编码器脉冲数为单位）

    float weight_sum = 0;
    unsigned char digtal = IIC_Get_Digtal();

    debug_print("digtal:%d\n", (s32)digtal);
    for (uint8_t i = 0; i < 8; i++) {
        if ((digtal & (0X80 >> i)) != 1)
            weight_sum += weight[i];
    }
    // 形成差速
    left_turn_speed = weight_sum;
    right_turn_speed = -weight_sum;
    
    // 叠加，计算出综合速度
    left_speed  = left_base_speed + left_turn_speed;
    right_speed = right_base_speed + right_turn_speed;
    // 输入进速度环
    speed_loop_control(left_speed, right_speed);
}

void usage(void)
{
    set_base_speed(1000, 1000);
    track_control();
}
