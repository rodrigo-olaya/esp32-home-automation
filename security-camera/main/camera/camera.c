#include "camera.h"

static const char *CAM_TAG = "camera";

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