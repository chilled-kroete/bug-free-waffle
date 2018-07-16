
#include <stdio.h>
#include <Arduino.h>
#include <FastLED.h>
#include "PositionCalculator.cpp"

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

CHSV leds[NUM_LEDS];
CRGB display[NUM_LEDS];


struct ColorConfig {
    int stepsize;
    int halfStepsize;

    ColorConfig(int stepsize) {
        this->stepsize = stepsize;
        this->halfStepsize = stepsize / 2;
    }
};


MatrixConfig matrixConfig = MatrixConfig(16, 9);
PositionCalculator calculator = PositionCalculator(matrixConfig);


/********************************************
 *  coloring
 * 
 */

CHSV getAverage(CHSV prev, CHSV leftOrUp) {
    return CHSV((prev.h + leftOrUp.h) / 2, 255, 255);
}

CHSV getAverage(CHSV prev, CHSV left, CHSV up) {
    return CHSV((left.h + up.h + prev.h) / 3, 255, 255);
}

CHSV modifyColor(CHSV baseValue) {
    baseValue.hue += random(1, STEPSIZE) - HALF_STEPSIZE;

    return baseValue;
}


/********************************************
 *  Main iterative stuff 
 * 
 */



void determineNextColor(int row, int column) {

    extern PositionCalculator calculator;
    Neighbour neighbour = calculator.translate(row, column);
    
    CHSV base;
    if (neighbour.isFirstRow(row)) {
        if (neighbour.isFirstColumn(column)) {
            base = leds[neighbour.current];
        } else {
            base = getAverage(leds[neighbour.current], leds[neighbour.left]);
        }
    } else {
        if (neighbour.isFirstColumn(column)) {
            base = getAverage(leds[neighbour.current], leds[neighbour.up]);
        } else {
            base = getAverage(leds[neighbour.current], leds[neighbour.up], leds[neighbour.left]);
        }
    }

    leds[neighbour.current] = modifyColor(base);
}

void blit() {
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
    FastLED.setBrightness(255 / 3);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(display, NUM_LEDS);

    draw();
    blit();
    FastLED.show();

    delay(3000);
}

void loop() {
    unsigned long time = millis();

    //iterate();
    blit();
    FastLED.show();
    
    time = millis() - time;

    delay(FRAME_TIME - time);
}
