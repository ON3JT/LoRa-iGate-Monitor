#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>


/************************************************************************************
 * Compute great-circle distance in km, using haversine formula
 * 
 * The haversine formula 'remains particularly well-conditioned for numerical 
 * computation even at small distances'
 * 
 * It was published by R W Sinnott in Sky and Telescope, 1984, though known about 
 * for much longer by navigators. (For the curious, c is the angular distance in 
 * radians, and a is the square of half the chord length between the points).
 * 
 * We don't adjust for altitude. Maybe in a future version.
 ************************************************************************************/
float Distance(float lat1, float lon1, float lat2, float lon2)
{
  float ToRad = PI / 180.0;
  float R = 6371;   // radius earth in Km
  
  float dLat = (lat2-lat1) * ToRad;
  float dLon = (lon2-lon1) * ToRad; 
  
  float a = sin(dLat/2) * sin(dLat/2) +
        cos(lat1 * ToRad) * cos(lat2 * ToRad) * 
        sin(dLon/2) * sin(dLon/2); 
        
  float c = 2 * atan2(sqrt(a), sqrt(1-a)); 
  
  float d = R * c;
  return d;
}


/************************************************************************************
* RSSI to Signal Strength routines
************************************************************************************/
typedef struct
{
    int RSSI;
    String Signal;
    
} SignalMeter;

SignalMeter Strength[110] = 
{
  {-142,"S0"},  {-141,"S1"},  {-140,"S1"},  {-139,"S1"},  {-138,"S1"},  {-137,"S1"},  {-136,"S1"},  {-135,"S2"},  {-134,"S2"},  {-133,"S2"},  {-132,"S2"},  {-131,"S2"},  {-130,"S2"},
  {-129,"S3"},  {-128,"S3"},  {-127,"S3"},  {-126,"S3"},  {-125,"S3"},  {-124,"S3"},  {-123,"S4"},  {-122,"S4"},  {-121,"S4"},  {-120,"S4"},  {-119,"S4"},  {-118,"S4"},  {-117,"S5"},
  {-116,"S5"},  {-115,"S5"},  {-114,"S5"},  {-113,"S5"},  {-112,"S5"},  {-111,"S6"},  {-110,"S6"},  {-109,"S6"},  {-108,"S6"},  {-107,"S6"},  {-106,"S6"},  {-105,"S7"},  {-104,"S7"},
  {-103,"S7"},  {-102,"S7"},  {-101,"S7"},  {-100,"S7"},  {-99,"S8"},   {-98,"S8"},   {-97,"S8"},   {-96,"S8"},   {-95,"S8"},   {-94,"S8"},   {-93,"S9"},   {-92,"S9"},   {-91,"S9"},
  {-90,"S9"},   {-89,"S9"},   {-88,"S9"},   {-87,"S9"},   {-86,"S9"},   {-85,"S9"},   {-84,"S9"},   {-83,"S9+10"},{-82,"S9+10"},{-81,"S9+10"},{-80,"S9+10"},{-79,"S9+10"},{-78,"S9+10"},
  {-77,"S9+10"},{-76,"S9+10"},{-75,"S9+10"},{-74,"S9+10"},{-73,"S9+20"},{-72,"S9+20"},{-71,"S9+20"},{-70,"S9+20"},{-69,"S9+20"},{-68,"S9+20"},{-67,"S9+20"},{-66,"S9+20"},{-65,"S9+20"},
  {-64,"S9+20"},  {-63,"S9+30"},  {-62,"S9+30"},  {-61,"S9+30"},  {-60,"S9+30"},  {-59,"S9+30"},  {-58,"S9+30"},  {-57,"S9+30"},  {-56,"S9+30"},  {-55,"S9+30"},  {-54,"S9+30"},  {-53,"S9+40"},
  {-52,"S9+40"},  {-51,"S9+40"},  {-50,"S9+40"},  {-49,"S9+40"},  {-48,"S9+40"},  {-47,"S9+40"},  {-46,"S9+40"},  {-45,"S9+40"},  {-44,"S9+40"},  {-43,"S9+50"},  {-42,"S9+50"},  {-41,"S9+50"},
  {-40,"S9+50"},  {-39,"S9+50"},  {-38,"S9+50"},  {-37,"S9+50"},  {-36,"S9+50"},  {-35,"S9+50"},  {-34,"S9+50"},  {-33,"S9+60"}
};

String GetSmeter(int Val) 
{
  String Smeter = "S0";
 
  for(int i=0;i<110;i++)
  {
    if(constrain(Val,-142,-33) == Strength[i].RSSI) 
    {  
      Smeter = Strength[i].Signal; 
    } 
  }
  return Smeter;
}


/************************************************************************************
* 
************************************************************************************/
float GetLat(String val)
{
  // !5110.28NL00428.03E>012/010/A=000081 Sats = 5 | LoRa Test
  
  float Part1 = 0;
  float Part2 = 0;
  float Latitude;
  
  String Direction = "";
  String Sentence = "";
  
  Direction = val.substring(8,9);
  if (Direction == "S") Sentence +="-";
  
  Part1 = val.substring(1,3).toFloat();
  Part2 = val.substring(3,8).toFloat();

  
  Part1 += Part2/60;
  Sentence +=  String((Part1),5);
  
  Latitude = Sentence.toFloat();
  return Latitude;
}

/************************************************************************************
* 
************************************************************************************/
float GetLon(String val)
{
  // !5110.28NL00428.03E>012/010/A=000081 Sats = 5 | LoRa Test
  
  float Part1 = 0;
  float Part2 = 0;
  float Longitude;
  
  String Direction = "";
  String Sentence = "";
  
  Direction = val.substring(18,19);
  if (Direction == "W") Sentence +="-";
  
  Part1 = val.substring(10,13).toFloat();
  Part2 = val.substring(13,18).toFloat();

  Part1 += Part2/60;
  Sentence +=  String((Part1),5);
 
  Longitude =Sentence.toFloat();
  return Longitude;
}


float GetLatCompressed(String val)
{
  // =/4XB;OY3{uUKH/A=-00017 Batt=3.77V,
  float Latitude = 0;
  Latitude = 90 - ((val[2]-33) * pow(91,3) + (val[3]-33) * pow(91,2) + (val[4]-33) * 91 + val[5]-33) / 380926;
  return Latitude;
}

float GetLonCompressed(String val)
{
  // =/4XB;OY3{uUKH/A=-00017 Batt=3.77V,
  float Longitude = 0;
  Longitude = -180 + ((val[6]-33) * pow(91,3) + (val[7]-33) * pow(91,2) + (val[8]-33) * 91 + val[9]-33) / 190463;
  return Longitude;
}

bool IsCompressed(String val)
{
  bool Compressed = false;
  Compressed = !isDigit(val[1]);
  return Compressed;
}

void ParseUploadData(String val)
{
  // example of val = <7>1 - ON3JT-10 ModemTask - - - ﻿[10:09:04] Received packet 'Source: ON3JT-9, Destination: APRS, Path: WIDE1-1, Type: Position Without Timestamp, Data: !5110.54NL00427.83E>147/011/A=-00014 Sats = 9 | LoRa Test' with RSSI -84dBm, SNR 10.00dB and FreqErr 3909Hz
  StaticJsonDocument<450> root;
  
  
  root["secret"] = SECRET;
  
  int index = 0;
  int index2 = 0;
  
  // Timestamp
  index = val.indexOf("[");
  root["time"] = val.substring(index+1,index+9);
  
  //Call
  index = val.indexOf("Source");
  index2 = val.indexOf("Destination");
  root["call"] = val.substring(index+8,index2-2);

  // Destination
  index = val.indexOf("Destination");
  index2 = val.indexOf("Path");
  root["destination"] = val.substring(index+13,index2-2);

  // Path
  index = val.indexOf("Path");
  index2 = val.indexOf("Type");
  root["path"] = val.substring(index+6,index2-2);
  
  // RSSI
  index = val.indexOf("RSSI");
  index2 = val.indexOf("SNR");
  root["rssi"] = val.substring(index+5,index2-5);
  root["signal"] = GetSmeter(val.substring(index+5,index2-5).toInt());
  
  // SNR
  index = val.indexOf("SNR");
  index2 = val.indexOf("FreqErr");
  root["snr"] = val.substring(index+4,index2-7);
  
  // Data
  index = val.indexOf("Data:");
  index2 = val.indexOf("with");
  String RawData = val.substring(index+6,index2-2);
  RawData.trim();
  float Lat = 0;
  float Lon = 0;
  if (IsCompressed(RawData))
  {
    Lat = GetLatCompressed(RawData);
    Lon = GetLonCompressed(RawData);
  }
  else
  {
    Lat = GetLat(RawData);
    Lon = GetLon(RawData);
  }
  root["payload"] = RawData;
  root["latitude"] = String(Lat,6);
  root["longitude"] = String(Lon,6);
  root["distance"] = String(Distance(IGATE_LAT, IGATE_LON, Lat, Lon),2);

  Serial.println("[API] : JSON data to upload:");
  serializeJsonPretty(root, Serial);
  Serial.println();     
  
  String json;
  serializeJson(root, json);


  HTTPClient https;   
  
  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  client.connect(JSON_URL, 443);
  
  https.begin(client,JSON_URL);  
  https.addHeader("Content-Type", "application/json");         
  https.addHeader("accept", "text/plain");
   
  Serial.println("[API] : Uploading data...");
  Serial.print("[API] : Response code: ");
  int httpResponseCode = https.PUT(json);
  if(httpResponseCode>0)
  {
    String response = https.getString();                       
    Serial.print(httpResponseCode); Serial.print(" - "); Serial.println(response); 
  }
  else 
  { 
    Serial.printf("Error code: %d\n",httpResponseCode);
    Serial.printf("Error occurred while sending HTTP POST: %s\n", https.errorToString(httpResponseCode).c_str());  
  }
  https.end();
}
