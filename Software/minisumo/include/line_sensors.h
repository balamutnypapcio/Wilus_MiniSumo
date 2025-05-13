#ifndef LINE_SENSORS_H
#define LINE_SENSORS_H

#include <avr/io.h>

// Definicje pinów czujników
#define LS_FRONT_RIGHT_PIN PC0  // Przedni lewy czujnik
#define LS_FRONT_LEFT_PIN PC1  // Tylny czujnik
#define LS_BACK_PIN  PC2  // Przedni prawy czujnik


// Próg dla wszystkich czujników - 0.9V (około 184 ADC)
#define BLACK_THRESHOLD_LEFT 500  // 0.9V
#define BLACK_THRESHOLD_RIGHT 220  // 0.9V
#define BLACK_THRESHOLD_BACK 220  // 0.9V

// Struktura przechowująca stan czujników
typedef struct {
    uint8_t left;     // Stan lewego przedniego czujnika
    uint8_t right;    // Stan prawego przedniego czujnika
    uint8_t back;     // Stan tylnego czujnika
} LineSensorsState;

// Inicjalizacja czujników linii
void line_sensors_init(void);

// Odczyt stanu wszystkich czujników
LineSensorsState read_line_sensors(void);

// Odczyt pojedynczych czujników
uint8_t read_front_left_sensor(void);
uint8_t read_front_right_sensor(void);
uint8_t read_back_sensor(void);

// Funkcja zwracająca pozycję względem linii (-2 do 2)
int8_t get_line_position(void);

// Sprawdzenie czy robot jest na linii
uint8_t is_on_line(void);

#endif // LINE_SENSORS_H