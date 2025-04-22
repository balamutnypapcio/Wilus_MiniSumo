#include "i2c.h"

#define F_SCL 400000UL // 400 kHz
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16) / 2)

void i2c_init(void) {
    TWSR = 0x00;          // Brak preskalera
    TWBR = (uint8_t)TWBR_VAL;
    TWCR = (1 << TWEN);   // Włącz I2C
}

uint8_t i2c_start(uint8_t address) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & 0xF8) != 0x08) return 1; // Błąd startu
    
    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return (TWSR & 0xF8) != 0x18; // 0 = ACK otrzymany
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

uint8_t i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return (TWSR & 0xF8) != 0x28; // 0 = ACK otrzymany
}

uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void i2c_write_16bit(uint8_t address, uint16_t reg, uint8_t data) {
    i2c_start(address << 1);
    i2c_write((reg >> 8) & 0xFF);   // Starszy bajt adresu
    i2c_write(reg & 0xFF);          // Młodszy bajt adresu
    i2c_write(data);                // Dane
    i2c_stop();
}

void i2c_read_buf(uint8_t address, uint16_t reg, uint8_t *data, uint8_t len) {
    i2c_start(address << 1);
    i2c_write((reg >> 8) & 0xFF);   // Starszy bajt adresu
    i2c_write(reg & 0xFF);          // Młodszy bajt adresu
    i2c_start((address << 1) | 1);
    for(uint8_t i=0; i<len-1; i++) data[i] = i2c_read_ack();
    data[len-1] = i2c_read_nack();
    i2c_stop();
}