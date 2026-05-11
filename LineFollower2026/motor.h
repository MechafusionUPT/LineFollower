#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializeaza pinii motoarelor ca OUTPUT.
 * Apelati o singura data in setup().
 */
void motor_init(void);

/**
 * Seteaza puterea motorului STANGA.
 * @param speed  Viteza in intervalul [-1.0, 1.0]
 *               > 0  : inainte
 *               < 0  : inapoi
 *               ~0   : stop (deadband +/-0.14)
 */
void setPowerL(float speed);

/**
 * Seteaza puterea motorului DREAPTA.
 * @param speed  Viteza in intervalul [-1.0, 1.0]
 */
void setPowerR(float speed);

/**
 * Opreste ambele motoare imediat.
 */
void motor_stop(void);

#ifdef __cplusplus
}
#endif

#endif // MOTOR_H
