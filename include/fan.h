#pragma once
#include <Arduino.h>

#define MIN_TIME_MILLIS 15
#define PWM_IN_PIN 0
#define PWM_OUT_PIN 4
#define PWM_EN_PIN 5
#define SERVO_PIN 14
#define MIN_ANGLE 0
#define MAX_ANGLE 180

class Fan
{
private:
    bool on = false;
    bool sweepMode = false;
    bool breatheMode = false;
    uint8_t speed = 127;
    int angle = 0;
    int rpm = 0;
    unsigned long count = 0;
    bool lastState = true;
    unsigned long start_time;
    unsigned long lastRead;
    

public:
    Fan();
    void setPower(bool p);
    bool getPower();
    void setSpeed(uint8_t s);
    double getSpeed();
    void setAngle(int a);
    int getAngle();
    void setSweep(bool p);
    bool getSweep();
    void setBreathe(bool p);
    bool getBreathe();
    int getRpm();
    void run();
};