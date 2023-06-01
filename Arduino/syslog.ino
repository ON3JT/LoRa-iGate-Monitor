#include <WiFiUdp.h>

WiFiUDP UDP;

char packet[300];
//char Task[] = "ModemTask";
char Task[] = "RadiolibTask";

String APRSdata ="";

void StartSyslogServer()
{
  UDP.begin(UDP_PORT);
  Serial.print("[UDP] : UDP Monitor started on port ");
  Serial.println(UDP_PORT);
}

void MonitorUDP()
{
  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) 
  {
    int len = UDP.read(packet, packetSize);
    if (len > 0)
    {
      packet[len] = '\0';
     // Serial.print("[Raw] : ");
     // Serial.println(packet);
      
      if (strstr (packet, Task))
      {
        //if(!strstr (packet, "failed") || !strstr(packet,"Transmitting") || !strstr(packet, "TX done"))
        if(strstr (packet, "Received packet"))
        {
          Serial.println("[UDP] : UDP Packet Received.");
          digitalWrite(LED_BUILTIN, HIGH);
          delay(100);
          digitalWrite(LED_BUILTIN, LOW);
          APRSdata = String(packet); 
          ParseUploadData(APRSdata);
        }
      } 
    }
  }
  
}
