#ifndef CONFIG_H
#define CONFIG_H

// ─── Motor pins - Stanga (Left) ───────────────────────────────────────────────
#define L1   10
#define L2   4
#define LPWM 3

// ─── Motor pins - Dreapta (Right) ─────────────────────────────────────────────
#define R1   5
#define R2   7
#define RPWM 6

// ---- HC04 - Ultrasound –––––––––––––––––––––––––––––––––––––––––––––––––––––––
#define ECHO
#define TRIG

// ─── Sensor pins (A0-A6) ──────────────────────────────────────────────────────
#define SENSOR_COUNT     7
#define SENSOR_BASE_PIN  15   // A0 = pin 14 on Nano ESP32

// ––– LED pins –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
#define GREEN 8
#define RED 9

// ─── PID tuning ───────────────────────────────────────────────────────────────
// #define PID_KP  0.003
// #define PID_KI  0.00008
// #define PID_KD  0.00075
#define PID_KP  0.003
#define PID_KI  0
#define PID_KD  0.000001

// ─── Motion parameters ────────────────────────────────────────────────────────
#define BASE_SPEED        0.5f   // viteza de baza [0.0 - 1.0]
#define MOTOR_DEADBAND    0.14f  // sub aceasta valoare motorul sta pe loc
#define OVERRIDE_TIME_MS  70     // ms de override PID la intrare cerc

// ─── Intersection / Circle tuning ─────────────────────────────────────────────
#define CIRCLE_ENTRY_BIAS     0.2f   // bias la intrarea in cerc (era INTERSECTION_BIAS)
#define CIRCLE_ENTRY_TIME_MS  120     // durata bias intrare [ms]
#define CIRCLE_EXIT_BIAS      0.4f   // bias mai puternic la iesire
#define CIRCLE_EXIT_TIME_MS   200   // durata bias iesire [ms] — mai lung decat intrare
#define CIRCLE_MIN_TIME_MS    500    // timp minim in cerc inainte sa permitem iesirea

// ─── PID output deadband ──────────────────────────────────────────────────────
#define PID_DEADBAND      0.05

// ─── Sensor calibration window ────────────────────────────────────────────────
#define CALIBRATION_MS    3000

// ─── Lost line recovery ───────────────────────────────────────────────────────
#define LOST_LINE_THRESHOLD   30
#define RECOVERY_SPIN_SPEED   0.35f

// --- Intersection Thresholds

#define LEFT_ZONE 90
#define RIGHT_ZONE 90
#define CENTER_ZONE 90

// ─── Speed scaling ────────────────────────────────────────────────────────────
#define SPEED_SCALE           0.0002f
#define MIN_SPEED             0.45f

// ── Wall Follow Constants ──────────────────────────────
#define LOST_CONSECUTIVE_READS    10
#define REGAIN_CONSECUTIVE_READS  3
#define REGAIN_CENTER_THRESHOLD   150  // Sum of central sensors (S2+S3+S4) to consider line regained

#define TARGET_WALL_DIST_MM       100  // Target distance from the wall in mm
#define MAX_VALID_WALL_DIST       400  // Ignore sudden 999mm readings
#define BASE_SPEED_WALL           100  // Base speed while following wall
#define KP_WALL                   1.5f // P-constant for wall following adjustment

#endif // CONFIG_H