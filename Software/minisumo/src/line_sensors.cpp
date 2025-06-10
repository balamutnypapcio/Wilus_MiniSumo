#include "line_sensors.h"
#include <util/delay.h>

/**
 * @file line_sensors.c
 * @brief Obsługa analogowych czujników linii (np. do robota linefollower) przy użyciu ADC mikrokontrolera AVR.
 * 
 * Zawiera funkcje inicjalizacji, odczytu pojedynczych czujników oraz zbiorczego odczytu stanu wszystkich czujników.
 */

/**
 * @brief Inicjalizuje piny czujników linii oraz przetwornik ADC.
 * 
 * - Ustawia piny czujników jako wejścia.
 * - Wyłącza wewnętrzne rezystory pull-up.
 * - Konfiguruje przetwornik ADC (włączenie, preskaler, napięcie referencyjne AVCC).
 */
void line_sensors_init(void) {
    // Ustawienie pinów jako wejścia (0 - wejście)
    DDRC &= ~((1 << LS_FRONT_LEFT_PIN) | (1 << LS_FRONT_RIGHT_PIN) | (1 << LS_BACK_PIN));
    
    // Wyłączenie pull-up na pinach wejściowych (0 - pull-up wyłączony)
    PORTC &= ~((1 << LS_FRONT_LEFT_PIN) | (1 << LS_FRONT_RIGHT_PIN) | (1 << LS_BACK_PIN));
    
    // Włączenie przetwornika ADC
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Enable + preskaler 128
    ADMUX |= (1 << REFS0); // Napięcie referencyjne AVCC (5V)
}

/**
 * @brief Odczytuje wartość analogową z wybranego kanału ADC (czujnika linii).
 * 
 * @param channel Numer kanału ADC (0-7, odpowiadający pinowi czujnika).
 * @return Wartość 10-bitowa z przetwornika ADC (0-1023).
 */
uint16_t read_adc(uint8_t channel) {
    // Wybór kanału ADC (dolne 4 bity)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Rozpoczęcie konwersji
    ADCSRA |= (1 << ADSC);
    
    // Czekaj na zakończenie konwersji
    while(ADCSRA & (1 << ADSC));
    
    return ADC;
}

/**
 * @brief Odczytuje stan przedniego lewego czujnika linii.
 * 
 * @return 1 (true) jeśli wykryto linię (wartość poniżej progu, biała linia), 0 w przeciwnym wypadku.
 */
uint8_t read_front_left_sensor(void) {
    uint16_t value = read_adc(LS_FRONT_LEFT_PIN);
    return (value < BLACK_THRESHOLD_LEFT);  // 1 gdy wartość poniżej progu (linia)
}

/**
 * @brief Odczytuje stan przedniego prawego czujnika linii.
 * 
 * @return 1 (true) jeśli wykryto linię (wartość poniżej progu, biała linia), 0 w przeciwnym wypadku.
 */
uint8_t read_front_right_sensor(void) {
    uint16_t value = read_adc(LS_FRONT_RIGHT_PIN);
    return (value < BLACK_THRESHOLD_RIGHT); // 1 gdy wartość poniżej progu (linia)
}

/**
 * @brief Odczytuje stan tylnego czujnika linii.
 * 
 * @return 1 (true) jeśli wykryto linię (wartość poniżej progu, biała linia), 0 w przeciwnym wypadku.
 */
uint8_t read_back_sensor(void) {
    uint16_t value = read_adc(LS_BACK_PIN);
    return (value < BLACK_THRESHOLD_BACK); // 1 gdy wartość poniżej progu (linia)
}

/**
 * @brief Zbiorczo odczytuje wszystkie czujniki linii i zwraca ich stany w strukturze.
 * 
 * @return Struktura LineSensorsState z aktualnym stanem czujników (1 = linia wykryta).
 */
LineSensorsState read_line_sensors(void) {
    LineSensorsState state;
    
    state.left = read_front_left_sensor();
    state.right = read_front_right_sensor();
    state.back = read_back_sensor();
    
    return state;
}