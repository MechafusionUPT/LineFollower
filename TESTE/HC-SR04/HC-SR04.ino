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

void setup() {
  Serial.begin(9600);   // Inițializează comunicarea serială
  pinMode(TRIG_PIN, OUTPUT); // Setează TRIG ca ieșire
  pinMode(ECHO_PIN, INPUT);  // Setează ECHO ca intrare
}

long lastDist=0;
void loop() {
  // Citește distanța și o afișează
  long dist = citesteDistanta();
  Serial.print(dist);
  if(dist>lastDist){
    //setPowerL(speedWall+0.04);
    //setPowerR(speedWall);
    Serial.println("Stanga");
  }
  else if(dist<lastDist){
    //setPowerL(speedWall);
    //setPowerR(speedWall+0.04);
    Serial.println("Dreapta");
  }
  else{
    //setPowerL(speeWall);
    //setPowerR(speedWall);
    Serial.println("Inainte");
  }
  lastDist=dist;
  delay(100);// Pauză de 500 ms înainte de următoarea măsurătoare
}
