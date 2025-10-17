#include "camera.h"

/* The following temp code was retrieved from official ESP32 documentation (Example in repo)*/

static const char *CAM_TAG = "camera";

esp_err_t camera_init(){
    //power up the camera if PWDN pin is defined
    // if(CAM_PIN_PWDN != -1){
    //     pinMode(CAM_PIN_PWDN, OUTPUT);
    //     digitalWrite(CAM_PIN_PWDN, LOW);
    // }

    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(CAM_TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

esp_err_t camera_capture(){
    //acquire a frame
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(CAM_TAG, "Camera Capture Failed");
        return ESP_FAIL;
    }
    //replace this with your own function
    // process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
  
    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}