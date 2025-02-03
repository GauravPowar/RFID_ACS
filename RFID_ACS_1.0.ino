#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Fingerprint.h>

// Pin Definitions
constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;
constexpr uint8_t RELAY_CONTROL_PIN = D1;
constexpr uint8_t FINGERPRINT_TX = D8;
constexpr uint8_t FINGERPRINT_RX = D7;

// Hardware Objects
MFRC522 mfrc522(SS_PIN, RST_PIN);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial fingerSerial(FINGERPRINT_TX, FINGERPRINT_RX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "http://yourserver.com/alert";

// Authorized RFID Tags
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
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID/FP");
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
  finger.begin(57600);
  finger.getTemplateCount();

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
  if (getID() || checkFingerprint()) {
    grantAccess();
  }

  if (millis() - lastOpenTime > 2000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEAM INNOVOBOTIX");
    lcd.setCursor(0, 1);
    lcd.print("Scan RFID/FP");
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
  for (int i = 0; i < 3; i++) {
    if (tagID == masterTags[i].uid) return true;
  }
  sendAlert(tagID);
  return false;
}

boolean checkFingerprint() {
  int id = finger.getImage();
  if (id != FINGERPRINT_OK) return false;
  id = finger.image2Tz();
  if (id != FINGERPRINT_OK) return false;
  id = finger.fingerFastSearch();
  if (id == FINGERPRINT_OK) return true;
  sendAlert("Fingerprint");
  return false;
}

void grantAccess() {
  Serial.println("Access Granted! Activating the relay...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted!");
  digitalWrite(RELAY_CONTROL_PIN, LOW);
  delay(5000);
  Serial.println("Deactivating the relay...");
  digitalWrite(RELAY_CONTROL_PIN, HIGH);
  lastOpenTime = millis();
}

void sendAlert(String id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"id\": \"" + id + "\"}";
    int httpResponseCode = http.POST(payload);
    Serial.print("Alert sent. Server Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi not connected. Alert not sent.");
  }
}
