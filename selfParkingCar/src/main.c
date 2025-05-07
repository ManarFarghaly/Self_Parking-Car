#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ultrasonic.h"
#include "slot_detection.h"
#include "motion_control.h"
#include "pin_defs.h"

#define TAG "MAIN"

void app_main() {
    // Initialize sensors
    init_sensor(TRIG_FRONT, ECHO_FRONT);
    init_sensor(TRIG_BACK, ECHO_BACK);
    init_sensor(TRIG_LEFT, ECHO_LEFT);
    init_sensor(TRIG_RIGHT, ECHO_RIGHT);
    
    // Initialize servo for front sensor
    servo_init();
    
    // Initialize motion control
    motion_control_init();
    
    // Initialize slot detection
    init_slot_detection();
    
    ParkingSlot current_slot = {0};
    
    while (1) {
        // Read all sensor distances
        float front_distance = read_distance(TRIG_FRONT, ECHO_FRONT);
        float back_distance = read_distance(TRIG_BACK, ECHO_BACK);
        float left_distance = read_distance(TRIG_LEFT, ECHO_LEFT);
        float right_distance = read_distance(TRIG_RIGHT, ECHO_RIGHT);
        
        // Check for parking slot
        if (detect_parking_slot(&current_slot)) {
            if (current_slot.is_valid) {
                ESP_LOGI(TAG, "Suitable parking slot found!");
                motion_control_set_mode(MOTION_STOP, 0);  // Stop when slot found
                break;
            }
        }
        
        // Keep moving straight while checking for slots
        if (front_distance > 30.0f) {  // Safe distance
            // Update motion control with sensor readings
            motion_control_set_mode(MOTION_FORWARD, BASE_SPEED);
            motion_control_update(front_distance, back_distance, 
                                left_distance, right_distance);
        } else {
            // Stop if obstacle is too close
            motion_control_set_mode(MOTION_STOP, 0);
        }
        
        custDelayMicroseconds(50000);  // 50ms sampling rate
    }
}


// void app_main() {
//   init_sensor(TRIG_FRONT, ECHO_FRONT);
//   init_sensor(TRIG_BACK, ECHO_BACK);
//   init_sensor(TRIG_LEFT, ECHO_LEFT);
//   init_sensor(TRIG_RIGHT, ECHO_RIGHT);

//   while (1) {
//     float front = read_distance(TRIG_FRONT, ECHO_FRONT);
//     float back = read_distance(TRIG_BACK, ECHO_BACK);
//     float left = read_distance(TRIG_LEFT, ECHO_LEFT);
//     float right = read_distance(TRIG_RIGHT, ECHO_RIGHT);

//     ESP_LOGI("DISTANCES", "Front: %.2f, Back: %.2f, Left: %.2f, Right: %.2f", front, back, left, right);

//     vTaskDelay(pdMS_TO_TICKS(500));
//   }
// }

