#include <Components.hpp>

void constructApp(char* hostname, U16 port_number) {
    // Initialize basic and project components
    init_basic_system();
    init_project();

    constructGwcRobotArchitecture();

    // Register commands from basic and project
    register_basic_system();
    register_project();

    // Starts threads for the basic system, and for project
    start_basic_system();
    start_project();

    // Communications begin and the system is running!
    start_communications(hostname, port_number);
}

void exitTasks() {
    stop_basic_system();
    stop_project();
}

