#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializeaza pinii senzorilor ca INPUT.
 * Apelati o singura data in setup().
 */
void sensors_init(void);

/**
 * Calibreaza toti senzorii timp de CALIBRATION_MS milisecunde.
 * Mutati robotul pe deasupra liniei in toata aceasta perioada.
 * Dupa calibrare, asteapta 1 secunda inainte sa continue.
 */
void sensorCalibration(void);

/**
 * Citeste toti senzorii, calculeaza suma ponderata si detecteaza intersectii.
 * La intersectie stanga, aplica override pe motoare si returneaza 0.
 * Daca linia e pierduta, activeaza recuperarea si returneaza ultima pozitie.
 * @return  Pozitia liniei in intervalul [-700, 700]
 */
int weightedValuePID(void);

/**
 * Returneaza ultima pozitie valida a liniei.
 * Folosit in main pentru speed scaling bazat pe eroare.
 */
int getLastPosition(void);

#ifdef __cplusplus
}
#endif

#endif // SENSORS_H