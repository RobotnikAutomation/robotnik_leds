
#include "ros/ros.h"
#include <iostream>
#include "../include/SerialDevice.h"
#include <robotnik_arduino_led/leds_value.h>
#include <string>
#include <unistd.h>
using namespace std;

string NameNode="service_leds_com";

bool conectadoArd;
SerialDevice * conexion;

unsigned int microseconds = 10000;


bool sendMess (char * c, int l){
	if (conectadoArd && conexion->SendMessage (c, l) < 0){
		conexion->Flush();		
		conectadoArd = false;
		return false;
	}
	else{
		conexion->Flush();
		return true;
	}
}

bool sendChar (char c){
	if (conectadoArd && conexion->SendMessage (&c, 1) < 0){
		conexion->Flush();		
		conectadoArd = false;
		return false;
	}
	else{
		conexion->Flush();
		return true;
	}
}

bool sendMess (string c){
	char *cstr = new char[c.length() + 1];
	strcpy(cstr, c.c_str());
	if (conectadoArd && conexion->SendMessage (cstr, c.length())< 0){
		conexion->Flush();		
		conectadoArd = false;
		return false;
	}else{
		conexion->Flush();
		return true;
	}
}

bool sendNum (int n){
	string s = static_cast<ostringstream*>( &(ostringstream() << n) )->str();
	s = s+'.';	
	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());
	if (conectadoArd && conexion->SendMessage (cstr, s.length())< 0){
		conexion->Flush();		
		conectadoArd = false;
		return false;
	}else{
		conexion->Flush();
		return true;
	}
}


bool serviceMsg (robotnik_arduino_led::leds_value::Request  &req, robotnik_arduino_led::leds_value::Response &res)
{
	switch(req.mode){
		case 'i':
		case 'I':
			char * msg;
			sendChar(req.mode);
			conexion->ReceiveMessage (msg);
			res.leds = atoi(msg);
			break;
		case 'c':
		case 'C':
			sendChar(req.mode);
			usleep(microseconds);
			sendNum(req.ledStart);
			usleep(microseconds);
			break;
		case 'p':
		case 'P':
			sendChar(req.mode);
			usleep(microseconds/2);
			sendNum(req.ledStart);
			usleep(microseconds/2);
			sendNum(req.white);
			usleep(microseconds/2);
			sendNum(req.red);
			usleep(microseconds/2);
			sendNum(req.green);
			usleep(microseconds/2);
			sendNum(req.blue);
			break;
		case 'z':
		case 'Z':
			sendChar(req.mode);
			usleep(microseconds/2);
			sendNum(req.ledStart);
			usleep(microseconds/2);
			sendNum(req.ledEnd);
			usleep(microseconds/2);
			sendNum(req.white);
			usleep(microseconds/2);
			sendNum(req.red);
			usleep(microseconds/2);
			sendNum(req.green);
			usleep(microseconds/2);
			sendNum(req.blue);
			usleep(microseconds);
			break;
		case 'm':
		case 'M':
			sendChar(req.mode);
			usleep(microseconds);
			sendNum(req.msgDefault);
			usleep(microseconds);
			break;
		default:
			res.msg = "Option not disponse";
			break;
	}
	if (!conectadoArd){
		res.msg = "Not is possible connect with the leds";
		res.error = true;
	}else{
		res.error = false;
	}
	return true;
}



int main (int argc, char **argv){
	conectadoArd = false;
	ros::init(argc,argv,NameNode);
	ros::NodeHandle n;
	cout << "Iniciando servicio de conexion con Arduino Leds" << endl;
	string path_device;
	string service_name;
	n.param<string>("path_device", path_device, "/dev/ttyUSB_LEDS");
	n.param<string>("service_name", service_name, "server_led_com");
	ROS_INFO_STREAM (path_device);
	conexion = new SerialDevice(path_device.c_str(),9600,"odd",8);
	
	if (conexion->OpenPort1()){
		delete conexion;
		ROS_ERROR_STREAM ("arduino_leds::node_LedsCom : The device is not connect to " + path_device);
		return -1;
	}
	conectadoArd = true;
	ros::Rate loop_rate(10);
	ros::ServiceServer service = n.advertiseService(service_name, serviceMsg);
	ros::spin();
}





