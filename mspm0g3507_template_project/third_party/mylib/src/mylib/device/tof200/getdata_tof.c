#include "string.h"
#include "getdata_tof.h"
#include <stdio.h>

uint8 a_testBuff[20];

uint16 data_1     = 0;
uint16 data_2     = 0;
uint16 deviceAddr = 0;

//读测量距离命令
#define REG_RD_ADDR 0x0010   //测量结果寄存器
void read_cmd_test_main(void)
{
    static uint32 timStamp = 0;
    if (tim_check_timeout(timStamp, tim_get_count(), 500))   // 500ms发送一次读命令
    {
        a_testBuff[0] = 0;                  //广播地址
        a_testBuff[1] = 3;                  //读命令码
        a_testBuff[2] = REG_RD_ADDR >> 8;   // regH
        a_testBuff[3] = REG_RD_ADDR;        // regL
        a_testBuff[4] = 0;                  // numH(仅支持读单个寄存器)
        a_testBuff[5] = 1;                  // numL
        uart_tpm_tx_data(UART_TPM_ONE, a_testBuff, 6);
        timStamp = tim_get_count();
    }
}
#define REG_WR_ADDR 0x0005   //连续输出控制寄存器
#define REG_WR_DATA 1000   //写入数据
void write_cmd_test_main(void)
{
    static uint32 timStamp = 0;
    static uint8  flag     = 1;
    if (tim_check_timeout(timStamp, tim_get_count(), 500))   // 500ms后只发送一次写命令
    {
        if (flag) {
            flag          = 0;
            a_testBuff[0] = 0;                    //广播地址
            a_testBuff[1] = 6;                    //写命令码
            a_testBuff[2] = REG_WR_ADDR >> 8;     // regH
            a_testBuff[3] = REG_WR_ADDR;          // regL
            a_testBuff[4] = REG_WR_DATA >> 8;     // dataH
            a_testBuff[5] = (uint8)REG_WR_DATA;   // dataL
            uart_tpm_tx_data(UART_TPM_ONE, a_testBuff, 6);
        }
        timStamp = tim_get_count();
    }
}
void cmd_test_main(void)
{
    // write_cmd_test_main();//发送写命令测试
    read_cmd_test_main();   //发送读命令测试
}

// modbus接收到正确数据帧回调函数
void modbus_recv_data(uint8* p_data, uint16 len)
{
    uint16 readData = 0;

    /*读命令响应
      deviceAddr = p_data[0];//响应的设备地址
      cmd = p_data[1];      //响应的命令,读03
      byteNum = p_data[2];  //返回读数据长*/

    if (p_data[0] == 1) {
        deviceAddr = p_data[0];   //响应的设备地址
        readData   = p_data[3];   //读回数据H
        readData <<= 8;
        readData |= p_data[4];   //读回数据H
        printf("deviceAddr_left:%u\n", deviceAddr);
        printf("Distance:%umm\n", readData);

        data_1 = readData;
    }
    if (p_data[0] == 2) {
        deviceAddr = p_data[0];   //响应的设备地址
        readData   = p_data[3];   //读回数据H
        readData <<= 8;
        readData |= p_data[4];   //读回数据H
        printf("deviceAddr_right:%u\n", deviceAddr);
        printf("Distance:%umm\n", readData);
        data_2 = readData;
    }





    /*传出测量数据*/
    /*	mcu_disable_irq();
        mcu_init();
        tim_user_init();
        uart_tpm_user_init();
        mcu_enable_irq();
      while(1)
        {
                cmd_test_main();
                uart_tpm_main();
        }
    使用时要用的初始化函数
    */
    uint16 getdata_tof(uint8 channel)
    {
        uint16 data;
        if（channel == 1） data = data_1;
        else data               = data_2;



        return data;
    }
