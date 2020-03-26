#include <WiFi.h>
#include <PubSubClient.h>

#define CHANNEL 5
#include "DHT.h"
#define DHTPIN 23     // Digital pin connected to the DHT sensor5
#define DHTTYPE DHT11

#define VERT 13
#define JAUNE 14
#define ROUGE 15


//Variable remplis par notre application sur ordinateur pour laide de configuration
const char* ssid = "MONRESEAU"; 
const char* password =  "MONMDP"; 
const char* mqttServer = "STOP-COVID.com";
const int mqttPort = 4242;
const char* mqttUser = "alfred_hitchcock";
const char* mqttPassword = "The_master_of_suspense";
//---------------------//


// On utilisera un serveur NTP pour avoir la date
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
// DHT 
DHT  dht(DHTPIN , DHTTYPE);



int getLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
   // Serial.println("Failed to obtain time");
    return 1;
  }
  return 0;
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
//-----------------------------//


//------
void wifiAPSetup() {
  Serial.println("wifiAPSetup...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip,gateway,subnet); // Configuration DHCP
  //WiFi.softAP(ssid,pass); // DHCP on 192.168.4.0/24 by default if no WiFi.softAPConfig()
  WiFi.softAP(ssid,pass,CHANNEL);
  IPAddress apIP=WiFi.softAPIP();
  Serial.printf("IP address: %s\r\n",apIP.toString().c_str());
  Serial.printf("BSSID: %s\r\n",WiFi.softAPmacAddress().c_str());
}

void publish(int Mesure) {
    while (!client.connected()) {
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
            Serial.println("connected");
            if(getLocalTime()){
                client.publish("/Temperature", ""+String(mqttUser)+","+ String(&timeinfo "%A, %B %d %Y %H:%M:%S")+ Mesure);
                client.subscribe(MQTT_SERIAL_RECEIVER_CH);
            }else{
                client.publish("/Temperature", ""+String(mqttUser)+","+"TIME_ERROR"+ Mesure);
                client.subscribe(MQTT_SERIAL_RECEIVER_CH);
            }
        } else {
            Serial.print("failed, rc=");
             Serial.print(client.state());
             Serial.println(" try again in 5 seconds");
             delay(5000);



        }}}
int mesure(){
   float t = dht.readTemperature ();
   return t; 

}
void allume(int led){
    di

}
void setup(){
    Serial.begin(115200);
    wifiAPSetup();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    pinMode(18,INPUT);
    dht.begin();
} 
void loop(){
    if( digitalRead(18)==LOW ){
        float mesure;
        mesure = mesure();
        if( mesure < 37.5){
            eteint(JAUNE);
            eteint(ROUGE);
            allume(VERT);
            
        }else{if( mesure>37.5 and mesure<38){
            eteint(VERT);
            allume(JAUNE);
            eteint(ROUGE);
        }else{
            eteint(VERT);
            eteint(JAUNE);
            allume(ROUGE);
        }}
        

        publish(mesure);
    }
}
