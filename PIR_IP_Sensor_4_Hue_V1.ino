/*
 This example code is in the public domain
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#Peripherals
const int LED = 16;
const int PIR = 4;

#WiFi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#Hue
const char* host = "YOUR_IP";
const int port = 80;
const char* username = "YOUR_USERNAME";
const int sensorId = 7;

#Sensor
const int motionEndDelay = 60 * 1000;

boolean sensorState = LOW;
boolean motionDetected = false;
unsigned long motionEndTime;
unsigned int mode;

void initLED()
{
  Serial.println("Initializing LED");  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
}

void initWiFi()
{
  Serial.println("Initializing Wifi");  
  Serial.print(String("Connecting to ") + ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("done");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void initPIR()
{
  Serial.println("Initializing PIR");  
  pinMode(PIR,INPUT);
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  initLED();
  initWiFi();
  initPIR();
  digitalWrite(LED,HIGH);
  Serial.println();
  delay(1000);
}

void loop()
{
  if (digitalRead(PIR) == HIGH)
  {
    motionDetected = true;
    if (sensorState == LOW)
      onSensorStateChange(HIGH);
  }
  else
  {
    if (motionDetected)
    {
      motionEndTime = millis();
      motionDetected = false;
    }
    if ((sensorState == HIGH) && (motionEndTime + motionEndDelay < millis()))
      onSensorStateChange(LOW);
  }
}

void onSensorStateChange(boolean state)
{
  sensorState = state;
  Serial.println("SensorState: " + state);
  digitalWrite(LED,!state);
  sendStatus(state + (2 * mode));
}

void printPutRequest(Print &p, JsonObject &object)
{
  p.println(String("PUT /api/") + username + "/sensors/" + sensorId+ "/state HTTP/1.1");
  p.println(String("Host: ") + host);
  p.println("Content-Type: application/json");
  p.println(String("Content-Length: ") + object.measureLength());
  p.println("Connection: close");
  p.println();
  object.printTo(p);
}

void sendStatus(int status)
{
  Serial.println(String("Opening connecting to ") + host);
  WiFiClient client;
  if (client.connect(host,port))
  {
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &object = jsonBuffer.createObject();
    object["status"] = status;
    printPutRequest(Serial,object);
    Serial.println();
    Serial.println();
    printPutRequest(client,object);
    delay(100);
    while(client.available())
      Serial.print(client.readStringUntil('\r'));
    Serial.println();
    Serial.println("Closing connection");
    client.stop();
  }
  else
  {
    Serial.println("Opening connection failed");
  }
}
