#include "Controller.h"

Controller::Controller() : _leds(LED_PIN, NUM_LEDS),
                           _servo(SERVO_PIN),
                           _sensorUltrassonico(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN),
                           _sensorFotosensivel(LDR_PIN),
                           _audio(&Serial2, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN) {}

void Controller::begin()
{
    Serial.begin(115200);

    // Inicializa os drivers
    _leds.begin();
    _servo.begin();
    _sensorUltrassonico.begin();
    _sensorFotosensivel.begin();
    _audio.begin();

    pinMode(_btnRestartPin, INPUT_PULLUP);
    _audio.setVolume(20);

    // Garante que o sistema comece do zero
    initSystem();

    Serial.println("Controller: Sistema Pronto. LEDs Neopixel configurados.");
}

void Controller::update()
{
    handleRestart(); // Verifica o botão de reset
    changePhase();   // Verifica o LDR

    // Se o áudio parou de tocar, guardamos esse momento para a contagem de repetição
    if (!isAudioPlaying()) {
        // Se passou o tempo de intervalo desde o último início de áudio
        if (millis() - _lastAudioStartTime > AUDIO_REPEAT_INTERVAL) {
            Serial.println("Repetindo áudio da fase atual...");
            executePhaseFeedback(_currentPhase);
        }
    }

    // O dispensador SÓ funciona na Fase 2
    if (_currentPhase == 2)
    {
        if (millis() - _lastUpdate > 100)
        {
            squeezeTheDispenser();
            _lastUpdate = millis();
        }
    }
    else
    {
        // Se mudar de fase enquanto a mão ainda está lá, garante que apaga os LEDs
        if (_alreadySqueezed)
        {
            _leds.off(); // APAGAR LEDS ao sair da fase
            _alreadySqueezed = false;
        }
    }
}

void Controller::moveArm()
{
    Serial.println("Mecânica: Apertando...");
    _servo.setAngle(50);
    delay(1000);
    _servo.setAngle(0);
    delay(1000);
}

void Controller::squeezeTheDispenser()
{
    float currentDist = _sensorUltrassonico.readDistance();

    // Mão detectada (Ação)
    if (currentDist > 0.1 && currentDist <= _dispenserActivationDistance)
    {
        if (!_alreadySqueezed)
        {
            Serial.printf("Dispensador Ativado! Distância: %.2f cm\n", currentDist);

            // ACENDER LEDS EM ROXO (R:128, G:0, B:128)
            _leds.setColor(128, 0, 128);

            // TOCAR SOM DE ATIVAÇÃO
            // tocar som

            moveArm();

            _alreadySqueezed = true;
        }
    }
    // Mão removida (Reset e Apagar LEDs)
    else if (currentDist > _resetDistance)
    {
        if (_alreadySqueezed)
        {
            Serial.println("Ação: Mão removida. Apagando LEDs.");
            _leds.off(); // APAGAR LEDS
            _alreadySqueezed = false;
        }
    }
}

void Controller::changePhase()
{
    bool shadowDetected = _sensorFotosensivel.estaEscuro();

    if (shadowDetected && !_itsDark && !isAudioPlaying())
    {
        _itsDark = true;
        _currentPhase++;
        if (_currentPhase >= _totalPhases)
            _currentPhase = 0;

        Serial.print("Sistema: Mudando para Fase ");
        Serial.println(_currentPhase);

        executePhaseFeedback(_currentPhase);
    }
    else if (!shadowDetected && _itsDark)
    {
        _itsDark = false;
    }
}

void Controller::executePhaseFeedback(int phase)
{
    Serial.printf("Executando Áudio da Fase %d\n", phase);
    // Toca 0001.mp3, 0002.mp3, etc.
    _audio.playTrack(phase + 1);
    _lastAudioStartTime = millis(); // Marca o início do áudio para controle de repetição
    if (phase == 2)
    {
        Serial.println(">>> Dispensador Habilitado nesta fase.");
    }
}

void Controller::handleRestart()
{
    if (digitalRead(_btnRestartPin) == LOW)
    {
        delay(50);
        if (digitalRead(_btnRestartPin) == LOW)
        {
            Serial.println("BOTÃO RESTART: Reiniciando sistema...");
            initSystem();
            while (digitalRead(_btnRestartPin) == LOW)
                ;
        }
    }
}

void Controller::initSystem()
{
    // 1. Reset de Variáveis de Estado
    _currentPhase = 0;
    _itsDark = false;
    _alreadySqueezed = false;
    _lastUpdate = 0;

    // 2. Reset de Atuadores (Hardware)
    _leds.off();        // Apaga os LEDs Neopixel
    _servo.setAngle(0); // Volta o braço para a posição inicial
    _audio.stop();      // Para qualquer áudio que esteja tocando

    // 3. Feedback de Reinício (Toca Fase 0)
    executePhaseFeedback(_currentPhase);
    Serial.println("SISTEMA REINICIADO: Fase 0 Ativa.");
}

bool Controller::isAudioPlaying()
{
    // O pino BUSY é LOW (0) quando está tocando e HIGH (1) quando parado
    return (digitalRead(DFPLAYER_BUSY_PIN) == LOW);
}