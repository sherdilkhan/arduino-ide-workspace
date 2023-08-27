# 1 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino"


# 4 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino" 2
# 5 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino" 2
# 6 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino" 2
# 7 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino" 2
# 8 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino" 2


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


BH1750 lightMeter1;
# 36 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32C3\\lux_monitor_websocket\\lux_monitor_websocket.ino"
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

  pinMode(4, 0x03); // to avoid bh1750 error
  pinMode(5, 0x03); // to avoid bh1750 error
  pinMode(6, 0x03); // to avoid bh1750 error
  pinMode(7, 0x03); // to avoid bh1750 error
  pinMode(18, 0x03); // to avoid bh1750 error
  digitalWrite(4, 0x1);
  digitalWrite(5, 0x1);
  digitalWrite(6, 0x1);
  digitalWrite(7, 0x1);
  digitalWrite(18, 0x1);


  Wire.begin(9, 8);
  lightMeter1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0X5C);

  // Initialize the second MAX6675 sensor
  pinMode(thermoCS2, 0x03);
  digitalWrite(thermoCS2, 0x1);
  digitalWrite(4, 0x0);
  digitalWrite(5, 0x0);
  digitalWrite(6, 0x0);
  digitalWrite(7, 0x0);
  digitalWrite(18, 0x0);

}

void loop() {

  // Read BH1750 @ 0X5C Value in Lux
  digitalWrite(4, 0x1);
  lux1 = lightMeter1.readLightLevel();
  digitalWrite(4, 0x0);

  digitalWrite(5, 0x1);
  lux2 = lightMeter1.readLightLevel();
  digitalWrite(5, 0x0);

  digitalWrite(6, 0x1);
  lux3 = lightMeter1.readLightLevel();
  digitalWrite(6, 0x0);

  digitalWrite(7, 0x1);
  lux4 = lightMeter1.readLightLevel();
  digitalWrite(7, 0x0);

  digitalWrite(18, 0x1);
  lux5 = lightMeter1.readLightLevel();
  digitalWrite(18, 0x0);


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
