#ifndef LEDMODE__H
#define LEDMODE__H

namespace LED_MODE
{
	enum TYPES_LEDS_MODE{
		none = 0,
		continous=1,
		blinking=2,
		round=3
	};
}
typedef LED_MODE::TYPES_LEDS_MODE TYPE_LED_MODE;


namespace ROBOT_MODE
{
	enum TYPES_ROBOT_MODE{
		follow = 0,
		automatic =1,
		manual =2,
		interesting = 3,
		danger = 4
	};
}
typedef ROBOT_MODE::TYPES_ROBOT_MODE TYPES_ROBOT_MODE;


#endif