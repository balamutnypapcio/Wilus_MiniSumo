#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>

// Definicje pinów dla silników
#define MOTOR_DIRR_A PD3  // Kierunek lewego silnika
#define MOTOR_PWM_A PB1   // PWM lewego silnika
#define MOTOR_DIRR_B PD6  // Kierunek prawego silnika
#define MOTOR_PWM_B PD5   // PWM prawego silnika

// Inicjalizacja silników
void motors_init(void);

// Sterowanie lewym silnikiem
// pwm_value: 0-255, gdzie 0 to stop, wartości dodatnie - przód, ujemne - tył
void set_left_motor(int16_t pwm_value);

// Sterowanie prawym silnikiem
// pwm_value: 0-255, gdzie 0 to stop, wartości dodatnie - przód, ujemne - tył
void set_right_motor(int16_t pwm_value);

// Zatrzymanie obu silników
void motors_stop(void);

#endif // MOTORS_H