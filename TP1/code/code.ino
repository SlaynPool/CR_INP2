#include <WiFi.h>
#include <WebServer.h>
#define CHANNEL 5
const char *ssid="NicolasV";
const char *pass="12345678";
IPAddress ip(192,168,42,1);
IPAddress gateway(192,168,42,254);
IPAddress subnet(255,255,255,0);
WebServer server(80);
int frequence = 1000;
int resolution = 8;
int canal = 0;
int valMot = 0;

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
void handleNotFound() {
  server.send(404,"text/plain","Fichier non trouve !");
}
void handleRoot() {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
s +="<h1>Vitesse moteur actuel :"+String(valMot)+" </h1>\n";
s +="p><a href=\"/plus\"><button class=\"button\"> Plus</button></a></p>";
s +="p><a href=\"/moins\"><button class=\"button\"> Moins</button></a></p>";
s += "</html>\n";
server.send(200,"text/html",s);

}

void handlePlus(){
    valMot = valMot + 10;
    ledcWrite(canal, valMot);  
}
void handleMoins(){
    valMot = valMot - 10;
    ledcWrite(canal, valMot);
}


void webSetup() {
  Serial.println("WEB server setup...");
  server.on("/",handleRoot);
  server.on("/plus",handlePlus);
  server.on("/moins", handleMoins);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("WEB server running...");
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Setup...");
  wifiAPSetup();
  webSetup();
  Serial.println("Setup done.");
  pinMode(23, OUTPUT);
  ledcSetup(canal, frequence, resolution);
  ledcAttachPin(23, canal);


}
void loop() {
  server.handleClient();
}
