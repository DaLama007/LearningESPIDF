#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define BLINK_LED 48
#define CONFIG_BLINK_PERIOD 1000 

static uint8_t s_led_state = 0;
static const char* TAG = "blink";
//define the variable for the configered led
static led_strip_handle_t led_strip;

static void blink_led(void){
  if(s_led_state){
    led_strip_set_pixel(led_strip, 0, 16, 16, 16);
    led_strip_refresh(led_strip);
  } else {
    led_strip_clear(led_strip);
  }
}
static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_LED,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
  ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
  led_strip_clear(led_strip);
}
void app_main(void)
{
  char *ourTaskName = pcTaskGetName(NULL);
  ESP_LOGI(ourTaskName, "Hello, starting up\n");
  
  configure_led();

  while(1){

    blink_led();
    s_led_state = !s_led_state;
    vTaskDelay(pdMS_TO_TICKS(CONFIG_BLINK_PERIOD));
  }
}
