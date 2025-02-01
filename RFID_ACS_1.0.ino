#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

constexpr uint8_t RST_PIN = 9;
constexpr uint8_t SS_PIN = 10;
constexpr uint8_t RELAY_CONTROL_PIN = 6;

MFRC522 mfrc522(SS_PIN, RST_PIN);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "http://yourserver.com/alert"; // Replace with your server URL

struct RFIDTag {
  String uid;
  String name;
};

RFIDTag masterTags[3] = {
  {"854ED383", "Tag_1"},
  {"19D968D3", "Tag_2"},
  {"57EF3835", "Tag_3"}
};

String tagID = "";
unsigned long lastOpenTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Tag");
  lcd.setCursor(0, 1);
  lcd.print("or Enter PIN");
  lcd.setBacklight(255);

  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  pinMode(RELAY_CONTROL_PIN, OUTPUT);
  digitalWrite(RELAY_CONTROL_PIN, HIGH);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() {
  if (getID()) {
    bool granted = false;
    String name = "";

    for (int i = 0; i < 3; i++) {
      if (tagID == masterTags[i].uid) {
        granted = true;
        name = masterTags[i].name;
        break;
      }
    }

    DateTime now = rtc.now();
    char dateTime[20];
    sprintf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

    Serial.print("Date & Time: ");
    Serial.println(dateTime);
    Serial.print("UID: ");
    Serial.println(tagID);
    Serial.print("Name: ");
    Serial.println(name);
    Serial.println(granted ? "Access Granted!" : "Access Denied!");

    lcd.clear();
    lcd.setCursor(0, 0);

    if (granted) {
      lcd.print("Access Granted!");
      openDoor();
    } else {
      lcd.print("Access Denied!");
      sendAlert(tagID, dateTime); // Send alert for unauthorized access
    }

    lastOpenTime = millis();
    mfrc522.PCD_Init();
    delay(2000);
  }

  if (millis() - lastOpenTime > 2000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEAM INNOVOBOTIX");
    lcd.setCursor(0, 1);
    lcd.print("Scan RFID Tag");
  }
}

boolean getID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}

void openDoor() {
  Serial.println("Access Granted! Activating the relay...");
  digitalWrite(RELAY_CONTROL_PIN, LOW);
  delay(5000);
  Serial.println("Deactivating the relay...");
  digitalWrite(RELAY_CONTROL_PIN, HIGH);
  delay(2000);
}

void sendAlert(String uid, String timestamp) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"uid\": \"" + uid + "\", \"timestamp\": \"" + timestamp + "\"}";
    int httpResponseCode = http.POST(payload);
    Serial.print("Alert sent. Server Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi not connected. Alert not sent.");
  }
}
