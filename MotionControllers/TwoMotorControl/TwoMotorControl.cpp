// ======================================================================
// \title  TwoMotorControlComponentImpl.cpp
// \author starchmd
// \brief  cpp file for TwoMotorControl component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <MotionControllers/TwoMotorControl/TwoMotorControl.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace GwcRobot {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

TwoMotorControlComponentImpl ::TwoMotorControlComponentImpl(const char* const compName)
    : TwoMotorControlComponentBase(compName) {}

void TwoMotorControlComponentImpl ::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance) {
    TwoMotorControlComponentBase::init(queueDepth, instance);
}

TwoMotorControlComponentImpl ::~TwoMotorControlComponentImpl(void) {}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void TwoMotorControlComponentImpl ::GO_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    motion_helper();
    this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
}

}  // end namespace GwcRobot
