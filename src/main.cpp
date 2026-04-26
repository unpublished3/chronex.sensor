#include "DFRobot_MAX30102.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "bluetooth/BluetoothService.h"
#include "bluetooth/BluetoothUuids.h"
#include "esp32-hal.h"
#include "heartrate/HeartRateService.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>
#include <cstdint>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// data
BluetoothService ble;
HeartRateService heartRateService;
Adafruit_MPU6050 mpu;

// put function declarations here:

// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);

  delay(2000);
  // Serial.println("Starting...");
  Wire.begin(33, 32);
  Wire.setClock(100000);

  Wire.beginTransmission(0x68);
  Wire.write(0x75); // WHO_AM_I register
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 1);
  if (Wire.available()) {
    byte id = Wire.read();
    Serial.print("Device ID reported: 0x");
    Serial.println(id, HEX);
  }

  // Serial.println("Scanning I2C...");
  // for (byte addr = 1; addr < 127; addr++) {
  //   Wire.beginTransmission(addr);
  //   byte error = Wire.endTransmission();
  //   if (error == 0) {
  //     Serial.print("Device found at 0x");
  //     Serial.println(addr, HEX);
  //   }
  // }
  // Serial.println("Scan done");

  // Hear Rate Service
  if (!heartRateService.begin()) {
    Serial.println("Sensor not found!");
    ;
    while (1)
      ;
  }

  Serial.println("Adafruit MPU6050 test!");

  // // // Accelerometer
  if (!mpu.begin(0x68, &Wire, 0x70)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // // Ble
  ble.begin("Chronex", BluetoothUuids::SERVICE);
  ble.createCharacteristic(BluetoothUuids::HEART,
                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Range: ");
  Serial.print(mpu.getAccelerometerRange());
  Serial.print(" | Accel X: ");
  Serial.println(a.acceleration.x);

  delay(200);
  // if (heartRateService.update()) {
  //   int32_t hr = heartRateService.getHr();
  //   Serial.println(hr);
  //   ble.sendHeartRate(BluetoothUuids::HEART, hr);
  // }
}

// put function definitions here:
