#include <Arduino.h>
#include <FreeRTOS.h>

const int ledPin = 2; // Built-in LED on ESP32-S3 Dev Kit is connected to GPIO 2

void blinkTask(void *pvParameters) {
  (void)pvParameters;

  pinMode(ledPin, OUTPUT);

  while (1) {
    digitalWrite(ledPin, HIGH); // Turn on the LED
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
    digitalWrite(ledPin, LOW); // Turn off the LED
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
  }
}


void loop() {
  // Nothing to do here in the loop() function
}

void setupRTOS() {
  // Create the LED blink task
  xTaskCreatePinnedToCore(blinkTask, "BlinkTask", 2048, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void setup() {
  // Setup serial port (if needed)
  Serial.begin(115200);

  // Setup FreeRTOS tasks
  setupRTOS();
}
