#include "sensors.h"
#include "motor.h"
#include "config.h"
#include "hc04.h" // Added for non-blocking distance reads
#include <Arduino.h>

// ─── Starea interna a senzorilor ──────────────────────────────────────────────
static int   minSensor[SENSOR_COUNT] = {1023, 1023, 1023, 1023, 1023, 1023, 1023};
static int   maxSensor[SENSOR_COUNT] = {0, 0, 0, 0, 0, 0, 0};
static int   sensor[SENSOR_COUNT];

// Greutatile pozitionale: stanga pozitiva, dreapta negativa
static const float weights[SENSOR_COUNT] = {3.6f, 2.0f, 1.0f, 0.0f, -1.0f, -2.0f, -3.6f};

// Ultima pozitie cunoscuta — folosita la recuperare linie pierduta
static int lastPosition = 0;

// ─── Stare masina ─────────────────────────────────────────────────────────────
typedef enum {
    STATE_NORMAL,
    STATE_INSIDE_CIRCLE,
    STATE_WALL_FOLLOW
} RobotState;

static RobotState currentState    = STATE_NORMAL;
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

// ─── Detectie intersectie stanga ──────────────────────────────────────────────
static int isIntersection(void)
{
    int leftZone   = (sensor[0] + sensor[1])/2;
    int centerZone = (sensor[2] + sensor[3] + sensor[4])/3;
    int rightZone  = (sensor[5] + sensor[6])/2;

    return centerZone > CENTER_ZONE;
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

    static int lostCount = 0;
    static int regainCount = 0;

    // ── Linie pierduta -> Trecere in Wall Follow ──────────────────────────────
    // Verificam isLineLost() doar daca urmam linia, ca sa nu incrementam cand suntem deja la perete
    if (currentState != STATE_WALL_FOLLOW) {
        if (isLineLost()) {
            lostCount++;
            if (lostCount >= LOST_CONSECUTIVE_READS && hc04_read_mm() < MAX_VALID_WALL_DIST) {
                currentState = STATE_WALL_FOLLOW;
                lostCount = 0;
            }
            return lastPosition; // Mentinem ultima pozitie cat se decide starea
        } else {
            lostCount = 0;
        }
    }

    // ── Stare masina: NORMAL vs INSIDE_CIRCLE vs WALL_FOLLOW ──────────────────
    if (currentState == STATE_NORMAL)
    {
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, LOW);
        if (isIntersection())
        {
            currentState    = STATE_INSIDE_CIRCLE;
            circleEnterTime = millis();

            setPowerL(BASE_SPEED);
            setPowerR(BASE_SPEED + CIRCLE_ENTRY_BIAS);
            delay(CIRCLE_ENTRY_TIME_MS);
            return 0;
        }
    }
    else if (currentState == STATE_INSIDE_CIRCLE)
    {
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, HIGH);
        unsigned long timeInCircle = millis() - circleEnterTime;

        if (timeInCircle >= CIRCLE_MIN_TIME_MS && isIntersection())
        {
            currentState = STATE_NORMAL;

            setPowerL(BASE_SPEED);
            setPowerR(BASE_SPEED + CIRCLE_EXIT_BIAS);
            delay(CIRCLE_EXIT_TIME_MS);
            return 0;
        }
    }
    else if (currentState == STATE_WALL_FOLLOW)
    {
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        // 1. Conditia de iesire: am dat de linie cu o intensitate buna?
        int centerZone = sensor[2] + sensor[3] + sensor[4];
        
        if (centerZone > REGAIN_CENTER_THRESHOLD) {
            regainCount++;
            if (regainCount >= REGAIN_CONSECUTIVE_READS) {
                currentState = STATE_NORMAL;
                regainCount = 0;
                return 0; // Returnam 0 ca sa re-centram PID-ul liniei
            }
        } else {
            regainCount = 0;
        }

        // 2. Mentinem peretele daca nu am iesit
        if (currentState == STATE_WALL_FOLLOW) {
            uint32_t dist = hc04_read_mm();
            
            if (dist > MAX_VALID_WALL_DIST) dist = MAX_VALID_WALL_DIST;

            // P-controller pt perete - Presupunem ca citim peretele in DREAPTA
            // dist > 100 => eroare > 0 => trebuie sa viram dreapta
            float error = (float)dist - TARGET_WALL_DIST_MM;
            float wallAdjust = error * KP_WALL; 

            setPowerL(BASE_SPEED_WALL + wallAdjust);
            setPowerR(BASE_SPEED_WALL - wallAdjust);

            return 0; 
        }
    }

    // ── Pozitie normalizata (Doar pt. Normal/Circle) ──────────────────────────
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