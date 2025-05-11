//pin-urile digitale pentru motor stanga
#define L1 3
#define L2 4
#define LPWM 6
//pin-urile digitale pentru motor dreapta
#define R1 11
#define R2 12
#define RPWM 5

typedef struct
{
    double kp, ki, kd;
    int prev_error;
    int integral;
}PIDController;

int weightedSum=0; //pentru a calcula suma senzorilor
unsigned int lastTime=0;
int maxP=0, maxI=0, maxD=0;
PIDController pid;
double speed;
double bias;
int time;
int minSensor[7]={1023, 1023, 1023, 1023, 1023, 1023, 1023};
int maxSensor[7]={0,0,0,0,0,0,0};
int sensor[7];
float weights[7] = {3.2, 2, 1, 0, -1, -2, -3.2};
float totalWeight = 0;
float DISPERSION_THRESHOLD;

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
    //if(error>maxP) maxP=error;
    pid->integral += error * dt;
    pid->integral=constrain(pid->integral, -200, 200);
    /*Serial.print("P: ");
    Serial.println(error);
    Serial.print("maxP: ");
    Serial.println(maxP);
    Serial.print("I: ");
    Serial.println(pid->integral);*/
    double derivative = (error - pid->prev_error) / dt;
    if(derivative>maxD) maxD=derivative;
    /*Serial.print("D: ");
    Serial.println(derivative);
    Serial.print("maxD: ");
    Serial.println(maxD);*/
    double corectie = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    pid->prev_error = error;
    
    return corectie;
}

void setPowerL(float speed) //pentru motoare (NU SCHIMBA)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0.14)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, HIGH);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, pwmValue);
        }
        else if (speed < -0.14)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, LOW);
            digitalWrite(L2, HIGH);
            analogWrite(LPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(L1, LOW);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, 0);
        }
}
void setPowerR(float speed) //pentru motoare (NU SCHIMBA)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0.14)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, HIGH);
            digitalWrite(R2, LOW);
            analogWrite(RPWM, pwmValue);
        }
        else if (speed < -0.14)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, LOW);
            digitalWrite(R2, HIGH);
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
  //Serial.print("Calibrarea a fost finalizata!");
  delay(1000);
} 

//luam valorile de la senzori, facem media pentru PID si calculam dispersia + BIAS
int weightedValuePID(){
  weightedSum=0;
  for(int i=0; i<7; i++){
    sensor[i]=(map(analogRead(i+14), minSensor[i], maxSensor[i], 0, 100));
    weightedSum += sensor[i] * weights[i];
    totalWeight += sensor[i];
  }

  //calculam dispersia senzorilor fata de linie
  float com=0; //center of mass
  //suma senzorilor fara pondere
  for(int i=0; i<7; i++){
    totalWeight += sensor[i];
  }
  if(totalWeight>0) {
    com = weightedSum/totalWeight; //Pozitia medie a liniei
  }
  float dispersion = 0;
  if (totalWeight > 0) {
    for (int i = 0; i < 7; i++) {
        float diff = weights[i] - com;
        dispersion += sensor[i] * diff * diff;
    }
    dispersion /= totalWeight;
  }
  if(dispersion>DISPERSION_THRESHOLD){
    return 800;
  }
  

  //ALEGE PE CE PARTE E BIAS-UL 
  //stanga
  if(sensor[0]+sensor[3]>120 || sensor[0]+sensor[2]>120 || sensor[1]+sensor[3]>120){
    setPowerL(speed);
    setPowerR(speed+bias);
    delay(time);
  }
  //dreapta
  /*if(sensor[6]+sensor[3]>120 || sensor[6]+sensor[4]>120 || sensor[5]+sensor[3]>120){
    setPowerR(speed);
    setPowerL(speed+bias);
    delay(time);
  }*/
  else
    return constrain(weightedSum/6, -700, 700);
  /**/
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
  //Serial.begin(9600);

  //Mutati robotul pe deasupra liniei pentru 5 secunde (pana incep motoarele sa functioneze)
  sensorCalibration();

  //POT FI SCHIMBATE
  //D: 0.000018
  init_PID(&pid, 0.023, 0.0001, 0.000013);
  speed = 0.26;
  bias = 0.3; // Viteza cu care creste in cazul de giratoriu
  //35
  time=90; //timpul in care dam override la PID
  DISPERSION_THRESHOLD=2;
}

void loop() {
  //calculeaza timpul dintre loop-uri
  unsigned int now = millis();
  double dt = (now - lastTime) / 10000.0; 
  //Serial.print("DT: ");
  //Serial.println(dt, 10);
  lastTime = now;

  double A=update_PID(&pid, 0, weightedValuePID(), dt);
    if(A==800){ //impuls in fata
      setPowerL(speed);
      setPowerR(speed);
      delay(40);
    }
    else if(A>-0.03 && A<0.03){ //inainte
        setPowerL(speed);
        setPowerR(speed);
    }
    else if(A<0){ //dreapta
      setPowerL(speed); //L - low
      setPowerR(speed+A); // R - high
    }
    else{ //stanga
      setPowerL(speed-A); //L - high
      setPowerR(speed); // R - low
    }
    //Serial.println(A);
}