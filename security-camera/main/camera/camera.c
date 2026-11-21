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

    // Tune OV2640 sensor registers for better image sharpness and barcode legibility
    sensor_t *s = esp_camera_sensor_get();
    if (s != NULL) {
        s->set_sharpness(s, 2);      // Increase edge sharpness (range: -2 to 2 on OV2640)
        s->set_denoise(s, 1);        // Light denoising to reduce JPEG artifacts without blurring edges
        s->set_gainceiling(s, GAINCEILING_4X); // Cap gain to reduce noise in brighter conditions
        s->set_whitebal(s, 1);       // Enable auto white balance
        s->set_awb_gain(s, 1);       // Enable AWB gain
        s->set_wb_mode(s, 0);        // Auto white balance mode
        s->set_exposure_ctrl(s, 1);  // Enable auto exposure control
        s->set_aec2(s, 1);           // Enable AEC DSP - better exposure in variable light
        s->set_ae_level(s, 0);       // Neutral auto-exposure target level
        s->set_brightness(s, 1);     // Slight brightness boost for contrast
        s->set_contrast(s, 2);       // Slight contrast boost to make barcode bars crisper
        s->set_sharpness(s,2);
        s->set_saturation(s, 0);     // Neutral saturation (barcodes are B&W anyway)
        s->set_lenc(s, 1);           // Enable lens correction to reduce vignetting
        s->set_hmirror(s, 0);
        s->set_vflip(s, 0);
        ESP_LOGI(CAM_TAG, "Sensor tuning applied");
    } else {
        ESP_LOGW(CAM_TAG, "Could not get sensor handle for tuning");
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
    vTaskDelay(pdMS_TO_TICKS(1000));
}}
