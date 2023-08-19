

#include <WiFi.h>
#include <WebSocketsServer.h>
#include "max6675.h"
#include <Wire.h>  //for i2c
#include <BH1750.h>


// MAX6675 Setup on SPI Interface for tc1
int thermoDO = 0;
int thermoCLK = 1;
int thermoCS1 = 2;
// MAX6675 Setup on SPI Interface for tc1

MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);

// MAX6675 Setup on SPI Interface for tc2
int thermoCS2 = 3;
MAX6675 thermocouple2(thermoCLK, thermoCS2, thermoDO);


// BH1750 Setup on I2C Interface
#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 8
BH1750 lightMeter1;


#define lux1_en 4
#define lux2_en 5
#define lux3_en 6
#define lux4_en 7
#define lux5_en 18


// Replace with your network credentials
const char* ssid = "Sherdil114";
const char* password = "crescent114";

// Set up the WebSocket server
WebSocketsServer webSocketServer = WebSocketsServer(81);

// Define Lux Variables
uint16_t lux1 = 0;
uint16_t lux2 = 0;
uint16_t lux3 = 0;
uint16_t lux4 = 0;
uint16_t lux5 = 0;

//TC Sensor 1
float tc1 = 0;
//TC Sensor 2
float tc2 = 0;

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

  pinMode(lux1_en, OUTPUT);  // to avoid bh1750 error
  pinMode(lux2_en, OUTPUT);  // to avoid bh1750 error
  pinMode(lux3_en, OUTPUT);  // to avoid bh1750 error
  pinMode(lux4_en, OUTPUT);  // to avoid bh1750 error
  pinMode(lux5_en, OUTPUT);  // to avoid bh1750 error
  digitalWrite(lux1_en, HIGH);
  digitalWrite(lux2_en, HIGH);
  digitalWrite(lux3_en, HIGH);
  digitalWrite(lux4_en, HIGH);
  digitalWrite(lux5_en, HIGH);


  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  lightMeter1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0X5C);
  
  // Initialize the second MAX6675 sensor
  pinMode(thermoCS2, OUTPUT);
  digitalWrite(thermoCS2, HIGH);
  digitalWrite(lux1_en, LOW);
  digitalWrite(lux2_en, LOW);
  digitalWrite(lux3_en, LOW);
  digitalWrite(lux4_en, LOW);
  digitalWrite(lux5_en, LOW);

}

void loop() {

  // Read BH1750 @ 0X5C Value in Lux
  digitalWrite(lux1_en, HIGH);
  lux1 = lightMeter1.readLightLevel();
  digitalWrite(lux1_en, LOW);

  digitalWrite(lux2_en, HIGH);
  lux2 = lightMeter1.readLightLevel();
  digitalWrite(lux2_en, LOW);

  digitalWrite(lux3_en, HIGH);
  lux3 = lightMeter1.readLightLevel();
  digitalWrite(lux3_en, LOW);

  digitalWrite(lux4_en, HIGH);
  lux4 = lightMeter1.readLightLevel();
  digitalWrite(lux4_en, LOW);

  digitalWrite(lux5_en, HIGH);
  lux5 = lightMeter1.readLightLevel();
  digitalWrite(lux5_en, LOW);


  // Read MAX6675 (01) Value in Degree C
  tc1 = thermocouple1.readCelsius();
  tc2 = thermocouple2.readCelsius();
  delay(180);
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
  String json = "{\"lux1\":" + String(lux1) + ",\"lux2\":" + String(lux2) + ",\"lux3\":" + String(lux3) + ",\"lux4\":" + String(lux4) + ",\"lux5\":" + String(lux5) + ", \"tc1\":" + String(tc1) + ", \"tc2\":" + String(tc2) + "}";
  webSocketServer.broadcastTXT(json);
}