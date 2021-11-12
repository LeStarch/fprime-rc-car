// ======================================================================
// \title  StepperMotorComponentImpl.hpp
// \author starchmd
// \brief  hpp file for StepperMotor component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef StepperMotor_HPP
#define StepperMotor_HPP

#include "Drivers/StepperMotor/StepperMotorComponentAc.hpp"

namespace GwcRobot {

class StepperMotorComponentImpl : public StepperMotorComponentBase {
  public:
    static const U32 MAX_SPEED = 100;
    static const U32 STEPS_PER_ROTATION = 100;
    static const U16 MICRO_TABLE[21];
    static const U32 COILS = 4;
    static const U32 MICRO_STEPS = FW_NUM_ARRAY_ELEMENTS(MICRO_TABLE) - 1;
    static const U32 PSEUDO_STEPS = MICRO_STEPS * COILS;


    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object StepperMotor
    //!
    StepperMotorComponentImpl(const char* const compName /*!< The component name*/
    );

    //! Initialize object StepperMotor
    //!
    void init(const NATIVE_INT_TYPE queueDepth,  /*!< The queue depth*/
              const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy object StepperMotor
    //!
    ~StepperMotorComponentImpl();

    //! Startup the stepper motor, call after the I2C device is online
    //!
    void startup();

    //! Deenergize the stepper coils
    //!
    void deenergize();

  private:

    //! Spin helper class
    //!
    void spin(const Direction& direction, U8 speed, F32 revoluions);

    //! Take one (macro) step
    //!
    void step(const Direction& direction, U32 micro_steps);

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for spin
    //!
    void spin_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                      Direction direction,           /*!< Direction of the spin*/
                      U8 speed,                      /*!< Speed of the spin*/
                      F32 revolutions                /*!< Count of the revolutions to spin*/
    );

  private:
    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for SPIN command handler
    //!
    void SPIN_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                         const U32 cmdSeq,          /*!< The command sequence number*/
                         Direction direction,
                         U8 speed,
                         F32 revolutions);

    //! Implementation for STEP command handler
    //!
    void STEP_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                         const U32 cmdSeq,          /*!< The command sequence number*/
                         Direction direction,
                         U32 count);

    U32 m_current;
};

}  // end namespace GwcRobot

#endif
