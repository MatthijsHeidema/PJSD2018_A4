#include "ESP8266WiFi.h"
#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
#define AAN true
#define UIT false

char buffer_array[3];
int i = 0, analog[2], movement_counter;
bool new_data_flag = false, led_status = false, led_update = false, in_bed = false, 
     epilepsy_attack = false, bed_update = false, epilepsy_timer_start = true, rookMelder, Rookstatus;
String request;
unsigned char schakelaar;
unsigned long time_led_on, time_led_current, time_in_bed, time_bed_current;

WiFiServer wifiServer(3000);

void wemosFi()
{

  const char* ssid = "PIWORKS";
  const char* password = "PInetPASSword123";
  
  // Static ip address
  IPAddress ip(192, 168, 4, 4);         // where xx is the desired IP Address
  IPAddress gateway (192, 168, 4, 1);   // set gateway to match your network 
  IPAddress subnet(255, 255, 255, 0);   // set subnet mask to match your network
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // While wifi is not connected 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
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

void setOutput(int output_address, bool output_status)
{
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

void readAnalog(int* analog_values)
{
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();  
  
  unsigned int anin1 = Wire.read()&0x03;  
  anin1=anin1<<8;
  anin1 = anin1|Wire.read();
  
  Serial.print("analog in 0: ");
  Serial.println(anin0);    
  Serial.println("");
  /*Serial.print("analog in 1: ");
  Serial.println(anin1);   
  Serial.println("");*/
 
  analog_values[0] = anin0;
  analog_values[1] = anin1;

    
  
}

int readSwitch(int switch_bit)
{
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

void button(int schakelaar)
{
  if(schakelaar == 0){
  
    if((led_update == true) & (led_status == AAN))
    {        
        //setOutput(0x02,UIT);
        led_status = UIT;
        led_update = false;
        Serial.println("Leds uit.");           
    }
    if((led_update == true) & (led_status == UIT))
    { 
        //setOutput(0x02,AAN);
        led_status = AAN;
        led_update = false;
        Serial.println("Leds aan.");           
    }
  }else if(schakelaar == 1){
    led_update = true;
  }
}

void smokeAlarm()
{
  readAnalog(analog);
  button(readSwitch(1));
  
  if(readSwitch(1) == 1){
      rookMelder = UIT;
      setOutput(0x02,UIT);
      Rookstatus = AAN;
   }
   else if(rookMelder){
      //setOutput(0x01,AAN); // BUZZER !!! UNCOMMENT ON YOUR OWN RISK
      setOutput(0x02,AAN);
      Serial.println("BUZZER!!!");
   }
   else if(Rookstatus and (analog[0] > 300)){
      Rookstatus = UIT;
   }
   else if((analog[0] < 300) and Rookstatus == UIT){
      rookMelder = AAN;
    }
}

void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  delay(1000);
  setDDR(0x01);
 // wemosFi(); 
}

void loop() 
{
  WiFiClient client = wifiServer.available();

  if (client) {
    while (client.connected()) {
      while (client.available() > 0) {
        request = client.readStringUntil('\r');
        Serial.println(request);        
      }
      //CODE HERE!!!
        smokeAlarm();
        client.write(analog[0]);  // analog value
        client.write(Rookstatus); // Alarm status 
        client.write(rookMelder); // Buzzer status
      delay(250); 
    }
    client.stop();
    Serial.println("Client disconnected");
  }else{
    
    
  }
   
}
