#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

void i2c_write_16bit(uint8_t address, uint16_t reg, uint8_t data);
void i2c_read_buf(uint8_t address, uint16_t reg, uint8_t *data, uint8_t len);

#endif
