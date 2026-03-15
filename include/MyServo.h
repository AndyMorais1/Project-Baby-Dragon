#ifndef MY_SERVO_H
#define MY_SERVO_H

#include <ESP32Servo.h>

class MyServo {
private:
    Servo _servo;
    uint8_t _pin;
    int _minPulse;
    int _maxPulse;
    int _currentAngle;

public:
    // Construtor: pino e pulsos min/max (padrão 544 e 2400 para a maioria dos SG90)
    MyServo(uint8_t pin, int minPulse = 544, int maxPulse = 2400);

    void begin();
    void setAngle(int angle);
    void detach(); // Para economizar energia e evitar trepidação (jitter)
    int getCurrentAngle();
};

#endif