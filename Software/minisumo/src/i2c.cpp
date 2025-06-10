#include "i2c.h"

/**
 * @file i2c.c
 * @brief Implementacja podstawowych funkcji obsługi magistrali I2C (TWI) dla mikrokontrolerów AVR.
 * 
 * Funkcje służą do inicjalizacji interfejsu, wysyłania i odbierania danych, 
 * obsługi start/stop oraz transmisji z adresowaniem 16-bitowym (np. rejestry EEPROM, czujniki).
 * 
 * Wszystkie funkcje zakładają poprawną konfigurację zegara oraz podłączenie sprzętowe magistrali.
 */

/**
 * @brief Inicjalizuje interfejs I2C (TWI).
 * 
 * Ustawia preskaler na 1 (brak podziału) oraz rejestr bitowy TWBR w celu ustawienia prędkości.
 * Włącza obsługę sprzętową I2C.
 */
void i2c_init(void) {
    TWSR = 0x00;          // Brak preskalera (prescaler = 1)
    TWBR = (uint8_t)TWBR_VAL; // Ustaw prędkość (bit rate register)
    TWCR = (1 << TWEN);   // Włącz I2C (Two-Wire Interface Enable)
}

/**
 * @brief Wysyła warunek START i adres urządzenia na magistrali I2C.
 * 
 * @param address 7-bitowy adres urządzenia (bez bitu R/W).
 * @return 0 jeśli ACK otrzymano, 1 jeśli wystąpił błąd.
 */
uint8_t i2c_start(uint8_t address) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // START
    while (!(TWCR & (1 << TWINT)));                   // Oczekiwanie na zakończenie
    if ((TWSR & 0xF8) != 0x08) return 1;              // Sprawdzenie kodu statusu (START)
    
    TWDR = address;                                   // Wysyłka adresu
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));                   // Oczekiwanie na zakończenie
    return (TWSR & 0xF8) != 0x18;                     // 0 = ACK otrzymany, 1 = błąd
}

/**
 * @brief Wysyła warunek STOP na magistrali I2C.
 * 
 * Zakończenie transmisji.
 */
void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // STOP
    while (TWCR & (1 << TWSTO));                      // Czekaj aż STOP się wykona
}

/**
 * @brief Wysyła pojedynczy bajt danych na magistrali I2C.
 * 
 * @param data Bajt do wysłania.
 * @return 0 jeśli ACK otrzymano, 1 jeśli wystąpił błąd.
 */
uint8_t i2c_write(uint8_t data) {
    TWDR = data;                                      // Załaduj dane do wysłania
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));                   // Oczekiwanie na zakończenie
    return (TWSR & 0xF8) != 0x28;                     // 0 = ACK otrzymany, 1 = błąd
}

/**
 * @brief Odczytuje bajt danych z magistrali I2C i potwierdza odbiór (ACK).
 * 
 * @return Odczytany bajt danych.
 */
uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);  // Odczytaj i wyślij ACK
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

/**
 * @brief Odczytuje bajt danych z magistrali I2C bez potwierdzenia odbioru (NACK).
 * 
 * @return Odczytany bajt danych.
 */
uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);                // Odczytaj bez ACK (NACK)
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

/**
 * @brief Zapisuje pojedynczy bajt danych pod wskazany 16-bitowy adres rejestru.
 * 
 * Typowe dla urządzeń z 16-bitowym adresowaniem rejestrów (np. pamięci EEPROM, czujniki).
 * @param address 7-bitowy adres urządzenia.
 * @param reg 16-bitowy adres rejestru.
 * @param data Bajt do zapisania.
 */
void i2c_write_16bit(uint8_t address, uint16_t reg, uint8_t data) {
    i2c_start(address << 1);                  // Rozpocznij transmisję (write)
    i2c_write((reg >> 8) & 0xFF);             // Starszy bajt adresu rejestru
    i2c_write(reg & 0xFF);                    // Młodszy bajt adresu rejestru
    i2c_write(data);                          // Wysłanie danych
    i2c_stop();                               // Zakończenie transmisji
}

/**
 * @brief Odczytuje wiele bajtów z danego 16-bitowego rejestru urządzenia I2C.
 * 
 * Wysyła adres rejestru, następnie powtarza warunek START i odbiera zadeklarowaną liczbę bajtów.
 * @param address 7-bitowy adres urządzenia.
 * @param reg 16-bitowy adres rejestru.
 * @param data Wskaźnik na bufor, do którego zapisane zostaną odczytane dane.
 * @param len Liczba bajtów do odczytania.
 */
void i2c_read_buf(uint8_t address, uint16_t reg, uint8_t *data, uint8_t len) {
    i2c_start(address << 1);                  // Rozpocznij transmisję (write)
    i2c_write((reg >> 8) & 0xFF);             // Starszy bajt adresu rejestru
    i2c_write(reg & 0xFF);                    // Młodszy bajt adresu rejestru
    i2c_start((address << 1) | 1);            // Powtórz START (read)
    for(uint8_t i=0; i<len-1; i++) 
        data[i] = i2c_read_ack();             // Odczytaj bajty z ACK
    data[len-1] = i2c_read_nack();            // Ostatni bajt z NACK
    i2c_stop();                               // Zakończenie transmisji
}