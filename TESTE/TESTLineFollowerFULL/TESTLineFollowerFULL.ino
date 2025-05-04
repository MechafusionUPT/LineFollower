//pin-urile digitale pentru motor stanga
#define LDIR
#define LPWM
//pin-urile digitale pentru motor dreapta
#define RDIR
#define RPWM

//pini pentru multiplexor
#define SEL0 2
#define SEL1 3
#define SEL2 4
#define SEL3 5
//pin de semnal analog pentru semnalul de la senzori
#define SIG A0
// Numărul total de senzori
#define NUM_SENSORS 14
#define STANGA 0
#define DREAPTA 1

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
            analogWrite(LPWM, pwmValue;
        }
        else
        { // Oprire
            digitalWrite(LDIR, LOW);
            analogWrite(LPWM, 0);
        }
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

void selectChannel(int channel) {
  digitalWrite(SEL0, channel & 0x01);
  digitalWrite(SEL1, (channel >> 1) & 0x01);
  digitalWrite(SEL2, (channel >> 2) & 0x01);
  digitalWrite(SEL3, (channel >> 3) & 0x01);
}

void setup() {
  // Configurare pini senzor
  pinMode(SEL0, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SEL3, OUTPUT);
  pinMode(SIG, INPUT);

  //Configurare pini motoare
  //Stanga
  pinMode(LDIR, OUTPUT);
  pinMode(LPWM, OUTPUT);
  //Dreapta
  pinMode(RDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);

  //Serial
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
