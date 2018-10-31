#include <robotnik_leds/robotnik_leds_rc_control.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "robotnik_leds_control");
    ros::NodeHandle n;

    robotnik_leds_control node(n);
    node.start();
}
