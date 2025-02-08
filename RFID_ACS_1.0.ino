#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include "secrets.h" // Separate file for Wi-Fi credentials

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

// Authorized RFID Tags
struct RFIDTag {
  const char* uid;
  const char* name;
};

RFIDTag masterTags[] = {
  {"854ED383", "Tag_1"},
  {"19D968D3", "Tag_2"},
  {"57EF3835", "Tag_3"}
};

const int tagCount = sizeof(masterTags) / sizeof(masterTags[0]);
String lastMessage = "";
unsigned long lastOpenTime = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  updateLCD("Scan RFID/FP\nor Enter PIN");

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
  connectWiFi();
}

void loop() {
  if (getID() || checkFingerprint()) {
    grantAccess();
  }
  if (millis() - lastOpenTime > 2000) {
    updateLCD("TEAM INNOVOBOTIX\nScan RFID/FP");
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected!" : "Failed to connect to Wi-Fi");
}

boolean getID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return false;

  char tagID[9];
  snprintf(tagID, sizeof(tagID), "%02X%02X%02X%02X", mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);
  mfrc522.PICC_HaltA();

  for (int i = 0; i < tagCount; i++) {
    if (strcmp(tagID, masterTags[i].uid) == 0) return true;
  }

  sendAlert(tagID);
  return false;
}

boolean checkFingerprint() {
  if (finger.getImage() != FINGERPRINT_OK) return false;
  if (finger.image2Tz() != FINGERPRINT_OK) return false;
  if (finger.fingerFastSearch() == FINGERPRINT_OK) return true;

  sendAlert("Fingerprint");
  return false;
}

void grantAccess() {
  Serial.println("Access Granted! Activating the relay...");
  updateLCD("Access Granted!");

  digitalWrite(RELAY_CONTROL_PIN, LOW);
  unsigned long grantStart = millis();
  while (millis() - grantStart < 5000) {
    delay(1);
  }
  Serial.println("Deactivating the relay...");
  digitalWrite(RELAY_CONTROL_PIN, HIGH);
  lastOpenTime = millis();
}

void sendAlert(String id) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Alert not sent.");
    return;
  }
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"id\": \"" + id + "\"}";
  
  int retries = 3;
  int httpResponseCode = -1;
  while (retries-- > 0 && httpResponseCode < 0) {
    httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) break;
    Serial.println("Retrying alert...");
    delay(2000);
  }
  Serial.print("Alert sent. Server Response: ");
  Serial.println(httpResponseCode);
  http.end();
}

void updateLCD(const String& message) {
  if (message != lastMessage) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    lastMessage = message;
  }
}
// Please Do Not Use This For Your Project without Prior Information, The Project Is Yet To Meet The Expected Outcomes.
