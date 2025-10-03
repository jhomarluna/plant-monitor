#include <DHT11.h>
#include <LiquidCrystal.h>

// Consts
const int WET_THRESHOLD = 400;
const int DRY_THRESHOLD = 900;
const int UPDATE_LCD_TIME = 1500;
const int SOIL_SENSOR_TIME = 2000;

// global interval timer 
int interval = 0;

// LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set up pins
int moisture_signal = A0; // Analog input
int red_led = 9;
int yellow_led = 8;

// Create instance of DHT11 class and connect it to pin 2
DHT11 dht11(7);

// Temp and Humidity vars
int temperature = 0;
int humidity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  lcd.begin(16, 2);

}

void loop() {
  // Keep track of how long the program has been going on for 
  int current_time = millis();

  // Get analog reading from Soil Moisture Sensor 
  int moisture = analogRead(moisture_signal);

  // Print moisture level from sensor
  Serial.print("Soil moisture level: ");
  Serial.println(moisture);

  // Turn off each LED for each loop start
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);

  // Turn on LEDS based on soil moisture level
  if (moisture > DRY_THRESHOLD) {                            
    digitalWrite(red_led, HIGH);
  } else if (moisture < WET_THRESHOLD) {   
    digitalWrite(red_led, LOW);
    digitalWrite(yellow_led, LOW); 
  } else {                                      
    digitalWrite(yellow_led, LOW);        // Anywhere in between 400 and 900 is moderate soil moisture
  }

  // Reading the temperature huumidity in serial and LCD
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (current_time - interval > UPDATE_LCD_TIME) {
    interval = current_time;
    if (result == 0) {
    Serial.print("Surrounding Temperature: " );
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Surrounding Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.println("");

    // Print to LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(temperature) + " C");
    lcd.setCursor(0,1);
    lcd.print("Humidity: " + String(humidity) + "%");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
  }
  
  


}
