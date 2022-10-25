#ifndef LED_STRIP_HANDLER_H
#define LED_STRIP_HANDLER_H

#define LCD_STATUS_ON (1 << 0)
//#define LCD_STATUS_OFF (0 << 0)

#include "nvs.h"

typedef union {
   struct {
      union {
         uint8_t r;
         uint8_t red;
      };	
      union {
         uint8_t g;
         uint8_t green;
      };
      union {
         uint8_t b;
         uint8_t blue;
      };
   };
   uint8_t raw [3];
} RGB;

extern uint32_t lcd_current_status;
extern uint32_t strip_num_leds;
extern RGB lcd_current_color;

void initialize_led_strip(void);
void turn_off_led(void);
void turn_on_led(void);
void set_led_color(uint8_t r, uint8_t g, uint8_t b);

#endif