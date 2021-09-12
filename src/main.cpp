#include <Arduino.h>
#include "fan.h"
#include "wifiFunctions.h"

Fan fan;

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nespSmartFan");
    Serial.print(F("\nStarting up "));
    wifiSetup1();
    checkReset();
    delay(100);
    wifiSetup2();
    startServer();
    // fan.setSpeed(50);
    // fan.setPower(true);
    // fan.setAngle(180);
}

void loop()
{
    wifiLoop();
    fan.run();
}
