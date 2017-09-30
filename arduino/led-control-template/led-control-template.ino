// PROTOCOL
// | byte0 | byte1 | byte2 | byte3 | byte4 | byte5 |
// |  cmd  | start |  end  |   R   |   G   |   B   |
// Possible commands:
//    b – Set brightness (0-255)
//    f - fill
//

#include "FastLED.h"
#define NUM_LEDS 100
#define MAX_BRIGHTNESS 120

CRGB leds[NUM_LEDS];

enum STATES {
  kWaiting,
  kFilling,
};

typedef struct led_msg {
    long cmd; // char ie 'f' or 'b'
    union content {
        struct fill {
            short r;
            short g;
            short b;
            char  strip;
            int   start;
            int   end;
        } fill;
        short brightness;;
    } content;
} led_msg;
led_msg msg;

bool needsRedraw = false;
unsigned long interval = 25;
unsigned long prev = 0;
void redrawIfNeeded() {
  unsigned long current = millis(); 
  unsigned long timeSince = current - prev;
  if (needsRedraw || (timeSince > interval)) {
    FastLED.show();
    prev = current;
    needsRedraw = false;
  }
}

void setRange(short r, short g, short b, unsigned start, unsigned ends) {
  for (int i = start; i < ends && i < NUM_LEDS; i++) 
    leds[i].setRGB(r * MAX_BRIGHTNESS / 255,
                   g * MAX_BRIGHTNESS / 255,
                   b * MAX_BRIGHTNESS / 255);
   needsRedraw = true;
}

void setup() {
  FastLED.addLeds<NEOPIXEL, 8>(leds, NUM_LEDS);
  Serial.begin(4800);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  setRange(255, 0, 0, 0, NUM_LEDS / 3);
  setRange(0, 255, 0, NUM_LEDS / 3, NUM_LEDS * 2 / 3);
  setRange(0, 0, 255, NUM_LEDS * 2 / 3, NUM_LEDS);
}

void loop() {
  int blue = 0;

  if (Serial.available()) {
    msg.cmd = Serial.read();
    switch (msg.cmd) {
      case 'f':{
        byte buff[8];
        while (Serial.available() < 6); // Wait for the rest of the command
        Serial.readBytes(buff, 6);
        setRange(buff[0], buff[1], buff[2],
                buff[4], buff[5]);
        digitalWrite(13, HIGH);
      }
        break;
      case 'b':
        break;
      default:
        break;
    }
  }
  redrawIfNeeded();
}
