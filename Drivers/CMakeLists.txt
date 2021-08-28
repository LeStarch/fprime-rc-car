####
# F prime CMakeLists.txt:
#
# Including all drivers to make them available to the project
####
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/MotorPorts/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/DcMotor/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/StepperMotor/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/PCA9685/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Gps/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Imaging/")
