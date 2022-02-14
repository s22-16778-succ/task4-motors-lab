#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin_angle = 0;// analog pin used to connect the potentiometer
int potpin_speed = 0;
int val_angle;    // variable to read the value from the analog pin
int val_speed;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  val_angle = analogRead(potpin_angle);            // reads the value of the potentiometer (value between 0 and 1023)
  val_speed = analogRead(potpin_speed);            // reads the value of the potentiometer (value between 0 and 1023)
  val_angle = map(val_angle, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  val_speed = map(val_speed, 0, 1023, 5, 300);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val_angle);                  // sets the servo position according to the scaled value
  delay(val_speed);                           // waits for the servo to get there
}
