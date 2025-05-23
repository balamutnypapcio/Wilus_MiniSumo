#include "line_sensors.h"
#include <util/delay.h>

void line_sensors_init(void) {
    // Ustawienie pinów jako wejścia
    DDRC &= ~((1 << LS_FRONT_LEFT_PIN) | (1 << LS_FRONT_RIGHT_PIN) | (1 << LS_BACK_PIN));
    
    // Wyłączenie pull-up na pinach wejściowych
    PORTC &= ~((1 << LS_FRONT_LEFT_PIN) | (1 << LS_FRONT_RIGHT_PIN) | (1 << LS_BACK_PIN));
    
    // Włączenie przetwornika ADC
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX |= (1 << REFS0); // Napięcie referencyjne AVCC 5V
}

uint16_t read_adc(uint8_t channel) {
    // Wybór kanału ADC (0-7)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Rozpoczęcie konwersji
    ADCSRA |= (1 << ADSC);
    
    // Czekaj na zakończenie konwersji
    while(ADCSRA & (1 << ADSC));
    
    return ADC;
}

uint8_t read_front_left_sensor(void) {
    uint16_t value = read_adc(LS_FRONT_LEFT_PIN);
    return (value <= BLACK_THRESHOLD_LEFT);  // Zwraca 1 (true) gdy wartość jest poniżej 0.9V (czarna linia)
}

uint8_t read_front_right_sensor(void) {
    uint16_t value = read_adc(LS_FRONT_RIGHT_PIN);
    return (value <= BLACK_THRESHOLD_RIGHT);  // Zwraca 1 (true) gdy wartość jest poniżej 0.9V (czarna linia)
}

uint8_t read_back_sensor(void) {
    uint16_t value = read_adc(LS_BACK_PIN);
    return (value <= BLACK_THRESHOLD_BACK);  // Zwraca 1 (true) gdy wartość jest poniżej 0.9V (czarna linia)
}


LineSensorsState read_line_sensors(void) {
    LineSensorsState state;
    
    state.left = read_front_left_sensor();
    state.right = read_front_right_sensor();
    state.back = read_back_sensor();
    
    return state;
}

uint8_t is_on_line(void) {
    LineSensorsState state = read_line_sensors();
    return (state.left || state.right || state.back);
}

//  int8_t get_line_position(void) {
    //     LineSensorsState state = read_line_sensors();
        
    //     if(state.left && !state.right) return -2;      // Mocno w lewo
    //     if(state.left && state.back) return -1;        // Lekko w lewo
    //     if(!state.left && !state.right && state.back) return 0;  // Środek (tylko tył)
    //     if(state.right && state.back) return 1;        // Lekko w prawo
    //     if(!state.left && state.right) return 2;       // Mocno w prawo
    //     if(state.left && state.right && state.back) return 0;    // Wszystkie na linii
    //     if(!state.left && !state.right && !state.back) return 0; // Żaden na linii
        
    //     return 0; // Domyślnie zwróć środek
    // }