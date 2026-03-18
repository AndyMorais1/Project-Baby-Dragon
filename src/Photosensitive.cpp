#include "Photosensitive.h"

Photosensitive::Photosensitive(uint8_t pin, int limite) : _pin(pin), _limiteEscuro(limite) {}

void Photosensitive::begin() {
    pinMode(_pin, INPUT);
}

int Photosensitive::lerBrilho() {
    return analogRead(_pin);
}

int Photosensitive::lerPercentual() {
    int valor = lerBrilho();
    // Invertemos o mapa se o sensor retornar valores menores para MAIS luz
    // Depende do modelo do módulo, geralmente: 0 (Luz) a 4095 (Escuro)
    return map(valor, 0, 4095, 100, 0); 
}

bool Photosensitive::estaEscuro() {
    return lerBrilho() > _limiteEscuro; // Ajuste conforme seu sensor
}