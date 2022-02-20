/*
* Carnegie Mellon University Mechatronics Spring 2022: Team E
* Motors Lab Code
* Authors: Felipe Borja (fborja)
*          Brandon Wang (bcwang)
*          Jaiden Napier (jinapier)
*          Ignacio Peon Zapata (ipeon)
*          Sahil Saini (sssaini)
* Functionality: Allows for GUI and sensor measurements to control three different types of motors.
*/

#include <Servo.h>

// Servo variables BEGIN
Servo myservo;  // create servo object to control a servo
int potpin_angle = 0;// analog pin used to connect the potentiometer
int val_angle;    // variable to read the value from the analog pin
// Servo variables END

// DC Motor Variables BEGIN
// Encoder Pins
int ENC_A = 2;    // yellow wire on motor
int ENC_B = 3;    // white wire on motor

// Motor (Controller) Pins
int ENA = 7;      // yellow wire (orange and brown wires on motor driver)
int IN_pin_1 = 8; // purple wire on motor driver
int IN_pin_2 = 9; // green wire on motor driver

// PID variables 
long prev_time = micros();
float integral = 0;
int current_pos = 0;
float previous_error = 0;

// Ultrasonic sensor variables 
int trig_pin = 12;
int echo_pin = 11;
double travel_time;
double distance;

// (1) tune PID constants
const float Kp = 1.5;    // proportional constant
const float Ki = 0.001;  // integral constant
const float Kd = 0.001;  // derivative constant
// DC Motor Variables END



// Pin assignments

void setup() {
  Serial.begin(9600);
  Serial.print("Hi :)");

  // Servo Setup
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  //DC Motor Setup
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder, RISING);
  Serial.println("Target pos");
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
//  if (Serial.available())
//  {
//    char c = Serial.read();
//    flagSwitch(c);
//  }
  driveServo_sensor();
  //  driveDC_GUI(2000); // (2) input a target position for motor; 420 = 1 rotation
  driveDC_sensor(150); // set motor speed
}

// Takes in char input and calls the appropriate function.
//void flagSwitch(char c) {
//  switch (c) {
//    case 'D':
//      driveDC_GUI();
//      break;
//    case 'd':
//      driveDC_sensor();
//      break;
//    case 'V':
//      driveServo_GUI();
//      break;
//    case 'v':
//      driveServo_sensor();
//      break;
//    case 'P':
//      driveStepper_GUI();
//      break;
//    case 'p':
//      driveStepper_sensor();
//      break;
//    default:
//      Serial.print("DO NOT TYPE HERE!!!");
//  }
//}

// Motor functions

//DC MOTOR FUNCTIONS
void driveDC_GUI(int target_pos) {
  // (3) difference in time
  long cu_time = micros(); //current time
  float time_diff = ((float) (cu_time - prev_time))/( 1.0e6 ); //delta time
  prev_time = cu_time;
 
  // (4) finding the error and PID
  int error = current_pos - target_pos;                     // error formula
  float derivative = (error - previous_error)/time_diff;    // derivative formula (rate of change)
  integral += error*time_diff;                              // integral formula
  float control_s = Kp*error + Kd*derivative + Ki*integral; // control signal
  previous_error = error;                                   // store previous error
 
  // (5) set DC motor power and direction
  float Mo_p = min(fabs(control_s), 255);
  Serial.println(control_s);
  int direc = (control_s >= 0) ? 1 : -1; // direction of shaft; >0 is CCW, <0 is CW
  
  // (6) set DC Motors
  analogWrite(ENA, Mo_p);
  if (direc == 0) {
    digitalWrite(IN_pin_1, LOW);
    digitalWrite(IN_pin_2, LOW);
  }
  else if (direc == 1) {
    digitalWrite(IN_pin_1, HIGH);
    digitalWrite(IN_pin_2, LOW);
  }
  else if (direc == -1) {
    digitalWrite(IN_pin_1, LOW);
    digitalWrite(IN_pin_2, HIGH);
  }

  // (7) read new shaft position
  Serial.print(target_pos);
  Serial.print(": ");
  Serial.println(current_pos);
}
void driveDC_sensor(float Mo_p) {
  UDS_distance_fn();
  if (distance < 10) {
    digitalWrite(IN_pin_1, LOW);
    digitalWrite(IN_pin_2, LOW);
    return;
  }
  else {
    analogWrite(ENA, Mo_p);
    digitalWrite(IN_pin_1, HIGH);
    digitalWrite(IN_pin_2, LOW);
  }
}

// SERVO MOTOR FUNCTIONS
void driveServo_GUI() {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}
void driveServo_sensor() {
  val_angle = analogRead(potpin_angle);
  val_angle = map(val_angle, 0, 1023, 0, 180);
  myservo.write(val_angle);
  val_angle = map(val_angle, 0, 180, 0, 200);
  Serial.write(val_angle);
  delay(5);
}

// STEPPER MOTOR FUNCTIONS
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





// Ignacio Functions

void encoder(){ //encoder function
  if(digitalRead(ENC_B) > 0)
    current_pos++;
  else
    current_pos--;
}

double UDS_distance_fn() { //ultrasonic sensor function
  digitalWrite(trig_pin, LOW);
  delay(1);
  digitalWrite(trig_pin, HIGH);
  delay(1);
  digitalWrite(trig_pin, LOW);

  travel_time = pulseIn(echo_pin, HIGH);
  distance = (travel_time / 2) / 29;
  delay(200);

  Serial.print("Distance (cm): ");
  Serial.println(distance);
}
