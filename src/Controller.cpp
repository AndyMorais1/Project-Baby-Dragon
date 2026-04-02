#include "Controller.h"

Controller::Controller() : _leds(), // Inicializa a classe que usa FastLED
                           _servo(SERVO_PIN),
                           _sensorUltrassonico(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN),
                           _sensorFase(TRIG2_PIN, ECHO2_PIN), // Sensor que substitui o LDR
                           // _sensorFotosensivel(LDR_PIN), // COMENTADO: LDR temporariamente removido
                           _audio(&Serial2, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN)
{
}

void Controller::begin()
{
    Serial.begin(115200);

    // Inicialização dos componentes
    _leds.begin();
    _servo.begin();
    _sensorUltrassonico.begin();
    _sensorFase.begin(); // Inicializa o segundo sensor (fases)
    // _sensorFotosensivel.begin(); // COMENTADO
    _audio.begin();

    pinMode(DFPLAYER_BUSY_PIN, INPUT); // Monitor de áudio (Pino BUSY)
    pinMode(_btnRestartPin, INPUT_PULLUP);

    // Coloca o sistema no estado inicial (Fase 1 - Introdução)
    initSystem();

    Serial.println("Controller: Sistema Pronto. Usando 2 Sensores Ultrassónicos.");
}

void Controller::update()
{
    handleRestart(); // Verifica o botão físico de reinício

    // 1. Lógica de Mudança de Fase (Sensor 2)
    // Usamos um intervalo de 150ms para não sobrecarregar as leituras sonoras
    if (millis() - _lastPhaseUpdate > 150)
    {
        changePhase();
        _lastPhaseUpdate = millis();
    }

    // 2. Lógica de Repetição de Áudio (Lembrete)
    if (!isAudioPlaying())
    {
        if (millis() - _lastAudioStartTime > AUDIO_REPEAT_INTERVAL)
        {
            Serial.println("Sistema: Repetindo áudio de instrução...");
            executePhaseFeedback(_currentPhase);
        }
    }

    // 3. Lógica do Dispensador (Sensor 1 - SÓ funciona na Fase 3 / Índice 2)
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

    // Mão detectada no sensor do dispensador
    if (currentDist > 0.1 && currentDist <= _dispenserActivationDistance)
    {
        if (!_alreadySqueezed)
        {
            Serial.printf("Dispensador Ativado! Distância: %.2f cm\n", currentDist);

            // tocar som de aperto do dispensador
            moveArm(); // Movimento do servo (0 -> 50 -> 0)

            _alreadySqueezed = true;
        }
    }
    else if (currentDist > _resetDistance)
    {
        if (_alreadySqueezed)
        {
            Serial.println("Ação: Mão removida do dispenser.");
            _alreadySqueezed = false;
        }
    }
}

void Controller::changePhase()
{
    /* // LÓGICA ANTIGA DO LDR (COMENTADA)
    bool shadowDetected = _sensorFotosensivel.estaEscuro();
    if (shadowDetected && !_itsDark && !isAudioPlaying()) { ... }
    */

    // NOVA LÓGICA: Segundo Ultrassónico para mudar fases
    float distFase = _sensorFase.readDistance();

    // Verifica se a mão está próxima do sensor de comando (menos de 10cm)
    bool handPresent = (distFase > 0.1 && distFase <= _phaseActivationDistance);

    // Só muda de fase se o áudio NÃO estiver tocando (Trava de Áudio)
    if (handPresent && !_handDetectedInPhaseSensor && !isAudioPlaying())
    {
        _handDetectedInPhaseSensor = true; // Trava para evitar pulos múltiplos
        _currentPhase++;

        if (_currentPhase >= _totalPhases)
            _currentPhase = 0;

        Serial.printf("Sistema: Comando detectado! Mudando para Fase %d\n", _currentPhase + 1);
        executePhaseFeedback(_currentPhase);
    }
    else if (!handPresent && _handDetectedInPhaseSensor)
    {
        // Libera a trava quando a mão é afastada do sensor
        _handDetectedInPhaseSensor = false;
    }
}

void Controller::executePhaseFeedback(int phase)
{
    // 1. Dispara o áudio da fase (0001.mp3, 0002.mp3, etc.)
    _audio.playTrack(phase + 1);
    _lastAudioStartTime = millis();

    // 2. Define a cor fixa da fase no FastLED
    switch (phase)
    {
    case 0:
        _leds.setColor(255, 255, 255);
        break; // 1 - Branco (Introdução)
    case 1:
        _leds.setColor(0, 0, 255);
        break; // 2 - Azul
    case 2:
        _leds.setColor(128, 0, 128);
        break; // 3 - Roxo (Dispensador)
    case 3:
        _leds.setColor(0, 255, 0);
        break; // 4 - Verde
    case 4:
        _leds.setColor(255, 20, 147);
        break; // 5 - Rosa
    case 5:
        _leds.setColor(255, 255, 0);
        break; // 6 - Amarelo
    case 6:
        _leds.setColor(255, 255, 255);
        break; // 7 - Branco (Conclusão)
    }
}

bool Controller::isAudioPlaying()
{
    // O DFPlayer coloca o pino BUSY em LOW enquanto o áudio toca
    return (digitalRead(DFPLAYER_BUSY_PIN) == LOW);
}

void Controller::moveArm()
{
    Serial.println("Mecânica: Movendo braço do servo...");
    _servo.setAngle(50);
    delay(1000);
    _servo.setAngle(0);
    delay(1000);
}

void Controller::handleRestart()
{
    if (digitalRead(_btnRestartPin) == LOW)
    {
        delay(50); // Debounce
        if (digitalRead(_btnRestartPin) == LOW)
        {
            Serial.println("BOTÃO RESTART: Reiniciando sistema...");
            initSystem();
            while (digitalRead(_btnRestartPin) == LOW)
                ; // Espera soltar o botão
        }
    }
}

void Controller::initSystem()
{
    // 1. Reinicia Variáveis de Estado
    _currentPhase = 0;
    _handDetectedInPhaseSensor = false; // Reset da trava do sensor de fase
    // _itsDark = false; // COMENTADO (LDR)
    _alreadySqueezed = false;
    _lastUpdate = 0;
    _lastPhaseUpdate = 0;
    _lastAudioStartTime = millis();

    // 2. Reinicia Atuadores
    _servo.setAngle(0);
    _audio.stop();

    // 3. Inicia a Fase 1 (Introdução)
    executePhaseFeedback(_currentPhase);
    Serial.println("SISTEMA INICIALIZADO: Fase 1 Ativa.");
}