#ifndef __ROBOTNIK_LEDS_RC_CONTROL_H_
#define __ROBOTNIK_LEDS_RC_CONTROL_H_

#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <rcomponent/rcomponent.h>
#include <robotnik_leds/leds_effects.h>
#include <robotnik_leds/leds_value.h>
#include <robotnik_msgs/ElevatorStatus.h>
#include <std_msgs/Bool.h>
#include <string.h>

using namespace std;

class robotnik_leds_control : public rcomponent::RComponent {
   private:
    LedsEffects leds_effects;
    float min_value_activation_x, min_value_activation_y,
        min_value_activation_z;
    string topic_direction, topic_elevator, topic_estop;
    string service_led_name;

   public:
    robotnik_leds_control(ros::NodeHandle h);
    virtual ~robotnik_leds_control();

   protected:
    /* RComponent stuff */
    //! Publish freq in seconds
    float fTimePub;

    //! Setups all the ROS' stuff
    virtual int rosSetup();
    //! Shutdowns all the ROS' stuff
    virtual int rosShutdown();
    //! Reads data a publish several info into different topics
    virtual void rosPublish();
    //! Reads params from params server
    virtual void rosReadParams();

    virtual void allState();

    virtual void initState();

    virtual void standbyState();
    //! Actions performed on ready state
    virtual void readyState();
    //! Actions performed on the emergency state
    virtual void emergencyState();
    //! Actions performed on Failure state
    virtual void failureState();

    /* RComponent stuff !*/

   protected:
    /* ROS stuff */

    //! Public node handle, to receive data
    ros::NodeHandle nh_;
    //! Private node hanlde, to read params and publish data
    ros::NodeHandle pnh_;

    // Alarms
    ros::Timer timer;
    ros::ServiceClient client_led;
    ros::Subscriber direction, elevator, estop;
    ros::Time last_direction_stamp;
    ros::Time last_elevator_stamp;
    void timerPublish(const ros::TimerEvent& event);
    void directionCallback(const geometry_msgs::Twist& msg);
    void elevatorAction(const robotnik_msgs::ElevatorStatus& msg);
    void estopCallback(const std_msgs::Bool& msg);

    ROBOT_MODE::TYPES_ROBOT_MODE robot_mode, new_robot_mode;
    ROBOT_MODE::TYPES_BATTERY battery_mode;
    int not_move_msg;
    bool elevator_moving;
    bool bInitialized;
};

#endif  // __ROBOTNIK_LEDS_
