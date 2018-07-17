#ifndef __COLOR_CALCULATOR
#define __COLOR_CALCULATOR

struct Position;

#include <FastLED.h>
#include "PositionCalculator.h"

struct ColorConfig {
    int stepsize;
    int halfStepsize;

    ColorConfig(int stepsize);
};

class ColorCalculator {
    ColorConfig config = ColorConfig(0);

    public:
    ColorCalculator(ColorConfig config);

    CHSV getAverage(CHSV prev, CHSV leftOrUp);
    CHSV getAverage(CHSV prev, CHSV left, CHSV up);
    CHSV modifyColor(CHSV baseValue);
    void determineNextColor(Position position);
};

#endif // __COLOR_CALCULATOR