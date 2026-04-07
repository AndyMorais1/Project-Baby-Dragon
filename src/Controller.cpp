#include "Controller.h"

Controller::Controller() : _leds(), 
                           _servo(SERVO_PIN),
                           _sensorUltrassonico(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN),
                           _sensorFase(TRIG2_PIN, ECHO2_PIN), 
                           _audio(&Serial2, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN)
{
}

void Controller::begin()
{
    Serial.begin(115200);

    _leds.begin();
    _servo.begin();
    _sensorUltrassonico.begin();
    _sensorFase.begin();
    _audio.begin();

    // MUDANÇA 1: INPUT_PULLUP garante que o pino não flutue quando o áudio para
    pinMode(DFPLAYER_BUSY_PIN, INPUT_PULLUP); 
    pinMode(_btnRestartPin, INPUT_PULLUP);

    initSystem();

    Serial.println("Controller: Sistema Pronto. Estabilização de áudio aplicada.");
}

void Controller::update()
{
    handleRestart(); 

    if (millis() - _lastPhaseUpdate > 150)
    {
        changePhase();
        _lastPhaseUpdate = millis();
    }

    static bool waitingToRepeat = false;
    static unsigned long waitStartTime = 0;

    float dist = _sensorFase.readDistance();
    bool handPresent = (dist > 0.1 && dist <= _phaseActivationDistance);

    // 🎵 Detecta fim do áudio (baseado no tempo real definido)
    bool audioFinished = (millis() >= _audioEndTime);

    if (audioFinished && !waitingToRepeat)
    {
        Serial.println("Áudio terminou.");
        waitingToRepeat = true;
        waitStartTime = millis();
    }

    // ⏱ Espera 20s REAIS
    if (waitingToRepeat)
    {
        if (!handPresent && (millis() - waitStartTime >= AUDIO_REPEAT_INTERVAL))
        {
            Serial.println("Repetindo áudio após 20s reais.");
            executePhaseFeedback(_currentPhase);
            waitingToRepeat = false;
        }
    }

    if (_currentPhase == 2)
    {
        if (millis() - _lastUpdate > 100)
        {
            squeezeTheDispenser();
            _lastUpdate = millis();
        }
    }
}

void Controller::squeezeTheDispenser()
{
    float currentDist = _sensorUltrassonico.readDistance();

    if (currentDist > 0.1 && currentDist <= _dispenserActivationDistance)
    {
        if (!_alreadySqueezed)
        {
            Serial.printf("Dispensador Ativado! Distância: %.2f cm\n", currentDist);
            moveArm(); 
            _alreadySqueezed = true;
        }
    }
    else if (currentDist > _resetDistance)
    {
        if (_alreadySqueezed)
        {
            _alreadySqueezed = false;
        }
    }
}

void Controller::changePhase()
{
    float distFase = _sensorFase.readDistance();
    bool handPresent = (distFase > 0.1 && distFase <= _phaseActivationDistance);

    // Trava de segurança: Só permite mudar de fase se o áudio atual 
    // já estiver a tocar há pelo menos 3 segundos (tempo para a pessoa ouvir o início)
    bool audioMinTimePassed = (millis() - _lastAudioStartTime > 3000);

    if (handPresent && !_handDetectedInPhaseSensor && audioMinTimePassed)
    {
        _handDetectedInPhaseSensor = true; 
        _currentPhase++;

        if (_currentPhase >= _totalPhases)
            _currentPhase = 0;

        Serial.printf("Sistema: Mudando para Fase %d\n", _currentPhase + 1);
        executePhaseFeedback(_currentPhase);
    }
    else if (!handPresent && _handDetectedInPhaseSensor)
    {
        _handDetectedInPhaseSensor = false;
    }
}

void Controller::executePhaseFeedback(int phase)
{
    _audio.playTrack(phase + 1);
    
    // Aumentamos para 500ms. Com o pino TX quebrado, o módulo 
    // demora mais para estabilizar o pino BUSY.
    delay(500); 

    _lastAudioStartTime = millis();
    _audioEndTime = _lastAudioStartTime + AUDIO_DURATION;

    switch (phase)
    {
        case 0: _leds.setColor(255, 255, 255); break;
        case 1: _leds.setColor(0, 0, 255);     break;
        case 2: _leds.setColor(128, 0, 128);   break;
        case 3: _leds.setColor(0, 255, 0);     break;
        case 4: _leds.setColor(255, 20, 147);  break;
        case 5: _leds.setColor(255, 255, 0);   break;
        case 6: _leds.setColor(255, 255, 255); break;
    }
}

bool Controller::isAudioPlaying()
{
    return (digitalRead(DFPLAYER_BUSY_PIN) == LOW);
}

void Controller::moveArm()
{
    _servo.setAngle(50);
    delay(1000);
    _servo.setAngle(0);
    delay(1000);
}

void Controller::handleRestart()
{
    if (digitalRead(_btnRestartPin) == LOW)
    {
        delay(50); 
        if (digitalRead(_btnRestartPin) == LOW)
        {
            initSystem();
            while (digitalRead(_btnRestartPin) == LOW); 
        }
    }
}

void Controller::initSystem()
{
    _currentPhase = 0;
    _handDetectedInPhaseSensor = false; 
    _alreadySqueezed = false;
    _lastUpdate = 0;
    _lastPhaseUpdate = 0;
    
    _servo.setAngle(0);
    _audio.stop();

    executePhaseFeedback(_currentPhase);
}