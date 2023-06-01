#include <WiFi.h>
#include "config.h"


void setup() 
{
  pinMode (LED_BUILTIN, OUTPUT);
// Setup serial port
  Serial.begin(115200);
  Serial.println();
  delay(2000);
  Serial.println();
  Serial.println();
  Serial.println("--< LoRa iGate Monitor by ON3JT >--");
  Serial.println();
  
  // Begin WiFi
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(HOSTNAME.c_str()); 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Connecting to WiFi...
  Serial.println("[WiFi] : Starting WiFi.");
  Serial.print("[WiFi] : Connecting to ");
  Serial.print(WIFI_SSID);
  
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  
  // Connected to WiFi
  Serial.println();
  Serial.print("[WiFi] : Connected!, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("[WiFi] : Hostname: ");
  Serial.println(WiFi.getHostname());
  Serial.print("[WiFi] : RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // Start UDP monitor
  StartSyslogServer();
  
}

void loop() 
{
  MonitorUDP();
}
