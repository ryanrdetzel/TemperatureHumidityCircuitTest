#include <Arduino.h>
#include <Wire.h>

#include <BMP180MI.h>
#include <BMx280MI.h>
#include "DHTesp.h"
#include "Adafruit_SHT31.h"

#define BMP180_I2C_ADDRESS 0x77
//#define BMP_280_I2C_ADDRESS 0x76

BMP180I2C bmp180(BMP180_I2C_ADDRESS);
//BMx280I2C bmx280(BMP_280_I2C_ADDRESS);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

DHTesp dht;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();

  /* BMP 180 */
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");
    while (1);
  }
  bmp180.resetToDefaults();
  //enable ultra high resolution mode for pressure measurements
  bmp180.setSamplingMode(BMP180MI::MODE_UHR);

  /* BMP280 */
//  if (!bmx280.begin())
//  {
//    Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
//    while (1);
//  }
//  if (bmx280.isBME280())
//    Serial.println("sensor is a BME280");
//  else
//    Serial.println("sensor is a BMP280");
//
//  //reset sensor to default parameters.
//  bmx280.resetToDefaults();
//  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
//  if (bmx280.isBME280())
//    bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

    /* SHT31
     */
   if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  dht.setup(13, DHTesp::DHT22);
}

void bmp180Temp(){
    if (!bmp180.measureTemperature())
  {
    Serial.println("could not start temperature measurement, is a measurement already running?");
    return;
  }

  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  Serial.print("Temperature 180: "); 
  Serial.println(bmp180.getTemperature()); 
}

//void bmp280(){
//  if (!bmx280.measure())
//  {
//    Serial.println("could not start measurement, is a measurement already running?");
//    return;
//  }
//
//  //wait for the measurement to finish
//  do
//  {
//    delay(100);
//  } while (!bmx280.hasValue());
//
//  Serial.print("Temperature 280: "); Serial.println(bmx280.getTemperature());
//  if (bmx280.isBME280())
//    Serial.print("Humidity 280: "); Serial.println(bmx280.getHumidity());
//}

void sht31sensor(){
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temperature SHT31: "); Serial.println(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    //Serial.print("Humidity SHT31: "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
}

void dhtsensor(){
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("DHT22 Temp: "); Serial.println(temperature);
  //Serial.print("DHT22 humidity: "); Serial.println(humidity);
}

int timeSinceLastRead = 0;

void loop() {
  if(timeSinceLastRead > 2000) {
    bmp180Temp();
    //bmp280();
    sht31sensor();
    dhtsensor();
    Serial.println("");
    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}
