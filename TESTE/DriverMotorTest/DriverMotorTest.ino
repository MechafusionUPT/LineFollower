//pin-urile digitale pentru motor stanga
#define LDIR 1
#define LPWM 1

#define BI0 1
#define BI1 1
//pin-urile digitale pentru motor dreapta
#define RDIR 12
#define RPWM 11
#define AI0 7
#define AI1 8


void setup() {
  //Configurare pini motoare
  //Dreapta
  pinMode(RDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(AI0, OUTPUT);
  pinMode(AI1, OUTPUT);
  digitalWrite(AI0, 0);
  digitalWrite(AI1, 0);
  //Stanga
  pinMode(LDIR, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(BI0, OUTPUT);
  pinMode(BI1, OUTPUT);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  
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

void setPowerR(float speed, unsigned int currentAI) //currentAI trebuie sa fie 0,1,2,3
{
        digitalWrite(AI0, currentAI%2);
        digitalWrite(AI1, currentAI>>1%2);
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

void setPowerL(float speed) // viteza pins:in1 in2 pwm
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

void loop() {
  // put your main code here, to run repeatedly:
  setPowerR(0.4);
  Serial.print("sugi pula");
  Serial.println();
}
