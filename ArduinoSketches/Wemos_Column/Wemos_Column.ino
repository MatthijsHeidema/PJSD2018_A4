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
unsigned char schakelaar;
unsigned long time_led_on, time_led_current, time_in_bed, time_bed_current;

String request;
const char* preceived = "";
char key[64];
char value[64];
bool state;

WiFiServer wifiServer(3000);

const char* ssid = "PIWORKS";
const char* password = "PInetPASSword123";
  
// Static ip address
IPAddress ip(192, 168, 4, 4);         // where xx is the desired IP Address
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

int splitInto(const char* received, char key[64], char value[64]) {

  const char *tracker = received;     //pointer to go through received
  int command = 0;                    //differentiate between key and value

  //Empty key and value arrays
  memset(&key[0], 0, 64);
  memset(&value[0], 0, 64);

  for (int i = 0; i < 63 && *tracker; i++, tracker++) { //loops untill \0 is found or max size of arrays is reached

    if (*tracker == ':') {
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
  if (command) return 0;                              //if neither ':' nor '?' was found before end of received, it is invalid
  return 1;
}

void Status(char value[]){
  if (!strcmp(value, "0")) {
    state = false;
  } else if (!strcmp(value, "1")) {
    state = true;
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

void setup() {
  Wire.begin();                         // Starting the I2C communication
  Serial.begin(115200);                 // Starting the I2C communication
  wifiSetup();                          // Starting the WiFi server
}

void loop() {
  WiFiClient client = wifiServer.available();

  if (client)
  {
    while (client.connected())
    {
      while (client.available() > 0)
      {
        request = client.readStringUntil('\r');
        preceived = request.c_str();
        Serial.println(preceived);
        Serial.print("De pi zond: ");
        Serial.println(request);
        //CODE HERE!!!
        smokeAlarm();
        client.write(analog[0]);  // send to analog value to server
        client.write(Rookstatus); // send Alarm status to server
        client.write(rookMelder); // send Buzzer status to server
      }

      if (!splitInto(preceived, key, value))
      {
        Serial.print("key: ");
        Serial.print(key);
        Serial.print("; value: ");
        Serial.println(value);

        if (!strcmp(key, "ToggleLed"))
        {
          //Status(value);

        } else if (!strcmp(key, "Color"))
        {
          //colorPicker(value);

        } else if (!strcmp(key, "PIR"))
        {
          //Send PIR value
          //String pir_val = String(inputs & 0x0F);
          //client.write(pir_val.c_str(), 64);
        } else
        {
          Serial.println("Unknown command");
        }
      } else
      {
        Serial.println("Invalid input");
      }
    }
  }
}
