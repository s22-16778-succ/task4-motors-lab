
const int IR_PIN = A1;

// Constants for IR sensor quadratic formula
const float a = -8169.1;
const float b = 4789.2;
const float c = 18.8;

// Define pin connections & motor's steps per revolution
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;
int totalSteps = 0;
int steps = 0;
int count = 0;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  digitalWrite(dirPin, LOW);
  
  Serial.begin(9600);
  Serial.println(100);
}

void loop() {
     // runMotor(); 

      if (count ==0){
        SetAngle(180);
        delay(2000);
        count++;
      }
      
}

void  runMotor(){
  float Data = readIR()-45; // the middle of the Ir sensor range is 35
  int microdelay = (6000-(abs(Data)/45)*5500);
  
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
 
// IR Proximity Sensor
int readIR(){
  int value = analogRead(IR_PIN); //Read the distance in cm and store it
  float x = sqrt(-a/(c-value) + pow(b/(2*(c-value)),2)) - b/(2*(c-value));
  Serial.println("Distance: "+String(x)+(" cm")); 
  return x;

   
}

void SetAngle(float angle){

   int set = angle*stepsPerRevolution/360;
  
    if (set < 1){
     steps =( totalSteps%stepsPerRevolution - set);
    }
    else
    {
      steps =(set - totalSteps%stepsPerRevolution);
    }
    
    if (steps < 0){
    digitalWrite(dirPin, HIGH);
    }
    else{
      digitalWrite(dirPin, LOW);
    }
    for(int x = 0; x <200; x++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    totalSteps = totalSteps + steps;
 }

 


  
