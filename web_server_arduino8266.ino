#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "M_"
#define STAPSK  "03062013"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 15;

void handleToggle() {
 // Kuman writes his code here
 int pinState = LOW;
 if(digitalRead(led) == LOW) {
   pinState = HIGH;
 }
 digitalWrite(led, pinState);
 server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
 digitalWrite(led, 1);
 String message = "File Not Found\n\n";
 message += "URI: ";
 message += server.uri();
 message += "\nMethod: ";
 message += (server.method() == HTTP_GET) ? "GET" : "POST";
 message += "\nArguments: ";
 message += server.args();
 message += "\n";
 for (uint8_t i = 0; i < server.args(); i++) {
   message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
 }
 server.send(404, "text/plain", message);
 digitalWrite(led, LOW);
}

void setup(void) {
 pinMode(led, OUTPUT);
 digitalWrite(led, LOW);
 Serial.begin(115200);
 WiFi.begin(ssid, password);
 Serial.println("");

 // Wait for connection
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 String ipAddress = WiFi.localIP()
 Serial.println(ipAddress);

 if (MDNS.begin("esp8266")) {
   Serial.println("MDNS responder started");
 }

 server.on("/toggle/", handleToggle);

 server.onNotFound(handleNotFound);

 server.begin();
 Serial.println("HTTP server started");
}

void loop(void) {
 server.handleClient();
}
