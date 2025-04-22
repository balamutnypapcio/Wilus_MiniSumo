#include "VL53L0X.h"
#include "i2c.h"
#include <util/delay.h>

// Minimalna inicjalizacja do single shot
uint8_t vl53l0x_init(uint8_t address) {
    uint8_t id = 0;
    i2c_read_buf(address, 0xC0, &id, 1);
    if (id != 0xEE) return 0;

    // Soft reset
    i2c_write_16bit(address, 0x0000, 0x01);
    _delay_ms(100);

    // Startowanie pomiaru w trybie single-shot
    i2c_write_16bit(address, 0x000E, 0x01); // SYSRANGE__START
    _delay_ms(10);

    return 1;
}

uint16_t vl53l0x_read_distance(uint8_t address) {
    uint8_t status;
    i2c_read_buf(address, 0x14, &status, 1);
    if ((status & 0x01) == 0) return 8190;

    uint8_t buf[2];
    i2c_read_buf(address, 0x1E, buf, 2); // FINAL_CROSSTALK_CORRECTED_RANGE
    return (buf[0] << 8) | buf[1];
}
