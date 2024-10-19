#ifndef NEO_PIXEL_CONTROL_H
#define NEO_PIXEL_CONTROL_H

#include <FastLED.h>
#include <neo_pixel.h>

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

class NeoPixelControl {
  private:
    NeoPixel *pixels;
    unsigned int num_leds;
    CRGB *leds;
    unsigned int led_pin;

    void FillLEDsFromPaletteColors( uint8_t colorIndex);
    void ChangePalettePeriodically();
    void SetupTotallyRandomPalette();
    void SetupBlackAndWhiteStripedPalette();
    void SetupPurpleAndGreenPalette();

  public:
    NeoPixelControl(NeoPixel* pixels, CRGB *leds, int num_leds, int led_pin);

    void setup();
    void turn_on();
    void turn_off_all_lights();
    void blink(NeoPixel* pixels, int duration_ms, int delay_ms);
    void loop();


};

#endif // NEO_PIXEL_CONTROL_H