/**
 * BasicSystem.hpp:
 *
 * This header file defines a basic setup for the spacecraft "bus"'s software. This means that all of the software
 * needed to control the spacecraft/embedded system in a standard way (command uplink, telemetry downlink).
 *
 * This does not contain any setup for the project-specific software.  That is done in "Project.hpp".  The reason for
 * this separation is simple: 90% of fprime systems use the same setup seen here, and it keeps the project setup very
 * simple.
 *
 * Author: LeStarch
 */
#ifndef __ROBOT_BASIC_SYSTEM_HPP__
#define __ROBOT_BASIC_SYSTEM_HPP__

#include <Svc/ActiveRateGroup/ActiveRateGroupImpl.hpp>
#include <Svc/RateGroupDriver/RateGroupDriverImpl.hpp>
#include <Svc/CmdDispatcher/CommandDispatcherImpl.hpp>
#include <Svc/CmdSequencer/CmdSequencerImpl.hpp>
#include <Svc/PassiveConsoleTextLogger/ConsoleTextLoggerImpl.hpp>
#include <Svc/ActiveLogger/ActiveLoggerImpl.hpp>
#include <Svc/LinuxTime/LinuxTimeImpl.hpp>
#include <Svc/LinuxTimer/LinuxTimerComponentImpl.hpp>
#include <Svc/TlmChan/TlmChanImpl.hpp>
#include <Svc/PrmDb/PrmDbImpl.hpp>
#include <Fw/Obj/SimpleObjRegistry.hpp>
#include <Svc/FileUplink/FileUplink.hpp>
#include <Svc/FileDownlink/FileDownlink.hpp>
#include <Svc/BufferManager/BufferManagerComponentImpl.hpp>
#include <Svc/Health/HealthComponentImpl.hpp>
#include <Svc/StaticMemory/StaticMemoryComponentImpl.hpp>
#include <Svc/Framer/FramerComponentImpl.hpp>
#include <Svc/Deframer/DeframerComponentImpl.hpp>
#include <Drv/TcpClient/TcpClientComponentImpl.hpp>
#include <Svc/AssertFatalAdapter/AssertFatalAdapterComponentImpl.hpp>
#include <Svc/FatalHandler/FatalHandlerComponentImpl.hpp>

// Drivers for the basic system
#include <Drv/TcpClient/TcpClientComponentImpl.hpp>
#include <Drv/Udp/UdpComponentImpl.hpp>

// Clock setup
static U32 RATE_10HZ_DIVISOR = 1; // 10Hz input clock divided by 1 becomes a 10Hz clock
static U32 RATE_1HZ_DIVISOR = 10; // 10Hz input clock divided by 10 becomes a 1Hz clock
static NATIVE_INT_TYPE RATE_GROUP_DIVISORS[] = {RATE_10HZ_DIVISOR, RATE_1HZ_DIVISOR};

// Clock context setup
static const U32 RATE_10HZ_CONTEXT = 10;
static const U32 RATE_1HZ_CONTEXT = 1;
static NATIVE_UINT_TYPE RG_10HZ_CONTEXTS[] = {RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT, RATE_10HZ_CONTEXT};
static NATIVE_UINT_TYPE RG_1HZ_CONTEXTS[] = {RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT, RATE_1HZ_CONTEXT};

enum SystemSettings {
    RATE_10HZ_QUEUE_DEPTH = 10,
    RATE_1HZ_QUEUE_DEPTH = 10,
    PARAM_QUEUE_DEPTH = 10,
    EVENT_QUEUE_DEPTH = 10,
    CHAN_QUEUE_DEPTH = 10,
    CMD_DISP_QUEUE_DEPTH = 20,
    CMD_SEQ_QUEUE_DEPTH = 10,
    FILE_UP_QUEUE_DEPTH = 30,
    FILE_DWN_QUEUE_DEPTH = 30,
    HEALTH_QUEUE_DEPTH = 25,

    // Thread priorities, 255 being the highest, and 0 being lowest
    RATE_10HZ_PRIORITY = 250,
    RATE_1HZ_PRIORITY = 249,
    COMM_PRIORITY = 231,
    CMD_DISP_PRIORITY = 230,
    CMD_SEQ_PRIORITY = 229,
    PARAM_PRIORITY = 220,
    EVENT_PRIORITY = 219,
    CHAN_PRIORITY = 218,
    FILE_UP_PRIORITY = 210,
    FILE_DOWN_PRIORITY = 209,

    THREAD_STACK_SIZE = 10 * 1024,
    CMD_SEQ_MEMORY_ALLOCATION = 50 * 1024,
};


/**
 * Initializes the basic system components. This is a required step to get that system running.
 */
void init_basic_system();

/**
 * Registers commands defined in the standard system.
 */
void register_basic_system();

/**
 * Start the active components (threads) in the standard system.
 */
void start_basic_system();

/**
 * Start the communications with the ground system
 * @param hostname: name of the ground system computer
 * @param port_number: port number of ground system connection
 */
void start_communications(char* hostname, U16 port_number);

/**
 * Stop the active components (threads) in the standard system.
 */
void stop_basic_system();

// Component definitions
extern Svc::RateGroupDriverImpl rateGroupDriverComp;
extern Svc::ActiveRateGroupImpl rateGroup10HzComp;
extern Svc::ActiveRateGroupImpl rateGroup1HzComp;
extern Svc::CmdSequencerComponentImpl cmdSeq;
extern Svc::ConsoleTextLoggerImpl textLogger;
extern Svc::ActiveLoggerImpl eventLogger;
extern Svc::LinuxTimeImpl linuxTime;
extern Svc::LinuxTimerComponentImpl linuxTimer;
extern Svc::TlmChanImpl chanTlm;
extern Svc::CommandDispatcherImpl cmdDisp;
extern Svc::PrmDbImpl prmDb;
extern Svc::FileUplink fileUplink;
extern Svc::FileDownlink fileDownlink;
extern Svc::BufferManagerComponentImpl fileUplinkBufferManager;
extern Svc::AssertFatalAdapterComponentImpl fatalAdapter;
extern Svc::FatalHandlerComponentImpl fatalHandler;
extern Svc::HealthImpl health;

extern Svc::StaticMemoryComponentImpl staticMemory;
extern Drv::TcpClientComponentImpl comm;
extern Svc::FramerComponentImpl downlink;
extern Svc::DeframerComponentImpl uplink;

#endif //__ROBOT_BASIC_SYSTEM_HPP__
