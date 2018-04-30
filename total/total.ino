#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#define MAX_LED 20
#define MIN_LED 100
#define MAX_DAT 600
#define MIN_DAT 400
#include <DHT12.h>
#include "Arduino.h"
DHT12 dht12;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
int timeSinceLastRead = 0;


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

void setup() {
   Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println("");
    pinMode (5, OUTPUT);
  
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
  Serial.println("");
  dht12.begin();
  pinMode (3, OUTPUT);
  pinMode (2, INPUT);
  pinMode(A0, INPUT);
  pinMode (13, OUTPUT);
  pinMode (2, OUTPUT);
}

void loop() {
 Serial.print("test\n");
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    Serial.print(event.light); Serial.println(" lux");
    if(event.light<MAX_LED)
      digitalWrite (5, HIGH);
    else if(event.light>MIN_LED) 
    digitalWrite (5, LOW);
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  // Report every 2 seconds.
if(timeSinceLastRead > 2000) {
// Reading temperature or humidity takes about 250 milliseconds!
// Read temperature as Celsius (the default)
float t12 = dht12.readTemperature();
if(t12>30){
  digitalWrite (3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
// Read temperature as Fahrenheit (isFahrenheit = true)
float f12 = dht12.readTemperature(true);
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h12 = dht12.readHumidity();
 
bool dht12Read = true;
// Check if any reads failed and exit early (to try again).
if (isnan(h12) || isnan(t12) || isnan(f12)) {
Serial.println("Failed to read from DHT12 sensor!");
 
dht12Read = false;
}
 
if (dht12Read){
// Compute heat index in Fahrenheit (the default)
float hif12 = dht12.computeHeatIndex(f12, h12);
// Compute heat index in Celsius (isFahreheit = false)
float hic12 = dht12.computeHeatIndex(t12, h12, false);
// Compute dew point in Fahrenheit (the default)
float dpf12 = dht12.dewPoint(f12, h12);
// Compute dew point in Celsius (isFahreheit = false)
float dpc12 = dht12.dewPoint(t12, h12, false);
 
Serial.print("DHT12=> Humidity: ");
Serial.print(h12);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t12);
Serial.print(" *C ");
Serial.print(f12);
Serial.print(" *F\t");
Serial.print(" Heat index: ");
Serial.print(hic12);
Serial.print(" *C ");
Serial.print(hif12);
Serial.print(" *F");
Serial.print(" Dew point: ");
Serial.print(dpc12);
Serial.print(" *C ");
Serial.print(dpf12);
Serial.println(" *F");
}
timeSinceLastRead = 0;
}
timeSinceLastRead += 250;
 int value = analogRead(A0);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
                                      // Giá trị được số hóa thành 1 số nguyên có giá trị
                                      // trong khoảng từ 0 đến 1023
  Serial.println(value);//Xuất ra serial Monitor                   
  delay(10);
  if(value>MAX_DAT){
    digitalWrite (2, HIGH);
    }
  if(value<MIN_DAT){
    digitalWrite (2, LOW);
    }
  // Đọc giá trị D0 rồi điều khiển Led 13...Các bạn cũng có thể điều khiển bơm nước thông qua rơle....
  if (digitalRead (2) == 0)
  {
   digitalWrite (13, HIGH);
  }
  else{
  digitalWrite (13, LOW);
       
}
  delay(250);
}
