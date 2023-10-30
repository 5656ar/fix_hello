#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#include <time.h>
#include <BH1750.h>
#include <Wire.h>
#include <TridentTD_LineNotify.h>
 
 
// Config Firebase
#define FIREBASE_HOST "final-test-7328e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "UIr29C25sCfajsVyBN5FURrd4LeQOuahnRSJcm9n"


// Config connect WiFi
#define WIFI_SSID "Bunnapon"
#define WIFI_PASSWORD "Bunnapon123"
#define LINE_TOKEN  "fw7T6yctjysHilpe5ETnMfzqIXCia0bWbuIVNqQEuKg"
 
// Config DHT
#define DHTPIN D4
#define DHTTYPE DHT22
 
// Config time
int timezone = 7;
 
char ntp_server1[20] = "ntp.ku.ac.th";
char ntp_server2[20] = "fw.eng.ku.ac.th";
char ntp_server3[20] = "time.uni.net.th";
 
int dst = 0;
 
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
 
void setup() {
Serial.begin(9600);
 
WiFi.mode(WIFI_STA);
// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
Wire.begin();
lightMeter.begin();

while (WiFi.status() != WL_CONNECTED) {
Serial.print("...");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());

LINE.setToken(LINE_TOKEN);

configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
Serial.println("Waiting for time");
while (!time(nullptr)) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.println("Now: " + NowString());
 
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
dht.begin();
}
 
void loop() {
  // Read temp & Humidity for DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float lux = lightMeter.readLightLevel();
  lux = round(lux * 100) / 100.0;
  LINE.notify("Temperature: " + String(t) + " Humidity: " + String(h)+ " Light: " + String(lux)+ "                      Date & Time: " + NowString());
  
  if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  delay(500);
  return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" %\t");
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println();
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["temperature"] = t;
  root["humidity"] = h;
  root["light"] = lux;
  root["time"] = NowString();
  
  // append a new value to /logDHT
  String name = Firebase.push("logDHT", root);
  // handle error
  if (Firebase.failed()) {
  Serial.print("pushing /logs failed:");
  Serial.println(Firebase.error());
  return;
  }
  Serial.print("pushed: /logDHT/");
  Serial.println(name);
  delay(10000);
}
 
String NowString() {
time_t now = time(nullptr);
struct tm* newtime = localtime(&now);
 
String tmpNow = "";
tmpNow += String(newtime->tm_year + 1900);
tmpNow += "-";
tmpNow += String(newtime->tm_mon + 1);
tmpNow += "-";
tmpNow += String(newtime->tm_mday);
tmpNow += " ";
tmpNow += String(newtime->tm_hour);
tmpNow += ":";
tmpNow += String(newtime->tm_min);
tmpNow += ":";
tmpNow += String(newtime->tm_sec);
return tmpNow;
}
