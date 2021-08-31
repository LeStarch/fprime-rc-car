/**
 * BasicSystem.cpp:
 *
 * This file pairs with "BasicSystem.hpp" and contains the code needed to setup the spacecraft bus software.  The
 * definitions are in "BasicSystem.hpp" and can be reviewed for further explanation.
 */
#include "BaiscSystem.hpp"
#include <Svc/FramingProtocol/FprimeProtocol.hpp>
#include <Fw/Types/MallocAllocator.hpp>
#include <Fw/Types/StringType.hpp>

enum {
    UPLINK_BUFFER_STORE_SIZE = 3000,
    UPLINK_BUFFER_QUEUE_SIZE = 30,
    UPLINK_BUFFER_MGR_ID = 200
};

// Choose the ground-to-spacecraft wire format. Here we use the fprime format as it will work well with the fprime
// ground system needed to control the robot.
Svc::FprimeDeframing deframing;
Svc::FprimeFraming framing;

// Memory management component
Fw::MallocAllocator allocator;

// Communication component construction
Drv::TcpClientComponentImpl comm(FW_OPTIONAL_NAME("comm[tcp]"));
Svc::StaticMemoryComponentImpl staticMemory(FW_OPTIONAL_NAME("staticMemory"));
Svc::FramerComponentImpl downlink(FW_OPTIONAL_NAME("downlink"));
Svc::DeframerComponentImpl uplink(FW_OPTIONAL_NAME("uplink"));

// Clock component construction
Svc::RateGroupDriverImpl rateGroupDriverComp(FW_OPTIONAL_NAME("rateGroupDriverComp"), RATE_GROUP_DIVISORS, FW_NUM_ARRAY_ELEMENTS(RATE_GROUP_DIVISORS));

Svc::ActiveRateGroupImpl rateGroup10HzComp(FW_OPTIONAL_NAME("rateGroup10HzComp"), RG_10HZ_CONTEXTS, FW_NUM_ARRAY_ELEMENTS(RG_10HZ_CONTEXTS));
Svc::ActiveRateGroupImpl rateGroup1HzComp(FW_OPTIONAL_NAME("rateGroup1HzComp"), RG_1HZ_CONTEXTS, FW_NUM_ARRAY_ELEMENTS(RG_1HZ_CONTEXTS));

Svc::LinuxTimeImpl linuxTime(FW_OPTIONAL_NAME("linuxTime"));
Svc::LinuxTimerComponentImpl linuxTimer(FW_OPTIONAL_NAME("linuxTimer"));

// Command and Data Handling components
Svc::PrmDbImpl prmDb(FW_OPTIONAL_NAME("prmDb"),"PrmDb.dat");

Svc::ConsoleTextLoggerImpl textLogger(FW_OPTIONAL_NAME("textLogger"));
Svc::ActiveLoggerImpl eventLogger(FW_OPTIONAL_NAME("eventLogger"));
Svc::TlmChanImpl chanTlm(FW_OPTIONAL_NAME("chanTlm"));

Svc::CommandDispatcherImpl cmdDisp(FW_OPTIONAL_NAME("cmdDisp"));
Svc::CmdSequencerComponentImpl cmdSeq(FW_OPTIONAL_NAME("cmdSeq"));

// File upload and download component construction
Svc::FileUplink fileUplink(FW_OPTIONAL_NAME("fileUplink"));
Svc::FileDownlink fileDownlink(FW_OPTIONAL_NAME("fileDownlink"));
Svc::BufferManagerComponentImpl fileUplinkBufferManager(FW_OPTIONAL_NAME("fileUplinkBufferManager"));


// Fault, error, and health handling component construction
Svc::HealthImpl health(FW_OPTIONAL_NAME("health"));
Svc::AssertFatalAdapterComponentImpl fatalAdapter(FW_OPTIONAL_NAME("fatalAdapter"));
Svc::FatalHandlerComponentImpl fatalHandler(FW_OPTIONAL_NAME("fatalHandler"));


void init_basic_system() {
    // Communication component instantiations
    staticMemory.init();
    comm.init();
    downlink.init();
    uplink.init();
    downlink.setup(framing); // Setup the uplinker and downlinker components
    uplink.setup(deframing);

    // Initialize time components
    rateGroupDriverComp.init();
    rateGroup10HzComp.init(RATE_10HZ_QUEUE_DEPTH, 0); // Two instances of rate groups, each needs a unique instance number
    rateGroup1HzComp.init(RATE_1HZ_QUEUE_DEPTH, 1);
    linuxTime.init(0);
    linuxTimer.init();

    // C and DH component initialization calls
    prmDb.init(PARAM_QUEUE_DEPTH,0);
    textLogger.init();
    eventLogger.init(EVENT_QUEUE_DEPTH, 0);
    chanTlm.init(CHAN_QUEUE_DEPTH,0);

    cmdDisp.init(CMD_DISP_QUEUE_DEPTH,0);
    cmdSeq.init(CMD_SEQ_QUEUE_DEPTH,0);
    cmdSeq.allocateBuffer(0, allocator, CMD_SEQ_MEMORY_ALLOCATION);
    cmdSeq.setTimeout(30);

    // File handling component initialization
    fileDownlink.configure(1000, 200, 100, 10);
    fileDownlink.init(FILE_DWN_QUEUE_DEPTH,0);
    fileUplink.init(FILE_UP_QUEUE_DEPTH,0);
    fileUplinkBufferManager.init();

    // Health, errors, and fatals initialization
    fatalAdapter.init();
    fatalHandler.init();
    health.init(HEALTH_QUEUE_DEPTH,0);
}

void register_basic_system() {
    prmDb.regCommands();
    prmDb.readParamFile();

    eventLogger.regCommands();
    cmdSeq.regCommands();
    cmdDisp.regCommands();

    fileDownlink.regCommands();

    health.regCommands();

    Svc::BufferManagerComponentImpl::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins,0,sizeof(upBuffMgrBins));
    upBuffMgrBins.bins[0].bufferSize = UPLINK_BUFFER_STORE_SIZE;
    upBuffMgrBins.bins[0].numBuffers = UPLINK_BUFFER_QUEUE_SIZE;
    fileUplinkBufferManager.setup(UPLINK_BUFFER_MGR_ID,0,allocator,upBuffMgrBins);
}

void start_basic_system() {
    rateGroup10HzComp.start(0, RATE_10HZ_PRIORITY, THREAD_STACK_SIZE);
    rateGroup1HzComp.start(0, RATE_1HZ_PRIORITY, THREAD_STACK_SIZE);

    prmDb.start(0, PARAM_PRIORITY, THREAD_STACK_SIZE);
    eventLogger.start(0, EVENT_PRIORITY, THREAD_STACK_SIZE);
    chanTlm.start(0, CHAN_PRIORITY, THREAD_STACK_SIZE);
    cmdDisp.start(0, CMD_DISP_PRIORITY, THREAD_STACK_SIZE);
    cmdSeq.start(0, CMD_SEQ_PRIORITY, THREAD_STACK_SIZE);

    fileDownlink.start(0, FILE_DOWN_PRIORITY, THREAD_STACK_SIZE);
    fileUplink.start(0, FILE_UP_PRIORITY, THREAD_STACK_SIZE);
}

void start_communications(char* hostname, U16 port_number) {
    // Initialize socket server if and only if there is a valid specification
    if (hostname != NULL && port_number != 0) {
        Fw::String name("ReceiveTask");
        // Uplink is configured for receive so a socket task is started
        comm.configure(hostname, port_number);
        comm.startSocketTask(name, COMM_PRIORITY, THREAD_STACK_SIZE);
    }
}

void stop_basic_system() {
    linuxTimer.quit();
    rateGroup1HzComp.exit();
    rateGroup10HzComp.exit();

    prmDb.exit();
    cmdDisp.exit();
    eventLogger.exit();
    chanTlm.exit();
    cmdSeq.exit();

    fileUplink.exit();
    fileDownlink.exit();

    // Socket task handling
    comm.stopSocketTask();
    (void) comm.joinSocketTask(NULL);

    rateGroup1HzComp.ActiveComponentBase::join(NULL);
    rateGroup10HzComp.ActiveComponentBase::join(NULL);

    prmDb.ActiveComponentBase::join(NULL);
    cmdDisp.ActiveComponentBase::join(NULL);
    eventLogger.ActiveComponentBase::join(NULL);
    chanTlm.ActiveComponentBase::join(NULL);
    cmdSeq.ActiveComponentBase::join(NULL);

    fileUplink.ActiveComponentBase::join(NULL);
    fileDownlink.ActiveComponentBase::join(NULL);

    // Cleanup
    cmdSeq.deallocateBuffer(allocator);
    fileUplinkBufferManager.cleanup();
}