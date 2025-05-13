#define TRIG_PIN 9  // Pinul TRIG conectat la pinul 9 digital
#define ECHO_PIN 10 // Pinul ECHO conectat la pinul 10 digital

// Funcția pentru a măsura distanța
float citesteDistanta() {
  // Trimite un impuls de 10 microsecunde pentru a începe măsurarea
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Măsoară timpul pentru care semnalul se întoarce
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculează distanța în cm
  long distance = duration * 0.0344 / 2;
  
  return distance;
}

typedef struct //NU MODIFICA
{
    double kp, ki, kd;
    float prev_error;
    float integral;
}PIDController;

void init_PID(PIDController *pid, double kp, double ki, double kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->prev_error = 0.0;
    pid->integral = 0.0;
}

float update_PID(PIDController *pid, float setpoint, float measured_value, double dt) //DIFERIT DE CEL NORMAL
{
    float error = setpoint - measured_value;
    pid->integral += error * dt;
    pid->integral=constrain(pid->integral, -200, 200);
    float derivative = (error - pid->prev_error) / dt;
    float corectie = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    pid->prev_error = error;
    
    return corectie;
  }

PidController pidWall;
float targetPosition=10;

void setup() {
  pinMode(TRIG_PIN, OUTPUT); // Setează TRIG ca ieșire
  pinMode(ECHO_PIN, INPUT);  // Setează ECHO ca intrare
  init_PID(pidWall, 1, 0, 0); //Modificati Kp Ki Kd (D TREBUIE IGNORAT)
}

long lastDist=0;
void loop() {
  long dist = citesteDistanta();
  floatA=update_PID(pidWall, 10.0, dist)
}
