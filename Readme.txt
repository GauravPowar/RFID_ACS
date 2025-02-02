# RFID Access System with ESP8266, RTC, and LCD Display

## Gaurav Powar, Team Innovobotix

### Description
This project is an RFID-based access control system that utilizes an ESP8266 microcontroller, an MFRC522 RFID module, a DS3231 RTC module, and an I2C LCD display. It grants access to authorized users and logs unauthorized attempts, sending alerts via WiFi.

### Components Required
- **ESP8266** (NodeMCU or similar)
- **MFRC522 RFID Reader**
- **RFID Tags**
- **DS3231 RTC Module**
- **I2C LCD Display (16x2)**
- **Relay Module**
- **Jumper Wires**
- **Power Supply**

### Wiring Instructions
#### **MFRC522 to ESP8266**
| MFRC522 Pin | ESP8266 Pin |
|------------|------------|
| RST        | D3         |
| SS (SDA)   | D4         |
| MOSI       | D7         |
| MISO       | D6         |
| SCK        | D5         |
| GND        | GND        |
| 3.3V       | 3.3V       |

#### **Relay Module**
| Relay Pin | ESP8266 Pin |
|-----------|------------|
| Signal    | D1         |
| GND       | GND        |
| VCC       | 5V         |

#### **LCD Display (I2C)**
| LCD Pin | ESP8266 Pin |
|---------|------------|
| SDA     | D2         |
| SCL     | D1         |
| GND     | GND        |
| VCC     | 5V         |

### Software Dependencies
- **Arduino IDE** (or PlatformIO)
- **Libraries:**
  - MFRC522
  - SPI
  - Wire
  - RTClib
  - LiquidCrystal_I2C
  - ESP8266WiFi
  - ESP8266HTTPClient

### Setup & Configuration
1. Install the required libraries in the Arduino IDE.
2. Update the WiFi credentials in the code:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Set up the server URL for logging unauthorized access attempts:
   ```cpp
   const char* serverUrl = "http://yourserver.com/alert";
   ```
4. Upload the code to the ESP8266 using Arduino IDE.
5. Power the circuit and scan an RFID tag.

### Functionality
- **Authorized Access:** If the scanned RFID tag is in the predefined list, access is granted and the relay is activated.
- **Unauthorized Access:** If an unregistered tag is scanned, an alert is sent to the server, and access is denied.
- **Real-Time Clock (RTC):** The system logs access timestamps using the DS3231 module.
- **LCD Display:** Provides user feedback such as access granted, denied, and system status.

### Future Enhancements
- Integration with a database for dynamic RFID tag management.
- Web interface for remote monitoring and access control.
- Additional security layers like PIN entry or biometric authentication.

### Troubleshooting
- **WiFi Not Connecting:** Verify SSID and password, check for signal strength.
- **RFID Not Scanning:** Ensure proper wiring and power supply.
- **LCD Not Displaying:** Check I2C address and connections.

### License
This project is open-source and free to use under the MIT License.

---
**Team Innovobotix** 🚀
