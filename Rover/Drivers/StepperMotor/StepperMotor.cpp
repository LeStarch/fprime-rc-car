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


#include <Rover/Drivers/StepperMotor/StepperMotor.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Rover {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  StepperMotorComponentImpl ::
    StepperMotorComponentImpl(
        const char *const compName
    ) : StepperMotorComponentBase(compName)
  {

  }

  void StepperMotorComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    StepperMotorComponentBase::init(instance);
  }

  StepperMotorComponentImpl ::
    ~StepperMotorComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Status StepperMotorComponentImpl ::
    spin_handler(
        const NATIVE_INT_TYPE portNum,
        Direction direction,
        U8 speed,
        F32 revolutions
    )
  {
    // TODO return
  }

  Status StepperMotorComponentImpl ::
    stop_handler(
        const NATIVE_INT_TYPE portNum
    )
  {
    // TODO return
  }

  void StepperMotorComponentImpl ::
    run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

} // end namespace Rover
