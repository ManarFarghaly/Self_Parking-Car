#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ultrasonic.h"

#define TRIG_FRONT 5
#define ECHO_FRONT 18
#define TRIG_BACK 2
#define ECHO_BACK 15
#define TRIG_LEFT 32
#define ECHO_LEFT 33
#define TRIG_RIGHT 12
#define ECHO_RIGHT 13

void app_main() {
  init_sensor(TRIG_FRONT, ECHO_FRONT);
  init_sensor(TRIG_BACK, ECHO_BACK);
  init_sensor(TRIG_LEFT, ECHO_LEFT);
  init_sensor(TRIG_RIGHT, ECHO_RIGHT);

  while (1) {
    float front = read_distance(TRIG_FRONT, ECHO_FRONT);
    float back = read_distance(TRIG_BACK, ECHO_BACK);
    float left = read_distance(TRIG_LEFT, ECHO_LEFT);
    float right = read_distance(TRIG_RIGHT, ECHO_RIGHT);

    ESP_LOGI("DISTANCES", "Front: %.2f, Back: %.2f, Left: %.2f, Right: %.2f", front, back, left, right);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}