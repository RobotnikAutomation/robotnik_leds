cmake_minimum_required(VERSION 2.8.3)
project(robotnik_leds)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
    message_generation
    rcomponent
    std_msgs
    robotnik_msgs
    roscpp
)

add_service_files(
   FILES
   leds_value.srv
)

generate_messages(

)

catkin_package(
    CATKIN_DEPENDS  message_runtime roscpp std_msgs
#  INCLUDE_DIRS include
#  LIBRARIES robotnik_leds
#  CATKIN_DEPENDS rcomponent std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(robotnik_leds_node
                    src/robotnik_leds_node.cpp
                    src/robotnik_leds_rc_node.cpp
                    src/leds_effects.cpp
                    src/color.cpp
                    src/leds_part.cpp
                    include/robotnik_leds/robotnik_leds_rc_node.h
                    include/robotnik_leds/color.h
                    include/robotnik_leds/led_mode.h
                    include/robotnik_leds/part.h
                    include/robotnik_leds/leds_part.h
                    include/robotnik_leds/leds_effects.h

)

add_executable(robotnik_leds_service
                    src/robotnik_leds_service.cpp
                    src/robotnik_leds_rc_service.cpp
                    src/SerialDevice.cc
                    include/robotnik_leds/robotnik_leds_rc_service.h
                    include/robotnik_leds/SerialDevice.h

)


## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(robotnik_leds_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
                 robotnik_msgs_generate_messages_cpp)

add_dependencies(robotnik_leds_service ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
                 robotnik_msgs_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(
  robotnik_leds_node
  ${catkin_LIBRARIES}
)

target_link_libraries(
  robotnik_leds_service
  ${catkin_LIBRARIES}
)



