#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#Peripherals
const int LED = 13;
const int PIR = 4;

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

boolean sensorState = LOW;
boolean motionDetected = false;
unsigned long motionEndTime;

void initLED()
{
  Serial.println("Initializing LED");  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
}

void initWiFi()
{
  Serial.println("Initializing Wifi");  
  Serial.print(String("Connecting to ") + ssid);
  WiFi.mode(WIFI_STA);
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
  digitalWrite(LED,LOW);
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
  digitalWrite(LED,state);
  sendStatus(state);
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
  Serial.println(String("Connecting to ") + host);
  WiFiClient client;
  if (!client.connect(host,port))
  {
    Serial.println("Connection failed");
    return;
  }
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject &object = jsonBuffer.createObject();
  object["status"] = status;
  printPutRequest(Serial,object);
  Serial.println();
  Serial.println();
  printPutRequest(client,object);
  delay(100);
  while(client.available())
  {
    String s = client.readStringUntil('\r');
    Serial.print(s);
  }
  Serial.println();
  Serial.println("Closing connection");
  client.stop();
}

