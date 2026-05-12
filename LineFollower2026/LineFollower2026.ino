#include <Arduino.h>
#include "config.h"
#include "pid.h"
#include "motor.h"
#include "sensors.h"

// ─── Stare globala ────────────────────────────────────────────────────────────
static PIDController pid;
static unsigned int  lastTime = 0;
static unsigned int  lastTimePID = 0;
int position = 0;
double correction = 0;

// ─── Speed scaling ────────────────────────────────────────────────────────────
// Returneaza viteza redusa in functie de eroarea curenta
// La eroare 0 (drept)  -> BASE_SPEED
// La eroare mare (curba) -> viteza scade pana la MIN_SPEED
static float scaledSpeed(double correction)
{
    float absError = (float)(correction < 0 ? -correction : correction);
    float s = BASE_SPEED - (absError * SPEED_SCALE);
    if (s < MIN_SPEED) s = MIN_SPEED;
    return s;
}

// ─────────────────────────────────────────────────────────────────────────────
void setup()
{
    Serial.begin(9600);

    pinMode(GREEN, OUTPUT);
    pinMode(RED, OUTPUT);

    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, HIGH);

    sensors_init();
    motor_init();

    sensorCalibration();

    init_PID(&pid, PID_KP, PID_KI, PID_KD);    
}

// ─────────────────────────────────────────────────────────────────────────────
void loop()
{
    unsigned int now = millis();
    double dt = (now - lastTime) / 10000.0;

    position   = weightedValuePID();
    if(now - lastTimePID > 50){
        correction = update_PID(&pid, 0, position, dt);
        lastTimePID = now;
    }
    

    lastTime  = now;

    // Daca suntem in recuperare (linie pierduta), weightedValuePID() a aplicat
    // deja comenzile motoarelor — iesim fara sa apelam PID
    // (detectam recuperarea: motoarele sunt deja setate in sensors.c)
    // Totusi lasam PID-ul sa ruleze si in recuperare — va mentine integral-ul cald
    
    float  dynSpeed   = scaledSpeed(correction);

    if (correction > -PID_DEADBAND && correction < PID_DEADBAND)
    {
        // Mers drept — viteza maxima
        setPowerL(BASE_SPEED);
        setPowerR(BASE_SPEED);
    }
    else if (correction < 0)
    {
        // Vireaza stanga: micsoram dreapta, aplicam speed scaling
        setPowerL(dynSpeed);
        setPowerR(dynSpeed + (float)correction);
    }
    else
    {
        // Vireaza dreapta: micsoram stanga, aplicam speed scaling
        setPowerL(dynSpeed - (float)correction);
        setPowerR(dynSpeed);
    }

    // Serial.println(correction);
    // Serial.println();
}