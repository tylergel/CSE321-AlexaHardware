#include <Arduino.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Servo.h>
Servo myservo;
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#define DHTPIN 0   
#define DHTTYPE DHT11  
ESP8266WiFiMulti WiFiMulti;

DHT dht(DHTPIN, DHTTYPE);
void setup() {   
  dht.begin();             
  Serial.begin(115200);
    Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Fios-W3QT", "3507internet9b20");
   myservo.attach(2);

 

}
 
void loop() {
  int water_level = getWaterLevel();
  Serial.print("Water Level");
  Serial.println(water_level);
  
  int voltage_level = getVoltageLevel();  
  Serial.print("Voltage Level");    
  Serial.println(voltage_level); 
 
  int temp = getTemperature();
  Serial.print("Temperature");
  Serial.println(temp);
 
   insertStuff(temp, water_level, voltage_level);
   delay(5000);
}

int getVoltageLevel() {
  return analogRead(1);
}
int getWaterLevel() {
  return analogRead(0);
}
void moveMotor() {
  Serial.println("moving");
    myservo.write(180);
  delay(3000);
    myservo.write(-180);
 
}
  
int getTemperature() {
    float temp = dht.readTemperature(true);
   return temp;
}

void insertStuff(int temp, int level,int quality)   //CONNECTING to API
{

  String temp1 = String(temp);
  String quality1=String(quality);
  String level1=String(level);
  // if there's a successful connection:

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://steamraffles.com/cse321/api.php?temperature="+temp1+"&level="+level1+"&quality="+quality1)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          if(payload == "1") {
            moveMotor();
            deleteStuff();
          }
          Serial.println(payload);
          
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  
}

void deleteStuff()   //CONNECTING to API
{

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://steamraffles.com/cse321/api.php?delete=true")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}
