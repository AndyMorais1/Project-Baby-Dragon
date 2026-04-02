#ifndef RGB_LED_H
#define RGB_LED_H

#include <FastLED.h>
#include <Arduino.h>
#include "Config.h" // Para acessar NUM_LEDS, LED_PIN, etc.

class   RGBLed {
private:
    CRGB _leds[NUM_LEDS]; // Array que representa as suas 3 LEDs

public:
    RGBLed();
    
    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void off();
};

#endif