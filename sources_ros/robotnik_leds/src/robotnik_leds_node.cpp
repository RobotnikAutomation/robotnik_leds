#include <robotnik_leds/robotnik_leds_rc_node.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "robotnik_leds_node");
    ros::NodeHandle n;

    robotnik_led_string node(n);
    node.start();
}
