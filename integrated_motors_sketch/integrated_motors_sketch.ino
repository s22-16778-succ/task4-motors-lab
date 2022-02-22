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
const int POT = A5;// analog pin used to connect the potentiometer
const int SERVO = 13; // servo pin
// Servo variables END

// DC Motor Variables BEGIN
// Encoder Pins
const int ENC_A = 2;    // yellow wire on motor
const int ENC_B = 3;    // white wire on motor

// DC Motor (Controller) Pins
const int ENA = 6;      // yellow wire (orange and brown wires on motor driver)
const int DC_IN_1 = 8; // purple wire on motor driver
const int DC_IN_2 = 9; // green wire on motor driver

// PID variables
int current_pos;

// Ultrasonic sensor variables 
const int TRIG = 12;
const int ECHO = 11;

// (1) tune PID constants
const float Kp = 1.5;    // proportional constant
const float Ki = 0.001;  // integral constant
const float Kd = 0.001;  // derivative constant
// DC Motor Variables END

// Stepper Motor Variables BEGIN

const int IR_SENSOR = A1;

// Constants for IR sensor quadratic formula
const float a = -8169.1;
const float b = 4789.2;
const float c = 18.8;

// Define pin connections & motor's steps per revolution
const int dirPin = 4;
const int stepPin = 5;
const int stepsPerRevolution = 200;
int totalSteps = 0;
int steps = 0;
int count = 0;

const int switchPin = A0;    // the number of the switchpin

// Variables will change:
int outState = true;         // the current state of the output pin
int switchState;             // the current reading from the input pin
int lastswitchState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggout
unsigned long debounceDelay = 5;    // the debounce time; increase if the output flickers
// Stepper Motor Variables END

// Pin assignments

void setup() {
  Serial.begin(9600);

  // Servo Setup
  myservo.attach(SERVO);  // attaches the servo on pin 9 to the servo object

  // DC Motor Setup
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder, RISING);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT);

  // Stepper Motor Setup
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, LOW);

  delay(200);
}

bool GUI = false;

void loop() {
  if (GUI) // run full GUI
  {
    String inData = "";
    while (Serial.available() > 0)
    {
      inData = Serial.readStringUntil('\n');
      if (inData.length() < 2) break;
  
      char c = inData.charAt(0);
      String param = inData.substring(1);
      flagSwitch(c, param);
    } 
  }
  else // Test calls to motor functions
  {
//    driveDC_GUI(90); // D360
//    driveDC_GUI(360); // D360
//    driveDC_GUI(0); // D360
//    driveDC_GUI(50); // D360
    driveDC_sensor(255); // d255
//    driveServo_GUI(0); // V0
//    driveServo_sensor(); // v
//    driveStepper_GUI(180); // P180
//    driveStepper_sensor(); // p    
  }
}



// Takes in char input and calls the appropriate function.
void flagSwitch(char c, String param) {
  switch (c) {
    case 'D':
      driveDC_GUI(param.toFloat());
      break;
    case 'd':
      driveDC_sensor(param.toFloat());
      break;
    case 'V':
      driveServo_GUI(param.toFloat());
      break;
    case 'v':
      driveServo_sensor();
      break;
    case 'P':
      driveStepper_GUI(param.toInt());
      break;
    case 'p':
      driveStepper_sensor();
      break;
  }
}

// Motor functions

//DC MOTOR FUNCTIONS
void driveDC_GUI(int angle) {
  int target_pos = (int) (angle * 440.0/360.0); // pos=440 is 1 rotation
  int error = 1000;

  long prev_time = micros();
  float integral = 0;
  current_pos = 0;
  float previous_error = 0;

  while (true) {
    // (3) difference in time
    long cu_time = micros(); //current time
    float time_diff = ((float) (cu_time - prev_time))/( 1.0e6 ); //delta time
    prev_time = cu_time;
   
    // (4) finding the error and PID
    error = current_pos - target_pos;                     // error formula
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
      digitalWrite(DC_IN_1, LOW);
      digitalWrite(DC_IN_2, LOW);
    }
    else if (direc == 1) {
      digitalWrite(DC_IN_1, HIGH);
      digitalWrite(DC_IN_2, LOW);
    }
    else if (direc == -1) {
      digitalWrite(DC_IN_1, LOW);
      digitalWrite(DC_IN_2, HIGH);
    }
  
    // (7) read new shaft position
    Serial.print(target_pos);
    Serial.print(": ");
    Serial.println(current_pos);

    if (abs(error) < 40) break;
  }

  digitalWrite(DC_IN_1, LOW);
  digitalWrite(DC_IN_2, LOW);
  delay(500);
  
}

void driveDC_sensor(float Mo_p) {
  double distance = UDS_distance_fn();
  Serial.println(distance);
  
  if (distance < 10) {
    digitalWrite(DC_IN_1, LOW);
    digitalWrite(DC_IN_2, LOW);
  }
  else {
    analogWrite(ENA, Mo_p);
    digitalWrite(DC_IN_1, LOW);
    digitalWrite(DC_IN_2, HIGH);
  }
//  delay(100);
}

// SERVO MOTOR FUNCTIONS
void driveServo_GUI(float angle) {
  float angle_cal = map(angle, 0, 200, 0, 180);   // calibrated angle
  myservo.write(angle_cal);
  Serial.println(angle_cal);
}

void driveServo_sensor() {
  float pot_value = analogRead(POT); // potentiometer measurement that corresponds to angle
  float angle = map(pot_value, 0, 1023, 0, 180);
  myservo.write(angle);
  // float angle_cal = map(angle, 0, 180, 0, 200);  // calibrated angle
  Serial.println(angle);
//  delay(100);
}

// STEPPER MOTOR FUNCTIONS

void driveStepper_GUI(int angle) {
   int set = angle*stepsPerRevolution/360;
   int signTotal = ( totalSteps> 0) - (totalSteps < 0);
   steps =(set - signTotal*abs(totalSteps)%stepsPerRevolution); // total step # and ditection to get to correvct position
   int signStep = (steps> 0) - (steps < 0);// sign of the steps
   steps = signStep*abs(steps)%stepsPerRevolution;// numbers of steps to get to equivelent position on a range of -360 to 360 degrees
   // take the shortest path to the location ie if at -90 degress and told to go to 180 it well move 90 
   //degrees to -180 instead of 270 degress to positive since they are the same angle
   if(abs(steps) > stepsPerRevolution/2)
   {
     steps = steps-signStep*stepsPerRevolution;
   }
   if (steps < 0){
   digitalWrite(dirPin, HIGH);
   }
   else{
    digitalWrite(dirPin, LOW);
   }
   for(int x = 0; x <abs(steps); x++){
     digitalWrite(stepPin, HIGH);
     delayMicroseconds(2000);    
     digitalWrite(stepPin, LOW);
     delayMicroseconds(2000);
   }
   totalSteps += steps;
//   delay(500);
}

void driveStepper_sensor(){   
  int reading = digitalRead(switchPin); // read the state of the switch into a local variable
  if (reading != lastswitchState) {     // If the switch changed, due to noise or pressing:
    lastDebounceTime = millis();        // reset the debouncing timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {    // if the switch state has changed:   
    if (reading != switchState) {
      switchState = reading;   
      if (switchState == HIGH) {    // only toggle the out if the new switch state is HIGH
        outState = !outState;
      }
    }
  }
  lastswitchState = reading;
  Serial.println(outState);
  if (outState == true){
    float data = readIR()-25; // the middle of the IR sensor range is 35
    data = min(data,25);
    int microdelay = 6000-(abs(data)/25*5500);
    
    int sign = (data > 0) - (data < 0);
    if (sign == -1)
      digitalWrite(dirPin, HIGH);
    else
      digitalWrite(dirPin, LOW);

    // Spin motor
    for(int x=0; x<15; x++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(microdelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(microdelay);
    }
    steps += 10;
  }
  delay(5);
}

/** Ignacio Helper Functions */
void encoder(){ //encoder function
  if(digitalRead(ENC_B) > 0)
    current_pos++;
  else
    current_pos--;
}

double UDS_distance_fn() { //ultrasonic sensor function
  digitalWrite(TRIG, LOW);
  delay(1);
  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  delay(100);

  double travel_time = pulseIn(ECHO, HIGH);
  double distance = (travel_time / 2) / 29;
  return distance;
}


/** Jaiden Helper Functions */
int readIR(){
  int value = analogRead(IR_SENSOR); //Read the distance in cm and store it
  float x = sqrt(-a/(c-value) + pow(b/(2*(c-value)),2)) - b/(2*(c-value));
  // Serial.println("Distance: "+String(x)+(" cm"));
  Serial.println(x);
  return x; 
}
