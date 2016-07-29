#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define STAY_CONNECTED 1
//#define DEBUG 1

#ifdef DEBUG
const int LED = 2;
#else
const int LED = 1;
const int BTN = 3;
#endif
const int PIR = 0;

#WiFi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#Hue
const char* host = "YOUR_IP";
const int port = 80;
const char* username = "YOUR_USERNAME";
const int sensorId = YOUR_SENSOR_ID;

#Sensor
const int motionEndDelay = 60 * 1000;

#ifdef DEBUG
enum LEDState
{
  LED_LOW,
  LED_HIGH
};
#else
enum LEDState
{
  LED_HIGH,
  LED_LOW
};
enum SensorState
{
  S_OFF,
  S_LOW,
  S_HIGH,
  S_PIR
};
unsigned int sensorState = S_LOW;
unsigned int buttonState = S_PIR;
boolean stateChanged = false;
#endif
boolean motionDetected = false;
unsigned long motionEndTime;
unsigned int mode = 0;

void handleButton()
{
  buttonState = (buttonState + 1) % 4;
  stateChanged = true;
}

void initLED()
{
#ifdef DEBUG
  Serial.println("Initializing LED");
#endif
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LED_HIGH);
}

void initBTN()
{
#ifdef DEBUG
  Serial.println("Initializing BTN");
#endif
  attachInterrupt(digitalPinToInterrupt(BTN),handleButton,RISING);
}

void initWiFi()
{
#ifdef DEBUG
  Serial.println("Initializing Wifi");  
  Serial.print(String("Connecting to ") + ssid);
#endif
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
#ifdef DEBUG
  Serial.println("done");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

void initPIR()
{
#ifdef DEBUG
  Serial.println("Initializing PIR");  
#endif
  pinMode(PIR,INPUT);
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
#endif
  initLED();
#ifdef STAY_CONNECTED
  initWiFi();
#endif
  initPIR();
#ifndef DEBUG
  initBTN();
#endif
#ifdef DEBUG
  Serial.println();
#endif
  digitalWrite(LED,LED_LOW);
  delay(1000);
}

void loop()
{
#ifndef DEBUG
  if (buttonState == S_PIR)
  {
#endif
    if (digitalRead(PIR) == HIGH)
    {
      motionDetected = true;
      if (sensorState == S_LOW)
        onSensorStateChange(S_HIGH);
    }
    else
    {
      if (motionDetected)
      {
        motionEndTime = millis();
        motionDetected = false;
      }
      if ((sensorState == S_HIGH) && (motionEndTime + motionEndDelay < millis()))
        onSensorStateChange(S_LOW);
    }
#ifndef DEBUG
  }
  else if (stateChanged)
  {
    stateChanged = false;
    onSensorStateChange(buttonState);
  }
#endif
  delay(100);
}

void onSensorStateChange(unsigned int state)
{
#ifndef DEBUG
  digitalWrite(LED,LED_HIGH);
#endif
  sensorState = state;
#ifdef DEBUG
  Serial.println("SensorState: " + state);
  digitalWrite(LED,state);
#endif
  sendStatus(state + (2 * mode));
#ifndef DEBUG
  digitalWrite(LED,LED_LOW);
#endif
}

void printPutRequest(Print &p, JsonObject &object)
{
  p.println(String("PUT /api/") + username + "/sensors/" + sensorId + "/state HTTP/1.1");
  p.println(String("Host: ") + host);
  p.println("Content-Type: application/json");
  p.println(String("Content-Length: ") + object.measureLength());
  p.println("Connection: close");
  p.println();
  object.printTo(p);
}

void sendStatus(int status)
{
#ifdef DEBUG
  Serial.println(String("Opening connecting to ") + host);
#endif
#ifndef STAY_CONNECTED
  initWiFi();
#endif
  WiFiClient client;
  if (client.connect(host,port))
  {
    StaticJsonBuffer<256> buffer;
    JsonObject &object = buffer.createObject();
    object["status"] = status;
#ifdef DEBUG
    printPutRequest(Serial,object);
    Serial.println();
    Serial.println();
#endif
    printPutRequest(client,object);
    delay(100);
    while(client.available())
#ifdef DEBUG
      Serial.print(client.readStringUntil('\r'));
    Serial.println();
    Serial.println("Closing connection");
#else
      client.readStringUntil('\r');
#endif
    client.stop();
  }
  else
  {
#ifdef DEBUG
    Serial.println("Opening connection failed");
#endif
  }
#ifndef STAY_CONNECTED
  WiFi.disconnect();
#endif
}

