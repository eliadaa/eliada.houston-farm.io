#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>
#include <ThingSpeak.h>

// sensor codes and fields for reading data from ThingSpeak
#define TEMPERATURE_CODE       501
#define HUMIDITY_CODE          502
#define LUMINOSITY_CODE        503
#define MOISTURE_CODE          504
#define WATERLEVEL_CODE        505

#define temp_field             1
#define humid_field            2
#define lumi_field             3
#define moisture_field         4
#define water_level_field      5

// ThingSpeak settings for reading the sensor data 
const unsigned long channelID = 2683325;  // ThingSpeak Channel ID
const char* writeAPIKey = "OCOTAC5XQMBWIKX1";  // Write API Key

// network alternatives 
const char* primary_ssid = "DCTI-PUBLIC";
const char* primary_password = "dcti1966";

// channel ID, read API key for the ThingSpeak channel that handles
// the min moisture value received from the Android application
unsigned long ChannelNumber = 2683329;            // Channel ID
const char * myReadAPIKey = "Q9CFNUTUMELGK68M";   // Read API Key
const int FieldNumber1 = 1;  // moisture
const int FieldNumber3 = 3;  // temperature

unsigned long connectionTimeout = 15000; // Timeout duration in milliseconds
unsigned long connectionStartTime;

// WiFiClient is a class provided by the ESP8266 WiFi library
WiFiClient client;

String incomingData = ""; // Store incoming data from Arduino

// Variables to store the latest sensor values
float temp = NAN;
float humid = NAN;
float lumi = NAN;
float moisture = NAN;
float waterLevel = NAN;

void setup() {
  Serial.begin(9600);  // start serial communication at 9600 baud
  WiFi.disconnect(true); // disconnect from any network
  delay(1000);

  // Connecting to the network
  tryConnecting();       

  // Establish connection with the ThingSpeak server to send and receive data 
  ThingSpeak.begin(client);
}

void tryConnecting() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Already connected to the primary network!");
  } else { 
    Serial.print("Connecting to the primary network...");
    connectToNetwork(primary_ssid, primary_password);
  }
}

void connectToNetwork(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  connectionStartTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - connectionStartTime >= connectionTimeout) {
      break; 
    }
    delay(1000);
  }
}

void loop() {
  // Non-blocking serial reading
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    incomingData += receivedChar;

    // When a newline character is received, process the data
    if (receivedChar == '\n') {
      processIncomingData(incomingData);
      incomingData = "";  // Clear the buffer after processing
    }
  }

  // Read min moisture value from ThingSpeak
  long meanTempValue = ThingSpeak.readLongField(ChannelNumber, FieldNumber3, myReadAPIKey);
  int statusCode = ThingSpeak.getLastReadStatus();
  
  if (statusCode == 200) {
    // Send the min moisture value to the Arduino
    Serial.print("<TEMP,");
    Serial.print(meanTempValue);
    Serial.println(">");  // Adding a newline to ensure proper transmission
  } else {
    Serial.println("Unable to read channel / No internet connection");
  }

  // After collecting all data, send to ThingSpeak in one call
  sendAllDataToThingSpeak();

  delay(25000);  // Wait for 25 seconds before reading again
}

// Process incoming data from the Arduino
void processIncomingData(String data) {
  int sensorCode = data.substring(0, data.indexOf(',')).toInt();
  float value = data.substring(data.indexOf(',') + 1).toFloat();

  // Update the corresponding variable based on sensor code
  if (sensorCode == MOISTURE_CODE) {
    moisture = value;
  } else if (sensorCode == WATERLEVEL_CODE) {
    waterLevel = value;
  } else if (sensorCode == LUMINOSITY_CODE) {
    lumi = value;
  } else if (sensorCode == HUMIDITY_CODE) {
    humid = value;
  } else if (sensorCode == TEMPERATURE_CODE) {
    temp = value;
  }
}

// Send all the accumulated sensor data to ThingSpeak in one write
void sendAllDataToThingSpeak() {
  if (!isnan(temp)) {
    ThingSpeak.setField(temp_field, temp);
  }
  if (!isnan(humid)) {
    ThingSpeak.setField(humid_field, humid);
  }
  if (!isnan(lumi)) {
    ThingSpeak.setField(lumi_field, lumi);
  }
  if (!isnan(moisture)) {
    ThingSpeak.setField(moisture_field, moisture);
  }
  if (!isnan(waterLevel)) {
    ThingSpeak.setField(water_level_field, waterLevel);
  }

  // Write the accumulated data to ThingSpeak once all fields are set
  ThingSpeak.writeFields(channelID, writeAPIKey);
}
