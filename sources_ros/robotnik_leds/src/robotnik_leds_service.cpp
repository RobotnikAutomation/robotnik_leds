#include <robotnik_leds/robotnik_leds_rc_service.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "robotnik_leds_service");
    ros::NodeHandle n;

    robotnik_led_service node(n);
    node.start();
}
