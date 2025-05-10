//pin-urile digitale pentru motor stanga
#define L1 3
#define L2 4
#define LPWM 5
//pin-urile digitale pentru motor dreapta
#define R1 11
#define R2 12
#define RPWM 6

typedef struct
{
    double kp, ki, kd;
    int prev_error;
    int integral;
} PIDController;

int weightedSum=0; //pentru a calcula suma senzorilor
unsigned int lastTime=0;
int maxP=0, maxI=0, maxD=0;
PIDController pid;
double speed=0.13;
int minSensor[7]={1023, 1023, 1023, 1023, 1023, 1023, 1023};
int maxSensor[7]={0,0,0,0,0,0,0};
int sensor[7];

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
    if(error>maxP) maxP=error;
    pid->integral += error * dt;
    pid->integral=constrain(pid->integral, -500, 500);
    Serial.print("P: ");
    Serial.println(error);
    Serial.print("maxP: ");
    Serial.println(maxP);
    Serial.print("I: ");
    Serial.println(pid->integral);
    int derivative = (error - pid->prev_error) / dt;
    if(derivative>maxD) maxD=derivative;
    Serial.print("D: ");
    Serial.println(derivative);
    Serial.print("maxD: ");
    Serial.println(maxD);
    int corectie = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    pid->prev_error = error;
    return corectie;
}

void setPowerR(float speed) //pentru motoare (NU SCHIMBA)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, LOW);
            digitalWrite(L2, HIGH);
            analogWrite(LPWM, pwmValue);
        }
        else if (speed < 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, HIGH);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(L1, LOW);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, 0);
        }
}
void setPowerL(float speed) //pentru motoare (NU SCHIMBA)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed < 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, LOW);
            digitalWrite(R2, HIGH);
            analogWrite(RPWM, pwmValue);
        }
        else if (speed > 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, HIGH);
            digitalWrite(R2, LOW);
            analogWrite(RPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(R1, LOW);
            digitalWrite(R2, LOW);
            analogWrite(RPWM, 0);
        }
}

void sensorCalibration(){
  while(millis()<5000){
    for(int i=0; i<7; i++){
      int aux=analogRead(14+i);
      if(aux>maxSensor[i])
        maxSensor[i]=aux;
      if(aux<minSensor[i])
        minSensor[i]=aux;
    }
  }
  Serial.print("Calibrarea a fost finalizata!");
  delay(1000);
}

//luam valorile de la senzori, facem media pentru PID
int weightedValuePID(){
  weightedSum=0;
  for(int i=14; i<17; i++){
    weightedSum += (17-i) * (map(analogRead(i), minSensor[i-14], maxSensor[i-14], 40, 700));
  }
  
  for(int i=18; i<21; i++){
    weightedSum -= (i-17) * (map(analogRead(i), minSensor[i-14], maxSensor[i-14], 40, 700));
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
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LPWM, OUTPUT);
  //Dreapta
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(RPWM, OUTPUT);

  //Serial
  Serial.begin(9600);

  //Mutati robotul pe deasupra liniei pentru 5 secunde (pana incep motoarele sa functioneze)
  sensorCalibration();

  //0.25 0 0.12
  init_PID(&pid, 0.003, 0.00002, 0.00007);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  unsigned int now = micros();
  double dt = (now - lastTime) / 100000.0; 
  lastTime = now;

  double A=update_PID(&pid, 0, weightedValuePID(), dt);

  /*if(abs(A)<100 && (analogRead(A3)-threshold[3])>200){
    setPowerR(0.1);
    setPowerL(0.1);
    Serial.print("Inainte");
  }
  else{*/
    if(A<0){
      setPowerL(speed+A); //L - high
      setPowerR(speed); // R - low
      Serial.print("Dreapta");
    }
    else{
      setPowerL(speed); //L - low
      setPowerR(speed-A); // R - high
       Serial.print("Stanga");
    }
  
  Serial.println(A);

}
