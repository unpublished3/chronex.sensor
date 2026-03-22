#pragma once
#include <NimBLEDevice.h>
#include <map>

class BluetoothService {
public:
  void begin(const char *name, const char *serviceUuid);

  NimBLECharacteristic *createCharacteristic(const char *uuid,
                                             uint32_t properties);

  void startAdvertising(const char* name, const char *serviceUuid);

  void sendUInt32(const char *uuid, uint32_t value);

  void sendHeartRate(const char *uuid, uint8_t hr);

private:
  NimBLEService *_service = nullptr;

  std::map<std::string, NimBLECharacteristic *> _characteristics;
};
