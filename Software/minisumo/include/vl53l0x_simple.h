#ifndef VL53L0X_SIMPLE_H
#define VL53L0X_SIMPLE_H

#include <stdint.h>

void vl53l0x_init(void);
uint16_t vl53l0x_read_mm(void);

#endif
