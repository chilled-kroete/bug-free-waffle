#include <FastLED.h>
//#include "PositionCalculator.cpp"


struct ColorConfig {
    int stepsize;
    int halfStepsize;

    ColorConfig(int stepsize) {
        this->stepsize = stepsize;
        this->halfStepsize = stepsize / 2;
    }
};


class ColorCalculator {
    ColorConfig config = ColorConfig(0);

    public:

    ColorCalculator(ColorConfig config) {
        this->config = config;
    }

    /**
     * Calculate the average between 2 HSV colors (by hue averaging) 
     */
    CHSV getAverage(CHSV prev, CHSV leftOrUp) {
        return CHSV((prev.h + leftOrUp.h) / 2, 255, 255);
    }

    /**
     * Calculate the average between 3 HSV colors (by hue averaging) 
     */
    CHSV getAverage(CHSV prev, CHSV left, CHSV up) {
        return CHSV((left.h + up.h + prev.h) / 3, 255, 255);
    }

    /**
     * Modify a color by applying a not-so-random random value 
     */
    CHSV modifyColor(CHSV baseValue) {
        baseValue.hue += random(1, this->config.stepsize) - this->config.halfStepsize;

        return baseValue;
    }

    /**
     * Given the current position calculate the next color
     */
    void determineNextColor(Position position) {
        CHSV base;
        extern CHSV leds[];
        if (position.isFirstRow()) {
            if (position.isFirstColumn()) {
                base = leds[position.current];
            } else {
                base = this->getAverage(leds[position.current], leds[position.left]);
            }
        } else {
            if (position.isFirstColumn()) {
                base = this->getAverage(leds[position.current], leds[position.up]);
            } else {
                base = this->getAverage(leds[position.current], leds[position.up], leds[position.left]);
            }
        }

        leds[position.current] = this->modifyColor(base);
    }
};
