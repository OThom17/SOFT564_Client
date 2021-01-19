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

    //xTaskCreatePinnedToCore(
    //    this->TaskSensorDispatcher
    //    , "PacketDispatcher"
    //    , 1024
    //    , this
    //    , 2
    //    , NULL
    //    , ARDUINO_RUNNING_CORE);

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






//void clsSensors::TaskBlink()  // This is a task.
//{
//
//    /*
//      Blink
//      Turns on an LED on for one second, then off for one second, repeatedly.
//
//      If you want to know what pin the on-board LED is connected to on your ESP32 model, check
//      the Technical Specs of your board.
//    */
//
//    // initialize digital LED_BUILTIN on pin 13 as an output.
//    pinMode(LED_BUILTIN, OUTPUT);
//
//    for (;;) // A Task shall never return or exit.
//    {
//        digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on (HIGH is the voltage level)
//        vTaskDelay(100);                    // one tick delay (15ms) in between reads for stability
//        digitalWrite(LED_BUILTIN, LOW);     // turn the LED off by making the voltage LOW
//        vTaskDelay(100);                    // one tick delay (15ms) in between reads for stability
//    }
//}
//
//void clsSensors::TaskCollectDHTData()  // This is a task.
//{
//    /*
//        Reads and outputs onto the terminal the Humidity, Temperature and Heat Index every 2 seconds
//        Code modified from example provided by www.makerguides.com.
//    */
//    const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;
//
//    DHT dht = DHT(DHTPIN, DHTTYPE);
//    // DHT Initialisation
//    dht.begin();
//
//    for (;;)
//    {
//        // Reading temperature or humidity takes about 250 milliseconds!
//        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//        vTaskDelay(xDelay);
//
//
//        // Read the humidity in %:
//        float h = dht.readHumidity();
//        // Read the temperature as Celsius:
//        float t = dht.readTemperature();
//        // Read the temperature as Fahrenheit:
//        float f = dht.readTemperature(true);
//
//        // Check if any reads failed and exit early (to try again):
//        if (isnan(h) || isnan(t) || isnan(f)) {
//            // Serial.println("Failed to read from DHT sensor!");
//        }
//
//        // Compute heat index in Fahrenheit (default):
//        float hif = dht.computeHeatIndex(f, h);
//        // Compute heat index in Celsius:
//        float hic = dht.computeHeatIndex(t, h, false);
//
//        TemperatureCelcius = t;
//        Humidity = h;
//
//        Serial.print("Humidity: ");
//        Serial.print(h);
//        Serial.print(" % ");
//        Serial.print("Temperature: ");
//        Serial.print(t);
//        Serial.print(" \xC2\xB0");
//        Serial.print("C | ");
//        Serial.print(f);
//        Serial.print(" \xC2\xB0");
//        Serial.print("F ");
//        Serial.print("Heat index: ");
//        Serial.print(hic);
//        Serial.print(" \xC2\xB0");
//        Serial.print("C | ");
//        Serial.print(hif);
//        Serial.print(" \xC2\xB0");
//        Serial.println("F");
//    }
//}
//
//void clsSensors::TaskGetUltraRange()  // This is a task.
//{
//    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
//
//    for (;;)
//    {
//        // Sleep for half a second
//        vTaskDelay(xDelay);
//
//        // Clears the trigPin condition
//        digitalWrite(trigPin, LOW);
//        delayMicroseconds(2);
//        // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
//        digitalWrite(trigPin, HIGH);
//        delayMicroseconds(10);
//        digitalWrite(trigPin, LOW);
//        // Reads the echoPin, returns the sound wave travel time in microseconds
//        float duration = pulseIn(echoPin, HIGH);
//        // Calculating the distance
//        float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
//        // Displays the distance on the Serial Monitor
//
//        Serial.print("Distance: ");
//        Serial.print(distance);
//        Serial.println(" cm");
//
//        Distance = distance;
//    }
//}
//
//void clsSensors::TaskSensorDispatcher()
//{
//    /*
//        Periodically Ouput Data to Console in a single string packet
//    */
//    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
//
//    for (;;)
//    {
//        if (pClientProps->bAutoSensorSample)
//        {
//            vTaskDelay(xDelay);
//            Serial.print("Sensor Packet -> Distance: ");
//            Serial.print(Distance); Serial.print("  cm");
//            Serial.print("  Temp: "); Serial.print(TemperatureCelcius); Serial.print("  C");
//            Serial.print("  Humidity: "); Serial.print(Humidity); Serial.print("  %");
//            Serial.println("    -> END");
//        }
//    }
//}
//
//void clsSensors::TaskLCDController()
//{
//    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
//    LiquidCrystal_I2C lcd(0x27, 16, 2);
//
//    // initialize LCD
//    lcd.init();
//    // turn on LCD backlight                      
//    lcd.backlight();
//
//
//    for (;;)
//    {
//        vTaskDelay(xDelay);
//
//        // set cursor to first column, first row
//        lcd.setCursor(0, 0);
//        // print message
//        lcd.print("Hello, World!");
//        vTaskDelay(xDelay);
//        // clears the display to print new message
//        lcd.clear();
//        // set cursor to first column, second row
//        lcd.setCursor(0, 1);
//        lcd.print("Hello, World!");
//        vTaskDelay(xDelay);
//        lcd.clear();
//    }
//}
