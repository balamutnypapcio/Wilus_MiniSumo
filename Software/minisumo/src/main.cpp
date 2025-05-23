// #include <avr/io.h>
// #include <util/delay.h>
// #include "i2c.h"
// #include "vl53l0x_simple.h"
// #include "motors.h"
// #include "line_sensors.h"

// #define LED_2 PB4
// #define LED_1 PB3


// uint8_t sensor_addrs[4] = {ADDR1, ADDR2, ADDR3, ADDR4};
// uint8_t xshut_pins[4]    = {XSHUT1, XSHUT2, XSHUT3, XSHUT4};
// uint16_t prev_distances[4];


// void blinkD1(uint8_t times) {
//     for (uint8_t i = 0; i < times; i++) {
//         PORTB |= (1 << LED_1);
//         _delay_ms(200);
//         PORTB &= ~(1 << LED_1);
//         _delay_ms(200);
//     }
// }

// void blinkD2(uint8_t times) {
//     for (uint8_t i = 0; i < times; i++) {
//         PORTB |= (1 << LED_2);
//         _delay_ms(200);
//         PORTB &= ~(1 << LED_2);
//         _delay_ms(200);
//     }
// }


// void init_pins(void) {
//     // STATUS_LED + XSHUT jako output
//     DDRB |= (1 << LED_1) | (1 << LED_2) | (1 << XSHUT3);
//     DDRD |= (1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4);

//     // Wyłącz wszystkie czujniki (XSHUT = LOW)
//     PORTB &= ~(1 << XSHUT3);
//     PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
//     _delay_ms(20);
// }

// // void init_all_sensors(void) {
// //     // Wyłącz wszystkie czujniki
// //     PORTB &= ~(1 << XSHUT3);
// //     PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
// //     _delay_ms(10);
    
// //     for (uint8_t i = 0; i < 4; i++) {
// //         // Włącz tylko jeden czujnik
// //         if(i == 2) {
// //             PORTB |= (1 << xshut_pins[i]);
// //         } else {
// //             PORTD |= (1 << xshut_pins[i]);
// //         }
// //         _delay_ms(10);
        
// //         // Inicjalizacja z domyślnym adresem
// //         vl53l0x_init();
// //         _delay_ms(10);
        
// //         // Zmiana adresu
// //         vl53l0x_set_address(sensor_addrs[i]);
// //         _delay_ms(10);
// //     }
// // }
// uint8_t k=1;

// void init_leftTof_sensors(void) {
//     PORTD |= (1 << xshut_pins[k]);  // Włącz czujnik
//     _delay_ms(10);
//     vl53l0x_init();                  // Najpierw inicjalizuj
//     _delay_ms(10);
//     vl53l0x_set_address(sensor_addrs[k]); // Potem ustaw adres
//     _delay_ms(10);
// }

// void init_specific_sensor(uint8_t index) {
//     // Wyłącz wszystkie czujniki
//     PORTB &= ~(1 << XSHUT3);
//     PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
//     _delay_ms(20);
    
//     // Włącz tylko wybrany czujnik
//     if(index == 2) {
//         PORTB |= (1 << xshut_pins[index]);
//     } else {
//         PORTD |= (1 << xshut_pins[index]);
//     }
//     _delay_ms(10);
    
//     // Inicjalizacja z domyślnym adresem
//     vl53l0x_init();
//     _delay_ms(10);
    
//     // Zmiana adresu
//     vl53l0x_set_address(sensor_addrs[index]);
//     _delay_ms(10);
// }

// int main(void) {
//     i2c_init();
//     init_pins();
//     motors_init();
//     line_sensors_init(); 
//     blinkD2(3);

//     //init_leftTof_sensors();
//     //Zainicjuj wartości początkowe
//     // for (uint8_t i = 0; i < 4; i++) {
//     //     prev_distances[i] = read_sensor(sensor_addrs[i]);
//     //     _delay_ms(100);
//     // }
//    // blinkD1(5);

//        // Przetestuj każdy czujnik osobno
//        for (uint8_t test_k = 0; test_k < 4; test_k++) {
//         init_specific_sensor(test_k);
        
//         // Pokaż który czujnik testujemy
//         blinkD2(test_k + 1);
//         _delay_ms(500);
        
//         // Spróbuj odczytać odległość
//         uint16_t distance = read_sensor(sensor_addrs[test_k]);
        
//         // Jeśli odczyt się powiódł i jest w zakresie, migaj D1
//         if (distance < 200 && distance > 50) {
//             blinkD1(distance / 10);
//         } else {
//             // Migaj szybko D1, jeśli czujnik nie działa
//             for (uint8_t j = 0; j < 5; j++) {
//                 PORTB |= (1 << LED_1);
//                 _delay_ms(100);
//                 PORTB &= ~(1 << LED_1);
//                 _delay_ms(100);
//             }
//         }
//         _delay_ms(1000);
//     }
    
//     // Wybierz czujnik do użycia w głównej pętli
//     uint8_t k = 3; // Możesz zmienić na działający czujnik
//     init_specific_sensor(k);
    
//     while (1) {
//         //Obsługa czujników ToF 
//         // for (uint8_t i = 0; i < 4; i++) {
//         //     uint16_t distance = read_sensor(sensor_addrs[i]);
            
//         //     if (distance < 200 && distance > 50) {
//         //         blinkD2(i+1);
//         //         blinkD1(distance / 10);
//         //     }
//         //     _delay_ms(100);
//         // }



        



//             uint16_t distance = read_sensor(sensor_addrs[k]);
            
//             if (distance < 200 && distance > 50) {
//                 blinkD1(distance / 10);
//             }
//             _delay_ms(100);















//         // // Przedni lewy czujnik - miga 1 raz
//         // if (read_front_left_sensor()) {
//         //     blinkD1(1);
//         //     set_left_motor(-50);   // Tył
//         //     set_right_motor(+50);  // Tył
//         // }

//         // // Tylny czujnik - miga 2 razy
//         // if (read_front_right_sensor()) {
//         //     blinkD1(2);
//         //     set_left_motor(-100);   // Tył
//         //     set_right_motor(+100);  // Tył
//         // }

//         // // Przedni prawy czujnik - miga 3 razy
//         // if (read_back_sensor()) {
//         //     blinkD1(3);
//         //     set_left_motor(150);   // Przód
//         //     set_right_motor(-150);  // Przód
//         // }

//         // if(!read_front_left_sensor() && !read_front_right_sensor() && !read_back_sensor()){
//         //     set_left_motor(0);   // Brak ruchu
//         //     set_right_motor(0);  // Brak ruchu
//         // }

//         blinkD2(1);
//         _delay_ms(100);
//     }
// }




#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "vl53l0x_simple.h"
#include "motors.h"
#include "line_sensors.h"

#define LED_2 PB4
#define LED_1 PB3

// Adresy czujników (po zmianie adresów)
uint8_t sensor_addrs[4] = {ADDR1, ADDR2, ADDR3, ADDR4};
// Piny XSHUT do włączania/wyłączania czujników
uint8_t xshut_pins[4] = {XSHUT1, XSHUT2, XSHUT3, XSHUT4};
// Tablica na poprzednie odczyty odległości
uint16_t distances[4] = {0, 0, 0, 0};
// Tablica na informację o stanie czujników (0 = nie działa, 1 = działa)
uint8_t sensor_working[4] = {1, 0, 1, 1};

// Miga diodą D1 określoną liczbę razy
void blinkD1(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        _delay_ms(200);
        PORTB &= ~(1 << LED_1);
        _delay_ms(200);
    }
}

// Miga diodą D2 określoną liczbę razy
void blinkD2(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_2);
        _delay_ms(200);
        PORTB &= ~(1 << LED_2);
        _delay_ms(200);
    }
}

// Szybkie miganie diodą D1 (sygnalizacja błędu)
void fastBlinkD1(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        PORTB |= (1 << LED_1);
        _delay_ms(50);
        PORTB &= ~(1 << LED_1);
        _delay_ms(50);
    }
}

// Inicjalizacja pinów (LED i XSHUT)
void init_pins(void) {
    // Konfiguracja pinów LED i XSHUT jako wyjścia
    DDRB |= (1 << LED_1) | (1 << LED_2) | (1 << XSHUT3);
    DDRD |= (1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4);

    // Wyłącz wszystkie czujniki (XSHUT = LOW)
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
}

// Sprawdza, czy czujnik odpowiada na podanym adresie
uint8_t check_sensor_responds(uint8_t addr) {
    i2c_start(addr << 1);
    uint8_t status = (TWSR & 0xF8);
    i2c_stop();
    
    // 0x18 oznacza, że urządzenie odpowiedziało ACK po wysłaniu SLA+W
    return (status == 0x18) ? 1 : 0;
}

// Inicjalizuje pojedynczy czujnik
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
    
    // Inicjalizacja z domyślnym adresem
    vl53l0x_init();
    _delay_ms(10);
    
    // Zmiana adresu
    vl53l0x_set_address(sensor_addrs[index]);
    _delay_ms(10);
    
    // Sprawdź, czy czujnik odpowiada na nowym adresie
    sensor_working[index] = check_sensor_responds(sensor_addrs[index]);
}

// Inicjalizuje wszystkie czujniki - każdy z unikalnym adresem
void init_all_sensors(void) {
    // Najpierw wyłącz wszystkie czujniki
    PORTB &= ~(1 << XSHUT3);
    PORTD &= ~((1 << XSHUT1) | (1 << XSHUT2) | (1 << XSHUT4));
    _delay_ms(20);
    
    // Inicjalizuj każdy czujnik pojedynczo
    for (uint8_t i = 0; i < 4; i++) {
        // Pokaż który czujnik inicjalizujemy (diagnostyka)
        blinkD2(i+1);
        
        // Włącz tylko ten jeden czujnik
        if (i == 2) {
            PORTB |= (1 << xshut_pins[i]);
        } else {
            PORTD |= (1 << xshut_pins[i]);
        }
        _delay_ms(10);
        
        // Inicjalizacja z domyślnym adresem
        vl53l0x_init();
        _delay_ms(10);
        
        // Zmiana adresu
        vl53l0x_set_address(sensor_addrs[i]);
        _delay_ms(10);
        
        // Sprawdź, czy czujnik odpowiada na nowym adresie
        sensor_working[i] = check_sensor_responds(sensor_addrs[i]);
        
        // Pokaż czy inicjalizacja się udała
        if (sensor_working[i]) {
            blinkD1(1); // OK - jedno mignięcie
        } else {
            fastBlinkD1(5); // Błąd - 5 szybkich mignięć
        }
        
        _delay_ms(500);
    }
    
    // Włącz wszystkie czujniki, które zostały zainicjowane
    // Te, które nie odpowiedziały, pozostaną wyłączone
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
    // Inicjalizacja peryferiów
    i2c_init();
    init_pins();
    motors_init();
    line_sensors_init(); 
    
    // Sygnał startu programu - 3 mignięcia D2
    blinkD2(3);
    _delay_ms(500);
    
    // Inicjalizacja wszystkich czujników
    init_all_sensors();
    
    // Sygnał zakończenia inicjalizacji - 2 mignięcia obu diod
    for (uint8_t i = 0; i < 2; i++) {
        PORTB |= (1 << LED_1) | (1 << LED_2);
        _delay_ms(200);
        PORTB &= ~((1 << LED_1) | (1 << LED_2));
        _delay_ms(200);
    }
    
       

    // Główna pętla programu
    while (1) {

            // Sprawdź każdy czujnik
        for (uint8_t i = 0; i < 4; i++) {
            // Pomiń czujniki, które nie działają
            if (!sensor_working[i]) continue;
                
            // Pokaż który czujnik testujemy (1-4 mignięcia D2)
            blinkD2(i+1);
                
            // Odczyt odległości z czujnika
            uint16_t distance = read_sensor(sensor_addrs[i]);
            distances[i] = distance; // Zapisz odczyt
                
            // Pokaż odległość, jeśli jest w zakresie
            if (distance > 50 && distance < 200) {
                blinkD1(distance / 10); // Jedno mignięcie na każde 10 mm
            }
            //  else if (distance > 0) {
            //     // Odległość poza zakresem ale > 0
            //     blinkD1(1);
            //     _delay_ms(500);
            // } else {
            //     // Błąd lub brak odczytu
            //     fastBlinkD1(3);
        }
        
                // Przedni lewy czujnik - miga 1 raz
        if (read_front_left_sensor()) {
            //blinkD1(1);
            set_left_motor(-50);   // Tył
            set_right_motor(+50);  // Tył
        }

        // Tylny czujnik - miga 2 razy
        if (read_front_right_sensor()) {
            //blinkD1(2);
            set_left_motor(-100);   // Tył
            set_right_motor(+100);  // Tył
        }

        // Przedni prawy czujnik - miga 3 razy
        if (read_back_sensor()) {
            //blinkD1(3);
            set_left_motor(150);   // Przód
            set_right_motor(-150);  // Przód
        }

        if(!read_front_left_sensor() && !read_front_right_sensor() && !read_back_sensor()){
            set_left_motor(0);   // Brak ruchu
            set_right_motor(0);  // Brak ruchu
        }

        _delay_ms(100);
    }
}