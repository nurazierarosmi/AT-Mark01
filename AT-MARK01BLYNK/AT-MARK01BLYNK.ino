// Name : Nuraziera Rosmi Group 8
// Name project : AT-Mark 01
// This program code is for artifact monitoring system
// It will detect the room temperature using DHT11 and state whether it is fit or unfit for the artifact in the museum.
// Then, it will detect the human presence  using HC-SR04 infront of the artifact. If, there is human , it will detect
// the presence of artifact. If not, it will state human not present. When the artifact is moved, it will light up the LED
// and the buzzer. If not moved, the LCD will display item is inplace.

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <dht11.h>
#define echoPin D1  // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin D2  //attach pin D3 Arduino to pin Trig of HC-SR04

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8c23LhxxuIRwDr_PKaqs4K_k-WUz3XdR";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "safros@unifi"; //WIFI
char pass[] = "pass"; //PASSWORD

#define DHT11PIN 0         // What digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
long duration;     // variable for the duration of sound wave travel
int distance;      // variable for the distance measurement
int fsrPin = A0;    // the FSR and 10K pulldown are connected to a0
int fsrReading;    // the analog reading from the FSR resistor divider
//int LEDRpin = 8;   //LED at pin D8
int Buzzerpin = D7; //Buzzer at pin D9
dht11 DHT11;       //variable DHT11 form dht11 library 
SimpleTimer timer;

void sendSensor(){
  int chk = DHT11.read(DHT11PIN); //assigning variable to read dht11pin
  float h = DHT11.humidity;
  float t = DHT11.temperature; // or dht.readTemperature(true) for Fahrenheit

  //monitoring temperature and humidity in serial monitor
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println(t);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  // SETUP the ALARM Trigger and Send EMAIL 
  // and PUSH Notification

  if(t > 32){
   
    Blynk.notify("ESP8266 Alert - Temperature over 28C!");
  }
  else{
  delay(1000);                   // wait for 1s
  //digitalWrite(LEDRpin,LOW);     // light off the first red LED
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
    //  digitalWrite(LEDRpin,LOW);        // light off the first red LED
  //  digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    
 /*   lcd.clear();                      // clearing LCD
    lcd.setCursor(0, 0);              // Set the cursor on the first column and first row.
    lcd.print("iTEM INPLACE");        // display some text on LCD*/
    // display on serial monitor
    Serial.println(); 
    Serial.println("The item still in place");
   
     }
     
   else{
   Serial.println(" - No pressure");
 //   digitalWrite(LEDRpin,HIGH);      // light up the first red LED
    digitalWrite(Buzzerpin,HIGH);    // switch on the buzzer to alarm
 //   digitalWrite(LEDR2pin,HIGH);     // light up the second red LED
    Serial.println("The item is not in place");  //display some text on serial monitor
    Blynk.notify("ALERT!!!-ITEM GONE!!!");
 /*   lcd.clear();                     // clearing LCD
    lcd.setCursor(0, 0);             // Set the cursor on the first column and first row.
    lcd.print("iTEM NOT INPLACE");   //display some text on LCD*/
    delay(1000);                     //wait for 1s
       }
 }
 else{
 //   digitalWrite(LEDRpin,LOW);        // light off the first red LED
 //   digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    // display on serial monitor
    Serial.println();
    Serial.println("No person in front of the item");
 /*   lcd.clear();                      // clearing LCD
    lcd.setCursor(0, 0);              // Set the cursor on the first column and first row.
    lcd.print("NO PERSON");           // print some text on LCD*/
 }
  //  digitalWrite(LEDRpin,LOW);        // light off the first red LED
  //  digitalWrite(LEDR2pin,LOW);       // light off the second red LED
    digitalWrite(Buzzerpin,LOW);      // switch off the buzzer to alarm
    delay(2000);                      // wait for 2s
}
}

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(Buzzerpin,OUTPUT); // Sets the BuzzerPin as an OUTPUT
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);   // Sets the echoPin as an INPUT
//  dht.begin();
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}
