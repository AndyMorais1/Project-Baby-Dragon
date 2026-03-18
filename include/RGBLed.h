#ifndef RGB_LED_H
#define RGB_LED_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class RGBLed {
private:
    Adafruit_NeoPixel _strip;

public:
    // Construtor: pinos, canais PWM (0-15) e se é anodo comum
    RGBLed(uint8_t pin, uint8_t numLeds);
    
    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void off();
};

#endif