
/*
 * 
 * 
 * V1.1.3
 * Everything Working except:
 * - increaste size of payload
 * - crash in noise()
 * TODO
 * revisar char msg large https://github.com/knollear/pubsubclient/blob/master/examples/mqtt_large_message/mqtt_large_message.ino
 * 
 * 
 * v1.1.2.
 * SPIFFS
 *
 * Publish MQTT
 * 
 * V1.1.1
 * Read Sensors
 * MQTT Suscribe and get payload
 * RTC Time from Nodered
 * 
 */

#include "wireless.h"
#include "readNoise.h"
#include "readSensors.h"
#include "SoftwareStack.h"
#include "neoTimer.h" 
#include "MQTTHandler.h"
#include "storageHandler.h"



//Device Information
String FIRMWARE_VERSION = "1.1";
String idSensor = "7";
#define MQTT_PUB_ID "Sensores/7"



//Timers
const long interval = 2000;                 // Interval at which to read sensors
Neotimer dataAcqTimer = Neotimer(interval); // Set timer's preset to 1s
unsigned long previousMillis = 0;


//CAMBIAR ESTA VARIABLE
unsigned long eventTime_2_mqttPub = 1000 * 10;     // Interval at which to publish data(after every 1 minutes)
Neotimer mqttPubTimer = Neotimer(eventTime_2_mqttPub); // Set timer's preset to 1s
boolean turnOffNTPSuscribe = false;
unsigned long timerOffNTPSuscribe = 10000;

TimerHandle_t sendDataTimer;


//Variables
uint8_t writingToFile = 1;
String lux_s, temp_s, noise_s, batt_s, timeStamp_s;



void publisher()
{
  writingToFile = 0;
  enableWiFi();
  connectmqtt();
  Serial.print("Sending data: ");
  appendToFile("}"); //closes the JSON
  Serial.println(readFileContent());
  if (boolean connected = true)
  {


  mqttPublish(String(MQTT_PUB_ID), readFileContent());


  }
  else
  {
    //don't do anything
  } 
  Serial.println();
  Serial.print("Clearing the file");
  clearFile();
  startFileWithBracket();
  writingToFile = 1;
   client.disconnect();
  delay(150);
  disableWiFi(); //turn off WiFi
}



uint16_t mqttPublish(String path, String msg)
{
    //String path = String("channels/") + channelId + String("/publish/") + apiKey;
    Serial.print("Publishing to: ");
    Serial.println(path);
  client.publish(path.c_str(), msg.c_str());
    pubPackets++;
    return pubPackets;

    


  
}




void setup() {
  setCpuFrequencyMhz(80); // It should run as low as 80MHz


  Serial.begin(115200);
  delay(500);

  setupSPIFFS();
  startFileWithBracket();

//Initialize Sensors
  Wire.begin();
  lightMeter.begin(); 
  bme.begin(0x76);


  client.setServer(mqtt_server, 1883);//connecting to mqtt server
  client.setCallback(callback);
  client.setBufferSize(2048);

  lastReconnectAttempt = 0;
  
  enableWiFi();

  connectmqtt();



Serial.println("-------------------- SETUP --------------------");

  Serial.print("Sensor ID: ");
  Serial.println(idSensor);
  Serial.print("Firmware Version :");
  Serial.println(FIRMWARE_VERSION);
  Serial.print("WiFi MAC Address :");
  Serial.println(WiFi.macAddress());
  Serial.print("Connected to WiFi :");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}




 
void loop() {


while (timerOffNTPSuscribe > millis() ){

   // Serial.println("timerOffNTPSuscribe bigger than millis");

  
  while(turnOffNTPSuscribe = false ){

  Serial.println("unsuscribing ntp time");
  client.unsubscribe("ntp/time");
  client.disconnect();
  delay(150);
  disableWiFi();
 
 turnOffNTPSuscribe = true;
  }
}



 if (mqttPubTimer.repeat())
  {
    Serial.println("Calling Publisher");
    publisher();
  }

  if (dataAcqTimer.repeat())
  {


   Serial.println();
    Serial.println("----- Reading Sensors -----");
    readLux();
    readBME();
      Serial.println("about to read Noise");

    readNoise();
      Serial.println("Reading Noise");

    readBatt();
    timeStamp = rtc.getEpoch();
    Serial.println((String) "DateTime:" + timeStamp);
    Serial.println();
    String lux_s(lux);
    String temp_s(temp);
    String noise_s(noise);
    String batt_s(batt);
    String timeStamp_s(timeStamp);

    if(timeStamp.toInt() < 150000){
      ESP.restart();
    }
    
  
  size_t xPortGetMinimumEverFreeHeapSizeCaps(uint32_t caps);
 
  int freelen = esp_get_free_heap_size();
  int minimum = esp_get_minimum_free_heap_size();
  printf("heap_caps_get_free_size: %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
  printf("heap_caps_get_minimum_free_size: %d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT));
  printf("heap_caps_get_largest_free_block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    

    
  int bytesFree;
  bytesFree = heap_caps_get_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    ESP_LOGI(TAG, "memReport(): there are %zu MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM bytes free.", bytesFree);


    

    if (writingToFile == 1)
    {
       //Create Payload for MQTT
      String payload = "{";
      payload += "\"id\":\"" + idSensor + "\",";
      payload += "\"ts\":\"" + timeStamp_s + "\",";
      payload += "\"l\":\"" + lux_s + "\",";
      payload += "\"t\":\"" + temp_s + "\",";
      payload += "\"n\":\"" + noise_s + "\",";
      payload += "\"b\":\"" + batt_s + "\"";
      payload += "}";
      appendToFile(payload);
      payload = ""; //empty the variable
    }
  }

 
/*


 if (!client.connected()) {
    Serial.println("Client Disconnected");
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (!client.connected())
    {
      reconnect();
    }
    }
  } 

*/






   // readSensors();
   //  delay(1000);


     

  
  if (boolean connected = true)
  {
     client.loop();
  }

  }


//    client.loop();






/*
void writeToFile(){

   //Create Payload for MQTT
      String payload = "{";
      payload += "\"Sensor\":\"" + idSensor + "\",";
      payload += "\"timeStamp\":\"" + timeStamp_s + "\",";
      payload += "\"Lux\":\"" + lux_s + "\",";
      payload += "\"Temp\":\"" + temp_s + "\",";
      payload += "\"Noise\":\"" + noise_s + "\",";
      payload += "\"Battery\":\"" + batt_s + "\"";
      payload += "}";
      appendToFile(payload);
      payload = ""; //empty the variable
      
}

 */
