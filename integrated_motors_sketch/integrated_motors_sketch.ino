/*
* Carnegie Mellon University Mechatronics Spring 2022: Team E
* Motors Lab Code
* Authors: Felipe Borja (fborja)
*          Brandon Wang (bcwang)
*          Jaiden Napier (jinapier)
*          Ignacio Peon Zapata (ipeon)
*          Sahil Saini (sssaini)
*/

#include <Servo.h>

// Servo variables
Servo myservo;  // create servo object to control a servo
int potpin_angle = 0;// analog pin used to connect the potentiometer
int val_angle;    // variable to read the value from the analog pin

// Pin assignments

void setup() {
  Serial.begin(9600);
  Serial.print("Hi :)");

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  if (Serial.available())
  {
    char c = Serial.read();
    flagSwitch(c);
  }
  setServo();
}

void flagSwitch(char c) {
  switch (c) {
    case 'D':
      driveDC_GUI();
      break;
    case 'd':
      driveDC_sensor();
      break;
    case 'V':
      driveServo_GUI();
      break;
    case 'v':
      driveServo_sensor();
      break;
    case 'P':
      driveStepper_GUI();
      break;
    case 'p':
      driveStepper_sensor();
      break;
    default:
      Serial.print("DO NOT TYPE HERE!!!");
  }
}

// Basic LED Test. Replace functions with code for each motor

void driveDC_GUI() {
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
}
void driveDC_sensor() {
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
}

void driveServo_GUI() {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}
void driveServo_sensor() {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}

void driveStepper_GUI() {
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(3, LOW);
}
void driveStepper_sensor() {
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(3, LOW);
}


// Sahil's Servo Code

void setServo() {
  val_angle = analogRead(potpin_angle);
  val_angle = map(val_angle, 0, 1023, 0, 180);
  myservo.write(val_angle);
  val_angle = map(val_angle, 0, 180, 0, 200);
  Serial.write(val_angle);
  delay(5);

}
