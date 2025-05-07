#include "ultrasonic.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "low_level_functions.h"
#include "gpio_defs.h"

#define TAG "ULTRASONIC"
#define MAX_TIMEOUT_US 30000
#define IRAM_ATTR __attribute__((section(".iram1")))

static volatile uint64_t echo_start = 0;
static volatile uint64_t echo_end = 0;
static volatile bool measurement_ready = false;


static void IRAM_ATTR echo_isr_handler(void* arg) {
  uint8_t gpio_num = (uint8_t)(uint32_t)arg;
  
  if (getGPIOInterruptStatus(gpio_num)) {
    if (digitalReadLowLevel(gpio_num)) {
      echo_start = esp_timer_get_time();
    } else {
      echo_end = esp_timer_get_time();
      measurement_ready = true;
    }
    clearGPIOInterruptStatus(gpio_num);  // move here
  }
  
}

void init_sensor(uint8_t trig, uint8_t echo) {
    // Configure trigger pin as output
    pinConfig(trig, OUTPUT);
    
    // Configure echo pin as input
    pinConfig(echo, INPUT);
    
    // Configure and install interrupt
    configureGPIOInterrupt(echo, GPIO_INTR_ANYEDGE);
    installGPIOISR(echo, echo_isr_handler, (void*)(uint32_t)echo);
}

float read_distance(uint8_t trig, uint8_t echo) {
    measurement_ready = false;
    
    // Generate trigger pulse
    digitalWriteLowLevel(trig, 0);
    custDelayMicroseconds(2);
    digitalWriteLowLevel(trig, 1);
    custDelayMicroseconds(10);
    digitalWriteLowLevel(trig, 0);
    
    // Wait for measurement with timeout
    uint32_t timeout_count = 0;
    while (!measurement_ready && timeout_count < MAX_TIMEOUT_US) {
        custDelayMicroseconds(1);
        timeout_count++;
    }
    
    if (!measurement_ready) {
        ESP_LOGW(TAG, "Measurement timeout");
        return -1;
    }
    
    float duration = echo_end - echo_start;
    float distance_cm = (duration * 0.0343f) / 2.0f;
    return distance_cm;
}
