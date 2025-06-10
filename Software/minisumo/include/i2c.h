#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

#define F_SCL 400000UL // 400 kHz
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16) / 2)

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

void i2c_write_16bit(uint8_t address, uint16_t reg, uint8_t data);
void i2c_read_buf(uint8_t address, uint16_t reg, uint8_t *data, uint8_t len);

#endif
