#include <stm32f4xx.h>
#include "i2c_wrapper.h"

#include "lcd.h"

#define LCD_I2C_FREQUENCY 400000 /* 100 kHz */

#if   LCD_I2C_PORT == 1 /* I2C1 */
    #error "I2C1 is not supported yet. Please use I2C2 for now"
#elif LCD_I2C_PORT == 2 /* I2C2 */
    #define I2C_HANDLE            I2C2
    #define I2C_RCC_CLK           RCC_APB1Periph_I2C2
    #define I2C_GPIO_PORT         GPIOB
    #define I2C_GPIO_CLK          RCC_AHB1Periph_GPIOB
    #define I2C_GPIO_PIN_SCL      GPIO_Pin_10
    #define I2C_GPIO_PIN_SDA      GPIO_Pin_11
    #define I2C_GPIO_AF_CONF      GPIO_AF_I2C2
    #define I2C_GPIO_PIN_SRC_SCL  GPIO_PinSource10
    #define I2C_GPIO_PIN_SRC_SDA  GPIO_PinSource11
#elif LCD_I2C_PORT == 3 /* I2C3 */
    #error "I2C1 is not supported yet. Please use I2C3 for now"
#else
#error "Unsupported I2C value"
#endif

typedef struct lcd_packet_s
{
    uint8_t lcdp_rs  : 1;
    uint8_t lcdp_rw  : 1;
    uint8_t lcdp_e   : 1;
    uint8_t lcdp_led : 1;
    uint8_t lcdp_d4  : 1;
    uint8_t lcdp_d5  : 1;
    uint8_t lcdp_d6  : 1;
    uint8_t lcdp_d7  : 1;
} lcd_packet_t;

static uint32_t gs_delay_multiplyer = 0;

static void
s_delay_us(uint32_t us)
{
    us = us * gs_delay_multiplyer - 10;
    while (us--);
}

static void
s_delay_ms(uint32_t ms)
{
    ms = 1000 * ms * gs_delay_multiplyer - 10;
    /* 4 cycles for one loop */
    while (ms--);
}

static void
s_reset_lcd_packet(lcd_packet_t * packet)
{
    uint8_t reset_packet = 0;
    /* Some pointers magic here */
    *packet = *((lcd_packet_t *)&reset_packet);
}

/**
 * @brief Initializes GPIO for defined I2C port
 */
static void
s_initialize_gpio(void)
{
    GPIO_InitTypeDef gpio_init;
    GPIO_StructInit(&gpio_init);
    /* Enable GPIO clocking */
    RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK, ENABLE);
    /* Initialize GPIO itself */
    gpio_init.GPIO_Pin   = I2C_GPIO_PIN_SCL | I2C_GPIO_PIN_SDA;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_OD;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &gpio_init);
    /* Initialize AF for I2C pins */
    GPIO_PinAFConfig(I2C_GPIO_PORT, I2C_GPIO_PIN_SRC_SCL, I2C_GPIO_AF_CONF);
    GPIO_PinAFConfig(I2C_GPIO_PORT, I2C_GPIO_PIN_SRC_SDA, I2C_GPIO_AF_CONF);

    return;
}

static void
s_initialize_delay(void)
{
    RCC_ClocksTypeDef rcc_clocks;

    /* Get Clocks frequencies */
    RCC_GetClocksFreq(&rcc_clocks);

    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M */
    gs_delay_multiplyer = rcc_clocks.HCLK_Frequency / 4000000;

    return;
}

/**
 * @brief Transmit one byte
 */
static void
s_lcd_transmit_byte_command(lcd_packet_t * packet)
{
    uint8_t * packet_payload = (uint8_t *)packet;

    /* Step 1: send packet with e-bit set to HIGH */
    /* Send signal of starting transmission */
    packet->lcdp_led = 1;
    packet->lcdp_e = 1;
    I2C_write_byte(I2C_HANDLE, LCD_I2C_ADDRESS, *packet_payload);
    /* Wait a bit */
    s_delay_us(1000);
    /* Step 2: send packet with e-bit set to LOW */
    packet->lcdp_e = 0;
    I2C_write_byte(I2C_HANDLE, LCD_I2C_ADDRESS, *packet_payload);

    return;
}

static void
s_lcd_send_command(lcd_commands_t lcd_command, uint8_t flags)
{
    uint16_t cmd = (uint16_t)lcd_command | flags;
    lcd_packet_t packet;

    s_reset_lcd_packet(&packet);
    /* Set RS and RW command values */
    packet.lcdp_rw = (cmd >> 8) & 0x01;
    packet.lcdp_rs = (cmd >> 9) & 0x01;
    /* Set remaining values for the first part */
    packet.lcdp_d7 = (cmd >> 7) & 0x01;
    packet.lcdp_d6 = (cmd >> 6) & 0x01;
    packet.lcdp_d5 = (cmd >> 5) & 0x01;
    packet.lcdp_d4 = (cmd >> 4) & 0x01;
    /* Send first part of the packet */
    s_lcd_transmit_byte_command(&packet);
    /* Set values for the second part */
    packet.lcdp_d7 = (cmd >> 3) & 0x01;
    packet.lcdp_d6 = (cmd >> 2) & 0x01;
    packet.lcdp_d5 = (cmd >> 1) & 0x01;
    packet.lcdp_d4 =  cmd & 0x01;
    /* Send the second part of the packet */
    s_lcd_transmit_byte_command(&packet);
}

static void
s_lcd_enable_four_bits_mode(void)
{
    lcd_packet_t packet;
    /* Reset LCD packet */
    s_reset_lcd_packet(&packet);
    /* Send "Function Set" command to enable 4-bits addressing only */
    packet.lcdp_d5 = 1;
    packet.lcdp_d4 = 1;
    s_lcd_transmit_byte_command(&packet);
    /* Sleep for more than 5 ms after that */
    s_delay_ms(6);
    /* Send again */
    s_lcd_transmit_byte_command(&packet);
    /* Sleep for more that 100 us */
    s_delay_us(110);
    /* Send it again! */
    s_lcd_transmit_byte_command(&packet);
    /* And sleep again! */
    s_delay_us(110);
    /* And send it for the last time */
    /* Some weird stuff is going on. Only in that way display initializes */
    packet.lcdp_d4 = 0;
    s_lcd_transmit_byte_command(&packet);
    s_delay_us(110);
}
/**
 * @brief Initializes defined I2C port
 */
static void s_initialize_i2c(void)
{
    I2C_InitTypeDef i2c_init;
    I2C_StructInit(&i2c_init);
    /* Enable I2C clocking */
    RCC_APB1PeriphClockCmd(I2C_RCC_CLK, ENABLE);
    /* Initialize I2C itself */
    i2c_init.I2C_ClockSpeed          = LCD_I2C_FREQUENCY; /* 400 kHz */
    i2c_init.I2C_Mode                = I2C_Mode_I2C;
    i2c_init.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2c_init.I2C_Ack                 = I2C_Ack_Disable;
    i2c_init.I2C_OwnAddress1         = LCD_OWN_ADDRESS;
    i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C_HANDLE, &i2c_init);
    /* Enable I2C port */
    I2C_Cmd(I2C_HANDLE, ENABLE);

    return;
}

lcd_ret_code_t lcd_platform_init(lcd_handle_t * lcd_handle)
{
    lcd_ret_code_t ret = LCD_OK;

    s_initialize_delay();

    s_initialize_gpio();
    s_initialize_i2c();

    return ret;
}

lcd_ret_code_t lcd_initialize_display(lcd_handle_t *lcd_handle)
{
    /* Step 0: Set all the bits to zero */
    I2C_write_byte(I2C_HANDLE, LCD_I2C_ADDRESS, 0x00);
    /* Step 1: Enable 4-bit mode */
    s_lcd_enable_four_bits_mode();
    /* Step 2: Send "Clear display" command */
    s_lcd_send_command(LCD_CLEAR_DISPLAY, LCD_CMD_FLAG_NO_FLAGS);
    s_delay_ms(20);
    /* Step 3: Send "Entry Mode Set" command */
    s_lcd_send_command(LCD_ENTRY_MODE_SET, LCD_CMD_FLAG_INCREMENT | LCD_CMD_FLAG_DISPLAY_DISABLE_SHIFT);
    s_delay_us(50);
    /* Step 4: initialize display */
    s_lcd_send_command(LCD_DISPLAY_ON_OFF, LCD_CMD_FLAG_DISPLAY_ENABLE | LCD_CMD_FLAG_CURSOR_ENABLE | LCD_CMD_FLAG_CURSOR_BLINK_ENABLE);
    s_delay_us(50);
    return LCD_OK;
}

lcd_ret_code_t
lcd_print_str(const char * str)
{
    uint16_t i = 0;
    uint8_t line_offsets[] = { 0x0, 0x40, 0x14, 0x54 };

    for (i = 0; str[i] != '\0' && i < 80; ++i)
    {
        if (i % 20 == 0)
        {
            s_lcd_send_command(LCD_SET_DDRAM_ADDRESS, line_offsets[i / 20]);
            s_delay_us(50);
        }
        s_lcd_send_command(LCD_WRITE_DATA, (uint8_t)str[i]);
        s_delay_us(50);
    }
}