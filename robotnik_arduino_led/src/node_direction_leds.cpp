#include "ros/ros.h"
#include "robotnik_arduino_led/leds_value.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <thread>
#include <chrono>

#define NLEDS 86

#include "../include/color.h"
#include "../include/led_mode.h"
#include "../include/part.h"
using namespace std;



ros::ServiceClient client;


float min_value_activation_x, min_value_activation_y, min_value_activation_z;
int leds;
map< TYPE_PART, pair <int, int> > parts;
bool brinking;
TYPE_LED_MODE led_mode;
TYPE_COLOR led_color;
TYPE_PART direction_light;

void modeMsg (int v){
	robotnik_arduino_led::leds_value srv;
	srv.request.mode = 'm';
	srv.request.msgDefault = v;
	client.call(srv);
	
}

void initLeds (){
	robotnik_arduino_led::leds_value srv;
	srv.request.mode = 'c';
	srv.request.ledStart = leds;
	modeMsg (5);
	client.call(srv);
}

void ledsZoneOn (int ledStar, int ledEnd, TYPE_COLOR c, TYPE_LED_MODE lm){
	robotnik_arduino_led::leds_value srv;
	Color color(c);
	srv.request.mode = 'z';
	srv.request.ledStart = ledStar;
	srv.request.ledEnd = ledEnd;
	srv.request.white = color.w;
	srv.request.red = color.r;
	srv.request.green = color.g;
	srv.request.blue = color.b;
	modeMsg(0);
	client.call(srv);
	
}

int contador;

void directionCallback(const geometry_msgs::Twist& msg)
{
	contador = 0;
	float linear_x = msg.linear.x;
	float linear_y = msg.linear.y;
	float angular_z = msg.angular.z;
	
	if(abs(angular_z) >= min_value_activation_z /*&& led_mode != TYPE_LED_MODE::blinking*/){
		cout << "Giro" << endl;
		if (direction_light != TYPE_PART::left){
			modeMsg(4);		
		}
		direction_light = TYPE_PART::left;
		
	}
	else{
		
		if(abs(linear_x)>= min_value_activation_x || abs(linear_y) >= min_value_activation_y){
			cout << "Delante" << endl;
			if(direction_light != TYPE_PART::front)			
				modeMsg(3);
			direction_light = TYPE_PART::front;
		} 
		else{
			cout << "None" << endl;
			if (direction_light != TYPE_PART::none){
				modeMsg(1);
			}
			direction_light = TYPE_PART::none;
		}
	}
}

void noneSignal (){
	
}


//Falta por definir el modelo

void modeControl (const std_msgs::Int32& msg){
	switch (msg.data){
		case TYPES_ROBOT_MODE::follow:
			led_color = TYPE_COLOR::blue;
			led_mode = TYPE_LED_MODE::continous;
			break;
		case TYPES_ROBOT_MODE::manual:
			led_color = TYPE_COLOR::orange;
			led_mode = TYPE_LED_MODE::continous;
			break;
		case TYPES_ROBOT_MODE::automatic:
			led_color = TYPE_COLOR::green;
			led_mode = TYPE_LED_MODE::continous;
			break;
		case TYPES_ROBOT_MODE::interesting:
			led_color = TYPE_COLOR::white;
			led_mode = TYPE_LED_MODE::blinking;
		default:
			led_color = TYPE_COLOR::red;
			led_mode = TYPE_LED_MODE::blinking;
			break;
	}
	
}

void insertValueInPart (TYPE_PART part, int led_start,int led_end){
	pair <int, int> aux (led_start, led_end);
	parts.insert (pair <TYPE_PART, pair <int, int> > (part, aux));
}

int main (int argc, char **argv){
	float max_value_lineal_x;
	float max_value_lineal_y;
	float max_value_angular_z ;
	float porc_min_linear_x;
	float porc_min_linear_y;
	float porc_min_angular_z;
	direction_light = TYPE_PART::none;
	leds = 0;
	ros::init(argc, argv, "node_direction_led");
	ros::NodeHandle n;
	string topic_direction = "";
	string service_name;
	n.param<string>("service_name", service_name, "server_led_com");
	
	n.param<string>("topic_direction", topic_direction, "robotnik_base_control/cmd_vel");
	n.param<float>("max_value_lineal_x", max_value_lineal_x, 1.0);
	n.param<float>("max_value_lineal_y", max_value_lineal_y, 1.0);
	n.param<float>("max_value_angular_z", max_value_angular_z, 3.0);
	n.param<float>("porc_min_linear_x", porc_min_linear_x, 0.01);
	n.param<float>("porc_min_linear_y", porc_min_linear_y, 0.25);
	n.param<float>("porc_min_angular_z", porc_min_angular_z, 0.25);
	n.param<int>("leds", leds, 120);
	
	cout << "topic_direction = " << topic_direction << endl;
	cout << "max_value_lineal_x = " << max_value_lineal_x << endl;
	cout << "max_value_lineal_Y = " << max_value_lineal_y << endl;
	cout << "max_value_angular_z = " << max_value_angular_z << endl;		
	cout << "porc_min_linear_x = " << porc_min_linear_x << endl;
	cout << "porc_min_linear_y = " << porc_min_linear_y << endl;
	cout << "porc_min_angular_z = " << porc_min_angular_z << endl;


	int led_start, led_end;
	pair <int, int> values_led (0,0);
	pair <TYPE_PART, pair<int, int> > value_part;
	n.param<int>("led_start_front", led_start, 0);
	n.param<int>("led_end_front", led_end, leds/4);
	cout << "F: " << led_start << " " << led_end << endl;
	insertValueInPart (TYPE_PART::front, led_start, led_end);
	
	n.param<int>("led_start_left", led_start, leds/4);
	n.param<int>("led_end_left", led_end, 2*(leds/4));
	cout << "L: " << led_start << " " << led_end << endl;
	insertValueInPart (TYPE_PART::left, led_start, led_end);
	
	n.param<int>("led_start_back", led_start, 2*(leds/4));
	n.param<int>("led_end_back", led_end, 3*(leds/4));
	cout << "B: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::back, led_start, led_end);
	
	n.param<int>("led_start_right", led_start, 3*(leds/4));
	n.param<int>("led_end_right", led_end, leds);
	cout << "R: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::right, led_start, led_end);
	
	n.param<int>("led_start_front_left", led_start, ((7*(leds/8)-7))%leds);
	n.param<int>("led_end_front_left", led_end, (((leds/8)-7))%leds);
	cout << "F-L: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::front_left, led_start, led_end);
	
	n.param<int>("led_start_front_right", led_start, ((2*(leds/8)-7))%leds);
	n.param<int>("led_end_front_right", led_end, ((3*(leds/8)-7))%leds);
	cout << "F-R: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::front_right, led_start, led_end);
	
	n.param<int>("led_start_back_right", led_start, ((4*(leds/8)-7))%leds);
	n.param<int>("led_end_back_right", led_end, ((5*(leds/8)-7))%leds);
	cout << "B-R: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::back_right, led_start, led_end);
	
	n.param<int>("led_start_back_left", led_start, ((6*(leds/8)-7))%leds);
	n.param<int>("led_end_back_left", led_end, ((7*(leds/8)-7))%leds);
	cout << "B-L: " << led_start << " " << led_end << endl;	
	insertValueInPart (TYPE_PART::back_left, led_start, led_end);
	
	min_value_activation_x = max_value_lineal_x * porc_min_linear_x;
	min_value_activation_y = max_value_lineal_y * porc_min_linear_y;
	min_value_activation_z = max_value_angular_z * porc_min_angular_z;
	led_mode = TYPE_LED_MODE::continous;
	led_color = TYPE_COLOR::red;
	
	direction_light = TYPE_PART::none;
	contador=0;
	ros::Rate loop_rate(100);
	client = n.serviceClient<robotnik_arduino_led::leds_value>(service_name);
	ros::Subscriber sub = n.subscribe(topic_direction, 1, directionCallback);
	initLeds();
	ros::spin();
	modeMsg(0);
	return 0;
 	
}
