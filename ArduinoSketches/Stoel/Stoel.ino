#include "ESP8266WiFi.h"
#include <Wire.h>

#define AAN true
#define UIT false

const char* ssid = "RaspberryA4";
const char* password =  "kamerplant";
//const char* ssid = "PIWORKS";
//const char* password = "PInetPASSword123";

char buffer_array[5];
int analog[2];
String request;
const char* analogValueString;

WiFiServer wifiServer(3000);

void setup() {
  
  Wire.begin();

  Serial.begin(115200);

  delay(1000);

  setDDR(0x02);
  setOutput(0x02,UIT);     
     
  IPAddress ip(10,10,10,21); // where xx is the desired IP Address
  //IPAddress ip(192,168,4,9);
  IPAddress gateway(10,10,10,1); // set gateway to match your network 
  //IPAddress gateway(192,168,4,1);
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
  WiFi.config(ip, gateway, subnet);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();  

  
  
}

void loop() 
{

  WiFiClient client = wifiServer.available();

  if (client) 
  {

    while (client.connected()) 
    {

      while (client.available() > 0) 
      {
        request = client.readStringUntil('\r');
        Serial.print("Pi zond: ");        
        Serial.println(request);       
      }

      if(!strcmp(request.c_str(),"ChairVibrate:1"))
      {
        setOutput(0x02,AAN);
        request = "";    
      }
      if(!strcmp(request.c_str(),"ChairVibrate:0"))
      {
        setOutput(0x02,UIT);
        request = "";        
      }
      if(!strcmp(request.c_str(),"PressureSensor?"))
      {        
        Serial.println(analogValueString);        
        client.write(analogValueString,5);
        request = "";
      }
            
      readAnalog(analog); 
      analogValueString = itoa(analog[0], buffer_array, 10);
     
    }
      
      client.stop();
      Serial.println("Client disconnected");
  }
}  


void setDDR(int IO){
  
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(IO));         
  Wire.endTransmission();

  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));          
  Wire.write(byte(0x03));  
  Wire.endTransmission();
}

int readAnalog(int* analog_values){
  
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();  
  
  unsigned int anin1 = Wire.read()&0x03;  
  anin1=anin1<<8;
  anin1 = anin1|Wire.read();

  analog_values[0] = anin0;
  analog_values[1] = anin1;
  return anin1;
}

int readSwitch(int switch_bit){
  
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  int all_switches = Wire.read();
  int switch_address = 0;
  switch_address = 1 << (switch_bit - 1);
  int single_switch = all_switches & switch_address;

  return single_switch;
}

void setOutput(int output_address, bool output_status){
  
  if(output_status == AAN){    
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));            
    Wire.write(byte(output_address << 4));            
    Wire.endTransmission();
    Serial.println("Licht aan");
  }
  if(output_status == UIT){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));            
    Wire.write(byte(0 << 4));            
    Wire.endTransmission();
    Serial.println("Licht uit");
  } 
  
}

