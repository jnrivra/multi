
#include <WiFi.h>
#include <esp_wifi.h>
//#include "driver/adc.h"



const char* ssid = "ivy";
const char* password = "12345ivy";

const char* mqtt_server = "186.64.123.26"; //mqtt server
const char *mqtt_user = "mosquittoivy";
const char *mqtt_pass = "mosquittoivy!";

long lastReconnectAttempt = 0;            //for MQTTT

void disableWiFi(){
 //    adc_power_off();



   WiFi.disconnect(true);  // Disconnect from the network
   WiFi.mode(WIFI_OFF);    // Switch WiFi off


while (WiFi.status() == WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.status() != WL_CONNECTED ? "DISCONNECTED" : "FAILED");
}

   



void enableWiFi(){
 //   adc_power_on();
    WiFi.disconnect(false);  // Reconnect the network
    WiFi.mode(WIFI_STA);    // Switch WiFi off
    Serial.println("START WIFI");
    WiFi.begin(ssid, password);

        int crashCounter;


    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("WiFi Connected!");
        Serial.print("WiFi Connected!");
        crashCounter++;
        Serial.println((String)"CrashCounter =" + crashCounter);
    if (crashCounter > 20) {
    ESP.restart();
}
        
    
    }
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
        crashCounter = 0;

}
