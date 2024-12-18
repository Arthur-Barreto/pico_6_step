#ifndef UTILS_H
#define UTILS_H

#include "consts.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <stdio.h>

typedef struct {
  uint slice_num;
  uint chan_num;
} pwm_config_motor;

void init_pwm(int pwm_pin_gp, uint resolution, uint *slice_num, uint *chan_num);

bool timer_0_callback(repeating_timer_t *rt);
bool timer_1_callback(repeating_timer_t *rt);
void extern_callback(uint gpio, uint32_t events);
void align_rotor(pwm_config_motor pwm_a, pwm_config_motor pwm_b,
                 pwm_config_motor pwm_c);
void move_clockwise();
void get_sector();
void get_angular_speed();
void move_motor_pwm(pwm_config_motor pwm_a, pwm_config_motor pwm_b,
                    pwm_config_motor pwm_c, uint8_t direction);

void setup_uart();
void write_uart(const char *message);
void core1_main();
#endif // UTILS_H