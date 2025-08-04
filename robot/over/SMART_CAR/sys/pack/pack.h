#ifndef __PACK_H__
#define __PACK_H__

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/*C++编译器以C语言规则编译，宏__cplusplus在使用C++编译器时被定义*/
#ifdef __cplusplus
extern "C" {
#endif
	
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */	

#include "WT901C.h"	
#include "sys.h"
#include "delay.h"
#include "lcd.h"

/* USER CODE END Includes */

/* Private define -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define      IF_FLAT           flag_Device.Infrared_down_left==Offline&&flag_Device.Infrared_down_right==Online
#define      IF_UPHILL         flag_Device.Infrared_down_left==Online&&flag_Device.Infrared_down_right==Online
#define      IF_DOWNHILL       flag_Device.Infrared_down_left==Offline&&flag_Device.Infrared_down_right==Offline

/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

extern int16_t Encoder_Left_Overflow_Count;		 
extern int16_t Encoder_Right_Overflow_Count;	
//extern uint8_t Camara_RxBuffer[80];          //摄像头串口接收缓冲

/* 名为location的位置集 */
struct Slocation
{
  u8 terrace;		                           //平台    5的时候模式1，6的时候模式2，7的时候模式3
  u8 fork;			                           //岔路口标志(每阶段都会清零)
  u8 mode;			                           //模式，用于二次跑线 0：未到门，1：3号门开，2：4号门开，3：3、4门都没开
};

/* 放置所有行驶状态位的结构体 */
struct Sflag
{
	/*路线规划标志位*/
	u8 Routing;  				
	/*发车标志位*/
	u8 start;    				
	/*宝物位置标志位（用于抓宝定位）有6种可能*/
	u8 Treasure_location;		
	/*目标宝物位置（处理后）有3种可能（左：return 1，中：return 2，右 return 3）*/
	u8 Target_Treasure_location;
	/*此次要抓的宝物颜色*/
	u8 Color;					
	/*二维码信息标志位（用于路线规划）*/
	u8 QR_code;					
	/*摄像头模式标志位*/
	u8 Camera_mode;				
	/*屏幕菜单页*/
	u8 Lcd_page;				
	/*存储按键翻页*/
	u8 Key_page;
	/*屏幕模式切换*/
	u8 Lcd;	
	/*机器人位置*/
	u8 Robot_loc;	
	/*宝物数量*/
	u8 Treasure_number;			
};

/* 放置所有设备状态位的结构体 */
struct Sflag_Device
{
	/*车上部红外线避障主开关*/		
	u8 Infrared_up;
	/*车下部左边红外线避障主开关*/	
	u8 Infrared_down_left;
	/*车下部右边红外线避障主开关*/	
	u8 Infrared_down_right;	
	/*车左边色标主开关*/	
	u8 Color_code_left;
	/*车右边色标主开关*/				
	u8 Color_code_right;
	/*车左前边色标主开关*/	
	u8 Color_code_front_left;
	/*车右前边色标主开关*/				
	u8 Color_code_front_right;
};
/* USER CODE END PV */



/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

void Pack_Init(void);//硬件初始化
void __All_Data_Handler(void);
void LCD_Show(void);

/* USER CODE END PFP */
#ifdef __cplusplus
}
#endif	/*__CPLUSPLUS*/

#endif


