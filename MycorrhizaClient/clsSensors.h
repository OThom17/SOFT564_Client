#ifndef _CLSSENSORS_H
#define _CLSSENSORS_H

#if CONFIG_FREERTOS_UNICORE
#include <Adafruit_SensorLab.h>
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

// Set DHT pin:
#define DHTPIN 4
// Set DHT type, uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define echoPin 16              // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 17              //attach pin D3 Arduino to pin Trig of HC-SR04
#define LED_BUILTIN 2

class clsSensors
{
public:
    clsSensors() 
    {
        
    };
    ~clsSensors() {};

	void Initialise();
	void StartTasks();

private:
    // Initialize DHT sensor for normal 16mhz Arduino:

    float TemperatureCelcius = -1.0; // Outgoing Variable
    float Humidity = -1.0;           // Outgoing Variable    
    float HeatIndex = -1.0;          // Outgoing Variable
    float Distance = -1.0;          // Outgoing Variable

	void InitialiseSensors();

    static void TaskBlink(void* pvParameters)  // This is a task.
    {
        (void)pvParameters;

        /*
          Blink
          Turns on an LED on for one second, then off for one second, repeatedly.

          If you want to know what pin the on-board LED is connected to on your ESP32 model, check
          the Technical Specs of your board.
        */

        // initialize digital LED_BUILTIN on pin 13 as an output.
        pinMode(LED_BUILTIN, OUTPUT);

        for (;;) // A Task shall never return or exit.
        {
            digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on (HIGH is the voltage level)
            vTaskDelay(100);                    // one tick delay (15ms) in between reads for stability
            digitalWrite(LED_BUILTIN, LOW);     // turn the LED off by making the voltage LOW
            vTaskDelay(100);                    // one tick delay (15ms) in between reads for stability
        }
    }

    static void TaskCollectDHTData(void* pvParameters)  // This is a task.
    {
        (void)pvParameters;
        clsSensors o = *((clsSensors*)pvParameters);

        /*
            Reads and outputs onto the terminal the Humidity, Temperature and Heat Index every 2 seconds
            Code modified from example provided by www.makerguides.com.
        */
        const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;

        DHT dht = DHT(DHTPIN, DHTTYPE);
        // DHT Initialisation
        dht.begin();

        for (;;)
        {
            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            vTaskDelay(xDelay);


            // Read the humidity in %:
            float h = dht.readHumidity();
            // Read the temperature as Celsius:
            float t = dht.readTemperature();
            // Read the temperature as Fahrenheit:
            float f = dht.readTemperature(true);

            // Check if any reads failed and exit early (to try again):
            if (isnan(h) || isnan(t) || isnan(f)) {
                // Serial.println("Failed to read from DHT sensor!");
            }

            // Compute heat index in Fahrenheit (default):
            float hif = dht.computeHeatIndex(f, h);
            // Compute heat index in Celsius:
            float hic = dht.computeHeatIndex(t, h, false);

            o.TemperatureCelcius = t;
            o.Humidity = h;

            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print(" % ");
            Serial.print("Temperature: ");
            Serial.print(t);
            Serial.print(" \xC2\xB0");
            Serial.print("C | ");
            Serial.print(f);
            Serial.print(" \xC2\xB0");
            Serial.print("F ");
            Serial.print("Heat index: ");
            Serial.print(hic);
            Serial.print(" \xC2\xB0");
            Serial.print("C | ");
            Serial.print(hif);
            Serial.print(" \xC2\xB0");
            Serial.println("F");
        }
    }

    static void TaskGetUltraRange(void* pvParameters)  // This is a task.
    {
        (void)pvParameters;
        clsSensors o = *((clsSensors*)pvParameters);
        const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

        for (;;)
        {
            // Sleep for half a second
            vTaskDelay(xDelay);

            // Clears the trigPin condition
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            // Reads the echoPin, returns the sound wave travel time in microseconds
            float duration = pulseIn(echoPin, HIGH);
            // Calculating the distance
            float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
            // Displays the distance on the Serial Monitor

            //Serial.print("Distance: ");
            //Serial.print(distance);
            //Serial.println(" cm");

            o.Distance = distance;
        }
    }

    static void TaskSensorDispatcher(void* pvParameters)
    {
        /*
            Periodically Ouput Data to Console in a single string packet
        */
        (void)pvParameters;
         clsSensors o = *((clsSensors*)pvParameters);
         const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

        for (;;)
        {
            vTaskDelay(xDelay);
            Serial.print("Sensor Packet -> Distance: ");
            Serial.print(o.Distance); Serial.print("  cm");
            Serial.print("  Temp: "); Serial.print(o.TemperatureCelcius); Serial.print("  C");
            Serial.print("  Humidity: "); Serial.print(o.Humidity); Serial.print("  %");
            Serial.println("    -> END");
        }
    }

    static void TaskLCDController(void* pvParameters)
    {
        (void)pvParameters;
        clsSensors o = *((clsSensors*)pvParameters);
        const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
        LiquidCrystal_I2C lcd(0x27, 16, 2);

        // initialize LCD
        lcd.init();
        // turn on LCD backlight                      
        lcd.backlight();

        
        for (;;)
        {
            vTaskDelay(xDelay);

            // set cursor to first column, first row
            lcd.setCursor(0, 0);
            // print message
            lcd.print("Hello, World!");
            vTaskDelay(xDelay);
            // clears the display to print new message
            lcd.clear();
            // set cursor to first column, second row
            lcd.setCursor(0, 1);
            lcd.print("Hello, World!");
            vTaskDelay(xDelay);
            lcd.clear();
        }
    }

};

#endif