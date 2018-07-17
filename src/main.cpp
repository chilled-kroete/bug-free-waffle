
#include <stdio.h>
#include <Arduino.h>
#include <FastLED.h>
#include "config.cpp"
#include "PositionCalculator.h"
#include "ColorCalculator.h"

CHSV leds[NUM_LEDS];
CRGB display[NUM_LEDS];

MatrixConfig matrixConfig = MatrixConfig(WIDTH, HEIGHT);
PositionCalculator calculator = PositionCalculator(matrixConfig);

ColorConfig colorConfig = ColorConfig(STEPSIZE);
ColorCalculator colorCalculator = ColorCalculator(colorConfig);

void bitblt() {
    for(int n = 0; n < NUM_LEDS; n++) {
        display[n] = leds[n];
    }
}

void draw() {
    CHSV blubsi;
    blubsi.setHSV(255, 0, 255);
    for(int row = 0; row < HEIGHT; row++) {
        for (int column = 0; column < WIDTH; column++) {
            leds[row * WIDTH + column] = blubsi;
        }
    }
}

void iterate() {
    extern PositionCalculator calculator;
    extern ColorCalculator colorCalculator;
    
    for(int row = 0; row < HEIGHT; row++) {
        for (int column = 0; column < WIDTH; column++) {
            Position position = calculator.translate(row, column);
            colorCalculator.determineNextColor(position);
        }
    }
}

/********************************************
 *  Arduino Callbacks 
 * 
 */

void setup() {
    FastLED.setBrightness(255 / 3);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(display, NUM_LEDS);

    draw();
    bitblt();
    FastLED.show();

    delay(3000);
}

void loop() {
    unsigned long time = millis();

    iterate();
    bitblt();
    FastLED.show();
    
    time = millis() - time;

    delay(FRAME_TIME - time);
}
