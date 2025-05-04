void setup() {
  Serial.begin(9600);
  pinMode(A6, INPUT); //Senzor 0
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  for(int pin=14; pin<21; pin++)
  { 
    int val = analogRead(pin);
    Serial.print(val);
    Serial.print(" | ");
    delay(10);
  }
  delay(500);
  Serial.println();
}


/*void loop(){

    int val = analogRead(A6);
    Serial.print(val);
    Serial.print(" ");
    delay(100);

    Serial.println();

}*/