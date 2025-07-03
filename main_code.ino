#include <WiFi.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDRPIN 36
#define PIRPIN 34
#define RELAYPIN 2

const char* ssid = "Mahtorfi";
const char* password = "Andy8654";

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(PIRPIN, INPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    float temperature = dht.readTemperature();
    int ldrValue = analogRead(LDRPIN);
    int motion = digitalRead(PIRPIN);

    if (motion == HIGH && ldrValue < 1500 && temperature > 30) {
      digitalWrite(RELAYPIN, HIGH);
    } else {
      digitalWrite(RELAYPIN, LOW);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<h1>Home Automation Dashboard</h1>");
    client.print("<p>Temperature: "); client.print(temperature); client.println(" C</p>");
    client.print("<p>Light Level: "); client.print(ldrValue); client.println("</p>");
    client.print("<p>Motion Detected: "); client.print(motion); client.println("</p>");
    client.print("<p>Relay Status: "); client.print(digitalRead(RELAYPIN)); client.println("</p>");

    client.println();
    delay(2000);
  }
}
