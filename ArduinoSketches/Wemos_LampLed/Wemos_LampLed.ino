#include <Wire.h>             //Wire lib for i2c
#include <ESP8266WiFi.h>      //Wifi lib for socket communication
#include <FastLED.h>

#define NUM_LEDS   1
#define DATA_PIN   D5

#define I2C_SDL    D1
#define I2C_SDA    D2

CRGB led[NUM_LEDS];


IPAddress ip(172, 16, 4, 4);
IPAddress gateway (172, 16, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid     = "WinnieThePi";
const char* password = "3114800R";

int delayval = 500;
unsigned int outputs = 0;

String received = " ";
const char* preceived = "";
char key[64];
char value[64];

int modus = 0;
int ledval = 0;
int button = 0;
int gHue;
unsigned int inputs;
bool state;
//char color[64] = "";

WiFiServer wifiServer(3005);
void wemosFi();
int buttons();

void setup() {
  Serial.begin(115200);
  Serial.println("Serial geladen");
  delay(10);
  Wire.begin();
  Serial.println("Wire begin");
  pinMode(D5, OUTPUT);
  wemosFi();
  leds(15);
  delay(1000);
  leds(0);
  delay(1000);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(led, NUM_LEDS);

}

void loop() {
   WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        received = client.readStringUntil('\r');
        preceived = received.c_str();
        Serial.println(preceived);        
      }

     if(!splitInto(preceived, key, value)) {

        Serial.print("key: ");
        Serial.print(key);
        Serial.print("; value: ");
        Serial.println(value);
        
        if(!strcmp(key, "ToggleLed")) {
          Status(value);
          
        } else if (!strcmp(key, "Color")) {
          colorPicker(value);
          
        } else if (!strcmp(key, "PIR")) {
          //Send PIR value
          String pir_val = String(inputs&0x0F);
          client.write(pir_val.c_str(), 64);
        
        } else {
          Serial.println("Unknown command");
        }
        
      } else {
        Serial.println("Invalid input");
      }

      //colorPicker(color);
                  
      delay(250);      
    }

    buttons();
    color();

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

int buttons()
{
  //Serial.println("begin but");
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  inputs = Wire.read();
  //Serial.println("eind but");
  return (inputs);
}

void leds(int i)
{
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(i << 4));
  Wire.endTransmission();
}

void rainbow(){
  gHue++;
  fill_rainbow(led,NUM_LEDS, gHue, 7);
  FastLED.show();
}

void color(){
   if(inputs&0x0F == 15){
    rainbow();
    //Serial.println("light is ON"); 
  }else{
    led[0].fadeToBlackBy(70);
    FastLED.show();
    //Serial.println("light is OFF");
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( led, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  led[pos] += CHSV( gHue + random8(64), 200, 255);
}

void colorPicker(const char* color){
  if(state){
       if(!strcmp(color,"Green")){
          led[0] = CRGB::Green;
          FastLED.show();
       }
       if(!strcmp(color,"White")){
          led[0] = CRGB::White;
          FastLED.show();
       }
       if(!strcmp(color,"Red")){ 
          led[0] = CRGB::Red;
          FastLED.show();
       }
       if(!strcmp(color,"Blue")){
          led[0] = CRGB::Blue;
          FastLED.show();
       }
       if(!strcmp(color,"Yellow")){
          led[0] = CRGB::Yellow;
          FastLED.show();
       }
  }
}

void Status(char value[]){
  if(!strcmp(value, "0")) {
    state = false;
  }else if(!strcmp(value, "1")) {
    state = true;
  }
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
