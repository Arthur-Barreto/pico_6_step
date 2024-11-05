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

// number of angles from encoder
#define NUM_ANGLES 200
// Number of angles within a single sector
#define SECTOR_ANGLES 60
// Step within each sector
#define SECTOR_ANGLE_STEP (M_PI / 3 / NUM_ANGLES)

// Conversion factor for ADC (3.3V / 12-bit resolution)
extern const float CONVERSION_FACTOR;

// Motor control sequences
extern const uint8_t in_seq[6][3];
extern const uint8_t en_seq[6][3];

// Volatile variables that are shared between main loop and ISR
extern volatile uint8_t timer_200hz_status;
extern volatile uint8_t timer_1000hz_status;
extern volatile uint8_t encoder_status;
extern volatile uint8_t step_index;

// Angle of the current (used in FOC or other control algorithms)
extern volatile float current_angle;
extern volatile uint8_t previous_step_index;
extern volatile uint8_t status;

// pi control section
extern float id_ref;
extern float iq_ref;

extern float kp;
extern float ki;

extern float id_error, iq_error;
extern float id_integrator;
extern float iq_integrator;
extern float id_output, iq_output;

extern float integrator_max;

#endif // CONSTS_H
