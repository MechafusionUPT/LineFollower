#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

// ─── PID Controller ───────────────────────────────────────────────────────────
typedef struct
{
    double kp, ki, kd;
    double prev_error;
    double integral;
} PIDController;

/**
 * Initializeaza controlerul PID cu constantele date.
 * @param pid     Pointer catre structura PIDController
 * @param kp      Constanta proportionala
 * @param ki      Constanta integrala
 * @param kd      Constanta derivativa
 */
void   init_PID(PIDController *pid, double kp, double ki, double kd);

/**
 * Calculeaza corectia PID pentru un pas de timp dt.
 * @param pid            Pointer catre structura PIDController
 * @param setpoint       Valoarea dorita (de obicei 0)
 * @param measured_value Valoarea masurata (pozitia liniei)
 * @param dt             Timpul scurs de la ultimul apel [s]
 * @return               Corectia de aplicat motoarelor
 */
double update_PID(PIDController *pid, int setpoint, int measured_value, double dt);

#ifdef __cplusplus
}
#endif

#endif // PID_H
