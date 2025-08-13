# ESP32 Fake Wi-Fi Captive Portal

⚠️ **Educational use only.**  
This project demonstrates how to set up a custom ESP32 Wi-Fi access point with a captive portal.  
It can serve different HTML login pages (Google-style or Wi-Fi password prompt) based on user input over Serial.

---

## Features
- Starts an ESP32 in Access Point mode
- Custom SSID & password (or open network)
- Two portal types:
  - **Super Google** – fake Google login page
  - **Super Pass** – fake Wi-Fi password page
- Captures submitted form data and prints to Serial
- DNS redirection so all HTTP requests show your page

---

## Requirements
- ESP32 board
- Arduino IDE with **ESP32 core** installed

---

## Installation
1. Open Arduino IDE
2. Install **ESP32 board support** (Tools → Board → Board Manager → Search "ESP32")
3. Select your ESP32 board (e.g., ESP32 Dev Module)
4. Copy `Super_Google.ino` into Arduino IDE
5. Connect ESP32 via USB
6. Upload sketch

---

## Usage
1. Open **Serial Monitor** at `115200 baud`
2. When prompted:
   - Enter SSID  
   - Enter Password (leave blank for open network)  
   - Enter portal type:  
     - `super google` for Google-style page  
     - `super pass` for Wi-Fi password page  
3. ESP32 will:
   - Create a Wi-Fi AP with given SSID/password
   - Start a captive portal redirecting all requests to the chosen page
4. Any login form submissions will be printed to the Serial Monitor.
