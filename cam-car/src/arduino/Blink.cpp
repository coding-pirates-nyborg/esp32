#include "Blink.h"
#include <Arduino.h>


Blink::Blink(NeoPixelControl *neoPixelControl, CRGB *leds, ) : neoPixelControl(neoPixelControl) {}

void Blink::execute() const
{
  Serial.println("Blink");
  neoPixelControl.blink(100, 5);
}