#include "test.hpp"


Motor motor123;


void Motor_Init_Control(void)
{
	motor123.Init();
	motor123.Set_limit_spd(800);
	motor123.Enable();
}

void Motor_Run(void)
{
	motor123.Run_straight(0);
}



