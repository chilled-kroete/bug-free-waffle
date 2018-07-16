
#include <stdio.h>
#include <Arduino.h>
#include <FastLED.h>

// Hardware Stuff
#define NUM_LEDS 144
#define DATA_PIN 6

#define HEIGHT 9
#define WIDTH 16

// Application stuff
#define FRAMES 10
#define FRAME_TIME 1000 / FRAMES

#define STEPSIZE 12
#define HALF_STEPSIZE STEPSIZE / 2

#define CONTROLLER WS2812B

CRGB leds[NUM_LEDS];




/********************************************
 *  positioning 
 * 
 */

bool isFirstRow(int row) {
    return row == 0;
}

bool isFirstColumn(int column) {
    return column == 0;
}



/********************************************
 *  coloring
 * 
 */

CRGB getAverage(CRGB prev, CRGB leftOrUp) {
    return CRGB(
        (prev.r + leftOrUp.r) / 2,
        (prev.g + leftOrUp.g) / 2,
        (prev.b + leftOrUp.b) / 2
    );
}

CRGB getAverage(CRGB prev, CRGB left, CRGB up) {
    return CRGB(
        (left.r + up.r + prev.r) / 3,
        (left.g + up.g + prev.g) / 3,
        (left.b + up.b + prev.b) / 3
    );
}

CRGB modifyColor(CRGB baseValue) {
    CHSV hsv = rgb2hsv_approximate(baseValue);
    hsv.hue += random(1, STEPSIZE) - HALF_STEPSIZE;
    hsv.value = 255;
    hsv.saturation = 255;

    return CRGB(hsv);
}


/********************************************
 *  Main iterative stuff 
 * 
 */



void determineNextColor(int row, int column) {
    int current;
    int previous;
    int up;

    if (row % 2 == 0) {
        current = row * WIDTH + column;
        previous = current - 1;
        up = current - (2 * column) - 1;

    } else {
        current = (row + 1) * WIDTH - column - 1;
        previous = current + 1;
        up = (row - 1) * WIDTH + column;
    }

    CRGB base;
    if (isFirstRow(row)) {
        if (isFirstColumn(column)) {
            base = leds[current];
        } else {
            base = getAverage(leds[current], leds[previous]);
        }
    } else {
        if (isFirstColumn(column)) {
            base = getAverage(leds[current], leds[up]);
        } else {
            base = getAverage(leds[current], leds[up], leds[previous]);
        }
    }


    leds[current] = modifyColor(base);
}

void draw() {
    for(int row = 0; row < HEIGHT; row++) {
        for (int column = 0; column < WIDTH; column++) {
            leds[row * WIDTH + column] = CRGB::Blue;
        }
    }
}

void iterate() {
    for(int row = 0; row < HEIGHT; row++) {
        for (int column = 0; column < WIDTH; column++) {
            determineNextColor(row, column);
        }
    }
}

/********************************************
 *  Arduino Callbacks 
 * 
 */

void setup() {
    FastLED.setBrightness(255 / 4);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    draw();
    FastLED.show();
}

void loop() {
    unsigned long time = millis();

    iterate();
    FastLED.show();
    time = millis() - time;

    delay(FRAME_TIME - time);
}
