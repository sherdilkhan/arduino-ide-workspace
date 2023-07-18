#include <Wire.h>

#define CURRENT_ADDRESS 0x23
#define NEW_ADDRESS 0x42

#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 8

void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.begin(9600);
  while (!Serial);

  // Change I2C address
  bool success = changeI2CAddress(CURRENT_ADDRESS, NEW_ADDRESS);

  if (success) {
    Serial.println("I2C address change successful.");
  } else {
    Serial.println("I2C address change failed.");
  }
}

void loop() {
  // Do nothing in the loop
}

bool changeI2CAddress(byte currentAddress, byte newAddress) {
  Wire.beginTransmission(currentAddress);
  Wire.write(0x00);  // Address change command or sequence
  Wire.write(newAddress);  // New I2C address
  byte result = Wire.endTransmission();

  if (result == 0) {
    return true;  // Address change successful
  } else {
    return false;  // Address change failed
  }
}
