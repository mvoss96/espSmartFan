#include <Arduino.h>
#include "fan.h"

Fan fan;

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nespSmartFan");
    delay(5000);
    fan.setSpeed(255);
    fan.setPower(true);

}

void loop()
{
    fan.run();
}
