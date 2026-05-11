#ifndef HC04_H
#define HC04_H

#include <stdint.h>

// Initializes the sensor pins and attaches the interrupt
void hc04_init(int trigPin, int echoPin);

// Triggers a pulse (max once every 40ms) and returns the last known distance
uint32_t hc04_read_mm(void);

#endif