#ifndef WIFI_H
#define WIFI_H

#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "../config/private_config.h"

void WiFi_init();

#endif // WIFI_H