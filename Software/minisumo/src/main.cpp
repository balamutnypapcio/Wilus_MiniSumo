#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "vl53l0x_simple.h"
#include "motors.h"
#include "line_sensors.h"

#define LED_2 PB4
#define LED_1 PB3

uint8_t sensor_addrs[4] = {ADDR1, ADDR2, ADDR3, ADDR4};
uint8_t xshut_pins[4]    = {XSHUT1, XSHUT2, XSHUT3, XSHUT4};
uint16_t prev_distances[4];


void blinkD1(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        _delay_ms(200);
        PORTB &= ~(1 << LED_1);
        _delay_ms(200);
    }
}

void blinkD2(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_2);
        _delay_ms(200);
        PORTB &= ~(1 << LED_2);
        _delay_ms(200);
    }
}


void init_pins(void) {
    // STATUS_LED + XSHUT jako output
    DDRB |= (1 << LED_2) | (1 << XSHUT3);
    DDRD |= (1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4);

    // Wyłącz wszystkie czujniki (XSHUT = LOW)
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
}

void init_all_sensors(void) {
    for (uint8_t i = 0; i < 4; i++) {
        // Włącz tylko jeden czujnik
        if(i == 2){
        PORTB |= (1 << xshut_pins[i]);
        }else{
            PORTD |= (1 << xshut_pins[i]);
        }
        _delay_ms(10);

        vl53l0x_init();
        vl53l0x_set_address(sensor_addrs[i]);
        _delay_ms(10);
    }
}



int main(void) {
    i2c_init();
    init_pins();
    motors_init();
    line_sensors_init(); 
    blinkD2(3);

    //init_all_sensors();
    // Zainicjuj wartości początkowe
    // for (uint8_t i = 0; i < 4; i++) {
    //     prev_distances[i] = read_sensor(sensor_addrs[i]);
    //     _delay_ms(100);
    // }
    //blinkD1(5);

    
    while (1) {
        // Obsługa czujników (bez zmian)
        // for (uint8_t i = 0; i < 4; i++) {
        //     uint16_t current = read_sensor(sensor_addrs[i]);

        //     if ((current > prev_distances[i] + 50) || (current + 50 < prev_distances[i])) {
        //         if(i==0 || i == 2)
        //             blinkD2(2);
        //         if(i==1 || i == 3)
        //             blinkD2(3);
        //     }
        //     blinkD1(2);
        //     prev_distances[i] = current;
        //     _delay_ms(100);
        // }

        // Obsługa silników - przykład zmiany kierunku co 5 sekund
        // motor_timer++;
        // if (motor_timer >= 10) {  // Po około 5 sekundach (50 * 100ms)
        //     blinkD1(2);
        //     motor_timer = 0;
        //     // Zmiana kierunku
        //     static uint8_t direction = 0;
        //     direction = !direction;
            
        //     if (direction) {
        //         set_left_motor(50);   // Przód
        //         set_right_motor(-50);  // Przód
        //     } else {
        //         set_left_motor(-50);  // Tył
        //         set_right_motor(50); // Tył
        //     }
        // }
        

        // Przedni lewy czujnik - miga 1 raz
        if (read_front_left_sensor()) {
            blinkD1(1);
            set_left_motor(-50);   // Tył
            set_right_motor(+50);  // Tył
        }

        // Tylny czujnik - miga 2 razy
        if (read_front_right_sensor()) {
            blinkD1(2);
            set_left_motor(-100);   // Tył
            set_right_motor(+100);  // Tył
        }

        // Przedni prawy czujnik - miga 3 razy
        if (read_back_sensor()) {
            blinkD1(3);
            set_left_motor(150);   // Przód
            set_right_motor(-150);  // Przód
        }
        blinkD2(1);
        _delay_ms(100);
    }
}
