//ACS
#include <Wire.h>
//#include <JY901.h>
#include "JY901_Serial.h"
//ACS

#include <SD.h> //sd

//disp
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//disp

// pin for sd
int CS_pin = 53;
float refresh_rate = 5000.0;
// pin for sd

void setup()
{
  //for sd
  Serial.begin(9600);
  Serial1.begin(9600);
  JY901.attach(Serial1);
  //Serial.println("Initializing Card");
  pinMode(CS_pin, OUTPUT);
  
  if (!SD.begin(CS_pin)) 
  {
      Serial.println("Card Failure");
      return;
  }
//  Serial.println("Card Ready");
  //for sd

  //for tft
 // Serial.begin(38400);
  tft.begin(0x9341);
  //tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  //tft.println(1000);
  tft.setRotation(3);
  //for tft
}

void loop() 
{
    //open fail and write string
    //if no fail - it's will be created
  File logFile = SD.open("LOG.txt", FILE_WRITE);
  if (logFile) 
 {
  String string_time = String(JY901.getTime("year")) + "-" + String(JY901.getTime("month")) + "-" + String(JY901.getTime("day")) + " " + String(JY901.getTime("hour")) + ":" + String(JY901.getTime("minute")) + ":" + String((float)JY901.getTime("second"));
  Serial.println("Time:" + string_time); 
   
  String string_Acc = String(JY901.getAccX()) + " " + String(JY901.getAccY()) + " " + String(JY901.getAccZ());     
  Serial.println("Acc:" + string_Acc);
  
  String string_Gyro = String(JY901.getGyroX()) + " " + String(JY901.getGyroY()) + " " + String(JY901.getGyroZ());
  Serial.println("Gyro:" + string_Gyro);

  String string_Angle = " Roll " + String(JY901.getRoll()) + " Pitch " + String(JY901.getPitch()) + " Yaw " + String(JY901.getYaw());
  Serial.println("Angle:" + string_Angle);

  String string_Mag = String(JY901.getMagX()) + " " + String(JY901.getMagY()) + " " + String(JY901.getMagZ());
  Serial.println("Mag:" + string_Mag);

//  String string_DStatus = String(JY901.getD0Status()) + " " + String(JY901.getD1Status()) + " " + String(JY901.getD2Status()) + " " + String(JY901.getD3Status());
//  Serial.println("DStatus:" + string_DStatus);
  
  String string_Longitude = String(JY901.getLon() / 10000000) + "Deg " + String((double)(JY901.getLon() % 10000000) / 1e5) + "m"; 
  Serial.println("Longitude:" + string_Longitude);

  String string_Lattitude = String(JY901.getLat() / 10000000) + "Deg " + String((double)(JY901.getLat() % 10000000) / 1e5) + "m";
  Serial.println("Lattitude:" + string_Lattitude);

  String string_GPSHeight = String(JY901.getGPSH()) + "m GPSYaw:" + String(JY901.getGPSY()) + " Deg GPSV:" + String(JY901.getGPSV()) + "km/h";
  Serial.println("GPSHeight: " +string_GPSHeight);

  String string_dop = String(JY901.getDOP("sn")) + " PDOP" + String(JY901.getDOP("pdop")) + " HDOP" + String(JY901.getDOP("hdop")) + " VDOP" + String(JY901.getDOP("vdop"));
  Serial.println("SN:" + string_dop + "\n");
  
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.print("Time " + String(JY901.getTime("day")) + "/" + String(JY901.getTime("month")) + " " +  String(JY901.getTime("hour")) + ":" + String(JY901.getTime("minute")) + ":" + String((float)JY901.getTime("second")));
    tft.setCursor(0, 20);
    tft.print("Lon " + string_Longitude);
    tft.setCursor(0, 40);
    tft.print("Lat " + string_Lattitude);
    tft.setCursor(0, 60);
    tft.print("AC: " + string_Acc);
    tft.setCursor(0, 80);
    tft.print("GY: " + string_Gyro);
    tft.setCursor(0, 100);
    tft.print("AN: " + string_Angle);
    tft.setCursor(0, 140);
    tft.print("GPSHeight: " +string_GPSHeight);
    tft.setCursor(0, 180);
    tft.print("SN:" +string_dop);
    
    logFile.println("Time:" + string_time);
    logFile.println("Acc:" + string_Acc);
    logFile.println("Gyro:" + string_Gyro);
    logFile.println("Angle:" + string_Angle);
    logFile.println("Mag:" + string_Mag);
    logFile.println("Longitude:" + string_Longitude);
    logFile.println("Lattitude:" + string_Lattitude);
    logFile.println("GPSHeight: " +string_GPSHeight);
    logFile.println("SN:" + string_dop + "\n");
    
    logFile.close();
  }  
  else 
  {
    Serial.println("LOG.txt");
    Serial.println("Couldn't open log file");
    tft.print("Couldn't open log file");
  }
    while (Serial1.available()) 
  {
    JY901.receiveSerialData(); //Call JY901 data cope function
  }
 delay(500);
}
