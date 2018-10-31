#ifndef COLOR__H_
#define COLOR__H_

enum TYPE_COLOR
{
  red = 0,
  green = 1,
  blue = 2,
  white = 3,
  black = 4,
  orange = 5,
  yellow = 6
};

struct Color
{
  int w, r, g, b;
  Color();

  Color(int _w, int _r, int _g, int _b);

  Color(TYPE_COLOR tc);

  void setColor(int _w, int _r, int _g, int _b);

  void setColor(TYPE_COLOR tc);

  void setRed();

  void setGreen();

  void setBlue();

  void setWhite();

  void setBlack();

  void setOrange();

  void setYellow();
};

#endif
