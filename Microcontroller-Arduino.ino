#include <ArduinoJson.h>

void setup()
{
    // Start serial connection with a set baudrate.
    Serial.begin(115200);

    // Waits for transmission of outgoing serial data to complete.
    Serial.flush();
}

void loop()
{
    // Dynamic memory of the Arduino Mega.
    const int memoryMax = 8192;

    // Leave some room for other variables.
    int memoryMaxUsable = memoryMax - 1024;

    if (Serial.available())
    {
        // Allocate the JsonDocument.
        DynamicJsonDocument doc(memoryMaxUsable);

        // Parse the JSON input via serial.
        DeserializationError error = deserializeJson(doc, Serial);

        // Log memory usage to output.
        LogMemoryUsage(doc, memoryMaxUsable);

        // Parse succeeded?
        if (error)
        {
            // Log deserialization error to output.
            LogDeserializationError(error);
            return;
        }
        else
        {
            // Extract pattern.
            JsonArray patternArray = doc["pattern"].as<JsonArray>();

            // Perform the vibration pattern.
            PerformVibrationPattern(patternArray);

            // End the line. DO NOT REMOVE. Python will wait forever if there is no end of the line.
            Serial.println("");
        }
    }
}

void PerformVibrationPattern(const JsonArray &patternArray)
{
    // Loop through vibration pattern.
    for (JsonObject vibration : patternArray)
    {
        int duration = vibration["time"];

        // Log duration to output.
        LogDuration(duration);

        // Set vibration speeds.
        SetVibration(vibration);

        // Leave the motors on for a set duration.
        delay(duration);
    }
}

void SetVibration(const JsonObject &vibration)
{
    JsonArray pinArray = vibration["pins"].as<JsonArray>();

    for (JsonObject pinItem : pinArray)
    {
        int pin = pinItem["pin"];
        int motorSpeed = pinItem["pwm"];

        // Set vibration motor speed.
        analogWrite(pin, motorSpeed);

        // Log speed set to output.
        LogSpeedSet(pin, motorSpeed);
    }
}

void LogDuration(const int duration)
{
    Serial.print("vibrating for ");
    Serial.print(duration);
    Serial.print("ms on pins ");
}


void LogSpeedSet(const int pin, const int motorSpeed)
{
    Serial.print(pin);
    Serial.print(":");
    Serial.print(motorSpeed);
    Serial.print(", ");
}

void LogMemoryUsage(const DynamicJsonDocument &doc, const int memoryMaxUsable)
{
    Serial.print("Memory used: ");
    int memoryUsed = doc.memoryUsage();
    Serial.print(memoryUsed);
    Serial.print("; ");

    Serial.print("Max. memory: ");
    Serial.print(memoryMaxUsable);
    Serial.print("; ");
}

void LogDeserializationError(const DeserializationError &error)
{
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
}
