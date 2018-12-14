#include "ESP8266WiFi.h"
#include <Wire.h>

#define AAN true
#define UIT false

const char* ssid = "RaspberryA4";
const char* password =  "kamerplant";
bool new_data_flag = false, led_status = false, led_update = false;
String request;
unsigned char schakelaar;

WiFiServer wifiServer(80);

void setup() {
  
  Wire.begin();

  Serial.begin(115200);

  delay(1000);

  setDDR(0x01); 

  /*    
     
  IPAddress ip(10,10,10,20); // where xx is the desired IP Address
  IPAddress gateway(10,10,10,1); // set gateway to match your network 
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
  WiFi.config(ip, gateway, subnet);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();  

  */
  
}

void loop() {


  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        request = client.readStringUntil('\r');
        Serial.println(request);        
      }

      if (request == " ") {

        request = "";
      }
                  
      delay(250);      
    }

    client.stop();
    Serial.println("Client disconnected");
  }

  schakelaar = readSwitch(1);

  if(schakelaar == 0){
    
    if((led_update == true) & (led_status == AAN)){        

        setOutput(0x01,UIT);
        
        led_status = UIT;
        led_update = false;
        Serial.println("Leds uit.");           
    }
    if((led_update == true) & (led_status == UIT)){ 
            
        setOutput(0x01,AAN);
        
        led_status = AAN;
        led_update = false;
        Serial.println("Leds aan.");           
    }
    
  }

  if(schakelaar == 1){
    led_update = true;
  }
  
  //delay(500);
  
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

void readAnalog(int* analog_values){
  
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();  
  
  unsigned int anin1 = Wire.read()&0x03;  
  anin1=anin1<<8;
  anin1 = anin1|Wire.read();
  /*
  Serial.print("analog in 0: ");
  Serial.println(anin0);    
  Serial.println("");
  Serial.print("analog in 1: ");
  Serial.println(anin1);   
  Serial.println("");
  */
  analog_values[0] = anin0;
  analog_values[1] = anin1;
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
  /*
  Serial.print("Bit adres: ");
  Serial.println(switch_address);
  Serial.print("Waarde: ");
  Serial.println(single_switch);
  */
  return single_switch;
}

void setOutput(int output_address, bool output_status){
  
  if(output_status == AAN){    
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));            
    Wire.write(byte(output_address << 4));            
    Wire.endTransmission();
  }
  if(output_status == UIT){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));            
    Wire.write(byte(0 << 4));            
    Wire.endTransmission();
  }
  
  
}

