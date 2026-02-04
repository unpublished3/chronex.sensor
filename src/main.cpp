#include <Arduino.h>
#include <NimBLEDevice.h>

// data
NimBLECharacteristic *serialChar;

// put function declarations here:
NimBLECharacteristic* setupBluetooth(NimBLEServer*);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  NimBLEDevice::init("Chronex");

  NimBLEServer *pServer = NimBLEDevice::createServer();
  serialChar =  setupBluetooth(pServer);
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-1234-1234-123456789abc"); 
  pAdvertising->setName("Chronex");
  pAdvertising->start(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  serialChar->setValue((uint32_t)millis());
  serialChar->notify();
  delay(1000);
}

// put function definitions here:
NimBLECharacteristic* setupBluetooth(NimBLEServer* server) {
  NimBLEService *service = server->createService("12345678-1234-1234-1234-123456789abc");
  NimBLECharacteristic *pCharacteristic = service->createCharacteristic("87654321-4321-4321-4321-cba987654321", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

  service->start();
  return pCharacteristic;
}