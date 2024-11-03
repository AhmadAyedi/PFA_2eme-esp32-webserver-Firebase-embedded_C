#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "TOPNET4F57F07B";
const char *password = "CF0D1A748E";

const int redPin = 15;
const int greenPin = 2;
const int bluePin = 0;

WebServer server(80);

String webpage = ""
"<!DOCTYPE html><html><head><title>RGB Control</title></head><body>"
"<h1>RGB Control</h1>"
"<form method='POST' action='/setcolor'>"
"Red (0-255): <input type='text' name='red'><br>"
"Green (0-255): <input type='text' name='green'><br>"
"Blue (0-255): <input type='text' name='blue'><br>"
"<input type='submit' value='Set Color'>"
"</form>"
"</body></html>";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleSetColor() {
  int redValue = server.arg("red").toInt();
  int greenValue = server.arg("green").toInt();
  int blueValue = server.arg("blue").toInt();

  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  server.send(200, "text/plain", "Color set successfully");
}

void setup() {
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/setcolor", HTTP_POST, handleSetColor);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}