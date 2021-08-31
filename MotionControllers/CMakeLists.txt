####
# F prime CMakeLists.txt:
#
# Including all motor controllers to make them available for the project, and thus available to choose as a driving
# platform.
####
#add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RearDriveControl/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/TwoMotorControl/")
