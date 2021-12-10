#include <MotionControllers/TwoMotorControl/TwoMotorControl.hpp>
#include <Drivers/MotorPorts/DirectionEnumAc.hpp>


namespace GwcRobot {

/**
 * First assignment:
 *
 * Determine if the following enumeration is correct for your robot.  We have two motors that are attached as "0" and
 * "1".  We use this enumeration to "name" them in the code such that we can write code that uses labels instead of
 * confusing numbers.  However, your mentor just guessed which motor was which number.  Try running the code and
 * determining if the numbers are correct.
 */
enum Motors {
    LEFT_MOTOR = 0,
    RIGHT_MOTOR = 1,
};


/**
 * Second assignment:
 *
 * Work with the code below, switch who is "driving" the keyboard, to determine how to implement the following actions:
 *
 * 1. Drive forward, drive backward
 * 2. Pivot 90 degrees left and right
 * 3. Spin in-place 90 degrees, 180 degrees
 * 4. Do something cool with your robotic navigation
 */
void TwoMotorControlComponentImpl:: motion_helper() {
    Direction direction = Direction::FORWARD;
    U8 speed = 100; // Speed is defined as 0% to 100% with fine-control in 1% steps
    F32 revolutions = 1.0f; // Number of revolutions of the motor before the command is done

    // Drives the motors
    motor_out(LEFT_MOTOR, direction, speed, revolutions);
    Os::Task::delay(1000); // Pause for 1 second
    motor_out(RIGHT_MOTOR, direction, speed, revolutions);
}

void TwoMotorControlComponentImpl ::FORWARD_cmdHandler(const FwOpcodeType opCode,
                                                       const U32 cmdSeq,
                                                       U8 speed,
                                                       F32 revolutions) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

void TwoMotorControlComponentImpl ::BACKWARD_cmdHandler(const FwOpcodeType opCode,
                                                        const U32 cmdSeq,
                                                        U8 speed,
                                                        F32 revolutions) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

void TwoMotorControlComponentImpl ::LEFT_cmdHandler(const FwOpcodeType opCode,
                                                    const U32 cmdSeq,
                                                    U8 speed,
                                                    F32 degrees) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

void TwoMotorControlComponentImpl ::RIGHT_cmdHandler(const FwOpcodeType opCode,
                                                     const U32 cmdSeq,
                                                     U8 speed,
                                                     F32 degrees) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

};  // namespace GwcRobot
