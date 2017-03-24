#include <Stepper.h>

//pins
#define BUTTON_PIN 13 //Set correct pins for stuff
#define TEMP_PIN 12
#define LIGHT_PIN 14
#define LIGHT_SENSOR1_PIN 15
#define LIGHT_SENSOR2_PIN 16
#define SOIL_SENSOR_PIN 17

//blinds variables
int calibrated=0; //1- calibrating down 2- calibtaring up 3 - calibrated
int state = 3; //1-up 2-down 3-middle(unknown)
int stepsMAX=0; // maximum steps for full travel

// Sensors variables
int light_out=0;
int light_in=0;
int soil_humid=0;
int temperature=0;
int humidity=0;


//additional variables
#define BUTTON_TRESHOLD 150
unsigned long BUTTON_TIME=0;
unsigned long time_start;
unsigned long time_end;

//Stepper
const int stepsPerRevolution = 4096;  // 64 steps per revolution and 1/64 gear is 64*64=4096 steps per revolution
Stepper stepper1(stepsPerRevolution,5,0,2,4);
stepper.setSpeed(10); // may need to move it somewhere
int direction = 1; //Change to -1 if stepper is going in wrong direction

//WI-FI

void setup()
{
  Serial.begin(9600);
  Serial.println("Booted");
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); 
}

void calibration()
{ 
Serial.println("Starting calibration");
while(digitalRead(BUTTON_PIN) == LOW) { }; //wait for button press to start calibration
  
time_start = millis();
while(digitalRead(BUTTON_PIN) == HIGH) { };
time_end = millis();
BUTTON_TIME=time_end-time_start;
  
if (BUTTON_TIME>=2000){ //closes at the end of sub

 calibrated=1; //calibrating low position
 Serial.println("Going down");
while(digitalRead(BUTTON_PIN) == LOW) {
 stepper1.step(-1*direction);
}
state = 2; // Note that we are in bottom position
calibrated=2; //calibrating high position
Serial.println("Going up");

while(digitalRead(BUTTON_PIN) == LOW) { //go up untill button is pressed again and count steps
  stepper1.step(1*direction);
  stepsMAX++; //steps counted in run up
 }
 calibrated=3;
 state = 1; // Note that we are in top position
 Serial.println("Calibration complete"); 
 Serial.print("Maximum steps for one run is ");
 Serial.println(stepsMAX); 
}
}

void loop()
{ 
if (calibrated==0){ //Check if we are calibrated
Serial.println("Requires calibration"); 
calibration();
Serial.println("Returned after calibration");
Serial.println("Blinds are full open now");
}

//do something

}
