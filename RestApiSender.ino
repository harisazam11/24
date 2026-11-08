#include <WiFi.h>
#include <HTTPClient.h>

#define FIREBASE_HOST "health-monitorin-101d7-default-rtdb.firebaseio.com"

#define WIFI_SSID "Alpha_Beta_Gamma" // Change the name of your WiFi

#define WIFI_PASSWORD "Alpha@123" // Change the password of your WIFI your WiFi

#define FIREBASE_Authorization_key
"eJxXRNeUbDAEX8xsPYJ0cyxT56buKgD2G6wCUYTF"

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin("https://" + String(firebaseHost) + String(firebasePath));
    http.addHeader("Content-Type", "application/json");

    // Example POST request
    int httpResponseCode = http.POST("{\"data\":123}");

    if (httpResponseCode > 0) {
      Serial.print("POST Request sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error on POST Request. Error code: ");
      Serial.println(httpResponseCode);
    }

    // Example GET request
    // int httpResponseCode = http.GET();

    // if (httpResponseCode > 0) {
    // String payload = http.getString();
    // Serial.print("GET Request sent. Response payload: ");
    // Serial.println(payload);
  //} else {
  // Serial.print("Error on GET Request. Error code: ");
  // Serial.println(httpResponseCode);
  }

  http.end();

  delay(5000); // Adjust delay as needed
}