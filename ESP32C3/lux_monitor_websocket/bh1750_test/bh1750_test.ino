#include <Wire.h>
#include <BH1750.h>

#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 8

BH1750 lightMeter;

void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.begin(9600);
  while (!Serial);

  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light intensity: ");
  Serial.print(lux);
  Serial.println(" lux");

  delay(1000);
}