/* Switch demo implementation using button and RGB LED
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h> 
#include <app_reset.h>
#include <esp_log.h> 

#include "app_priv.h"

/* This is the button that is used for reset WiFi and Factory */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

/* Sesnor INPUT GPIO */
#define SENSOR_GPIO          CONFIG_EXAMPLE_SENSOR_GPIO
#define SENSOR_ACTIVE_LEVEL  0

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

extern esp_rmaker_device_t *sensor_device;
static bool last_state = false;

static const char *TAG = "app_drive";

static void sensor_push_cb(void *arg)
{
    ESP_LOGI(TAG,"Sensor PUSH Call Back");  
       
    if(last_state == 0)
    {
		last_state = 1;
		esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_name(sensor_device, "Motion State"),
                esp_rmaker_bool(last_state));
        esp_rmaker_raise_alert("Alguém entrou na sala");
	}
    
}

static void sensor_release_cb(void *arg)
{
    ESP_LOGI(TAG,"Sensor Release Call Back");   
       
    if(last_state == 1)
    {
		last_state = 0;
		esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_name(sensor_device, "Motion State"),
                esp_rmaker_bool(last_state));
        esp_rmaker_raise_alert("Saiu da sala");
	}
}


void app_driver_init()
{
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }
    
    button_handle_t sensor_handle = iot_button_create(SENSOR_GPIO, SENSOR_ACTIVE_LEVEL);
    if (sensor_handle) {
        /* Register a callback for a push event */
        //iot_button_set_evt_cb(sensor_handle, BUTTON_CB_PUSH, sensor_push_cb, NULL);  
        iot_button_add_on_press_cb(sensor_handle,5,sensor_push_cb,NULL);
        /* Register a callback for a release event */
       // iot_button_set_evt_cb(sensor_handle, BUTTON_CB_RELEASE, sensor_release_cb, NULL);
        iot_button_add_on_release_cb(sensor_handle,5,sensor_release_cb,NULL);  
    }

    
}


