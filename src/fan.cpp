#include <math.h>
#include "fan.h"

double clamp(double x, double upper, double lower)
{
    if (x < lower)
        return lower;
    else if (x > upper)
        return upper;
    else
        return x;
}

Fan::Fan()
{
    pinMode(PWM_IN_PIN, INPUT);
    pinMode(PWM_OUT_PIN, OUTPUT);
    pinMode(PWM_EN_PIN, OUTPUT);
    pinMode(SERVO_PIN, OUTPUT);
    startTime = millis();
    servoTime = millis();
    lastRead = millis();
    servo.attach(SERVO_PIN, SERVO_TMIN, SERVO_TMAX);
    delay(100);
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

uint8_t Fan::getSpeed()
{
    return speed;
}

void Fan::setAngle(int a)
{
    sweepMode = false;
    targetAngle = clamp(a, MAX_ANGLE, MIN_ANGLE);
}

int Fan::getAngle()
{
    return angle;
}

void Fan::setTimer(int t)
{
    if (on)
    {
        timer = clamp(t, 60 * 60 * 12, 10);
    }
}

int Fan::getTimer()
{
    return timer;
}

void Fan::setSweep(bool p)
{
    sweepMode = p;
    if (sweepMode)
    {
        sweepDir = false;
        targetAngle = MIDDLE_ANGLE - SWEEP_ANGLE / 2;
    }
    else
    {
        targetAngle = angle;
    }
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
    //write power and speed to pins
    analogWrite(PWM_OUT_PIN, speed);
    digitalWrite(PWM_EN_PIN, on);

    //move servo towards target
    long unsigned int servospeed = (sweepMode) ? SERVO_MVSPEED * 5 : SERVO_MVSPEED;
    if (millis() - servoTime > servospeed)
    {
        if (targetAngle > angle)
        {
            servo.write(angle++);
        }
        else if (targetAngle < angle)
        {
            servo.write(angle--);
        }
        servoTime = millis();
    }

    if (angle == targetAngle)
    {
        //SweepMode
        if (sweepMode)
        {
            targetAngle = (sweepDir) ? MIDDLE_ANGLE - SWEEP_ANGLE / 2 : MIDDLE_ANGLE + SWEEP_ANGLE / 2;
            sweepDir = !sweepDir;
        }
    }

    //count pulses
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

    //once per second decrease timer and calculate rpm
    unsigned long deltaTime = millis() - startTime;
    if (deltaTime > 1000)
    {
        rpm = count * 60 * (1000.0 / deltaTime) / 2;
        count = 0;
        startTime = millis();
        if (timer > 0)
        {
            if (--timer == 0)
            {
                Serial.println("Timer zero");
                setPower(false);
                setSweep(false);
            }
        }
        Serial.print(rpm);
        Serial.println(" rpm");
    }
}
