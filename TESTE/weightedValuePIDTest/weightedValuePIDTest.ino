// Configurare 

// Numărul total de senzori
#define NUM_SENSORS 14

// Prag pentru detectarea liniei
const int threshold = 1000; // Ajustează acest prag în funcție de condițiile suprafeței
int weightedSum=0; //pentru a calcula suma senzorilor

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
  weightedSum += 4 * analogRead(14);
  weightedSum=0;
  for(int i=15; i<17; i++){
    weightedSum += (17-i) * analogRead(i);
  }
    weightedSum -= 4 * analogRead(18);
  for(int i=19; i<21; i++){
    weightedSum -= (i-17) * analogRead(i);
  }
  return constrain(weightedSum/6, -700, 700);
}


int update_PID(PIDController *pid, int setpoint, int measured_value, double dt) {
  double error = setpoint - measured_value;
  pid->integral += error * dt;
  double derivative = (error - pid->prev_error) / dt;
  double output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
  pid->prev_error = error;
}

