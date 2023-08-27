# 1 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
# 2 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2
# 3 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2
# 4 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2
# 5 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2
# 6 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2
# 7 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 2

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
    vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 100 ) * ( TickType_t ) ( 
# 66 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
              1000 
# 66 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
              ) ) / ( TickType_t ) 1000U ) ));
  }
}

void taskMotor1(void *parameter) {
  pinMode(motor1PWM, 0x03);
  pinMode(motor1DirFwd, 0x03);
  pinMode(motor1DirBwd, 0x03);

  // Set motor 1 PWM frequency and resolution
  ledcSetup(0, 20000, 8); // Channel 0, 20 kHz frequency, 8-bit resolution
  ledcAttachPin(motor1PWM, 0); // Attach motor 1 PWM to channel 0

  while (true) {
    // Motor 1 control logic here
    digitalWrite(motor1DirFwd, 0x1); // Set direction forward
    digitalWrite(motor1DirBwd, 0x0);
    ledcWrite(0, motorSpeed1); // Set PWM duty cycle to motorspeed1%

    vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 50 ) * ( TickType_t ) ( 
# 85 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
              1000 
# 85 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
              ) ) / ( TickType_t ) 1000U ) )); // Task delay
  }
}

void taskMotor2(void *parameter) {
  pinMode(motor2PWM, 0x03);
  pinMode(motor2DirFwd, 0x03);
  pinMode(motor2DirBwd, 0x03);

  // Set motor 2 PWM frequency and resolution
  ledcSetup(1, 20000, 8); // Channel 1, 20 kHz frequency, 8-bit resolution
  ledcAttachPin(motor2PWM, 1); // Attach motor 2 PWM to channel 1

  while (true) {
    // Motor 2 control logic here
    digitalWrite(motor2DirFwd, 0x1); // Set direction forward
    digitalWrite(motor2DirBwd, 0x0);
    ledcWrite(1, motorSpeed2); // Set PWM duty cycle to 50%

    vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 40 ) * ( TickType_t ) ( 
# 104 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
              1000 
# 104 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
              ) ) / ( TickType_t ) 1000U ) )); // Task delay
  }
}

void taskPrintPWM(void *parameter) {
  while (true) {

    Serial.print("Heart Beat>> \n");

    vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 1000 ) * ( TickType_t ) ( 
# 113 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
              1000 
# 113 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
              ) ) / ( TickType_t ) 1000U ) )); // Task delay
  }
}

void taskPushButton(void *parameter) {
  pinMode(pushButton, 0x05);
  pinMode(ledPin, 0x03);

  while (true) {
    // Check the push button state
    bool buttonState = digitalRead(pushButton);

    if (buttonState) {
      digitalWrite(ledPin, 0x1); // Turn on LED when button is pressed
    }
    else if (!buttonState) {
      digitalWrite(ledPin, 0x0); // Turn off LED when button is released
    }

    vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 50 ) * ( TickType_t ) ( 
# 132 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
              1000 
# 132 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
              ) ) / ( TickType_t ) 1000U ) )); // Task delay
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

  xTaskCreate(taskMQTT, "MQTTTask", 2048, 
# 152 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                         __null
# 152 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                             , 1, 
# 152 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                  __null
# 152 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                      );
  xTaskCreate(taskMotor1, "Motor1Task", 2048, 
# 153 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                             __null
# 153 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                 , 4, 
# 153 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                      __null
# 153 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                          ); // Highest priority
  xTaskCreate(taskMotor2, "Motor2Task", 2048, 
# 154 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                             __null
# 154 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                 , 3, 
# 154 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                      __null
# 154 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                          ); // 2nd highest priority
  xTaskCreate(taskPrintPWM, "PrintPWMTask", 2048, 
# 155 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                 __null
# 155 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                     , 2, 
# 155 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                          __null
# 155 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                              ); // 3rd highest priority
  xTaskCreate(taskPushButton, "PushButtonTask", 2048, 
# 156 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                     __null
# 156 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                         , 1, 
# 156 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino" 3 4
                                                              __null
# 156 "C:\\Users\\sherd\\Documents\\GitHub\\arduino-ide-workspace\\ESP32S3\\rtos_pwm_example_1\\rtos_pwm_example_1.ino"
                                                                  ); // Lowest priority
}

void loop() {
  // Nothing to do here, tasks are managed by FreeRTOS
}
