#include <robotnik_leds/leds_effects.h>
using namespace std;

LedsEffects::LedsEffects(int _leds, int _period){
    leds = _leds;
    period = _period;
    clock=1;
    clock_frame=1;
    move_cont=0;
    LedsPart aux_all = LedsPart (TYPE_PART::all, 0, leds, TYPE_LED_MODE::none, TYPE_COLOR::red);
    aux_all.setEndable(false);
    parts.insert(pair<TYPE_PART, LedsPart> (TYPE_PART::all, aux_all));
    init = false;
    cleaner = false;
}

LedsEffects::LedsEffects(map<TYPE_PART, LedsPart> _parts, int _leds, int _period){
    leds = _leds;
    period=_period;
    clock = 1;
    clock_frame=1;
    move_cont=0;
    bool have_part_all = false;
    for(map<TYPE_PART, LedsPart>::iterator it= _parts.begin(); it!=_parts.end(); ++it){
        if(it->first==TYPE_PART::all){
            have_part_all = true;
        }
        parts.insert(pair<TYPE_PART, LedsPart> (it->first, it->second));
    }
    if (!have_part_all){
        LedsPart aux_all = LedsPart (TYPE_PART::all, 0, leds, TYPE_LED_MODE::none, TYPE_COLOR::red);
        aux_all.setEndable(false);
        parts.insert(pair<TYPE_PART, LedsPart> (TYPE_PART::all, aux_all));
    }
    init = false;
    cleaner = false;
}

void LedsEffects::setPeriod(int _period){
    period = _period;
}

void LedsEffects::increment_clock(){
    clock = (clock + 1)% UINT16_MAX;
    if (clock == 0){
        clock +1;
    }
    if (clock % period == 0){
        move_cont = (move_cont+1) % UINT16_MAX;

    }
}

void LedsEffects::insertPart (TYPE_PART part, int led_start, int led_end, Color color1, Color color2, Color color3){
    parts.insert(pair<TYPE_PART, LedsPart> (part, LedsPart(part, led_start, led_end, TYPE_LED_MODE::none,color1, color2, color3)));
}

void LedsEffects::insertPart (TYPE_PART part, int led_start, int led_end, TYPE_COLOR color1, TYPE_COLOR color2, TYPE_COLOR color3){
    parts.insert(pair<TYPE_PART, LedsPart> (part, LedsPart(part, led_start, led_end, TYPE_LED_MODE::none,Color(color1), Color(color2), Color(color3))));
}

void LedsEffects::inserEffect(TYPE_PART part, TYPE_LED_MODE mode, Color color1, Color color2, Color color3){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        it->second.setMode(mode);
        it->second.setColor1(color1);
        it->second.setColor2(color2);
        it->second.setColor3(color3);
        it->second.setPainted(false);
        it->second.setEndable(true);
    }else{
        cout << "Error - leds effects: You don't insert the part " << part << endl;
    }
}

void LedsEffects::inserEffect(TYPE_PART part, TYPE_LED_MODE mode, TYPE_COLOR color1, TYPE_COLOR color2, TYPE_COLOR color3){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        it->second.setMode(mode);
        it->second.setColor1(color1);
        it->second.setColor2(color2);
        it->second.setColor3(color3);
        it->second.setPainted(false);
        it->second.setEndable(true);
    }else{
        cout << "Error - leds effects: You don't insert the part " << part << endl;
    }
}

void LedsEffects::endableEffect(TYPE_PART part){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        it->second.setEndable(true);
    }else{
        cout << "Error - leds effects: You don't insert the part " << part << endl;
    }
}


void LedsEffects::disableEffect(TYPE_PART part){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        it->second.setEndable(false);
    }else{
        cout << "Error - leds effects: You don't insert the part " << part << endl;
    }
}

void LedsEffects::disableAllEffect(){
    for(map <TYPE_PART, LedsPart>::iterator it = parts.begin(); it != parts.end(); ++it){
        disableEffect(it->first);
    }
    cleaner = true;
}

bool LedsEffects::isPartAllActive(){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(TYPE_PART::all);
    if (it!=parts.end()){
        return it->second.getEndable();
    }else{
        cout << "Error - leds effects: You don't insert the part ALL" << endl;
        return false;
    }
}

bool LedsEffects::isActivePart(TYPE_PART part){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        return it->second.getEndable();
    }else{
        cout << "Error - leds effects: You don't' insert the part " << part << endl;
        return false;
    }
}

LedsPart LedsEffects::getLedPart(TYPE_PART part){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        return it->second;
    }else{
        cout << "Error - leds effects: You don't' insert the part " << part << endl;
        return LedsPart(TYPE_PART::none, 0, 0, TYPE_LED_MODE::none, TYPE_COLOR::white, TYPE_COLOR::white, TYPE_COLOR::white);
    }
}

bool LedsEffects::isPaintedZone(TYPE_PART part){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(part);
    if (it!=parts.end()){
        return it->second.getPainted();
    }else{
        cout << "Error - leds effects: You don't' insert the part " << part << endl;
        return false;
    }
}

void LedsEffects::setPaintedZone(TYPE_PART _part, bool _painted){
    map< TYPE_PART, LedsPart>::iterator it = parts.find(_part);
    if (it!=parts.end()){
        it->second.setPainted(_painted);
    }else{
        cout << "Error - leds effects: You don't' insert the part " << _part << endl;
    }
}


int LedsEffects::zoneLeds(TYPE_PART _part){
    return (parts.at(_part).getLedEnd() - parts.at(_part).getLedStart());

}

/***********************************PAINT MESSAGE**************************/
robotnik_leds::leds_value LedsEffects::zone(TYPE_PART part, int numColor){
    robotnik_leds::leds_value zone_msg;
    zone_msg.request.mode = 'z';
    zone_msg.request.ledStart = parts.at(part).getLedStart();
    zone_msg.request.ledEnd = parts.at(part).getLedEnd();
    if (numColor==1){
        zone_msg.request.white = parts.at(part).getColor1().w;
        zone_msg.request.red = parts.at(part).getColor1().r;
        zone_msg.request.green = parts.at(part).getColor1().g;
        zone_msg.request.blue = parts.at(part).getColor1().b;
    }else if (numColor==2){
        zone_msg.request.white = parts.at(part).getColor2().w;
        zone_msg.request.red = parts.at(part).getColor2().r;
        zone_msg.request.green = parts.at(part).getColor2().g;
        zone_msg.request.blue = parts.at(part).getColor2().b;
    }else if (numColor == 3){
        zone_msg.request.white = parts.at(part).getColor3().w;
        zone_msg.request.red = parts.at(part).getColor3().r;
        zone_msg.request.green = parts.at(part).getColor3().g;
        zone_msg.request.blue = parts.at(part).getColor3().b;
    }else{
        zone_msg.request.white=0;
        zone_msg.request.red = 0;
        zone_msg.request.green = 0;
        zone_msg.request.blue = 0;
    }
    return zone_msg;
}

robotnik_leds::leds_value LedsEffects::zone(TYPE_PART part, TYPE_COLOR color){
    robotnik_leds::leds_value zone_msg;
    zone_msg.request.mode = 'z';
    zone_msg.request.ledStart = parts.at(part).getLedStart();
    zone_msg.request.ledEnd = parts.at(part).getLedEnd();
    Color aux (color);
    zone_msg.request.white = aux.w;
    zone_msg.request.red = aux.r;
    zone_msg.request.green = aux.g;
    zone_msg.request.blue = aux.b;

    return zone_msg;
}

robotnik_leds::leds_value LedsEffects::zone(TYPE_PART part, Color color){
    robotnik_leds::leds_value zone_msg;
    zone_msg.request.mode = 'z';
    zone_msg.request.ledStart = parts.at(part).getLedStart();
    zone_msg.request.ledEnd = parts.at(part).getLedEnd();
    zone_msg.request.white = color.w;
    zone_msg.request.red = color.r;
    zone_msg.request.green = color.g;
    zone_msg.request.blue = color.b;

    return zone_msg;
}


robotnik_leds::leds_value LedsEffects::zone(TYPE_PART part, int led_start, int led_end, int numColor){
    robotnik_leds::leds_value zone_msg;
    zone_msg.request.mode = 'z';
    zone_msg.request.ledStart = parts.at(part).getLedStart() + led_start%(parts.at(part).getLedEnd()+1);
    zone_msg.request.ledEnd = parts.at(part).getLedStart() + led_end%(parts.at(part).getLedEnd()+1);
    if (numColor == 1){
        zone_msg.request.white = parts.at(part).getColor1().w;
        zone_msg.request.red = parts.at(part).getColor1().r;
        zone_msg.request.green = parts.at(part).getColor1().g;
        zone_msg.request.blue = parts.at(part).getColor1().b;
    }else if (numColor == 2){
        zone_msg.request.white = parts.at(part).getColor2().w;
        zone_msg.request.red = parts.at(part).getColor2().r;
        zone_msg.request.green = parts.at(part).getColor2().g;
        zone_msg.request.blue = parts.at(part).getColor2().b;
    }else if (numColor == 3){
        zone_msg.request.white = parts.at(part).getColor3().w;
        zone_msg.request.red = parts.at(part).getColor3().r;
        zone_msg.request.green = parts.at(part).getColor3().g;
        zone_msg.request.blue = parts.at(part).getColor3().b;
    }else{
        zone_msg.request.white = 0;
        zone_msg.request.red = 0;
        zone_msg.request.green = 0;
        zone_msg.request.blue = 0;
    }
    return zone_msg;
}


robotnik_leds::leds_value LedsEffects::configuration(){
    robotnik_leds::leds_value configuration_msg;
    configuration_msg.request.mode = 'c';
    configuration_msg.request.ledStart = leds;
    return configuration_msg;
}

robotnik_leds::leds_value LedsEffects::pixel(TYPE_PART part, int led, int numColor){
    robotnik_leds::leds_value pixel_msg;
    pixel_msg.request.mode = 'p';
    pixel_msg.request.ledStart = parts.at(part).getLedStart() + led%(parts.at(part).getLedEnd()+1);
    if (numColor == 1){
        pixel_msg.request.white = parts.at(part).getColor1().w;
        pixel_msg.request.red = parts.at(part).getColor1().r;
        pixel_msg.request.green = parts.at(part).getColor1().g;
        pixel_msg.request.blue = parts.at(part).getColor1().b;
    }else if (numColor == 2){
        pixel_msg.request.white = parts.at(part).getColor2().w;
        pixel_msg.request.red = parts.at(part).getColor2().r;
        pixel_msg.request.green = parts.at(part).getColor2().g;
        pixel_msg.request.blue = parts.at(part).getColor2().b;
    }else if (numColor == 3){
        pixel_msg.request.white = parts.at(part).getColor3().w;
        pixel_msg.request.red = parts.at(part).getColor3().r;
        pixel_msg.request.green = parts.at(part).getColor3().g;
        pixel_msg.request.blue = parts.at(part).getColor3().b;
    }else{
        pixel_msg.request.white = 0;
        pixel_msg.request.red = 0;
        pixel_msg.request.green = 0;
        pixel_msg.request.blue = 0;
    }
    return pixel_msg;
}

robotnik_leds::leds_value LedsEffects::msgPredefined(int value){
    robotnik_leds::leds_value msg_predefined;
    msg_predefined.request.mode = 'm';
    msg_predefined.request.msgDefault = value;
    return msg_predefined;
}


/************************************EFFECTS********************************/

vector <robotnik_leds::leds_value> LedsEffects::clearZoneEffect(TYPE_PART _part){
    if(_part == TYPE_PART::all){
        return clearAllEffect();
    }
    vector <robotnik_leds::leds_value> vsrv;
    parts.at(_part).setColor1(TYPE_COLOR::black);
    robotnik_leds::leds_value aux_vsrv = zone(_part);
    parts.at(_part).setPainted(true);
    parts.at(_part).setEndable(false);
    vsrv.push_back(aux_vsrv);
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::clearAllEffect(){
    vector <robotnik_leds::leds_value> vsrv;
    for (map<TYPE_PART, LedsPart>::iterator it = parts.begin(); it != parts.end(); ++it){
        it->second.setMode(TYPE_LED_MODE::none);
        it->second.setPainted(true);
        it->second.setEndable(false);
    }
    vsrv.push_back(this->msgPredefined(0));
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::continuousEffect(TYPE_PART _part){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    parts.at(_part).setPainted(true);
    vsrv.push_back(zone(_part));
    return vsrv;
}


vector <robotnik_leds::leds_value> LedsEffects::blinkingEffect(TYPE_PART _part){
    bool light = (move_cont%2)==0;
    vector <robotnik_leds::leds_value> vsrv;
    int color = 1;
    if (!light) color = 2;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    parts.at(_part).setPainted(true);
    vsrv.push_back(zone(_part,color));

    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::round(TYPE_PART _part, bool inverse){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    if((clock_frame)%period == 0){
        int led = (move_cont)%(zoneLeds(_part));
        if (inverse){
            led = zoneLeds(_part) - led;
        }
        parts.at(_part).setPainted(true);
        vsrv.push_back(zone(_part, 2));
        vsrv.push_back(pixel(_part, led, 1));
    }
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::cumulative(TYPE_PART _part, bool inverse){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    if((clock_frame)%period == 0){
        int led = move_cont%zoneLeds(_part);
        int color1 = 1;
        int color2 = 2;

        if (inverse){
            led = zoneLeds(_part) - led;
            color1 = 2;
            color2 = 1;
            vsrv.push_back(zone(_part, led+1, parts.at(_part).getLedEnd()-led, color2));
            vsrv.push_back(zone(_part, 0, led, color1));

        }else{
            vsrv.push_back(zone(_part, led+1, parts.at(_part).getLedEnd()-led, color1));
            vsrv.push_back(zone(_part, 0, led, color2));

        }
        parts.at(_part).setPainted(true);


    }
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::cumulativeCenter(TYPE_PART _part, bool inverse){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    if((clock_frame)%period == 0){
        int led = move_cont%zoneLeds(_part)/2;
        int color1 = 1;
        int color2 = 2;

        if (inverse){
            led = zoneLeds(_part) - led;
            color1 = 2;
            color2 = 1;
            vsrv.push_back(zone(_part, color2));
            vsrv.push_back(zone(_part, led+1, parts.at(_part).getLedEnd()-led, color2));
            vsrv.push_back(zone(_part, 0, led, color1));

        }else{
            vsrv.push_back(zone(_part, led+1, parts.at(_part).getLedEnd()-led, color1));
            vsrv.push_back(zone(_part, 0, led, color2));

        }
        parts.at(_part).setPainted(true);


    }
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::knightRiderEffect(TYPE_PART _part){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    parts.at(_part).setPainted(true);
    if((clock_frame)%period==0){
        int led = move_cont % (zoneLeds(_part)*2);
        vsrv.push_back(zone(_part, 2));
        if(led > zoneLeds(_part)){
            vsrv.push_back(pixel(_part,led));
        }else{
            vsrv.push_back(pixel(_part,zoneLeds(_part)-led));
        }
    }
    return vsrv;
}

vector <robotnik_leds::leds_value> LedsEffects::doubleKnightRiderEffect(TYPE_PART _part){
    vector <robotnik_leds::leds_value> vsrv;
    if(!parts.at(_part).getPainted()){
        vsrv.push_back(zone(_part, TYPE_COLOR::black));
    }
    parts.at(_part).setPainted(true);
    if((clock_frame)%period==0){
        int led = move_cont % (zoneLeds(_part));
        vsrv.push_back(zone(_part, 2));
        if(led > zoneLeds(_part)/2){
            vsrv.push_back(pixel(_part,led));
            vsrv.push_back(pixel(_part,zoneLeds(_part)-led));
        }else{
            vsrv.push_back(pixel(_part,(zoneLeds(_part)/2)-led));
            vsrv.push_back(pixel(_part,(zoneLeds(_part)/2)+led));
        }
    }
    return vsrv;

}

vector <robotnik_leds::leds_value> LedsEffects::callEffect(TYPE_PART _part){

    switch (parts.at(_part).getMode()) {
    case TYPE_LED_MODE::none:
        if(!isPaintedZone(_part)&& isActivePart(_part))
            return clearZoneEffect(_part);
        break;
    case TYPE_LED_MODE::continous:
        if (!isPaintedZone(_part) && isActivePart(_part))
            return continuousEffect(_part);
        break;
    case TYPE_LED_MODE::blinking:
        if (isActivePart(_part))
            return blinkingEffect(_part);
        break;
    case TYPE_LED_MODE::round:
        if (isActivePart(_part))
            return round(_part);
        break;
    case TYPE_LED_MODE::roundInv:
        if (isActivePart(_part))
            return round (_part, true);
        break;
    case TYPE_LED_MODE::knightRider:
        if (isActivePart(_part))
            return knightRiderEffect(_part);
        break;
    case TYPE_LED_MODE::doubleKnightRider:
        if (isActivePart(_part))
            return doubleKnightRiderEffect(_part);
        break;
    case TYPE_LED_MODE::cumulative:
        if (isActivePart(_part))
            return cumulative(_part);
        break;
    case TYPE_LED_MODE::cumulativeInv:
        if (isActivePart(_part))
            return cumulative(_part, true);
        break;
    case TYPE_LED_MODE::cumulativeCenter:
        if (isActivePart(_part))
            return cumulativeCenter(_part);
        break;
    case TYPE_LED_MODE::cumulativeCenterInv:
        if (isActivePart(_part))
            return cumulativeCenter(_part, true);
        break;
    }

    return vector <robotnik_leds::leds_value>();
}


/***************************************Paint ********************************************/
vector <robotnik_leds::leds_value> LedsEffects::paint(bool increment_clock){
    vector <robotnik_leds::leds_value> ret;
    ret.empty();
    if(!init){
        ret.push_back(configuration());
        init = true;
        return ret;
    }
    if(increment_clock){
        this->increment_clock();
    }
    clock_frame = clock;

    vector <robotnik_leds::leds_value> aux;
    if(cleaner){
        ret.push_back(msgPredefined(0));
        cleaner = false;
    }
    if(isActivePart(TYPE_PART::all)){
        //cout << "Parte all activa" << endl;
        return callEffect(TYPE_PART::all);
    }else{
        for(map<TYPE_PART, LedsPart>::iterator it = parts.begin(); it != parts.end(); ++it){
            if(it->second.getPart()!=TYPE_PART::all && isActivePart(it->first)){
                aux.empty();
                aux = (callEffect(it->first));
                for (int cont=0; cont < aux.size(); cont++){
                    ret.push_back(aux.at(cont));
                }

            }
        }
    }
    return ret;
}

vector <robotnik_leds::leds_value> LedsEffects::clearPaint(){
    vector <robotnik_leds::leds_value> ret;
    for(map<TYPE_PART, LedsPart>::iterator it = parts.begin(); it != parts.end(); ++it){
            it->second.setMode(TYPE_LED_MODE::none);
            it->second.setEndable(false);
            it->second.setPainted(true);

    }
    ret.push_back(msgPredefined(0));
    return (ret);
}

void LedsEffects::clear(){
    cleaner = true;
    for(map<TYPE_PART, LedsPart>::iterator it = parts.begin(); it != parts.end(); ++it){
        it->second.setMode(TYPE_LED_MODE::none);
        it->second.setEndable(false);
        it->second.setPainted(false);
    }
}
