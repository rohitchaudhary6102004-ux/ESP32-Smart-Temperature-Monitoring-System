#include <FS.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <WebServer.h>

#define DHTTYPE DHT11
#define DHTPIN 4
#define ledPin 2

const char ssid[] = "IOT";
const char password[] = "12345678";
unsigned long _time = 0;

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void initWifi() {
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not Connected");
    Serial.print("Connecting..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("Wifi Connected to : " + WiFi.SSID());
  } else {
    Serial.println("Wifi Connected : " + WiFi.SSID());
  }
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  WiFi.setHostname("espbyrohit");
  initWifi();
  if (MDNS.begin("espbyrohit")) {
    Serial.println("mDNS has Started");
  } else {
    Serial.println("mDNS has not Started");
  }
  Serial.println("Server Started");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("LittleFS Mounted");
  dht.begin();
  server.on("/", []() {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      server.send(404, "text/plain", "File Not Found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });
  server.on("/style.css", []() {
    File file = LittleFS.open("/style.css", "r");
    server.streamFile(file, "text/css");
    file.close();
  });
  server.on("/script.js", []() {
    File file = LittleFS.open("/script.js", "r");
    server.streamFile(file, "application/javascript");
    file.close();
  });
  server.on("/temperature", []() {
    server.send(200, "text/plain", String(dht.readTemperature()));
  });

  server.on("/humidity", []() {
    server.send(200, "text/plain", String(dht.readHumidity()));
  });
  server.begin();
}

void loop() {
  server.handleClient();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (millis() - _time > 1000) {
    _time = millis();
    Serial.println("Temperature : " + String(temperature));
    Serial.println("Humidity : " + String(humidity));
    Serial.println(" ");
  }
}