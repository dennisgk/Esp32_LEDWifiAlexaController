#include "led-strip-handler.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#include "nvs.h"

static const char *TAG = "led_strip";
#define BLINK_GPIO 5
static led_strip_handle_t led_strip;

uint32_t lcd_current_status = 0x00000000;
RGB lcd_current_color = {0};
uint32_t strip_num_leds = 300;


void turn_off_led(void){
    if((lcd_current_status & LCD_STATUS_ON) == 0){
        return;
    }

    for(uint32_t i = 0; i < strip_num_leds; i++){
        led_strip_set_pixel(led_strip, i, 0, 0, 0);
    }

    led_strip_refresh(led_strip);
    lcd_current_status &= ~(LCD_STATUS_ON);
}

void turn_on_led(void){
    if((lcd_current_status & LCD_STATUS_ON) != 0){
        return;
    }

    for(uint32_t i = 0; i < strip_num_leds; i++){
        led_strip_set_pixel(led_strip, i, (uint32_t)lcd_current_color.raw[0], (uint32_t)lcd_current_color.raw[1], (uint32_t)lcd_current_color.raw[2]);
    }

    led_strip_refresh(led_strip);
    lcd_current_status |= LCD_STATUS_ON;
}

void set_led_color(uint8_t r, uint8_t g, uint8_t b){

    if(lcd_current_color.raw[0] == r && lcd_current_color.raw[1] == g && lcd_current_color.raw[2] == b){
        return;
    }

    lcd_current_color.raw[0] = r;
    lcd_current_color.raw[1] = g;
    lcd_current_color.raw[2] = b;

    if((lcd_current_status & LCD_STATUS_ON) == 0){
        return;
    }

    for(uint32_t i = 0; i < strip_num_leds; i++){
        led_strip_set_pixel(led_strip, i, (uint32_t)r, (uint32_t)g, (uint32_t)b);
    }

    led_strip_refresh(led_strip);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = strip_num_leds, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

void initialize_led_strip(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();

    vTaskDelay(100 / portTICK_PERIOD_MS);

    lcd_current_color.r = 220;
    lcd_current_color.g = 220;
    lcd_current_color.b = 220;
    turn_on_led();

}