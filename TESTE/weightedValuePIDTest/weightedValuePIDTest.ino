// Prag pentru detectarea liniei
int weightedSum=0; //pentru a calcula suma senzorilor
int minSensor[7]={1023, 1023, 1023, 1023, 1023, 1023, 1023};
int maxSensor[7]={0,0,0,0,0,0,0};
int sensor[7];
float weights[7] = {3.2, 2, 1, 0, -1, -2, -3.2};


void setup() {
  // Configurare pini
  pinMode(A6, INPUT); //Senzor 0
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  // Pornire comunicare serială
  Serial.begin(9600);
  Serial.println("Incep testarea senzorilor...");
}

void loop() {
  Serial.println(weightedValuePID());
  delay(100);
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
  for(int i=0; i<7; i++){
    sensor[i]=(map(analogRead(i+14), minSensor[i], maxSensor[i], 0, 100));
    weightedSum += sensor[i] * weights[i];
    totalWeight += sensor[i];
  }
  return constrain(weightedSum/6, -700, 700);
}

