# RFID_ACS
# RFID Access Control System with ESP8266, RTC, and LCD Display

## Gaurav Powar, Team Innovobotix

### Description
This RFID-based Access Control System enhances security by verifying user credentials through RFID tags. Built around an ESP8266 microcontroller, the system integrates an MFRC522 RFID reader, DS3231 Real-Time Clock (RTC) module, and a 16x2 I2C LCD display. 

Upon successful authentication, a relay module unlocks a door while logging the access event with an accurate timestamp. Unauthorized attempts are logged, and alerts are sent via WiFi for remote monitoring. This system is ideal for securing restricted areas in homes, offices, or industrial facilities.

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

### Software Setup

#### Prerequisites  
1. Install **Arduino IDE** or **PlatformIO**.  
2. Add ESP8266 Board Support in the Arduino IDE (**Board Manager > ESP8266 by ESP8266 Community**).  

#### Required Libraries  
- **MFRC522** (for RFID module)  
- **SPI** (for SPI communication)  
- **Wire** (for I2C communication)  
- **RTClib** (for DS3231 RTC)  
- **LiquidCrystal_I2C** (for LCD display)  
- **ESP8266WiFi** (for WiFi communication)  
- **ESP8266HTTPClient** (for sending alerts)  

**Installation:**  
To install these libraries in Arduino IDE, navigate to **Sketch > Include Library > Manage Libraries**, then search and install each library.

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

### Functionality Overview

1. **User Scans RFID Tag** â†’ The MFRC522 module reads the card's unique ID.  
2. **Authentication Check** â†’ The ESP8266 verifies the ID against a stored list.  
3. **Access Granted:**  
   - The relay module activates, unlocking the door.  
   - LCD displays a welcome message.  
   - The RTC logs the access timestamp.  
4. **Access Denied (Unauthorized Tag):**  
   - LCD displays "Access Denied."  
   - The attempt is logged with a timestamp.  
   - An alert is sent via WiFi.  

### Future Enhancements
- **Web-Based Management Panel**: A web dashboard for remote access control.  
- **Cloud Storage Integration**: Storing logs in a database.  
- **Mobile Notifications**: Sending alerts via push notifications.  
- **Multi-Factor Authentication**: Adding a password or biometric layer.  
- **Integration with a database** for dynamic RFID tag management.

### Troubleshooting
- **WiFi Not Connecting:** Verify SSID and password, check for signal strength.
- **RFID Not Scanning:** Ensure proper wiring and power supply.
- **LCD Not Displaying:** Check I2C address and connections.

### License
This project is open-source and free to use under the MIT License.

---
*Gaurav Powar*  
*Team Innovobotix*  
*gaurav.teaminnovobotix@gmail.com*