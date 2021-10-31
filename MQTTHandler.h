
#include <PubSubClient.h>


#include <ESP32Time.h>
ESP32Time rtc;
String timeStamp;
RTC_DATA_ATTR int bootCount = 0;

uint16_t mqttPublish(String path, String msg);
int pubPackets = 0;

WiFiClient espClient;
PubSubClient client(espClient); //lib required for mqttuint16_t client(String path, String msg);

void reconnect() {
  if (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32_clientID")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    //  client.publish("Sensores/6", "Nodemcu connected to MQTT");
      // ... and resubscribe
      client.subscribe("ntp/time");

    } /*else {
      
      Serial.print("failed to reconnect to MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }*/
  }
}




void connectmqtt()
{
  client.connect("ESP32_clientID");  // ESP will connect to mqtt broker with clientID
  {
    Serial.println("connected to MQTT");
    // Once connected, publish an announcement...

    // ... and resubscribe
    client.subscribe("ntp/time"); //topic=Demo
 //   client.publish("Sensores/6",  "connected to MQTT");

    if (!client.connected())
    {
      reconnect();
    }
  }
}





void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    String messageDateTime;

    for (int i = 0; i < length; i++)
    {
        messageDateTime += (char)payload[i];
    }

    String rawNTP;
    rawNTP = messageDateTime;
    int a, m, d, h, n, s;
    a = rawNTP.substring(0, 4).toInt();
    m = rawNTP.substring(5, 7).toInt();
    d = rawNTP.substring(8, 10).toInt();
    h = rawNTP.substring(11, 13).toInt();
    n = rawNTP.substring(14, 16).toInt();
    s = rawNTP.substring(17, 19).toInt();
    rtc.setTime(s, n, h, d, m, a);
    timeStamp = rtc.getEpoch();

    String dateTime = rtc.getDateTime();
    Serial.println((String) "SETTING UP DATE AND TIME: " + dateTime);

    delay(1000);
}
