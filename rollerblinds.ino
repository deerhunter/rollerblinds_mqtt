#include <Stepper.h>

//pins
#define BUTTON_PIN 13
#define TEMP_PIN 12
#define LIGHT_PIN 14
#define LIGHT_SENSOR1_PIN 15
#define LIGHT_SENSOR2_PIN 16
#define SOIL_SENSOR_PIN 17

//blinds variables
int calibrated=0; //1- calibrating down 2- calibtaring up 3 - calibrated
int state = 3; //1-up 2-down 3-middle(unknown)
int stepsMAX=0; // maximum steps for full travel

//additional variables
#define BUTTON_TRESHOLD 150
int BUTTON_TIME=0;

//Stepper
Stepper stepper(8,5,0,2,4);

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
//do calibration
while(digitalRead(BUTTON_PIN) == LOW) { };
while(digitalRead(BUTTON_PIN) == HIGH) { 
//Count for how long the button was pressed
}
if (BUTTON_TIME>=2000){
 calibrated=1;
 Serial.println("Going down");
}

  while(digitalRead(BUTTON_PIN) == LOW) {
//go down untill button is pressed again
}
calibrated=2;
Serial.println("Going up");

  while(digitalRead(BUTTON_PIN) == LOW) {
//go up untill button is pressed again and count steps
}
 calibrated=3;
 Serial.println("Calibration complete"); 
stepsMAX=0; //steps counted in run up
}


void loop()
{ 

if (calibrated==0){
calibration();
}

//do something

}
