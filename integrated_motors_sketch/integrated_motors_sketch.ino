/*
* Carnegie Mellon University Mechatronics Spring 2022: Team E
* Motors Lab Code
* Authors: Felipe Borja (fborja)
*          Brandon Wang (bcwang)
*          Jaiden Napier (jinapier)
*          Ignacio Peon Zapata (ipeon)
*          Sahil Saini (sssaini)
*/

// Pin assignments

void setup() {
  Serial.begin(9600);
  Serial.print("Hi :)");
}

void loop() {
  if (Serial.available())
  {
    char c = Serial.read();
    flagSwitch(c);
  }
}

void flagSwitch(char c) {
  switch (c) {
    case 'd':
      driveDC();
      break;
    case 'v':
      driveServo();
      break;
    case 'p':
      driveStepper();
      break;
    default:
      Serial.print("DO NOT TYPE HERE!!!");
  }
}

// Basic LED Test. Replace functions with code for each motor

void driveDC() {
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
}
void driveServo() {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}
void driveStepper() {
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(3, LOW);
}
