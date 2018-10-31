
#include <iostream>

using namespace std;

#include <robotnik_leds/color.h>

Color::Color() { w = r = g = b = 0; }

Color::Color(int _w, int _r, int _g, int _b) { setColor(_w, _r, _g, _b); }

Color::Color(TYPE_COLOR tc) { setColor(tc); }

void Color::setColor(int _w, int _r, int _g, int _b) {
    w = _w;
    r = _r;
    g = _g;
    b = _b;
}

void Color::setColor(TYPE_COLOR tc) {
    switch (tc) {
        case TYPE_COLOR::red:
            setRed();
            break;
        case TYPE_COLOR::green:
            setGreen();
            break;
        case TYPE_COLOR::blue:
            setBlue();
            break;
        case TYPE_COLOR::white:
            setWhite();
            break;
        case TYPE_COLOR::black:
            setBlack();
            break;
        case TYPE_COLOR::orange:
            setOrange();
            break;
        case TYPE_COLOR::yellow:
            setYellow();
            break;
    }
}

void Color::setRed() {
    w = g = b = 0;
    r = 255;
}

void Color::setGreen() {
    w = r = b = 0;
    g = 255;
}

void Color::setBlue() {
    w = r = g = 0;
    b = 255;
}

void Color::setWhite() {
    w = 255;
    r = g = b = 0;
}

void Color::setBlack() { w = r = g = b = 0; }

void Color::setOrange() {
    w = 0;
    r = 255;
    g = 102;
    b = 0;
}

void Color::setYellow() {
    w = 10;
    r = 255;
    g = 255;
    b = 0;
}
