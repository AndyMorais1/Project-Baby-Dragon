#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "RGBLed.h"
#include "MyServo.h"
#include "Ultrassonico.h"
#include "AudioManager.h"
#include "Photosensitive.h"

class Controller {
private:
    // Instâncias dos componentes
    RGBLed _leds;
    MyServo _servo;
    Ultrassonico _sensorUltrassonico;
    AudioManager _audio;
    Photosensitive _sensorFotosensivel;
    uint8_t _btnRestartPin = BUTTON_RESTART_PIN;
    // Constantes de Configuração
    const int _dispenserActivationDistance = 5;
    const int _resetDistance = 8;
    const int _totalPhases = 7;

    // Variáveis de Estado (Flags)
    int _currentPhase = 0;
    bool _itsDark = false;           // Trava para o LDR
    bool _alreadySqueezed = false;  // Trava para o Servo
    unsigned long _lastUpdate = 0;
    bool _audioLock = false;     // Trava para o Áudio
    unsigned long _lastAudioStartTime = 0;


    // Métodos Privados de Lógica
    void moveArm();
    void squeezeTheDispenser();
    void changePhase();
    void executePhaseFeedback(int phase);
    void handleRestart(); // Verifica o botão
    void initSystem(); // Reseta variáveis e estados para o início ou reinício do sistema
    bool isAudioPlaying(); // Verifica se o áudio ainda está tocando para evitar sobreposição de sons

public:
    Controller();
    void begin();
    void update();
};

#endif