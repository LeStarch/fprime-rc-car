#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>

#include <GwcRobot/System/Components.hpp>

void print_usage() {
    (void) printf("Usage: ./Rover [options]\n-p\tport_number\n-a\thostname/IP address\n");
}

#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t terminate = 0;

static void sighandler(int signum) {
    exitTasks();
    terminate = 1;
}

int main(int argc, char* argv[]) {
    U32 port_number;
    I32 option;
    char *hostname;
    port_number = 0;
    option = 0;
    hostname = NULL;

    while ((option = getopt(argc, argv, "hp:a:")) != -1){
        switch(option) {
            case 'h':
                print_usage();
                return 0;
                break;
            case 'p':
                port_number = atoi(optarg);
                break;
            case 'a':
                hostname = optarg;
                break;
            case '?':
                return 1;
            default:
                print_usage();
                return 1;
        }
    }

    (void) printf("Hit Ctrl-C to quit\n");

    constructApp(hostname, port_number);

    // register signal handlers to exit program
    signal(SIGINT,sighandler);
    signal(SIGTERM,sighandler);

    linuxTimer.startTimer(10); //!< 100Hz

    // Give time for threads to exit
    (void) printf("Waiting for threads...\n");
    Os::Task::delay(1000);

    (void) printf("Exiting...\n");

    return 0;
}
