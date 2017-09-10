/**
 * Test fetching data from a server
 * 09/2017
 */

#include <ESP8266WiFi.h>
#include "credentials.h"

// ---- WiFi credentials
const char* ssid = WIFI_SSID; // defined in credentials.h
const char* password = WIFI_PASSWORD; // defined in credentials.h

// ---- API data
const char* host = "www.colourlovers.com";
const int httpPort = 80; // port 80 for http, port 443 for https
const char* url = "/api/colors/random?format=json";


/**
 * 
 */
void setup() {
  
  Serial.begin(115200);
  delay(10);

  // ---- connect to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}




/**

*/
void loop() {

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  // if httpS is needed use WiFiClientSecure / 443
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println(String("Fetching http://") + host + url  + " ...");

  // ---- This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // ---- Read all the lines (header and body) of the response from server and print to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  // ---- sleep 5 seconds
  Serial.println("sleeping 30 seconds");
  delay( 30 * 1000);
}




