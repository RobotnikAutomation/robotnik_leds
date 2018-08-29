#ifndef LEDMODE__H_
#define LEDMODE__H_

#define LED_kNIGHTRIDER 5
#define LED_ROUND 5


namespace LED_MODE
{
	enum TYPES_LEDS_MODE{
		none = 0,
		continous=1,
		blinking=2,
        round=3,
        roundInv = 4,
        knightRider=5,
        doubleKnightRider=6,
        cumulative=7,
        cumulativeInv = 8,
        cumulativeCenter = 9,
        cumulativeCenterInv = 10
	};
}
typedef LED_MODE::TYPES_LEDS_MODE TYPE_LED_MODE;


namespace ROBOT_MODE
{
	enum TYPES_ROBOT_MODE{
        NotLight = 0,
        FrontDriving =1,
        BackDriving = 2,
        FrontLeftDriving =3,
        FrontRightDriving = 4,
        BackLeftDriving =5,
        BackRightDriving = 6,
        LeftTurning = 7,
        RightTurning = 8,
        Turning,
        Lifting,
        Lowering,
        Ready,
        Estop,
        InMission,
        NeedReset,
	};
        enum TYPES_BATTERY {
        Charged,
        Low,
        Charging
        };
}
typedef ROBOT_MODE::TYPES_ROBOT_MODE TYPES_ROBOT_MODE;


#endif
