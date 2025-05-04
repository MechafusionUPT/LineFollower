// Configurare pini pentru multiplexor
#define SEL0 2
#define SEL1 3
#define SEL2 4
#define SEL3 5
#define SIG A0

// Numărul total de senzori
#define NUM_SENSORS 14

// Prag pentru detectarea liniei
const int threshold = 1000; // Ajustează acest prag în funcție de condițiile suprafeței
int weightedSum=0; //pentru a calcula suma senzorilor

int right[7]={11,9,10,8,3,1,2};
int left[7]={4,6,5,7,12,14,13};

int k=4; //de schimbat

void setup() {
  // Configurare pini
  pinMode(A6, INPUT); //Senzor 0
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Pornire comunicare serială
  Serial.begin(9600);
  Serial.println("Incep testarea senzorilor...");
}

void loop() {
  Serial.println(weightedValuePID());
  delay(100);
}

//luam valorile de la senzori, facem media pentru PID
int weightedValuePID(){
  weightedSum=0;
  for(int i=14; i<17; i++){
    weightedSum +=  analogRead(i);
  }
  for(int i=18; i<21; i++){
    weightedSum -=  analogRead(i);
  }
  return weightedSum/6;
}

/*int update_PID(PIDController *pid, int setpoint, int measured_value, double dt) {
  double error = setpoint - measured_value;
  pid->integral += error * dt;
  double derivative = (error - pid->prev_error) / dt;
  double output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
  pid->prev_error = error;
}*/
