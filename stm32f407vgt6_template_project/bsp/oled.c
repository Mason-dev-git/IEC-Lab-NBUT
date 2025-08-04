#include "bsp.h"
#include "time_util.h"
#include "oled.h"

#define OLED_I2C_ADDRESS 0x78
#define OLED_I2C_TIMEOUT 0x100

/////// ��Ҫ��ֲ�Ĳ���

extern I2C_HandleTypeDef hi2c1;

/**
 * @function: void OLED_WR_CMD(uint8_t cmd)
 * @description: ���豸д��������
 * @param {uint8_t} cmd оƬ�ֲ�涨������
 * @return {*}
 */
static inline void oled_write_command(uint8_t cmd)
{
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, OLED_I2C_TIMEOUT);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: ���豸д��������
 * @param {uint8_t} data ����
 * @return {*}
 */
static inline void oled_write_data(uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, OLED_I2C_TIMEOUT);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**********************************************************
 * ��ʼ������,����оƬ�ֲ���д����ϸ�������ͼ�Լ�ע������
 ***********************************************************/
uint8_t g_init_cmds[] = {0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xA1, 0xC8, 0xDA, 0x12,
                         0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0x8D, 0x14, 0xAF};



/**
 * @function: void oled_init(void)
 * @description: OLED��ʼ��
 * @return {*}
 */
void oled_init(void)
{
    delay_ms(200);

    uint8_t i = 0;
    for (i = 0; i < 23; i++) {
        oled_write_command(g_init_cmds[i]);
    }
}

/**
 * @function: void oled_update(void)
 * @description: ������ʾ

 * @return {*}
 */
void oled_update(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        oled_write_command(0xb0 + i);   // ����ҳ��ַ��0~7��
        oled_write_command(0x00);       // ������ʾλ�á��е͵�ַ
        oled_write_command(0x10);       // ������ʾλ�á��иߵ�ַ
        for (n = 0; n < 128; n++)
            oled_write_data(1);
    }
}


/**
 * @function: oled_clear(void)
 * @description: ����,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @return {*}
 */
void oled_clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        oled_write_command(0xb0 + i);   // ����ҳ��ַ��0~7��
        oled_write_command(0x00);       // ������ʾλ�á��е͵�ַ
        oled_write_command(0x10);       // ������ʾλ�á��иߵ�ַ
        for (n = 0; n < 128; n++)
            oled_write_data(0);
    }
}

/**
 * @function: void oled_display_on(void)
 * @description: ����OLED��ʾ
 * @return {*}
 */
void oled_display_on(void)
{
    oled_write_command(0X8D);   // SET DCDC����
    oled_write_command(0X14);   // DCDC ON
    oled_write_command(0XAF);   // DISPLAY ON,����ʾ
}


/**
 * @function: void oled_display_off(void)
 * @description: �ر�OLED��ʾ
 * @return {*}
 */
void oled_display_off(void)
{
    oled_write_command(0X8D);   // SET DCDC����
    oled_write_command(0X10);   // DCDC OFF
    oled_write_command(0XAE);   // DISPLAY OFF���ر���ʾ
}

/**
 * @function: void oled_set_pos(uint8_t x, uint8_t y)
 * @description: ��������
 * @param {uint8_t} x,y
 * @return {*}
 */
void oled_set_pos(uint8_t x, uint8_t y)
{
    oled_write_command(0xb0 + y);                   // ����ҳ��ַ��0~7��
    oled_write_command(((x & 0xf0) >> 4) | 0x10);   // ������ʾλ�á��иߵ�ַ
    oled_write_command(x & 0x0f);                   // ������ʾλ�á��е͵�ַ
}


/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n����
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m, uint8_t n)
{
    unsigned int result = 1;
    while (n--)
        result *= m;
    return result;
}

/**
 * @function: void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾһ���ַ�
 * @param {uint8_t} x�ַ���ʼ��ʾ�ĺ�����
 * @param {uint8_t} y�ַ���ʼ��ʾ��������
 * @param {uint8_t} chr����ʾ���ַ�
 * @param {uint8_t} Char_Size����ʾ�ַ��������С,ѡ������ 16/12
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, uint8_t Color_Turn)
{
    unsigned char c = 0, i = 0;
    c = chr - ' ';   // �õ�ƫ�ƺ��ֵ
    if (x > 128 - 1) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        oled_set_pos(x, y);
        for (i = 0; i < 8; i++) {
            if (Color_Turn)
                oled_write_data(~F8X16[c * 16 + i]);
            else
                oled_write_data(F8X16[c * 16 + i]);
        }
        oled_set_pos(x, y + 1);
        for (i = 0; i < 8; i++) {
            if (Color_Turn)
                oled_write_data(~F8X16[c * 16 + i + 8]);
            else
                oled_write_data(F8X16[c * 16 + i + 8]);
        }
    }
    else {
        oled_set_pos(x, y);
        for (i = 0; i < 6; i++) {
            if (Color_Turn)
                oled_write_data(~F6x8[c][i]);
            else
                oled_write_data(F6x8[c][i]);
        }
    }
}

/**
 * @function: void oled_show_string(uint8_t x, uint8_t y, uint8_t *chr, uint8_tChar_Size, uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾ�ַ���
 * @param {uint8_t} x����ʾ�ַ����Ŀ�ʼ������x:0~127
 * @param {uint8_t} y����ʾ�ַ����Ŀ�ʼ������
 * y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {uint8_t} *chr����ʾ���ַ���
 * @param {uint8_t} Char_Size����ʾ�ַ����������С,ѡ������ 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void oled_show_string(uint8_t x, uint8_t y, char* chr, uint8_t Char_Size, uint8_t Color_Turn)
{
    uint8_t j = 0;
    while (chr[j] != '\0') {
        oled_show_char(x, y, chr[j], Char_Size, Color_Turn);
        if (Char_Size == OLED_FONT_6X8)   // 6X8�������м�6����ʾ��һ���ַ�
            x += 6;
        else   // 8X16�������м�8����ʾ��һ���ַ�
            x += 8;

        if (x > 122 && Char_Size == 12)   // TextSize6x8���һ�в�����ʾ�ˣ�����һ�м�����ʾ
        {
            x = 0;
            y++;
        }
        if (x > 120 && Char_Size == 16)   // TextSize8x16���һ�в�����ʾ�ˣ�����һ�м�����ʾ
        {
            x = 0;
            y++;
        }
        j++;
    }
}

/**
 * @function: void oled_show_num(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: ��ʾ����
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������,
 * y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {unsigned int} num:���������
 * @param {uint8_t } len:���������λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void oled_show_num(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2, uint8_t Color_Turn)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                oled_show_char(x + (size2 / 2) * t, y, ' ', size2, Color_Turn);
                continue;
            }
            else
                enshow = 1;
        }
        oled_show_char(x + (size2 / 2) * t, y, temp + '0', size2, Color_Turn);
    }
}


/**
 * @function: void oled_show_decimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t
 * Color_Turn)
 * @description: ��ʾ����������
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������,
 * y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {float} num:����ĸ���������
 * @param {uint8_t } z_ len:�������ֵ�λ��
 * @param {uint8_t } f_len: С�����ֵ�λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void oled_show_decimal(uint8_t x, uint8_t y, float num, uint8_t z_len, uint8_t f_len, uint8_t size2, uint8_t Color_Turn)
{
    uint8_t t, temp, i = 0;   // iΪ������־λ
    uint8_t enshow;
    int z_temp, f_temp;
    if (num < 0) {
        z_len += 1;
        i = 1;
        num = -num;
    }
    z_temp = (int)num;
    // ��������
    for (t = 0; t < z_len; t++) {
        temp = (z_temp / oled_pow(10, z_len - t - 1)) % 10;
        if (enshow == 0 && t < (z_len - 1)) {
            if (temp == 0) {
                oled_show_char(x + (size2 / 2) * t, y, ' ', size2, Color_Turn);
                continue;
            }
            else
                enshow = 1;
        }
        oled_show_char(x + (size2 / 2) * t, y, temp + '0', size2, Color_Turn);
    }
    // С����
    oled_show_char(x + (size2 / 2) * (z_len), y, '.', size2, Color_Turn);

    f_temp = (int)((num - z_temp) * (oled_pow(10, f_len)));
    // С������
    for (t = 0; t < f_len; t++) {
        temp = (f_temp / oled_pow(10, f_len - t - 1)) % 10;
        oled_show_char(x + (size2 / 2) * (t + z_len) + 5, y, temp + '0', size2, Color_Turn);
    }
    if (i == 1)   // ���Ϊ�����ͽ���ǰ��һλ��ֵ��-��
    {
        oled_show_char(x, y, '-', size2, Color_Turn);
        i = 0;
    }
}

/**
 * @function: void oled_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: ��OLED�ض�������ʾBMPͼƬ
 * @param {uint8_t} x0ͼ��ʼ��ʾ������  x0:0~127
 * @param {uint8_t} y0ͼ��ʼ��ʾ������  y0:0~7
 * @param {uint8_t} x1ͼ�������ʾ������  x1:1~128
 * @param {uint8_t} y1ͼ�������ʾ������  y1:1~8
 * @param {uint8_t} *BMP����ʾ��ͼ������
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void oled_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t* BMP, uint8_t Color_Turn)
{
    uint32_t j = 0;
    uint8_t x = 0, y = 0;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        oled_set_pos(x0, y);
        for (x = x0; x < x1; x++) {
            if (Color_Turn)
                oled_write_data(~BMP[j++]);   // ��ʾ����ͼƬ
            else
                oled_write_data(BMP[j++]);   // ��ʾͼƬ
        }
    }
}


/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: ��Ļ����ˮƽȫ����������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)
{
    oled_write_command(0x2e);        // ֹͣ����
    oled_write_command(direction);   // ���ù�������
    oled_write_command(0x00);        // �����ֽ����ã�Ĭ��Ϊ0x00
    oled_write_command(0x00);        // ���ÿ�ʼҳ��ַ
    oled_write_command(0x07);        // ����ÿ����������֮���ʱ������֡Ƶ
    //  0x00-5֡�� 0x01-64֡�� 0x02-128֡�� 0x03-256֡�� 0x04-3֡�� 0x05-4֡�� 0x06-25֡�� 0x07-2֡��
    oled_write_command(0x07);   // ���ý���ҳ��ַ
    oled_write_command(0x00);   // �����ֽ����ã�Ĭ��Ϊ0x00
    oled_write_command(0xff);   // �����ֽ����ã�Ĭ��Ϊ0xff
    oled_write_command(0x2f);   // ��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: ��Ļ��������ˮƽ��������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start ��ʼҳ��ַ  0x00~0x07
 * @param {uint8_t} end  ����ҳ��ַ  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction, uint8_t start, uint8_t end)
{
    oled_write_command(0x2e);        // ֹͣ����
    oled_write_command(direction);   // ���ù�������
    oled_write_command(0x00);        // �����ֽ����ã�Ĭ��Ϊ0x00
    oled_write_command(start);       // ���ÿ�ʼҳ��ַ
    oled_write_command(0x07);        // ����ÿ����������֮���ʱ������֡Ƶ,0x07�������ٶ�2֡
    oled_write_command(end);         // ���ý���ҳ��ַ
    oled_write_command(0x00);        // �����ֽ����ã�Ĭ��Ϊ0x00
    oled_write_command(0xff);        // �����ֽ����ã�Ĭ��Ϊ0xff
    oled_write_command(0x2f);        // ��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: ��Ļ���ݴ�ֱˮƽȫ����������
 * @param {uint8_t} direction				���Ϲ���	 0x29
 *                                                            ���Ϲ���   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
    oled_write_command(0x2e);        // ֹͣ����
    oled_write_command(direction);   // ���ù�������
    oled_write_command(0x01);        // �����ֽ�����
    oled_write_command(0x00);        // ���ÿ�ʼҳ��ַ
    oled_write_command(0x07);        // ����ÿ����������֮���ʱ������֡Ƶ���������ٶ�
    oled_write_command(0x07);        // ���ý���ҳ��ַ
    oled_write_command(0x01);        // ��ֱ����ƫ����
    oled_write_command(0x00);        // �����ֽ����ã�Ĭ��Ϊ0x00
    oled_write_command(0xff);        // �����ֽ����ã�Ĭ��Ϊ0xff
    oled_write_command(0x2f);        // ��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: ��Ļ����ȡ����ʾ
 * @param {uint8_t} direction			ON	0xA7  ��
 *                                                          OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode)
{
    oled_write_command(mode);
}

/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: ��Ļ���ȵ���
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity)
{
    oled_write_command(0x81);
    oled_write_command(intensity);
}
