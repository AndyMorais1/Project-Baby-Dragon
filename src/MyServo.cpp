#include "MyServo.h"

MyServo::MyServo(uint8_t pin, int minPulse, int maxPulse) {
    _pin = pin;
    _minPulse = minPulse;
    _maxPulse = maxPulse;
    _currentAngle = 0;
}

void MyServo::begin() {
    // Permite que o timer do ESP32 aloque o PWM automaticamente
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    _servo.setPeriodHertz(50); // Frequência padrão de 50Hz para servos
    _servo.attach(_pin, _minPulse, _maxPulse);
    _servo.write(_currentAngle);
}

void MyServo::setAngle(int angle) {
    // Garante que o ângulo esteja entre 0 e 180
    angle = constrain(angle, 0, 180);
    _servo.write(angle);
    _currentAngle = angle;
}

void MyServo::detach() {
    _servo.detach();
}

int MyServo::getCurrentAngle() {
    return _currentAngle;
}
