#include <Arduino.h>

// #include <BMP180.h>
#include<Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  bmp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = bmp.readTemperature();
  float press = bmp.readPressure();
  float alt = bmp.readAltitude();
  
  Serial.printf("Temp: %.3f*C Pressure: %.2fPa Altitude: %.3fm\n", temp, press, alt);
  delay(3000);
}
