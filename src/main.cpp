#include <Arduino.h>
#include <BetonWatchy.h>
#include "settings.h"

BetonWatchy watchy(settings);

void setup() {
  watchy.init();
}

void loop() {
  // put your main code here, to run repeatedly:
}