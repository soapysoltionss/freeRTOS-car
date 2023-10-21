/*
 * @file motor_init.h
 * @brief define the constants and initialize the motor
 * @author Richie
 */

#ifndef MOTOR_INIT_H
#define MOTOR_INIT_H

#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "motor_config.h"

motor_speed_t g_motor_speed_left  = { .pwm_level   = 2500u,
                                      .pwm_channel = PWM_CHAN_A };

motor_speed_t g_motor_speed_right = { .pwm_level   = 2500u,
                                      .pwm_channel = PWM_CHAN_B };

void
motor_init(void)
{
    // Semaphore
    g_motor_speed_left.sem  = xSemaphoreCreateBinary();
    g_motor_speed_right.sem = xSemaphoreCreateBinary();

    gpio_init(SPEED_PIN_RIGHT);
    gpio_init(SPEED_PIN_LEFT);
    gpio_set_dir(SPEED_PIN_RIGHT, GPIO_IN);
    gpio_set_dir(SPEED_PIN_LEFT, GPIO_IN);

    // Initialize direction pins as outputs
    gpio_init(DIRECTION_PIN_RIGHT_IN1);
    gpio_init(DIRECTION_PIN_RIGHT_IN2);
    gpio_init(DIRECTION_PIN_LEFT_IN3);
    gpio_init(DIRECTION_PIN_LEFT_IN4);

    gpio_set_dir(DIRECTION_PIN_RIGHT_IN1, GPIO_OUT);
    gpio_set_dir(DIRECTION_PIN_RIGHT_IN2, GPIO_OUT);
    gpio_set_dir(DIRECTION_PIN_LEFT_IN3, GPIO_OUT);
    gpio_set_dir(DIRECTION_PIN_LEFT_IN4, GPIO_OUT);

    // Initialise PWM
    gpio_set_function(PWM_PIN_LEFT, GPIO_FUNC_PWM);
    gpio_set_function(PWM_PIN_RIGHT, GPIO_FUNC_PWM);

    g_motor_speed_left.slice_num  = pwm_gpio_to_slice_num(PWM_PIN_LEFT);
    g_motor_speed_right.slice_num = pwm_gpio_to_slice_num(PWM_PIN_RIGHT);

    // NOTE: PWM clock is 125MHz for raspberrypi pico w by default

    // 125MHz / 250 = 500kHz
    pwm_set_clkdiv(g_motor_speed_left.slice_num, PWM_CLK_DIV);
    pwm_set_clkdiv(g_motor_speed_right.slice_num, PWM_CLK_DIV);

    // have them to be 500kHz / 5000 = 100Hz
    pwm_set_wrap(g_motor_speed_left.slice_num, (PWM_WRAP - 1U));
    pwm_set_wrap(g_motor_speed_right.slice_num, (PWM_WRAP - 1U));

    pwm_set_enabled(g_motor_speed_left.slice_num, true);
    pwm_set_enabled(g_motor_speed_right.slice_num, true);
}

#endif /* MOTOR_INIT_H */