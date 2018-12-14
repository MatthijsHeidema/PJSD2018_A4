#include <Wire.h>             //Wire lib for i2c
#include <ESP8266WiFi.h>      //Wifi lib for socket communication
#include <FastLED.h>

#define NUM_LEDS   1
#define DATA_PIN   D5

#define I2C_SDL    D1
#define I2C_SDA    D2

CRGB led[NUM_LEDS];


IPAddress ip(192, 168, 3, 1);
IPAddress gateway (192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid     = "PIWORKS";
const char* password = "PInetPASSword123";

int delayval = 500;
unsigned int outputs = 0;

String request = " ";
int modus = 0;
int ledval = 0;
int button = 0;
int gHue;
unsigned int inputs;
bool state;

WiFiServer wifiServer(3000);
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

void colorPicker(String color){
  if(state){
       if(color == "ToggleLed:1Color:green"){
          led[0] = CRGB::Green;
          FastLED.show();
       }
       if(color == "Color:White"){
          led[0] = CRGB::White;
          FastLED.show();
       }
       if(color == "Color:Red"){ 
          led[0] = CRGB::Red;
          FastLED.show();
       }
       if(color == "Color:Blue"){
          led[0] = CRGB::Blue;
          FastLED.show();
       }
       if(color == "Color:Yellow"){
          led[0] = CRGB::Yellow;
          FastLED.show();
       }
  }
}

void Status(String request){
  if(request == "ToggleLed:0"){
    state = false;
  }else if(request == "ToggleLed:1"){
    state = true;
  }
}

void loop() {
   WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        request = client.readStringUntil('\r');
        Serial.println(request);        
      }

      colorPicker(request);
                  
      delay(250);      
    }

    client.stop();
    Serial.println("Client disconnected");
  }else{
    buttons();
    color();
  }
  
}
