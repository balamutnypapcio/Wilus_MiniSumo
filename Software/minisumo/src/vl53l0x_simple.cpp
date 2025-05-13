#include "vl53l0x_simple.h"
#include "i2c.h"
#include <util/delay.h>

#define VL53L0X_I2C_ADDR 0x29

// Pomocnicze funkcje I2C
void vl53l0x_write_reg(uint8_t reg, uint8_t value) {
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

uint8_t vl53l0x_read_reg(uint8_t reg) {
    uint8_t value;
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(reg);
    i2c_stop();

    i2c_start((VL53L0X_I2C_ADDR << 1) | 1);
    value = i2c_read_nack();
    i2c_stop();
    return value;
}

uint16_t vl53l0x_read_range(void) {
    uint16_t val;
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(0x1E); // wynik pomiaru
    i2c_stop();

    i2c_start((VL53L0X_I2C_ADDR << 1) | 1);
    uint8_t high = i2c_read_ack();
    uint8_t low = i2c_read_nack();
    i2c_stop();

    val = ((uint16_t)high << 8) | low;
    return val;
}

void vl53l0x_init(void) {
    // Soft reset
    vl53l0x_write_reg(0x88, 0x00);
    vl53l0x_write_reg(0x80, 0x01);
    vl53l0x_write_reg(0xFF, 0x01);
    vl53l0x_write_reg(0x00, 0x00);
    vl53l0x_write_reg(0x91, 0x3c);
    vl53l0x_write_reg(0x00, 0x01);
    vl53l0x_write_reg(0xFF, 0x00);
    vl53l0x_write_reg(0x80, 0x00);

    // Start pomiarów
    vl53l0x_write_reg(0x00, 0x01); // SYSRANGE_START = 1 (single shot)
    _delay_ms(10);
}

uint16_t vl53l0x_read_mm(void) {
    // Trigger single shot
    vl53l0x_write_reg(0x00, 0x01); // SYSRANGE_START
    _delay_ms(30); // czas na pomiar
    return vl53l0x_read_range();
}

uint16_t read_sensor(uint8_t addr) {
    vl53l0x_set_address(addr); // opcjonalne: upewnij się, że czujnik reaguje

    // Zainicjuj pomiar
    i2c_start(addr << 1);
    i2c_write(0x00);  // przykład – zależne od Twojej wersji vl53l0x_simple
    i2c_write(0x01);
    i2c_stop();

    _delay_ms(30); // czas na pomiar

    i2c_start(addr << 1);
    i2c_write(0x1E); // rejestr wyniku (adres może się różnić)
    i2c_stop();

    i2c_start((addr << 1) | 1);
    uint8_t high = i2c_read_ack();
    uint8_t low = i2c_read_nack();
    i2c_stop();

    return ((uint16_t)high << 8) | low;
}


void vl53l0x_set_address(uint8_t new_addr) {
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(0x8A);  // register I2C_SLAVE_DEVICE_ADDRESS
    i2c_write(new_addr);
    i2c_stop();
}
