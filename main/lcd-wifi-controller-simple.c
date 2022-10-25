#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"

#include "http-server-handler.h"
#include "led-strip-handler.h"
#include "wifi-connection-handler.h"

void initialize_reqs(void){
	//initialize storage
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

	//initialize default esp event loop
	ESP_ERROR_CHECK(esp_event_loop_create_default());
}

void app_main(void)
{
	//initialize requirements
    initialize_reqs();

    //initialize led strip
    initialize_led_strip();

    //connect the wifi
    connect_wifi();

    //start the webserver
    httpd_handle_t http_server = start_webserver();
}
