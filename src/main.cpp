#include <Arduino.h>
#include "fan.h"
#include "wifiFunctions.h"

Fan fan;

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nespSmartFan");
    Serial.print(F("\nStarting up "));
    randomSeed(millis());
    wifiSetup1();
    checkReset();
    delay(100);
    wifiSetup2();
    startServer();
}

void loop()
{
    wifiLoop();
    fan.run();
}
