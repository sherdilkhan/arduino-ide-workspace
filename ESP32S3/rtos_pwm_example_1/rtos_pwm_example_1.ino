#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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
    ledcWrite(0, 255);  // Set PWM duty cycle to 50%

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
    ledcWrite(1, 255);  // Set PWM duty cycle to 50%

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
  pinMode(pushButton, INPUT);
  pinMode(ledPin, OUTPUT);
  bool isButtonPressed = false;

  while (true) {
    // Check the push button state
    bool buttonState = digitalRead(pushButton);

    if (buttonState && !isButtonPressed) {
      isButtonPressed = true;
      digitalWrite(ledPin, HIGH);  // Turn on LED when button is pressed
    } else if (!buttonState && isButtonPressed) {
      isButtonPressed = false;
      digitalWrite(ledPin, LOW);   // Turn off LED when button is released
    }

    vTaskDelay(pdMS_TO_TICKS(50));  // Task delay
  }
}



void setup() {
  Serial.begin(115200);
  xTaskCreate(taskMotor1, "Motor1Task", 2048, NULL, 4, NULL);  // Highest priority
  xTaskCreate(taskMotor2, "Motor2Task", 2048, NULL, 3, NULL);  // 2nd highest priority
  xTaskCreate(taskPrintPWM, "PrintPWMTask", 2048, NULL, 2, NULL);  // 3rd highest priority
  xTaskCreate(taskPushButton, "PushButtonTask", 2048, NULL, 1, NULL);  // Lowest priority
}

void loop() {
  // Nothing to do here, tasks are managed by FreeRTOS
}
