#ifndef NEO_PIXEL_H
#define NEO_PIXEL_H

#include <FastLED.h>

struct NeoPixel {
    int led_index;
    int light_strength;
    CRGB rgb_color;
};

#endif // NEO_PIXEL_H