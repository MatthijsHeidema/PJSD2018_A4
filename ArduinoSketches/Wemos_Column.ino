#include "ESP8266WiFi.h"
#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
#define AAN true
#define UIT false

char buffer_array[3];
int i = 0, analog[2];
bool rookMelder, Rookstatus;

String request;
const char* analogValueString;

WiFiServer wifiServer(3015);

const char* ssid = "PIWORK";
const char* password = "PASSword";
  
// Static ip address
IPAddress ip(192, 168, 4, 11);         // where xx is the desired IP Address
IPAddress gateway (192, 168, 4, 1);   // set gateway to match your network 
IPAddress subnet(255, 255, 255, 0);   // set subnet mask to match your network
  
void wifiSetup()
{
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
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

void setOutput(int output_address, bool output_status) // Function for setting the output registers on the WIB. Requires the specific bit and true or false
{
  if(output_status == AAN){    
    Wire.beginTransmission(0x38);           // Send a write byte to the I2C device
    Wire.write(byte(0x01));                 // Telling the I2C device what byte we want to write to
    Wire.write(byte(output_address << 4));  // Setting the bits           
    Wire.endTransmission();                 // Ending the transmission. An I2C stop byte has been sent
  }
  if(output_status == UIT){
    Wire.beginTransmission(0x38);           // Send a write byte to the I2C device
    Wire.write(byte(0x01));                 // Telling the I2C device what byte we want to write to
    Wire.write(byte(0 << 4));               // Setting the bits
    Wire.endTransmission();                 // Ending the transmission. An I2C stop byte has been sent
  }
}

void readAnalog(int* analog_values)          // Function for reading an 10 bit analog value from the WIB. Requires an pointer to an analog values array
{         
  Wire.requestFrom(0x36, 4);                 // Send a read byte to the I2C device
  unsigned int anin0 = Wire.read()&0x03;     // Reading the 2 Most Significant Bits from analog 0 input on the WIB
  anin0=anin0<<8;                            // Shifting these 2 bits 8 places to give these bits their original value
  anin0 = anin0|Wire.read();                 // Store the last 8 bits to their original value. (OR comparision 1 - 0 == 1)
  
  unsigned int anin1 = Wire.read()&0x03;     // Reading the 2 Most Significant Bits from analog 1 input on the WIB
  anin1=anin1<<8;                            // Shifting these 2 bits 8 places to give these bits their original value
  anin1 = anin1|Wire.read();                 //Store the last 8 bits to their original value. (OR comparision 1 - 0 == 1
  
  Serial.print("analog in 0: ");
  Serial.println(anin0);    
  Serial.println("");
  /*Serial.print("analog in 1: ");
  Serial.println(anin1);   
  Serial.println("");*/
  analog_values[0] = anin0;
  analog_values[1] = anin1;
}

void setup() {
  Wire.begin();                         // Starting the I2C communication
  Serial.begin(115200);                 // Starting the I2C communication
  wifiSetup();                          // Starting the WiFi server
}

void loop() 
{

  WiFiClient client = wifiServer.available();

  if (client) 
  {

    while (client.connected()) 
    {
      Serial.println("success");
      while (client.available() > 0) 
      {
        request = client.readStringUntil('\r');
        Serial.print("Pi zond: ");        
        Serial.println(request);       
      }

      if(!strcmp(request.c_str(),"RookMelderStatus:1"))
      {
        setOutput(0x01,AAN); // Buzzer aan 
        Serial.print("Buzzer ON");
        request = "";    
      }
      if(!strcmp(request.c_str(),"RookMelderStatus:0"))
      {
        setOutput(0x01,UIT); // Buzzer uit
        Serial.print("Buzzer OFF");
        request = "";        
      }
      if(!strcmp(request.c_str(),"Buzzer?"))
      {        
        Serial.println(analogValueString);        
        client.write(analogValueString,5);
        request = "";
      }

         
      readAnalog(analog); 
      analogValueString = itoa(analog[0], buffer_array, 10);
       Serial.print(analogValueString);
    }
      
      client.stop();
      Serial.println("Client disconnected");
  }
 
} 
