#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "vl53l0x_simple.h"
#define VL53L0X_ADDR 0x29 // domyślny adres czujnika
#define XSHUT_PIN PB0
#define STATUS_LED PB3
#define SENSOR_LED PB4

void init_pins(void) {
    DDRB |= (1 << XSHUT_PIN) | (1 << STATUS_LED) | (1 << SENSOR_LED);

    // XSHUT -> HIGH, wybudź czujnik
    PORTB |= (1 << XSHUT_PIN);
    _delay_ms(10);
}

void blink(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << STATUS_LED);
        _delay_ms(200);
        PORTB &= ~(1 << STATUS_LED);
        _delay_ms(200);
    }
}

// int main(void) {
//     init_pins();
//     i2c_init();

//     _delay_ms(100); // czas na start czujnika

//     // test komunikacji
//     uint8_t ack = i2c_start(VL53L0X_ADDR << 1);
//     i2c_stop();

//     if (ack == 0) {
//         // czujnik odpowiedział
//         PORTB |= (1 << SENSOR_LED); // zapal diodę
//     } else {
//         // brak odpowiedzi – migaj szybko
//         while (1) {
//             blink(1, 100);
//         }
//     }

//     // wolne miganie = OK
//     while (1) {
//         blink(1, 1500);
//     }
// }



int main(void) {
    i2c_init();
    vl53l0x_init();
    init_pins();
    
    _delay_ms(100);
    blink(5);

    // Sprawdzenie komunikacji
    uint8_t ack = i2c_start(VL53L0X_ADDR << 1);
    i2c_stop();

    if (ack == 0) {
        PORTB |= (1 << SENSOR_LED); // czujnik działa
    } else {
        while (1) {
            blink(1); // migaj szybko jeśli brak czujnika
        }
    }
    
    uint16_t prev_distance = vl53l0x_read_mm();
    _delay_ms(200);

    while (1) {
        uint16_t current_distance = vl53l0x_read_mm();

        // Jeśli zmiana większa niż 50mm -> uznaj to za "ruch"
        if ((current_distance > prev_distance + 50) || (current_distance + 50 < prev_distance)) {
            blink(3); // migaj szybko 3 razy
        }

        prev_distance = current_distance;
        _delay_ms(300); // Odstęp między pomiarami
    }
}
