#ifndef CONFIG_H
#define CONFIG_H

// --- LED RGB 1 ---
#define LED1_PIN_R 18
#define LED1_PIN_G 19
#define LED1_PIN_B 21
#define LED1_CH_R  0
#define LED1_CH_G  1
#define LED1_CH_B  2

// --- LED RGB 2 ---
#define LED2_PIN_R 25
#define LED2_PIN_G 26
#define LED2_PIN_B 27
#define LED2_CH_R  3
#define LED2_CH_G  4
#define LED2_CH_B  5

// --- SERVO MOTOR ---
//#define SERVO_PIN 13
#define SERVO_PIN 18

// --- SENSOR ULTRASSÓNICO (HC-SR04) ---
#define ULTRASONIC_TRIG_PIN 32
#define ULTRASONIC_ECHO_PIN 33

// --- DFPLAYER MINI (Comunicação Serial) ---
// No ESP32, é melhor usar a Serial2 para o DFPlayer
#define DFPLAYER_RX_PIN 16 // Conecta no TX do DFPlayer
#define DFPLAYER_TX_PIN 17 // Conecta no RX do DFPlayer (através de resistor 1k)

#endif