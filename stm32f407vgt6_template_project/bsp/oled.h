#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "oled_font.h"

#define OLED_FONT_6X8 12
#define OLED_FONT_8X16 16

void oled_write_command(uint8_t cmd);
void oled_write_data(uint8_t data);
void oled_init(void);
void oled_clear(void);
void oled_display_on(void);
void oled_display_off(void);
void oled_set_pos(uint8_t x, uint8_t y);
void oled_update(void);
void oled_show_num(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2, uint8_t Color_Turn);
void oled_show_decimal(uint8_t x, uint8_t y, float num, uint8_t z_len, uint8_t f_len, uint8_t size2,
                       uint8_t Color_Turn);
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, uint8_t Color_Turn);
void oled_show_string(uint8_t x, uint8_t y, char* chr, uint8_t Char_Size, uint8_t Color_Turn);
void oled_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t* BMP, uint8_t Color_Turn);

// 
void OLED_HorizontalShift(uint8_t direction);
void OLED_Some_HorizontalShift(uint8_t direction, uint8_t start, uint8_t end);
void OLED_VerticalAndHorizontalShift(uint8_t direction);
void OLED_DisplayMode(uint8_t mode);
void OLED_IntensityControl(uint8_t intensity);


#endif
