
const int IR_PIN = A1;

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

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, LOW);
  Serial.begin(9600);
  delay(500);
  
}

void loop() {
     runMotor(); 
     //test code motor should hit 90 ,180 and -45 degrees respec
//      if (count ==0){
//        SetAngle(90);
//        delay(1000);
//        SetAngle(180);
//        delay(1000);
//        SetAngle(-45);
//        delay(1000);
//        count++;
//      }  
}

void  runMotor(){
   // read the state of the switch into a local variable:
  int reading = digitalRead(switchPin);
  // If the switch changed, due to noise or pressing:
  if (reading != lastswitchState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the switch state has changed:
    if (reading != switchState) {
      switchState = reading;

      // only toggle the out if the new switch state is HIGH
      if (switchState == HIGH) {
        outState = !outState;
      }
    }
  }
  lastswitchState = reading;
  Serial.println(outState);
  if (outState == true){
    float Data = readIR()-25; // the middle of the Ir sensor range is 35
    Data= min(Data,25);
    int microdelay = 6000-(abs(Data)/25*5500);
    
    int sign = (Data > 0) - (Data < 0);
    if (sign == -1){
      digitalWrite(dirPin, HIGH);
    }
    else{
      digitalWrite(dirPin, LOW);
    }
    // Spin motor
    for(int x = 0; x <15; x++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(microdelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(microdelay);
    }
     steps = steps+10;
  }
 }
// IR Proximity Sensor
int readIR(){
  int value = analogRead(IR_PIN); //Read the distance in cm and store it
  float x = sqrt(-a/(c-value) + pow(b/(2*(c-value)),2)) - b/(2*(c-value));
  Serial.println("Distance: "+String(x)+(" cm")); 
  return x; 
}

void SetAngle(float angle){
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
   totalSteps = totalSteps + steps;
 }

 


  
