#include "utils.h"

pwm_config_motor pwm_a, pwm_b, pwm_c;

#define teste_pin 15
 int main() {
  stdio_init_all();
  sleep_ms(2000);

  // Configure the GPIOs for the motor
  gpio_init(EN1);
  gpio_init(EN2);
  gpio_init(EN3);

  gpio_set_dir(EN1, GPIO_OUT);
  gpio_set_dir(EN2, GPIO_OUT);
  gpio_set_dir(EN3, GPIO_OUT);

  gpio_init(teste_pin);
  gpio_set_dir(teste_pin, GPIO_OUT);

  // configure pwm for motor
  uint pwm_a_slice, pwm_a_chan;
  uint pwm_b_slice, pwm_b_chan;
  uint pwm_c_slice, pwm_c_chan;
  init_pwm(IN1, PWM_RES, &pwm_a_slice, &pwm_a_chan);
  init_pwm(IN2, PWM_RES, &pwm_b_slice, &pwm_b_chan);
  init_pwm(IN3, PWM_RES, &pwm_c_slice, &pwm_c_chan);

  pwm_a.slice_num = pwm_a_slice;
  pwm_a.chan_num = pwm_a_chan;
  pwm_b.slice_num = pwm_b_slice;
  pwm_b.chan_num = pwm_b_chan;
  pwm_c.slice_num = pwm_c_slice;
  pwm_c.chan_num = pwm_c_chan;

  // Configure the encoder GPIO
  gpio_init(ENCODER);
  gpio_set_dir(ENCODER, GPIO_IN);
  gpio_pull_up(ENCODER);
  gpio_set_irq_enabled_with_callback(ENCODER, GPIO_IRQ_EDGE_FALL, true,
                                     &extern_callback);

  // Set up a repeating timer for motor control (10 Hz = 100 ms interval)
  int timer_0_hz = 200;
  repeating_timer_t timer_0;

  if (!add_repeating_timer_us(-1000000 / timer_0_hz, timer_0_callback, NULL,
                              &timer_0)) {
    printf("Failed to add timer\n");
    return 1;
  }

  int timer_1_hz = 10000;
  repeating_timer_t timer_1;

  if (!add_repeating_timer_us(-1000000 / timer_1_hz, timer_1_callback, NULL,
                              &timer_1)) {
    printf("Failed to add timer\n");
    return 1;
  }

  // Align the rotor before starting the motor
  align_rotor(pwm_a, pwm_b, pwm_c);

  // printf("Motor started\n");

  // init uart

  multicore_launch_core1(core1_main);    
  sleep_ms(500);

  t1 = to_us_since_boot(get_absolute_time());
  t_actual = t1 / 1e6;

  while (1) {
    get_sector();
    move_motor_pwm(pwm_a, pwm_b, pwm_c, 1);

    if (trigered) {

      t2 = to_us_since_boot(get_absolute_time());
      dt = (t2 - t1) / 1e6;
      t_actual += dt;
      t1 = t2;
      t2 = 0;
      angular_speed = rad_1_8 / dt;
      trigered = 0;
      send_log = 1;
    }
  }
}
