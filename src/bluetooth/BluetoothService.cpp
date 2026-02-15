#include "BluetoothService.h"

void BluetoothService::begin(const char* name, const char* serviceUuid) {
    NimBLEDevice::init(name);

    NimBLEServer* server = NimBLEDevice::createServer();
    _service = server->createService(serviceUuid);

    _service->start();

    NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
    advertising->addServiceUUID(serviceUuid);
    advertising->start();
}

NimBLECharacteristic* BluetoothService::createCharacteristic(
    const char* uuid,
    uint32_t properties
) {
    NimBLECharacteristic* ch =
        _service->createCharacteristic(uuid, properties);

    _characteristics[uuid] = ch;

    return ch;
}

void BluetoothService::sendUInt32(const char* uuid, uint32_t value) {
    auto it = _characteristics.find(uuid);

    if (it != _characteristics.end()) {
        NimBLECharacteristic* ch = it->second;
        ch->setValue((uint8_t*)&value, sizeof(value));
        ch->notify();
    }
}
