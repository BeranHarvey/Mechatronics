///// libraries
#include <Wire.h>
#include <SD.h>
#include <Adafruit_AHTX0.h>
#include "ScioSense_ENS160.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

///// ENS160 + AHT variables
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);
Adafruit_AHTX0 aht;
int tempC;
int tempF;
int humidity;

///// soil moisture - I calibrated the values in air and pure water
const int valAir = 785;
const int valWater = 400;

///// time variables
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 300000; // 5 minutes (Change to 10 secs when testing)
unsigned int timestampmins = 0;

///// file variable
File myFile;

///// OLED display variables
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

///// start setup
void setup() {
  Serial.begin(9600);

  while (!Serial) {}

  ///// Set Up SD Card Start
  #define PIN_SPI_CS 53

  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
    while (1);
  }
  Serial.println(F("SD CARD INITIALIZED."));

  if (SD.exists("data.csv")) {
    Serial.println(F("Deleting old data.csv..."));
    SD.remove("data.csv");
  }

  // create a new file by opening a new file and immediately close it
  Serial.println(F("Creating data.csv file..."));
  myFile = SD.open("data.csv", FILE_WRITE);
  myFile.println("Time,Temperature,Humidity,Soil Moisture,AQI,TVOC,CO2");
  myFile.close();

  // recheck if file is created or not
  if (SD.exists("data.csv"))
    Serial.println(F("data.csv exists on SD Card."));
  else
    Serial.println(F("data.csv doesn't exist on SD Card."));

  // print ENS160 status
  Serial.println("------------------------------------------------------------");
  Serial.println("ENS160 - Digital air quality sensor");
  Serial.println();
  Serial.println("Sensor readout in standard mode");
  Serial.println();
  Serial.println("------------------------------------------------------------");
  Serial.print("ENS160...");
  ens160.begin();
  Serial.println(ens160.available() ? "done." : "failed!");

  // Print ENS160 version
  if (ens160.available()) {
    Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());
    Serial.print("\tStandard mode ");
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }

  // print AHT21 status
  Serial.println("Adafruit AHT21 demo!");
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1);
  }
  Serial.println("AHT21 found");

  // print OLED display status
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while (1);
  }
  display.clearDisplay();
} //end setup

///// Main Loop - cycle every 5 mins and perform measurment
void loop() {
  if (millis() - lastLogTime < LOG_INTERVAL) {
    return;  // not time yet
  }
  lastLogTime = millis();

  // open file
  File myFile = SD.open("data.csv", FILE_WRITE);
  if (!myFile) {
    Serial.println("SD open failed!");
    return;
  }

  // AHT21
  sensors_event_t humidity1, temp;
  aht.getEvent(&humidity1, &temp);
  int tempC = temp.temperature;
  int humidity = humidity1.relative_humidity;

  // soil moisture
  int val = analogRead(A0);
  int soilMoistPercent = map(val, valAir, valWater, 0, 100);
  soilMoistPercent = constrain(soilMoistPercent, 0, 100);

  // ENS160
  int aqi = -1, tvoc = -1, eco2 = -1;
  if (ens160.available()) {
    ens160.set_envdata(tempC, humidity);
    ens160.measure(true);
    ens160.measureRaw(true);
    aqi  = ens160.getAQI();
    tvoc = ens160.getTVOC();
    eco2 = ens160.geteCO2();
  }

  // time
  unsigned long t = millis() / 1000;
  int hours   = t / 3600;
  int minutes = (t % 3600) / 60;
  int seconds = t % 60;

  // write to CSV
  Serial.println("Writing data to csv file...");

  myFile.print(hours); myFile.print(':');
  myFile.print(minutes); myFile.print(':');
  myFile.print(seconds); myFile.print(',');

  myFile.print(tempC); myFile.print(',');
  myFile.print(humidity); myFile.print(',');
  myFile.print(soilMoistPercent); myFile.print(',');
  myFile.print(aqi); myFile.print(',');
  myFile.print(tvoc); myFile.print(',');
  myFile.println(eco2);

  // OLED display
  Serial.println("Sending data to OLED Display...");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Time: ");
  display.print(hours); display.print(':');
  display.print(minutes); display.print(':');
  display.println(seconds); 
  display.print("Temperature: "); display.print(tempC); display.println(" deg.");
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.print("Soil Moisture: "); display.print(soilMoistPercent); display.println(" %");
  display.print("AQI: "); display.println(aqi);
  display.print("TVOC: "); display.print(tvoc); display.println(" ppm");
  display.print("CO2: "); display.print(eco2); display.println( "ppm");
  display.display();

  myFile.flush();
  myFile.close();

  Serial.print("Logged data at ");
  Serial.print(timestampmins);
  Serial.println(" mins");

  timestampmins += 5;

  // Optional: stop after 24 hours (288 samples)
  if (timestampmins >= 1440) {
    Serial.println("24-hour logging complete.");
    while (true); // halt
  }
}


