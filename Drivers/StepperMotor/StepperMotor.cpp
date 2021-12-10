// ======================================================================
// \title  StepperMotorComponentImpl.cpp
// \author starchmd
// \brief  cpp file for StepperMotor component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <Drivers/StepperMotor/StepperMotor.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace GwcRobot {

const U16 StepperMotorComponentImpl ::MICRO_TABLE[21] = { 0, 321, 641, 956, 1265, 1567, 1859, 2140, 2407, 2659, 2896,
                                                          3114, 3313, 3492, 3649, 3783, 3895, 3982, 4045, 4082, 4095 };

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

StepperMotorComponentImpl ::StepperMotorComponentImpl(const char* const compName)
    : StepperMotorComponentBase(compName), m_current(0) {}

void StepperMotorComponentImpl ::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance) {
    StepperMotorComponentBase::init(queueDepth, instance);
}

StepperMotorComponentImpl ::~StepperMotorComponentImpl(void) {}

void StepperMotorComponentImpl ::startup() {
    // Turn on both power channels
    power_out(0, 0xFFF);
    power_out(1, 0xFFF);

    // Shutoff all stepper coils
    for (U32 i = 0; i < COILS; i++) {
        pwm_out(i, 0);
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void StepperMotorComponentImpl ::spin_handler(const NATIVE_INT_TYPE portNum,
                                              Direction direction,
                                              U8 speed,
                                              F32 revolutions) {
    spin(direction, speed, revolutions);
}

// ----------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------

void StepperMotorComponentImpl ::spin(const Direction& direction, U8 speed, F32 revoluions) {
    speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
    // Break-out when speed is zero, as nothing will be done
    if (speed == 0) {
        return;
    }
    // Calculate micros steps per "step" given our speed
    U32 micros = ((speed/2 - 1) * MICRO_STEPS) / MAX_SPEED + 1;
    // Number of steps to get the revolutions we need
    U32 steps = static_cast<U32>(revoluions * STEPS_PER_ROTATION);

    // Step the given number of times
    for (U32 i = 0; i < steps; i++) {
        step(direction, micros);
    }
}

void StepperMotorComponentImpl ::step(const Direction& direction, U32 micros) {
    FW_ASSERT(micros < PSEUDO_STEPS, micros);

    const U32 delta = (direction == Direction::FORWARD) ? (micros) : (PSEUDO_STEPS - micros);
    m_current = (m_current + delta) % PSEUDO_STEPS;

    // Duty cycles are reset to zero
    U16 duty[COILS] = {0, 0, 0, 0};

    const NATIVE_INT_TYPE tail_coil = (m_current / MICRO_STEPS) % COILS;
    const NATIVE_INT_TYPE head_coil = (tail_coil + 1) % COILS;
    const NATIVE_INT_TYPE table_index = m_current % MICRO_STEPS;

    duty[tail_coil] = MICRO_TABLE[MICRO_STEPS - table_index];
    duty[head_coil] = MICRO_TABLE[table_index];

    // Output the duty cycles
    for (U32 i = 0; i < COILS; i++) {
        pwm_out(i, duty[i]);
    }
    Os::Task::delay(10); // Delay between steps
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void StepperMotorComponentImpl ::SPIN_cmdHandler(const FwOpcodeType opCode,
                                                 const U32 cmdSeq,
                                                 Direction direction,
                                                 U8 speed,
                                                 F32 revolutions) {
    spin(direction, speed, revolutions);
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

void StepperMotorComponentImpl ::STEP_cmdHandler(const FwOpcodeType opCode,
                                                 const U32 cmdSeq,
                                                 Direction direction,
                                                 U32 count) {
    // Loop over number of (full) steps using
    for (U32 i = 0; i < count; i++) {
        step(direction, MICRO_STEPS);
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

}  // end namespace GwcRobot
