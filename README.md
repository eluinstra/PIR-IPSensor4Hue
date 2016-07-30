
# PIR-IPSensor4Hue
PIR motion sensor for Philips Hue bridge

This project is an example of a PIR motion sensor for the Philips Hue bridge. The sensor can be registered at the Hue bridge as an IP sensor to control lights connected to the hue bridge.
The sensor  can have 2 statuses. The sensor changes to status 1 if a motion is detected. The sensor changes to status 0 after a predefined delay after the last motion is detected. The delay is configurable. Rules can be defined on the Hue bridge to handle the sensors' different statuses.

The sensor can for example be used in a hall or a kitchen to trigger the lights when people enter that room:
- When a motion is detected and a scene can be triggered that turn the lights on or make them bright.
- A predefined delay after the last motion is detected a scene can be triggerd that will dim the lights or turn them off.

What you need:
- Philips Hue Brigde + light bulps
- NodeMCU or other EPS2866 MCU
- PIR motion sensor
- LED (optional)
- Arduino (https://www.arduino.cc/en/Main/Software)
  - ArduinoJson (https://github.com/bblanchon/ArduinoJson)
  - ESP8266 Arduino (https://github.com/esp8266/Arduino)
- PIR_IP_Sensor_4_Hue_V1.ino

#1 Sensor V1
Connect the LED to pin 13 and the PIR to pin 4 of the NodeMCU.

Before you uploading can upload PIR_IP_Sensor_4_Hue_V1.ino to the NodeMCU you have to configure the wifi and the sensor.

Wifi configuration
To configure wifi you have to change the following constants:
- ssid
- password

Sensor configuration
To configure wifi you have to change the following constants:
- host (= ip address of the Hue bridge)
- port (= port of the Hue bridge)
- username (= username for this sensor on the Hue bridge (see http://www.developers.meethue.com/documentation/configuration-api#71_create_user how to create a user))
- sensorId (= sensor id for this sensor on the Hue bridge (see http://www.developers.meethue.com/documentation/how-use-ip-sensors how to create a sensor))
- motionEndDelay (= the delay after the last motion before the sensor will change its status)

See http://www.developers.meethue.com/documentation/getting-started how to connect to your Hue bridge

After you changed the configuration you can upload PIR_IP_Sensor_4_Hue_V1.ino to the NodeMCU using Arduino.

Then you can create scenes through the Hue app and add rules to your Hue bridge to act on status changes of the sensor. See http://www.developers.meethue.com/documentation/how-use-ip-sensors how to create rules.

You have to create 2 rules in the hue bridge for the sensor with id sensorId. One for each of the 2 statuses (for example dimmed and bright). The rules should have conditions with the following address:
/sensors/<sensorId>/state/status
with value 0 or 1 (for status 0 or 1) and an action with the right scene defined.

Note: when you change a scene using the Hue app a new scene is created. So when the (old) scene was connected to a sensor the new scene will not be used, but the old scene will still be used. To use the new scene you have to reconfigure the rules for the sensor.

#2 Sensor Software V2
Connect the Button to GPIO3 (RX) and the PIR to pin GPIO0 of the ESP-01.

With the button you can select the function of the sensor: off, status 1, status 2, pir (default).

Before you uploading can upload PIR_IP_Sensor_4_Hue_V2.ino to the ESP-01 you have to configure the wifi and the sensor.

See V1 for wifi and sensor configuration.

You have to create 2 rules in the hue bridge for the sensor with id sensorId. One for each of the 2 statuses (for example dimmed and bright). The rules should have conditions with the following address:
/sensors/<sensorId>/state/status
with value 0, 1 or 2 (0 = off, 1 and 2 are the 2 pir statuses) and an action with the right scene defined.


# Todo
The following functionality will be added
- configure the sensor through rest/http
- add a photoresistor sensor

# Ideas
Here are some ideas to extend the sensor
- You can combine different sensors (like the Hue Daylight sensor) to create more complicated rules
- You could replace the PIR sensor by a different digital sensor
- You could add more PIR sensors and combine these to a single motion sensor
- You could add other sensors (for example a photoresistor) and register these as seperate sensors at the Hue bridge

