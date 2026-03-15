#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>

class RGBLed {
private:
    uint8_t _pinR, _pinG, _pinB;
    uint8_t _chR, _chG, _chB;
    bool _isAnode;

public:
    // Construtor: pinos, canais PWM (0-15) e se é anodo comum
    RGBLed(uint8_t r, uint8_t g, uint8_t b, uint8_t chR, uint8_t chG, uint8_t chB, bool isAnode = true);
    
    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void off();
};

#endif