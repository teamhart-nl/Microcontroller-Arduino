#include <ArduinoJson.h>

void setup()
{
    Serial.begin(9600);
    Serial.flush();
}

void loop()
{
    int memoryUsed;

    // Dynamic memory of the Arduino Nano.
    // const int memoryMax = 2048;

    // Dynamic memory of the Arduino Mega.
    const int memoryMax = 8192;

    // Leave some room for other variables.
    int memoryMaxUsable = memoryMax - 1024;

    if (Serial.available())
    {
        // Allocate the JsonDocument.
        DynamicJsonDocument doc(memoryMaxUsable);

        Serial.print("Memory used before deserialization: ");
        memoryUsed = doc.memoryUsage();
        Serial.print(memoryUsed);
        Serial.print("; ");

        // Parse the JSON input via serial.
        DeserializationError error = deserializeJson(doc, Serial);

        Serial.print("Memory used after deserialization: ");
        memoryUsed = doc.memoryUsage();
        Serial.print(memoryUsed);
        Serial.print("; ");

        Serial.print("Maximum amount of memory in json: ");
        Serial.print(memoryMaxUsable);
        Serial.print("; ");

        // Parse succeeded?
        if (error)
        {
            Serial.print("deserializeJson() returned ");
            Serial.println(error.c_str());
            return;
        }
        else
        {
            // Get a reference to the root array.
            JsonArray patternArray = doc["pattern"].as<JsonArray>();
            for (JsonObject vibration : patternArray)
            {
                // Get value from json document.
                int duration = vibration["time"];

                // Output prints.
                Serial.print("vibrating for ");
                Serial.print(duration);
                Serial.print("ms on pins ");

                // Get a reference to the root array.
                JsonArray pinArray = vibration["pins"].as<JsonArray>();
                for (JsonObject pinItem : pinArray)
                {
                    // Get values from json document.
                    int pin = pinItem["pin"];
                    int motorSpeed = pinItem["pwm"];

                    // Set vibration motor speed.
                    analogWrite(pin, motorSpeed);

                    // Output prints.
                    Serial.print(pin);
                    Serial.print(":");
                    Serial.print(motorSpeed);
                    Serial.print(", ");
                }
                delay(duration);
            }
            // Output print. DO NOT REMOVE. Python will wait forever if there is no output.
            Serial.println();
        }
    }
}
