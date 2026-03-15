#include "RGBLed.h"

RGBLed::RGBLed(uint8_t r, uint8_t g, uint8_t b, uint8_t chR, uint8_t chG, uint8_t chB, bool isAnode) {
    _pinR = r; _pinG = g; _pinB = b;
    _chR = chR; _chG = chG; _chB = chB;
    _isAnode = isAnode;
}

void RGBLed::begin() {
    // Configura o PWM do ESP32: Frequência 5000Hz, Resolução 8 bits (0-255)
    ledcSetup(_chR, 5000, 8);
    ledcSetup(_chG, 5000, 8);
    ledcSetup(_chB, 5000, 8);

    // Associa os pinos aos canais
    ledcAttachPin(_pinR, _chR);
    ledcAttachPin(_pinG, _chG);
    ledcAttachPin(_pinB, _chB);
    
    off(); // Começa desligado
}

void RGBLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (_isAnode) {
        // Inverte o valor: 255 (cor total) vira 0 (pino LOW/GND)
        ledcWrite(_chR, 255 - r);
        ledcWrite(_chG, 255 - g);
        ledcWrite(_chB, 255 - b);
    } else {
        ledcWrite(_chR, r);
        ledcWrite(_chG, g);
        ledcWrite(_chB, b);
    }
}

void RGBLed::off() {
    setColor(0, 0, 0);
}
