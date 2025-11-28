#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include<Adafruit_BMP085.h>

const char* ssid = "Robotics Institute of Kenya";
const char* password = "RObOT1C&#357";

// Your Flask server address (change this to match YOUR PC )
String serverURL = "http://192.168.1.56:5000/api/bmp180";

Adafruit_BMP085 bmp;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
//bmp connect
  bmp.begin();

//display begin
   display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
   display.clearDisplay();
   display.display();
   delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = bmp.readTemperature();
  float press = bmp.readPressure();
  float alt = bmp.readAltitude();

  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);     
  display.printf("Temp: %.2f C\n", temp); // same as: display.println("Temp: " + String(temp));
  display.setCursor(0, 20);     
  display.printf("Press: %.2f Pa\n",press);
  display.setCursor(0, 40);     
  display.printf("Alt: %.2f m\n" , alt);
  display.display();

  
  Serial.printf("Temp: %.3f*C Pressure: %.2fPa Altitude: %.3fm\n", temp, press, alt);
  //delay(3000);

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(serverURL);

    http.addHeader("Content-Type", "application/json");

    // JSON you send to Flask
    String jsonData = "{\"temperature\":" + String(temp) +
                      ",\"pressure\":" + String(press) +",\"altitude\":"+String(alt) + "}";

    int httpResponseCode = http.POST(jsonData);

    Serial.print("Server Response: ");
    Serial.println(httpResponseCode);

    if(httpResponseCode > 0){
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.println("Error sending POST request");
    }

    http.end();
  }
  delay(5000);
}
