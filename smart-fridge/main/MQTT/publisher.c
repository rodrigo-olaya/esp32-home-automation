#include "publisher.h"

static const char* TAG = "publisher";

static bool mqtt_initialized = false;
static esp_mqtt_client_handle_t mqtt_client = NULL;

void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            int message_id = esp_mqtt_client_subscribe_single(mqtt_client, HEATER_TOPIC, 0);
            if (message_id < 0) {
                ESP_LOGE(TAG, "Not able to subscribe, returned %d", message_id);
            }
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
            ESP_LOGI(TAG, "TOPIC=%.*s\r", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s\r", event->data_len, event->data);
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

void publish_data(sensorData_t *sensor_data) {
    ESP_LOGI(TAG, "Attempting to publish");
    vTaskDelay(pdMS_TO_TICKS(100));

    char mqtt_payload[64];
    snprintf(mqtt_payload, sizeof(mqtt_payload), "%f", sensor_data->data);

    int message_id;

    if (sensor_data->type == TEMPERATURE){
        ESP_LOGI(TAG, "Temp: %f", sensor_data->data);
        message_id = esp_mqtt_client_publish(mqtt_client, TEMP_TOPIC, mqtt_payload, strlen(mqtt_payload), 0, false);
    }
    else {
        ESP_LOGI(TAG, "Humidity: %f", sensor_data->data);
        message_id = esp_mqtt_client_publish(mqtt_client, HUMIDITY_TOPIC, mqtt_payload, strlen(mqtt_payload), 0, false);
    }

    if (message_id != 0) {
        ESP_LOGE(TAG, "Message was not sent, returned %d", message_id);
    }
}