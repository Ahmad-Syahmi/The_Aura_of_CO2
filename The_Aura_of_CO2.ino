#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <MQ135.h>

#define gas135 A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTPIN 8
#define OLED_RESET  -1
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

MQ135 gasSensor(gas135, 145); //RZero is 145

const int buzzer = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp = dht.readTemperature();
  int humy = dht.readHumidity();
  
  int correctedPPM = gasSensor.getCorrectedPPM(temp, humy);

  display.clearDisplay();
  
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

  if (correctedPPM >= 1000)
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  
  delay(1000);
}
