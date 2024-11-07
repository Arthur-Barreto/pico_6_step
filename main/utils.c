#include "utils.h"

// Timer callback to control motor movement
bool timer_0_callback(repeating_timer_t *rt) {
  timer_200hz_status = 1;
  return true;
}

// timer to update currents values
bool timer_1_callback(repeating_timer_t *rt) {
  timer_1000hz_status = 1;
  return true;
}

// Encoder interrupt callback
void extern_callback(uint gpio, uint32_t events) {

  if (gpio == ENCODER) {

    // gpio_put(15, status);
    // status = !status;

    current_angle += 1.8;

    // Reset the angle if it exceeds 360 degrees
    if (current_angle >= 360.0) {
      current_angle -= 360.0;
    }

    t2 = to_us_since_boot(get_absolute_time());
    dt = (t2 - t1) / 1e6;
    t1 = t2;
    angular_speed = 1.8 / dt;
    // angular_speed[angular_speed_index] = 1.8 / dt;
    // time[angular_speed_index] = dt;
    // angular_speed_index++;

    // if (angular_speed_index >= MAX_VALUES) {
    //   angular_speed_index = 0;
    //   data_collected = 1;
    // }
  }
}

// Function to align rotor to a known position
void align_rotor(pwm_config_motor pwm_a, pwm_config_motor pwm_b,
                 pwm_config_motor pwm_c) {

  gpio_put(EN1, 0);
  gpio_put(EN2, 1);
  gpio_put(EN3, 1);
  // Set the motor to a known position
  pwm_set_chan_level(pwm_a.slice_num, pwm_a.chan_num, 0 * PWM_RES);
  pwm_set_chan_level(pwm_b.slice_num, pwm_b.chan_num, 1 * PWM_RES);
  pwm_set_chan_level(pwm_c.slice_num, pwm_c.chan_num, 0 * PWM_RES);

  current_angle = 30.0; // Assume rotor is now aligned to 30 degrees

  // printf("Rotor aligned at 30 degrees.\n");

  // busy_wait_ms(50); // Give time for the rotor to stabilize

  // Disable all windings after alignment
  gpio_put(EN1, 0);
  gpio_put(EN2, 0);
  gpio_put(EN3, 0);

}

void init_pwm(int pwm_pin_gp, uint wrap_value, uint *slice_num,
              uint *chan_num) {
  gpio_set_function(pwm_pin_gp, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(pwm_pin_gp);
  uint chan = pwm_gpio_to_channel(pwm_pin_gp);

  // Set the minimum clock divisor of 1.0 for high frequency
  float clkdiv = 1.0f;

  // Use the provided wrap value directly for better control
  pwm_set_wrap(slice, wrap_value);

  // Set the calculated clock divisor
  pwm_set_clkdiv(slice, clkdiv);

  // Initialize the PWM channel level to 0 (duty cycle)
  pwm_set_chan_level(slice, chan, 0);

  // Enable PWM on this slice
  pwm_set_enabled(slice, true);

  // Pass back the slice and channel numbers
  *slice_num = slice;
  *chan_num = chan;
}

// Motor movement function based on the timer flag
void move_clockwise() {
  if (timer_200hz_status) {
    // Set motor pins based on the current step
    gpio_put(IN1, in_seq[step_index][0]);
    gpio_put(IN2, in_seq[step_index][1]);
    gpio_put(IN3, in_seq[step_index][2]);

    gpio_put(EN1, en_seq[step_index][0]);
    gpio_put(EN2, en_seq[step_index][1]);
    gpio_put(EN3, en_seq[step_index][2]);

    // Increment and wrap around the step index
    step_index = (step_index + 1) % 6;

    // Reset the timer flag
    timer_200hz_status = 0;
  }
}

void get_sector() {

  if (current_angle >= 0 && current_angle < 60) {
    sector_index = 0;
  } else if (current_angle >= 60 && current_angle < 120) {
    sector_index = 1;
  } else if (current_angle >= 120 && current_angle < 180) {
    sector_index = 2;
  } else if (current_angle >= 180 && current_angle < 240) {
    sector_index = 3;
  } else if (current_angle >= 240 && current_angle < 300) {
    sector_index = 4;
  } else if (current_angle >= 300 && current_angle < 360) {
    sector_index = 5;
  }
}

void get_angular_speed() {
}

void move_motor_pwm(pwm_config_motor pwm_a, pwm_config_motor pwm_b,
                    pwm_config_motor pwm_c, uint8_t direction) {


  // compute the next step index based on the current sector index
  // direction is -1 to move clockwise and +1 to move counter-clockwise
  // if the current sector is sector is 6, moving clockwise will move to sector 5

  step_index = (sector_index + direction) % 6;

  // Define duty cycles
  float duty_cycle = 0.4;

  // Apply PWM signals and enable signals based on the new step index
  pwm_set_chan_level(pwm_a.slice_num, pwm_a.chan_num,
                      in_seq[step_index][0] * PWM_RES * duty_cycle);
  pwm_set_chan_level(pwm_b.slice_num, pwm_b.chan_num,
                      in_seq[step_index][1] * PWM_RES * duty_cycle);
  pwm_set_chan_level(pwm_c.slice_num, pwm_c.chan_num,
                      in_seq[step_index][2] * PWM_RES * duty_cycle);

  gpio_put(EN1, en_seq[step_index][0]);
  gpio_put(EN2, en_seq[step_index][1]);
  gpio_put(EN3, en_seq[step_index][2]);
  
}

// void move_clockwise_pwm(pwm_config_motor pwm_a,
//                         pwm_config_motor pwm_b,
//                         pwm_config_motor pwm_c) {

//   if (timer_200hz_status) {
//     // Set motor pins based on the current step
//     pwm_set_chan_level(pwm_a.slice_num, pwm_a.chan_num,
//                        in_seq[step_index][0] * PWM_RES);
//     pwm_set_chan_level(pwm_b.slice_num, pwm_b.chan_num,
//                        in_seq[step_index][1] * PWM_RES);
//     pwm_set_chan_level(pwm_c.slice_num, pwm_c.chan_num,
//                        in_seq[step_index][2] * PWM_RES);

//     gpio_put(EN1, en_seq[step_index][0]);
//     gpio_put(EN2, en_seq[step_index][1]);
//     gpio_put(EN3, en_seq[step_index][2]);

//     // Increment and wrap around the step index
//     step_index = (step_index + 1) % 6;

//     // Reset the timer flag
//     timer_200hz_status = 0;
//   }
// }