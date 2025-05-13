#include "motors.h"

void motors_init(void) {
    // Konfiguracja pinów kierunku jako wyjścia
    DDRD |= (1 << MOTOR_DIRR_A) | (1 << MOTOR_DIRR_B) | (1 << MOTOR_PWM_B);
    DDRB |= (1 << MOTOR_PWM_A);
    
    // Konfiguracja Timer1 dla PWM na PB1 (OC1A)
    // Fast PWM 8-bit
    TCCR1A |= (1 << COM1A1) | (1 << WGM10);
    TCCR1B |= (1 << CS11);  // prescaler 8
    
    // Konfiguracja Timer0 dla PWM na PD5 (OC0B)
    // Fast PWM
    TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B |= (1 << CS01);  // prescaler 8

    // Początkowo zatrzymaj silniki
    motors_stop();
}

void set_left_motor(int16_t pwm_value) {
    uint8_t pwm_abs;
    
    // Zabezpieczenie przed przekroczeniem zakresu
    if(pwm_value > 255) pwm_value = 255;
    if(pwm_value < -255) pwm_value = -255;
    
    // Oblicz wartość bezwzględną PWM
    pwm_abs = (pwm_value < 0) ? -pwm_value : pwm_value;
    
    // Ustaw kierunek
    if(pwm_value >= 0) {
        PORTD |= (1 << MOTOR_DIRR_A);  // Przód
    } else {
        PORTD &= ~(1 << MOTOR_DIRR_A); // Tył
    }
    
    // Ustaw wypełnienie PWM
    OCR1A = pwm_abs;
}

void set_right_motor(int16_t pwm_value) {
    uint8_t pwm_abs;
    
    // Zabezpieczenie przed przekroczeniem zakresu
    if(pwm_value > 255) pwm_value = 255;
    if(pwm_value < -255) pwm_value = -255;
    
    // Oblicz wartość bezwzględną PWM
    pwm_abs = (pwm_value < 0) ? -pwm_value : pwm_value;
    
    // Ustaw kierunek
    if(pwm_value >= 0) {
        PORTD |= (1 << MOTOR_DIRR_B);  // Przód
    } else {
        PORTD &= ~(1 << MOTOR_DIRR_B); // Tył
    }
    
    // Ustaw wypełnienie PWM
    OCR0B = pwm_abs;
}

void motors_stop(void) {
    // Zatrzymaj oba silniki
    OCR1A = 0;  // Lewy silnik
    OCR0B = 0;  // Prawy silnik
}