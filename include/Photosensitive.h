#ifndef PHOTOSENSITIVE_H
#define PHOTOSENSITIVE_H

#include <Arduino.h>

class Photosensitive {
private:
    uint8_t _pin;
    int _limiteEscuro; // Valor abaixo do qual consideramos "escuro"

public:
    Photosensitive(uint8_t pin, int limite = 1500);
    
    void begin();
    int lerBrilho();       // Retorna valor bruto (0-4095)
    int lerPercentual();   // Retorna 0% a 100%
    bool estaEscuro();     // Retorna true se estiver abaixo do limite
};

#endif