#include <Arduino.h>

#define MIN_TIME_MILLIS 15
#define PWM_IN_PIN 0
#define PWM_OUT_PIN 4
#define PWM_EN_PIN 5
#define SERVO_PIN 14

volatile unsigned long count = 0;
unsigned long start_time;
unsigned long lastRead;
bool lastState;
int rpm;

ICACHE_RAM_ATTR void counter()
{
    count++;
}

void setup()
{
    Serial.begin(115200);
    pinMode(PWM_IN_PIN, INPUT);
    pinMode(PWM_OUT_PIN, OUTPUT);
    pinMode(PWM_EN_PIN, OUTPUT);
    pinMode(SERVO_PIN, OUTPUT);
    start_time = millis();
    lastRead = millis();
    analogWrite(PWM_OUT_PIN, 127);
    digitalWrite(PWM_EN_PIN, HIGH);
    lastState = HIGH;
}

void loop()
{
    if (lastState == HIGH && digitalRead(PWM_IN_PIN) == LOW)
    {
        count++;
        lastState = LOW;
        lastRead = millis();
    }
    else if (millis() - lastRead > MIN_TIME_MILLIS && lastState == LOW && digitalRead(PWM_IN_PIN) == HIGH)
    {
        lastState = HIGH;
    }

    while ((millis() - start_time) > 1000)
    {

        rpm = count * 60 / 2;
        count = 0;
        Serial.print(rpm);
        Serial.println(" rpm");
        start_time = millis();
    }
}
