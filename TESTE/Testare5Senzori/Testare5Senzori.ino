//alb~40 negru~800-1000
void setup() {
  Serial.begin(9600);
  pinMode(A6, INPUT); //Senzor 0
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  //pinMode(A5, INPUT);
}

//verificare toti senzori
/*void loop() {
  for(int pin=0; pin<7; pin++)
  { 
    int val = analogRead(pin);
    Serial.print(val);
    Serial.print(" | ");
    delay(10);
  }
  delay(500);
  Serial.println();
}*/

//verificare 1 senzor
void loop(){

    int val = analogRead(A7);//A0, A1... A6
    Serial.println(val);
    delay(100);

    Serial.println();

}