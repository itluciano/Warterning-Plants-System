#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 2);

#define BLYNK_PRINT Serial

//blynk code
char auth[] = "c643f248154345a4b9dd100b6ed63617";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "_Itluciano";
char pass[] = "jjacknarongrit";

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(D4,OUTPUT);
  pinMode(D3,OUTPUT);
  digitalWrite(D3, HIGH);
  lcd.begin();
  lcd.backlight(); // เปิดไฟ backlight

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}


void loop()
{
  Blynk.run();
  
  //variable
  const int sensor_pin = A0;
  float moisture_percentage;
  float sensor_value;
  
  //Calulate
  sensor_value = analogRead(sensor_pin) - 280; //หาค่าค่าที่ได้จากเซนเซอร์ (ค่าที่ต่ำสุด) 
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1024.00) * 100.00 )); //หาค่าความชื้นจากเซนเซอร์เป็นเปอร์เซ็นต์

  Blynk.virtualWrite(V1, moisture_percentage);
  Blynk.virtualWrite(V2, sensor_value);

  lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่  บรรทัดที่ 0
  lcd.print("Sensor: " + String(sensor_value));
  
  lcd.setCursor(0, 1); // ไปที่ตัวอักษรที่ 0 บรรทัดที่ 1
  lcd.print("Moisture: " + String(moisture_percentage) + '%');

  //Serial Monitor แสดงค่าความชื้นเป็นเปอร์เซ็น
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  //Serial Monitor แสดงค่าต่ำสุดที่หาได้จากเซ็นเซอร์
  Serial.print("Sensor = ");
  Serial.println(sensor_value);
  
  delay(2000);
  
  if(moisture_percentage <= 40)
  {
  Blynk.notify("กำลังทำการรดน้ำต้นไม้");
  digitalWrite(D4,HIGH);
  digitalWrite(D3,LOW);
  Serial.println("กำลังทำการรดน้ำต้นไม้");
  }
  else{
  Blynk.notify("หยุดทำการรดน้ำต้นไม้");
  digitalWrite(D4,LOW);
  digitalWrite(D3,HIGH);
  Serial.println("หยุดทำการรดน้ำต้นไม้");
}
}
