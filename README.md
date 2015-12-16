
# PIR-IPSensor4Hue
PIR motion sensor for Philips Hue bridge

This project is an example of a PIR motion sensor for the Philips Hue bridge. The sensor can be registered at the Hue bridge as an IP sensor to control lights connected to the hue bridge by defining rules.
The sensor  can have 2 states. The sensor gets state 1 if a motion is detected. The sensor gets state 0 after a predefined delay after the last motion is detected. The delay is configurable. Rules can be defined on the hue bridge to handle the sensors' different statuses.

The sensor can for example be used in a hall or a kitchen to trigger the lights when people enter that room:
- When a motion is detected and a scene can be triggered that turn the lights on or make them bright.
- A predefined delay after the last motion is detected a scene can be triggerd that will dim the lights or turn them off.

What you need:
- Philips Hue Brigde + light bulps
- NodeMCU or other EPS2866 MCU
- PIR motion sensor
- LED (optopnal)
- Arduino development tool
- ESP8266 Arduino
- PIR_IP_Sensor_4_Hue_V1.ino

TODO
