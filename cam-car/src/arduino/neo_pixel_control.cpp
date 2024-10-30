#include "neo_pixel_control.h"
#include <FastLED.h>
#include "Arduino.h"
#define LED_TYPE WS2812
#define DATA_PIN 2
#define COLOR_ORDER RGB


// Constructor implementation
NeoPixelControl::NeoPixelControl(NeoPixel* pixels, CRGB *leds, int num_leds, int led_pin) : pixels(pixels), leds(leds), 
  num_leds(num_leds), led_pin(led_pin){}

void NeoPixelControl::turn_on() {
  Serial.printf("Turning on %d lights", num_leds);
  leds[0] = CRGB::DarkGoldenrod;
  FastLED.setBrightness(pixels[0].brightness);
  FastLED.show();  
}

void NeoPixelControl::turn_off() {
    Serial.println("Turning off all lights");    
    FastLED.setBrightness(0); // Set brightness to 0
    FastLED.showColor(CRGB::Black); // Show the color
}

void NeoPixelControl::blink(int duration_ms, int count) {
    Serial.printf("Blinking LEDS for %d ms %d times", duration_ms, count);
    FastLED.setBrightness(pixels[0].brightness);
    for(int i= 0; i < count; i++){
      leds[0] = CRGB::Orange;
      FastLED.show();
      delay(duration_ms);
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(duration_ms);
    }
}

void NeoPixelControl::setup() {
    delay(3000);
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, num_leds);
}
