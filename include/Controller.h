#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "RGBLed.h"
#include "MyServo.h"
#include "Ultrassonico.h"
#include "AudioManager.h"

class Controller {
private:
    // Instâncias das suas classes
    RGBLed _led1;
    RGBLed _led2;
    MyServo _servo;
    Ultrassonico _sensor;
    AudioManager _audio;

    unsigned long _lastUpdate;
    bool _alertaAtivo;
    void moveArm();

public:
    Controller();
    void begin();
    void update();
};

#endif