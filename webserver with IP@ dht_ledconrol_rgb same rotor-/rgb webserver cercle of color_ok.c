#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "TOPNET4F57F07B";
const char *password = "CF0D1A748E";

const int redPin = 15;
const int greenPin = 2;
const int bluePin = 0;

WebServer server(80);

String webpage = ""
                 "<!DOCTYPE html><html><head><title>RGB control</title><meta name='mobile-web-app-capable' content='yes' />"
                 "<meta name='viewport' content='width=device-width' /></head><body style='margin: 0px; padding: 0px;'>"
                 "<canvas id='colorspace'></canvas></body>"
                 "<script type='text/javascript'>"
                 "(function () {"
                 " var canvas = document.getElementById('colorspace');"
                 " var ctx = canvas.getContext('2d');"
                 " function drawCanvas() {"
                 " var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
                 " for(var i=0; i <= 360; i+=10) {"
                 " colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
                 " }"
                 " ctx.fillStyle = colours;"
                 " ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
                 " var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
                 " luminance.addColorStop(0, '#ffffff');"
                 " luminance.addColorStop(0.05, '#ffffff');"
                 " luminance.addColorStop(0.5, 'rgba(0,0,0,0)');"
                 " luminance.addColorStop(0.95, '#000000');"
                 " luminance.addColorStop(1, '#000000');"
                 " ctx.fillStyle = luminance;"
                 " ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
                 " }"
                 " var eventLocked = false;"
                 " function handleEvent(clientX, clientY) {"
                 " if(eventLocked) {"
                 " return;"
                 " }"
                 " function colourCorrect(v) {"
                 " return Math.round(1023-(v*v)/64);"
                 " }"
                 " var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
                 " var params = ["
                 " 'red=' + colourCorrect(data[0]),"
                 " 'green=' + colourCorrect(data[1]),"
                 " 'blue=' + colourCorrect(data[2])"
                 " ].join('&');"
                 " var req = new XMLHttpRequest();"
                 " req.open('POST', '/setcolor?' + params, true);"
                 " req.send();"
                 " eventLocked = true;"
                 " req.onreadystatechange = function() {"
                 " if(req.readyState == 4) {"
                 " eventLocked = false;"
                 " }"
                 " }"
                 " }"
                 " canvas.addEventListener('click', function(event) {"
                 " handleEvent(event.clientX, event.clientY, true);"
                 " }, false);"
                 " canvas.addEventListener('touchmove', function(event){"
                 " handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
                 "}, false);"
                 " function resizeCanvas() {"
                 " canvas.width = window.innerWidth;"
                 " canvas.height = window.innerHeight;"
                 " drawCanvas();"
                 " }"
                 " window.addEventListener('resize', resizeCanvas, false);"
                 " resizeCanvas();"
                 " drawCanvas();"
                 " document.ontouchmove = function(e) {e.preventDefault()};"
                 " })();"
                 "</script></html>";

void handleRoot()
{
  server.send(200, "text/html", webpage); // pos1 success response pos2 type of displyed format(html txt..) pos3 the content of the page you wanna display
}

void handleSetColor()
{
  int redValue = server.arg("red").toInt();
  int greenValue = server.arg("green").toInt();
  int blueValue = server.arg("blue").toInt();

  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  server.send(200, "text/plain", "Color set successfully");
}

void setup()
{
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
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

void loop()
{
  server.handleClient();
}