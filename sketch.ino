#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>


// WiFi Credentials
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define ENDPOINT_URL "https://iot-edge-sample-ws71.free.beeceptor.com/api/v1/data-records"
HTTPClient httpClient;

#define CONTENT_TYPE_HEADER "Content-Type"
#define APPLICATION_JSON "application/json"

#define DEVICE_ID "HC001"

void setup() {
  // Initialize Serial Output
  Serial.begin(115200);
  
  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // HTTP Client setyup
  httpClient.begin(ENDPOINT_URL);
  httpClient.addHeader(CONTENT_TYPE_HEADER, APPLICATION_JSON);

  // Data Record Build
  JsonDocument dataRecord;
  dataRecord["deviceId"] = DEVICE_ID;
  dataRecord["distance"] = 250;

  // Serialize and POST data record
  String dataRecordResource;
  serializeJson(dataRecord, dataRecordResource);
  httpClient.POST(dataRecordResource);

  // Check HTTP Response
  JsonDocument response;
  String responseResource;
  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);
  Serial.println();
  Serial.print("Distance: ");
  Serial.println(response["distance"].as<long>());
  httpClient.end();

  // Get all
  httpClient.begin(ENDPOINT_URL);
  int httpResponseCode = httpClient.GET();
  Serial.print("Response code: ");
  Serial.println(httpResponseCode);

  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);

  Serial.println("About to show id");
  JsonArray dataRecords = response.as<JsonArray>();
  if(dataRecords != NULL) {
    for(JsonVariant dataRecord : dataRecords) {
      Serial.print("id: ");
      Serial.println(dataRecord["id"].as<String>());
    }
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
