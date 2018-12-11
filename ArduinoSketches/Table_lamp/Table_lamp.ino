#include <Wire.h>
#include <FastLED.h>

#define NUM_LEDS   1
#define DATA_PIN   D5
#define I2C_SDL    D1
#define I2C_SDA    D2

CRGB leds[NUM_LEDS];
int gHue = 1;
unsigned int inputs;

void setup(void) {
  pinMode(DATA_PIN, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(96);
  
}

void readInput(){
   //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  inputs = Wire.read();  
  Serial.print("Digital in: ");
  Serial.println(inputs&0x0F);
}

unsigned int outputs=0;
void loop(void) {
 
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
  readInput();
  color();

}

void rainbow(){
  gHue++;
  fill_rainbow(leds,NUM_LEDS, gHue, 7);
}

void color(){
   if(inputs&0x0F == 15){
    rainbow();
    FastLED.show();
    Serial.println("light is ON"); 
  }else{
    leds[0].fadeToBlackBy(70);
    FastLED.show();
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void colorPicker(char color[]){

     if(color == "green"){
        leds[0] = CRGB::Green;
        FastLED.show();
     }
     if(color == "white"){
        leds[0] = CRGB::White;
        FastLED.show();
     }
     if(color == "black"){
        leds[0] = CRGB::Black;
        FastLED.show();
     }
     if(color == "red"){ 
        leds[0] = CRGB::Red;
        FastLED.show();
     }
     if(color == "blue"){
        leds[0] = CRGB::Blue;
        FastLED.show();
     }
     if(color == "yellow"){
        leds[0] = CRGB::Yellow;
        FastLED.show();
     }
  
}
