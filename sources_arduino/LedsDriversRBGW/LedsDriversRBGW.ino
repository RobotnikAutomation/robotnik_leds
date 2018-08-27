#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define LEDS 255
//Definicion de Adafruit_NeoPixel
Adafruit_NeoPixel strip;

//g,r,b,w
//r,b,w,g

#define WHITE strip.Color (0,0,255, 0)
#define RED    strip.Color(255, 0, 0, 0)
#define GREEN  strip.Color(0, 0, 0, 255)
#define BLUE   strip.Color(0, 255, 0, 0)
#define ORANGE strip.Color(102, 255, 0, 0)
#define BLACK strip.Color (0, 0, 0, 0)

int menStand;
#define NONE 0
#define ERR 1
#define ERRI 2
#define OK 3
#define OKI 4
#define WAIT 5
#define WAITI 6


boolean intermitente;
boolean mensajenuevo;
int leds;

int stateACK;
#define ACK_WAIT 0
#define ACK_OK 1
#define ACK_ERROR 2

#define TIMER_ERROR_ACK 5000
unsigned long time;
unsigned long last_time;


void setup (){
  Serial.begin(9600);
  Serial.setTimeout(100);
  leds = LEDS;
  //strip = Adafruit_NeoPixel(LEDS, PIN, NEO_KHZ800 + NEO_RGB );
  strip = Adafruit_NeoPixel(LEDS, PIN, NEO_KHZ800 + NEO_WRGB );
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  intermitente = true;
  menStand = NONE;
  stateACK = 0;
  time = last_time = millis();
}

void loop(){
  MenuReader();
  Mensajes();
  ACKState();
}

char CharReadingWait (){
  Serial.flush();
  return Serial.read();
}

int IntReadingWait (){
  return IntReadingWaitAtoi();
}



int IntReadingWaitAtoi (){
  Serial.flush();
  char buffer[255];
  int lg = 0;
  delay (5);
  while (true) {
    char c;
    c = Serial.read();
    if (c == '.') {
      buffer[lg] = '\0';
      //Serial.println("");
      return atoi(buffer);
      lg = 0;
    }
    else if (lg < (255-1)) {
      if (c>='0' && c <='9'){
        buffer[lg]= c;
        lg++;
      }
    }
  }
  return 0;
}

//Menu reader
boolean MenuReader(){
  if(Serial.available()>0){
    if(stateACK!=ACK_OK){
      stateACK = ACK_OK;
      menStand = NONE;
    }
    time = last_time = millis();
    int p, pend, paux1, paux2;
    int r, g, b, w;
    boolean jump;
    switch (Serial.read()){
      case 'i': //Get information about number of leds
      case 'I':
        Serial.println(leds);
        break;
      case 'c': //Set number of leds
      case 'C':
        leds = IntReadingWaitAtoi();
        leds = leds%LEDS;
        colorWipe (BLACK, 0, leds);
        strip.begin();
        strip.show();
        break;
      case 'p': //Change color to a led
      case 'P':
        menStand = NONE;
        mensajenuevo = false;
        p = IntReadingWaitAtoi();
        w = IntReadingWaitAtoi();
        r = IntReadingWaitAtoi();
        g = IntReadingWaitAtoi();
        b = IntReadingWaitAtoi();
        p>leds?p=0:p=p;
        r>255?r=255:r=r;
        g>255?g=255:g=g;
        b>255?b=255:b=b;
        w>255?w=255:w=w;
        strip.setPixelColor(uint16_t(p), strip.Color(r,b,w,g));
        strip.show();
        
        break;
     case 'z': //Change color to a zone of leds
     case 'Z':
        menStand = NONE;
        mensajenuevo = false;
        p = IntReadingWaitAtoi();
        pend = IntReadingWaitAtoi();
        w = IntReadingWaitAtoi();
        r = IntReadingWaitAtoi();
        g = IntReadingWaitAtoi();
        b = IntReadingWaitAtoi();
        p>leds?p=0:p=p;
        pend>leds?pend=leds:pend=pend;
        
        r>255?r=255:r=r;
        g>255?g=255:g=g;
        b>255?b=255:b=b;
        w>255?w=255:w=w;
        colorWipe (strip.Color(r,b,w,g), p, pend);
        break;
     case 'k':
     case 'K':
        Serial.println("k");
        //Type message ACK
        break;
     case 'a': //Change color to all leds
     case 'A':
        menStand = NONE;
        mensajenuevo = false;
        w = IntReadingWaitAtoi();
        r = IntReadingWaitAtoi();
        g = IntReadingWaitAtoi();
        b = IntReadingWaitAtoi();
        r>255?r=255:r=r;
        g>255?g=255:g=g;
        b>255?b=255:b=b;
        w>255?w=255:w=w;
        colorWipe (strip.Color(r,b,w,g), 0, leds);
        break;
     case 'm': //Predeterminate messages
     case 'M':
       switch(IntReadingWaitAtoi()){
         case NONE: //Clear all leds
           menStand = NONE;
           break;
         case ERR: //Red
           menStand = ERR;
           break;
         case ERRI: //Blinking Red
           menStand = ERRI;
           break;
         case OK:  //Green
           menStand = OK;
           break;
         case OKI:  //Blinking Green
           menStand = OKI;
           break;
         case WAIT:  //Blue
           menStand = WAIT;
           break;
         case WAITI:  //Blinking Blue
           menStand = WAITI;
           break;
         
       }
       mensajenuevo = true;
       break;
    }
    Serial.flush();
    return true;  
  }
  return false;
}


//Print Color from a led to other led
void colorWipe(uint32_t c, int pfirst, int pend) {
  boolean jump=false;
  if (pend < pfirst){
    jump = true;
  }
  //Serial.println(c);
  if (!jump){
    for(uint16_t i=pfirst; i<pend; i++) {
      strip.setPixelColor(i, c);
    }
  }else{
    for(uint16_t i = pfirst; i < leds; i++){
      strip.setPixelColor(i, c);
    }
    for(uint16_t i = 0; i < pend; i++){
      strip.setPixelColor(i, c);
    }
  }
  strip.show();
}

//ACK state
void ACKState(){
  if(stateACK!=ACK_WAIT){
    time = millis();
    if((time-last_time)>TIMER_ERROR_ACK){
      stateACK = ACK_ERROR;
    }
  }
  switch (stateACK){
    case ACK_WAIT:
      menStand = WAITI;
      break;
    case ACK_ERROR:
      menStand = ERRI;
      break;
  }
}

//Paint message predeterminate 
void Mensajes(){
  switch(menStand){
         case NONE:
           if(mensajenuevo){
             None();
             mensajenuevo = false;
           }
           break;
         case ERR:
           Error();
           break;
         case ERRI:
           ErrorI();
           break;
         case OK:
           Ok();
           break;
         case OKI:
           OkI();
           break;
         case WAIT:
           Wait();
           break;
         case WAITI:
           WaitI();
           break;
       }
}

void None(){
  colorWipe(BLACK,0, leds);
  strip.show();
}

void Error(){
  colorWipe(RED, 0, leds);
   strip.show();
}

void ErrorI(){
  if(intermitente){
    Error();
  }else{
    None();
  }
  intermitente = !intermitente;
  delay(500);
}

void Ok(){
  colorWipe(GREEN, 0, leds);
  strip.show();
}

void OkI(){
  if(intermitente){
    Ok();
  }else{
    None();
  }
  intermitente = !intermitente;
  delay(500);
}

void Wait(){
  colorWipe(BLUE, 0, leds);
}


void WaitI(){
  if(intermitente){
    Wait();
  }else{
    None();
  }
  intermitente = !intermitente;
  delay(500);
}
