#include <math.h>
#include "fan.h"

double clamp(double x, double upper, double lower)
{
    return std::min(upper, std::max(x, lower));
}

Fan::Fan()
{
    pinMode(PWM_IN_PIN, INPUT);
    pinMode(PWM_OUT_PIN, OUTPUT);
    pinMode(PWM_EN_PIN, OUTPUT);
    pinMode(SERVO_PIN, OUTPUT);
    start_time = millis();
    lastRead = millis();
    run();
}

void Fan::setPower(bool p)
{
    on = p;
}

bool Fan::getPower()
{
    return on;
}

void Fan::setSpeed(uint8_t s)
{
    speed = s;
}

double Fan::getSpeed()
{
    return speed / 255.0;
}

void Fan::setAngle(int a)
{
    angle = clamp(angle, MIN_ANGLE, MAX_ANGLE);
}

int Fan::getAngle()
{
    return angle;
}

void Fan::setSweep(bool p)
{
    sweepMode = true;
}

bool Fan::getSweep()
{
    return sweepMode;
}

void Fan::setBreathe(bool p)
{
    breatheMode = true;
}

bool Fan::getBreathe()
{
    return breatheMode;
}

int Fan::getRpm()
{
    return rpm;
}

void Fan::run()
{
    analogWrite(PWM_OUT_PIN, speed);
    digitalWrite(PWM_EN_PIN, on);

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

    unsigned long deltaTime = millis() - start_time;
    if (deltaTime > 1000)
    {
        rpm = count * 60 * (1000.0 / deltaTime) / 2;
        count = 0;
        Serial.print(rpm);
        Serial.println(" rpm");
        start_time = millis();
    }
}
