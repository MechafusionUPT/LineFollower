//pin-urile digitale pentru motor stanga
#define LDIR 4
#define LPWM 3
//pin-urile digitale pentru motor dreapta
#define RDIR 12
#define RPWM 11

typedef struct
{
    double kp, ki, kd;
    int prev_error;
    int integral;
} PIDController;

int weightedSum=0; //pentru a calcula suma senzorilor
unsigned int lastTime=0;
PIDController pid;
double speed;

void init_PID(PIDController *pid, double kp, double ki, double kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->prev_error = 0.0;
    pid->integral = 0.0;
}

double update_PID(PIDController *pid, int setpoint, int measured_value, double dt)
{
    int error = setpoint - measured_value;
    pid->integral += error * dt;
    int derivative = (error - pid->prev_error) / dt;
    int corectie = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    pid->prev_error = error;
    return corectie;
}

void setPowerL(double speed) // viteza pins:in1 in2 pwm
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(LDIR, LOW);
            analogWrite(LPWM, pwmValue);
        }
        else if (speed < 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(LDIR, HIGH);
            analogWrite(LPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(LDIR, LOW);
            analogWrite(LPWM, 0);
        }
}
void setPowerR(double speed)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(RDIR, LOW);
            analogWrite(RPWM, pwmValue);
        }
        else if (speed < 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(RDIR, HIGH);
            analogWrite(RPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(RDIR, LOW);
            analogWrite(RPWM, 0);
        }
}

//luam valorile de la senzori, facem media pentru PID
int weightedValuePID(){
  weightedSum=0;
  for(int i=14; i<17; i++){
    weightedSum += (17-i) * analogRead(i);
  }
  
  for(int i=18; i<21; i++){
    weightedSum -= (i-17) * analogRead(i);
  }
  return constrain(weightedSum/6, -700, 700);
}

void setup() {
  // Configurare pini
  pinMode(A6, INPUT); //Senzor 0
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  //Configurare pini motoare
  //Stanga
  pinMode(LDIR, OUTPUT);
  pinMode(LPWM, OUTPUT);
  //Dreapta
  pinMode(RDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);

  init_PID(&pid, 1, 0, 0);

  speed=0.5;
  //Serial
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int now = micros();
  double dt = (now - lastTime) / 1000.0; // Convertim în secunde
  lastTime = now;

  double A=update_PID(&pid, 0, weightedValuePID(), dt)/400;

  if(abs(A)<100 && analogRead(A3)>200){
    setPowerR(1);
    setPowerL(1);
  }
  else{
    if(A<0){
      setPowerL(speed-A);
      setPowerR(speed);
    }
    else{
      setPowerL(speed);
      setPowerR(speed+A);
    }
  }
  Serial.println(A);
  delay(100);

}
