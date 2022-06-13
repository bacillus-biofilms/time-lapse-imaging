#include <DHT.h> 
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <Servo.h>

#define DHTPIN 8                        
#define DHTTYPE DHT22                   
DHT dht(DHTPIN, DHTTYPE);               

const int chipSelect = 10;              
RTC_DS1307 RTC;

Servo rotor;
Servo left;
Servo right;

int rightStart = 57;
int rotorStart = 87;
int leftStart = 138;

int posRotor = rotorStart;
int posRight = rightStart;
int posLeft = leftStart;

int rightStop = 10;
int rotorStop = 135;

int focus = 7;
int shot = 4;
int light = 2;

int counter = 1;

const int waterSens = A0;
const int buzz = 3;
int SensorValue;
int relay = A1;
int servoDelay = 20;


void setup() {
	
  pinMode(light, OUTPUT);
  pinMode(focus, OUTPUT);
  pinMode(shot, OUTPUT);

  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (!RTC.isrunning()) {
	  Serial.println("RTC is NOT running!");
  }
  pinMode(buzz, OUTPUT);
  pinMode(waterSens, INPUT);
  pinMode(relay, OUTPUT);
}
  
void loop() { 

  // turn light on
  digitalWrite(light, LOW);
  delay(1000);
    
  right.attach(6);
  left.attach(5);
  rotor.attach(9);

  posRotor = rotorStart;
  posRight = rightStart;
  posLeft = leftStart;
  
  left.write(posLeft);
  delay(50);
  right.write(posRight);
  delay(50);
  rotor.write(posRotor);
  delay(50);
  
  // raise hand
  for (int i = 0; i < 10; i++) {
    posRight -= 1;
    right.write(posRight);
    delay(servoDelay);
  }
  
  // raise hand
  for (; posRight >= rightStop; posRight -= 1) {           
    right.write(posRight);
    if (posRight % 2 == 0) { 
      posLeft -= 1;
      left.write(posLeft);
    }             
    delay(servoDelay);
  }

  delay(500);
  right.detach();
  left.detach();
  
  // rotate open hand
  for (; posRotor <= rotorStop; posRotor += 1) {           
        rotor.write(posRotor);
        delay(servoDelay);
  }
  delay(1000);
  
  pickUpTemperature();
  counter++;
  
  // take image
  digitalWrite(focus, HIGH);
  delay(1000);
  digitalWrite(shot, HIGH);
  delay(1000);
  
  digitalWrite(focus, LOW);
  digitalWrite(shot, LOW);
  
  delay(1000);
  
  // rotate close handa
  for (; posRotor >= rotorStart; posRotor -= 1) {
    rotor.write(posRotor);
    delay(servoDelay);
  }
  
  rotor.detach();
  right.attach(6);
  left.attach(5);
  delay(1000);
  
  // lower hand
  for (; posRight <= (rightStart - 10); posRight += 1) {           
    right.write(posRight);
    if (posRight % 2 == 0) { 
      posLeft += 1;
      left.write(posLeft);
    }                         
    delay(servoDelay);
  }
  
  // lower hand
  for (int i = 0; i < 10; i++) {
    posRight += 1;
    right.write(posRight);
    delay(servoDelay);
  }
  
  delay(1000);

  right.detach();
  left.detach();
  delay(1000);
  // turn of light
  digitalWrite(light, HIGH);

 
  digitalWrite(relay, LOW);
  delay(1000);
  
  int sensorValue = analogRead(waterSens);
  int i = 0;
  int alarmOn = 0;
  
  digitalWrite(relay, HIGH);
  delay(100);
  
  // activate alaram if water level is low
  if (sensorValue <= 50) {
	alarmOn = 1;
    for (i = 0; i < 60; i++) {
        tone(buzz, 800, 800);
        delay(500);
        tone(buzz, 600, 600);
        delay(500);
    }
  }
  
  // correct wait time interval by program execution time
  unsigned long bigDelay = 345942;
  if (alarmOn) {
    bigDelay -= 60000; 
  }
  
  delay(bigDelay);
}
