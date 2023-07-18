#include <Wire.h>

#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 8

void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.begin(9600);
  while (!Serial);

  Serial.println("Scanning I2C devices...");
  scanI2C();
}

void loop() {
  // Do nothing in the loop
}

void scanI2C() {
  byte error, address;
  int deviceCount = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found device at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println();

      deviceCount++;
    }
  }

  if (deviceCount == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.println("Scan complete.");
  }
}
