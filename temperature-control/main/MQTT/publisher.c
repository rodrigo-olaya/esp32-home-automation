#include "publisher.h"
#include "topic.h"

static const char* TAG = "publisher";

/* Where each sensor on this node is (set in config/private_config.h). */
static const char *location_for(sensor_flag_t source) {
    switch (source) {
        case SENSOR_SHT31:   return SHT31_LOCATION;
        case SENSOR_DS18B20: return DS18B20_LOCATION;
        default:             return "unknown";
    }
}

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
        {
            ESP_LOGI(TAG, "TOPIC=%.*s\r", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s\r", event->data_len, event->data);

            char topic[event->topic_len + 1];
            // snprintf(topic, event->topic_len, "%s", event->topic);
            memcpy(topic, event->topic, event->topic_len);
            topic[event->topic_len] = '\0';

            ESP_LOGI(TAG, "HANDLED TOPIC=%s", topic);
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

    vTaskDelay(pdMS_TO_TICKS(5000));

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

    char mqtt_payload[64];
    snprintf(mqtt_payload, sizeof(mqtt_payload), "%f", sensor_data->data);

    char topic[64];
    if (!build_topic(topic, sizeof(topic), location_for(sensor_data->source), sensor_data->type)) {
        ESP_LOGE(TAG, "Could not build a topic for source %d, reading dropped", (int)sensor_data->source);
        return;
    }

    ESP_LOGI(TAG, "%s: %f", topic, sensor_data->data);
    int message_id = esp_mqtt_client_publish(mqtt_client, topic, mqtt_payload, strlen(mqtt_payload), 0, false);

    if (message_id != 0) {
        ESP_LOGE(TAG, "Message was not sent, returned %d", message_id);
    }
}