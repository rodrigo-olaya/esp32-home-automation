#include "camera.h"

static const char *CAM_TAG = "camera";

cam_status_t cam_status = CAM_OFF;

esp_err_t camera_init(){
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(CAM_TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

esp_err_t camera_capture(){
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(CAM_TAG, "Camera Capture Failed");
        return ESP_FAIL;
    }

    ESP_LOGI(CAM_TAG, "Width: %d", fb->width);
    ESP_LOGI(CAM_TAG, "Height: %d", fb->height);
    ESP_LOGI(CAM_TAG, "Format: %d", fb->format);
    ESP_LOGI(CAM_TAG, "Size in bytes: %d", fb->len);

    publish_data(fb);
  
    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}

void turn_camera_on_or_off(esp_mqtt_event_handle_t event) {
    char data[event->data_len + 1];
    memcpy(data, event->data, event->data_len);
    data[event->data_len] = '\0';

    if (strcmp(data, "CAM_OFF") == 0 && cam_status == CAM_ON) {
        ESP_LOGI(CAM_TAG, "Received Camera OFF command");
        cam_status = CAM_OFF;
    }
    else if (strcmp(data, "CAM_ON") == 0 && cam_status == CAM_OFF) {
        ESP_LOGI(CAM_TAG, "Received Camera ON command");
        cam_status = CAM_ON;
    }
}

void camera_handler(){
    while (1){
    if (cam_status == CAM_ON) {
        ESP_LOGI(CAM_TAG, "calling capture");
        camera_capture();
        
    }
vTaskDelay(pdMS_TO_TICKS(100));}}
