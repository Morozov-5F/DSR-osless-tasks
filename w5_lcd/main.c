#include "main.h"

typedef struct led_packet_s
{
    uint8_t pin_d4  : 1;
    uint8_t pin_d5  : 1;
    uint8_t pin_d6  : 1;
    uint8_t pin_d7  : 1;
    uint8_t pin_rs  : 1;
    uint8_t pin_rw  : 1;
    uint8_t pin_e   : 1;
    uint8_t pin_led : 1;
} led_packet_t;

static void
s_reset_packet(led_packet_t * packet)
{
    packet->pin_d4  = 0;
    packet->pin_d5  = 0;
    packet->pin_d6  = 0;
    packet->pin_d7  = 0;
    packet->pin_rs  = 0;
    packet->pin_rw  = 0;
    packet->pin_e   = 0;
    packet->pin_led = 0;

    return;
}


#include "lcd.h"

int main(void)
{
    lcd_handle_t handle;

    lcd_platform_init(&handle);
    lcd_initialize_display(&handle);
    lcd_print_str("TEST");
    for (;;);
}
