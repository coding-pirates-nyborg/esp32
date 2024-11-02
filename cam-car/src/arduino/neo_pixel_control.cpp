#include "neo_pixel_control.h"
#include <FastLED.h>
#include "Arduino.h"
#define LED_TYPE NEOPIXEL
#define DATA_PIN 2 // NeoPixel pin
#define COLOR_ORDER RGB

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
    FastLED.setBrightness(0); /
    FastLED.showColor(CRGB::Black); // Turn of all lights
}

void NeoPixelControl::reset() {
    Serial.println("Resetting all lights");
    for(int i = 0; i < num_leds; i++){
        leds[i] = pixels[i].rgb_color;
    }
}

void NeoPixelControl::blink(int duration_ms, int count) {
    Serial.printf("Blinking LEDS for %d ms %d times\n", duration_ms, count);
    FastLED.setBrightness(pixels[0].brightness);    
    for(int i= 0; i < count; i++){    
      FastLED.show();
      delay(duration_ms);
      FastLED.showColor(CRGB::Black);
      delay(duration_ms);
    }
}

void NeoPixelControl::setup() {
    Serial.println("Setting up NeoPixels");
    delay(3000);
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, num_leds);
}
