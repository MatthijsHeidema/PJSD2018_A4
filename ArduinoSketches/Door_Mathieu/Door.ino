#include <Wire.h>
#include <Servo.h>
#include "ESP8266WiFi.h"

#define I2C_SDL   D1
#define I2C_SDA   D2
#define AAN true
#define UIT false

int button1 = 0;

const char* ssid = "PIWORKS";
const char* password =  "PInetPASSword123";
String received;
char key[64];
char value[64];
bool state;
char buffer_array[4];
const char* ValueString;

int analogValues[2];
void Status(char value[]);
void wifiSetup();

IPAddress ip(192, 168, 4, 10); // where xx is the desired IP Address
IPAddress gateway(192, 168, 4, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

WiFiServer wifiServer(3010);
Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(14);  // attaches the servo on pin D2 to the servo object
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
        received = client.readStringUntil('\r');
      }

      if (!splitInto(received.c_str(), key, value)) {

        Serial.print("key: ");
        Serial.print(key);
        Serial.print("; value: ");
        Serial.println(value);

        if (!strcmp(key, "doorStatus")) {
          Status(value);
          if (state == 1)
          {
            openDoor();
          } else {
            closeDoor();
          }

        } else if (!strcmp(key, "redLight")) {
          Status(value);
          if (state == 1)
          {
            Serial.println("Led ON");
            writeToOutput(00000010);
          }
          else
          {
            Serial.println("Led OFF");
            writeToOutput(00000000);
          }

        } else if (!strcmp(key, "redButton")) {
          ValueString = itoa(readSwitch(2), buffer_array, 10);
          client.write(ValueString, sizeof(ValueString));

        } else {
          Serial.println("Unknown command");
        }

        received = "";
      } else {
        //Serial.println("invalid input");
      }
      //Serial.println(readSwitch(00000001));
    }
  }
}

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

void writeToOutput(byte outputs) { // Function for setting the output registers on the WIB. Requires the specific bit and true or false

  Wire.beginTransmission(0x38);                                // Send a write byte to the I2C device
  Wire.write(byte(0x01));                                      // Telling the I2C device what byte we want to write to
  Wire.write(byte(outputs << 4));                              // Setting the bits
  Wire.endTransmission();                                      // Ending the transmission. An I2C stop byte has been sent
}

void readFromAnalog(int* analog_values) {            // Function for reading an 10 bit analog value from the WIB. Requires an pointer to an analog values array
  Wire.requestFrom(0x36, 4);                            // Send a read byte to the I2C device

  unsigned int analogInput0 = Wire.read() & 0x03;             // Reading the 2 Most Significant Bits from analog 0 input on the WIB
  analogInput0 = analogInput0 << 8;                           // Shifting these 2 bits 8 places to give these bits their original value
  analogInput0 = analogInput0 | Wire.read();                  // Store the last 8 bits to their original value. (OR comparision 1 - 0 == 1)

  unsigned int analogInput1 = Wire.read() & 0x03;             // Reading the 2 Most Significant Bits from analog 1 input on the WIB
  analogInput1 = analogInput1 << 8;                           // Shifting these 2 bits 8 places to give these bits their original value
  analogInput1 = analogInput1 | Wire.read();                  //Store the last 8 bits to their original value. (OR comparision 1 - 0 == 1
  analog_values[0] = analogInput0;
  analog_values[1] = analogInput1;
}

int readSwitch(int switch_bit) {
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

int splitInto(const char* input, char key[64], char value[64]) {

  //Serial.print("input is:"); Serial.println(input);
  const char *tracker = input;     //pointer to go through received
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

void Status(char value[]) {
  if (!strcmp(value, "0")) {
    state = false;
  } else if (!strcmp(value, "1")) {
    state = true;
  }
}

void closeDoor()
{
  myservo.write(82);              // tell servo to go to closed position (82 degree)
  Serial.println("closed the door");
}

void openDoor()
{
  myservo.write(163);             // tell servo to go to open position (163 degree)
  Serial.println("opened the door");
}
