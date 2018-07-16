
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

#define STEPSIZE 64
#define HALF_STEPSIZE STEPSIZE / 2

#define CONTROLLER WS2812B

CRGB leds[NUM_LEDS];

uint8_t gimmeColor(uint8_t value) {
    return (uint8_t)((float)value / WIDTH * 255) + 1;
}

uint8_t gimmeNextColor(int16_t prev) {
    prev += random(1, STEPSIZE) - HALF_STEPSIZE;

    return max(10, min(255, prev));
}

uint8_t gimmeNextColor(int16_t prev, int16_t left) {
    int16_t avg = (prev + left) / 2;

    return gimmeNextColor(avg);
}

uint8_t gimmeNextColor(int16_t prev, int16_t left, int16_t up) {
    int16_t avg = (left + up + prev) / 3;

    return gimmeNextColor(avg);
}

void draw() {
    for(int row = 0; row < HEIGHT; row++) {
        for (int column = 0; column < WIDTH; column++) {
            if (row % 2 == 0) {
                leds[row * WIDTH + column] = CRGB::Red;
            } else {
                leds[(row + 1) * WIDTH - column - 1] = CRGB::Green;
            }
        }
    }
}

bool isFirstRow(int row) {
    return row == 0;
}

bool isFirstColumn(int column) {
    return column == 0;
}

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
    
    if (isFirstRow(row)) {
        if (isFirstColumn(column)) {
            leds[current].setRGB(
                gimmeNextColor(leds[current].r),
                gimmeNextColor(leds[current].g),
                gimmeNextColor(leds[current].b)
            );
        } else {
            leds[current].setRGB(
                gimmeNextColor(leds[current].r, leds[previous].r),
                gimmeNextColor(leds[current].g, leds[previous].g),
                gimmeNextColor(leds[current].b, leds[previous].b)
            );
        }
    } else {
        if (isFirstColumn(column)) {
            leds[current].setRGB(
                gimmeNextColor(leds[current].r, leds[up].r),
                gimmeNextColor(leds[current].g, leds[up].g),
                gimmeNextColor(leds[current].b, leds[up].b)
            );
        } else {
            leds[current].setRGB(
                gimmeNextColor(leds[current].r, leds[up].r, leds[previous].r),
                gimmeNextColor(leds[current].g, leds[up].g, leds[previous].g),
                gimmeNextColor(leds[current].b, leds[up].b, leds[previous].b)
            );
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

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    FastLED.show();
}

void loop() {
    unsigned long time = millis();

    iterate();
    FastLED.show();
    time = millis() - time;

    delay(FRAME_TIME - time);
}

