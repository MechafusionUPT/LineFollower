#include "motor.h"
#include "config.h"
#include <Arduino.h>

void motor_init(void)
{
    // Stanga
    pinMode(L1,   OUTPUT);
    pinMode(L2,   OUTPUT);
    pinMode(LPWM, OUTPUT);
    // Dreapta
    pinMode(R1,   OUTPUT);
    pinMode(R2,   OUTPUT);
    pinMode(RPWM, OUTPUT);
}

// ─── Motor Stanga ─────────────────────────────────────────────────────────────
void setPowerL(float speed)
{
    speed = constrain(speed, -1.0f, 1.0f);

    if (speed > MOTOR_DEADBAND)
    {   // Inainte
        int pwmValue = (int)(speed * 255);
        digitalWrite(L1, HIGH);
        digitalWrite(L2, LOW);
        analogWrite(LPWM, pwmValue);
    }
    else if (speed < -MOTOR_DEADBAND)
    {   // Inapoi
        int pwmValue = (int)(-speed * 255);
        digitalWrite(L1, LOW);
        digitalWrite(L2, HIGH);
        analogWrite(LPWM, pwmValue);
    }
    else
    {   // Stop
        digitalWrite(L1, LOW);
        digitalWrite(L2, LOW);
        analogWrite(LPWM, 0);
    }
}

// ─── Motor Dreapta ────────────────────────────────────────────────────────────
void setPowerR(float speed)
{
    speed = constrain(speed, -1.0f, 1.0f);

    if (speed > MOTOR_DEADBAND)
    {   // Inainte
        int pwmValue = (int)(speed * 255);
        digitalWrite(R1, HIGH);
        digitalWrite(R2, LOW);
        analogWrite(RPWM, pwmValue);
    }
    else if (speed < -MOTOR_DEADBAND)
    {   // Inapoi
        int pwmValue = (int)(-speed * 255);
        digitalWrite(R1, LOW);
        digitalWrite(R2, HIGH);
        analogWrite(RPWM, pwmValue);
    }
    else
    {   // Stop
        digitalWrite(R1, LOW);
        digitalWrite(R2, LOW);
        analogWrite(RPWM, 0);
    }
}

// ─── Stop imediat ─────────────────────────────────────────────────────────────
void motor_stop(void)
{
    setPowerL(0.0f);
    setPowerR(0.0f);
}
