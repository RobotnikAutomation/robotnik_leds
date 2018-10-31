#ifndef __LEDS_EFFECTS_H_
#define __LEDS_EFFECTS_H_

#include <iostream>
#include <map>
#include <vector>

#include <robotnik_leds/leds_part.h>
#include <robotnik_leds/leds_value.h>

using namespace std;

class LedsEffects {
    bool init;
    int leds;
    map<TYPE_PART, LedsPart> parts;
    unsigned int clock;
    int period;
    int move_cont;
    unsigned int clock_frame;
    bool cleaner;

   public:
    LedsEffects(int _leds = 60, int _period = 10);
    LedsEffects(map<TYPE_PART, LedsPart> _parts, int _leds, int _period = 1);
    void setPeriod(int _period);
    void increment_clock();
    void insertPart(TYPE_PART part, int led_start, int led_end, Color color1,
                    Color color2 = Color(TYPE_COLOR::black),
                    Color color3 = Color(TYPE_COLOR::black));
    void insertPart(TYPE_PART part, int led_start, int led_end,
                    TYPE_COLOR color1, TYPE_COLOR color2 = TYPE_COLOR::black,
                    TYPE_COLOR color3 = TYPE_COLOR::black);
    void inserEffect(TYPE_PART part, TYPE_LED_MODE mode, Color color1,
                     Color color2 = Color(TYPE_COLOR::black),
                     Color color3 = Color(TYPE_COLOR::black));
    void inserEffect(TYPE_PART part, TYPE_LED_MODE mode, TYPE_COLOR color1,
                     TYPE_COLOR color2 = TYPE_COLOR::black,
                     TYPE_COLOR color3 = TYPE_COLOR::black);
    void endableEffect(TYPE_PART part);
    void disableEffect(TYPE_PART part);
    void disableAllEffect();
    bool isPartAllActive();
    bool isActivePart(TYPE_PART part);
    LedsPart getLedPart(TYPE_PART part);
    vector<robotnik_leds::leds_value> paint(bool increment_clock = true);
    vector<robotnik_leds::leds_value> clearPaint();
    void clear();

   private:
    int zoneLeds(TYPE_PART _part);
    bool isPaintedZone(TYPE_PART _part);
    void setPaintedZone(TYPE_PART _part, bool _painted);
    robotnik_leds::leds_value zone(TYPE_PART part, int numColor = 1);
    robotnik_leds::leds_value zone(TYPE_PART part, int led_start, int led_end,
                                   int numColor = 1);
    robotnik_leds::leds_value zone(TYPE_PART part, TYPE_COLOR color);
    robotnik_leds::leds_value zone(TYPE_PART part, Color color);

    robotnik_leds::leds_value configuration();
    robotnik_leds::leds_value pixel(TYPE_PART part, int led, int numColor = 1);
    robotnik_leds::leds_value msgPredefined(int value);

    vector<robotnik_leds::leds_value> callEffect(TYPE_PART _part);
    vector<robotnik_leds::leds_value> clearAllEffect();
    vector<robotnik_leds::leds_value> clearZoneEffect(TYPE_PART _part);
    vector<robotnik_leds::leds_value> continuousEffect(TYPE_PART _part);
    vector<robotnik_leds::leds_value> cumulative(TYPE_PART _part,
                                                 bool inverse = false);
    vector<robotnik_leds::leds_value> cumulativeCenter(TYPE_PART _part,
                                                       bool inverse = false);
    vector<robotnik_leds::leds_value> blinkingEffect(TYPE_PART _part);
    vector<robotnik_leds::leds_value> round(TYPE_PART _part,
                                            bool inverse = false);
    vector<robotnik_leds::leds_value> knightRiderEffect(TYPE_PART _part);
    vector<robotnik_leds::leds_value> doubleKnightRiderEffect(TYPE_PART _part);
};

#endif
