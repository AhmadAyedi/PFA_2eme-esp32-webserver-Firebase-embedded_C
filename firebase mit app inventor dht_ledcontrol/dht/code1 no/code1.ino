//#include "DHT.h"
#include <DHT.h>
#define DHTPIN 2 // Change pin number to 2 for Wemos Lolin32
#define DHTTYPE DHT11

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

DHT dht(DHTPIN, DHTTYPE);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "TOPNET4F57F07B"
#define WIFI_PASSWORD "CF0D1A748E"

// Insert Firebase project API Key
#define API_KEY "AIzaSyARpoTNNe-CviB4gJWxe01V2W9Y1QkC1CQ"

// Insert RTDB URL
#define DATABASE_URL "https://dht11-9289b-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

void setup() {
  pinMode(DHTPIN, INPUT);
  dht.begin();
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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (Firebase.ready() && signupOK ) {
    
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/kelembapan", h)) {
       Serial.print("Kelembapan: ");
       Serial.println(h);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/suhu", t)) {
       Serial.print("Suhu: ");
       Serial.println(t);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  Serial.println("______________________________");
}
