#include "sensors.h"
#include "motor.h"
#include "config.h"
#include <Arduino.h>

// ─── Starea interna a senzorilor ──────────────────────────────────────────────
static int   minSensor[SENSOR_COUNT] = {1023, 1023, 1023, 1023, 1023, 1023, 1023};
static int   maxSensor[SENSOR_COUNT] = {0, 0, 0, 0, 0, 0, 0};
static int   sensor[SENSOR_COUNT];

// Greutatile pozitionale: stanga pozitiva, dreapta negativa
static const float weights[SENSOR_COUNT] = {3.6f, 2.0f, 1.0f, 0.0f, -1.0f, -2.0f, -3.6f};

// Ultima pozitie cunoscuta — folosita la recuperare linie pierduta
static int lastPosition = 0;

static unsigned long circleEnterTime = 0;  // moment intrare in cerc

// ─── Init pini ────────────────────────────────────────────────────────────────
void sensors_init(void)
{
    for (int i = 0; i < SENSOR_COUNT; i++) {
        pinMode(SENSOR_BASE_PIN + i, INPUT);
    }
}

// ─── Calibrare ────────────────────────────────────────────────────────────────
void sensorCalibration(void)
{
    while (millis() < CALIBRATION_MS) {
        for (int i = 0; i < SENSOR_COUNT; i++) {
            int val = analogRead(SENSOR_BASE_PIN + i);
            if (val > maxSensor[i]) maxSensor[i] = val;
            if (val < minSensor[i]) minSensor[i] = val;
        }
    }
    delay(1000);
}

// ─── Verifica daca linia e pierduta ───────────────────────────────────────────
static int isLineLost(void)
{
    int total = 0;
    for (int i = 0; i < SENSOR_COUNT; i++)
        total += sensor[i];
    return (total < LOST_LINE_THRESHOLD);
}

// ─── Citire + suma ponderata ──────────────────────────────────────────────────
int weightedValuePID(void)
{
    float weightedSum = 0.0f;
    float totalWeight = 0.0f;

    for (int i = 0; i < SENSOR_COUNT; i++) {
        int raw = analogRead(SENSOR_BASE_PIN + i);
        sensor[i] = (int)((float)(raw - minSensor[i])
                    / (float)(maxSensor[i] - minSensor[i]) * 100.0f);
        if      (sensor[i] > 100) sensor[i] = 100;
        else if (sensor[i] < 0)   sensor[i] = 0;

        weightedSum += sensor[i] * weights[i];
        totalWeight += sensor[i];
    }

    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    
    int result = (totalWeight > 0) ? (int)(weightedSum / totalWeight * 100.0f) : lastPosition;
    if      (result >  700) result =  700;
    else if (result < -700) result = -700;

    lastPosition = result;
    return result;
}

// ─── Getter pentru lastPosition ───────────────────────────────────────────────
int getLastPosition(void)
{
    return lastPosition;
}