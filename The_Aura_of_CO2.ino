#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <MQ135.h>

#define gas135 A0 // Define MQ135 Analog Sensor Pin
#define SCREEN_WIDTH 128 // Define width of OLED display
#define SCREEN_HEIGHT 64 // Define height of OLED display
#define DHTPIN 8 // Define DHT22 Digital Sensor Pin
#define OLED_RESET  -1 // If your OLED Display does not use an onboard reset pin, -1 allows you to share the reset pin with the arduino microcontroller
#define SCREEN_ADDRESS 0x3C // The I2C address of the OLED display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

MQ135 gasSensor(gas135, 145); // My calculated Resistance Zero value is 145, you should calibrate the value yourself with your sensor as our atmosphere is likely different.
                              
const int buzzer = 3; // Define pin for buzzer and red LED light as alarm

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT); // Declare buzzer pin mode
  
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Begin display
  display.clearDisplay(); // Clear garbage on display
  display.setTextColor(SSD1306_WHITE);

  dht.begin(); // Begint the DHT22 sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp = dht.readTemperature(); // Read temperature
  int humy = dht.readHumidity(); // Read humidity
  
  int correctedPPM = gasSensor.getCorrectedPPM(temp, humy); // Read CO2 PPM

  display.clearDisplay(); // Clear garbage on display again from end of loop
  
  // Design and print display
  
  display.setTextSize(1);
  display.setCursor(0,7);
  display.print("Temperature:");
  display.setTextSize(2);
  display.setCursor(82,5);
  display.print(temp);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(82, 28);
  display.print(humy);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(0, 51);
  display.print("CO2 Quantity: ");
  display.setTextSize(2);
  display.setCursor(82, 50);
  display.print(correctedPPM);
  
  display.display();

  if (correctedPPM >= 1000) // If CO2 PPM exceeds 1000, trigger Alarm
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  
  delay(5000); // Loop and read sensors 5 every seconds
}
