#ifndef __ROBOTNIK_LEDS_RC_SERVICE_H_
#define __ROBOTNIK_LEDS_RC_SERVICE_H_

#include <ros/ros.h>

#include <rcomponent/rcomponent.h>
#include <robotnik_leds/SerialDevice.h>
#include <robotnik_leds/leds_value.h>
#include <string.h>
#include <unistd.h>

using namespace std;

class robotnik_led_service : public rcomponent::RComponent
{

private:



public:
  robotnik_led_service(ros::NodeHandle h);
  virtual ~robotnik_led_service();

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

  string path_device;
  string service_led_name;
  unsigned int microseconds = 9000;
  bool connected;
  int try_connect;
  bool sendMsg;


  //! Public node handle, to receive data
  ros::NodeHandle nh_;
  //! Private node hanlde, to read params and publish data
  ros::NodeHandle pnh_; 

  //Alarms
  ros::Timer timer;
  ros::ServiceServer service_led;
  SerialDevice * conexion;
  
  void messageACK();
  void timerPublish(const ros::TimerEvent& event);
  bool serviceMsg (robotnik_leds::leds_value::Request  &req, robotnik_leds::leds_value::Response &res);
  bool sendMess (char * c, int l);
  bool sendChar (char c);
  bool sendMess (string c);
  bool sendNum (int n);

  bool bInitialized;
};

#endif  // __ROBOTNIK_LEDS_
