#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>

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
    #pragma message "LCD_I2C_ADDRESS is not defined. Setting it to 0x27"
    #define LCD_I2C_ADDRESS 0x27
#endif

/**
 * @brief LCD library handle.
 */
typedef struct lcd_handle_s
{
    uint8_t reserved;
} lcd_handle_t;

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
 * @brief LCD controller commands
 */
typedef enum lcd_commands_e
{
    LCD_CLEAR_DISPLAY     = 0x0001,
    LCD_RETURN_HOME       = 0x0002,
    LCD_ENTRY_MODE_SET    = 0x0004,
    LCD_DISPLAY_ON_OFF    = 0x0008,
    LCD_CURSOR_SHIFT      = 0x0010,
    LCD_FUNCTION_SET      = 0x0020,
    LCD_SET_CGRAM_ADDRESS = 0x0040,
    LCD_SET_DDRAM_ADDRESS = 0x0080,
    LCD_READ_BUSY_FLAG    = 0x0100,
    LCD_WRITE_DATA        = 0x0200,
    LCD_READ_DATA         = 0x0300,
} lcd_commands_t;

typedef enum lcd_command_flags_e
{
    LCD_CMD_FLAG_NO_FLAGS = 0x00,
    /* Entry mode set command flags */
    LCD_CMD_FLAG_DISPLAY_DISABLE_SHIFT = 0x00,
    LCD_CMD_FLAG_DISPLAY_ENABLE_SHIFT  = 0x01,
    LCD_CMD_FLAG_DECREMENT             = 0x00,
    LCD_CMD_FLAG_INCREMENT             = 0x02,
    /* Display On/Off command flags */
    LCD_CMD_FLAG_CURSOR_BLINK_DISABLE    = 0x00,
    LCD_CMD_FLAG_CURSOR_BLINK_ENABLE     = 0x01,
    LCD_CMD_FLAG_CURSOR_DISABLE          = 0x00,
    LCD_CMD_FLAG_CURSOR_ENABLE           = 0x02,
    LCD_CMD_FLAG_DISPLAY_DISABLE         = 0x00,
    LCD_CMD_FLAG_DISPLAY_ENABLE          = 0x04,
    /* Cursor shift command flags */
    LCD_CMD_FLAG_LSHIFT_ENABLE           = 0x00,
    LCD_CMD_FLAG_RSHIFT_ENABLE           = 0x04,
    LCD_CMD_FLAG_CURSOR_MOVE_ENABLE      = 0x00,
    LCD_CMD_FLAG_DISPLAY_SHIFT_ENABLE    = 0x08,
} lcd_command_flags_t;

/**
 * @brief Initializes platform in order to use LCD dislpay
 *
 * @name                          lcd_platform_init
 *
 * @param  lcd_handle             LCD library handle
 *
 * @return LCD_OK on success, error code otherwise
 */
lcd_ret_code_t
lcd_platform_init(lcd_handle_t * lcd_handle);

/**
 * @brief Initializes LCD display in provided mode
 *
 * @name                          lcd_platform_init
 *
 * @param  lcd_handle             LCD library handle
 *
 * @return LCD_OK on success, error code otherwise
 */
lcd_ret_code_t
lcd_initialize_display(lcd_handle_t * lcd_handle);

lcd_ret_code_t
lcd_print_str(const char * str);

#endif