#ifndef CONSTS_H
#define CONSTS_H

#include <stdint.h>
#include <math.h>

// Pin definitions
#define IN1 2
#define IN2 3
#define IN3 4
#define EN1 5
#define EN2 6
#define EN3 7
#define ENCODER 16
#define CUR_A 26
#define CUR_B 27

// PWM frequency
#define PWM_FREQ 1e6
#define PWM_RES 4096

// motor voltage
#define MOTOR_VOLTAGE 12

#define MAX_VALUES 200 // angular speed measurement

// Motor control sequences
extern const uint8_t in_seq[6][3];
extern const uint8_t en_seq[6][3];

// Volatile variables that are shared between main loop and ISR
extern volatile uint8_t timer_200hz_status;
extern volatile uint8_t timer_1000hz_status;
extern volatile uint8_t encoder_status;
extern volatile uint8_t step_index;
extern volatile uint8_t sector_index;

// Angle of the current (used in FOC or other control algorithms)
extern volatile float current_angle;
extern volatile uint8_t status;

// pi control section

extern uint32_t t1;
extern uint32_t t2;
extern float dt;
extern float angular_speed;
// extern volatile float angular_speed[MAX_VALUES];
extern volatile float time[MAX_VALUES];
extern volatile uint8_t angular_speed_index;
extern volatile uint8_t data_collected;

#endif // CONSTS_H
