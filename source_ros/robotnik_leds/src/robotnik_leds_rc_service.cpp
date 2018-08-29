#include <robotnik_leds/robotnik_leds_rc_service.h>

robotnik_led_service::robotnik_led_service(ros::NodeHandle h) : RComponent(h), nh_(h), pnh_("~"){
    component_name.assign(pnh_.getNamespace());
    conexion = NULL;
    connected = false;
    try_connect = 0;
    rosReadParams();
    sendMsg = false;
}

robotnik_led_service::~robotnik_led_service(){
    if(conexion!=NULL){
        sendMess("m6.");
        delete conexion;
    }
}

int robotnik_led_service::rosSetup(){
    RComponent::rosSetup();
    //RobotStatService   =pnh_.advertiseService("get_robot_status",&robotnik_leds::get_robot_statusCB,this);
    //RobotGoToService   =pnh_.advertiseService("go_to",&robotnik_leds::go_toCB,this);
    ROS_WARN_STREAM("Service start");
    service_led = pnh_.advertiseService(service_led_name, &robotnik_led_service::serviceMsg, this);
	timer = pnh_.createTimer(ros::Duration(1), &robotnik_led_service::timerPublish, this);
}

void robotnik_led_service::messageACK(){
	if (connected){
		sendMess("k");
		//sendMess("m1.");
	}
}

void robotnik_led_service::timerPublish(const ros::TimerEvent& event){
	if(!sendMsg){
		messageACK();
	}
    //rosPublish();
}

bool robotnik_led_service::serviceMsg (robotnik_leds::leds_value::Request  &req, robotnik_leds::leds_value::Response &res){

    if(!connected){
        ROS_ERROR_STREAM ("The device is not connect to " + path_device);
        switchToState(robotnik_msgs::State::FAILURE_STATE);
        return true;
    }
    sendMsg = true;
    //conexion->Flush();
     //ROS_INFO_STREAM("----Recived one msg ---");
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
            if (!sendChar(req.mode)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.ledStart)){
                    ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                    switchToState(robotnik_msgs::State::FAILURE_STATE);
                    return true;
            }

            usleep(microseconds);
            break;
        case 'p':
        case 'P':
            if (!sendChar(req.mode)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.ledStart)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.white)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.red)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.green)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.blue)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds);
            break;
        case 'z':
        case 'Z':

            if(!sendChar(req.mode)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.ledStart)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }

            usleep(microseconds/2);
            if (!sendNum(req.ledEnd)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.white)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.red)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.green)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.blue)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds);
            break;
        case 'm':
        case 'M':
            if (!sendChar(req.mode)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds/2);
            if (!sendNum(req.msgDefault)){
                ROS_ERROR_STREAM ("The device is not connect to " + path_device);
                switchToState(robotnik_msgs::State::FAILURE_STATE);
                return true;
            }
            usleep(microseconds);
            break;
        default:
            res.msg = "Option not disponse";
            break;
    }
    
    sendMsg = false;
    //conexion->Flush();
    if (!connected){
        res.msg = "Not is possible connect with the leds";
        res.error = true;
        res.paint = false;
        ROS_ERROR_STREAM ("The device is not connect to " + path_device);
        switchToState(robotnik_msgs::State::FAILURE_STATE);
        return true;

    }else{
        res.msg = "Succeful";
        res.error = false;
        res.paint = true;
    }
    return true;
}


int robotnik_led_service::rosShutdown(){
    RComponent::rosShutdown();
}

void robotnik_led_service::rosPublish(){

    RComponent::rosPublish();    
}

void robotnik_led_service::rosReadParams(){
    ROS_WARN_STREAM("Read param");
    // RComponent::rosReadParams(); //not need to call it because it is called by the constructor of RComponent    
    //pnh_.param<std::string>("robot_base_frame", robot_base_frame_, "base_footprint");
    pnh_.param<string>("path_device", path_device, "/dev/ttyUSB0");
    pnh_.param<string>("service_name", service_led_name, "server_led_com");
    ROS_INFO_STREAM("Params:\n path_device - " + path_device + "\n service_name - "+service_led_name);

}

void robotnik_led_service::standbyState(){
    ROS_WARN_STREAM("Service in StandbyState");

    if (bInitialized) switchToState(robotnik_msgs::State::READY_STATE);
    else switchToState(robotnik_msgs::State::INIT_STATE);
}


void robotnik_led_service::initState(){
    RComponent::initState();
    ROS_WARN_STREAM("InitState");
    conexion = new SerialDevice(path_device.c_str(),9600,"none",8);
    if (conexion->OpenPort1()){
        delete conexion;
        conexion = NULL;
        connected = false;
        ROS_ERROR_STREAM ("In try create conexion: The device is not connect to " + path_device);
        switchToState(robotnik_msgs::State::FAILURE_STATE);
        try_connect ++;
    }
    try_connect = 0;
    connected = true;
    if(!sendMess("......")){
        ROS_ERROR_STREAM ("The device is not connect to " + path_device);
        switchToState(robotnik_msgs::State::FAILURE_STATE);
    }
    //leds_effects.inserEffect(TYPE_LED_MODE.blinking, TYPE_PART.all, TYPE_COLOR.red, TYPE_COLOR.green);

    switchToState(robotnik_msgs::State::READY_STATE);
}


void robotnik_led_service::allState(){

}

void robotnik_led_service::readyState(){


}

void robotnik_led_service::emergencyState(){
}

void robotnik_led_service::failureState(){
    if (try_connect > 10){
        ROS_ERROR_STREAM("ROBOTNIK_LED_SERVICE FATAL ERROR - It is impossible to connect with the device LED "+ path_device+ " ---> SHUTDOWN");
        switchToState(robotnik_msgs::State::SHUTDOWN_STATE);
    }
    usleep(5000000);
    ROS_WARN_STREAM("Try connect to device: " + to_string(try_connect) + " - 10");
    switchToState(robotnik_msgs::State::STANDBY_STATE);
}



bool robotnik_led_service::sendMess (char * c, int l){
    if(conexion==NULL || !connected){
        return false;
    }
    //conexion->Flush();
    if (conexion->SendMessage (c, l) < 0){
        connected = false;
        return false;
    }
    else{
        return true;
    }
}

bool robotnik_led_service::sendChar (char c){
    if(conexion==NULL || !connected){
        return false;
    }
    if (conexion->SendMessage (&c, 1) < 0){
        connected = false;
        return false;
    }
    else{
        return true;
    }
}

bool robotnik_led_service::sendMess (string c){
    if(conexion==NULL || !connected){
        return false;
    }
    char *cstr = new char[c.length() + 1];
    strcpy(cstr, c.c_str());
    int nmsg = conexion->SendMessage (cstr, c.length());
    if (nmsg< 0){
        connected = false;
        return false;
    }else{
        return true;
    }

}

bool robotnik_led_service::sendNum (int n){
    if(conexion==NULL || !connected){
        return false;
    }
    string s = to_string(n)+".";
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    if (conexion->SendMessage (cstr, s.length())< 0){
        connected = false;
        return false;
    }else{
        return true;
    }
}



