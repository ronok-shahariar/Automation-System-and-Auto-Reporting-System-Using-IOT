#include <DHT.h>
#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
 
String apiKey = "UGEFYFF5HYUX94U1";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Death Note";     // replace with your wifi ssid and wpa2 key
const char *pass =  "lightyagami37";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D2          //pin where the dht11 is connected
#define VR A0
 
DHT dht(DHTPIN, DHT12);
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float v = analogRead(VR);
      
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field3=";
                             postStr += String(v);
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(String(postStr.length()));
                             //client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                             Serial.println(v);
                             Serial.println();
                        }
          else
         {
          ESP.restart();
         }    
          delay(200);          
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
