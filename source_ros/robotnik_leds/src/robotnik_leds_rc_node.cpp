#include <robotnik_leds/robotnik_leds_rc_node.h>

robotnik_led_string::robotnik_led_string(ros::NodeHandle h) : RComponent(h), nh_(h), pnh_("~"){
    component_name.assign(pnh_.getNamespace());    
    rosReadParams();
    robot_mode = ROBOT_MODE::TYPES_ROBOT_MODE::NotLight;
    new_robot_mode = ROBOT_MODE::TYPES_ROBOT_MODE::Ready;
    not_move_msg=0;
    elevator_moving = false;
}

robotnik_led_string::~robotnik_led_string(){
}

int robotnik_led_string::rosSetup(){
    RComponent::rosSetup();
    //RobotStatService   =pnh_.advertiseService("get_robot_status",&robotnik_leds::get_robot_statusCB,this);
    //RobotGoToService   =pnh_.advertiseService("go_to",&robotnik_leds::go_toCB,this);

    timer  = pnh_.createTimer(ros::Duration(0.00001), &robotnik_led_string::timerPublish, this);
    client_led = nh_.serviceClient<robotnik_leds::leds_value>(service_led_name);
    direction = nh_.subscribe(topic_direction, 1, &robotnik_led_string::directionCallback, this);
    elevator = nh_.subscribe(topic_elevator, 1, &robotnik_led_string::elevatorAction, this);
    estop = nh_.subscribe(topic_estop, 1, &robotnik_led_string::estopCallback, this);
}

bool isDrivingMode(ROBOT_MODE::TYPES_ROBOT_MODE mode)
{
    return mode == TYPES_ROBOT_MODE::FrontDriving or
    mode ==  TYPES_ROBOT_MODE::FrontDriving or
 mode ==        TYPES_ROBOT_MODE::BackDriving or
mode ==         TYPES_ROBOT_MODE::FrontLeftDriving or
mode ==         TYPES_ROBOT_MODE::FrontRightDriving or
mode ==         TYPES_ROBOT_MODE::BackLeftDriving or
mode ==         TYPES_ROBOT_MODE::BackRightDriving or
mode ==         TYPES_ROBOT_MODE::LeftTurning or
mode ==         TYPES_ROBOT_MODE::RightTurning or
mode ==         TYPES_ROBOT_MODE::Turning;
}

void robotnik_led_string::timerPublish(const ros::TimerEvent& event){
    //rosPublish();
//    not_move_msg = (not_move_msg +1 ) % UINT16_MAX;
//    if(not_move_msg > 5){
//        new_robot_mode=TYPES_ROBOT_MODE::NotLight;
//    }

static ros::Time last_change_mode_stamp = ros::Time::now();


//   if (isDrivingMode(robot_mode))
//   {
//      if (ros::Time::now() - last_direction_stamp > ros::Duration(1))
//      {
//          new_robot_mode = TYPES_ROBOT_MODE::Ready;
//    RCOMPONENT_WARN_STREAM("Direction timeout");
//         } 
//   }
    
    RCOMPONENT_INFO_STREAM_THROTTLE(1, "Current mode " << robot_mode);
    if (new_robot_mode != robot_mode and ros::Time::now() - last_change_mode_stamp > ros::Duration(1)){
last_change_mode_stamp = ros::Time::now();
        RCOMPONENT_INFO_STREAM("Switch mode from " << robot_mode << " to " << new_robot_mode);
        leds_effects.clear();
        switch (new_robot_mode) {
        case TYPES_ROBOT_MODE::NotLight:
            break;
        case TYPES_ROBOT_MODE::Estop:
                leds_effects.inserEffect(TYPE_PART::all, TYPE_LED_MODE::continous, TYPE_COLOR::red);
break;
        case TYPES_ROBOT_MODE::Ready:
            if (battery_mode == ROBOT_MODE::TYPES_BATTERY::Charged)
            {
                leds_effects.inserEffect(TYPE_PART::all, TYPE_LED_MODE::continous, TYPE_COLOR::green);
            }
            else if (battery_mode == ROBOT_MODE::TYPES_BATTERY::Low)
            {
                leds_effects.inserEffect(TYPE_PART::all, TYPE_LED_MODE::continous, TYPE_COLOR::yellow);
            }
            break;
        case TYPES_ROBOT_MODE::FrontDriving:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            //leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::continous, TYPE_COLOR::black);
            break;
        case TYPES_ROBOT_MODE::BackDriving:
            //leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::continous, TYPE_COLOR::black);
            break;
        case TYPES_ROBOT_MODE::FrontLeftDriving:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            //leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::continous, TYPE_COLOR::black);
            break;
        case TYPES_ROBOT_MODE::FrontRightDriving:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            //leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            break;
        case TYPES_ROBOT_MODE::BackLeftDriving:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            //leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            break;
        case TYPES_ROBOT_MODE::BackRightDriving:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            //leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::green);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::red);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::continous, TYPE_COLOR::black);
            break;

        case TYPES_ROBOT_MODE::LeftTurning:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::continous, TYPE_COLOR::black);
            break;
        case TYPES_ROBOT_MODE::RightTurning:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            break;
        case TYPES_ROBOT_MODE::Turning:
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::continous, TYPE_COLOR::black);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::blinking, TYPE_COLOR::yellow);
            break;
        case TYPES_ROBOT_MODE::Lifting:
            leds_effects.inserEffect(TYPE_PART::all, TYPE_LED_MODE::blinking, TYPE_COLOR::black, TYPE_COLOR::green);
            /*
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            */
            break;

        case TYPES_ROBOT_MODE::Lowering:
            leds_effects.inserEffect(TYPE_PART::all, TYPE_LED_MODE::blinking, TYPE_COLOR::black, TYPE_COLOR::red);
            /*
            leds_effects.inserEffect(TYPE_PART::front,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::back,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::left,TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            leds_effects.inserEffect(TYPE_PART::right, TYPE_LED_MODE::roundInv, TYPE_COLOR::white);
            */
            break;



        default:
            break;
        }
        robot_mode = new_robot_mode;

    }
    vector <robotnik_leds::leds_value> send = leds_effects.paint();

    for (int cont=0; cont < send.size(); cont++){
        client_led.call(send[cont]);
    }
}

void robotnik_led_string::estopCallback (const std_msgs::Bool& msg){
if (msg.data == true)
new_robot_mode = TYPES_ROBOT_MODE::Estop;
else if (robot_mode == TYPES_ROBOT_MODE::Estop)
new_robot_mode = TYPES_ROBOT_MODE::Ready;
}

void robotnik_led_string::directionCallback (const geometry_msgs::Twist& msg){
//    if (msg.linear.x == 0 and msg.angular.z == 0)
//return;
//
//    last_direction_stamp = ros::Time::now();
    if (robot_mode == TYPES_ROBOT_MODE::Lifting or robot_mode == TYPES_ROBOT_MODE::Lowering or robot_mode == TYPES_ROBOT_MODE::Estop or robot_mode == TYPES_ROBOT_MODE::NeedReset) {
        return;
    }
    if(abs(msg.linear.x)>min_value_activation_x){
        if (msg.linear.x<0){
                if (msg.angular.z>min_value_activation_z){
                    new_robot_mode=TYPES_ROBOT_MODE::BackRightDriving;
                }else if (abs(msg.angular.z)>min_value_activation_z){
                    new_robot_mode=TYPES_ROBOT_MODE::BackLeftDriving;
                }else{
                    new_robot_mode=TYPES_ROBOT_MODE::BackDriving;
                }
        }else{
            if (msg.angular.z>min_value_activation_z){
                new_robot_mode=TYPES_ROBOT_MODE::FrontRightDriving;
            }else if (abs(msg.angular.z)>min_value_activation_z){
                new_robot_mode=TYPES_ROBOT_MODE::FrontLeftDriving;
            }else{
                new_robot_mode=TYPES_ROBOT_MODE::FrontDriving;
            }
        }
    }
    else if(abs(msg.angular.z)>min_value_activation_z){
        new_robot_mode = TYPES_ROBOT_MODE::Turning;
        //if(msg.angular.z<0){
        //    new_robot_mode = TYPES_ROBOT_MODE::LeftTurning;
        //}else{
        //    new_robot_mode = TYPES_ROBOT_MODE::RightTurning;
        //}
   }
   else
   {
        new_robot_mode = TYPES_ROBOT_MODE::Ready;

   }
}

void robotnik_led_string::elevatorAction (const robotnik_msgs::ElevatorStatus &msg){
    last_elevator_stamp = ros::Time::now();

// this should be implemented, but as right now the leds seem a disco each time they are on...
//   if (robot_mode == TYPES_ROBOT_MODE::Estop or robot_mode == TYPES_ROBOT_MODE::NeedReset) {
//        return;
//    }
//   if (isDrivingMode(robot_mode))
//    if(msg.state != robotnik_msgs::ElevatorStatus::IDLE){
//        if(msg.state == robotnik_msgs::ElevatorStatus::LOWERING){
//            new_robot_mode = TYPES_ROBOT_MODE::Lowering;
//        }else if (msg.state == robotnik_msgs::ElevatorStatus::RAISING){
//            new_robot_mode = TYPES_ROBOT_MODE::Lifting;
//        }
//    }
//    else { 
//      new_robot_mode = TYPES_ROBOT_MODE::Ready;
//    }
}

int robotnik_led_string::rosShutdown(){
    RComponent::rosShutdown();
}

void robotnik_led_string::rosPublish(){

    RComponent::rosPublish();    
}

void robotnik_led_string::rosReadParams(){
    // RComponent::rosReadParams(); //not need to call it because it is called by the constructor of RComponent    
    //pnh_.param<std::string>("robot_base_frame", robot_base_frame_, "base_footprint");
    float max_value_lineal_x;
    float max_value_lineal_y;
    float max_value_angular_z ;
    float porc_min_linear_x;
    float porc_min_linear_y;
    float porc_min_angular_z;
    int leds;
    pnh_.param<string>("service_name", service_led_name, "server_led_com");

    pnh_.param<string>("topic_direction", topic_direction, "robotnik_base_control/cmd_vel");
    pnh_.param<string>("topic_elevator", topic_elevator, "robotnik_base_control/elevator_status");
    pnh_.param<string>("topic_estop", topic_estop, "robotnik_base_hw/emergency_stop");
    pnh_.param<float>("max_value_lineal_x", max_value_lineal_x, 1.0);
    pnh_.param<float>("max_value_lineal_y", max_value_lineal_y, 1.0);
    pnh_.param<float>("max_value_angular_z", max_value_angular_z, 3.0);
    pnh_.param<float>("porc_min_linear_x", porc_min_linear_x, 0.01);
    pnh_.param<float>("porc_min_linear_y", porc_min_linear_y, 0.25);
    pnh_.param<float>("porc_min_angular_z", porc_min_angular_z, 0.25);
    min_value_activation_x = 0; //max_value_lineal_x * porc_min_linear_x;
    min_value_activation_y = 0; //max_value_lineal_y * porc_min_linear_y;
    min_value_activation_z = 0; //max_value_angular_z * porc_min_angular_z;

    pnh_.param<int>("leds", leds, 120);
    leds_effects= LedsEffects(leds, 5);

    string msgRos = "Params: \n service_led_name - " + service_led_name + "\n topic_direction - " + topic_direction
            +"\n max_val_lineal_x - " +to_string(max_value_lineal_x) + "\tporc_min_linear_x - " + to_string(porc_min_linear_x)
            +"\n max_val_lineal_y - " +to_string(max_value_lineal_y) + "\tporc_min_linear_y - " + to_string(porc_min_linear_y)
            +"\n max_val_angular_z - " +to_string(max_value_angular_z) + "\tporc_min_angular_z - " + to_string(porc_min_angular_z)
            +"\n leds - "+ to_string(leds) ;
    ROS_INFO_STREAM(msgRos);
    int led_start, led_end;

    pnh_.param<int>("led_start_front", led_start, 0);
    pnh_.param<int>("led_end_front", led_end, leds/4);
    cout << "F: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::front, led_start, led_end, TYPE_COLOR::green);


    pnh_.param<int>("led_start_left", led_start, leds/4);
    pnh_.param<int>("led_end_left", led_end, 2*(leds/4));
    cout << "L: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::left, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_back", led_start, 2*(leds/4));
    pnh_.param<int>("led_end_back", led_end, 3*(leds/4));
    cout << "B: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::back, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_right", led_start, 3*(leds/4));
    pnh_.param<int>("led_end_right", led_end, leds);
    cout << "R: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::right, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_front_left", led_start, ((7*(leds/8)-7))%leds);
    pnh_.param<int>("led_end_front_left", led_end, (((leds/8)-7))%leds);
    cout << "F-L: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::front_left, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_front_right", led_start, ((2*(leds/8)-7))%leds);
    pnh_.param<int>("led_end_front_right", led_end, ((3*(leds/8)-7))%leds);
    cout << "F-R: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::front_right, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_back_right", led_start, ((4*(leds/8)-7))%leds);
    pnh_.param<int>("led_end_back_right", led_end, ((5*(leds/8)-7))%leds);
    cout << "B-R: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::back_right, led_start, led_end, TYPE_COLOR::green);

    pnh_.param<int>("led_start_back_left", led_start, ((6*(leds/8)-7))%leds);
    pnh_.param<int>("led_end_back_left", led_end, ((7*(leds/8)-7))%leds);
    cout << "B-L: " << led_start << " " << led_end << endl;
    leds_effects.insertPart(TYPE_PART::back_left, led_start, led_end, TYPE_COLOR::green);

    //leds_effects.inserEffect(TYPE_PART::front, TYPE_LED_MODE::blinking, TYPE_COLOR::white);

    //leds_effects.disableAllEffect();

}

void robotnik_led_string::standbyState(){
    if (bInitialized) switchToState(robotnik_msgs::State::READY_STATE);
    else switchToState(robotnik_msgs::State::INIT_STATE);
}


void robotnik_led_string::initState(){
    // OPEN PORT ......
    RComponent::initState();
    //leds_effects.inserEffect(TYPE_LED_MODE.blinking, TYPE_PART.all, TYPE_COLOR.red, TYPE_COLOR.green);

    switchToState(robotnik_msgs::State::READY_STATE);
}


void robotnik_led_string::allState(){

}

void robotnik_led_string::readyState(){


}

void robotnik_led_string::emergencyState(){
}

void robotnik_led_string::failureState(){
}





