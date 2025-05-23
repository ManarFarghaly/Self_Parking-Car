#include "motor_control.h"

#include "low_level_functions.h"
#include "motor_pwm.h"

// Define LEDC_TIMER_8_BIT if not already defined
#ifndef LEDC_TIMER_8_BIT
#define LEDC_TIMER_8_BIT 8
#endif

// Example GPIO pin assignments (adjust for your setup)
#define LEFT_PWM_PIN 25
#define LEFT_IN1_PIN 27
#define LEFT_IN2_PIN 14

#define RIGHT_PWM_PIN 26
#define RIGHT_IN3_PIN 33
#define RIGHT_IN4_PIN 32

#define LEFT_PWM_CHANNEL 0
#define RIGHT_PWM_CHANNEL 1

void motor_init() {
  // Direction pins as outputs
  pinConfig(LEFT_IN1_PIN, OUTPUT);
  pinConfig(LEFT_IN2_PIN, OUTPUT);
  pinConfig(RIGHT_IN3_PIN, OUTPUT);
  pinConfig(RIGHT_IN4_PIN, OUTPUT);

  // PWM pins init
  motor_pwm_init(LEFT_PWM_PIN, LEFT_PWM_CHANNEL, 5000, LEDC_TIMER_8_BIT);
  motor_pwm_init(RIGHT_PWM_PIN, RIGHT_PWM_CHANNEL, 5000, LEDC_TIMER_8_BIT);
}
// to use this function, you need to specify which side you are using and the direction you want to go

void motor_drive(MotorSide side, MotorDirection dir, uint8_t speed_percent) {
  uint8_t in1, in2, pwm_channel;

  if (side == MOTOR_LEFT) {
    in1 = LEFT_IN1_PIN;
    in2 = LEFT_IN2_PIN;
    pwm_channel = LEFT_PWM_CHANNEL;
  } else {
    in1 = RIGHT_IN3_PIN;
    in2 = RIGHT_IN4_PIN;
    pwm_channel = RIGHT_PWM_CHANNEL;
  }

  // Set direction
  switch (dir) {
    case MOTOR_FORWARD:
      digitalWriteLowLevel(in1, 1);
      digitalWriteLowLevel(in2, 0);
      break;
    case MOTOR_REVERSE:
      digitalWriteLowLevel(in1, 0);
      digitalWriteLowLevel(in2, 1);
      break;
    case MOTOR_STOP:
      digitalWriteLowLevel(in1, 0);
      digitalWriteLowLevel(in2, 0);
      break;
  }

  // Set speed
  motor_set_speed(pwm_channel, speed_percent);
}
// Function to make the car spin in place
void car_spin(SpinDirection spin_dir, uint8_t speed_percent) {
  switch (spin_dir) {
    case SPIN_CLOCKWISE:
      // Right turn in place
      motor_drive(MOTOR_LEFT, MOTOR_FORWARD, speed_percent);
      motor_drive(MOTOR_RIGHT, MOTOR_REVERSE, speed_percent);
      break;
    case SPIN_COUNTER_CLOCKWISE:
      // Left turn in place
      motor_drive(MOTOR_LEFT, MOTOR_REVERSE, speed_percent);
      motor_drive(MOTOR_RIGHT, MOTOR_FORWARD, speed_percent);
      break;
  }
}
