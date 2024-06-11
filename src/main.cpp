#include <Arduino.h>
#include <FastLED.h>
#include <NewPing.h>

// LED configuration
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 121
#define BRIGHTNESS 120
#define DENSITY 80

#define LED_PIN1 23
#define LED_PIN2 22
#define LED_PIN3 21
#define LED_PIN4 19

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];

// Sensor configuration
#define MAX_DISTANCE 400

#define TRIG_PIN1 33
#define ECHO_PIN1 32

#define TRIG_PIN2 26
#define ECHO_PIN2 25

#define TRIG_PIN3 14
#define ECHO_PIN3 27

#define TRIG_PIN4 13
#define ECHO_PIN4 12

NewPing sonar1(TRIG_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIG_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIG_PIN4, ECHO_PIN4, MAX_DISTANCE);

const CRGB lightcolor(8, 7, 1);

// Function declarations
void handleSensor(NewPing &sonar, CRGB *leds);
void softtwinkles(CRGB *leds);
void moveWhiteLeds(CRGB *leds, unsigned int distance);

void setup()
{
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(leds4, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  handleSensor(sonar1, leds1);
  handleSensor(sonar2, leds2);
  handleSensor(sonar3, leds3);
  handleSensor(sonar4, leds4);

  FastLED.show();
  delay(100);
}

void handleSensor(NewPing &sonar, CRGB *leds)
{
  unsigned int distance = sonar.ping_cm();

  if (distance == 0)
  {
    distance = MAX_DISTANCE;
  }

  if (distance > 100)
  {
    softtwinkles(leds);
  }
  else if (distance >= 10 && distance <= 100)
  {
    moveWhiteLeds(leds, distance);
  }
}

void softtwinkles(CRGB *leds)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (!leds[i])
      continue;
    if (leds[i].r & 1)
    {
      leds[i] -= lightcolor;
    }
    else
    {
      leds[i] += lightcolor;
    }
  }

  if (random8() < DENSITY)
  {
    int j = random16(NUM_LEDS);
    if (!leds[j])
      leds[j] = lightcolor;
  }
}

void moveWhiteLeds(CRGB *leds, unsigned int distance)
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  int numLedsToLight = 10;
  int position = map(distance, 10, 100, NUM_LEDS - numLedsToLight, 0);

  for (int i = position; i < position + numLedsToLight; i++)
  {
    leds[i] = CRGB::White;
  }
}
