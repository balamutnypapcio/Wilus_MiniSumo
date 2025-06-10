#include "vl53l0x_simple.h"
#include "i2c.h"
#include <util/delay.h>

/**
 * @file vl53l0x_simple.c
 * @brief Proste funkcje obsługi czujnika odległości VL53L0X przez interfejs I2C.
 * 
 * Zawiera podstawowe funkcje do inicjalizacji, odczytu dystansu, zmiany adresu
 * oraz niskopoziomowej obsługi rejestrów czujnika VL53L0X.
 */

#define VL53L0X_I2C_ADDR 0x29 ///< Domyślny adres I2C czujnika VL53L0X

/**
 * @brief Zapisuje pojedynczy bajt do rejestru czujnika VL53L0X.
 * 
 * @param reg Adres rejestru.
 * @param value Wartość do zapisania.
 */
void vl53l0x_write_reg(uint8_t reg, uint8_t value) {
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

/**
 * @brief Odczytuje pojedynczy bajt z rejestru czujnika VL53L0X.
 * 
 * @param reg Adres rejestru do odczytu.
 * @return Odczytana wartość bajtu.
 */
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

/**
 * @brief Odczytuje 16-bitową wartość dystansu z czujnika VL53L0X (z rejestru wyniku pomiaru).
 * 
 * @return Zmierzoną odległość (raw, jednostka zależna od konfiguracji - zwykle mm).
 */
uint16_t vl53l0x_read_range(void) {
    uint16_t val;
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(0x1E); // Adres rejestru wyniku pomiaru
    i2c_stop();

    i2c_start((VL53L0X_I2C_ADDR << 1) | 1);
    uint8_t high = i2c_read_ack();
    uint8_t low = i2c_read_nack();
    i2c_stop();

    val = ((uint16_t)high << 8) | low;
    return val;
}

/**
 * @brief Inicjalizuje czujnik VL53L0X (podstawowy soft-reset i konfiguracja startowa).
 * 
 * Wymagane przed rozpoczęciem pomiarów.
 */
void vl53l0x_init(void) {
    // Soft reset sekwencji inicjalizacji (wg dokumentacji ST)
    vl53l0x_write_reg(0x88, 0x00);
    vl53l0x_write_reg(0x80, 0x01);
    vl53l0x_write_reg(0xFF, 0x01);
    vl53l0x_write_reg(0x00, 0x00);
    vl53l0x_write_reg(0x91, 0x3c);
    vl53l0x_write_reg(0x00, 0x01);
    vl53l0x_write_reg(0xFF, 0x00);
    vl53l0x_write_reg(0x80, 0x00);

    // Rozpoczęcie pomiarów (tryb single-shot)
    vl53l0x_write_reg(0x00, 0x01); // SYSRANGE_START = 1
    _delay_ms(10);
}

/**
 * @brief Wykonuje pojedynczy pomiar dystansu i zwraca wynik (w mm).
 * 
 * @return Zmierzoną odległość w milimetrach.
 */
uint16_t vl53l0x_read_mm(void) {
    vl53l0x_write_reg(0x00, 0x01); // SYSRANGE_START - wyzwolenie pojedynczego pomiaru
    _delay_ms(30); // Czas na wykonanie pomiaru (wg dokumentacji)
    return vl53l0x_read_range();
}

/**
 * @brief Alternatywna funkcja do odczytu dystansu z czujnika VL53L0X pod dowolnym adresem.
 * 
 * Pozwala obsługiwać wiele czujników o różnych adresach.
 * 
 * @param addr Adres I2C czujnika.
 * @return Zmierzoną odległość (raw, jednostka zależna od konfiguracji - zwykle mm).
 */
uint16_t read_sensor(uint8_t addr) {
    // Wyzwolenie pojedynczego pomiaru
    i2c_start(addr << 1);
    i2c_write(0x00);
    i2c_write(0x01); // SYSRANGE_START
    i2c_stop();
    _delay_ms(15);
    
    // Odczyt wyniku pomiaru
    i2c_start(addr << 1);
    i2c_write(0x1E);
    i2c_stop();
    
    i2c_start((addr << 1) | 1);
    uint8_t high = i2c_read_ack();
    uint8_t low = i2c_read_nack();
    i2c_stop();
    
    return ((uint16_t)high << 8) | low;
}

/**
 * @brief Zmienia adres I2C czujnika VL53L0X.
 * 
 * Przydatne przy stosowaniu wielu czujników na jednej magistrali.
 * 
 * @param new_addr Nowy adres I2C (7-bitowy).
 */
void vl53l0x_set_address(uint8_t new_addr) {
    i2c_start(VL53L0X_I2C_ADDR << 1);
    i2c_write(0x8A);  // Rejestr I2C_SLAVE_DEVICE_ADDRESS
    i2c_write(new_addr);
    i2c_stop();
}