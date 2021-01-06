#include "clsBLEServer.h"


void clsBLEServer::Initialise()
{

    //Serial.begin(115200);

    //Serial.println("1- Download and install an BLE scanner app in your phone");
    //Serial.println("2- Scan for BLE devices in the app");
    //Serial.println("3- Connect to MyESP32");
    //Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
    //Serial.println("5- See the magic =)");

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