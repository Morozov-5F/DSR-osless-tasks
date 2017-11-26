#include "lcd.h"
#include "stm32f4xx.h"

#define LCD_I2C_FREQUENCY 100000 /* 100 kHz */

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

struct lcd_handle_s
{
    uint8_t reserved;
};

/**
 * @brief Initializes GPIO for defined I2C port
 */
static void
s_initialize_gpio()
{
    GPIO_InitTypeDef gpio_init;
    GPIO_StructInit(&gpio_init);
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
    /* Enable GPIO clocking */
    RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK, ENABLE);

    return;
}

/**
 * @brief Initializes defined I2C port
 */
static void s_initialize_i2c()
{
    I2C_InitTypeDef i2c_init;
    GPIO_StructInit(&i2c_init);
    /* Initialize I2C itself */
    i2c_init.I2C_ClockSpeed          = LCD_I2C_FREQUENCY; /* 400 kHz */
    i2c_init.I2C_Mode                = I2C_Mode_I2C;
    i2c_init.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2c_init.I2C_Ack                 = I2C_Ack_Disable;
    i2c_init.I2C_OwnAddress1         = LCD_OWN_ADDRESS;
    i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C_HANDLE, &i2c_init);
    /* Enable I2C clocking */
    RCC_APB1PeriphClockCmd(I2C_RCC_CLK, ENABLE);
    /* Enable I2C port */
    I2C_Cmd(I2C_HANDLE, &i2c_init);

    return;
}

lcd_ret_code_t lcd_platform_init(lcd_handle_t * lcd_handle))
{
    lcd_ret_code_t ret = LCD_OK;

    s_initialize_gpio();
    s_initialize_i2c();

    return LCD_OK;
}