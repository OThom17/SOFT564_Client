/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    Modified further by Oliver Thompson for SOFT564
*/
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "clsQueue.h"


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class clsBLEServer
{
public: 
    void Initialise();
    void SetPacketQueue(clsQueue<String>* pQueue);

};

class MyCallbacks : public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic* pCharacteristic) 
    {
        std::string value = pCharacteristic->getValue();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
        if (value.length() > 0) {
            for (int i = 0; i < value.length(); i++)
                Serial.print(value[i]);
            pCharacteristic->setValue("Received");
        }
    }
};