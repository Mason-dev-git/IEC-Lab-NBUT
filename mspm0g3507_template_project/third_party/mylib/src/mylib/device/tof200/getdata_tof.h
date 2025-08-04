
#ifndef __GETDATA_TOF_H__
#define __GETDATA_TOF_H__

#include "mcu_hal_user.h"
#include "timer_user.h"
#include "uart_tpm_user.h"

extern uint16 data_1;

void read_cmd_test_main(void);
void write_cmd_test_main(void);
void cmd_test_main(void);

void modbus_recv_data_1(uint8* p_data, uint16 len);
void modbus_recv_data_2(uint8* p_data, uint16 len);

uint16 getdata_tof(uint8 channel);


#endif   //__GETDATA_TOF_H__
