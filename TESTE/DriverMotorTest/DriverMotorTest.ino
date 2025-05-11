//pin-urile digitale pentru motor stanga
#define L1 3
#define L2 4
#define LPWM 6

//pin-urile digitale pentru motor dreapta
#define R1 11
#define R2 12
#define RPWM 5



void setup() {
  //Configurare pini motoare
  //Dreapta
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(RPWM, OUTPUT);
  
  //Stanga
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LPWM, OUTPUT);

  
  
  //set internal current pulldown
  //00-100%
  //01-71%
  //10-38%
  //11-0%

  //Serial
  Serial.begin(9600);
}

void setPowerR(float speed)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, LOW);
            digitalWrite(R2, HIGH);
            analogWrite(RPWM, pwmValue);
        }
        else if (speed < 0)
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


void setPowerL(float speed) // viteza pins:in1 in2 pwm
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

void loop() {
  // put your main code here, to run repeatedly:
  //setPowerL(0.5);
  //setPowerR(0.5);
  //digitalWrite(L1, HIGH);
  //digitalWrite(L2, LOW);
  //analogWrite(LPWM, 50);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  analogWrite(RPWM, 50);
  //digitalWrite(RPWM, HIGH);
  
  Serial.print("merge");
  Serial.println();
}
