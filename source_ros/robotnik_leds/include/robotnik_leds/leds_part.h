#ifndef _LEDS_PART_H__
#define _LEDS_PART_H__

#include <robotnik_leds/color.h>
#include <robotnik_leds/part.h>
#include <robotnik_leds/led_mode.h>

using namespace std;

class LedsPart{
    TYPE_PART part;
    int led_start;
    int led_end;
    TYPE_LED_MODE mode;
    Color color1;
    Color color2;
    Color color3;
    bool endable;
    bool painted;

public:
    LedsPart(TYPE_PART _part, int _led_start, int _led_end, TYPE_LED_MODE _mode = TYPE_LED_MODE::continous,TYPE_COLOR _color1= TYPE_COLOR::white, TYPE_COLOR _color2=TYPE_COLOR::white, TYPE_COLOR _color3=TYPE_COLOR::white);
    LedsPart(TYPE_PART _part, int _led_start, int _led_end,  TYPE_LED_MODE _mode = TYPE_LED_MODE::continous,Color _color1 = Color(TYPE_COLOR::white), Color _color2= Color(TYPE_COLOR::white), Color _color3= Color(TYPE_COLOR::white));
    TYPE_PART getPart ();
    TYPE_LED_MODE getMode();
    int getLedStart();
    int getLedEnd();
    Color getColor1();
    Color getColor2();
    Color getColor3();
    bool getPainted();
    void setMode(TYPE_LED_MODE _mode);
    void setColor1(Color _color1);
    void setColor1(TYPE_COLOR _color1);
    void setColor2(Color _color2);
    void setColor2(TYPE_COLOR _color2);
    void setColor3(Color _color3);
    void setColor3(TYPE_COLOR _color3);
    void setPainted(bool _painted);
    void setEndable(bool _endable);
    bool getEndable();

};

#endif //_LEDS_PART_H__
