#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

class AudioManager {
private:
    DFRobotDFPlayerMini _player;
    HardwareSerial* _serial;
    uint8_t _rx, _tx;
    int _volume;

public:
    // Recebe o ponteiro da Serial2 e os pinos 16/17 do teu diagrama
    AudioManager(HardwareSerial* serial, uint8_t rx, uint8_t tx);

    void begin();
    void setVolume(int vol);
    void nextVolume(); // Alterna entre níveis de volume pré-definidos
    void stop();
    void playTrack(int trackNumber); // Toca uma faixa específica do SD
};

#endif