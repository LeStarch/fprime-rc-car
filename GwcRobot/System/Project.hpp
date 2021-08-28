/**
 * Project.hpp:
 *
 * Author: LeStarch
 */
#ifndef __ROBOT_PROJECT_SYSTEM_HPP__
#define __ROBOT_PROJECT_SYSTEM_HPP__
#include <Drv/LinuxI2cDriver/LinuxI2cDriverComponentImpl.hpp>
#include <Drivers/PCA9685/Pca9685.hpp>
#include <Drivers/StepperMotor/StepperMotor.hpp>

enum ProjectsSettings {
    STEPPER_0_QUEUE_DEPTH = 100,
    STEPPER_1_QUEUE_DEPTH = 100,

    // Thread priorities, 255 being the highest, and 0 being lowest
    STEPPER_0_PRIORITY = 222,
    STEPPER_1_PRIORITY = 222,
};

// Hardware drivers
extern Drv::LinuxI2cDriverComponentImpl i2c;
extern GwcRobot::Pca9685ComponentImpl pca9685;

// Two stepper motors
extern GwcRobot::StepperMotorComponentImpl stepper0;
extern GwcRobot::StepperMotorComponentImpl stepper1;


/**
 * Initializes the project components. This is a required step to get that system running.
 */
void init_project();

/**
 * Registers commands defined in the project.
 */
void register_project();

/**
 * Start the active components (threads) in the project.
 */
void start_project();

/**
 * Stop the active components (threads) in the project.
 */
void stop_project();
#endif //__ROBOT_PROJECT_SYSTEM_HPP__
