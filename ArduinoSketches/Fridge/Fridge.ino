#include <ESP8266WiFi.h>
#include <Wire.h>

#define I2C_SDL D1
#define I2C_SDA D2

WiFiServer wifiServer(3005);
const char* ssid = "";
const char* password = "";
IPAddress ip(192, 168, 4, 16);
IPAddress gateway (192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char* received = "";
char key[64] = "";
char value[64] = "";

int analog[2] = {0};
int set_temp = 0;
int cur_temp = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  Wire.begin();
  pinMode(D5, OUTPUT);
  wemosFi();
}

void loop() {
  WiFiClient client = wifiServer.available();

  if(client) {

    while(client.connected()) {

      while(client.available() > 0) {
        received = client.readStringUntil('\r').c_str();
      }

      if(!splitInto(received, key, value)) {

        if(!strcmp(key, "Door")) {
          //send Door value
        
        } else if(!strcmp(key, "Temp") && !strcmp(value, "?")) {
          //send temperature
        
        } else if(!strcmp(key, "Temp")) {
          set_temp = atoi(value);
        
        } else {
          Serial.println("Unknown command!");
        }
      
      } else {
        Serial.println("Invalid input");
      }

      delay(250);
    }

    client.stop();
    Serial.println("Client disconnected");
  }

  readAnalog(analog);

  cur_temp = analog[0];

  if(cur_temp > set_temp) {
    //peltier on
  } else {
    //peltier off
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
