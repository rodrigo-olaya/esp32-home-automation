#include "publisher.h"

void publish_data() {

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };

    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        printf("Could not create client\n");
        return;
    }

    esp_err_t client_start_ret = esp_mqtt_client_start(mqtt_client);
    if (client_start_ret != ESP_OK) {
        printf("Could not start client");
        esp_mqtt_client_destroy(mqtt_client);
        return;
    }

    int message_id = esp_mqtt_client_publish(mqtt_client, PI4_TOPIC, MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), 0, false);
    if (message_id != 0) {
        printf("Message was not sent, returned %d", message_id);
    }

    esp_err_t stop_ret = esp_mqtt_client_stop(mqtt_client);
    if (stop_ret != ESP_OK) {
        printf("Could not stop client");
    }

    esp_mqtt_client_destroy(mqtt_client);
}
