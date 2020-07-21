// Name : Nuraziera Rosmi Group 8
// Name project : AT-Mark 01
// This program code is for artifact monitoring system
// It will detect the room temperature using DHT11 and state whether it is fit or unfit for the artifact in the museum.
// Then, it will detect the human presence  using HC-SR04 infront of the artifact. If, there is human , it will detect
// the presence of artifact. If not, it will state human not present. When the artifact is moved, it will light up the LED
// and the buzzer. If not moved, the LCD will display item is inplace.

#include <Wire.h>  //including library for i2c 
#include <LiquidCrystal_I2C.h>  //including library for LCD
#include <dht11.h> // including dht11 library
#define DHT11PIN 4 //pin D4 Arduino to pin out of DHT11
#define echoPin 2  // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3  //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
dht11 DHT11;       //variable DHT11 form dht11 library 
long duration;     // variable for the duration of sound wave travel
int distance;      // variable for the distance measurement
int fsrPin = 0;    // the FSR and 10K pulldown are connected to a0
int fsrReading;    // the analog reading from the FSR resistor divider
int LEDRpin = 8;   //LED at pin D8
int Buzzerpin = 9; //Buzzer at pin D9
int LEDR2pin = 10; //LED at pin D8
LiquidCrystal_I2C lcd (0x27, 16, 2); // configuring for 16x2 LCD.

void setup(void) {
  // Sending debugging information via the Serial monitor
  Serial.begin(9600);        // Serial Communication is starting with 9600 of baudrate speed
  pinMode(LEDRpin,OUTPUT);   // Sets the LEDRPin as an OUTPUT
  pinMode(LEDR2pin,OUTPUT);  // Sets the LEDR2Pin as an OUTPUT
  pinMode(Buzzerpin,OUTPUT); // Sets the BuzzerPin as an OUTPUT
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);   // Sets the echoPin as an INPUT
  Serial.println();
  Serial.println("Welcome to AT-Mark 1"); // print text " welcome" in Serial Monitor
  Serial.println("Let's start");          // print text in Serial Monitor
  lcd.init();
  lcd.backlight();          // switching on the backlight
  lcd.setCursor(3, 0);      // Set the cursor on the fourth column and first row.
  lcd.print("Welcome to "); // Print welcome
  lcd.setCursor(2, 1);      // Set the cursor on the third column and second row.
  lcd.print(" AT-Mark 01"); // Print AT-Mark 01
  delay(3000);              // wait for 3s
}
 
void loop(void) {
  lcd.clear();              //clearing the lcd
  Serial.println();         //print empty in serial monitor
  int chk = DHT11.read(DHT11PIN); //assigning variable to read dht11pin

  //monitoring temperature and humidity in serial monitor
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);

  //print temperature and humidity on LCD
  lcd.print("Humidity:");
  lcd.print(DHT11.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(DHT11.temperature);
  delay(5000);   //to let the temperature and humidity display 5s on LCD 
  lcd.clear();   //clearing lcd
  
  //checking room temperature if it is okay for the artifact
 if(DHT11.temperature>25)             // setting conditions for the temperature more than 25 degree celcius
  {    digitalWrite(LEDRpin,HIGH);    // light up the first red LED
       lcd.print("Temp unfit");       // display the following text on LCD 
       delay(1000);                   // wait for 1s
       lcd.clear();                   // clearing lcd
       lcd.setCursor(0, 0);           // Set the cursor on the first column and first row.
       lcd.print("Move the item or"); // display the following text
       lcd.setCursor(0, 1);           // Set the cursor on the first column and first row.
       lcd.print("cool down room");   // display the following text
       delay(4000);                   // wait for 4s
       lcd.clear();                   // clearing LCD
       //monitoring temperature in serial monitor
       Serial.print("Temperature (C): ");     
       Serial.println((float)DHT11.temperature, 2); 
       delay(4000);                   // wait for 4s
  }
  //if false
 else{
  lcd.print("Temp suitable");    // display the following text on LCD 
  delay(1000);                   // wait for 1s
  digitalWrite(LEDRpin,LOW);     // light off the first red LED
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(2000);                       // wait for 2s
  fsrReading = analogRead(fsrPin);   // assigning variables to read fsrPin
  Serial.print("Analog reading = "); // print on serial monitor 
  Serial.print(fsrReading);          // the raw analog reading
  
  // setting condition when a person is infront of the artifact
 if(distance < 15)
 {
  // We'll have a few threshholds, qualitatively determined
  if (fsrReading < 200) {
    Serial.println(" - No pressure");
    digitalWrite(LEDRpin,HIGH);      // light up the first red LED
    digitalWrite(Buzzerpin,HIGH);    // switch on the buzzer to alarm
    digitalWrite(LEDR2pin,HIGH);     // light up the second red LED
    Serial.println("The item is not in place");  //display some text on serial monitor
    lcd.clear();                     // clearing LCD
    lcd.setCursor(0, 0);             // Set the cursor on the first column and first row.
    lcd.print("iTEM NOT INPLACE");   //display some text on LCD
    delay(1000);                     //wait for 1s
     }
     
   else{
    digitalWrite(LEDRpin,LOW);        // light off the first red LED
    digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    
    lcd.clear();                      // clearing LCD
    lcd.setCursor(0, 0);              // Set the cursor on the first column and first row.
    lcd.print("iTEM INPLACE");        // display some text on LCD
    // display on serial monitor
    Serial.println(); 
    Serial.println("The item still in place");
       }
 }
 else{
    digitalWrite(LEDRpin,LOW);        // light off the first red LED
    digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    // display on serial monitor
    Serial.println();
    Serial.println("No person in front of the item");
    lcd.clear();                      // clearing LCD
    lcd.setCursor(0, 0);              // Set the cursor on the first column and first row.
    lcd.print("NO PERSON");           // print some text on LCD
 }
    digitalWrite(LEDRpin,LOW);        // light off the first red LED
    digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    delay(2000);                      // wait for 2s
} }
