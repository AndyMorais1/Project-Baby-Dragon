#include "Ultrassonico.h"

Ultrassonico::Ultrassonico(uint8_t trig, uint8_t echo) : _trig(trig), _echo(echo), _lastDistance(0.0) {}

void Ultrassonico::begin() {
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
    digitalWrite(_trig, LOW); // Garantir que o pino TRIG começa em LOW
}

float Ultrassonico::readDistance() {
    // Enviar um pulso de 10 microsegundos no pino TRIG
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);

    // Ler o tempo do pulso no pino ECHO
    unsigned long duration = pulseIn(_echo, HIGH);

    // Calcular a distância em centímetros (velocidade do som é ~343 m/s)
    _lastDistance = (duration / 2.0) * 0.0343; // Dividir por 2 para ida e volta

    return _lastDistance;
}