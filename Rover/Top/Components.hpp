#ifndef __ROVER_COMPONENTS_HEADER__
#define __ROVER_COMPONENTS_HEADER__

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
#include <Svc/BufferManager/BufferManager.hpp>
#include <Svc/Health/HealthComponentImpl.hpp>

#include <Drv/SocketIpDriver/SocketIpDriverComponentImpl.hpp>
#include <Svc/GroundInterface/GroundInterface.hpp>

#include <Svc/AssertFatalAdapter/AssertFatalAdapterComponentImpl.hpp>
#include <Svc/FatalHandler/FatalHandlerComponentImpl.hpp>

// Drivers

#include <Drv/LinuxSerialDriver/LinuxSerialDriverComponentImpl.hpp>
#include <Drv/LinuxSpiDriver/LinuxSpiDriverComponentImpl.hpp>
#include <Drv/LinuxGpioDriver/LinuxGpioDriverComponentImpl.hpp>

// Main app
#include <Rover/Control/ControlComponentImpl.hpp>

void constructRoverArchitecture(void);
void exitTasks(void);
void constructApp(U32 port_number, char* hostname);

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
extern Svc::BufferManager fileDownlinkBufferManager;
extern Svc::BufferManager fileUplinkBufferManager;
extern Svc::AssertFatalAdapterComponentImpl fatalAdapter;
extern Svc::FatalHandlerComponentImpl fatalHandler;
extern Svc::HealthImpl health;

extern Drv::LinuxSerialDriverComponentImpl uartDrv;
extern Drv::LinuxSpiDriverComponentImpl spiDrv;
extern Drv::LinuxGpioDriverComponentImpl headlight;
extern Drv::LinuxGpioDriverComponentImpl left;
extern Drv::LinuxGpioDriverComponentImpl right;
extern Drv::LinuxGpioDriverComponentImpl forward;
extern Drv::LinuxGpioDriverComponentImpl backward;

extern Rover::ControlComponentImpl control;

extern Drv::SocketIpDriverComponentImpl socketIpDriver;
extern Svc::GroundInterfaceComponentImpl groundIf;

#endif
