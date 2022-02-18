
//encoder variables
int ENC_A = 2; //yellow cable 
int ENC_B = 3; //white cable 

//motor variables (from motor controller)
int IN_pin_2 = 9;
int IN_pin_1 = 8;
int ENA = 6;

//PID variables 
long prev_time = micros();
float integral = 0;
int current_pos = 0;
float previous_error = 0;

//ultrasonic sensor variables 
int trig_pin = 12;
int echo_pin = 11;
double travel_time;
double distance;

// (2) tunnig of PID constants
  const float k_pro = 2.9; //proportional constant
  const float k_int = 0.0016;//integral constant
  const float k_der = 0.001; //derivative constant

   // (1) inputting a target position for motor
  const int target_position = 1000; 
  //target = 250*sin(prevT/1e6);

void setup() {
  
  Serial.begin(9600);
  pinMode(ENC_A,INPUT);
  pinMode(ENC_B,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A),encoder,RISING);
  Serial.println("target pos");
  pinMode (trig_pin, OUTPUT);
  pinMode (echo_pin, INPUT);
}
 
void loop() {

 
  // (3) difference in time
  long cu_time = micros(); //current time
  float time_diff = ((float) (cu_time - prev_time))/( 1.0e6 ); //delta time 
  prev_time = cu_time;
  
 
  // (4) finding the error!
  int error = current_pos-target_position; //error formula
 
  
  float derivative = (error-previous_error)/(time_diff); // derivative formula (rate of change)
 

  integral = integral+error*time_diff;   // integral calculation
 

  float control_s = k_pro*error + k_der*derivative + k_int*integral;   // control signal
 
  // (5) dc motor power
  float Mo_p = fabs(control_s);
  if( Mo_p > 255 ){
    Mo_p = 255;
  }
 
  // motor direction
  int direc_shaft = 1; //cw
  if(control_s<0){
    direc_shaft = -1; //ccw
  }
 
  //motor comand 
  dc_Motor_fn (direc_shaft,Mo_p,ENA,IN_pin_1,IN_pin_2);
 
  // store previous error
  previous_error = error;
 
  Serial.print(target_position);
  Serial.print(" ");
  Serial.print(current_pos);
  Serial.println();

  distance_fn(); 
}

//functions:

void encoder(){ //encoder function
  int b = digitalRead(ENC_B);
  if(b > 0){
    current_pos++;
  }
  else{
    current_pos--;
  }
}


double distance_fn() { //ultrasonic sensor function
  digitalWrite (trig_pin, LOW);
  delay (1);

  digitalWrite (trig_pin, HIGH);
  delay(1);

  digitalWrite (trig_pin, LOW);

  travel_time = pulseIn (echo_pin, HIGH);
  distance = (((travel_time / 2) / 29)/ 2.54);
  delay (200);


  Serial.print ("The distance is (inches):");
  Serial.println (distance);
}

void dc_Motor_fn (int direc, int pwmVal, int ena, int in_1, int in_2){ //dc motor function 
  analogWrite(ena,pwmVal);
  if(direc == 1){
     if (distance <= 3){
      digitalWrite(in_1,LOW);
      digitalWrite(in_2,LOW);
     }
     else{
    digitalWrite(in_1,HIGH);
    digitalWrite(in_2,LOW);
  }
  }
  else if(direc == -1){
    if (distance <= 3){
      digitalWrite(in_1,LOW);
      digitalWrite(in_2,LOW);
    }
    else{
    digitalWrite(in_1,LOW);
    digitalWrite(in_2,HIGH);
        }
  }
 else{
    digitalWrite(in_1,LOW);
    digitalWrite(in_2,LOW);
 }  
}
