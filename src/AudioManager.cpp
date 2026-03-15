#include "AudioManager.h"

AudioManager::AudioManager(HardwareSerial* serial, uint8_t rx, uint8_t tx) 
    : _serial(serial), _rx(rx), _tx(tx), _volume(20) {}

void AudioManager::begin() {
    // 1. Inicia a porta Serial2 com os pinos definidos no Config.h
    // O baud rate padrão do DFPlayer é 9600
    _serial->begin(9600, SERIAL_8N1, _rx, _tx);

    Serial.println(F("AudioManager: A iniciar DFPlayer..."));

    // 2. Tenta comunicar com o módulo
    // O timeout de 500ms é uma margem segura para o módulo responder
    if (!_player.begin(*_serial, /*isACK = */true, /*doReset = */true)) {
        Serial.println(F("Erro: Não foi possível conectar ao DFPlayer Mini."));
        Serial.println(F("1. Verifique as ligações RX/TX."));
        Serial.println(F("2. Verifique se o Cartão SD está inserido."));
        return;
    }

    // 3. Configuração inicial após sucesso
    _player.setTimeOut(500); // Timeout para comandos
    _player.volume(_volume); // Define o volume inicial (0-30)
    _player.EQ(DFPLAYER_EQ_NORMAL);
    _player.outputDevice(DFPLAYER_DEVICE_SD);

    Serial.println(F("AudioManager: DFPlayer pronto!"));
}

void AudioManager::playAlert() {
    // Espaço para tocar track de alerta
}

void AudioManager::setVolume(int vol) {
    // Espaço para ajustar volume do módulo
}

void AudioManager::nextVolume() {
    // Espaço para lógica de ciclo de volume
}

void AudioManager::stop() {
    // Espaço para parar áudio
}