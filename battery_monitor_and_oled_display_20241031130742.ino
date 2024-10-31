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


// BATTERY MONITOR
#define VOUTPIN 28

// Declaration for an SSD1306 display connected to I2C(SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int display_type = 0;
int counter = 0;


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
  
  // BATTERY MONITOR -----------------------------------------------------------------------------
  float VOLTAGE_DROP_FACTOR = 5.90134529148;
  float vout = analogRead(VOUTPIN);
  float voltage = vout * (3.3 / 1023);

  float finalVoltage = voltage * VOLTAGE_DROP_FACTOR;

  float stateOfCharge = 100;
  if (finalVoltage < 13.6 && finalVoltage > 13.4) {
    stateOfCharge = 95;
  }
  else if (finalVoltage <= 13.4 && finalVoltage > 13.3) {
    stateOfCharge = 90;
  }
  else if (finalVoltage <= 13.3 && finalVoltage > 13.2) {
    stateOfCharge = 80;
  }
  else if (finalVoltage <= 13.2 && finalVoltage > 13.1) {
    stateOfCharge = 70;
  }
  else if (finalVoltage <= 13.1 && finalVoltage > 13.0) {
    stateOfCharge = 60;
  }
  else if (finalVoltage <= 13.0 && finalVoltage > 12.9) {
    stateOfCharge = 30;
  }
  else if (finalVoltage <= 12.9 && finalVoltage > 12.8) {
    stateOfCharge = 20;
  }
  else if (finalVoltage <= 12.8 && finalVoltage > 12.0) {
    stateOfCharge = 10;
  }
  else if (finalVoltage < 12.0) {
    stateOfCharge = 0;
  }



  Serial.print(F("Battery: "));
  Serial.print(stateOfCharge);
  Serial.print("% | ");
  Serial.print(finalVoltage);
  Serial.print(" | ");
  Serial.print(voltage);
  Serial.print(" | ");
  Serial.println(vout);


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

  // PRINT WEATHER
  if (display_type == 0) {
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
  }
  else {
    // PRINT BATTERY

    // Heading
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Battery");  

    // State of charge      
    display.setCursor(0, 25);            
    display.print(int(trunc(stateOfCharge)));  
    display.println("%");
  }

  // Write to display
  display.display();

  // Blink the pico led so we know it has power
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  if (counter > 10) {
    counter = 0;
    if (display_type == 0) {
      display_type = 1;
    }
    else {
      display_type = 0;
    }
  }

  counter++;

  Serial.print(F("Type: "));
  Serial.print(display_type);
  Serial.print(F("Counter: "));
  Serial.print(counter);
}
