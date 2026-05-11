#include "hc04.h"
#include <Arduino.h>

static int _trig;
static int _echo;
static volatile unsigned long _pulseStart = 0;
static volatile uint32_t _currentDist_mm = 999; // Default safe distance

// ISR: Triggered whenever the Echo pin changes state
void echo_isr(void) {
    if (digitalRead(_echo) == HIGH) {
        // Echo pin just went HIGH, pulse started returning
        _pulseStart = micros();
    } else {
        // Echo pin went LOW, pulse ended
        unsigned long duration = micros() - _pulseStart;
        
        // Speed of sound = 343 m/s = 0.343 mm/us
        // Distance = (duration * 0.343) / 2 = (duration * 343) / 2000
        _currentDist_mm = (duration * 343) / 2000;
    }
}

void hc04_init(int trigPin, int echoPin) {
    _trig = trigPin;
    _echo = echoPin;
    
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
    
    // Attach the interrupt to run the ISR whenever the echo pin changes
    attachInterrupt(digitalPinToInterrupt(_echo), echo_isr, CHANGE);
}

uint32_t hc04_read_mm(void) {
    static unsigned long lastTriggerTime = 0;
    
    // Rate limiter: Don't trigger faster than 40ms to avoid overlapping echoes
    if (millis() - lastTriggerTime > 40) {
        lastTriggerTime = millis();
        
        // Non-blocking trigger: Send 10us pulse
        digitalWrite(_trig, LOW);
        delayMicroseconds(2);
        digitalWrite(_trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(_trig, LOW);
    }
    
    return _currentDist_mm;
}