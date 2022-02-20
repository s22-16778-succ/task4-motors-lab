
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

void setup() {
  Serial.begin(9600);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder, RISING);
  Serial.println("Target pos");
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
//  driveDC_GUI(2000); // (2) input a target position for motor; 420 = 1 rotation
  driveDC_servo(150); // set motor speed
}

void encoder(){ //encoder function
  if(digitalRead(ENC_B) > 0)
    current_pos++;
  else
    current_pos--;
}

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

void driveDC_servo(float Mo_p) {
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
