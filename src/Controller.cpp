// inicialmente o servo vai começar no angulo 0
// no grau o, o mecanismontem tem 12.6cm
// o tamanho max que ele atinge com angulo de 180 são 15cm
// o tamanho do mecanismo esticado no max é 15cm
// o objetivo é que ele saia de 0 a 90 graus para apertar
// e depois volte para 0 para soltar
// o mecanismo com 90 graus, tem 12.8cm
//o dispensador tem 15cm
//12.8 + 15 = 27.8cm
//27.8cm é o tamanho total do mecanismo
//com o servo em 90 graus, mais 15cm do dispensador
// então a distancia entre para que haja espaco de apertar e voltar
// deve ser de 2 cm
// 90 graus o braço percorrera 4cm ate a cabeca
// com distancia de 2cm entre eles
//ele deve percorrer 2cm para apertar e depois voltar 2cm para soltar
// e depois mais 2cm para voltar para a posição inicial

#include "Controller.h"
#include <Arduino.h>

Controller::Controller() : 
    _led1(LED1_PIN_R, LED1_PIN_G, LED1_PIN_B, LED1_CH_R, LED1_CH_G, LED1_CH_B),
    _led2(LED2_PIN_R, LED2_PIN_G, LED2_PIN_B, LED2_CH_R, LED2_CH_G, LED2_CH_B),
    _servo(SERVO_PIN),
    _sensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN),
    _audio(&Serial2, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN),
    _lastUpdate(0),
    _alertaAtivo(false) {}

void Controller::begin() {
    Serial.begin(115200);
    
    // Inicialização coordenada de todos os drivers
    _led1.begin();
    _led2.begin();
    _servo.begin();
    _sensor.begin();
    _audio.begin();

    _audio.setVolume(20);
    Serial.println("Controller: Sistema Inicializado.");
}

void Controller::update() {
    moveArm();
}

void Controller::moveArm() {
    _servo.setAngle(90);
    delay(1000);
    _servo.setAngle(0);
    delay(1000); // é necessario este delay para o servo voltar para a posição inicial antes de tentar apertar novamente
}