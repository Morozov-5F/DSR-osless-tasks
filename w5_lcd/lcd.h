#ifndef _LCD_H_
#define _LCD_H_

/**
 * Define proper I2C port to use (1, 2 or 3)
 * This option directly affects GPIO Pins to use
 */
#ifndef LCD_I2C_PORT
    #pragma message "LCD_I2C_PORT is not defined. Setting it to I2C2"
    #define LCD_I2C_PORT 2
#endif

/**
 * Define board I2C own address
 */
#ifndef LCD_OWN_ADDRESS
    #pragma message "LCD_OWN_ADDRESS is not defined. Setting it to 0xFE"
    #define LCD_OWN_ADDRESS 0xFE
#endif

/**
 * Define LCD I2C address
 */
#ifndef LCD_I2C_ADDRESS
    #pragma message "LCD_I2C_ADDRESS is not defined. Setting it to 0x07"
    #define LCD_I2C_ADDRESS 0x07
#endif

/**
 * @brief LCD library handle.
 */
typedef struct lcd_handle_s lcd_handle_t;

/**
 * @brief alias for lcd mode bitmask
 */
typedef uint32_t lcd_mode_t;

/**
 * @brief Library error codes
 */
typedef enum lcd_ret_code_e {
    LCD_OK = 0,
    LCD_ERROR = 1,
} lcd_ret_code_t;

/**
 * @brief Initializes platform in order to use LCD dislpay
 *
 * @name                          lcd_platform_init
 *
 * @param  lcd_handle             LCD library handle
 *
 * @return LCD_OK on success, error code otherwise
 */
lcd_ret_code_t lcd_platform_init(lcd_handle_t * lcd_handle);

/**
 * @brief Initializes LCD display in provided mode
 *
 * @name                          lcd_platform_init
 *
 * @param  lcd_handle             LCD library handle
 *
 * @return LCD_OK on success, error code otherwise
 */
lcd_ret_code_t lcd_initialize_display(lcd_handle_t * lcd_handle);

#endif