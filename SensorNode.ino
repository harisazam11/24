#include <FirebaseESP32.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define FIREBASE_HOST "patient-health-monitorin-101d7-default-rtdb.firebaseio.com"

#define WIFI_SSID "ESP8266WIFI" // Change the name of your WiFi
#define WIFI_PASSWORD "smart32198" // Change the password of your WIFI your WIFI

#define FIREBASE_Authorization_key
"eJxXRNeUbDAEX8xsPYJ0cyxT56buKgD2G6wCUYTF"

FirebaseData firebaseData;
FirebaseJson json;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100]; //red LED sensor data
#else

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100]; //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read

void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per
second:

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");

    delay(300);
  }

  Serial.println();

  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());

  Serial.println();

  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411

  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate,
pulseWidth, adcRange); //Configure sensor with these settings

  particleSensor.enableDIETEMPRDY();
}

void loop()
{
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next
    
    //sample
    Serial.print(F("red="));
    Serial.print(redBuffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.print(irBuffer[i], DEC);

    Serial.print(F(", HR="));
    Serial.print(heartRate, DEC);

    Serial.print(F(", SPO2="));
    Serial.print(spo2, DEC);

    Serial.print(redBuffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(irBuffer[i], DEC);
  }

  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer,
&spo2, &validSPO2, &heartRate, &validHeartRate);

  while (1)
  {

    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every
data read

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to
next sample

      //send samples
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);
    }

    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer,
&spo2, &validSPO2, &heartRate, &validHeartRate);

    Firebase.setFloat(firebaseData, "/ESP32_APP/spo2", spo2);
    Firebase.setFloat(firebaseData, "/ESP32_APP/HEART RATE", heartRate);

    float temperature = particleSensor.readTemperature();
    Serial.print("temperatureC=");
    Serial.print(temperature, 2);
    Firebase.setFloat(firebaseData, "/ESP32_APP/TEMPERATURE C",
temperature);
  }

  float temperatureF = particleSensor.readTemperatureF(); //Because I am a
bad global citizen

  Serial.print("temperatureF=");
  Serial.print(temperatureF, 2);
}