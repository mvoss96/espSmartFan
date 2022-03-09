#pragma once
#include <Arduino.h>
#include "Servo.h"

#define MIN_TIME_MILLIS 15
#define PWM_IN_PIN 0
#define PWM_OUT_PIN 4
#define PWM_EN_PIN 5
#define SERVO_PIN 14
#define SERVO_TMIN 544
#define SERVO_TMAX 2400
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MIDDLE_ANGLE (MAX_ANGLE - MIN_ANGLE) / 2
#define SWEEP_ANGLE 90
#define SERVO_MVSPEED 15 //higher is slower

class Fan
{
private:
    bool on = false;
    bool sweepMode = false;
    bool sweepDir = false;
    bool breatheMode = false;
    bool breathePower = false;
    float bMod = 0.3;
    int enterBreathePowerChance = 10;
    int leaveBreathePowerChance = 10;
    uint8_t speed = 127;
    int angle = MIDDLE_ANGLE;
    int timer = 0;
    int targetAngle = MIDDLE_ANGLE;
    int rpm = 0;
    unsigned long count = 0;
    bool lastState = true;
    unsigned long startTime;
    unsigned long servoTime;
    unsigned long lastRead;
    Servo servo;
    

public:
    Fan();
    void setPower(bool p);
    bool getPower();
    void setSpeed(uint8_t s);
    uint8_t getSpeed();
    void setAngle(int a);
    int getAngle();
    int getTargetAngle();
    void setTimer(int t);
    int getTimer();
    void setSweep(bool p);
    bool getSweep();
    void setBreathe(bool p);
    bool getBreathe();
    int getRpm();
    void run();
};