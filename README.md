# RFID & Fingerprint-Based Access Control System

## Overview
This project implements a secure access control system using RFID and fingerprint authentication. It is powered by an ESP8266 microcontroller and integrates an MFRC522 RFID module, an Adafruit fingerprint sensor, an RTC module, and an LCD display for user feedback. Unauthorized access attempts trigger an alert via an HTTP request.

## Features
- RFID authentication with pre-registered tags
- Fingerprint authentication with Adafruit fingerprint sensor
- Real-time clock (RTC) integration
- LCD display for real-time feedback
- Wi-Fi connectivity for remote alerting
- Secure relay control for access management
- PIN-based fallback authentication (planned feature)

## Hardware Requirements
- ESP8266 (NodeMCU or similar)
- MFRC522 RFID Reader
- Adafruit Fingerprint Sensor
- DS3231 RTC Module
- LiquidCrystal_I2C 16x2 LCD Display
- Relay Module
- Jumper Wires
- Power Supply (5V)

## Software Requirements
- Arduino IDE with ESP8266 Board Support
- Required Libraries:
  - `MFRC522.h`
  - `SPI.h`
  - `Wire.h`
  - `RTClib.h`
  - `LiquidCrystal_I2C.h`
  - `ESP8266WiFi.h`
  - `ESP8266HTTPClient.h`
  - `Adafruit_Fingerprint.h`

## Circuit Connections
| Component        | ESP8266 Pin |
|-----------------|------------|
| MFRC522 SS      | D4         |
| MFRC522 RST     | D3         |
| Fingerprint TX  | D8         |
| Fingerprint RX  | D7         |
| Relay Control   | D1         |
| LCD SDA         | D2         |
| LCD SCL         | D5         |

## Setup Instructions
1. Install Arduino IDE and add ESP8266 board support.
2. Install the required libraries via the Library Manager.
3. Clone this repository and open the `.ino` file in Arduino IDE.
4. Create a `secrets.h` file with Wi-Fi credentials:
   ```cpp
   #define ssid "YOUR_WIFI_SSID"
   #define password "YOUR_WIFI_PASSWORD"
   #define serverUrl "http://yourserver.com/alert"
   ```
5. Upload the code to your ESP8266.
6. Power the system and monitor serial output for debug messages.

## How It Works
- The system continuously scans for RFID tags and fingerprints.
- If a recognized RFID tag or fingerprint is detected, the relay is activated, granting access.
- Unauthorized attempts trigger an HTTP alert to a remote server.
- The LCD provides real-time status updates.

## Future Improvements
- Integration with a secure database for dynamic access control
- Mobile app for remote monitoring and access management
- Additional authentication methods (PIN entry, facial recognition)

## Disclaimer
**Do not use this project in real-world security applications without extensive testing.** The project is still under development and has not yet met all expected outcomes.

## Acknowledgment
Developed under **Team Innovobotix**.
