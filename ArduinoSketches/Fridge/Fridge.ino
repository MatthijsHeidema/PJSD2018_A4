#include <ESP8266WiFi.h>
#include <Wire.h>

#define I2C_SDL D1
#define I2C_SDA D2
#define AAN 1
#define UIT 0

WiFiServer wifiServer(3020);
const char* ssid = "WinnieThePi";
const char* password = "3114800R";
IPAddress ip(172, 16, 4, 20);
IPAddress gateway (172, 16, 4, 1);
IPAddress subnet(255, 255, 255, 0);

String received;
char key[64] = {0};
char value[64] = {0};
char temp_buffer[16] = {0};
char door_buffer[16] = {0};

int analog[2] = {0};
int coolingOn = 1;
int doorSwitch = 0;

//functions
void setDDR(int IO);
void setOutput(int output_address, bool output_status);
void wemosFi();
int readSwitch(int switch_bit);
int readAnalog(int* analog_values);
int splitInto(const char* received, char key[64], char value[64]);



void setup() {
  Serial.begin(115200);
  delay(10);
  Wire.begin();
  pinMode(D5, OUTPUT);
  setDDR(0x01);
  setOutput(0x01, UIT);
  wemosFi();
}

void loop() {
  WiFiClient client = wifiServer.available();

  if(client) {

    while(client.connected()) {

      while(client.available() > 0) {
        received = client.readStringUntil('\r');
      }

      if(!splitInto(received.c_str(), key, value)) {
        
        if(!strcmp(key, "Door")) {
          //send Door value
          doorSwitch = readSwitch(1);
          itoa(doorSwitch, door_buffer, 10);
          Serial.println(door_buffer);
          client.write(door_buffer, 16);
          
        
        } else if(!strcmp(key, "PeltierTemp")) {
          //send temperature
          readAnalog(analog);
          itoa(analog[0], temp_buffer, 10);
          Serial.println(temp_buffer);
          client.write(temp_buffer, 16);
        
        } else if(!strcmp(key, "FanTemp")) {
          readAnalog(analog);
          itoa(analog[1], temp_buffer, 10);
          Serial.println(temp_buffer);
          client.write(temp_buffer, 16);
        
        }else if(!strcmp(key, "Cooling")) {          
          if (!strcmp(value, "1")) {
            //peltier
            Serial.println("Cooling");
            analogWrite(D5, 255);
          } else {
            //peltier off
            Serial.println("Not cooling");
            analogWrite(D5, 0);
          }
        
        } else if (!strcmp(key, "Fan")) {
          if (!strcmp(value, "1")) {
            setOutput(0x01, AAN);
          } else {
            setOutput(0x01, UIT);
          }
        }

        received = "";
      
      } else {
        //
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}

void wemosFi()
{
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

int splitInto(const char* received, char key[64], char value[64]) {

  const char *tracker = received;     //pointer to go through received
  int command = 0;                    //differentiate between key and value

  //Empty key and value arrays
  memset(&key[0], 0, 64);
  memset(&value[0], 0, 64);
  
  for(int i = 0; i < 63 && *tracker; i++, tracker++) { //loops untill \0 is found or max size of arrays is reached
    
    if(*tracker == ':') {
      command = 1;                                     //received is a command, the rest of the input is the value associated with it
      i = -1;                                          //reset i (will become 0 before next iteration)
      continue;
    }
    
    if (command) {                                     //if ':' was found, write the rest of the string to value
      value[i] = *tracker;
    
    } else if (*tracker == '?') {                     //if '?' is found, received is a request, no value is given
      value[0] = '?';                                 //write "?" to value
      return 0;                                       //return
    
    } else {                                          //if neither ':' nor '?' was found, continue writing to key
      key[i] = *tracker;
    }
  }
  
  if(command) return 0;                               //if neither ':' nor '?' was found before end of received, it is invalid
  return 1;
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
