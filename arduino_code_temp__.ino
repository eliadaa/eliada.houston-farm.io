#include <DHT.h>
#include <Servo.h>

#define LIGHTSENSOR_PIN    A2   // SEN0043 TEMT600 ANALOG AMBIENT LIGHT SENSOR
#define dhtPin 2
#define dhtType DHT11
#define RELAY_PIN 8             // 5V relay connected to submersible water pump
#define MOISTURESENSOR_PIN A0   // SEN0193 ANALOG CAPACITIVE SOIL MOISTURE SENSOR
#define WATERLEVEL_PIN     A4
#define WATERLEVEL_POWER_PIN 7  // The water level sensor shouldn't be powered constantly
#define SERVO_PIN 9 
#define LED_PIN 12


#define tempCODE         501
#define humidCODE        502
#define luminCODE        503
#define moistureCODE     504
#define waterlevelCODE   505

#define MIN_MOISTURE 35

DHT dht(dhtPin, dhtType);

float temp, humid, lumin, moist, water;
float moisture_level, moisture_percentage, water_level, water_percentage, lumin_level, lumin_percentage;

const int open_air_reading = 520;  // Calibration data 1: readings when sensor is exposed to air
const int water_reading = 260;     // Calibration data 2: readings when sensor is exposed to water

const int no_water = 0;            // Calibration data 3: readings when the water level sensor is dry
const int full_water = 540;        // Calibration data 4: readings when the water level sensor is fully immersed

int min_moisture = 0;
int mean_temperature = 0;
Servo myServo;

unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds interval

void setup() {
  Serial.begin(9600);
  pinMode(LIGHTSENSOR_PIN, INPUT);
  pinMode(MOISTURESENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(WATERLEVEL_POWER_PIN, OUTPUT);
  // pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  
  myServo.attach(SERVO_PIN);  // Attach the servo to the defined pin
  
  digitalWrite(RELAY_PIN, HIGH); // stop the pump initially
  digitalWrite(WATERLEVEL_POWER_PIN, LOW); // stop the power to the water level sensor
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check for incoming serial data from ESP8266
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('>');  // Read until the closing '>'
    if (data.startsWith("<TEMP,")) {
      int commaIndex = data.indexOf(',');  
      String meanTempStr = data.substring(commaIndex + 1, data.length()); // Extract the value after the comma
      mean_temperature = meanTempStr.toInt();  // Convert to integer
      Serial.println("Received mean Temp: " + String(mean_temperature));
    }
  }

  // Non-blocking delay for sensor readings
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read and send sensor data
    temp = dht.readTemperature();
    humid = dht.readHumidity();

    // Send temperature and humidity
    if (!isnan(temp) && !isnan(humid)) {  
      Serial.print(tempCODE);
      Serial.print(",");
      Serial.println(temp);

      Serial.print(humidCODE);
      Serial.print(",");
      Serial.println(humid);
    }

    // Luminosity
    lumin_level = analogRead(LIGHTSENSOR_PIN);
    lumin_percentage = map(lumin_level, 0, 1023, 0, 100);
    
    if (lumin_percentage >= 0 && lumin_percentage <= 100) {
      Serial.print(luminCODE);
      Serial.print(",");
      Serial.println(lumin_percentage);
    }

    // Soil moisture
    moisture_level = analogRead(MOISTURESENSOR_PIN);    
    moisture_percentage = map(moisture_level, open_air_reading, water_reading, 0, 100);
    
    if (moisture_percentage >= 0 && moisture_percentage <= 100) {
      Serial.print(moistureCODE);
      Serial.print(",");
      Serial.println(moisture_percentage);
    }

    // Water level
    digitalWrite(WATERLEVEL_POWER_PIN, HIGH);  
    delay(20);                               
    water_level = analogRead(WATERLEVEL_PIN);  
    digitalWrite(WATERLEVEL_POWER_PIN, LOW);   
    water_percentage = map(water_level, no_water, full_water, 0, 100);
    
    
    if (water_percentage >= 0 && water_percentage <= 100) {
      Serial.print(waterlevelCODE);
      Serial.print(",");
      Serial.println(water_percentage);
    }
    
    // Water pump and servo motor control based on moisture and temperature
    if (moisture_percentage <= MIN_MOISTURE){
      if(mean_temperature >= 11){
        Serial.println("Turning ON the pump longer--dry");
        digitalWrite(RELAY_PIN, LOW);  // Turn pump ON
        delay(6000);                   // Pump ON for 6 seconds -- longer since it's dry
        digitalWrite(RELAY_PIN, HIGH);   // Turn pump OFF
        Serial.println("Turning OFF the pump");
        myServo.write(0);               // Close the roof
      } else {
        Serial.println("Turning ON the pump");
        digitalWrite(RELAY_PIN, LOW);  // Turn pump ON
        delay(2000);                   // Pump ON for 2 seconds
        digitalWrite(RELAY_PIN, HIGH);   // Turn pump OFF
        myServo.write(180);             // Open the roof
        Serial.println("Turning OFF the pump");   
        digitalWrite(LED_PIN, HIGH);  // turn on heating inside     
        delay(9000);
      }
    } 
  }
}
