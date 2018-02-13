//#include <AccelStepper.h>
#include <Stepper.h>
#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SimpleTimer.h>

//pins
#define BUTTON_PIN 16 //Set correct pins for stuff
#define DHT_PIN 12 
#define LIGHT_PIN 14 //not used yet
#define LIGHT_SENSOR1_PIN 6 //not used yet
#define LIGHT_SENSOR2_PIN 5 //not used yet
#define SOIL_SENSOR_PIN 7 //not used yet

//blinds variables
int calibrated=0; //1- calibrating down 2- calibtaring up 3 - calibrated
int state = 50; //% of opening. 0 - closed; 100 - open.
int newstate = 50; //state of blinds, recieved from MQTT or button
int stepsMAX=0L; // maximum steps for full travel not sure about L, perhaps use long or unsigned long
int stepspersent=0L; //steps for 1% of moving

// Sensors variables not implemented yet
// int light_out=0;
// int light_in=0;
float soil_humid=0;
float temperature=0;
// float humidity=0;
DHT dht;

//additional variables
#define BUTTON_TRESHOLD 150
unsigned long BUTTON_TIME=2000;
unsigned long time_start=0;
unsigned long time_end=0;

//Stepper
const int stepsPerRevolution = 4096;  // 64 steps per revolution and 1/64 gear is 64*64=4096 steps per revolution
#define motorPin1  5 // 
#define motorPin2  0 // 
#define motorPin3  4 // 
#define motorPin4  2 // 
//AccelStepper stepper1(8, motorPin1, motorPin2, motorPin3, motorPin4);
//stepper1.setSpeed(10); // may need to move it somewhere
int direction = 1; //Change to -1 if stepper is going in wrong direction
Stepper stepper1(stepsPerRevolution,5,0,2,4);

//WI-FI
char ssid[] = "";
char pass[] = "";
//SimpleTimer timer;


void setup()
{
  dht.setup(DHT_PIN);
  Serial.begin(9600);
  Serial.println("Booted");
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); 

  Serial.println("Starting selftests...");
  //get sensors data and print it to console
  delay(1000);
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    //implement DHT sensor
    // somehow subscribe to two topics - for plant light and for blinds
    
}
Serial.println(temperature);
Serial.println(humidity);
}

void calibration()
{ 
Serial.println("Starting calibration");
delay(2000);

while(digitalRead(BUTTON_PIN) == HIGH) { }; //wait for button press to start calibration

  Serial.println("button pressed");
time_start = millis();
while(digitalRead(BUTTON_PIN) == LOW) { };
time_end = millis();
BUTTON_TIME=time_end-time_start;
  
if (BUTTON_TIME>=2000){ //closes at the end of sub

 calibrated=1; //calibrating low position
 Serial.println("Going down");
while(digitalRead(BUTTON_PIN) == HIGH) {
 
 stepper1.step(-1*direction*20000);
}
state = 0; // Note that we are in bottom position
calibrated=2; //calibrating high position
Serial.println("Fully closed"); 
delay(1000);
Serial.println("Going up");

while(digitalRead(BUTTON_PIN) == HIGH) { //go up untill button is pressed again and count steps
  stepper1.step(1*direction);
  stepsMAX++; //steps counted in run up
 }
 calibrated=3;
 state = 100; // Note that we are in top position
 Serial.println("Calibration complete");
 Serial.println("Fully opened"); 
 Serial.print("Maximum steps for one run is ");
 Serial.println(stepsMAX); 
 stepspersent = floor(stepsMAX/100);
 Serial.print("Steps in one percent:"); 
 Serial.println(stepspersent); 
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
if (newstate>state){ //anti-fool protection
for (int i=0; i<=newstate; i++){
stepper1.step(stepspersent*direction);
}
  state = newstate;
}
  else{
    closeblinds();
  }
}


void closeblinds()
{ 
//code to close blinds
  if (newstate<state){ //anti-fool protection
for (int i=0; i<=newstate; i++){
stepper1.step(-1*stepspersent*direction);
}
  state = newstate;
  }
  else{
    openblinds();
  }
  }

void buttonpressed() //button is pressed, long press close blinds to 0% and short press open to 100%
{ 
time_start = millis();
while(digitalRead(BUTTON_PIN) == LOW) { };
time_end = millis();
BUTTON_TIME=time_end-time_start;

  if (BUTTON_TIME >= 2000){
  newstate=0;
    closeblinds();
  }
  else{
    //open blinds
      newstate=100;
    openblinds();
  }
}

void loop()
{ 
if (calibrated==0){ //Check if we are calibrated
Serial.println("Requires calibration"); 
calibration();
Serial.println("Returned after calibration");
}
if (digitalRead(BUTTON_PIN) == LOW){
  buttonpressed();
}
  
//wait for MQTT comand
//get and post sensors data

}
