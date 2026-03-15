#include <Arduino.h>
#include "MyServo.h"
#include "Config.h"
#include "Controller.h"

Controller controller;

void setup() {
  // controle inicializa os componentes e configurações
  controller.begin();
}

void loop() {
  controller.update();
}