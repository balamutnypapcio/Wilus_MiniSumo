#ifndef VL53L0X_SIMPLE_H
#define VL53L0X_SIMPLE_H

#include <avr/io.h>
#include <stdint.h>

// XSHUT piny
#define XSHUT1 PD0 // left left
#define XSHUT2 PD1 //left front
#define XSHUT3 PB0 // left left
#define XSHUT4 PD7 // right right

// Nowe adresy, bo na start kazdy ToF ma adres 0x29
#define ADDR1 0x30
#define ADDR2 0x31
#define ADDR3 0x32
#define ADDR4 0x33

void vl53l0x_init(void);
uint16_t vl53l0x_read_mm(void);
void vl53l0x_set_address(uint8_t new_addr);
uint16_t read_sensor(uint8_t addr);

#endif
