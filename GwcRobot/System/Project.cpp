#include <Project.hpp>
#include <BaiscSystem.hpp>

// Hardware drivers
Drv::LinuxI2cDriverComponentImpl i2c(FW_OPTIONAL_NAME("i2c"));
GwcRobot::Pca9685ComponentImpl pca9685(FW_OPTIONAL_NAME("pca9685"));

// Two stepper motors
GwcRobot::StepperMotorComponentImpl stepper0(FW_OPTIONAL_NAME("stepper0"));
GwcRobot::StepperMotorComponentImpl stepper1(FW_OPTIONAL_NAME("stepper1"));

GwcRobot::TwoMotorControlComponentImpl motion(FW_OPTIONAL_NAME("motion"));

void init_project() {
    i2c.init();
    pca9685.init();
    stepper0.init(STEPPER_0_QUEUE_DEPTH);
    stepper1.init(STEPPER_1_QUEUE_DEPTH);
    motion.init(MOTION_QUEUE_DEPTH);
}

void register_project() {
    stepper0.regCommands();
    stepper1.regCommands();
    motion.regCommands();
}


void start_project() {
    // Try to open i2c and fail if it is not available as it is critical
    if (not i2c.open("/dev/i2c-1")) {
        printf("Failed to open i2c device");
        FW_ASSERT(0);
    }
    pca9685.startup(0x60);
    stepper0.startup();
    stepper1.startup();

    stepper0.start(0, STEPPER_0_PRIORITY, THREAD_STACK_SIZE);
    stepper1.start(0, STEPPER_1_PRIORITY, THREAD_STACK_SIZE);
    motion.start(0, MOTION_PRIORITY, THREAD_STACK_SIZE);
}

void stop_project() {
    stepper0.exit();
    stepper1.exit();
    motion.exit();
    stepper0.ActiveComponentBase::join(NULL);
    stepper1.ActiveComponentBase::join(NULL);
    motion.ActiveComponentBase::join(NULL);
}