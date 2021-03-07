#include "WiFiCredentials.h"
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
String mode;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiServer server(80);
IPAddress ip(192, 168, 0, 0);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected!");
  server.begin();
  pinMode(ledPin, OUTPUT);
}

void blinkFast(int numTimes) {
  for(int i = 0; i < numTimes; i++) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    delay(100);
  }
}

void blinkSlow() {
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      String request = client.readStringUntil('\r');
      Serial.println("From client: " + request);
      if (request == "blink") {
        blinkFast(5);
      }
      client.flush();
    }
    client.stop();
  }
  blinkSlow();
}
