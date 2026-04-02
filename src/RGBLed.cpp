#include "RGBLed.h"

RGBLed::RGBLed() {
    // O construtor FastLED é vazio pois a inicialização real ocorre no begin()
}

void RGBLed::begin() {
    // Inicializa a fita de LEDs com as configurações do seu Config.h
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    
    // Define o brilho máximo (SHINE = 55)
    FastLED.setBrightness(SHINE);
    
    // Garante que comecem apagadas
    off();
}

void RGBLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    // Preenche todo o array de LEDs com a cor desejada
    fill_solid(_leds, NUM_LEDS, CRGB(r, g, b));
    
    // Empurra a cor para a fita física
    FastLED.show();
}

void RGBLed::off() {
    // Limpa os dados de cor (preto)
    FastLED.clear();
    
    // Atualiza a fita para apagar fisicamente
    FastLED.show();
}