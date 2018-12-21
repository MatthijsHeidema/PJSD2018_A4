#include "ESP8266WiFi.h"
#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
#define AAN true
#define UIT false

const char* ssid = "RaspberryA4";
const char* password =  "kamerplant";
const char* switchStatusChange = "0";
char buffer_array[4];
int i = 0, analog[2], movement_counter;
bool new_data_flag = false, led_status = false, led_update = false, in_bed = false, 
     epilepsy_attack = false, bed_update = false, epilepsy_timer_start = true;
int previousSwitchStatus;
String request;
unsigned char schakelaar;
unsigned long time_led_on, time_led_current, time_in_bed, time_bed_current;
const char* analogValueString;

WiFiServer wifiServer(3000);

void setup() {
  
  Wire.begin();

  Serial.begin(115200);

  //delay(1000);

  setDDR(0x01);
  setOutput(0x01,UIT);     
     
  IPAddress ip(10,10,10,20); // where xx is the desired IP Address
  //IPAddress ip(192,168,4,5);
  IPAddress gateway(10,10,10,1); // set gateway to match your network 
  //IPAddress gateway(192,168,4,1);
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

  
  
}

void loop() {


  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        request = client.readStringUntil('\r');        
        Serial.println(request);               
       
      }

      if(request == "BedLight:?")
      {
        client.write(switchStatusChange,2);
        Serial.print("Switch status verandering: ");
        Serial.println(switchStatusChange);        
        request = "";
        switchStatusChange = "0";
      }
      if(request == "BedLight:1")
      {
        setOutput(0x01,AAN);
        request = "";    
      }
      if(request == "BedLight:0")
      {
        setOutput(0x01,UIT);
        request = "";
      }

      if(request == "PressureSensor:?")
      {        
        analogValueString = itoa(analog[0], buffer_array, 10);
        client.write(analogValueString,sizeof(analogValueString));
        request = "";
      }
            
      //delay(250);
      
      int switchStatus = readSwitch(1);
      readAnalog(analog);     
      
      if(switchStatus == 0 && previousSwitchStatus == 1){
        switchStatusChange = "1";
        Serial.println("Knop gedrukt");
      }
      
      previousSwitchStatus = switchStatus;
      
    }

    client.stop();
    Serial.println("Client disconnected");
    
  }

/*
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

        time_led_on = millis();
        
        led_status = AAN;
        led_update = false;
        Serial.println("Leds aan.");           
    }
    
  }

  if(schakelaar == 1){
    led_update = true;
  }

  if(led_status == AAN){
    time_led_current = millis();
  }

  if(time_led_current - time_led_on > 5000){

    setOutput(0x01,UIT);

    led_status = UIT;
    led_update = false;

    time_led_on = time_led_current;
    Serial.println("Automatisch uit.");
  }
  
  
  readAnalog(analog);
  //Serial.println(analog[0]);
  //Serial.println(analog[1]);
  time_bed_current = millis();

  if((analog[0] > 500) & (bed_update == true)){
    in_bed = true;
    bed_update = false;
    Serial.println("In bed.");
    movement_counter++;    
    if(epilepsy_timer_start == true){
      time_in_bed = millis();
      epilepsy_timer_start = false;
    }            
  }else if(analog[0] < 500){
    in_bed = false;
    bed_update = true;
    //Serial.println("Uit bed.");
  }

  if(movement_counter >= 5){
    epilepsy_attack = true;
    Serial.println("EPILEPSIE AANVAL GEDETECTEERD");
  }

  if((time_bed_current - time_in_bed > 8000) & (epilepsy_attack == false)){
    movement_counter = 0;
    epilepsy_timer_start = true;
    time_in_bed = time_bed_current;
    Serial.println("Epilepsie timer gereset");  
  }
  
  //delay(500);

  */
  
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

