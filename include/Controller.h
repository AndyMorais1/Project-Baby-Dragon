#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "RGBLed.h" // Atualizado para a classe que usa FastLED
#include "MyServo.h"
#include "Ultrassonico.h"
#include "AudioManager.h"
//#include "Photosensitive.h" // COMENTADO: LDR temporariamente removido

class Controller {
private:
    RGBLed _leds;
    MyServo _servo;
    Ultrassonico _sensorUltrassonico; // Sensor 1: Dispensador
    Ultrassonico _sensorFase;         // Sensor 2: Substitui o LDR
    AudioManager _audio;
    // Photosensitive _sensorFotosensivel; // COMENTADO

    uint8_t _btnRestartPin = BUTTON_RESTART_PIN;
    
    const int _dispenserActivationDistance = 5;
    const int _phaseActivationDistance = 10; // Distância para "clicar" e mudar de fase
    const int _resetDistance = 8;
    const int _totalPhases = 7;

    int _currentPhase = 0;
    bool _handDetectedInPhaseSensor = false; // Substitui o _itsDark (trava para o novo sensor)
    bool _alreadySqueezed = false;  
    unsigned long _lastUpdate = 0;
    unsigned long _lastPhaseUpdate = 0; // Timer para o sensor de fase
    unsigned long _lastAudioStartTime = 0;

    void moveArm();
    void squeezeTheDispenser();
    void changePhase();
    void executePhaseFeedback(int phase);
    void handleRestart(); 
    void initSystem(); 
    bool isAudioPlaying(); 

public:
    Controller();
    void begin();
    void update();
};

#endif