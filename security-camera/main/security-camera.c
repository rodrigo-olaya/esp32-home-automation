#include <stdio.h>
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include <inttypes.h>
#include <portmacro.h>
#include "esp_mac.h"
#include "WiFi/WiFi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "camera/camera.h"
#include "MQTT/publisher.h"

void app_main(void)
{
    WiFi_init();

    mqtt_init();

    camera_init();

    vTaskDelay(pdMS_TO_TICKS(2000));
    
    while(1) {
        camera_capture();
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
