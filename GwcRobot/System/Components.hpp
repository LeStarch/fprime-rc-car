#ifndef __ROBOT_COMPONENTS_HEADER__
#define __ROBOT_COMPONENTS_HEADER__

#include "BaiscSystem.hpp"
#include "Project.hpp"

/**
 * Constructs our project's app. This calls through the functions defined in "BasicSystem.hpp" and "Project.hpp".
 * @param hostname: name of the ground system computer
 * @param port_number: port number of ground system connection
 */
void constructApp(char* hostname, U16 port_number);

/**
 * Called to exit the overall system.
 */
void exitTasks();

/**
 * Constructs the overall systems connections.  (Autocoded function)
 */
void constructGwcRobotArchitecture();
#endif
