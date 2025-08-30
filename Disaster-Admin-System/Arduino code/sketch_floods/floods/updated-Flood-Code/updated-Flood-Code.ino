#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "time.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>  // BMP180 library (compatible)

// --- WiFi & Firebase Configuration ---
#define WIFI_SSID "SLTFIBER"
#define WIFI_PASSWORD "10BLK1963"
#define API_KEY "AIzaSyALJsTJb_Iz3wVhGk4EcaziYs8BYXCCZeI"
#define DATABASE_URL "https://disasteralertsystem-78a0c-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// --- Pin Assignments ---
#define WATER_SENSOR_PIN 32   // Analog pin for water level
#define RAIN_ANALOG_PIN 33    // Analog pin for rain sensor
#define RAIN_DIGITAL_PIN 25   // Digital pin for rain sensor

// --- Sensor Objects ---
Adafruit_BMP085 bmp;

// --- Globals ---
bool signupOK = false;
unsigned long dataMillis = 0; 
String city = "Gampaha";  // set default city

// --- Time Settings ---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5.5 * 3600;    
const int daylightOffset_sec = 0;

// --- Separate Counters for each sensor ---
int waterCounter = 1;
int bmpCounter = 1;
int rainCounter = 1;

String getEntryID(int counter) {
  char buf[4];
  sprintf(buf, "%03d", counter);  // e.g. 1 -> "001"
  return String(buf);
}

String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "N/A";
  }
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

void setup() {
  Serial.begin(9600);

  // Pin Modes
  pinMode(WATER_SENSOR_PIN, INPUT);
  pinMode(RAIN_ANALOG_PIN, INPUT);
  pinMode(RAIN_DIGITAL_PIN, INPUT);

  // WiFi Connect
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected with IP: " + WiFi.localIP().toString());

  // Time Sync
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Firebase Setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  if (Firebase.signUp(&config, &auth,"","")) {
    Serial.println("Firebase signup OK");
    signupOK = true;
  } else {
    Serial.printf("Signup failed: %s\n", config.signer.signupError.message.c_str());
  }
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // BMP180 Init
  if (!bmp.begin()) {
    Serial.println("Could not find BMP180/BMP085 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  int waterRaw = analogRead(WATER_SENSOR_PIN);
  float waterLevelPercentage = (waterRaw / 4095.0) * 100;

  int rainAnalog = analogRead(RAIN_ANALOG_PIN);
  int rainDigital = digitalRead(RAIN_DIGITAL_PIN);

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0; // hPa
  float altitude = bmp.readAltitude();

  String timeStamp = getFormattedTime();

  // Push Data Every 5 sec
  if (Firebase.ready() && (millis() - dataMillis > 5000 || dataMillis == 0)) {
    dataMillis = millis();

    // --- Water Level ---
    {
      String newKey = getEntryID(waterCounter++);
      Firebase.RTDB.setFloat(&fbdo, "Sensors/waterLevelSensor/levelData/" + newKey + "/WaterLevelPercentage", waterLevelPercentage);
      Firebase.RTDB.setString(&fbdo, "Sensors/waterLevelSensor/levelData/" + newKey + "/time", timeStamp);
      Firebase.RTDB.setString(&fbdo, "Sensors/waterLevelSensor/levelData/" + newKey + "/location", city);
      Serial.println("Water data saved under key " + newKey);
    }

    // --- BMP180 Readings ---
    {
      String newKey = getEntryID(bmpCounter++);
      Firebase.RTDB.setFloat(&fbdo, "Sensors/BMP180Readings/" + newKey + "/temperature_C", temperature);
      Firebase.RTDB.setFloat(&fbdo, "Sensors/BMP180Readings/" + newKey + "/pressure_hPa", pressure);
      Firebase.RTDB.setFloat(&fbdo, "Sensors/BMP180Readings/" + newKey + "/altitude_m", altitude);
      Firebase.RTDB.setString(&fbdo, "Sensors/BMP180Readings/" + newKey + "/timestamp", timeStamp);
      Firebase.RTDB.setString(&fbdo, "Sensors/BMP180Readings/" + newKey + "/location", city);
      Serial.println("BMP180 data saved under key " + newKey);
    }

    // --- Floods Rain Sensor ---
    {
      String newKey = getEntryID(rainCounter++);
      Firebase.RTDB.setInt(&fbdo, "Sensors/FloodsRainReadings/" + newKey + "/analog", rainAnalog);
      Firebase.RTDB.setInt(&fbdo, "Sensors/FloodsRainReadings/" + newKey + "/digital", rainDigital);
      Firebase.RTDB.setString(&fbdo, "Sensors/FloodsRainReadings/" + newKey + "/timestamp", timeStamp);
      Firebase.RTDB.setString(&fbdo, "Sensors/FloodsRainReadings/" + newKey + "/location", city);
      Serial.println("FloodsRainReadings data saved under key " + newKey);
    }
  }

  delay(1000);
}
