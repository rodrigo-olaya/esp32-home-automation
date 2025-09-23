#include "publisher.h"

static bool mqtt_initialized = false;
static esp_mqtt_client_handle_t mqtt_client = NULL;

void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t mqtt_client = event->client;

    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("Event connected\n");
            mqtt_initialized = true;
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT event disconnected\n");
            mqtt_initialized = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            printf("MQTT event subscribed\n");
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            printf("MQTT event unsubscribed");
            break;

        case MQTT_EVENT_PUBLISHED:
            printf("MQTT event published\n");
            break;

        case MQTT_EVENT_DATA:
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            printf("MQTT event eror\n");
            break;

        default:
            printf("default case\n");
            break;
    }
}

void mqtt_init() {
    if (mqtt_initialized) {
        printf("MQTT already initialized.\n");
        return;
    }

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        printf("Could not create client\n");
        return;
    }

    printf("created event\n");

    esp_err_t register_ret = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, event_handler, NULL);

    printf("event registered\n");

    esp_err_t client_start_ret = esp_mqtt_client_start(mqtt_client);
    if (client_start_ret != ESP_OK) {
        printf("Could not start client");
        esp_mqtt_client_destroy(mqtt_client);
        return;
    }

    printf("event started\n");
}

void publish_data() {
    printf("try to publish\n");
    vTaskDelay(pdMS_TO_TICKS(100));
    int message_id = esp_mqtt_client_publish(mqtt_client, PI4_TOPIC, MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), 0, false);
    if (message_id != 0) {
        printf("Message was not sent, returned %d", message_id);
    }
}
