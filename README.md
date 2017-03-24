# rollerblinds_mqtt
Rollerblinds with MQTT, button, two ambient light sensors for outomated opening and closing, soil humidity sensor for a plant, light for a plant and DHT22 sensor for room temp and humidity readings

BOM:
NodeMCU
Motor shield
28BYJ motor
TEMPT6000 sensors
DHT22 sensor
Some USB light for a plant that will work from 3v power supply
Soil humidity sensor

about 15-20 dollars + price of rollershade

On powerON, after a long press starts calibration. 
Go down untill short press, go up untill short press and count steps.

Every 5 seconds check sensors data and send it and state of light and blinds to MQTT topics
On short buttonpress change state of blinds from full open to full closed and back
listen to MQTT commands for plant light and rollerblinds. may open them to % of full according to data from MQTT
