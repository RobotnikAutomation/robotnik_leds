#include <robotnik_leds/leds_part.h>

using namespace std;

LedsPart::LedsPart(TYPE_PART _part, int _led_start, int _led_end,
                   TYPE_LED_MODE _mode, Color _color1, Color _color2,
                   Color _color3) {
    part = _part;
    led_start = _led_start;
    led_end = _led_end;
    mode = _mode;
    color1 = _color1;
    color2 = _color2;
    color3 = _color3;
    painted = false;
    endable = false;
}

LedsPart::LedsPart(TYPE_PART _part, int _led_start, int _led_end,
                   TYPE_LED_MODE _mode, TYPE_COLOR _color1, TYPE_COLOR _color2,
                   TYPE_COLOR _color3) {
    part = _part;
    led_start = _led_start;
    led_end = _led_end;
    mode = _mode;
    color1 = Color(_color1);
    color2 = Color(_color2);
    color3 = Color(_color3);
    painted = false;
    endable = false;
}

TYPE_PART LedsPart::getPart() { return part; }

int LedsPart::getLedStart() { return led_start; }

int LedsPart::getLedEnd() { return led_end; }

TYPE_LED_MODE LedsPart::getMode() { return mode; }

Color LedsPart::getColor1() { return color1; }

Color LedsPart::getColor2() { return color2; }

Color LedsPart::getColor3() { return color3; }

bool LedsPart::getPainted() { return painted; }

void LedsPart::setMode(TYPE_LED_MODE _mode) { mode = _mode; }

void LedsPart::setColor1(Color _color1) { color1 = _color1; }

void LedsPart::setColor1(TYPE_COLOR _color1) { color1 = Color(_color1); }

void LedsPart::setColor2(Color _color2) { color2 = _color2; }

void LedsPart::setColor2(TYPE_COLOR _color2) { color2 = Color(_color2); }

void LedsPart::setColor3(Color _color3) { color3 = _color3; }

void LedsPart::setColor3(TYPE_COLOR _color3) { color3 = Color(_color3); }

void LedsPart::setPainted(bool _painted) { painted = _painted; }

void LedsPart::setEndable(bool _endable) { endable = _endable; }

bool LedsPart::getEndable() { return endable; }
