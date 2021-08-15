####
# 'Rover' Deployment:
#
# This sets up the build for the 'Rover' Application, including the custom
# components. In addition, it imports FPrime.cmake, which includes the core F 
# Prime components.
#
# Author: lestarch
####

##
# Section 1: Basic Project Setup
#
# This contains the basic project information. Specifically, a cmake version and
# project definition.
##
project(Rover C CXX)
cmake_minimum_required(VERSION 3.13)

# Include the two parts of the build system. Not needing to register separate targets
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime.cmake")
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")

##
# Section 3: Components and Topology
#
# This section includes deployment specific directories. This allows use of non-
# core components in the topology, which is also added here.
##
# Add component subdirectories
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Common/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Drivers/MotorPorts/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Drivers/DcMotor/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Drivers/StepperMotor/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Drivers/Gps/")

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Control/")

# Add Topology subdirectory
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/System/")
