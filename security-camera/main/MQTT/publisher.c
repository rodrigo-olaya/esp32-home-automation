#include "publisher.h"

static const char* TAG = "publisher";

static bool mqtt_initialized = false;
static esp_mqtt_client_handle_t mqtt_client = NULL;

void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Event connected");
            mqtt_initialized = true;
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT event disconnected");
            mqtt_initialized = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT event subscribed");
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT event unsubscribed");
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT event published");
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s\r\n", event->data_len, event->data);

            char topic[event->topic_len + 1];
            memcpy(topic, event->topic, event->topic_len);
            topic[event->topic_len] = '\0';
            ESP_LOGI(TAG, "HANDLED TOPIC=%s", topic);

            if (strcmp(topic, "home/esp32/controls/camera") == 0){
                ESP_LOGI(TAG, "Calling config function");
                turn_camera_on_or_off(event);
            }
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT event error");
            break;

        default:
            ESP_LOGI(TAG, "Default case");
            break;
    }
}

void mqtt_init() {
    if (mqtt_initialized) {
        ESP_LOGI(TAG, "MQTT already initialized");
        return;
    }

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGI(TAG, "Could not create client");
        return;
    }

    ESP_LOGI(TAG, "Created event");

    esp_err_t register_ret = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, event_handler, NULL);
    if (register_ret != ESP_OK){
        ESP_LOGE(TAG, "Could not register");
        return;
    }
    ESP_LOGI(TAG, "Event registered");

    esp_err_t client_start_ret = esp_mqtt_client_start(mqtt_client);
    if (client_start_ret != ESP_OK) {
        ESP_LOGE(TAG, "Could not start client");
        esp_mqtt_client_destroy(mqtt_client);
        return;
    }
    ESP_LOGI(TAG, "Event started");
}

void publish_data( camera_fb_t *fb) {
    ESP_LOGI(TAG, "Attempting to publish");
    vTaskDelay(pdMS_TO_TICKS(100));

    const char* mqtt_payload = (const char*)fb->buf;

    int message_id;

    message_id = esp_mqtt_client_publish(mqtt_client, CAM_TOPIC, mqtt_payload, fb->len, 0, false);

    if (message_id < 0) {
        ESP_LOGE(TAG, "Message was not sent, returned %d", message_id);
    }else {
    ESP_LOGI(TAG, "Published successfully, msg_id: %d, size: %d bytes", message_id, fb->len);
}
}
