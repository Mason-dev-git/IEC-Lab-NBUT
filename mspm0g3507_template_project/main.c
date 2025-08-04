//
// sysconfig 生成的    .isTimerWithFourCC = false,这一行需要删除
//
#include "bsp.h"

#define DELAY (16000000)


// 第一题
void on_question_1(void);
// 第二题
void on_question_2(void);
// 第三题
void on_question_3(void);
// 第四题
void on_question_4(void);

/////////////////////////////////////////////////////////////////////////////////////
// 基础直线速度
float left_base_speed, right_base_speed;
// 转向速度
float left_turn_speed, right_turn_speed;
// 合成速度
float left_speed = 350, right_speed = 350;
u32 left_distance = 0, right_distance = 0;
pid_incremental_t incremental_pid_left;
pid_incremental_t incremental_pid_right;

float maxf(float a, float b)
{
    return a > b ? a : b;
}
float minf(float a, float b)
{
    return a < b ? a : b;
}
// 目标圈数所需的转弯次数，1圈5个转弯，2圈9个转弯，3圈13个转弯，4圈17个转弯。
u32 target_turn_cnt;
// 当前转弯的次数
u32 current_turn_cnt;

// 当前问题编号
int32_t question = 1;


float Velocity_KP = 400, Velocity_KI = 300;
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_Left(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder;                                        // 计算偏差
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;   // 增量式PI控制器
    if (Pwm > 7800)
        Pwm = 7800;
    if (Pwm < -7800)
        Pwm = -7800;
    Last_bias = Bias;   // 保存上一次偏差
    return Pwm;         // 增量输出
}


int Incremental_PI_Right(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder;                                        // 计算偏差
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;   // 增量式PI控制器
    if (Pwm > 7800)
        Pwm = 7800;
    if (Pwm < -7800)
        Pwm = -7800;
    Last_bias = Bias;   // 保存上一次偏差
    return Pwm;         // 增量输出
}

/**
 * @brief 速度环控制，输入目标的速度，自动设置电机的占空比
 *
 * @param ltarget 左轮的目标速度，以编码器脉冲数为单位
 * @param rtarget 右轮的目标速度，以编码器脉冲数为单位
 */
void speed_loop_control(pid_incremental_t* pid_left, pid_incremental_t* pid_right)
{
    // 速度环
    // 编码器的值（50ms下的脉冲数）作为输入，目标是速度（以50ms下编码器的脉冲数为单位）
    // 输出是达到目标所需的pwm的ccr值

    // 拿编码器值
    s32 leften = motor_left_encoder();
    s32 righten = motor_right_encoder();
    motor_left_encoder_reset();
    motor_right_encoder_reset();
    // 记录路程
    left_distance += leften;
    right_distance += righten;

    // 计算PID输出
    float left_out = pid_incremental_calculate(pid_left, leften);
    float right_out = pid_incremental_calculate(pid_right, righten);

    // 增加死区控制，减少小幅度调整
    if (fabs(left_out) < 5.0f)
        left_out = 0;
    if (fabs(right_out) < 5.0f)
        right_out = 0;

    // 更新目标速度值
    left_speed += left_out;
    right_speed += right_out;

    // 限制速度范围

    left_speed = pid_out_clamp(left_speed, 9000);
    right_speed = pid_out_clamp(right_speed, 9000);

    debug_printf("%d,%d,%d,%d,%d,%d,%d,%d\n",
                 (int)leften,
                 (int)righten,
                 (int)left_speed,
                 (int)right_speed,
                 (int)left_out,
                 (int)right_out,
                 (int)pid_left->target,
                 (int)pid_right->target);

    motor_set_speed(left_speed, right_speed);
}






void angle_loop_control(float current_angle)
{
    // float current_angle = wit_data.yaw;
}










// 各路灰度权重
float weight[7] = {5, 3, 1, 0, -1, -3, -5};
// 灰度权重和
int16_t weight_sum;
// 基础速度（推荐值）
float base_speed = 16;
pid_incremental_t tracking_incremental_pid_left;
pid_incremental_t tracking_incremental_pid_right;
void tracking_control(uint8_t state)
{
    debug_printf("state=%d\n", state);
    weight_sum = 0;
    for (uint8_t i = 0; i < 7; i++) {
        if ((state >> (7 - i)) & 0x01)
            weight_sum += weight[i];
    }
    debug_printf("weight_sum=%d\n", weight_sum);
    tracking_incremental_pid_left.target = base_speed - weight_sum;
    tracking_incremental_pid_right.target = base_speed + weight_sum;
    debug_printf("target %.2f,%.2f\n", tracking_incremental_pid_left.target, tracking_incremental_pid_right.target);
    speed_loop_control(&tracking_incremental_pid_left, &tracking_incremental_pid_right);
}



////////////////////////////////////////////////////////////////////////////////////
// 主函数
int main(void)
{
    // 为了防止出现进不了中断，在一开始先清除标志位
    NVIC_ClearPendingIRQ(GPIO_GRP_ENCODER_INT_IRQN);
    NVIC_ClearPendingIRQ(UART_DEBUG_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(UART_WIT_INST_INT_IRQN);
    // 芯片资源初始化,由SysConfig配置软件自动生成
    SYSCFG_DL_init();

    // 开启中断
    NVIC_EnableIRQ(GPIO_GRP_ENCODER_INT_IRQN);
    NVIC_EnableIRQ(UART_DEBUG_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);   // 定时器中断

    // 外设初始化
    debug_init(UART_DEBUG_INST);
    // 输出调试信息
    DEBUG_INFO("start");

    // 维特陀螺仪初始化
    WIT_Init();
    // oled初始化
    OLED_Init();
    OLED_Clear();
    DEBUG_INFO("OLED_Init");

    // 按键初始化
    key_init();
    DEBUG_INFO("key_init");

    // 初始化pid
    // 分别调整左右轮PID参数，解决不对称问题
    // 左轮参数 - 降低比例系数，减小积分系数
    pid_incremental_init(&incremental_pid_left, 45, 6, 0, 16, 5000);
    // 右轮参数 - 增大比例系数，因为右轮似乎动力不足
    pid_incremental_init(&incremental_pid_right, 45, 6, 0, 16, 5000);

    // 巡线参数初始化
    pid_incremental_init(&tracking_incremental_pid_left, 40, 10, 0, 16, 5000);
    pid_incremental_init(&tracking_incremental_pid_right, 40, 10, 0, 16, 5000);
    DEBUG_INFO("pid_init");

    // 开电机的定时器
    DL_TimerG_startCounter(PWM_FRONT_MOTOR_INST);

    // 开pid控制的定时器
    DL_TimerG_startCounter(TIMER_0_INST);

    /********debuge测试 ********/
//     while (1) {
//         debug_printf("%.2f,%.2f,%.2f\n", wit_data.roll, wit_data.pitch, wit_data.yaw);
//         delay_ms(100);
//     }
    /********电机编码器测试 ********/
    motor_set_speed(0, 0);
// while (1) {
//     s32 leften = motor_left_encoder();
//     s32 righten = motor_right_encoder();
//     motor_left_encoder_reset();
//     motor_right_encoder_reset();
//     debug_printf("%d,%d\n", leften, righten);
//     delay_ms(20);
// }
    /*******灰度调试********/
// while (1) {
//     uint8_t state = gray_scan();
//     debug_printf("state=%d\n", state);
//     delay_ms(20);
// }
    /******************************/

    // 获取按键键值
    // 阻塞式读取按键
    extern int32_t key;
    char buf[32];
    ui_loop();

    switch (question) {
       case 1: on_question_1(); break;
       case 2: on_question_2(); break;
       case 3: on_question_3(); break;
       case 4: on_question_4(); break;
    }

    //  - 9550
    int16_t sum;
    while (1) {
        /*******************************/
        OLED_Clear();
        sprintf(buf, "turn times:%d", target_turn_cnt);
        OLED_ShowString(2, 1, (u8*)buf, 8);
        // while (1) {
        //     int32_t e1 = motor_encoder_get_count(&g_encoder_e1);
        //     // 清空编码器的count
        //     motor_encoder_clear_count(&g_encoder_e1);
        //     debug_printf("e1=%d", e1);
        //     key = key_get_value();
        //     sprintf(buf, "%d", key);
        //     OLED_ShowString(2, 1, (u8*)buf, 8);

        // delay_ms(20);
        // }
        //    while (1) {
        //        // 调试打印陀螺仪数据（欧拉角）
        //        // debug_printf("%.2f,%.2f,%.2f\n",wit_data.roll,wit_data.pitch,wit_data.yaw);
        //        /*******加权求和巡线*********/
        //        //        uint8_t state = gray_scan();
        //        //        debug_printf("state=%d\n",state);
        //        //        sum = 0;
        //        //        for (uint8_t i = 0; i < 7; i++) {
        //        //        if ((state>>(7-i)) & 0x01)
        //        //            sum+=weight[i];
        //        //        }
        //        //        if(sum>0)
        //        //        {
        //        //        motor_set_speed(3000,3000+sum);
        //        //        }else if(sum<0){
        //        //        motor_set_speed(3000+sum,3000);
        //        //        }else{
        //        //        motor_set_speed(3000,3000);
        //        //        }
        //        //        delay_ms(20);


        //        /********定值巡线*********/
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_4_PORT, GPIO_GRP_GRAY_PIN_SENSER_4_PIN))
        //        // {
        //        //     motor_set_speed(3000,3000);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_3_PORT, GPIO_GRP_GRAY_PIN_SENSER_3_PIN))
        //        // {
        //        //     motor_set_speed(3000,3100);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_5_PORT, GPIO_GRP_GRAY_PIN_SENSER_5_PIN))
        //        // {
        //        //     motor_set_speed(3100,3000);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_2_PORT, GPIO_GRP_GRAY_PIN_SENSER_2_PIN))
        //        // {
        //        //     motor_set_speed(3000,3300);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_6_PORT, GPIO_GRP_GRAY_PIN_SENSER_6_PIN))
        //        // {
        //        //     motor_set_speed(3300,3000);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_1_PORT, GPIO_GRP_GRAY_PIN_SENSER_1_PIN))
        //        // {
        //        //     motor_set_speed(3000,3500);
        //        // }
        //        // if(DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_7_PORT, GPIO_GRP_GRAY_PIN_SENSER_7_PIN))
        //        // {
        //        //     motor_set_speed(3500,3000);
        //        // }
        //        delay_ms(0);
        //        /*******************************/
        //    }
        //    ui_loop();
        // while (1) {
        // int32_t e1 =  motor_encoder_get_count(&g_encoder_e1);
        // 清空编码器的count
        // motor_encoder_clear_count(&g_encoder_e1);
        // debug_printf("e1=%d",e1);
        // key= key_get_value();
        // sprintf(buf, "%d", key);
        // OLED_ShowString(2, 1, (u8 *)buf, 8);

        // delay_ms(50);
        // }

        //    DEBUG_INFO("final question=%d", question);
        //    sprintf(buf, "final question=%d", question);
        //    OLED_ShowString(1, 1, (u8*)buf, 16);

           switch (question) {
               case 1: on_question_1(); break;
               case 2: on_question_2(); break;
               case 3: on_question_3(); break;
               case 4: on_question_4(); break;
           }
           delay_ms(20);
    }
}


float target;
// pid控制定时器，20ms进入一次
void TIMER_0_INST_IRQHandler()
{
    static unsigned int beeptimecount = 0;
    // DEBUG_INFO("TIMER_0_INST_IRQHandler");
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERA_IIDX_ZERO: {
            // speed_loop_control(incremental_pid_left,incremental_pid_right);
            // tracking_control();
            // s32 leften = motor_encoder_get_count(&g_encoder_e1);
            // motor_encoder_clear_count(&g_encoder_e1);
            // s32 righten = motor_encoder_get_count(&g_encoder_e2);
            // motor_encoder_clear_count(&g_encoder_e2);
            // debug_printf("%d,%d\n", leften, righten);

            // switch (g_car_state) {
            // case car_state_question_1_plan_2_run:
            // {
            //     // 获取当前的计数值
            //     int32_t lcount = g_encoder_e1.count;
            //     int32_t rcount = g_encoder_e2.count;
            //     // 清空计数值
            //     g_encoder_e1.count = 0;
            //     g_encoder_e2.count = 0;
            //     // 累加所走过的路程
            //     leftroad += lcount;
            //     rightroad += rcount;

            //     Flag_PID++;   // PID控制标志加一
            //     Flag_Encode = 1;


            // } break;
            // case car_state_question_1_run: car_speed_control(); break;
            // default: break;
            // }
        } break;
        default: break;
    }
}

void xunji(void);

// 第一题
void on_question_1(void)
{
    xunji();
}
// 第二题
void on_question_2(void) {}
// 第三题
void on_question_3(void) {}
// 第四题
void on_question_4(void) {}




// 第一小题（指定圈数巡线）
float judge_angel(float angel)
{
    // 每次逆时针旋转加90°
    // 0 -> 90 -> 180 -> 270 -> 0
    if (angel < 180 && angel > 90) {
        return angel - 270;
    }
    return angel + 90;
}

float calc_distance(float angel1, float angel2)
{
    float diff = ((int)(angel2 - angel1)) % 360;
    if (diff > 180) {
        diff = 360 - diff;
    }
    return diff;
}

void xunji(void)
{
    uint8_t current_turn_cnt = 0;
    float current_angel = 0;
    float target_angel = 0;
    int flag = 0;
    while (current_turn_cnt < target_turn_cnt) {
        uint8_t gray = gray_scan();
        debug_printf("gray=%d\n", gray);
        if (gray != 0)   // 正常巡直线
        {
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_4_PORT, GPIO_GRP_GRAY_PIN_SENSER_4_PIN)) {
							         motor_set_speed(3000, 3000);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_3_PORT, GPIO_GRP_GRAY_PIN_SENSER_3_PIN)) {
							         motor_set_speed(3000, 3100);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_5_PORT, GPIO_GRP_GRAY_PIN_SENSER_5_PIN)) {
							         motor_set_speed(3100, 3000);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_2_PORT, GPIO_GRP_GRAY_PIN_SENSER_2_PIN)) {
							         motor_set_speed(3000, 3300);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_6_PORT, GPIO_GRP_GRAY_PIN_SENSER_6_PIN)) {
							         motor_set_speed(3300, 3000);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_1_PORT, GPIO_GRP_GRAY_PIN_SENSER_1_PIN)) {
							         motor_set_speed(3000, 3500);
							     }
							     if (DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_7_PORT, GPIO_GRP_GRAY_PIN_SENSER_7_PIN)) {
							         motor_set_speed(3500, 3000);
							     }
        }
        else {   // 脱线转弯
            float last_angel = wit_data.yaw;
            tracking_incremental_pid_left.target = 1;
            tracking_incremental_pid_right.target = 10;
            while (1) {
                float current_angel = wit_data.yaw;
                float max_angel = maxf(last_angel, current_angel);
                float min_angel = minf(last_angel, current_angel);
                float diff = calc_distance(min_angel, max_angel);
                 gray_scan();
                if (!(diff >= 75 || v1 == 1 || v2 == 1|| v3 ==1)) {
                     motor_set_speed(0, 2000);
                   
                }else{break;}
                //speed_loop_control(&tracking_incremental_pid_left, &tracking_incremental_pid_right);
                debug_printf("current_angel: %f, last_angel: %f, diff: %f\n", current_angel, last_angel, diff);
                delay_ms(20);
            }
            current_turn_cnt++;

        delay_ms(20);
    }
    motor_set_speed(0, 0);
    while(1);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0 

// 小车的状态
static enum {
    car_state_stop,
    car_state_question_1_run,
    car_state_question_1_plan_2_run,
    car_state_question_track,
    car_state_question_2_run,
} g_car_state = car_state_stop;

// 前置函数声明

// 第一题
void on_question_1(void);
// 第二题
void on_question_2(void);
// 第三题
void on_question_3(void);
// 第四题
void on_question_4(void);
// 小车停止
void car_stop(void);
// 小车前进
void car_forward(int32_t speed);
// 小车转angle度，左转是负，右转是正
void car_turn_angle(float angle);
// 速度环控制
void car_speed_control(void);
// 角度环控制
void car_angle_control(void);

// 陀螺仪数据
float g_acc_x, g_acc_y, g_acc_z;         // 加速度
float g_gyro_x, g_gyro_y, g_gyro_z;      // 角速度
float g_angle_x, g_angle_y, g_angle_z;   // 角度

PidTypeDef g_pid1;        // 左前轮速度的目标值
PidTypeDef g_pid2;        // 右前轮速度的目标值
PidTypeDef g_pid_angle;   // 角度的pid

// 速度为单位时间内编码器的脉冲数
int32_t g_target_speed1;
int32_t g_target_speed2;
int32_t g_target_angle;

float L_Kp, L_Ki, L_Kd, R_Kp, R_Ki, R_Kd;
double SumError = 0, LastError = 0;
int dError = 0;
double RSumError = 0, RLastError = 0;
int RdError = 0;
unsigned int outpwml, outpwmr, setspeedr, setspeedl;

uint32_t leftroad, rightroad;
uint32_t setleftroad, setrightroad;

float Road_L_Kp, Road_L_Ki, Road_L_Kd, Road_R_Kp, Road_R_Ki, Road_R_Kd;
double Road_SumError = 0, Road_LastError = 0;
int Road_dError = 0;
double Road_RSumError = 0, Road_RLastError = 0;
int Road_RdError = 0;

    // 初始化pid
    PID_Init(&g_pid1, PID_DELTA, PID1_KP, PID1_KI, PID1_KD, PID1_MAX_OUT, PID1_MAX_IOUT);
    PID_Init(&g_pid2, PID_DELTA, PID2_KP, PID2_KI, PID2_KD, PID2_MAX_OUT, PID2_MAX_IOUT);
    PID_Init(&g_pid_angle, PID_DELTA, PIDA_KP, PIDA_KI, PIDA_KD, PIDA_MAX_OUT, PIDA_MAX_IOUT);
    DEBUG_INFO("pid_init");

// 根据占空比设置左前轮目标速度
void set_target1_speed(int32_t speed)
{
    g_target_speed1 = speed;
    //    char buf[32];
    //    sprintf(buf, "res1=%.3f", g_target_speed1);
    //    OLED_ShowString(2, 1, buf);
}
// 根据占空比设置右前轮目标速度
void set_target2_speed(int32_t speed)
{
    g_target_speed2 = speed;
}
// 设置目标角度
void set_target_angle(float angle)
{
    g_target_angle = angle;
}
// 左转，要求右轮速度大于左轮速度
void car_turn_left(int32_t leftspeed, int32_t rightspeed)
{
    g_target_speed1 = leftspeed;
    g_target_speed2 = rightspeed;
}
// 右转，要求左轮速度大于右轮速度
void car_turn_right(int32_t leftspeed, int32_t rightspeed)
{
    g_target_speed1 = leftspeed;
    g_target_speed2 = rightspeed;
}

uint8_t Flag_PID = 0;
uint8_t Flag_Encode = 0;

int32_t detect_black_lines(void)
{
    uint8_t gray_arr[5];
    // 读取灰度的数据
    // stats_out(GPIOA, Front_one, gray_arr, 5);
    // 判断是否有黑线
    for (int i = 0; i < 5; i++) {
        if (gray_arr[i] == 1) {
            return 1;
        }
    }
    return 0;   // 没有黑线
}

void track(void)
{
    // 寻迹
    while (1) {
        uint8_t gray_arr[5];
        // 读取灰度的数据
        // stats_out(GPIOA, Front_one, gray_arr, 5);

        // todo
        // 获取灰度的数据
        if (gray_arr[0] == 1) {
            // 调整车左右轮子的速度
            car_turn_left(40, 30);
        }
        if (gray_arr[1] == 1) {
            // 调整车左右轮子的速度
            car_turn_left(40, 30);
        }
        if (gray_arr[2] == 1) {
            // 调整车左右轮子的速度
            car_turn_left(40, 30);
        }
        if (gray_arr[3] == 1) {
            // 调整车左右轮子的速度
            car_turn_left(40, 30);
        }
        if (gray_arr[4] == 1) {
            // 调整车左右轮子的速度
            car_turn_left(40, 30);
        }

        // 发现没有线了
        if (detect_black_lines() == 0) {
            // 如果检测不到黑线
            // 开灯光和蜂鸣器响应

            break;
        }
    }
}

void L_Pid_Ctrl(unsigned int Target, unsigned int Real)   // PID速度控制函数
{
    int Error = 0;   // 本次差值，上次差值
    float j = 0.0, i;

    if (Target == 0) {
        LastError = 0;
        dError = 0;
        SumError = 0;

        outpwml = 0;
        motor_set_compare_val(&g_motor_fl, outpwml);
        return;
    }

    Error = (Target - Real);      // 本次偏差
    if (SumError * L_Ki > 1600)   // 计算积分项
    {
        SumError = 1600;
        SumError = SumError / L_Ki;
    }   // 输出到最大值后，不再累计偏差，抗积分饱各
    else
        SumError += Error;

    dError = Error - LastError;   // 微分项，偏差的变化
    LastError = Error;            // 记录本次偏差，用于下次计算

    i = L_Kp;
    j = Error * i;
    i = L_Ki;
    j = j + SumError * i;
    i = L_Kd;
    j = j + dError * i;

    if (j > 1600)
        outpwml = 1600 + 1;
    else if (j < 1)
        outpwml = 1;
    else
        outpwml = j;
    motor_set_compare_val(&g_motor_fl, outpwml);
}

void R_Pid_Ctrl(unsigned int Target, unsigned int Real)   // PID速度控制函数
{
    int RError = 0;   // 本次差值，上次差值
    float j = 0.0, i;

    if (Target == 0) {
        RLastError = 0;
        RdError = 0;
        RSumError = 0;

        outpwmr = 0;
        motor_set_compare_val(&g_motor_fr, outpwml);
        return;
    }

    RError = (Target - Real);      // 本次偏差
    if (RSumError * R_Ki > 1600)   // 计算积分项
    {
        RSumError = 1600;
        RSumError = RSumError / L_Ki;
    }   // 输出到最大值后，不再累计偏差，抗积分饱各
    else
        RSumError += RError;

    RdError = RError - RLastError;   // 微分项，偏差的变化
    RLastError = RError;             // 记录本次偏差，用于下次计算

    i = R_Kp;
    j = RError * i;
    i = R_Ki;
    j = j + RSumError * i;
    i = R_Kd;
    j = j + RdError * i;

    if (j > 1600)
        outpwmr = 1600 + 1;
    else if (j < 1)
        outpwmr = 1;
    else
        outpwmr = j;

    motor_set_compare_val(&g_motor_fr, outpwml);
}
int32_t SPEED_RIGHT, SPEED_LEFT;   // 这两个应该是编码器脉冲数为单位，目前还没有写
// 第一题
void on_question_1(void)
{
    // 基于纯编码器的实现，一点一点挪动，然后修正
    g_car_state = car_state_question_1_plan_2_run;   // 切换到方案2的中断代码

    // 设置一个初始速度
    car_forward(80);

    // 理论上走完从A到B所需的编码器数值
    uint32_t left_road = 5000;    // unused
    uint32_t right_road = 5000;   // unused
    while (1) {
        // 读取灰度的值，并且通过灰度来判断是否检测到黑线
        if (detect_black_lines() == 1) {
            // 如果检测到黑线，先关闭电机（停车）
            motor_off(&g_motor_fl);
            motor_off(&g_motor_fr);
            // 开灯光和蜂鸣器响应

            break;
        }
        else {
            if (Flag_PID >= 1) {   // 保证一次中断以后产生一次pid控制
                // 根据编码器累加的值，不断调整速度
                if (rightroad > leftroad + 20) {
                    Flag_PID = 0;
                    car_turn_right(80, 40);
                }
                else if (leftroad > rightroad + 20) {
                    Flag_PID = 0;
                    car_turn_right(40, 80);
                }
                else {
                    Flag_PID = 0;
                    car_turn_right(80, 80);
                }

                R_Pid_Ctrl(setspeedr, SPEED_RIGHT);
                L_Pid_Ctrl(setspeedl, SPEED_LEFT);
            }
        }
        if (Flag_Encode == 1)   // 标志位检测执行 10ms 实时速度显示
        {
            char buf[256] = {0};
            // 获取当前的计数值
            int32_t lcount = -g_encoder_e1.count;   // 因为是50ms
            int32_t rcount = g_encoder_e2.count;

            sprintf(buf, "e1=%d,e2=%d\n", g_encoder_e1.count, g_encoder_e2.count);
            OLED_Clear();
            OLED_ShowString(1, 1, (u8*)buf, 16);
            // 清空计数值
            g_encoder_e1.count = 0;
            g_encoder_e2.count = 0;
            Flag_Encode = 0;
        }
    }

    while (1)
        ;

        // 基于速度式pid的实现
#    if 0    
    
    
    // 清空pid
    PID_clear(&g_pid1);
    PID_clear(&g_pid2);

    // 设置小车为前进，速度为800
    car_forward(80); 
    // 迁移小车状态为car_state_question_1_run
    g_car_state = car_state_question_1_run;
    while (1) {
        // 读取灰度的值，并且通过灰度来判断是否检测到黑线
        if(detect_black_lines() == 1){
            // 如果检测到黑线，先关闭电机（停车）
            motor_off(&g_motor_fl);
            motor_off(&g_motor_fr);
            // 再切换为car_state_stop状态
            g_car_state = car_state_stop;
            // 最后开灯光和蜂鸣器响应
          
            break;
        }
        delay_ms(10);
    }
    delay_ms(50);
 
    
    while(1);

#    endif
}

// 第二题
void on_question_2(void)
{
    // 设置为前进
    car_forward(800);
    // 切换到第一题的状态
    g_car_state = car_state_question_1_run;
    while (1) {
        // 读取灰度的值，并且通过灰度来判断是否检测到黑线
        if (detect_black_lines() == 1) {
            // 如果检测到黑线
            // 开灯光和蜂鸣器响应

            break;
        }
        delay_ms(10);
    }

    // 切换状态到track
    g_car_state = car_state_question_track;



    // 开始寻迹
    track();

    // 设置为前进
    // 切换到第二题的状态
    g_car_state = car_state_question_2_run;
    while (1) {
        // 读取灰度的值，并且通过灰度来判断是否检测到黑线
        if (detect_black_lines() == 1) {
            // 如果检测到黑线
            // 开灯光和蜂鸣器响应

            break;
        }
        else {}
        delay_ms(10);
    }
    // 切换状态到track
    g_car_state = car_state_question_track;


    // 开始寻迹
    track();

    // 停车
    motor_off(&g_motor_fl);
    motor_off(&g_motor_fr);
}

// 第三题
void on_question_3(void) {}

// 第四题
void on_question_4(void) {}

// 速度环控制
void car_speed_control(void)
{
    char buf[256] = {0};
    // 获取当前的计数值
    int32_t lcount = -g_encoder_e1.count;   // 因为是50ms
    int32_t rcount = g_encoder_e2.count;

    //    sprintf(buf, "e1=%d,e2=%d\n", g_encoder_e1.count, g_encoder_e2.count);
    // OLED_Clear();
    // OLED_ShowString(1, 1, buf);
    // 清空计数值
    g_encoder_e1.count = 0;
    g_encoder_e2.count = 0;
    // 计算pid，并且设置占空比
    // 根据目标速度计算出所需的compare值
    float res1 = Limit_Min_Max(PID_Calc(&g_pid1, lcount, g_target_speed1), 0, 3200);
    float res2 = Limit_Min_Max(PID_Calc(&g_pid2, rcount, g_target_speed2), 0, 3200);

    sprintf(buf,
            "%f,%f,%f,%f,%f,%f\n",
            (float)lcount,
            (float)g_target_speed1,
            (float)rcount,
            (float)g_target_speed2,
            (float)res1,
            (float)res2);
    usart_send_string(buf);

    // sprintf(buf, "res1=%d", (int)res1);

    // OLED_ShowString(2, 1, buf);

    //  sprintf(buf, "res2=%d", g_target_speed1);

    //  OLED_ShowString(3, 1, buf);
    // 根据pid算出来的速度设置compare value
    motor_set_compare_val(&g_motor_fl, res1);
    motor_set_compare_val(&g_motor_fr, res2);

#    if 0  
    char buf[32];
    // 获取当前的计数值
    int32_t lcount = g_encoder_e1.count; // 因为是50ms
    int32_t rcount = g_encoder_e2.count;
    sprintf(buf, "e1=%d,e2=%d", g_encoder_e1.count, g_encoder_e2.count);
    OLED_Clear();
    OLED_ShowString(1, 1, buf);
    // 清空计数值
    g_encoder_e1.count = 0;
    g_encoder_e2.count = 0;      
    // 计算pid，并且设置占空比
    // 根据目标速度计算出所需的compare值
    float res1 = PID_Calc(&g_pid1, lcount,g_target_speed1);
    float res2 = PID_Calc(&g_pid2, rcount,g_target_speed2);
    sprintf(buf, "res1=%d", (int)res1);
    OLED_ShowString(2, 1, buf);
    sprintf(buf, "res2=%d", g_target_speed1);
    OLED_ShowString(3, 1, buf);
    // 根据pid算出来的速度设置compare value
    motor_set_compare_val(&g_motor_fl, res1);
    motor_set_compare_val(&g_motor_fr, res2);
#    endif
}

// TODO: 角度环控制
void car_angle_control(void)
{
    // 获取偏航角yaw
    jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);
    // float yaw = g_angle_z;//-180~+180
    float delta;

    // 计算角度差距量(自己可以去推算一下对不对)
    if (g_target_angle - g_angle_z > 180) {
        delta = g_angle_z - g_target_angle + 360;
    }
    else if (g_angle_z - g_target_angle > 180) {
        delta = g_angle_z - g_target_angle - 360;
    }
    else {
        delta = g_angle_z - g_target_angle;
    }

    if (delta > -1.5f && delta < 1.5f)   // 死区，可以适当调整
    {
        PID_clear(&g_pid_angle);   // PID运算结束
        return;
    }

    // 计算pid
    float res = PID_Calc(&g_pid_angle, 0, delta);
    // 设置电机方向，电机方向自行调整，这里不一定对
    if (res > 0) {
        motor_set_direction(&g_motor_fl, motor_direction_corotation);
        motor_set_direction(&g_motor_fr, motor_direction_inversion);
    }
    else {
        motor_set_direction(&g_motor_fl, motor_direction_inversion);
        motor_set_direction(&g_motor_fr, motor_direction_corotation);
    }
    // 设置结果值，还是要注意方向问题
    set_target1_speed(res);
    set_target2_speed(res);

    // 不要让两个car_speed_control()同时运行
    car_speed_control();
}

// 小车停止
void car_stop(void)
{
    // 关电机
    motor_off(&g_motor_fl);
    motor_off(&g_motor_fr);
    // 清空pid
    PID_clear(&g_pid1);
    PID_clear(&g_pid2);
}

// 小车前进
void car_forward(int32_t speed)
{
    set_target1_speed(speed);
    set_target2_speed(speed);

    motor_set_compare_val(&g_motor_fl, speed);
    motor_set_compare_val(&g_motor_fr, speed);

    // 开电机
    motor_set_direction(&g_motor_fl, motor_direction_corotation);
    motor_set_direction(&g_motor_fr, motor_direction_corotation);
}

// 小车转angle度，左转是负，右转是正
void car_turn_angle(float angle)
{
    //    // 获取偏航角yaw
    //    jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);
    //    float yaw = g_angle_z;
    //
    //    PID_calc()
}

// unused in main
#    if 0

    // 开启蜂鸣器
//    beep_on(&g_beep);

    // 开启rgb led
//    rgb_led_set(&g_rgb_led, true, false, false);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, false, true, false);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, false, false, true);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, true, true, false);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, true, false, true);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, false, true, true);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, true, true, true);
//    delay_cycles(DELAY);   //延时0.5秒
//    rgb_led_set(&g_rgb_led, false, false, false);

    //DL_UART_disable();
//        sprintf(buf, "s1=%.2f", g_target_speed1);
//        OLED_ShowString(2, 1, buf);
//        sprintf(buf, "s2=%.2f", g_target_speed2);
//        OLED_ShowString(3, 1, buf);
        
        //        jy61p_get_acc(&g_acc_x, &g_acc_y, &g_acc_z);
        //        sprintf(buf, "ax=%.3f",  g_acc_x);
        //        OLED_ShowString(2, 1, buf);
        //        sprintf(buf, "ay=%.3f", g_acc_y);
        //        OLED_ShowString(3, 1, buf);
        //        sprintf(buf, "az=%.3f", g_acc_z);
        //        OLED_ShowString(4, 1, buf);

        //        jy61p_get_gyro(&g_gyro_x, &g_gyro_y, &g_gyro_z);
        //        sprintf(buf, "gx=%.3f",  g_gyro_x);
        //        OLED_ShowString(2, 1, buf);
        //        sprintf(buf, "gy=%.3f", g_gyro_y);
        //        OLED_ShowString(3, 1, buf);
        //        sprintf(buf, "gz=%.3f", g_gyro_z);
        //        OLED_ShowString(4, 1, buf);

//        jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);
//        sprintf(buf, "ax=%.3f", g_angle_x);
//        OLED_ShowString(2, 1, buf);
//        sprintf(buf, "ay=%.3f", g_angle_y);
//        OLED_ShowString(3, 1, buf);
//        sprintf(buf, "az=%.3f", g_angle_z);
//        OLED_ShowString(4, 1, buf);

        //        jy61p_get_gyro(&g_gyro_x, &g_gyro_y, &g_gyro_z);
        //        sprintf(buf, "gx=%.1f,gy=%.1f,gz=%.1f",   g_gyro_x, g_gyro_y, g_gyro_z);
        //        OLED_ShowString(3, 1, buf);
        //
        //        jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);
        //        sprintf(buf, "ax=%.1f,ay=%.1f,az=%.1f",   g_angle_x, g_angle_y, g_angle_z);
        //        OLED_ShowString(4, 1, buf);

        //        jy61p_get_acc(&g_acc_x, &g_acc_y, &g_acc_z);
        //        jy61p_get_gyro(&g_gyro_x, &g_gyro_y, &g_gyro_z);
        //        jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);

        //        DEBUG_INFO("g_acc_x=%.3f, g_acc_y=%.3f, g_acc_z=%.3f", g_acc_x, g_acc_y, g_acc_z);
        //        DEBUG_INFO("g_gyro_x=%.3f, g_gyro_y=%.3f, g_gyro_z=%.3f", g_gyro_x, g_gyro_y,
        //        g_gyro_z); DEBUG_INFO("g_angle_x=%.3f, g_angle_y=%.3f, g_angle_z=%.3f", g_angle_x,
        //        g_angle_y, g_angle_z);
        //        if(g_encoder_e1.count > 1500 && g_encoder_e2.count > 1500){
//            motor_off(&g_motor_fl);
//            motor_off(&g_motor_fr);
//        }
        
//        char buf[32];
//        

//        car_speed_control();
//        
//        delay_ms(50);
#        if 0

    //    DEBUG_INFO("motor->timer = %d", (int)PWM_FRONT_MOTOR_INST);
    //    DEBUG_INFO("motor->channel = %d", GPIO_PWM_FRONT_MOTOR_C1_IDX);
    //    DL_TimerA_setCaptureCompareValue(PWM_FRONT_MOTOR_INST, 1600, GPIO_PWM_FRONT_MOTOR_C1_IDX);

    // DL_GPIO_setPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN);
    // DL_GPIO_clearPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN);



    //    OLED_Clear();
    //    OLED_ShowString(1,1,"test str", OLED_8X16);
    //    OLED_Update();
#        endif
    int i       = 0;
    g_car_state = car_state_normal_run;
    uint16_t data;
    while (1) {

        // car_run();

        DEBUG_INFO("i = %d", i++);

        DEBUG_INFO("data=%d", data);
//        jy61p_get_acc(&g_acc_x, &g_acc_y, &g_acc_z);
//        jy61p_get_gyro(&g_gyro_x, &g_gyro_y, &g_gyro_z);
//        jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);

//        DEBUG_INFO("g_acc_x=%.3f, g_acc_y=%.3f, g_acc_z=%.3f", g_acc_x, g_acc_y, g_acc_z);
//        DEBUG_INFO("g_gyro_x=%.3f, g_gyro_y=%.3f, g_gyro_z=%.3f", g_gyro_x, g_gyro_y, g_gyro_z);
//        DEBUG_INFO("g_angle_x=%.3f, g_angle_y=%.3f, g_angle_z=%.3f", g_angle_x, g_angle_y, g_angle_z);
        delay_cycles(DELAY);   //延时0.5秒
        // DEBUG_INFO("e1.count = %d", g_encoder_e1.count);
        // DEBUG_INFO("e2.count = %d", g_encoder_e2.count);

        // DL_GPIO_togglePins(GPIO_GRP_BEEP_PORT,GPIO_GRP_BEEP_PIN_BEEP_PIN);//电平翻转驱动LED
        // DL_GPIO_togglePins(PORTB_PORT,PORTB_RGB_R_PIN | PORTB_RGB_G_PIN |
        // PORTB_RGB_B_PIN);//电平翻转驱动RGB
    }
#    endif

// unused
#    if 0

//void pid_speed_control(motor_t* motor, motor_encoder_t* encoder, pid_incremental_t* pid,
//                       float target)
//{
//    // 1.设定目标速度
//    // 2.获取当前速度
//    // 3.输入PID控制器进行计算（注意：电机输出限幅）
//    // 4.PID的输出值，输入给电机
//    //float duty     = pid_incremental_calculate(pid, target, encoder->count);
//    //encoder->count = 0;
//    //motor_set_duty(motor, duty);

//    //
//    //	if(motorA_dir){
//    //        motorA.now = Encoder_count1;
//    //    }
//    //    else{
//    //        motorA.now = -Encoder_count1;
//    //    }
//    //	if(motorB_dir){
//    // motorB.now = Encoder_count2;
//    //}
//    //    else{
//    // motorB.now = -Encoder_count2;
//    //}
//    //	Encoder_count1 = 0;
//    //	Encoder_count2 = 0;
//    //
//    //	pid_cal(&motorA);
//    //	pid_cal(&motorB);
//    //	//
//    //	pidout_limit(&motorA);
//    //	pidout_limit(&motorB);
//    //
//    //	motorA_duty(motorA.out);
//    //	motorB_duty(motorB.out);

//    //	datavision_send();
//}

void pid_angle_control(float angle) {
    // 1. 设定目标角度
    // 2. 获取当前角度
    // 3. 将当前值和目标值进行位置式pid计算
    // 4. 设置单电机目标速度，将角度环的输出设置为速度环的目标值（角度环的输出没有占空比限制）

}

#    endif
//

#endif
