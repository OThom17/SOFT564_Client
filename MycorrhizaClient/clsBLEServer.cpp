#include "clsBLEServer.h"


void clsBLEServer::Initialise()
{

    BLEDevice::init("MyESP32");
    BLEDevice::setMTU(512);
    BLEServer* pServer = BLEDevice::createServer();

    BLEService* pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic* pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("Hello World");
    pService->start();

    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}	