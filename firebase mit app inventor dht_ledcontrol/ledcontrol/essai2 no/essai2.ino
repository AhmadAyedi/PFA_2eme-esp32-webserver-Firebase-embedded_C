#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <TokenHelper.h> // Ensure this import is present

/* Define WiFi credentials */
#define WIFI_SSID "TOPNET4F57F07B"
#define WIFI_PASSWORD "CF0D1A748E"

/* Define Firebase credentials */
#define API_KEY "AIzaSyCZR1HehRNWtXhijDuopPe-9ZlnuDGl5WM"
#define DATABASE_URL "https://ledcontrolyes-default-rtdb.firebaseio.com/"
#define USER_EMAIL "ahmad.ayadi2020@gmail.com"
#define USER_PASSWORD "123456"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int bleuLED = 2;
const int RedLED = 4;

/* Define token status callback function */
void tokenStatusCallback(TokenInfo info) {
  Serial.printf("Token Info: Type = %s, Status = %s, Time = %ld\n",
                info.type.c_str(), info.status.c_str(), info.time);
}

void setup() {
  pinMode(bleuLED, OUTPUT);
  digitalWrite(bleuLED, LOW);
  pinMode(RedLED, OUTPUT);
  digitalWrite(RedLED, LOW);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  /* Assign user sign-in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the token generation task */
  config.token_status_callback = tokenStatusCallback;

  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096, 1024); // Set BSSL buffer size
  fbdo.setResponseSize(2048);         // Limit response payload size

  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000; // Set server response timeout
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getString(&fbdo, "/MITLedControl/blue/state")) {
      digitalWrite(bleuLED, atoi(fbdo.to<const char *>()));
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }

    if (Firebase.RTDB.getString(&fbdo, "/MITLedControl/red/state")) {
      digitalWrite(RedLED, atoi(fbdo.to<const char *>()));
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
  }
}
