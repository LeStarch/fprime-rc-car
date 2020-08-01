#include <Components.hpp>

#include <Fw/Types/Assert.hpp>
#include <Os/Task.hpp>
#include <Os/Log.hpp>
#include <Os/File.hpp>
#include <Fw/Types/MallocAllocator.hpp>
#include <Rover/Top/RateSchedContexts.hpp>

enum {
    DOWNLINK_PACKET_SIZE = 500,
    DOWNLINK_BUFFER_STORE_SIZE = 2500,
    DOWNLINK_BUFFER_QUEUE_SIZE = 5,
    UPLINK_BUFFER_STORE_SIZE = 3000,
    UPLINK_BUFFER_QUEUE_SIZE = 30
};

// Component instances

// Rate Group Dividers for 100Hz and 1Hz

static NATIVE_INT_TYPE rgDivs[] = {1,100,0};
Svc::RateGroupDriverImpl rateGroupDriverComp("RGDRV",rgDivs,FW_NUM_ARRAY_ELEMENTS(rgDivs));

// Context array variables are passed to rate group members if needed to distinguish one call from another
// These context must match the rate group members connected in TopologyAi.xml
static NATIVE_UINT_TYPE rg10HzContext[] = {Rover::CONTEXT_DEMO_10Hz,0,0,0,0,0,0,0,0,0};
Svc::ActiveRateGroupImpl rateGroup10HzComp("RG10Hz",rg10HzContext,FW_NUM_ARRAY_ELEMENTS(rg10HzContext));

static NATIVE_UINT_TYPE rg1HzContext[] = {0,0,Rover::CONTEXT_DEMO_1Hz,0,0,0,0,0,0,0};

Svc::ActiveRateGroupImpl rateGroup1HzComp("RG1Hz",rg1HzContext,FW_NUM_ARRAY_ELEMENTS(rg1HzContext));

// Command Components
Svc::GroundInterfaceComponentImpl groundIf("GNDIF");
Drv::SocketIpDriverComponentImpl socketIpDriver("SocketIpDriver");

#if FW_ENABLE_TEXT_LOGGING
Svc::ConsoleTextLoggerImpl textLogger("TLOG");
#endif

Svc::ActiveLoggerImpl eventLogger("ELOG");

Svc::LinuxTimeImpl linuxTime("LTIME");

Svc::LinuxTimerComponentImpl linuxTimer("LTIMER");

Svc::TlmChanImpl chanTlm("TLM");

Svc::CommandDispatcherImpl cmdDisp("CMDDISP");

// This needs to be statically allocated
Fw::MallocAllocator seqMallocator;

Svc::CmdSequencerComponentImpl cmdSeq("CMDSEQ");

Svc::PrmDbImpl prmDb("PRM","PrmDb.dat");

Svc::FileUplink fileUplink("fileUplink");

Svc::FileDownlink fileDownlink ("fileDownlink", DOWNLINK_PACKET_SIZE);

Svc::BufferManager fileDownlinkBufferManager("fileDownlinkBufferManager", DOWNLINK_BUFFER_STORE_SIZE, DOWNLINK_BUFFER_QUEUE_SIZE);

Svc::BufferManager fileUplinkBufferManager("fileUplinkBufferManager", UPLINK_BUFFER_STORE_SIZE, UPLINK_BUFFER_QUEUE_SIZE);

Svc::HealthImpl health("health");

Svc::AssertFatalAdapterComponentImpl fatalAdapter("fatalAdapter");

Svc::FatalHandlerComponentImpl fatalHandler("fatalHandler");


Drv::LinuxGpioDriverComponentImpl headlight("headlight");
Drv::LinuxGpioDriverComponentImpl left("left");
Drv::LinuxGpioDriverComponentImpl right("right");
Drv::LinuxGpioDriverComponentImpl forward("forward");
Drv::LinuxGpioDriverComponentImpl backward("backward");

Rover::ControlComponentImpl control("control");

void constructApp(U32 port_number, char* hostname) {

    // Initialize rate group driver
    rateGroupDriverComp.init();

    // Initialize the rate groups
    rateGroup10HzComp.init(10,0);
    rateGroup1HzComp.init(10,1);
    
#if FW_ENABLE_TEXT_LOGGING
    textLogger.init();
#endif

    eventLogger.init(10,0);

    linuxTime.init(0);

    linuxTimer.init(0);

    chanTlm.init(10,0);

    cmdDisp.init(20,0);

    cmdSeq.init(10,0);
    cmdSeq.allocateBuffer(0,seqMallocator,5*1024);

    prmDb.init(10,0);

    groundIf.init(0);
    socketIpDriver.init(0);

    fileUplink.init(30, 0);
    fileDownlink.init(30, 0);
    fileUplinkBufferManager.init(0);
    fileDownlinkBufferManager.init(1);

    fatalAdapter.init(0);
    fatalHandler.init(0);
    health.init(25,0);

    headlight.init(0);
    left.init(0);
    right.init(0);
    forward.init(0);
    backward.init(0);

    control.init(10,0);

    constructRoverArchitecture();

    /* Register commands */
    cmdSeq.regCommands();
    cmdDisp.regCommands();
    eventLogger.regCommands();
    prmDb.regCommands();
    fileDownlink.regCommands();
    health.regCommands();
    control.regCommands();

    // set sequencer timeout
    cmdSeq.setTimeout(30);

    // read parameters
    prmDb.readParamFile();

    // set health ping entries

    // This list has to match the connections in TopologyAppAi.xml

    Svc::HealthImpl::PingEntry pingEntries[] = {
        {3,5,rateGroup10HzComp.getObjName()}, // 0
        {3,5,rateGroup1HzComp.getObjName()}, // 1
        {3,5,cmdDisp.getObjName()}, // 2
        {3,5,cmdSeq.getObjName()}, // 3
        {3,5,chanTlm.getObjName()}, // 4
        {3,5,eventLogger.getObjName()}, // 5
        {3,5,prmDb.getObjName()}, // 6
        {3,5,fileDownlink.getObjName()}, // 7
        {3,5,fileUplink.getObjName()}, // 8
    };

    // register ping table
    health.setPingEntries(pingEntries,FW_NUM_ARRAY_ELEMENTS(pingEntries),0x123);

    // Active component startup
    // start rate groups
    rateGroup10HzComp.start(0, 120,10 * 1024);
    rateGroup1HzComp.start(0, 119,10 * 1024);
    // start dispatcher
    cmdDisp.start(0,101,10*1024);
    // start sequencer
    cmdSeq.start(0,100,10*1024);
    // start telemetry
    eventLogger.start(0,98,10*1024);
    chanTlm.start(0,97,10*1024);
    prmDb.start(0,96,10*1024);

    fileDownlink.start(0, 100, 10*1024);
    fileUplink.start(0, 100, 10*1024);
    control.start(0, 100, 10*1024);

    if (not headlight.open(26,Drv::LinuxGpioDriverComponentImpl::GPIO_OUT)) {
        return;
    }

    if (not backward.open(2,Drv::LinuxGpioDriverComponentImpl::GPIO_OUT)) {
        return;
    }

    if (not forward.open(3,Drv::LinuxGpioDriverComponentImpl::GPIO_OUT)) {
        return;
    }

    if (not left.open(4,Drv::LinuxGpioDriverComponentImpl::GPIO_OUT)) {
        return;
    }

    if (not right.open(17,Drv::LinuxGpioDriverComponentImpl::GPIO_OUT)) {
        return;
    }

    // Initialize socket server
    if (hostname != NULL && port_number != 0) {
        socketIpDriver.startSocketTask(100, 10 * 1024, hostname, port_number);
    }
}

void exitTasks(void) {
    linuxTimer.quit();
    rateGroup1HzComp.exit();
    rateGroup10HzComp.exit();
    cmdDisp.exit();
    eventLogger.exit();
    chanTlm.exit();
    prmDb.exit();
    fileUplink.exit();
    fileDownlink.exit();
    cmdSeq.exit();
    control.exit();
}

