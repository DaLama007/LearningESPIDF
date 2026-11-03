#include <stdio.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_st7789.h"
static esp_lcd_panel_handle_t panel_handle = NULL;

// define all the pins
#define LCD_HOST SPI2_HOST
#define PIN_NUM_SCLK 18
#define PIN_NUM_MISO 17
#define PIN_NUM_MOSI 11
#define LCD_H_RES 240

#define PIN_NUM_LCD_DC 16
#define PIN_NUM_LCD_CS 10
#define LCD_PIXEL_CLOCK_HZ (10 * 1000 * 1000)
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8
#define PIN_NUM_RST 15
// #define LCD_RGB_ELEMENT_ORDER_BGR 
static void config_spi(void){
  // create SPI bus
  spi_bus_config_t buscfg = {
    .sclk_io_num = PIN_NUM_SCLK,
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = PIN_NUM_MISO,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1, 
    .max_transfer_sz = LCD_H_RES * 80 * sizeof(uint16_t), // transfer 80 lines of pixels (assume pixel is RGB565) at most in one SPI transaction
  };
  esp_lcd_panel_io_spi_config_t io_config = {
    .dc_gpio_num = PIN_NUM_LCD_DC,
    .cs_gpio_num = PIN_NUM_LCD_CS,
    .pclk_hz = LCD_PIXEL_CLOCK_HZ,
    .lcd_cmd_bits = LCD_CMD_BITS,
    .lcd_param_bits = LCD_PARAM_BITS,
    .spi_mode = 0,
    .trans_queue_depth = 10,
  };
  esp_lcd_panel_io_handle_t io_handle = NULL;
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO)); // Enable the DMA feature
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));


  // Attach the LCD to the SPI bus
  esp_lcd_panel_dev_config_t panel_config = {
    .reset_gpio_num = PIN_NUM_RST,
    // .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
    .bits_per_pixel = 16,
  };
  // Create LCD panel handle for ST7789, with the SPI IO device handle
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
}


void app_main(void)
{
  config_spi();
  esp_lcd_panel_reset(panel_handle);
  esp_lcd_panel_init(panel_handle);
  esp_lcd_panel_disp_on_off(panel_handle, true);
  while(1){
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}
