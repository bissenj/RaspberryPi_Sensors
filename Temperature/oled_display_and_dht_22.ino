#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

// DHT22
#define DHTPIN 22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


// Declaration for an SSD1306 display connected to I2C(SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address for 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }
  
  dht.begin();

  Serial.println("Successfully connected");
}

void loop() {
  Serial.println("Beginning Loop");
  digitalWrite(LED_BUILTIN, HIGH);  
  

  // TEMP ----------------------------------------------------------------------------------------
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(500);
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  // put your main code here, to run repeatedly:  
  delay(250);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Heading
  display.setCursor(0,0);
  display.println("Weather");  

  // Temp  
  int temp = int(f + 0.5);
  display.setCursor(0, 25);
  display.print("Temp: ");  
  display.print(temp);  
  display.println("f");

  // Humidity
  int humidity = int(h + 0.5);
  display.setCursor(0, 50); 
  display.print("Hum: ");   
  display.print(humidity);  
  display.println("%");

  // Write to display
  display.display();

  // Blink the pico led so we know it has power
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}
