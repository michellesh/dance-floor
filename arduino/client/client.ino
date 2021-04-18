#include "secrets.h"
#include <SPI.h>
#include <ESP8266WiFi.h>

byte buttonPin = 0;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient client;
IPAddress server(192, 168, 0, 0);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected!");
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop () {
  if (digitalRead(buttonPin) == 0) {
    client.connect(server, 80);
    client.println("blink");
    client.flush();
  }
}
