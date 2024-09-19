/* Simple GPIO Demo
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h> 
#include <app_reset.h>

#include "app_priv.h"

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

/* This is the GPIOs on which the power will be set */
#define OUTPUT_GPIO_RELAY1    CONFIG_EXAMPLE_OUTPUT_GPIO_RELAY1
#define OUTPUT_GPIO_RELAY2    CONFIG_EXAMPLE_OUTPUT_GPIO_RELAY2

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10



void app_driver_init()
{
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    /* Configure your hardware*/
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    uint64_t pin_mask = (((uint64_t) 1 << OUTPUT_GPIO_RELAY1)|((uint64_t) 1 << OUTPUT_GPIO_RELAY2) );
    io_conf.pin_bit_mask = pin_mask;
    /* Configure the GPIO */
    gpio_config(&io_conf);
    gpio_set_level(OUTPUT_GPIO_RELAY1, true);
    gpio_set_level(OUTPUT_GPIO_RELAY2, true);
}


esp_err_t app_driver_set_gpio(const char *name, bool state)
{
    if (strcmp(name, "Relay1") == 0) {
        gpio_set_level(OUTPUT_GPIO_RELAY1, !state);
    } 
    if (strcmp(name, "Relay2") == 0) {
        gpio_set_level(OUTPUT_GPIO_RELAY2, !state);
    } 
    else {
        return ESP_FAIL;
    }
    return ESP_OK;
}
