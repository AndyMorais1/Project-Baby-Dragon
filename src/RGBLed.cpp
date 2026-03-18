#include "RGBLed.h"

RGBLed::RGBLed(uint8_t pin, uint8_t numLeds) : 
_strip(numLeds, pin, NEO_GRB + NEO_KHZ800) {
    
}

void RGBLed::begin() {
    _strip.begin();
    _strip.show(); // Inicializa todos os LEDs como apagados
    _strip.setBrightness(100); // Brilho de 0 a 255
}

void RGBLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < _strip.numPixels(); i++) {
        _strip.setPixelColor(i, _strip.Color(r, g, b));
    }
    _strip.show();
}

void RGBLed::off() {
    _strip.clear();
    _strip.show();
}
