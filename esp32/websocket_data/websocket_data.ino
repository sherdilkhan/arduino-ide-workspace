
#include <WiFi.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char* ssid = "Sherdil";
const char* password = "03336830763BB";

// Set up the WebSocket server
WebSocketsServer webSocketServer = WebSocketsServer(81);

// Define the ADC pins
const int ADC1_0 = 0;
const int ADC1_1 = 1;
const int ADC1_2 = 2;

// Store the ADC readings
int adcReading1 = 0;
int adcReading2 = 0;
int adcReading3 = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectToWiFi();

  // Start the WebSocket server
  webSocketServer.begin();

  // Set the callback function for WebSocket events
  webSocketServer.onEvent(webSocketEvent);

  Serial.println("WebSocket server started");
}

void loop() {
  // Read ADC values
  adcReading1 = analogRead(ADC1_0);
  adcReading2 = analogRead(ADC1_1);
  adcReading3 = analogRead(ADC1_2);

  // Send ADC readings to connected clients
  sendADCReadings();

  // Handle WebSocket events
  webSocketServer.loop();
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Client disconnected\n", num);
      break;
  }
}

void sendADCReadings() {
  String json = "{\"adc1\":" + String(adcReading1) + ", \"adc2\":" + String(adcReading2) + ", \"adc3\":" + String(adcReading3) + "}";
  webSocketServer.broadcastTXT(json);
}
