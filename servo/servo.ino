#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin_angle = 0;// analog pin used to connect the potentiometer
int val_angle;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void setServo() {
  val_angle = analogRead(potpin_angle);
 val_angle = map(val_angle, 0, 1023, 0, 180);
  myservo.write(val_angle);
  val_angle = map(val_angle, 0, 180, 0, 200);
  Serial.write(val_angle);
  delay(5);

}

void loop() {
  setServo();
}
