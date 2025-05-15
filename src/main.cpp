#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* NTP_SERVER = "time.google.com";
const long gmtOffset_sec = 7*3600;
const int dayoflightOffset_sec = 0;

LiquidCrystal_I2C lcd (0x27, 16, 2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, gmtOffset_sec, dayoflightOffset_sec);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 30*60*1000;
unsigned long lastLCD = 0;

int hours, minutes, seconds;
int day, month, year;
int dayOfWeek;

const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void updateTimeFromNTP() {
  if (timeClient.update()) {
    hours = timeClient.getHours();
    minutes = timeClient.getMinutes();
    seconds = timeClient.getSeconds();

    unsigned long epochTime = timeClient.getEpochTime();
    struct tm* timeInfo;
    time_t rawTime = epochTime;
    timeInfo = localtime(&rawTime);
    day = timeInfo->tm_mday;
    month = timeInfo->tm_mon + 1; // tm_mon bắt đầu từ 0
    year = timeInfo->tm_year + 1900; // tm_year là năm kể từ 1900
    dayOfWeek = timeInfo->tm_wday; // 0 = Chủ nhật, 1 = Thứ 2, ...

    Serial.printf("NTP Updated: %s %02d/%02d/%04d %02d:%02d:%02d\n",
                  daysOfWeek[dayOfWeek], day, month, year, hours, minutes, seconds);
  } else {
    Serial.println("NTP Update failed");
  }
}

void updateLocalTime() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
  }
  if (minutes >= 60) {
    minutes = 0;
    hours++;
  }
  if (hours >= 24) {
    hours = 0;
    updateTimeFromNTP();
    lastUpdate = millis();
  }
}

void displayTime() {
  char dateStr[16];
  sniprintf(dateStr, sizeof(dateStr), "%s %02d/%02d/%04d", daysOfWeek[dayOfWeek], day, month, year);
  lcd.setCursor(1,0);
  lcd.print(dateStr);

  char timeStr[16];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
  lcd.setCursor(4,1);
  lcd.print(timeStr);
}

void setup(){
  Serial.begin(115200);

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Connecting");
  lcd.setCursor(3,1);
  lcd.print("VanMinhCS");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  timeClient.begin();
  updateTimeFromNTP();
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= updateInterval) {
    updateTimeFromNTP();
    lastUpdate = currentMillis;
  }
  if (currentMillis - lastLCD >= 1000){
    updateLocalTime();
    displayTime();
    lastLCD = currentMillis;
  }
  

}
