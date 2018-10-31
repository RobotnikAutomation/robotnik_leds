#include <robotnik_leds/robotnik_leds_rc_driver.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "robotnik_leds_driver");
    ros::NodeHandle n;

    robotnik_leds_driver node(n);
    node.start();
}
