#line 1 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/neo_pixel_control.h"
#ifndef NEO_PIXEL_CONTROL_H
#define NEO_PIXEL_CONTROL_H

#include <FastLED.h>
#include <constants.h>
#include <neo_pixel.h>

/*----------------------NEO_PIXEL---------------------*/

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100
#define BRIGHTNESS  64

class NeoPixelControl {
  private:
    NeoPixel *pixels;
    unsigned int num_leds;

  public:
    NeoPixelControl(NeoPixel* pixels, int num_leds);

    void turn_on(NeoPixel* pixels);
    void turn_off_all_lights();
    void blink(NeoPixel* pixels, int duration_ms, int delay_ms);
};

#endif // NEO_PIXEL_CONTROL_H