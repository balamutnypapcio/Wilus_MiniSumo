#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdint.h>

uint8_t vl53l0x_init(uint8_t address);
uint16_t vl53l0x_read_distance(uint8_t address);

#endif
