#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#define SW 4
#define MO 2

Servo sv;
const char* state;
const char* ssid = "SSID";
const char* password = "PASSWORD";

//必要ならBasic認証 (If you need authentication.)
const char* www_username = "USERNAME";
const char* www_password = "PASSWORD";

int sw;
ESP8266WebServer server(80);
WiFiClient client;

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void keystate() {
  server.send ( 200, "POST", state );
}

void unlock() {
  delay(500);
  sv.write(必要な角度(You need angles));
  delay(1000);
  state = "UNLOCK";
  server.send ( 200, "POST", state);
}

void lock() {
  delay(500);
  sv.write(必要な角度(You need angles));
  delay(1000);
  state = "LOCK";
  server.send ( 200, "POST", state);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", keystate) {
    if (!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/plain", "Login OK");
  });
  server.on("/unlock", unlock) {
    if (!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/plain", "Login OK");
  });

  server.on("/lock", lock) {
    if (!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/plain", "Login OK");
  });
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  sv.attach(2, 800, 2300);

  pinMode(SW, INPUT);
  pinMode(MO, INPUT);

}

void loop(void) {
  server.handleClient();

  if (digitalRead(SW) == 1) {     //スイッチの状態を調べる
    if (state == "UNLOCK") {
      lock();
    } else {
      unlock();
    }
  }
}
