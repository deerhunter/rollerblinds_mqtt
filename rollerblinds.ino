#include <Stepper.h>
#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


//pins
#define BUTTON_PIN 13 //Set correct pins for stuff
#define DHT_PIN 12
#define LIGHT_PIN 14
#define LIGHT_SENSOR1_PIN 15
#define LIGHT_SENSOR2_PIN 16
#define SOIL_SENSOR_PIN 17

//blinds variables
int calibrated=0; //1- calibrating down 2- calibtaring up 3 - calibrated
int state = 50; //% of opening. 0 - closed; 100 - open.
int stepsMAX=0; // maximum steps for full travel
int stepspersent=0; //steps for 1% of moving

// Sensors variables
int light_out=0;
int light_in=0;
float soil_humid=0;
float temperature=0;
float humidity=0;


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
  Serial.println("Starting selftests...");
  //get sensors data and print it to console
  delay(1000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}
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
state = 0; // Note that we are in bottom position
calibrated=2; //calibrating high position
Serial.println("Fully closed"); 
delay(1000);
Serial.println("Going up");

while(digitalRead(BUTTON_PIN) == LOW) { //go up untill button is pressed again and count steps
  stepper1.step(1*direction);
  stepsMAX++; //steps counted in run up
 }
 calibrated=3;
 state = 100; // Note that we are in top position
 Serial.println("Calibration complete");
 Serial.println("Fully opened"); 
 Serial.print("Maximum steps for one run is ");
 Serial.println(stepsMAX); 
 stepspercent = floor(stepsMAX/100);
 Serial.print("Steps in one percent:"); 
 Serial.println(stepspercent); 
}
  else{
    Serial.print("Button is presed for:"); 
    Serial.print(BUTTON_TIME); 
    Serial.println("milliseconds."); 
  }
  }

void openblinds()
{ 
//code to open blinds
for (int i=0; i<newstate; i++){
stepper1.step(stepspercent*direction);
}
}


void closeblinds()
{ 
//code to close blinds
}

void loop()
{ 
if (calibrated==0){ //Check if we are calibrated
Serial.println("Requires calibration"); 
calibration();
Serial.println("Returned after calibration");
}

//do something

}
