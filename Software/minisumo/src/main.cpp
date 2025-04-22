#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

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

void blink(uint8_t times, uint16_t delay_ms) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << STATUS_LED);
        _delay_ms(delay_ms);
        PORTB &= ~(1 << STATUS_LED);
        _delay_ms(delay_ms);
    }
}

int main(void) {
    init_pins();
    i2c_init();

    _delay_ms(100); // czas na start czujnika

    // test komunikacji
    uint8_t ack = i2c_start(VL53L0X_ADDR << 1);
    i2c_stop();

    if (ack == 0) {
        // czujnik odpowiedział
        PORTB |= (1 << SENSOR_LED); // zapal diodę
    } else {
        // brak odpowiedzi – migaj szybko
        while (1) {
            blink(1, 200);
        }
    }

    // wolne miganie = OK
    while (1) {
        blink(1, 1500);
    }
}
