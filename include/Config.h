#ifndef CONFIG_H
#define CONFIG_H

// --- LED NEOPixel (WS2812B) ---
#define LED_PIN        13   // Apenas um pino para todas as LEDs
#define NUM_LEDS       3    // Quantidade de LEDs na sua fita

// --- SERVO MOTOR ---
#define SERVO_PIN      18

// --- SENSOR ULTRASSÓNICO ---
#define ULTRASONIC_TRIG_PIN 32
#define ULTRASONIC_ECHO_PIN 33

// --- DFPLAYER MINI ---
#define DFPLAYER_RX_PIN 16 
#define DFPLAYER_TX_PIN 17 
#define DFPLAYER_BUSY_PIN 35  // Pino que monitora se o áudio terminou
#define AUDIO_REPEAT_INTERVAL 20000 // Tempo para repetir o áudio (20 segundos)

// --- SENSOR FOTOSENSÍVEL (LDR) ---
#define LDR_PIN 34  

// --- BOTÃO DE REINÍCIO ---
#define BUTTON_RESTART_PIN 26

#endif