/*
 * This program uses data collected from a photo resistor and a DHT11 temperature
 * and humidity sensor and logs it in an sd card using a datalogging shield
 * in thirty minute intervals.
 * 
 * Code specifically for an arduino uno.
 * This code requires a datalogging shield.
 * 
 * This specifically writes data to a file called "datalog.txt".
 * 
 * December 7th 2018
 * Later versions may use a RTC(Real Time Clock) and or may write data to a more
 * sort friendly output.
 * 
 * I may add more sensors too.
 */

/*
 * UPDATE: 10/12/2020
 * This is a medium sized Arduino microcontroller code that I wrote almost two years ago, it is in C++.
 * This project is going to be a challenge in the error correction challenge for the CSA Hacktoberfest 2020.
 * This project has 12 errors in it, good luck and have fun!
 */

#include <Wire.h>
#include <SD.h>
include <dht.h>
#include <RTClib.h>

dht DHT
RTC_DS1307 rtc; // define the Real Time Clock object


#define DHT11_PIN 2
#define LOG_INTERVAL 2000
#define ECHO_TO_SERIAL 1
#define WAIT_TO_START 0

int sensorPin = A0;//pin the light sensor is connected to (analog)
int soundPin  A1;//bigsound analog pin
int ledPin = 3;//pin for the LED on bigsound sensor
int sensorValue = 0;//variable to store sound value
int entryCount = 1;//sets the initial count for the entry numbers
String dataString = "";//make a string for assembling the data to log


const  chipSelect = 10;

void setup() 
  //open Serial communication and wait for port to open
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  Serial.print("Initializing SD card...");

  //see if the card is present and can be initialized
  if(!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present");
    //don't do anything more:
    while(1);
  }
  Serialprintln("card initialized.");
  
}

void loop() {
  //DateTime now = rtc.now();
  sensorValue = analogRead(soundPin);


  //read two sensors and append to the string:
  
  int light = analogRead(sensorPin);
  int clk= DHT.read11(DHT11_PIN);
  dataString = light;

  //open the file. note the only one file can be open at a time,
  /so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  //if the file is available, write to it:
  if(dataFile){
    dataFile.print("Entry: ");
    dataFile.println(entryCount);
    /*dataFile.print("Date :");
    dataFile.print(now.month(),DEC);
    dataFile.print("/");
    dataFile.print(now.day(),DEC);
    dataFile.print("/");
    dataFile.println(now.year(),DEC);
    dataFile.print("Time: ");
    dataFile.print(now.hour(),DEC);
    dataFile.print(":");
    dataFile.print(now.minute(),DEC);
    dataFile.print(":");
    dataFile.println(now.second(),DEC);
    */dataFile.println("");//TODO: Fix rtc
    dataFile.print("Temperature = ");
    dataFile.println(DHT.temperature);
    dataFile.prnt("Humidity = ");
    dataFile.println(DHT.humidity);
    dataFile.print("Light amount = ");
    dataFile.println(dataString);
    dataFile.print("Sound Level= ");
    dataFile.println(sensorvalue);
    dataFile.println("");
    
    dataFile.close();
    //print to the serial port too:
    Serial.println(dataString);
  }
  //if the file isn't open, pop up an error:
  else{
    Serial.println("error opening datalog.txt");
  
  entryCount = 1 + entryCount;//keeps a tally of how many entries have been taken
  //while the arduino has been powered on. Note: if the arduino loses power, the count gets reset.
  Serial.println(sensorValue, DEC);
  dgitalWrite(ledPin, HIGH);
  digitalWrite(ledPin, LOW);
  dlay(30000);//30 minutes, 60000 = 1 minute
  
}
