#include <avr/io.h>
#include <stdlib.h>  
#include <util/delay.h>
#include "i2c.h"
#include "vl53l0x_simple.h"
#include "motors.h"
#include "line_sensors.h"

/**
 * @file main.c
 * @brief Główna pętla sterująca robotem sumo z czujnikami linii oraz czterema czujnikami ToF (VL53L0X).
 * 
 * Zawiera funkcje diagnostyczne (miganie LED), inicjalizację czujników, obsługę detekcji przeciwnika
 * oraz unikania linii.
 */

// Częstotliwość CPU
#define F_CPU 10000000UL // 10MHz

// Definicje pinów LED
#define LED_2 PB4
#define LED_1 PB3

// Adresy czujników ToF (po zmianie adresów przez XSHUT)
uint8_t sensor_addrs[4] = {ADDR1, ADDR2, ADDR3, ADDR4};
// Piny XSHUT dla każdego czujnika
uint8_t xshut_pins[4] = {XSHUT1, XSHUT2, XSHUT3, XSHUT4};
// Tablica na ostatnie odczyty odległości
uint16_t distances[4] = {0, 0, 0, 0};
// Tablica informująca, które czujniki są aktywne
uint8_t sensor_working[4] = {0, 0, 0, 0};

/**
 * @brief Miganie diodą D1 (LED_1) określoną liczbę razy (wolno).
 */
void blinkD1(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        _delay_ms(100);
        PORTB &= ~(1 << LED_1);
        _delay_ms(100);
    }
}

/**
 * @brief Miganie diodą D2 (LED_2) określoną liczbę razy (wolno).
 */
void blinkD2(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_2);
        _delay_ms(100);
        PORTB &= ~(1 << LED_2);
        _delay_ms(100);
    }
}

/**
 * @brief Szybkie miganie diodą D1 (LED_1) określoną liczbę razy.
 */
void fastBlinkD1(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        _delay_ms(50);
        PORTB &= ~(1 << LED_1);
        _delay_ms(50);
    }
}

/**
 * @brief Miganie obiema diodami jednocześnie określoną liczbę razy.
 */
void blinkBoth(uint8_t times){
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        PORTB |= (1 << LED_2);
        _delay_ms(100);
        PORTB &= ~(1 << LED_1);
        PORTB &= ~(1 << LED_2);
        _delay_ms(100);
    }
}

/**
 * @brief Wyświetla wartość binarną na diodach LED (debugging).
 * 
 * Pokazuje 8-bitową wartość: starsze 4 bity na D1, młodsze 4 na D2.
 */
void display_binary_value(uint8_t value) {
    uint8_t high_nibble = (value >> 4) & 0x0F;  // Starsze 4 bity
    uint8_t low_nibble = value & 0x0F;          // Młodsze 4 bity
    
    _delay_ms(500);
    // Starszy nibble na D1
    for (uint8_t i = 0; i < 4; i++) {
        if (high_nibble & (1 << (3-i))) {
            PORTB |= (1 << LED_1);
            _delay_ms(500);
            PORTB &= ~(1 << LED_1);
        } else {
            PORTB |= (1 << LED_1);
            _delay_ms(100);
            PORTB &= ~(1 << LED_1);
        }
        _delay_ms(300);
    }
    _delay_ms(1000);
    // Młodszy nibble na D2
    for (uint8_t i = 0; i < 4; i++) {
        if (low_nibble & (1 << (3-i))) {
            PORTB |= (1 << LED_2);
            _delay_ms(500);
            PORTB &= ~(1 << LED_2);
        } else {
            PORTB |= (1 << LED_2);
            _delay_ms(100);
            PORTB &= ~(1 << LED_2);
        }
        _delay_ms(300);
    }
    _delay_ms(500);
}

/**
 * @brief Inicjalizuje piny LED oraz XSHUT do czujników ToF.
 */
void init_pins(void) {
    // LEDy jako wyjścia
    DDRB |= (1 << LED_1) | (1 << LED_2) | (1 << XSHUT3);
    DDRD |= (1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4);

    // Wyłącz wszystkie czujniki (XSHUT = LOW)
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
}

/**
 * @brief Sprawdza czy czujnik ToF odpowiada na podanym adresie I2C.
 * @param addr Adres I2C czujnika
 * @return 1 jeśli czujnik odpowiada (ACK), 0 jeśli nie.
 */
uint8_t check_sensor_responds(uint8_t addr) {
    i2c_start(addr << 1);
    uint8_t status = (TWSR & 0xF8);
    i2c_stop();
    return (status == 0x18) ? 1 : 0;
}

/**
 * @brief Inicjalizuje pojedynczy czujnik ToF (z nadaniem nowego adresu).
 * 
 * @param index Indeks czujnika (0-3)
 */
void init_specific_sensor(uint8_t index) {
    // Wyłącz wszystkie czujniki
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
    
    // Włącz tylko jeden czujnik
    if (index == 2) {
        PORTB |= (1 << xshut_pins[index]);
    } else {
        PORTD |= (1 << xshut_pins[index]);
    }
    _delay_ms(10);
    
    vl53l0x_init();
    _delay_ms(10);
    vl53l0x_set_address(sensor_addrs[index]);
    _delay_ms(10);
    sensor_working[index] = check_sensor_responds(sensor_addrs[index]);
}

/**
 * @brief Inicjalizuje wszystkie czujniki ToF, nadaje im unikalne adresy.
 */
void init_all_sensors(void) {
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
    
    for (uint8_t i = 0; i < 4; i++) {
        // Włącz tylko jeden czujnik
        if (i == 2) {
            PORTB |= (1 << xshut_pins[i]);
        } else {
            PORTD |= (1 << xshut_pins[i]);
        }
        _delay_ms(10);
        vl53l0x_init();
        _delay_ms(10);
        vl53l0x_set_address(sensor_addrs[i]);
        _delay_ms(10);
        sensor_working[i] = check_sensor_responds(sensor_addrs[i]);
        if (!sensor_working[i]) {
            fastBlinkD1(5); // Błąd
        }
        _delay_ms(500);
    }
    // Włącz wszystkie czujniki, które są aktywne
    for (uint8_t i = 0; i < 4; i++) {
        if (sensor_working[i]) {
            if (i == 2) {
                PORTB |= (1 << xshut_pins[i]);
            } else {
                PORTD |= (1 << xshut_pins[i]);
            }
        }
    }
    _delay_ms(10);
}

int main(void) {
    // Inicjalizacja
    i2c_init();
    init_pins();
    motors_init();
    line_sensors_init(); 
    _delay_ms(1000);
    blinkD1(F_CPU/1000000);
    // Inicjalizacja czujników ToF
    init_all_sensors();
    
    // Sygnał zakończenia inicjalizacji
    for (uint8_t i = 0; i < 2; i++) {
        PORTB |= (1 << LED_1) | (1 << LED_2);
        _delay_ms(200);
        PORTB &= ~((1 << LED_1) | (1 << LED_2));
        _delay_ms(200);
    }

    uint8_t isEnemy = 0;
    uint8_t spinsWihoutEnemy = 0;
    LineSensorsState lineSS;
    uint8_t isInFrontRight = 0;
    uint8_t isInFrontLeft = 0;   
    uint8_t isInFront = 0;
    uint8_t rideInFrontAfterBackLine = 0;

    // Główna pętla programu
    while (1) {
        if (rideInFrontAfterBackLine > 0) {
            set_left_motor(120);
            set_right_motor(120);
            rideInFrontAfterBackLine++;
            if (rideInFrontAfterBackLine > 3) {
                rideInFrontAfterBackLine = 0;
            }
        }

        lineSS = read_line_sensors();
        if ((lineSS.back && (lineSS.left || lineSS.right))) {
            set_left_motor(0);
            set_right_motor(0);
            continue;
        }
        if (lineSS.left && lineSS.right) {
            set_left_motor(-150);   
            set_right_motor(150); 
            continue; 
        }
        if (lineSS.left) {
            set_left_motor(150);  
            set_right_motor(-150);
            continue;
        }
        if (lineSS.right) {
            set_left_motor(-150);   
            set_right_motor(150);
            continue;  
        }
        if (lineSS.back) {
            rideInFrontAfterBackLine = 1;
            set_left_motor(150);
            set_right_motor(150);
            continue;            
        }

        // Odczyty z czujników ToF
        for (uint8_t i = 0; i < 4; i++) {
            if (!sensor_working[i]) continue;
            uint16_t distance = read_sensor(sensor_addrs[i]);
            distances[i] = distance;
            if (distance > 60 && distance < 250) {
                if (i == 0 && !lineSS.right) {
                    set_left_motor(-120);   
                    set_right_motor(+120);
                    isEnemy++;
                    break;
                }
                else if (i == 1) {
                    if (distance < 100) {
                        isInFront++;
                    } else {
                        isInFrontLeft++;
                    }
                    isEnemy++;
                }
                else if (i == 2) {
                    if (distance < 100) {
                        isInFront++;
                    } else {
                        isInFrontRight++;
                    }
                    isEnemy++;
                    break;
                }
                else if (i == 3 && !lineSS.left) {
                    set_left_motor(120);   
                    set_right_motor(-120); 
                    isEnemy++; 
                    break;
                }
                spinsWihoutEnemy = 0;
            }
        }
        if (isInFront || (isInFrontLeft && isInFrontRight)) {
            set_left_motor(160);   
            set_right_motor(160); 
        } else if (isInFrontLeft) {
            set_left_motor(-70);   
            set_right_motor(70); 
        } else if (isInFrontRight) {
            set_left_motor(70);   
            set_right_motor(-70); 
        }
        // Jeśli nie ma przeciwnika
        if (isEnemy == 0 && (spinsWihoutEnemy > 5) && (spinsWihoutEnemy < 20)) {
            set_left_motor(120);        
            set_right_motor(-120);  
            spinsWihoutEnemy++;
        } else if (isEnemy == 0 && spinsWihoutEnemy <= 5) {
            set_left_motor(0);
            set_right_motor(0);
            spinsWihoutEnemy++;
        } else {
            spinsWihoutEnemy = 0;
        }

        // Reset flag
        isEnemy = 0;
        isInFront = 0;
        isInFrontLeft = 0;
        isInFrontRight = 0;
    }
}