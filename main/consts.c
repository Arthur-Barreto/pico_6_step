#include "consts.h"
#include <stdint.h>

const uint8_t in_seq[6][3] = {{0, 1, 0}, {0, 1, 0}, {0, 0, 1},
                              {0, 0, 1}, {1, 0, 0}, {1, 0, 0}};

const uint8_t en_seq[6][3] = {{0, 1, 1}, {1, 1, 0}, {1, 0, 1},
                              {0, 1, 1}, {1, 1, 0}, {1, 0, 1}};

const float rad_1_8 = 0.5729577951308232;

volatile uint8_t timer_200hz_status = 0;
volatile uint8_t timer_1000hz_status = 0;
volatile uint8_t encoder_status = 0;
volatile uint8_t step_index = 0;
volatile uint8_t sector_index = 0;
volatile uint8_t trigered = 0;
volatile float current_angle = 0.0;
volatile uint8_t status = 0;

// pi control section
uint32_t t1 = 0;
uint32_t t2 = 0;
float dt = 0.0;
float angular_speed = 0.0;
// volatile float angular_speed[MAX_VALUES];
// volatile float time[MAX_VALUES];
volatile uint8_t angular_speed_index = 0;
volatile uint8_t data_collected = 0;