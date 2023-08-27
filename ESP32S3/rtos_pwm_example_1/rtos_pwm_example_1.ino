#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <PubSubClient.h> // Include your MQTT library here
#include <math.h>
#include <WiFi.h>

// MQTT configurations
const char *mqttServer = "291cdd7712d74aa1bbc793e426f8ddc2.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char *mqttUser = "sdkn.mqtt.hive02";
const char *mqttPassword = "H5@viSu3H!5HG_g";

WiFiClient espClient;
PubSubClient client(espClient);

// Global variables to hold motor speeds
int motorSpeed1 = 0;
int motorSpeed2 = 0;


// Motor 1 configurations
const int motor1PWM = 2;
const int motor1DirFwd = 3;
const int motor1DirBwd = 4;

// Motor 2 configurations
const int motor2PWM = 5;
const int motor2DirFwd = 6;
const int motor2DirBwd = 7;

// Pushbutton and LED configurations
const int pushButton = 8;
const int ledPin = 9;

// MQTT message callback
void mqttCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = String((char *)payload);

  if (topicStr == "motorspeed1") {
    motorSpeed1 = payloadStr.toInt();
  } else if (topicStr == "motorspeed2") {
    motorSpeed2 = payloadStr.toInt();
  }
}

void taskMQTT(void *parameter) {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);

  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      client.subscribe("motorspeed1");
      client.subscribe("motorspeed2");
    } else {
      delay(5000);
    }
  }

  while (true) {
    if (!client.connected()) {
      client.connect("ESP32Client", mqttUser, mqttPassword);
    }
    client.loop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void taskMotor1(void *parameter) {
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1DirFwd, OUTPUT);
  pinMode(motor1DirBwd, OUTPUT);

  // Set motor 1 PWM frequency and resolution
  ledcSetup(0, 20000, 8);  // Channel 0, 20 kHz frequency, 8-bit resolution
  ledcAttachPin(motor1PWM, 0);  // Attach motor 1 PWM to channel 0

  while (true) {
    // Motor 1 control logic here
    digitalWrite(motor1DirFwd, HIGH); // Set direction forward
    digitalWrite(motor1DirBwd, LOW);
    ledcWrite(0, motorSpeed1);  // Set PWM duty cycle to motorspeed1%

    vTaskDelay(pdMS_TO_TICKS(50));  // Task delay
  }
}

void taskMotor2(void *parameter) {
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor2DirFwd, OUTPUT);
  pinMode(motor2DirBwd, OUTPUT);

  // Set motor 2 PWM frequency and resolution
  ledcSetup(1, 20000, 8);  // Channel 1, 20 kHz frequency, 8-bit resolution
  ledcAttachPin(motor2PWM, 1);  // Attach motor 2 PWM to channel 1

  while (true) {
    // Motor 2 control logic here
    digitalWrite(motor2DirFwd, HIGH); // Set direction forward
    digitalWrite(motor2DirBwd, LOW);
    ledcWrite(1, motorSpeed2);  // Set PWM duty cycle to 50%

    vTaskDelay(pdMS_TO_TICKS(40));  // Task delay
  }
}

void taskPrintPWM(void *parameter) {
  while (true) {
    
    Serial.print("Heart Beat>> \n");

    vTaskDelay(pdMS_TO_TICKS(1000));  // Task delay
  }
}

void taskPushButton(void *parameter) {
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  while (true) {
    // Check the push button state
    bool buttonState = digitalRead(pushButton);

    if (buttonState) {
      digitalWrite(ledPin, HIGH);  // Turn on LED when button is pressed
    } 
    else if (!buttonState) {
      digitalWrite(ledPin, LOW);   // Turn off LED when button is released
    }

    vTaskDelay(pdMS_TO_TICKS(50));  // Task delay
  }
}

void setup() {
  Serial.begin(115200);

  const char* ssid = "Sherdil";
  const char* password = "03336830863BB";

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  xTaskCreate(taskMQTT, "MQTTTask", 2048, NULL, 1, NULL);
  xTaskCreate(taskMotor1, "Motor1Task", 2048, NULL, 4, NULL);  // Highest priority
  xTaskCreate(taskMotor2, "Motor2Task", 2048, NULL, 3, NULL);  // 2nd highest priority
  xTaskCreate(taskPrintPWM, "PrintPWMTask", 2048, NULL, 2, NULL);  // 3rd highest priority
  xTaskCreate(taskPushButton, "PushButtonTask", 2048, NULL, 1, NULL);  // Lowest priority
}

void loop() {
  // Nothing to do here, tasks are managed by FreeRTOS
}
