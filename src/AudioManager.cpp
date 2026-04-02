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


// void AudioManager::startSound() {
//     for (int i = 1; i <= 7; i++) {
//         Serial.printf("Tocando faixa %d da pasta 02...\n", i);
        
//         // Toca o ficheiro 'i' da pasta '2'
//         _player.playFolder(2, i); 
        
//         delay(30000); // Espera 30 segundos cada música
//     }
// }

void AudioManager::playTrack(int number){
    Serial.printf("Playing track %d da Pasta 02\n", number);
    _player.playFolder(2, number); // Pasta 2, Ficheiro 002.mp3
}

// void AudioManager::setVolume(int vol) {
//     _player.volume(vol); // Função nativa para ajustar volume (0 a 30)
// }

void AudioManager::stop() {
    _player.stop(); // Função nativa para parar a reprodução
}