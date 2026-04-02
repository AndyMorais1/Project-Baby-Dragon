#include "AudioManager.h"

AudioManager::AudioManager(HardwareSerial* serial, uint8_t rx, uint8_t tx) 
    : _serial(serial), _rx(rx), _tx(tx), _volume(20) {}

void AudioManager::begin() {
    _serial->begin(9600, SERIAL_8N1, _rx, _tx);
    delay(3000);
    Serial.println(F("AudioManager: A iniciar DFPlayer..."));
     
    delay(3000);
    if (!_player.begin(*_serial, false, false)) {
        Serial.println(F("Erro: Não foi possível conectar ao DFPlayer Mini."));
        Serial.println(F("1. Verifique as ligações RX/TX."));
        Serial.println(F("2. Verifique se o Cartão SD está inserido."));
        return;
    }

    _player.volume(3); 
    _player.EQ(DFPLAYER_EQ_NORMAL);

    Serial.println(F("AudioManager: DFPlayer pronto!"));
}

void AudioManager::startSound() {
    for (int i = 1; i <= 7; i++) {
        Serial.printf("Tocando faixa %d da pasta 02...\n", i);
        
        // Toca o ficheiro 'i' da pasta '2'
        _player.playFolder(2, i); 
        
        delay(30000); // Espera 30 segundos cada música
    }
}

void AudioManager::wetHands(){
    Serial.println("Playing 002.mp3 da Pasta 02");
    _player.playFolder(2, 2); // Pasta 2, Ficheiro 002.mp3
}

void AudioManager::soap(){
    Serial.println("Playing 003.mp3 da Pasta 02");
    _player.playFolder(2, 3); // Pasta 2, Ficheiro 003.mp3
}

void AudioManager::rubHands(){
    Serial.println("Playing 004.mp3 da Pasta 02");
    _player.playFolder(2, 4); // Pasta 2, Ficheiro 004.mp3
}

void AudioManager::dryHands(){
    Serial.println("Playing 005.mp3 da Pasta 02");
    _player.playFolder(2, 5); // Pasta 2, Ficheiro 005.mp3
}

void AudioManager::conclusion(){
    Serial.println("Playing 006.mp3 da Pasta 02");
    _player.playFolder(2, 6); // Pasta 2, Ficheiro 006.mp3
}

void AudioManager::notStop(){
    Serial.println("Playing 007.mp3 da Pasta 02");
    _player.playFolder(2, 7); // Pasta 2, Ficheiro 007.mp3
}

void AudioManager::setVolume(int vol) {
    _player.volume(vol); // Função nativa para ajustar volume (0 a 30)
}

void AudioManager::stop() {
    _player.stop(); // Função nativa para parar a reprodução
}