// #include "HardwareSerial.h"
#include "MAX30105.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>

// data
NimBLECharacteristic *serialChar;
MAX30105 particleSensor;

// put function declarations here:
NimBLECharacteristic *setupBluetooth(NimBLEServer *);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin(33, 32);

  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  // Configure sensor (Red + IR only)
  particleSensor.setup(60, 4, 2, 100, 411, 4096);
  Serial.println("MAX30102 setup done");

  // Bluetooth
  NimBLEDevice::init("Chronex");
  NimBLEServer *pServer = NimBLEDevice::createServer();
  serialChar = setupBluetooth(pServer);
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-1234-1234-123456789abc");
  pAdvertising->setName("Chronex");
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  serialChar->setValue((uint32_t)millis());
  serialChar->notify();
  particleSensor.
  delay(1000);
}

// put function definitions here:
NimBLECharacteristic *setupBluetooth(NimBLEServer *server) {
  NimBLEService *service =
      server->createService("12345678-1234-1234-1234-123456789abc");
  NimBLECharacteristic *pCharacteristic = service->createCharacteristic(
      "87654321-4321-4321-4321-cba987654321",
      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

  service->start();
  return pCharacteristic;
}