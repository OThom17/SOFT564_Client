#include "clsSensors.h"

void clsSensors::StartTasks()
{
    
}

void clsSensors::InitialiseSensors()
{
    // Ultrasonic Range Finder
    pinMode(trigPin, OUTPUT);       // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT);        // Sets the echoPin as an INPUT
    Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
    Serial.println("with Arduino UNO R3");
}

void clsSensors::Initialise()
{
    InitialiseSensors();

    // Now set up two tasks to run independently.
    xTaskCreatePinnedToCore(
        this->TaskBlink
        , "TaskBlink"              // A name just for humans
        , 1024                     // This stack size can be checked & adjusted by reading the Stack Highwater
        , this
        , 1                        // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        , NULL
        , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        this->TaskCollectDHTData
        , "DHTDataCollect"
        , 2048                     // Stack size
        , this
        , 3                        // Priority
        , NULL
        , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        this->TaskGetUltraRange
        , "GetUltraRange"
        , 1024                    // Stack Size
        , this
        , 2                       // Priority
        , NULL
        , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        this->TaskSensorDispatcher
        , "PacketDispatcher"
        , 1024
        , this
        , 2
        , NULL
        , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        this->TaskLCDController
        , "LCDController"
        , 2048
        , this
        , 1
        , NULL
        , ARDUINO_RUNNING_CORE);

    // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}
